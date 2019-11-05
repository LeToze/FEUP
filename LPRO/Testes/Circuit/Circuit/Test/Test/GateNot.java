package Test;

public class GateNot extends LogicGate {

	public GateNot(LogicVariable output, LogicVariable input) {
		this.input = new LogicVariable[1];
		
//		if(output.getCalculatedBy() != null)
//			throw new ColisionException();
//		output.setCalculatedBy(this);
//	

		
		
		output.setValue(!input.getValue());	//prob8
		
		output.setCalculatedBy(this); //prob6
		
		
		this.input[0]=input;
		this.output=output;
	
	}

	@Override
	public String getSymbol() {
		
		return "NOT";
	}

	@Override
	public boolean getValue() {
	
		return !(input[0].getValue());
	}

	@Override
	public String getFormula() {
		//System.out.println("F_Not " +  input[0].getFormula());
		return "NOT(" + input[0].getFormula() + ")";
	}

}
