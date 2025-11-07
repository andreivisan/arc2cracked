// My initial solution:
// Nice start! You nailed the “borrow, don’t allocate” part. A few review notes:
// Your function removes all trailing \n/\r because of the recursion. The spec was “remove one trailing line ending,” treating \r\n as a single unit.
// bytes[len-1..] == *b"\n" is clunky. Prefer checking the last byte directly (e.g., bytes.last() == Some(&b'\n')) or use s.ends_with('\n').
// Prefer is_empty() to len() == 0.
// Avoid recursion here; a single pass with a couple of byte checks is simpler and O(1) stack.
// Target behavior (no code yet, just the logic):
// If the string ends with "\r\n", drop two bytes.
// Else if it ends with '\n', drop one byte.
// Else if it ends with '\r', drop one byte.
// Else return unchanged.
// This keeps inner newlines, removes exactly one trailing line ending, and stays zero-copy.
// Micro-idioms you’ll get from clippy/rust-analyzer:
// if s.is_empty() { return s; }
// Use let len = s.len(); once, then index from that.

// Byte checks: bytes[len-1] == b'\n' and (guarded) bytes[len-2] == b'\r'
// pub fn trim_line_end(s: &str) -> &str {
//     let bytes = s.as_bytes();
//     if bytes.len() == 0 { return s; }
//     if bytes[bytes.len() - 1..] == *b"\n" {
//         return trim_line_end(&s[..bytes.len() - 1]);
//     } else if bytes[bytes.len() - 1..] == *b"\r" {
//         return trim_line_end(&s[..bytes.len() - 1]);
//     }
//     s
// }

pub fn trim_line_end(s: &str) -> &str {
    if s.is_empty() { return s; }
    if s.ends_with("\r\n") { return &s[..s.len() - 2]; }
    if s.ends_with('\n') { return &s[..s.len() - 1]; }
    if s.ends_with('\r') { return &s[..s.len() - 1]; }
    s
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test] fn no_newline() { assert_eq!(trim_line_end("abc"), "abc"); }
    #[test] fn lf()         { assert_eq!(trim_line_end("abc\n"), "abc"); }
    #[test] fn crlf()       { assert_eq!(trim_line_end("abc\r\n"), "abc"); }
    #[test] fn cr()         { assert_eq!(trim_line_end("abc\r"), "abc"); }
    #[test] fn keeps_inner(){ assert_eq!(trim_line_end("a\nb\n"), "a\nb"); }
    #[test] fn double_newline() { assert_eq!(trim_line_end("abc\n\n"), "abc\n"); }
    #[test] fn empty()      { assert_eq!(trim_line_end(""), ""); }
}