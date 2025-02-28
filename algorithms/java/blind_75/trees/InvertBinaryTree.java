public static void main(String[] args) {
    // Create a tree from the array [4,2,7,1,3,6,9]
    TreeNode root = new TreeNode(4);
    root.left = new TreeNode(2);
    root.right = new TreeNode(7);
    root.left.left = new TreeNode(1);
    root.left.right = new TreeNode(3);
    root.right.left = new TreeNode(6);
    root.right.right = new TreeNode(9);
    
    InvertBinaryTree solution = new InvertBinaryTree();
    
    System.out.println("Original Tree (Pre-order traversal):");
    solution.printTree(root);
    
    // Invert the tree
    root = solution.invertTree(root);
    
    System.out.println("\nInverted Tree (Pre-order traversal):");
    solution.printTree(root);
} 