# Personal learnings

## Choosing between &str and String

- When I want to borrow an immutable slice of UTF-8 text, choose `&str`.
- When I want to mutate or own or grow the test, use `String`.
