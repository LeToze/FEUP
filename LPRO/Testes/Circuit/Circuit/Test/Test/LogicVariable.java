package Test;

public class LogicVariable {

	
	private boolean value;
	private String name;
	LogicGate gate;
	//private boolean value2;
	//private LogicGate calculatedBy;
	

	public LogicVariable(String string, boolean b) {
		super();
		
		this.name = string;
		this.value = b;
	
		
		
	}
	public void setValue(boolean b) {
		
	this.value = b;
	//System.out.println("my last is " + this.value);
		
	}

	public LogicVariable(String string) {
		super();
	
		this.name = string;

		
	}

	public Object getName() {
		
		return name;
	}

	public boolean getValue() {
		if(gate == null) {
			return value;
		}
		System.out.println("hi2");
		boolean r = this.gate.getValue();
		return r;
	}


	public boolean equals(Object obj) {
		
		return name.equals( ((LogicVariable) obj).getName());
	}

	public String getFormula() {
		if(gate == null) {
			return name;
		}
		//System.out.println("DONKEY GATE " + this.gate);
		return gate.getFormula();
	}

	public LogicGate getCalculatedBy() {

		return gate;
	}
	public void setCalculatedBy(LogicGate value) {
		
		this.gate = value;
	}
}
