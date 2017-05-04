#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h> //for the sleep function

int won_games = 0;
int lost_games = 0;

// Draw raster functions
void print_blocks(char *row_map, int draw_pixel_limit);
void new_line();
void print_final_block();
void draw_full_row(char *row_map);
void draw_first_row(char *row_map);
void draw_second_row(int* a, char *row_map, int *foo_cell_value);
void draw_third_row(int* a, char *row_map, int *foo_cell_value);
void draw_fourth_row(int* a, char *row_map, int *foo_cell_value);
void draw_all(int *foo_cell_value);
void drawline(char *row_map);

//input functions
void get_input(int player_mark_foo, int *foo_cell_value);
int select_player_mark(int* foo_player_mark);

//game functions
void win_check_state(int *end, int *foo_cell_value, int foo_player_mark, int foo_cpu_mark, int foo_marks_on_board);

//CPU moves functions
int select_cpu_mark(int player_mark_foo,int *foo_cpu_mark);
void cpu_move_master(int player_mark_foo, int *foo_cell_value, int foo_cpu_mark, int *foo_cpu_move_number);
void random_cpu_move(int *foo_cell_value, int foo_cpu_mark);
void cpu_takes_the_centre(int *foo_cell_value, int foo_cpu_mark);
void cpu_takes_the_corner(int *foo_cell_value, int foo_cpu_mark);
void cpu_counter(int *foo_cell_value, int foo_cpu_mark, int foo_player_mark,int *has_moved_foo);
void cpu_goes_for_victory(int *foo_cell_value, int foo_cpu_mark, int *has_moved_foo);
int choose_between_three_values(int first_number, int second_number, int third_number, int* foo_cell_value);
void cpu_first_move(int *foo_cell_value, int foo_cpu_mark);
void cpu_second_move(int *foo_cell_value, int foo_cpu_mark, int foo_player_mark);
void cpu_third_move(int *foo_cell_value, int foo_cpu_mark, int foo_player_mark);

//start & menu functions
void start_screen();
void player_won_screen();
void player_lost_screen();
void draw_screen();
void new_game(int *end);
void credits_screen();
void how_to_play();

int main(){

	int end_game = 0; // controls if the game is over or not
	int player_mark = 0;
	int cpu_mark = 0;
	int cpu_move_number = 0;
	int cell_value[10] = {0,0,0,0,0,0,0,0,0,0}; //Cell values: 0 = empty, 1 = x, 10 = O
	int marks_on_board = 0;
	srand(time(NULL)); //randomise
	rand();

	start_screen();
	credits_screen();
	

	select_player_mark(&player_mark);
	select_cpu_mark(player_mark, &cpu_mark);
	how_to_play();

	system("cls");

	while (end_game == 0) {

		draw_all(cell_value);
		get_input(player_mark, cell_value);
		marks_on_board ++;
		system("cls");
		draw_all(cell_value);
		printf("THINKING MY NEXT MOVE...(CPU move %d)", cpu_move_number);
		Sleep(1000);
		system("cls");
		win_check_state(&end_game, cell_value, player_mark, cpu_mark, marks_on_board);
	
	if (end_game == 1){
		break;
	} 
	else {
		cpu_move_master(player_mark, cell_value, cpu_mark, cpu_move_number);
		cpu_move_number += 1;
		marks_on_board ++;
		win_check_state(&end_game, cell_value, player_mark, cpu_mark, marks_on_board);
		}
	}

	new_game(&end_game);

	getchar(); getchar();
	return 0;
}

void get_input(int player_mark_foo, int *foo_cell_value){
	//gets input from player 1 and assigns it to cell_value[]. 
	//the value of player_mark depends on what the player chooses in choose_x_or_o(): 1 for x and 10 for o.
	char input = getch();
	int i = input - '0';
	
	
	
	// checks if the cell is empty.
	if(foo_cell_value[i] == 0){

		switch (input){

			case '1': foo_cell_value[1] = player_mark_foo; break;
			case '2': foo_cell_value[2] = player_mark_foo; break;
			case '3': foo_cell_value[3] = player_mark_foo; break;
			case '4': foo_cell_value[4] = player_mark_foo; break;
			case '5': foo_cell_value[5] = player_mark_foo; break;
			case '6': foo_cell_value[6] = player_mark_foo; break;
			case '7': foo_cell_value[7] = player_mark_foo; break;
			case '8': foo_cell_value[8] = player_mark_foo; break;
			case '9': foo_cell_value[9] = player_mark_foo; break;
			default: 
				printf("\nPlease press a number between 1 and 9."); 
				get_input(player_mark_foo, foo_cell_value); // small recursion :-)
			break;
		} 
	} else {
			printf("\nPlease select an empty cell."); 
			get_input(player_mark_foo, foo_cell_value);
		}
	
}

