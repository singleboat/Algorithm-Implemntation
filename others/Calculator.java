import java.util.HashMap;
import java.util.Map;
import java.util.Scanner;
import java.util.Stack;

public class Calculator {
	static Map<Character, Integer> priority;

	boolean isValid(String s) {
		if (s.equals("")) return true;
		if (isOperation(s.charAt(0)) && s.charAt(0) != '-') return false;
		int max = display.length()-1;
		if (isOperatioin.charAt(max)) return false;
		int leftBracket = 0, rightBracket = 0;
		for (int i = 1; i <= max; i++) {
			if ((isOperation(s.charAt(i)) && s.charAt(i) != '(') && isOperation(s.charAt(i-1))) return false;
			if (display[i] == '(') {
				if (s.charAt(i+1) == '+'|| s.charAt(i+1) == '*' || s.charAt(i+1) == '/') return false;
				leftBracket++;
			}
			if (display[i] == ')') {
				if (rightBracket >= leftBracket) return false;
				if (isOperation(s.charAt(i-1)) && s.charAt(i-1) != '(') return false;
				rightBracket++;
			}
		}
		if (leftBracket != rightBracket) return false;
		return true;
	}
	
	static boolean isOperation(char c) {
		return (c == '+' || c == '-' || c == '*' || c == '/' || c == '(' || c == ')');
	}
	
	static boolean isPrior(char op1, char op2) {
		return priority.get(op1) > priority.get(op2);
	}
	
	static String convert(String in) {
		String s = "", tmp = "";
		Stack<Character> op = new Stack<>();
		
		for (int i = 0; i < in.length(); i++) {
			while (i < in.length() && !isOperation(in.charAt(i))) s += in.charAt(i++);
			if (i != 0 && !isOperation(in.charAt(i-1)))s += " ";

			if (in.charAt(i) == '-' && (i == 0 || in.charAt(i-1) == '(')) s += "0 ";
			if (in.charAt(i) == ')') {
				char c;
				while ((c = op.pop()) != '(') s += (c+" ");
				continue;
			}
			
			if (op.isEmpty() || isPrior(in.charAt(i), op.peek())) {
				op.push(in.charAt(i));
			} else {
				while(!(op.isEmpty() || isPrior(in.charAt(i), op.peek()) || op.peek() == '(')) s += (op.pop()+" ");
				op.push(in.charAt(i));
			}
		}
		while (!op.isEmpty()) s += (op.pop()+" ");
		
		return s;
	}
	
	
	static Integer cal(int left, int right, String op) {
		if (op.equals("+")) return left+right;
		else if (op.equals("-")) return left-right;
		else if (op.equals("*")) return left*right;
		else if (op.equals("/")) return left/right;
		return 0;
	}
	
	
	public static void main(String[] args) {
		priority = new HashMap<Character, Integer>();
		priority.put('+', 0);
		priority.put('-', 0);
		priority.put('*', 1);
		priority.put('/', 1);
		priority.put('(', 2);
		
		Scanner scanner = new Scanner(System.in);
		String in = "";
		while (!(in = scanner.nextLine()).equals("exit")) {
			if (!isValid(in)) continue;

			String s = convert(in);
			
			String exp[] = s.split(" ");
			
			Stack<Integer> number = new Stack<>();
			
			for (int i = 0; i < exp.length; i++) {
				if (exp[i].equals("+") || exp[i].equals("-") || exp[i].equals("*") || exp[i].equals("/")) {
					int right = number.pop();
					int left = number.pop();
					number.push(cal(left, right, exp[i]));
				} else {
					number.push(Integer.parseInt(exp[i]));
				}
			}
			System.out.println("the result is "+number.peek().toString());
			
		}
	}
}
