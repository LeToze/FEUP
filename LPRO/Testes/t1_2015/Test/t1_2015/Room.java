package t1_2015;

import java.util.ArrayList;

public class Room implements Facility{
	private Building b_r;
	private String b_number;
	private int floor;
	private String name;
	private String number;
	private int capacity;
	private int icapacity;
	

	public Room(Building b, String string, int i) throws IllegalArgumentException, DuplicateRoomException {
		// TODO Auto-generated constructor stub
		
		this.floor=i;
		this.b_number=string;
		this.b_r=b;
		this.name = b.getName() + b_number;
		this.number=b.getName();
		
		ArrayList<Room> rooms = b.getRooms();
		for(Room elem: rooms)
		if(this.equals(elem)) {
			throw new DuplicateRoomException();
		} 
		b.addRoom(this);
		
		
		
		if(i > b.getMaxFloor()) {
			throw new IllegalArgumentException();
		}
	}

	public Room(Building b, String string, int i, int cap) throws DuplicateRoomException {
		this.floor=i;
		this.b_number=string;
		this.b_r=b;
		this.name = b.getName() + b_number;
		this.number=b.getName();
		this.capacity=cap;
		this.b_r.addCap(capacity);
		
		ArrayList<Room> rooms = b.getRooms();
		for(Room elem: rooms)
		if(this.equals(elem)) {
			throw new DuplicateRoomException();
		} 
		b.addRoom(this);
		
	}

	public Object getBuilding() {
		// TODO Auto-generated method stub
		return b_r;
	}

	public Object getNumber() {
		// TODO Auto-generated method stub
		return b_number;
	}

	public String getName() {
		// TODO Auto-generated method stub
	//	String Str = new String();
		return name;
	}

	public Object getFloor() {
		// TODO Auto-generated method stub
		return floor;
	}
public String toString() {
		
		return "Room(" + number + ","+ b_number + ")"; //Room(B,104)
	}

public int getCapacity() {
	
	System.out.println("capacidade" + icapacity);
	
	return this.capacity;
}
public int getBuildCapacity() {
	
	this.icapacity = this.capacity + this.icapacity;
	
	return this.icapacity ;
}
public boolean equals(Object obj) {
	return (b_r == ((Room)obj).getBuilding()) && 
			number.equals(((Room)obj).getNumber()) && 
			(floor == ((Room) obj).getFloor());
}
}
