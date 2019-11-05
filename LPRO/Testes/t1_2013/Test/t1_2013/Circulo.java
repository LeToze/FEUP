package t1_2013;

public class Circulo extends Figura {
	
	private Ponto ponto;
	private int raio;
	private double area;
	public Circulo(Ponto p, int i)  {
		// TODO Auto-generated constructor stub
		this.ponto=p;
		this.raio=i;
		
		if(this.raio <= 0) {
			throw new IllegalArgumentException();
		}
		
		
	}

	public Object getRaio() {
		// TODO Auto-generated method stub
		return raio;
	}

	public Object getCentro() {
		// TODO Auto-generated method stub
		return ponto;
	}
	public double getArea() {
		area = Math.PI*raio*raio;
		
		return area;
	}

	@Override
	protected double getPerimetro() {
		// TODO Auto-generated method stub
		return 0;
	}

	@Override
	int count() {
		// TODO Auto-generated method stub
		return 0;
	}

}
