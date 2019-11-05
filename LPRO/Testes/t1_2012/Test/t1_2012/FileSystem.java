package t1_2012;

public class FileSystem{

	private String file;
	private Folder Fold;
	
	public FileSystem(String string) {
		
		this.Fold = new Folder();
		this.file=string;
	}

	public String getType() {
		
		return file;
	}

	public Folder getRoot() {
		// TODO Auto-generated method stub
		return Fold;
	}

	

}
