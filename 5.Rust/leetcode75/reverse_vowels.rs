pub fn reverse_vowels(s: String) -> String {
    const VOWELS: &[u8; 10] = b"aeiouAEIOU";
    let mut start = 0;
    let mut end = s.len() - 1;
    let mut b = s.into_bytes();
    while start < end {
        if !VOWELS.contains(&b[start]) { start += 1; continue; }
        if !VOWELS.contains(&b[end]) { end -= 1; continue; }
        b.swap(start, end);
        start += 1;
        end -= 1;
    }
    String::from_utf8(b).unwrap()
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_reverse_vowels() {
        assert_eq!(reverse_vowels("hello".to_string()), "holle".to_string());
    }

    #[test]
    fn test_reverse_vowels_2() {
        assert_eq!(reverse_vowels("leetcode".to_string()), "leotcede".to_string());
    }
}