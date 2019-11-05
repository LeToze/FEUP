package t1_2015;

public class User {
	
private String userr;

	public User(String string) {
		
		this.userr=string;
	}

	public String getName() {
	
		return userr;
	}
	public boolean equals(Object obj) {
		
		
		return userr.equals(((User)obj).getName());	
	}

}