void win_check_state(int *end, int *foo_cell_value, int foo_player_mark, int foo_cpu_mark, int foo_marks_on_board){

	// this array stores the 8 possible winning combinations, which are going to be checked by the loop below.

	int win_combination[8] = {  foo_cell_value[1] + foo_cell_value[2] + foo_cell_value[3],
								foo_cell_value[4] + foo_cell_value[5] + foo_cell_value[6],
								foo_cell_value[7] + foo_cell_value[8] + foo_cell_value[9],
								foo_cell_value[1] + foo_cell_value[4] + foo_cell_value[7], 
								foo_cell_value[2] + foo_cell_value[5] + foo_cell_value[8], 
								foo_cell_value[3] + foo_cell_value[6] + foo_cell_value[9], 
								foo_cell_value[1] + foo_cell_value[5] + foo_cell_value[9], 
								foo_cell_value[7] + foo_cell_value[5] + foo_cell_value[3] 
								};

	int win_count = 0;
	for(;win_count <=8; win_count++){
		switch (win_combination[win_count]) {
			
			case 30: // o is the winner
				*end = 1;
				if (win_combination[win_count] == (foo_player_mark * 3)){
					won_games ++;
					player_won_screen();
				}
				else if (win_combination[win_count] == (foo_cpu_mark * 3)){
					lost_games ++;
					player_lost_screen();
				}
			break;
			
			case 3: // x is the winner
				*end = 1;
				if (win_combination[win_count] == (foo_player_mark * 3)){
					won_games ++;
					player_won_screen();
				}
				else if (win_combination[win_count] == (foo_cpu_mark * 3)){
					lost_games ++;
					player_lost_screen();
				}
			break;

			default:
				if (foo_marks_on_board >= 9) {
					*end = 1;
					win_count = 8;
					draw_screen();
				}

			break;
			}
		}
	}

