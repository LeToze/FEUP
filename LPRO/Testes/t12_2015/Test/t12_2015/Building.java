package t12_2015;

public class Building implements Facility{

	private String name;
	private int maxFloor;
	private int minFloor;

	public Building(String string, int i, int j) throws IllegalArgumentException{
		
		this.name=string;
		this.maxFloor=j;
		this.minFloor=i;
		if(this.minFloor>this.maxFloor) {
			throw new IllegalArgumentException();
		}
	}

	public String getName() {
		
		return this.name;
	}

	public int getMinFloor() {
		// TODO Auto-generated method stub
		return this.minFloor;
	}

	public int getMaxFloor() {
		// TODO Auto-generated method stub
		return this.maxFloor;
	}

	public Object getCapacity() {
		// TODO Auto-generated method stub
		return null;
	}

}
