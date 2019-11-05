package t1_2012;

public class File extends Node {

	private Folder rot;
	private String name;

	public File(Folder root, String string) {
		this.rot=root;
		this.name=string;
	}

	@Override
	protected Object getParent() {
		// TODO Auto-generated method stub
		return this.rot;
	}

	@Override
	protected String getName() {
		// TODO Auto-generated method stub
		return this.name;
	}

}
