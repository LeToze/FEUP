package dkeep.logic;
import java.util.Random; 


public class Dragon {
	
	private int x;
	private int y;

	
	private void dragonGenerator() 
	{
		 Random rand = new Random(); 
		//while(!wall_checker())
		x = rand.nextInt(10);
		y = rand.nextInt(10);
		
	}
	
		
	
	public int dragon_returnx() 
	{
		return x;
	}

	public int dragon_returny() 
	{
		return y;
	}
	
/*	public boolean getclose(int dragon, int actual_x, int actual_y) 
	{
		if(dragon == 1 & ) 
		{
			
		}
		
		return false;
		
	}*/
	
	
}
