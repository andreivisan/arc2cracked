pub fn gcd_of_strings(str1: String, str2: String) -> String {
    if str1.is_empty() || str2.is_empty() { return String::new(); }
    let k = gcd(str1.len(), str2.len());
    if str1[..k] != str2[..k] { return String::new(); }
    let result = str1[..k].to_string();
    for i in (k..str1.len()).step_by(k) { 
        if str1[i..i+k] != result { return String::new(); }
    }
    for i in (k..str2.len()).step_by(k) {
        if str2[i..i+k] != result { return String::new(); }
    }
    result
}

fn gcd(a: usize, b: usize) -> usize {
    if b == 0 { return a; }
    gcd(b, a % b)
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn test_gcd_of_strings() {
        assert_eq!(gcd_of_strings("ABCABC".to_string(), "ABC".to_string()), "ABC".to_string());
    }

    #[test]
    fn test_gcd_of_strings_2() {
        assert_eq!(gcd_of_strings("ABABAB".to_string(), "ABAB".to_string()), "AB".to_string());
    }

    #[test]
    fn test_gcd_of_strings_3() {
        assert_eq!(gcd_of_strings("LEET".to_string(), "CODE".to_string()), "".to_string());
    }

    #[test]
    fn test_gcd_of_strings_4() {
        assert_eq!(gcd_of_strings("ABCDEF".to_string(), "ABC".to_string()), "".to_string());
    }
}