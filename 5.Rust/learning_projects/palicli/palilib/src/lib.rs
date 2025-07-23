pub fn is_palindrome(s: &str) -> bool {
    if s.len() < 2 {
        return true;
    }
    let mut left: usize = 0;
    let mut right: usize = s.len() - 1;
    let s_bytes = s.as_bytes();
    while left < right && left < s.len() {
        let left_char = s_bytes[left as usize] as char;
        let right_char = s_bytes[right as usize] as char;
        if left_char != right_char {
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
    fn it_works() {
        let true_result = is_palindrome("madam");
        assert_eq!(true_result, true);
        let str_len_3 = is_palindrome("aba");
        assert_eq!(str_len_3, true);
        let false_result = is_palindrome("Hello");
        assert_eq!(false_result, false);
    }
}
