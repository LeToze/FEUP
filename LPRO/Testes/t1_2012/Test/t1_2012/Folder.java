package t1_2012;

import java.util.ArrayList;

public class Folder extends Node {
	
	private ArrayList<Node> children = new ArrayList<Node>();
	
	public FileSystem File;
	private Folder rot;
	private String name;
	
	public Folder(Folder root, String string) throws DuplicateNameException {
		this.rot=root;
		this.name=string;
	}

	public Folder() {
		// TODO Auto-generated constructor stub
	}

	public Object getParent() {
		
		return rot;
	}

	@Override
	protected String getName() {
		// TODO Auto-generated method stub
		return name;
	}

	public Node[] getChild() {
		// TODO Auto-generated method stub
		children.toArray(new Node[children.size()]);
		System.out.println("que tiro foi esse"+ children.size());
		return children.toArray(new Node[children.size()]);
	}
	
	

}
