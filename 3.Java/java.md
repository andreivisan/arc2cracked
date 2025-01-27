# Most important features

## Java 7-11 Highlights

### 1.1. Java 7 Highlights

1. **Diamond Operator (<>)**
    - Eliminates the need to repeat generic type parameters on the right-hand side.

2. **String in switch**
    - Allows using String values in switch statements.

3. **Try-With-Resources**
    - Ensures automatic resource management (e.g., closing files/streams).
    - **Functional Interfaces** - Interfaces with one abstract method (e.g., Predicate, Consumer, Supplier, Function).

### 1.2 Java 8 Highlights

1. **Lambdas & Functional Interfaces**
    - Introduced functional syntax ((args) -> expression) and built-in functional interfaces (Predicate, Consumer, etc.).

**Common Functional Interfaces**

| Interface      | Abstract Method | Purpose                     | Example                  |
|----------------|-----------------|-----------------------------|--------------------------|
|Predicate<T>    | test(T t)	   |Test input, return boolean	 |s -> s.isEmpty()          |
|Consumer<T>     | accept(T t)	   |Consume input, no return	 |s -> System.out.println(s)|
|Supplier<T>     | get()	       |Supply a value, no input	 |() -> Math.random()       |
|Function<T,R>   | apply(T t)	   |Transform input to output	 |s -> s.length()           |
|UnaryOperator<T>| apply(T t)	   |Function where input = output|s -> s.toUpperCase()      |

**Supplier example**

```java
Supplier<Double> randomSupplier = Math::random;
System.out.println(randomSupplier.get()); // e.g., 0.548
```

- **Composing Functions**

    - Predicate

    ```java
    Predicate<String> startsWithA = s -> s.startsWith("A");
    Predicate<String> endsWithE = s -> s.endsWith("e");
    Predicate<String> startsWithAAndEndsWithE = startsWithA.and(endsWithE);
    ```

    - Function

    ```java
    Function<Integer, Integer> timesTwo = x -> x * 2;
    Function<Integer, Integer> squared = x -> x * x;
    Function<Integer, Integer> composed = timesTwo.andThen(squared);
    int result = composed.apply(3); // (3*2)^2 = 36
    ```

- **Custom Functional Interface**

```java
@FunctionalInterface
interface StringJoiner {
    String join(String s1, String s2);
}

// Usage
StringJoiner joiner = (s1, s2) -> s1 + "-" + s2;
System.out.println(joiner.join("Hello", "World")); // "Hello-World"
```

2. **Streams API**



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

### How "Yielding" Works

**1. Blocking Operation Detected** 

When a virtual thread (VT) calls a blocking I/O method (e.g., Files.readString(), HttpClient.send(), JDBC query):

- The JVM intercepts the call (via "continuations").

- The VT is unmounted from its carrier thread (OS thread).

- The carrier thread is freed to execute other VTs.

**2. 2. I/O Completion**

- The OS notifies the JVM when the I/O operation completes.

- The VT is rescheduled onto an available carrier thread to resume execution.

```
Virtual Thread 1 ────▶ [Block on File I/O] ~~~yield~~~▶ Carrier Thread 1  
                        (VT parked, state saved)  
                          │  
                          ▼  
[I/O completes] ──────────┘  
Carrier Thread 2 ◀~~~resume~~~▶ Virtual Thread 1 continues  
```

## Module System

### Core Principles

- **Problem:** Monolithic JARs, fragile classpaths, and no true encapsulation (e.g., sun.misc.Unsafe leaks).

Enforce strong encapsulation and define explicit dependencies between components.

### Key Features

**1. Module Descriptor** (module-info.java)

- Declares dependencies (requires) and exposed APIs (exports).

```java
module com.example.myapp {
    requires java.sql;        // Dependency
    exports com.example.api;  // Public API
}
```

**2. Strong Encapsulation**

- Non-exported packages are inaccessible to other modules (unlike classic public classes).

**3. Services**

- provides and uses for loose coupling via service interfaces (e.g., JDBC drivers).

### Details & Nuances

- jlink Tool: Create custom runtime images with only required modules.

### Key Conflicts/Misunderstandings

**Reflection vs. Encapsulation**

- Annotations often rely on reflection (e.g., Spring’s @Autowired).

- Modules restrict reflection by default (non-exported packages are hidden).

- Fix: Use opens in module-info.java to allow reflective access:

```java
opens com.example.internal to spring.core;  
```