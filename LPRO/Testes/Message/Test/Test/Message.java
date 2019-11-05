package Test;

public class Message {
	
	private String sms;
	
	public Message(String random) {
		this.sms = random;
	}

	public Object getMessage() {
		
		return sms;
	}

	public void setMessage(String s) {
		this.sms = s;
		
	}

	public void setFormattingStrategy(MessageFormatter mf) {
		// TODO Auto-generated method stub
		
	}



}
