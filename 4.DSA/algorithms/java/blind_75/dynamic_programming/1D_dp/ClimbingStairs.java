public class ClimbingStairs {
    public int climbStairs(int n) {
        if (n <= 3) return n;
        if (dp.get(n) != null) return dp.get(n);
        int value = climbStairs(n-1) + climbStairs(n-2);
        dp.put(n, value);
        return dp.get(n);
    }
}
