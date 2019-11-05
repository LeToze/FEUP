package t1_2014;

public class Session implements Countable{
	
	private ATM atm;
	private Withdrawal withdraw;
	private Deposit deposit;
	private double balance;
	public Session(ATM atm) {
		
		this.atm=atm;
		
	}

	public ATM getATM() {
		return atm;
	}

	public void addTransaction(Withdrawal w) {
		
		this.withdraw=w;
		System.out.println("ammout w" + withdraw);
		
	}

	public void addTransaction(Deposit d) {
		// TODO Auto-generated method stub
		this.deposit=d;	
		System.out.println("ammout d" + deposit);
		
	}
	public void math() {
		balance = withdraw.getAmount()-deposit.getAmount();
	}
	@Override
	public int count() {
		
		return (int)balance;
	}
	
}
