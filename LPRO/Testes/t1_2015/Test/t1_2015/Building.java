package t1_2015;
import java.util.ArrayList;
public class Building implements Facility{

	private String b_name;
	private int build_min;
	private int build_max;
	private int capacity;
	private ArrayList<Room> rooms = new ArrayList<Room>();
	
	public Building(String block, int min_floor, int max_floor) throws IllegalArgumentException {
		
		this.b_name = block;
		this.build_min=min_floor;
		this.build_max=max_floor;
		if (build_min > build_max) {
			throw new IllegalArgumentException();
		}
	}

	public String getName() {
	
		return b_name;
	}

	public int getMinFloor() {
		
		return build_min;
	}

	public int getMaxFloor() {
		// TODO Auto-generated method stub
		return build_max;
	}
	
	public String toString() {
		
		return "Building(" + b_name + ")";
	}

	public int getCapacity() {
		
		return capacity;
	}
	public void addCap(int value) {
		capacity+=value;
	}
	
	public ArrayList<Room> getRooms(){
		ArrayList<Room> clone = (ArrayList<Room>) rooms.clone();
		return clone;
	}
	public void addRoom(Room r) throws DuplicateRoomException{
		rooms.add(r);
	}

	

}