void draw_all(int *foo_cell_value){
	int n = 0; // line by line, loop counter for the content of each cell 
	int a = 7; // variable A selects the row to be drawn. It starts from 7 because the console starts drawing from top.
	char *row_map = "0"; // string that maps the "pixels" on printed on the screen per each line. Used to draw the board.

	new_line();
	draw_full_row(row_map);
	
	for (n =0, a = 7; n<3; n++, a-=3) {

		draw_first_row(row_map);
		draw_second_row(&a, row_map, foo_cell_value);
		draw_third_row(&a, row_map, foo_cell_value);
		draw_fourth_row(&a, row_map, foo_cell_value);
		draw_third_row(&a, row_map, foo_cell_value);
		draw_second_row(&a, row_map, foo_cell_value);
		draw_first_row(row_map);
		draw_full_row(row_map);

	}
	n = 0;
	a =7;
	printf(	"_______\n"
			"|7|8|9|\t*****************************|Games Won|Games Lost|\n"
			"|4|5|6|\tUSE THE NUMERIC PAD TO PLAY  |   %2d    |    %2d    |\n"
			"|1|2|3|\t*****************************|         |          |",won_games, lost_games);
}
void print_blocks(char *row_map, int draw_pixel_limit) {

/* PRINT BLOCKS FUNCTION - Board
58 "pixels" per row are printed depending on the value of the row_map string: 
if value is char '0' it draws a black square, else a space. 
the drawPixelLimit variable defines how many "pixels" are printed per each line - 19 (e.g. the width of each cell) when drawing the board.
*/
	int i = 0;

	for (i=0; i<draw_pixel_limit; i++) {
		 char pixel = 219;
		 if (row_map[i] == '0') { 
			printf("%c",pixel);
			}
		else {printf(" ");}
		}
	//END PRINT
}
void new_line() {
//NEW LINE
	printf("\n");
}
void draw_full_row(char *row_map) {
	
	//draws a full row of 58 blocks.
	int j = 0;
	for (; j<3; j++){	
		row_map = "0000000000000000000";
		print_blocks(row_map,19);
	}
	print_final_block();

}
void draw_first_row(char *row_map){
	int j = 0;

	for (;j<3;j++){	
		//this is the first/last row, which is always drawn as empty as the x and o starts above/below it
		row_map = "0111111111111111111"; // empty cell
		print_blocks(row_map,19);
	}
	print_final_block();
	
}
void draw_second_row(int* a, char *row_map, int *foo_cell_value){
	
	int j = 0;

	for (j=*a + 0;j<*a + 3;j++){	

		

		switch(foo_cell_value[j]){

			case 0:
			row_map = "0111111111111111111";
			break;
	
			case 1:
			row_map = "0111100111111001111";
			break;

			case 10:
			row_map = "0111111100001111111";
			break;

			}

		print_blocks(row_map,19);
	}
	print_final_block();
	

}
void draw_third_row(int* a, char *row_map, int *foo_cell_value){

	int j = 0;
	for (j=*a + 0;j<*a + 3;j++){

		

		switch(foo_cell_value[j]){

			case 0:
			row_map = "0111111111111111111";
			break;

			case 1:
			row_map = "0111111001100111111";
			break;

			case 10:
			row_map = "0111110011110011111";
			break;

			}
		print_blocks(row_map,19);
	}
	print_final_block();
	
}
void draw_fourth_row(int* a, char *row_map, int *foo_cell_value){

	int j = 0;

	for (j=*a + 0;j<*a + 3;j++){


		switch(foo_cell_value[j]){
			case 0:
			row_map = "0111111111111111111";
			break;
	
			case 1:
			row_map = "0111111110011111111";
			break;

			case 10:
			row_map = "0111101111111101111";
			break;

			}
		print_blocks(row_map,19);
	}
	print_final_block();

}

void print_final_block(){

	//prints the final block at the end of each line
	char final_block = 219;
	printf("%c",final_block);
	new_line();
	} 

void drawline(char *row_map){

	print_blocks(row_map, 52);
	new_line();
}

void start_screen(){

	drawline("1111111111111111111111111111111111111111111111111111");
	drawline("1011010001010100010101000100010000010001001010111111");
	drawline("1001010101010101110101101101110000010001001010111111");
	drawline("1010010101010101010001101100010000010001001010111111");
	drawline("1011010001000101010101101111010000010001001010111111");
	drawline("1111111111111100010101101100010000010001001010111111");
	drawline("1000101101001111111111111111110000010001001010111111");
	drawline("1010100101010111111111111111110000010001001010111111");
	drawline("1000101001010111111111111111110000010001001010111111");
	drawline("1010101101001111111111111111110000010001001010111111");
	drawline("1111111111111111111111111111110000010001001010111111");
	drawline("1000100010001000100010001000110000010001001010111111");
	drawline("1011101010101011101110111011110000010001001010111111");
	drawline("1011100110101000100010001000110000010001001010111111");
	drawline("1011101010101110111010111110110000010001001010111111");
	drawline("1000101010001000100010001000110000010001001010111111");
	drawline("1111111111111111111111111111111111111111111111111111");
	drawline("0000000000000000000000000000000000000000000000000000");

	//-----------------------------------------------------------------

	printf("\n\tBy Michele Morelli - December 2016");
	printf("\n\n\tPRESS ANY BUTTON TO START A NEW GAME\n");
	new_line();
	drawline("0000000000000000000000000000000000000000000000000000");
	getch();
}


