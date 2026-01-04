pub fn shuffle(nums: Vec<i32>, n: i32) -> Vec<i32> {
    let n = n as usize;
    let mut result: Vec<i32> = Vec::with_capacity(nums.len());
    for i in 0..n {
        result.push(nums[i]);
        result.push(nums[n+i]);
    }
    result
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    pub fn test_shuffle() {
        let nums: Vec<i32> = vec![2,5,1,3,4,7];
        let n: i32 = 3;
        let result = shuffle(nums, n);
        assert_eq!(result, vec![2,3,5,4,1,7]);
    }
}
