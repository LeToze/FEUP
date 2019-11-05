package dkeep.logic;


public class Game {
	
	Map map;
	boolean GameOver_Index;
	Hero hero;
	int actual_x=1, actual_y=1;
	//private boolean meh;
	Dragon dragon;
	public Game() 
	{
		
		map = new Map();
		hero = new Hero();
		dragon= new Dragon();	
		
	}
	
	
	
	public void mover(String move)
	{
	
	//map.printer("  ", actual_x, actual_y);
	
	if(!map.wall_checker(actual_x, actual_y, move) ) 
	{
		map.printer("  ", actual_x, actual_y);
		//dragon.getclose(dragon, actual_x, actual_y);
		hero.move(move);
		actual_x = hero.returnx();
		actual_y = hero.returny();
		map.printer("H ",actual_x,actual_y);
	}
	
//	if(!d)
	

	//DEBUGS
//	System.out.println(move);
//	actual_x = hero.returnx(); System.out.println("Valor de x "+ hero.returnx());
//	actual_y = hero.returny(); System.out.println("Valor de y "+ actual_y);
	//DEBUGS
	
	//map.printer("H ",actual_x,actual_y);
	
	}
	
	public void draw() 
	{
		map.drawBoard();
		
	}
	

}