int select_player_mark(int* foo_player_mark){
	int cursor_position = 1;
	char arrow = 68;

	while (*foo_player_mark != 1 && *foo_player_mark != 10) {
			system("cls");
			drawline("0000000000000000000000000000000000000000000000000000");
			printf(" USE THE DIRECTIONAL ARROWS ON YOUR KEYBOARD TO CHOOSE X OR O\n"
					" PRESS THE ENTER BUTTON TO START PLAYING!\n");
			drawline("0000000000000000000000000000000000000000000000000000");
			new_line();

	switch(cursor_position){
		case 1:
			
			drawline("1100000000000000000111111111111111111111111111111111");
			drawline("1100000000000000000111111100000000000000011111111111");
			drawline("1100111111111111100111111101111111111111011111111111");
			drawline("1100100111111100100111111101111111111111011111111111");
			drawline("1100110011111001100111111101111000000111011111111111");
			drawline("1100111001110011100111111101110011110011011111111111");
			drawline("1100111100100111100111111101100111111001011111111111");
			drawline("1100111110001111100111111101101111111101011111111111");
			drawline("1100111110001111100111111101101111111101011111111111");
			drawline("1100111100100111100111111101101111111101011111111111");
			drawline("1100111001110011100111111101100111111001011111111111");
			drawline("1100110011111001100111111101110011110011011111111111");
			drawline("1100100111111100100111111101111000000111011111111111");
			drawline("1100111111111111100111111101111111111111011111111111");
			drawline("1100000000000000000111111100000000000000011111111111");
			drawline("1100000000000000000111111111111111111111111111111111");
			drawline("1111111111111111111111111111111111111111111111111111");
			drawline("1111111111011111111111111111111111111111111111111111");
			drawline("1111111110001111111111111111111111111111111111111111");
			drawline("1111111100000111111111111111111111111111111111111111");
			drawline("1111111110001111111111111111111111111111111111111111");
			drawline("1111111110001111111111111111111111111111111111111111");
			drawline("1111111110001111111111111111111111111111111111111111");
			drawline("1111111111111111111111111111111111111111111111111111");
		break;

		case 2:
			drawline("1111111111111111111111111000000000000000001111111111");
			drawline("1110000000000000001111111000000000000000001111111111");
			drawline("1110111111111111101111111001111111111111001111111111");
			drawline("1110100111111100101111111001111111111111001111111111");
			drawline("1110110011111001101111111001111000000111001111111111");
			drawline("1110111001110011101111111001110011110011001111111111");
			drawline("1110111100100111101111111001100111111001001111111111");
			drawline("1110111110001111101111111001101111111101001111111111");
			drawline("1110111110001111101111111001101111111101001111111111");
			drawline("1110111100100111101111111001101111111101001111111111");
			drawline("1110111001110011101111111001100111111001001111111111");
			drawline("1110110011111001101111111001110011110011001111111111");
			drawline("1110100111111100101111111001111000000111001111111111");
			drawline("1110111111111111101111111001111111111111001111111111");
			drawline("1110000000000000001111111000000000000000001111111111");
			drawline("1111111111111111111111111000000000000000001111111111");
			drawline("1111111111111111111111111111111111111111111111111111");
			drawline("1111111111111111111111111111111110111111111111111111");
			drawline("1111111111111111111111111111111100011111111111111111");
			drawline("1111111111111111111111111111111000001111111111111111");
			drawline("1111111111111111111111111111111100011111111111111111");
			drawline("1111111111111111111111111111111100011111111111111111");
			drawline("1111111111111111111111111111111100011111111111111111");
			drawline("1111111111111111111111111111111111111111111111111111");
		break;
	}

		arrow = getch();

		if (cursor_position == 1 && (arrow == 75|| arrow == 77)) {
			cursor_position = 2;
		}

		else if (cursor_position == 2 && (arrow == 75|| arrow == 77)) {
			cursor_position = 1;
		}

		if (arrow == 13) { //if enter is pressed
			switch(cursor_position){
				case 1:
				*foo_player_mark = 1;
				break;

				case 2:
				*foo_player_mark = 10;
				break;


			}
		}
	}

	return *foo_player_mark;
}

