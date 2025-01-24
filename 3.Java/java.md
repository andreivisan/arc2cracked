# Most important features

## Pattern Matching

Declarative type-driven logic, unifying type checks, casting, and data extraction in one step.

### Key Features

**1. instanceof + Auto-Casting**

- Replace:

```java
if (obj instanceof String) {
    String s = (String) obj; // Manual cast
}
```

With

```java
if (obj instanceof String s) { // Auto-cast to 's'
    // Use 's' directly
}
```

**2. Pattern Matching in switch**

- Match types and extract data in case labels:

```java
switch (shape) {
    case Circle c -> processCircle(c);
    case Rectangle r -> processRect(r);
}
```

**3. Deconstruct Records**

- Break records into components:

```java
if (person instanceof Person(var name, var age)) {
    // Directly use 'name' and 'age'
}
```

### Details & Nuances

- Guarded Patterns: Add conditions with when:

```java
case String s when s.length() > 5 -> ...
```

- Primitive Support: Works with primitives (e.g., case int i).

- Null Handling: case null can be explicitly matched.

### Synergy with Generics

```java
// Generics enforce type safety upfront
List<Shape> shapes = new ArrayList<>();
shapes.add(new Circle());
shapes.add(new Rectangle());

// Pattern matching simplifies processing
for (Shape s : shapes) {
    switch (s) {
        case Circle c -> System.out.println("Radius: " + c.radius());
        case Rectangle r -> System.out.println("Area: " + r.width() * r.height());
    }
}
```

## Virtual Threads

### Core principle

**Problem:** Traditional Java threads (OS-managed "platform threads") are heavyweight (1:1 with OS threads). Creating 10,000s of them causes memory exhaustion and context-switching overhead.

**Solution:** Virtual threads are lightweight, user-mode threads managed by the JVM. They enable massive concurrency (millions of threads) for I/O-bound tasks (e.g., web servers, databases).

**Key Metaphor:** Think of virtual threads as "green threads" that ride atop a small pool of carrier threads (OS threads).

### Key Features

- Virtual threads cost ~200 bytes each (vs. ~1MB for platform threads).

- Example: Serve 1 million concurrent requests without crashing:

```java
try (var executor = Executors.newVirtualThreadPerTaskExecutor()) {
    for (int i = 0; i < 1_000_000; i++) {
        executor.submit(() -> handleRequest()); // 1M virtual threads
    }
}
```

- Backward Compatibility: Use existing java.lang.Thread APIs. No rewrites needed!

```java
Thread.startVirtualThread(() -> System.out.println("Hello from VT!"));
```

- Structured Concurrency (Java 21) : Treat groups of threads as a single unit (prevents leaks/cancellation bugs):

```java
try (var scope = new StructuredTaskScope.ShutdownOnFailure()) {
    Future<String> user = scope.fork(() -> fetchUser());
    Future<Integer> order = scope.fork(() -> fetchOrder());
    scope.join(); // Both subtasks finish or fail together
} // Auto-close
```

- Non-Blocking I/O Integration: Blocking operations (e.g., Files.readString(), network calls) yield the thread automatically, freeing the carrier thread for other work.

### Nuances & Code Patterns

1. Creating Virtual Threads

```java
// Pre-Java 21 (Preview APIs):
Thread.ofVirtual().start(() -> {...});
// Java 21+:
Executors.newVirtualThreadPerTaskExecutor();
```

2. Pinning (When Virtual Threads Block)

Synchronized blocks pin the VT to a carrier thread (hurts scalability). Use ReentrantLock instead:

```java
private final ReentrantLock lock = new ReentrantLock();
void pinnedExample() {
    synchronized (this) { // ❌ Pins the VT!
        // ...
    }
}
void betterExample() {
    lock.lock(); // ✅ Doesn’t pin!
    try { /* ... */ } finally { lock.unlock(); }
}
```

3. Debugging

Use jcmd <pid> Thread.dump_to_file -format=json <file> for JSON thread dumps (supports 100k+ threads).

4.  Carrier Threads

By default, the number of carrier threads = number of CPU cores.

```
Virtual Threads (Millions)  
  ~~~~~~~~~~~~~~~~~~~~~~~  
  |      |      |      |  
  ▼      ▼      ▼      ▼  
Carrier Threads (OS Threads)  
  [ ][ ][ ][ ] (e.g., 4 cores)
```

### Common Usage & Examples

1. Web Server (Before vs. After Loom)

- Old

```java
ExecutorService pool = Executors.newFixedThreadPool(200); // Max 200 concurrent requests  
pool.submit(() -> handleRequest()); // Blocks under load  
```

- New (Virtual Threads):

```java
ExecutorService vtExecutor = Executors.newVirtualThreadPerTaskExecutor();  
vtExecutor.submit(() -> handleRequest()); // Scales to 1M+ requests  
```

2. Parallel Processing

```java
List<Future<Result>> futures = new ArrayList<>();
try (var executor = Executors.newVirtualThreadPerTaskExecutor()) {
    for (Task task : tasks) {
        futures.add(executor.submit(task::process));
    }
} // Auto-close after all tasks finish
```

Virtual threads don’t replace platform threads—they’re designed for high-throughput, blocking workloads. Use platform threads for CPU-bound tasks (e.g., video encoding).

### Examples

1. Partitioned DB Query + Virtual Threads

Use structured concurrency (Java 21) to enforce task boundaries and avoid thread leaks.

```java
import java.util.concurrent.*;
import jdk.incubator.concurrent.StructuredTaskScope;

// Assume a database client that fetches addresses by postal code partition
class AddressService {
    List<Address> fetchFromPartition(String partitionKey) { /* ... */ }
    
    List<Address> fetchAllAddresses() throws Exception {
        try (var scope = new StructuredTaskScope.ShutdownOnFailure()) {
            // Fork subtasks for each partition
            Future<List<Address>> partition1 = scope.fork(() -> fetchFromPartition("PART_1"));
            Future<List<Address>> partition2 = scope.fork(() -> fetchFromPartition("PART_2"));
            
            scope.join(); // Wait for both to finish
            scope.throwIfFailed(); // Propagate errors
            
            // Merge results (simple list concatenation)
            List<Address> merged = new ArrayList<>();
            merged.addAll(partition1.resultNow());
            merged.addAll(partition2.resultNow());
            return merged;
        }
    }
}
```

2. Non-Blocking I/O Integration Examples

**Core Idea:** Virtual threads automatically yield during blocking I/O, freeing the carrier thread.

**Example 1: File I/O** with Files.readString()

```java
try (var executor = Executors.newVirtualThreadPerTaskExecutor()) {
    executor.submit(() -> {
        String content = Files.readString(Path.of("large-file.txt")); // Blocks, but yields VT
        process(content);
    });
}
```

**Example 2: Network Call (HTTP Client)**

```java
import java.net.http.*;
import java.net.URI;

HttpClient client = HttpClient.newHttpClient();

try (var executor = Executors.newVirtualThreadPerTaskExecutor()) {
    executor.submit(() -> {
        HttpRequest request = HttpRequest.newBuilder()
            .uri(URI.create("https://api.example.com/data"))
            .build();
        HttpResponse<String> response = client.send(request, BodyHandlers.ofString()); // Blocks, yields VT
        process(response.body());
    });
}
```

**Visualization:**

```
Virtual Thread 1 ──▶ [Block on File Read] ~~yield~~▶ Carrier Thread 1  
Virtual Thread 2 ──▶ [Block on HTTP Call] ~~yield~~▶ Carrier Thread 2  
```



