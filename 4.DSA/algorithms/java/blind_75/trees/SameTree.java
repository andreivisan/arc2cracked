public class SameTree {
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

    public boolean isSameTree(TreeNode p, TreeNode q) {
        return dfs(p, q);   
    }

    public boolean dfs(TreeNode p, TreeNode q) {
        if (p == null && q == null) {
            return true;
        } else if (
                (p == null && q!= null)  || 
                (p != null && q == null) ||
                (p.val != q.val)
                ) {
            return false;
        }       
        return dfs(p.left, q.left) && dfs(p.right, q.right);
    }

    public static void main(String args[]) {
        TreeNode root = new TreeNode(4);
        root.left = new TreeNode(2);
        root.right = new TreeNode(7);
        root.left.left = new TreeNode(1);
        root.left.right = new TreeNode(3);
        root.right.left = new TreeNode(6);
        root.right.right = new TreeNode(9);

        TreeNode root1 = new TreeNode(4);
        root1.left = new TreeNode(2);
        root1.right = new TreeNode(7);
        root1.left.left = new TreeNode(1);
        root1.left.right = new TreeNode(3);
        root1.right.left = new TreeNode(6);

        SameTree st = new SameTree();
        System.out.println(st.isSameTree(root, root1));
    }
}
