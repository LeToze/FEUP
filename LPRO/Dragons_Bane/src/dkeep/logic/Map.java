package dkeep.logic;

public class Map 
{
	public boolean this_is_a_wall;
	
	
	static String [] [] gameboard = {
			
			{"X ", "X ", "X ","X ", "X ", "X ","X ", "X ","X ", "X ", "  "},
			{"X ", "H ", "  ","  ", "  ", "  ","  ", "  ","  ", "X ", "  "},
			{"X ", "  ", "X ","X ", "  ", "X ","  ", "X ","  ", "X ", "  "},
			{"X ", "D ", "X ","X ", "  ", "X ","  ", "X ","  ", "X ", "  "},
			{"X ", "  ", "X ","X ", "  ", "X ","  ", "X ","  ", "X ", "  "},
			{"X ", "  ", "  ","  ", "  ", "  ","  ", "X ","  ", "E ", "  "},
			{"X ", "  ", "X ","X ", "  ", "X ","  ", "X ","  ", "X ", "  "},
			{"X ", "  ", "X ","X ", "  ", "X ","  ", "X ","  ", "X ", "  "},
			{"X ", "S ", "X ","X ", "  ", "  ","  ", "  ","  ", "X ", "  "},
			{"X ", "X ", "X ","X ", "X ", "X ","X ", "X ","X ", "X ", "  "}
			
						 			};

	
	
	
	
public void printer(String string, int actual_x, int actual_y) 
{
	gameboard[actual_y] [actual_x] = string;	
	
}

public boolean wall_checker(int x, int y, String move) 
{
	if(move.equals("w") & (gameboard[y-1] [x]).equals("X ")) 
	{
	this_is_a_wall = true;
	return this_is_a_wall;
	}
	else if(move.equals("s") & (gameboard[y+1] [x]).equals("X ")) 
	{
	this_is_a_wall = true;
	return this_is_a_wall;
	}
	else if(move.equals("a") & (gameboard[y] [x-1]).equals("X ")) 
	{	
	this_is_a_wall = true;
	return this_is_a_wall;
	}
	else if(move.equals("d") & (gameboard[y] [x+1]).equals("X ")) 
	{
	this_is_a_wall = true;
	return this_is_a_wall;
	}
	else
	{
		
		this_is_a_wall = false;
		return this_is_a_wall;
	}

}

public boolean wall_checker(int x, int y) 
{
	if((gameboard[y] [x]).equals("X "))
	{
		return true;
	}
	else 
	{ 
		return false;
	}
}



public void drawBoard() 
{
for(int i = 0; i < 10 ; i++) {	
	for(int j = 0; j < 11 ; j++) {
		System.out.print(gameboard[i] [j]);
	}		
System.out.println();		
	}
}

}
