package Test;

public class GateOr extends LogicGate{

	public GateOr(LogicVariable output, LogicVariable input1, LogicVariable input2) throws ColisionException {
		if(output.getCalculatedBy() != null) {
			throw new ColisionException();
		}
		
		
		output.setValue(input1.getValue() || input2.getValue());
		
		
		output.setCalculatedBy(this);//prob6
		
		this.input = new LogicVariable[2];
		this.output=output;
		this.input[0]=input1;
		this.input[1]=input2;
		
	}

	@Override
	public String getSymbol() {

		return "OR";
	}

	@Override
	public boolean getValue() {
		System.out.println("HI");
		
		return (input[0].getValue() || input[1].getValue());
	}

	@Override
	public String getFormula() {
		//System.out.println("F_OR " +  input[0].getFormula() + " " + input[1].getFormula());
		return "OR(" + input[0].getFormula() + "," + input[1].getFormula() + ")";
	}

}
