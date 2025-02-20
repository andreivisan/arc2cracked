public class ValidPalindrome {

    public boolean isPalindrome(String s) {
        if (s == null || s.length() <= 1) return true;
        String cleanText = s.replaceAll("[^a-zA-Z0-9]", "").toLowerCase();
        int l = 0;
        int r = cleanText.length() - 1;
        while (l < r) {
            if (
                !String.valueOf(cleanText.charAt(l)).equals(
                    String.valueOf(cleanText.charAt(r))
                )
            ) {
                return false;
            }
            l++;
            r--;
        }
        return true;
    }

    public static void main(String[] args) {
        ValidPalindrome vp = new ValidPalindrome();
        System.out.println(vp.isPalindrome("A man, a plan, a canal: Panama"));
    }
}
