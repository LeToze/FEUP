package t1_2014;

public class Deposit {

	private ATM atm;
	private Session seas;
	private Card card;
	private int rand;
	private double deposit;

	public Deposit(ATM atm, Session s, Card c, int i) {
		this.atm=atm;
		this.seas=s;
		this.card=c;
		this.rand=i;
		
	}
	public void setAmount(double d) {
		this.deposit=d;
		
	}

	public double getAmount() {
		
		return this.deposit;
	}

}
