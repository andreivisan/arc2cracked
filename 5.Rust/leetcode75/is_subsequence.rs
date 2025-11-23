use std::collections::VecDeque;

pub fn is_subsequence(s: String, t: String) -> bool {
    if s.len() > t.len() { return false; }
    if s.is_empty() { return true; }
    if t.is_empty() { return false; }
    let mut queue: VecDeque<char> = s.chars().collect();
    for c in t.chars() {
        if let Some(q) = queue.front() {
            if q == &c { queue.pop_front(); }
        }
    }
    queue.is_empty()
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_is_subsequence() {
        assert_eq!(is_subsequence("abc".to_string(), "ahbgdc".to_string()), true);
    }

    #[test]
    fn test_is_subsequence_2() {
        assert_eq!(is_subsequence("axc".to_string(), "ahbgdc".to_string()), false);
    }

    #[test]
    fn test_is_subsequence_3() {
        assert_eq!(is_subsequence("".to_string(), "ahbgdc".to_string()), true);
    }   
}