package t12_2015;

public class User {

	private String user;

	public User(String string) {
		
		// TODO Auto-generated constructor stub
		this.user=string;
	}

	public String getName() {
		// TODO Auto-generated method stub
		return user;
	}
	
	public boolean equals(User u1) {
		
		if( this.user == u1.getName()) {
			return true;
		}
		
		return false;
	}

}
