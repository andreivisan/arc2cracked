pub fn is_valid(s: String) -> bool {
    let mut stack = Vec::with_capacity(s.len());

    for c in s.chars() {
        match c {
            // opening brackets → push
            '(' | '[' | '{' => stack.push(c),
            // closing brackets → pop and compare
            ')' | ']' | '}' => {
                if let Some(para) = stack.pop() {
                    if !matches!((para, c),
                        ('(', ')') | ('[', ']') | ('{', '}')) {
                        return false;
                    }
                } else {
                    return false;
                }
            }
            _ => return false,
        } 
    }

    stack.is_empty()
}

#[cfg(test)]
mod tests {
    use super::*;
    
    #[test]
    fn test_is_valid() {
        let s = "()[]{}";
        let result = is_valid(s.to_string());
        assert_eq!(result, true);
        let s2 = "(]";
        let result2 = is_valid(s2.to_string());
        assert_eq!(result2, false);
    }
}