void player_won_screen(){
	drawline("1111111111111111111111111111111111111111111111111111");
	drawline("1000000000000000000000000000000000000000000000011111");
	drawline("1011111111111111111111111111111111111111111111001111");
	drawline("1011111111111111101110100001011011111111111111000111");
	drawline("1011111111111111101110101101011011111111111111000111");
	drawline("1011111000001111110101101101011011111111111111000111");
	drawline("1011111011110111111011101101011011111111111111000111");
	drawline("1011110111111011111011101101011011111111111111000111");
	drawline("1011101001001101111011100001000011111111111111000111");
	drawline("1011101001001101111111111111111111111111111111000111");
	drawline("1011101111111101111111111111111111111111111111000111");
	drawline("1011101011101101110011100100100111011001001111000111");
	drawline("1011110100011011110011100100100011011001001111000111");
	drawline("1011111011110111110011100100100101011001001111000111");
	drawline("1011111100001111110010100100100110011001001111000111");
	drawline("1011111111111111111000001100100111011111111111000111");
	drawline("1011111111111111111101011100100111011001001111000111");
	drawline("1011111111111111111111111111111111111111111111000111");
	drawline("1011111111111111111111111111111111111111111111000111");
	drawline("1000000000000000000000000000000000000000000000000111");
	drawline("1100000000000000000000000000000000000000000000000111");
	drawline("1110000000000000000000000000000000000000000000000111");
	drawline("1111111111111111111111111111111111111111111111111111");
	drawline("1111111111111111111111111111111111111111111111111111");

}

void player_lost_screen(){
	
	drawline("1111111111111111111111111111111111111111111111111111");
	drawline("1111111111111111111111111111111111111111111111111111");
	drawline("1110000000000000000000000000000000000000000001111111");
	drawline("1110111111111111111111111111111111111111111100111111");
	drawline("1110111100000011111111111111111111111111111100011111");
	drawline("1110111011111101111111010100010101111111111100011111");
	drawline("1110110111111110111111010101010101111111111100011111");
	drawline("1110101111111110111111010101010101111111111100011111");
	drawline("1110101101110110111111000101010101111111111100011111");
	drawline("1110101111111110111111101101010101111111111100011111");
	drawline("1110101111111110111111101100010001111111111100011111");
	drawline("1110101100001110111111111111111111111111111100011111");
	drawline("1110101011110110111111101110001000100010101100011111");
	drawline("1110101111111110111111101110101011101110101100011111");
	drawline("1110110111111101111111101110101000101110101100011111");
	drawline("1110111011111011111111101110101110100010101100011111");
	drawline("1110111100000111111111101110101110101111111100011111");
	drawline("1110111111111111111111100010001000100010101100011111");
	drawline("1110111111111111111111111111111111111111111100011111");
	drawline("1110111111111111111111111111111111111111111100011111");
	drawline("1110000000000000000000000000000000000000000000011111");
	drawline("1111000000000000000000000000000000000000000000011111");
	drawline("1111100000000000000000000000000000000000000000011111");
	drawline("1111111111111111111111111111111111111111111111111111");


}

void draw_screen(){

	drawline("1111111111111111111111111111111111111111111111111111");
	drawline("1111111111111111111111111111111111111111111111111111");
	drawline("1000000000000000000000000000000000000000000011111111");
	drawline("1011111111111111111111111111111111111111111001111111");
	drawline("1011111111111111111111111111111111111111111000111111");
	drawline("1011100000111110000011110000001001111110011000111111");
	drawline("1011100000001110000000110000001001111110011000111111");
	drawline("1011100111001110011100110011001001111110011000111111");
	drawline("1011100111000010011100110011001001111110011000111111");
	drawline("1011100111110010011100110011001001111110011000111111");
	drawline("1011100111110010000000110000001001111110011000111111");
	drawline("1011100111110010000011110000001001100110011000111111");
	drawline("1011100111110010000011110011001001100110011000111111");
	drawline("1011100111000010010011110011001001100110011000111111");
	drawline("1011100111001110010000110011001001100110011000111111");
	drawline("1011100000001110011100110011001000000000011000111111");
	drawline("1011100000111110011100110011001000011000011000111111");
	drawline("1011111111111111111111111111111111111111111000111111");
	drawline("1011100000000000000000000000000000000000011000111111");
	drawline("1011111111111111111111111111111111111111111000111111");
	drawline("1000000000000000000000000000000000000000000000111111");
	drawline("1100000000000000000000000000000000000000000000111111");
	drawline("1110000000000000000000000000000000000000000000111111");
	drawline("1111111111111111111111111111111111111111111111111111");

}

void new_game(int *end){
	char response;

	printf("Play another game?(Y/N)\n");
	response = getch();
	switch(response){
	case 'y':
	case 'Y':
		system("cls");
		*end = 0;
		main();

	case 'n':
	case 'N':
		exit(0);
	}


}

