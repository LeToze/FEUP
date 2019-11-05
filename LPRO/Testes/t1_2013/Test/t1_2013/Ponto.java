package t1_2013;

public class Ponto implements Comparable<Ponto>{

	private int X;
	private int Y;

	public Ponto(int i, int j) {
		this.X=i;
		this.Y=j;
	}
	
	public String toString() {
		return "(" + this.X + ", " + this.Y + ")";
	}

	public int getX() {
		// TODO Auto-generated method stub
		return this.X;
	}

	public int getY() {
		// TODO Auto-generated method stub
		return this.Y;
	}

	public boolean equals(Object obj) {
		System.out.println("what is this value  X" + X +" Y "+ Y);
		return X == ((Ponto)obj).getX() &&
				Y == ((Ponto)obj).getY();
	}

	@Override
	public int compareTo(Ponto p2) {
		//System.out.println("what is this value" + this.X);
		if(this.X == p2.getX()) {
			System.out.println("dentro do if->" + (Y - p2.getY()));
			return Y - p2.getY();
		}
		return (X - p2.getX());
		//return 0;
	}

}
