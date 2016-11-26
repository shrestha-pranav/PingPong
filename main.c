#include "AT91SAM7L128.h"
#include "lcd.h"
#include "keyboard.h"

int main()
{
	// Disable the watchdog timer
	*AT91C_WDTC_WDMR = AT91C_WDTC_WDDIS;

	//Initialize lcd and keyboard
	lcd_init();
	keyboard_init();
	
	//Initialize delay
	const int delay = 100;
	int key;
	
	//Initialize scores	
	int p1_score = 0;
	int p2_score = 0;
	
	//Initialize ball (x,y) speed 
	int db_x = -1;
	int db_y = -1;
	
	//Print scoreboard
	lcd_put_char7('P', 0);
	lcd_put_char7('1', 1);
	lcd_put_char7('P', 7);
	lcd_put_char7('2', 8);
	
	//Play a game until score reaches five
	while(p1_score < 5 && p2_score <5){
		
		//Initialize paddle positions
		int p1_x = 1;
		int p1_y = 2;
		int p2_x = 41;
		int p2_y = 2;
		
		//Initialize ball position
		int b_x = 21;
		int b_y = 3;
	
		int scored = 0;
		
		//print score
		lcd_put_char7(p1_score+'0', 3);
		lcd_put_char7('-',4);
		lcd_put_char7(p2_score+'0', 5);
		
		//This is the physics loop, it handles all game logic
		while(scored == 0){
			//print paddles and ball
			int i;
			for(i = 0; i < 2; i++){
				lcd_print( p1_x, p1_y+i, 1);
				lcd_print( p2_x, p2_y+i, 1);
			}
			lcd_print( b_x, b_y, 1);
			
			//delay animation and get user input
			key = -1;
			for(i = 0; i < delay; i++){
				int tmpkey = keyboard_key();
				if(key == -1) key = tmpkey;
			}		
			
			//Check collision with paddles
			//If ball hits left paddle
			if(b_x == 2 && (p1_y == b_y || p1_y + 1 == b_y) ) {
				//db_y stays the same
				db_x = -db_x;
			}
			//If ball hits right paddle
			else if(b_x == 40 && (p2_y == b_y || p2_y +1 == b_y) ) {
				//db_y stays the same
				db_x = -db_x;
			}
			
			//Check collision with side walls
			//If so, reset game and increase scores
			if(b_x == 0 || b_x == 42) {
				if (b_x == 0){
					p2_score++;
					scored = 1;
				}
				if (b_x == 42) {
					p1_score++;
					scored = 2;
				}
			}
			
			//Check collision with top wall or bottom wall	
			if(b_y == 0 || b_y == 5) {
				db_y = -db_y;
			}
				
			//Else db_x, db_y stays the same
			
			//Clear the pixels of the paddles and balls.
			//Clear paddle
			for(i = 0; i < 2; i++){
				lcd_print(p1_x, p1_y+i, 0);
				lcd_print( p2_x,p2_y+i, 0);
			}
			//Clear ball
			lcd_print(b_x, b_y, 0);
			
			//Update paddle location using user input
			if(key == '9' && p2_y > 0) p2_y--;
			else if(key == '6' && p2_y <4) p2_y++;
			else if(key == '7' && p1_y >0) p1_y--;
			else if(key == '4' && p1_y <4) p1_y++;
			//Update the position of the ball based on its current velocity
			b_x += db_x;
			b_y += db_y;
		}

	}
	//When the score reaches 5, print winner message
	if(p1_score ==5 ) lcd_print7("CONGRATS P1!");
	else lcd_print7("CONGRATS P2!");
}