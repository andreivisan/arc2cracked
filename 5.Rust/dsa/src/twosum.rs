use std::collections::HashMap;

pub fn two_sum(nums: Vec<i32>, target: i32) -> Vec<i32> {
    let mut map: HashMap<i32, i32> = HashMap::new();

    for (index, &value) in nums.iter().enumerate() {
        if let Some(&diff) = map.get(&(target - value)) {
            return vec![index as i32, diff];
        }
        map.insert(value, index as i32);
    }

    vec![]
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
