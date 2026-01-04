pub fn find_max_consecutive_ones(nums: Vec<i32>) -> i32 {
    let n = nums.len() as usize;        
    let mut max_ones: i32 = 0;
    let mut temp_max: i32 = 0;
    for i in 0..n {
        if nums[i] == 1 { temp_max = temp_max + 1; } 
        else {
            max_ones = max_ones.max(temp_max);
            temp_max = 0;
        }
    }
    max_ones = max_ones.max(temp_max);
    max_ones
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    pub fn test_max_ones_1() {
        let nums: Vec<i32> = vec![1,1,0,1,1,1];
        let max_ones = find_max_consecutive_ones(nums);
        assert_eq!(max_ones, 3);
    }
}
