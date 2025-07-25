pub fn is_palindrome(s: &str) -> bool {
    if s.len() < 2 {
        return true;
    }

    let bytes = s.as_bytes();
    let (mut left, mut right) = (0usize, bytes.len() - 1);

    //helper closures for speed
    let to_lower = |b: u8| b.to_ascii_lowercase();
    let is_alnum = |b: u8| b.is_ascii_alphanumeric();

    while left < right {
        while left < right && !is_alnum(bytes[left]) { left += 1 };
        while left < right && !is_alnum(bytes[right]) { right -= 1 };

        if left >= right {
            break;
        }

        if to_lower(bytes[left]) != to_lower(bytes[right]) {
            return false;
        }
        left += 1;
        right -= 1;
    }

    return true;
}



#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn ascii_and_unicode() {
        assert!(is_palindrome("Madam, I'm Adam"));
        assert!(is_palindrome("А роза упала на лапу Азора"));
        assert!(is_palindrome(""));
        assert!(is_palindrome("aa"));
        assert!(!is_palindrome("Hello"));
    }

}
