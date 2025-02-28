public class MaximumDepthBT {
    public static class TreeNode {
        int val;
        TreeNode left;
        TreeNode right;
        TreeNode() {}
        TreeNode(int val) { this.val = val; }
        TreeNode(int val, TreeNode left, TreeNode right) {
            this.val = val;
            this.left = left;
            this.right = right;
        }
    }

    private int maxDepth;

    public MaximumDepthBT() {
        this.maxDepth = 0;
    }

    public int maxDepth(TreeNode root) {
        if (root == null) return 0;
        maxDepthHelper(root, 1);
        return maxDepth;
    }

    public void maxDepthHelper(TreeNode root, int currentDepth) {
         if (root == null) {
            return;
         }
         
         // Update maxDepth if we've reached a leaf node or a deeper level
         if (root.left == null && root.right == null) {
            this.maxDepth = Math.max(this.maxDepth, currentDepth);
         }
         
         // Pass currentDepth + 1 to the next level
         maxDepthHelper(root.left, currentDepth + 1);
         maxDepthHelper(root.right, currentDepth + 1);
    }
   
    public static void main(String args[]) {
        TreeNode root = new TreeNode(4);
        root.left = new TreeNode(2);
        root.right = new TreeNode(7);
        root.left.left = new TreeNode(1);
        root.left.right = new TreeNode(3);
        root.right.left = new TreeNode(6);
        root.right.right = new TreeNode(9);

        MaximumDepthBT mdb = new MaximumDepthBT();
        System.out.println(mdb.maxDepth(root));
    }
}
