package dkeep.logic;


public class Hero {
	
	private int x = 1; //hero position
	private int y = 1;

public void move(String move) 
	{	
	System.out.println(move);
		if(move.equals("w")) 
		{
		y--;	
		}
		if(move.equals("s")) 
		{
		y++;
		}
		if(move.equals("a")) 
		{
		x--;
		}
		if(move.equals("d")) 
		{
		x++;
		
		}
		else
		{
			move = "o";
			
		}
		System.out.println(x);
	}
public int returnx() 
{
	return x;
}
public int returny() 
{
	return y;
}


	}
