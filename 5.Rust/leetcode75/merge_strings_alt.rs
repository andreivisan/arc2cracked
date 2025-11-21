pub fn merge_alt(word1: String, word2: String) -> String {
    let mut result = String::with_capacity(word1.len() + word2.len());
    let mut iter1 = word1.chars();
    let mut iter2 = word2.chars();
    loop {
        match (iter1.next(), iter2.next()) {
            (Some(a), Some(b)) => {
                result.push(a);
                result.push(b);
            }
            (Some(a), None) => { result.push(a); }
            (None, Some(b)) => { result.push(b); }
            (None, None) => { break; }
        }
    }
    result
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_merge_alt() {
        assert_eq!(merge_alt("abc".to_string(), "pqr".to_string()), "apbqcr".to_string());
    }

    #[test]
    fn test_merge_alt_2() {
        assert_eq!(merge_alt("ab".to_string(), "pqrs".to_string()), "apbqrs".to_string());
    }

    #[test]
    fn test_merge_alt_3() {
        assert_eq!(merge_alt("abcd".to_string(), "pq".to_string()), "apbqcd".to_string());
    }
}