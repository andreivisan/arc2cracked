use std::collections::HashMap;

pub fn two_sum(nums: Vec<i32>, target: i32) -> Vec<i32> {
    let mut map: HashMap<i32, i32> = HashMap::new();
    let mut result: Vec<i32> = Vec::new();

    for (index, &value) in nums.iter().enumerate() {
        let diff = target - value;
        if map.contains_key(&diff) {
            result.push(index as i32);
            result.push(map.get(&diff).unwrap().clone());
        } else {
            map.insert(value, index as i32);
        }
    }

    result
}

#[cfg(test)]
mod tests {
    use super::*;
    
    #[test]
    fn test_two_sum() {
        let nums = vec![2, 7, 11, 15];
        let target = 9;
        let result = two_sum(nums, target);
        assert_eq!(result, vec![1, 0]);
    }
}
