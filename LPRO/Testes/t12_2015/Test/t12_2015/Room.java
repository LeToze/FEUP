package t12_2015;

public class Room implements Facility {

	private Building building;
	private String name;
	private int floor;

	public Room(Building b, String string, int i) throws IllegalArgumentException{
		
		
		this.building=b;
		this.name=string;
		this.floor=i;
		
	if(this.floor>b.getMaxFloor()) {
		throw new IllegalArgumentException();
	}
		
		// TODO Auto-generated constructor stub
	}

	public Object getBuilding() {
		// TODO Auto-generated method stub
		return building;
	}

	public String getNumber() {
		// TODO Auto-generated method stub
		return name;
	}

	public String getName() {
		// TODO Auto-generated method stub
		return (building.getName() + this.name);
	}

	public int getFloor() {
		// TODO Auto-generated method stub
		return this.floor;
	}

}
