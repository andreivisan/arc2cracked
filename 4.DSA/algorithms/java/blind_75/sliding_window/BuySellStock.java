public class BuySellStock {

    public int maxProfit(int[] prices) {
        if (prices.length == 1) return 0;
        int maxProfit = 0;
        int minPrice = prices[0];
        for (int i = 1; i < prices.length; i++) {
            minPrice = Math.min(minPrice, prices[i]);
            maxProfit = Math.max(maxProfit, prices[i] - minPrice);
        }
        return maxProfit;
    }

    public static void main(String[] args) {
        BuySellStock bss = new BuySellStock();
        int[] prices = { 7, 6, 4, 3, 1 };
        System.out.println(bss.maxProfit(prices));
    }
}