void credits_screen(){
	system("cls");

	drawline("0000000000000000001111111111111111000000000000000000");
	drawline("0000000000000000011111111111111111100000000000000000");
	drawline("0000000000000000111110000000011111110000000000000000");
	drawline("0000000000000001111001111111100111111000000000000000");
	drawline("0000000000000011110111111111110001111100000000000000");
	drawline("0000000000000111110111111111111101111110000000000000");
	drawline("0000000000000111101111111111111110111110000000000000");
	drawline("0000000000000111000000010000000111011110000000000000");
	drawline("0000000000000111001011000110110000001110000000000000");
	drawline("0000000000000111001111010111110111110110000000000000");
	drawline("0000000000000111000000010000000111110110000000000000");
	drawline("0000000000000111011111011111111110000110000000000000");
	drawline("0000000000000111011111001111111100011110000000000000");
	drawline("0000000000000111001111111111111000011110000000000000");
	drawline("0000000000000111100000000000000001111110000000000000");
	drawline("0000000000000111100000000000000011111110000000000000");
	drawline("0000000000000111100000100100000011111110000000000000");
	drawline("0000000000000111100000111100000111111110000000000000");
	drawline("0000000000000111111000000000001111111110000000000000");
	drawline("0000000000000011111100000000001111111100000000000000");
	drawline("0000000000000001111100000000001111111000000000000000");
	drawline("0000000000000000111100000000011111110000000000000000");
	drawline("0000000000000000011111111111111111100000000000000000");
	drawline("0000000000000000001111111111111111000000000000000000");

	printf("\nDesigned and coded by Michele Morelli - December 2016.");

	getchar();

}

void how_to_play(){

	system("cls");

	drawline("1111111111111111111111111111111111111111111111111111");
	drawline("0000000000000000000000000000111111110000000000000000");
	drawline("0111111110111111110111111110111111110111101111011110");
	drawline("0111000110110001110110001110111111110111101111011110");
	drawline("0111110110110101110110101110111111110111101111011110");
	drawline("0111101110110001110110001110111111110111101111011110");
	drawline("0111101110110101110111101110111111110000000000000000");
	drawline("0111101110110001110111011110111111110111101111011110");
	drawline("0000000000000000000000000000111111110111101111011110");
	drawline("0111111110111111110111111110111111110111101111011110");
	drawline("0111101110110001110111011110111111110111101111011110");
	drawline("0111001110110111110110111110111111110000000000000000");
	drawline("0110101110110001110110001110111111110111101111010010");
	drawline("0110001110111101110110101110111111110111101111001100");
	drawline("0111101110110011110110001110111111110111101111001100");
	drawline("0000000000000000000000000000111111110111101111010010");
	drawline("0111111110111111110111111110111111110000000000000000");
	drawline("0111011110111001110110001110111111111111111111111011");
	drawline("0110011110110101110111101110111111111111111111110001");
	drawline("0111011110111011110111011110111111111111111111100000");
	drawline("0111011110110111110111101110111111111111111111111011");
	drawline("0110001110110001110110001110100000000000000000000011");
	drawline("0000000000000000000000000000111111111111111111111111");
	drawline("1111111111111111111111111111111111111111111111111111");

	printf("\nHOW TO PLAY:\nUse the numeric pad on your keyboard to put your X or O mark on the board. They look similar don't they?\nFor example, if you press '3', you will take the cell on the bottom right corner.");

	getchar();

}

int select_cpu_mark(int player_mark_foo,int *foo_cpu_mark){

	//the mark of the CPU depends on the mark chosen by the player
	
	switch (player_mark_foo){
		case 1: *foo_cpu_mark = 10; break;
		case 10: *foo_cpu_mark = 1; break;
	}

}

void cpu_move_master(int player_mark_foo, int* foo_cell_value, int foo_cpu_mark, int foo_cpu_move_number){


	switch(foo_cpu_move_number){
	case 0:
		cpu_first_move(foo_cell_value, foo_cpu_mark);
	break;

	case 1:
		cpu_second_move(foo_cell_value, foo_cpu_mark, player_mark_foo);
	break;

	case 2:
		cpu_third_move(foo_cell_value, foo_cpu_mark, player_mark_foo);
	break;

	case 3:
		cpu_third_move(foo_cell_value, foo_cpu_mark, player_mark_foo);
	break;

	default:
		random_cpu_move(foo_cell_value, foo_cpu_mark);
	break;
	}

	
}


