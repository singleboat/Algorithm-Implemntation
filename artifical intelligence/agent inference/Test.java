package agent_inference;

import java.util.ArrayList;
import java.util.List;

public class Test {
	public static List<List<Logic>> SetKBForEnum() {
		List<List<Logic>> kb = new ArrayList<>();
		//R1
		List<Logic> r1 = new ArrayList<>();
		r1.add(new Logic(new Symbol('p', 1, 1), false));
		kb.add(r1);
		//R2
		List<Logic> r2_1 = new ArrayList<>();
		r2_1.add(new Logic(new Symbol('b', 1, 1), false));
		r2_1.add(new Logic(new Symbol('p', 1, 2), true));
		r2_1.add(new Logic(new Symbol('p', 2, 1), true));
		kb.add(r2_1);
		List<Logic> r2_2 = new ArrayList<>();
		r2_2.add(new Logic(new Symbol('b', 1, 1), true));
		r2_2.add(new Logic(new Symbol('p', 1, 2), false));
		kb.add(r2_2);
		List<Logic> r2_3 = new ArrayList<>();
		r2_3.add(new Logic(new Symbol('b', 1, 1), true));
		r2_3.add(new Logic(new Symbol('p', 2, 1), false));
		kb.add(r2_3);
		//R3
		List<Logic> r3_1 = new ArrayList<>();
		r3_1.add(new Logic(new Symbol('b', 2, 1), false));
		r3_1.add(new Logic(new Symbol('p', 1, 1), true));
		r3_1.add(new Logic(new Symbol('p', 2, 2), true));
		r3_1.add(new Logic(new Symbol('p', 3, 1), true));
		kb.add(r3_1);
		List<Logic> r3_2 = new ArrayList<>();
		r3_2.add(new Logic(new Symbol('b', 2, 1), true));
		r3_2.add(new Logic(new Symbol('p', 1, 1), false));
		kb.add(r3_2);
		List<Logic> r3_3 = new ArrayList<>();
		r3_3.add(new Logic(new Symbol('b', 2, 1), true));
		r3_3.add(new Logic(new Symbol('p', 2, 2), false));
		kb.add(r3_3);
		List<Logic> r3_4 = new ArrayList<>();
		r3_4.add(new Logic(new Symbol('b', 2, 1), true));
		r3_4.add(new Logic(new Symbol('p', 3, 1), false));
		kb.add(r3_4);
		//R4
		List<Logic> r4 = new ArrayList<>();
		r4.add(new Logic(new Symbol('b', 1, 1), false));
		kb.add(r4);
		//R5
		List<Logic> r5 = new ArrayList<>();
		r5.add(new Logic(new Symbol('b', 2, 1), true));
		kb.add(r5);
		
		return kb;
	}
	
	public static List<HornClause> SetKBForFC() {
		List<HornClause> kb = new ArrayList<>();
		//B11 <==> (P12 || P21)
		HornClause h1 = new HornClause();
		h1.addPremise(new Logic(new Symbol('p', 1, 1), false));
		h1.addPremise(new Logic(new Symbol('p', 1, 2), false));
		h1.setHead(new Logic(new Symbol('b', 1, 1), false));
		kb.add(h1);
		HornClause h2 = new HornClause();
		h2.addPremise(new Logic(new Symbol('p', 1, 2), true));
		h2.setHead(new Logic(new Symbol('b', 1, 1), true));
		kb.add(h2);
		HornClause h3 = new HornClause();
		h3.addPremise(new Logic(new Symbol('p', 2, 1), true));
		h3.setHead(new Logic(new Symbol('b', 1, 1), true));
		kb.add(h3);
		//B21 <==> (P11 || P22 || P31)
		HornClause h4 = new HornClause();
		h4.addPremise(new Logic(new Symbol('p', 1, 1), false));
		h4.addPremise(new Logic(new Symbol('p', 2, 2), false));
		h4.addPremise(new Logic(new Symbol('p', 3, 1), false));
		h4.setHead(new Logic(new Symbol('b', 2, 1), false));
		kb.add(h4);
		HornClause h5 = new HornClause();
		h5.addPremise(new Logic(new Symbol('b', 2, 1), false));
		h5.setHead(new Logic(new Symbol('p', 1, 1), false));
		kb.add(h5);
		HornClause h6 = new HornClause();
		h6.addPremise(new Logic(new Symbol('b', 2, 1), false));
		h6.setHead(new Logic(new Symbol('p', 2, 2), false));
		kb.add(h6);
		HornClause h7 = new HornClause();
		h7.addPremise(new Logic(new Symbol('b', 2, 1), false));
		h7.setHead(new Logic(new Symbol('p', 3, 1), false));
		kb.add(h7);
		
		HornClause h8 = new HornClause();
		h8.addPremise(new Logic(new Symbol('b', 1, 2), true));
		h8.addPremise(new Logic(new Symbol('p', 2, 2), false));
		h8.setHead(new Logic(new Symbol('p', 1, 3), true));
		kb.add(h8);
		return kb;
	}

	public static void main(String[] args) {
		List<List<Logic>> EnumKB = SetKBForEnum();
		List<HornClause> FCKB = SetKBForFC();
		
		List<List<Logic>> a = new ArrayList<>();
		List<Logic> query = new ArrayList<>();
		query.add(new Logic(new Symbol('p', 3, 1), true));
		a.add(query);
		
		Enumeration e = new Enumeration();
		e.tt_entails(EnumKB, a);
		
		List<Logic> agenda = new ArrayList<>();
		agenda.add(new Logic(new Symbol('p', 1, 1), false));
		agenda.add(new Logic(new Symbol('b', 1, 1), false));
		agenda.add(new Logic(new Symbol('p', 2, 1), false));
		agenda.add(new Logic(new Symbol('b', 2, 1), false));
		agenda.add(new Logic(new Symbol('p', 1, 2), false));
		agenda.add(new Logic(new Symbol('b', 1, 2), true));
		
		ForwardChaining fc = new ForwardChaining();
		fc.pl_fc_entails(agenda, FCKB, new Logic(new Symbol('p', 1, 3), true));
	}

}
