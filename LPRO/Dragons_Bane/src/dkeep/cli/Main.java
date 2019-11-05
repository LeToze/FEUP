package dkeep.cli;

import java.util.Scanner;
import dkeep.logic.Game;

public class Main 
{
	
	public static void main(String[] args)
	{
		Game Jogo = new Game();	//criei o jogo
		Scanner mv = new Scanner(System.in);//creiei scanner
		Jogo.draw(); // desenhei a tabua inicial
		
		while(true /*!Jogo.GameOverIndex()*/) 
		{
			//char move = mv.findInLine(".").charAt(0);//SCAN
			String move = mv.nextLine();
			Jogo.mover(move);
			
			
			Jogo.draw();//desenho
		}
		//mv.close();  // fechei scanner  //DESCOMENTAR ISTO 
	}
}
