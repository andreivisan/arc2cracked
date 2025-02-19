import java.util.HashMap;
import java.util.Map;

public class TwoSum {

    public int[] twoSum(int[] nums, int target) {
        Map<Integer, Integer> map = new HashMap<>();
        for (int i = 0; i < nums.length; i++) {
            int diff = target - nums[i];
            if (map.containsKey(diff)) {
                int[] result = new int[2];
                result[0] = map.get(diff);
                result[1] = i;
                return result;
            }
            map.put(nums[i], i);
        }
        return new int[2];
    }
}