void random_cpu_move(int *foo_cell_value, int foo_cpu_mark){

	int exit_loop = 0;

	while (exit_loop == 0){
				//chooses a random free cell and puts a CPU mark
				int i = (rand() % 9) + 1;

				if (foo_cell_value[i] == 0) { 
					foo_cell_value[i] = foo_cpu_mark;
					exit_loop = 1;
				}
				else {continue;}
			}

			exit_loop = 0;
}

void cpu_takes_the_centre(int *foo_cell_value, int foo_cpu_mark){
	//the CPU takes the centre
	foo_cell_value[5] = foo_cpu_mark;

}

void cpu_takes_the_corner(int *foo_cell_value, int foo_cpu_mark){

	int corners[4] = {1,3,7,9}; // corners on the board
	int exit_loop = 0;
	//chooses a random free cell in the corners of the board and puts a CPU mark
	
	while (exit_loop == 0){
		int i = corners[(rand() % 4)]; 
		if (foo_cell_value[i] == 0) { 
					foo_cell_value[i] = foo_cpu_mark;
					exit_loop = 1;
				}
				else {continue;}
			}

			exit_loop = 0;

}

void cpu_counter(int *foo_cell_value, int foo_cpu_mark, int foo_player_mark, int *has_moved_foo){

	// if there are 2 player marks in line, and a free cell, the CPU takes it

	int line_combinations[8] = {foo_cell_value[1] + foo_cell_value[2] + foo_cell_value[3],
								foo_cell_value[4] + foo_cell_value[5] + foo_cell_value[6],
								foo_cell_value[7] + foo_cell_value[8] + foo_cell_value[9],
								foo_cell_value[1] + foo_cell_value[4] + foo_cell_value[7], 
								foo_cell_value[2] + foo_cell_value[5] + foo_cell_value[8], 
								foo_cell_value[3] + foo_cell_value[6] + foo_cell_value[9], 
								foo_cell_value[1] + foo_cell_value[5] + foo_cell_value[9], 
								foo_cell_value[7] + foo_cell_value[5] + foo_cell_value[3] 
								};

	int combo_count = 0;
	int line_combo[3]; //3 cells in the combination in the line
	int exit_loop = 0;

	for(;combo_count  <=8; combo_count ++){

		switch (line_combinations[combo_count]) {
			
			case 20: // in case player is O
			case 2: // in case player is X
				if (line_combinations[combo_count] == (foo_player_mark *2)){
					*has_moved_foo = 1;
					switch(combo_count){

					case 0:foo_cell_value[choose_between_three_values(1,2,3,foo_cell_value)] = foo_cpu_mark; combo_count = 8; break;
					case 1:foo_cell_value[choose_between_three_values(4,5,6,foo_cell_value)] = foo_cpu_mark; combo_count = 8; break;
					case 2:foo_cell_value[choose_between_three_values(7,8,9,foo_cell_value)] = foo_cpu_mark; combo_count = 8; break;
					case 3:foo_cell_value[choose_between_three_values(1,4,7,foo_cell_value)] = foo_cpu_mark; combo_count = 8; break;
					case 4:foo_cell_value[choose_between_three_values(2,5,8,foo_cell_value)] = foo_cpu_mark; combo_count = 8; break;
					case 5:foo_cell_value[choose_between_three_values(3,6,9,foo_cell_value)] = foo_cpu_mark; combo_count = 8; break;
					case 6:foo_cell_value[choose_between_three_values(1,5,9,foo_cell_value)] = foo_cpu_mark; combo_count = 8; break;
					case 7:foo_cell_value[choose_between_three_values(7,5,3,foo_cell_value)] = foo_cpu_mark; combo_count = 8; break;
					}
				} 
				else {continue;}
			break;
			
			default: break;
			}
	}

}

