# Personal learnings

## Choosing between &str and String

- When I want to borrow an immutable slice of UTF-8 text, choose `&str`.
- When I want to mutate or own or grow the test, use `String`.

## Return types

- Sometimes I can return a set of 3 (maybe even more variables - but then the
case for a struct can be made) variables.

```rust
Result<(String, Duration, usize), TranscriptionError>
```
