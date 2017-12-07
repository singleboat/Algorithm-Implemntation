package agent_inference;

import java.util.ArrayList;
import java.util.List;

public class ForwardChaining {
	boolean pl_fc_entails(List<Logic> agenda, List<HornClause> kb, Logic query) {
		List<Logic> inferred = new ArrayList<>();
		while (!agenda.isEmpty()) {
			Logic s = agenda.get(0);
			agenda.remove(0);
			if (!inferred.contains(s)) {
				inferred.add(s);
				for (HornClause c: kb) {
					if (c.premise.contains(s)) {
						c.count--;
						if (c.count == 0 && !inferred.contains(c.head)) {
							System.out.printf("%c%d%d:%b", c.head.s.type,
									c.head.s.x, c.head.s.y, c.head.judge);
							if (c.head.equals(s)) return true;
							agenda.add(c.head);
							System.out.print("->");
						}
					}
				}
			}
		}
		return false;
	}
}

class HornClause {
	ArrayList<Logic> premise;
	Logic head;
	int count;
	HornClause() {
		premise = new ArrayList<>();
		count = 0;
	}
	
	public void addPremise(Logic s) {
		premise.add(s);
		count++;
	}
	
	public void setHead (Logic s) {
		this.head = s;
	}
}