void cpu_goes_for_victory(int *foo_cell_value, int foo_cpu_mark, int *has_moved_foo){

	// if there are 2 CPU marks in line, and a free cell, the CPU takes it - and goes for victory!

	int line_combinations[8] = {foo_cell_value[1] + foo_cell_value[2] + foo_cell_value[3],
								foo_cell_value[4] + foo_cell_value[5] + foo_cell_value[6],
								foo_cell_value[7] + foo_cell_value[8] + foo_cell_value[9],
								foo_cell_value[1] + foo_cell_value[4] + foo_cell_value[7], 
								foo_cell_value[2] + foo_cell_value[5] + foo_cell_value[8], 
								foo_cell_value[3] + foo_cell_value[6] + foo_cell_value[9], 
								foo_cell_value[1] + foo_cell_value[5] + foo_cell_value[9], 
								foo_cell_value[7] + foo_cell_value[5] + foo_cell_value[3] 
								};

	int combo_count = 0;
	int line_combo[3]; //3 cells in the combination in the line
	int exit_loop = 0;

	for(;combo_count  <=8; combo_count ++){

		switch (line_combinations[combo_count]) {
			
			case 20: // in case CPU is O
			case 2: // in case CPU is X
				if (line_combinations[combo_count] == (foo_cpu_mark *2)){
					*has_moved_foo = 1;
					switch(combo_count){

					case 0:foo_cell_value[choose_between_three_values(1,2,3,foo_cell_value)] = foo_cpu_mark; combo_count = 8; break;
					case 1:foo_cell_value[choose_between_three_values(4,5,6,foo_cell_value)] = foo_cpu_mark; combo_count = 8; break;
					case 2:foo_cell_value[choose_between_three_values(7,8,9,foo_cell_value)] = foo_cpu_mark; combo_count = 8; break;
					case 3:foo_cell_value[choose_between_three_values(1,4,7,foo_cell_value)] = foo_cpu_mark; combo_count = 8; break;
					case 4:foo_cell_value[choose_between_three_values(2,5,8,foo_cell_value)] = foo_cpu_mark; combo_count = 8; break;
					case 5:foo_cell_value[choose_between_three_values(3,6,9,foo_cell_value)] = foo_cpu_mark; combo_count = 8; break;
					case 6:foo_cell_value[choose_between_three_values(1,5,9,foo_cell_value)] = foo_cpu_mark; combo_count = 8; break;
					case 7:foo_cell_value[choose_between_three_values(7,5,3,foo_cell_value)] = foo_cpu_mark; combo_count = 8; break;
					}
				} 
				else {continue;}
			break;
			
			default: break;
			}
	}
}

int choose_between_three_values(int first_number, int second_number, int third_number, int* foo_cell_value){

	int cells_in_a_line[3] = {first_number, second_number, third_number}; // the three cells in a row
	 
	int exit_loop = 0;
	int empty_cell = 0;
	//finds the empty cell in the combination in the line
	
	while (exit_loop == 0){
		int i = cells_in_a_line[(rand() % 3)];
		if (foo_cell_value[i] == 0) { 			//if cell is empty
					empty_cell = i;
					exit_loop = 1;
				}
				else {continue;}
			}

			exit_loop = 0;
			return empty_cell;

}

void cpu_first_move(int *foo_cell_value, int foo_cpu_mark){

	//winning movelist - first move

	if (foo_cell_value[5] == 0) {
		cpu_takes_the_centre(foo_cell_value, foo_cpu_mark);
	}

	else {
			cpu_takes_the_corner(foo_cell_value,foo_cpu_mark);
		}
}

void cpu_second_move(int *foo_cell_value, int foo_cpu_mark, int foo_player_mark){

	int has_moved = 0;

	cpu_counter(foo_cell_value, foo_cpu_mark, foo_player_mark, &has_moved);
	if (has_moved == 0){
		random_cpu_move(foo_cell_value, foo_cpu_mark);
	}

}

void cpu_third_move(int *foo_cell_value, int foo_cpu_mark, int foo_player_mark){

	int has_moved = 0;

	cpu_goes_for_victory(foo_cell_value, foo_cpu_mark, &has_moved);
	if (has_moved == 0){
		cpu_counter(foo_cell_value, foo_cpu_mark, foo_player_mark, &has_moved);
		if (has_moved == 0){
			random_cpu_move(foo_cell_value, foo_cpu_mark);
		}
	}
}
