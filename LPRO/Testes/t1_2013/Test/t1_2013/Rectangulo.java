package t1_2013;

public class Rectangulo extends Figura {

	private int l1;
	private int l2;
	private int l3;
	private int l4;
	private double perimetro=0;
	private int area;


	public Rectangulo(int x1, int y1, int x2, int y2) {
		this.l1=x1;
		this.l2=x2;
		this.l3=y1;
		this.l4=y2;
	}

	@Override
	protected double getArea() {
		this.area= (l2-l1) * (l4-l3);
		return area;
	}

	@Override
	protected double getPerimetro() {
		this.perimetro = (l2-l1)*2 + (l4-l3)*2;
		
		return perimetro;
	}

	@Override
	int count() {
		// TODO Auto-generated method stub
		return 0;
	}

}
