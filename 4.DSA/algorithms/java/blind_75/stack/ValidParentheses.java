import java.util.ArrayDeque;
import java.util.Deque;
import java.util.HashMap;
import java.util.Map;

public class ValidParentheses {

    private static final Map<Character, Character> PARENTHESES = Map.of(
        ')',
        '(',
        ']',
        '[',
        '}',
        '{'
    );

    public boolean isValid(String s) {
        if (s.length() == 1) {
            return false;
        }
        Deque<Character> stack = new ArrayDeque<>();
        for (char c : s.toCharArray()) {
            Character match = PARENTHESES.get(c);
            if (match == null) {
                stack.push(c);
            } else if (stack.isEmpty() || stack.pop() != match) { // Closing bracket
                return false;
            }
        }
        return stack.isEmpty();
    }

    public static void main(String[] args) {
        ValidParentheses vp = new ValidParentheses();
        String s = "(]";
        System.out.println(vp.isValid(s));
    }
}
