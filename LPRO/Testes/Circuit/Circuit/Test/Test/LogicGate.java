package Test;

public abstract class LogicGate {
	protected LogicVariable input[];
	protected LogicVariable output;

	
	public LogicVariable[] getInputs() {
		return input.clone();
	}

	public LogicVariable getOutput() {
		
		return this.output;
	}
	
	public abstract String getSymbol();
	public abstract boolean getValue(); 
	public abstract String getFormula();
	
}