package t1_2013;

public class FiguraComposta extends Figura {
	private Figura[] figuras;

	public FiguraComposta(Figura[] figurass) {
		this.figuras = figurass;
		// TODO Auto-generated constructor stub
		
	}

	@Override
	protected double getArea() {
		// TODO Auto-generated method stub
		double result = 0;
		for(Figura elem : figuras) {
			result+=elem.getArea();
			//System.out.println("variando " + result);
		}
		
		return result;
	}

	@Override
	protected double getPerimetro() {
		// TODO Auto-generated method stub
		
		return 0;
	}

	@Override
	int count() {
		// TODO Auto-generated method stub
		return figuras.length;
	}

}
