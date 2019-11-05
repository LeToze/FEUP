package t1_2014;

public class Account {

	private String person;
	private double balance;

	public Account(String string, double d) {
		this.person=string;
		this.balance=d;
	}

	public Object getName() {
		// TODO Auto-generated method stub
		return this.person;
	}

	public double getBalance() {
		// TODO Auto-generated method stub
		return this.balance;
	}

}
