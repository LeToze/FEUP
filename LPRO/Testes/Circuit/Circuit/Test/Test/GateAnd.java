package Test;

public class GateAnd extends LogicGate {

	
	public GateAnd(LogicVariable output, LogicVariable input1, LogicVariable input2)  {
			

	
			
		output.setValue(input1.getValue() && input2.getValue());
		
		output.setCalculatedBy(this);//prob6
		this.input = new LogicVariable[2];		
		this.output=output;
		this.input[0]=input1;
		this.input[1]=input2;
		
		this.output.gate = this;
		
	}

	@Override
	public String getSymbol() {
		
		return "AND";
	}

	@Override
	public boolean getValue() {
		
		boolean r = (input[0].getValue() && input[1].getValue());
		
		return (input[0].getValue() && input[1].getValue()); 
	}

	@Override
	public String getFormula() {
		
		//System.out.println("F_AND " +  input[0].getFormula() + " " + input[1].getFormula());
		return "AND(" + input[0].getFormula() + "," + input[1].getFormula() + ")";
	}

}
