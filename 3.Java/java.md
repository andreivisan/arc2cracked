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





