package agent_inference;

import java.util.ArrayList;
import java.util.List;

public class Enumeration {
	final  int n = 8;
	boolean isEntail;
	
	boolean pl_true(List<List<Logic>> sentence, List<Logic> model) {
		boolean[][] breeze;
		boolean[][] pit;
		breeze = new boolean[n][n];
		pit = new boolean[n][n];
		
		for (Logic cell: model) {
			if (cell.s.type == 'b') breeze[cell.s.x][cell.s.y] = cell.judge;
			else pit[cell.s.x][cell.s.y] = cell.judge;
		}
		
		for (List<Logic> c: sentence) {
			boolean flag = false;
			for (Logic l: c) {
				if (l.s.type == 'b' && l.judge == breeze[l.s.x][l.s.y]) {
					flag = true;
					break;
				} else if (l.s.type == 'p' && l.judge == pit[l.s.x][l.s.y]) {
					flag = true;
					break;
				}
			}
			if (flag == false) return false;
		}
		return true;
	}
	
	void tt_check_all(List<List<Logic>> kb, List<List<Logic>> a, List<Symbol> symbols, List<Logic> model) {
		if (symbols.isEmpty()) {
			if (pl_true(kb, model)) {
				if (pl_true(a, model)) {
					isEntail = true;
					for (Logic l: model) System.out.printf("%c%d%d:%b ", l.s.type, l.s.x, l.s.y, l.judge);
					System.out.println("");
				}
			}
		} else {
			Symbol s = symbols.get(0);
			symbols.remove(0);
			List<Symbol> symbols1 = new ArrayList<Symbol>(symbols);
			List<Symbol> symbols2 = new ArrayList<Symbol>(symbols);
			
			List<Logic> model1 = new ArrayList<Logic>(model);
			model1.add(new Logic(s, true));
			List<Logic> model2 = new ArrayList<Logic>(model);
			model2.add(new Logic(s, false));
			
			tt_check_all(kb, a, symbols1, model1);
			tt_check_all(kb, a, symbols2, model2);
		}
	}
	
	boolean tt_entails(List<List<Logic>> kb, List<List<Logic>> a) {
		List<Symbol> symbols = new ArrayList<>();
		int i;
		
		for (List<Logic> list: kb) {
			for (Logic l: list) {
				for (i = 0; i < symbols.size(); i++) {
					if (symbols.get(i).equals(l.s)) break;
				}
				if (i == symbols.size()) symbols.add(l.s);
			}
		}
		
		List<Logic> model = new ArrayList<>();
		
		isEntail = false;
		tt_check_all(kb, a, symbols, model);
		
		if (isEntail) return true;
		return false;
	}
}

class Symbol {
	char type;
	int x, y;
	Symbol(char type, int x, int y) {
		this.type = type;
		this.x = x;
		this.y = y;
	}
	
	@Override
	public boolean equals(Object obj) {
		if (obj instanceof Symbol) {
			Symbol s = (Symbol)obj;
			if (s.type == this.type && s.x == this.x && s.y == this.y) return true;
		}
		return false;
	}
}

class Logic {
	Symbol s;
	boolean judge;
	Logic(Symbol s, boolean j) {
		this.s = s;
		this.judge = j;
	}
	
	@Override
	public boolean equals(Object obj) {
		Logic log = (Logic)obj;
		if (s.equals(log.s) && judge == log.judge) return true;
		return false;
	}
}