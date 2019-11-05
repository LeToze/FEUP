package t1_2014;

public class Withdrawal {

	private ATM atm;
	private Session seas;
	private Card card;
	private int amount;
	private double withdraw;

	public Withdrawal(ATM atm, Session s, Card c, int i) {
		this.atm=atm;
		this.seas=s;
		this.card=c;
		this.amount=i;
		
	}

	public void setAmount(double w) {
		this.withdraw=w;
		
	}

	public double getAmount() {
		// TODO Auto-generated method stub
		return this.withdraw;
	}

}
