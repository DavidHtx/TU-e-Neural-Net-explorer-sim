#include <SDL.h>
#include "SDL_image.h"
#include "SDL_ttf.h"
#include <stdio.h>
#include "menu_panel.h"
#include <string>
#include "maze.h"
#include "decision_point.h"
#include "SDL2_gfxPrimitives.h"
#include "configurations.h"




SDL_Window* mazeWindow = NULL;
SDL_Window* decWindow = NULL;
SDL_Surface* background = NULL;
menu_panel panel;
information_panel info_panel;
information_panel info_dec;
menu_button maze_panel_buttons[8] = { menu_button(0, 0, 0, 0, "Start"), menu_button(0, 0, 0, 0, "Pause"), menu_button(0, 0, 0, 0, "Restart"), menu_button(0, 0, 0, 0, "Reset"), menu_button(0, 0, 0, 0, "Exit"), menu_button(0, 0, 0, 0, "Manual"), menu_button(0, 0, 0, 0, " "), menu_button(0, 0, 0, 0, " ") };
menu_button decision_panel_buttons[8] = { menu_button(0, 0, 0, 0, "List"), menu_button(0, 0, 0, 0, "e"), menu_button(0, 0, 0, 0, "r"), menu_button(0, 0, 0, 0, "r"), menu_button(0, 0, 0, 0, "f"), menu_button(0, 0, 0, 0, "h"), menu_button(0, 0, 0, 0, "d"), menu_button(0, 0, 0, 0, "s") };
decision_point dec_point[91];
connection connect[61];
maze_points maze_point[60];
int  id_last_point_touched;
int total_maze_points = 0;
int total_dec_points = 0;
int total_connections = 0;
bool inteligence_on = false;
Maze  th_maze;
Mouse th_mouse;
End_Point end_point;
//SDL_Color blue = {100,255,122};
SDL_Renderer* renderer;
SDL_Renderer* renderer_2;
SDL_Rect main_background = { 0, 0, maze_SCREEN_WIDTH, maze_SCREEN_HEIGHT };
SDL_Rect decision_background = {0,default_panel_height, decision_SCREEN_WIDTH,decision_SCREEN_HEIGHT};
SDL_Rect panel_background = { 10,10,400,400 };
SDL_Event e;
SDL_Event d;
char STATE = 0;   //  0 - Choosing starting point //  1 - choosing end point //   2 - simulation in progress // 3 - post simulation mode // 4 - manual mode
void initialize()
{
	SDL_Init(SDL_INIT_VIDEO);
	TTF_Init();
	
	//  Create two windows
	mazeWindow = SDL_CreateWindow("Maze", 100, 100, maze_SCREEN_WIDTH, maze_SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	decWindow = SDL_CreateWindow("Decision network", 1300, 100,decision_SCREEN_WIDTH,decision_SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(mazeWindow, -1,SDL_RENDERER_ACCELERATED);
	renderer_2 = SDL_CreateRenderer(decWindow, -1,SDL_RENDERER_ACCELERATED);

	SDL_SetRenderDrawColor(renderer, background_color);
	SDL_SetRenderDrawColor(renderer_2, background_color);
	SDL_RenderClear(renderer);
	SDL_RenderClear(renderer_2);

	// Change the color of the background of the maze window 
	
	SDL_RenderFillRect(renderer, &main_background);
	
	// Change the color of the background of the decision window 
	SDL_RenderFillRect(renderer_2, &decision_background);
	panel.create(renderer, mazeWindow, maze_SCREEN_WIDTH,default_panel_height);
	panel.create(renderer_2, decWindow, decision_SCREEN_WIDTH,default_panel_height);
	info_panel.initialize_font();
	info_dec.initialize_font();
	info_dec.render_info_panel(renderer_2,decision_SCREEN_WIDTH/24,decision_SCREEN_HEIGHT*4/5,300,150);
	
	info_panel.render_info_panel(renderer,maze_SCREEN_WIDTH/2+info_panel_x,default_panel_height+5+info_panel_y,info_panel_w,info_panel_h);
	/*info_panel.write_text(renderer, 0, "Current simulation time:");
	info_panel.write_text(renderer, 1, "Current simulation decision points :");
	info_panel.write_text(renderer, 2, "Total simulations :");
	info_panel.write_text(renderer, 3, "Total decision points :");
	info_panel.write_text(renderer, 4, "Total time :");*/
	SDL_RenderPresent(renderer);
	SDL_RenderPresent(renderer_2);
	//Create buttons for the maze window
	
	th_maze.render_maze(renderer,maze_x_pos,maze_y_pos+default_panel_height,maze_width,maze_height);

	if (maze_buttons > 0)
	{
		int counter = 0;
		while (counter < maze_buttons)
		{
			maze_panel_buttons[counter].button_height = default_panel_height -2* button_panel_margin;
			maze_panel_buttons[counter].button_width = default_button_width - 2* button_panel_margin;
			maze_panel_buttons[counter].button_x_pos = button_panel_margin + counter*(default_button_width + default_distance_between_buttons);
			maze_panel_buttons[counter].button_y_pos = button_panel_margin;
			maze_panel_buttons[counter].render_button(renderer,0);
			counter++;
			SDL_RenderPresent(renderer);
		}
		
	}
	

	//Create buttons for the decision window 
	if (decision_buttons > 0)
	{
		int counter = 0;
		while (counter < decision_buttons)
		{
			decision_panel_buttons[counter].button_height = default_panel_height - 2 * button_panel_margin;
			decision_panel_buttons[counter].button_width = default_button_width - 2 * button_panel_margin;
			decision_panel_buttons[counter].button_x_pos = button_panel_margin + counter*(default_button_width + default_distance_between_buttons);
			decision_panel_buttons[counter].button_y_pos = button_panel_margin;
			decision_panel_buttons[counter].render_button(renderer_2,0);
			counter++;
			SDL_RenderPresent(renderer_2);
		}

	}

}


void quit_program()
{
	SDL_DestroyWindow(decWindow);
	SDL_DestroyWindow(mazeWindow);
	SDL_Quit();
}

char manual_mode_on = 0;
void button_actions(int button_number)
{ 
	if (button_number == 4)
	{
		quit_program();
		TTF_Quit();
		printf("\nProgram closed");

	}
 
	if (button_number == 2)
	{
		for (int i = 0; i < total_dec_points; i++)
		{
			dec_point[i].delete_point(renderer_2,dec_point[i].x_pos,dec_point[i].y_pos);

		}
		for (int i = 0; i < total_connections; i++)
		{
		
			connect[i].delete_connection(renderer_2);
		}

		STATE = 0;
		total_dec_points = 0;
		total_connections = 0;
		total_maze_points = 0;
	}
	if (button_number == 0 )
	{
	
		inteligence_on = true;
		
	}
	


	if (button_number == 5)
	{
		if (manual_mode_on == 1)
		{
			manual_mode_on = 0;
	    
		}

		else if (manual_mode_on == 0)
			manual_mode_on = 1;
               
	}

  }


int main(int argc, char* args[])
{
	bool quit = false;
	initialize();

	printf("Program initialized");
	
	


	//   TESTING STUFF -------------------------------------------------------------------
	/*
	
	int pl = 50;
	for (int i = 0; i <= total_dec_points; i++)
	{
		if (i <= 30)
		{
			dec_point[i].generate_decision_point(2, i );
			dec_point[i].render_point(renderer_2, 50+i*2, 80 + i * 15);
		}
		if (i > 30 && i<=60)
		{
			dec_point[i].generate_decision_point(2, i);
			dec_point[i].render_point(renderer_2, 100 + i * 2, 80 + (i - 30) * 15);
		}
	
	if (i > 60)
		{
			dec_point[i].generate_decision_point(2, i);
			dec_point[i].render_point(renderer_2, 150 + i * 2, 80 + (i - 60) * 15);
		}

		
		SDL_RenderPresent(renderer_2);
	}

	maze_point[1].create_maze_point(1, 20, 20, &th_maze, 0,&dec_point[1]);

	//--------------------
	dec_point[16].type = 3;
	dec_point[16].size = 10;
	dec_point[12].type = 0;
	dec_point[12].size = 8;
	dec_point[15].size = 9;
	//=========================
	for (int i = 0; i <= 30; i++)
	{
		connect[i].create_connection(&dec_point[i+2], &dec_point[i+29], i);
	}
	for (int i = 31; i <= total_connections; i++)
	{
	   connect[i].create_connection(&dec_point[i -30], &dec_point[i - 17], i);
	}
	*/
	//-----------------------------------------------------------------------------------

	STATE = 0;

	while (!quit)
	{
		
		while (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}

	//       *******************  Choosing starting point **************************
			
			if (STATE == 0)
			{
				manual_mode_on = 0;
				info_panel.render_info_panel(renderer, maze_SCREEN_WIDTH / 2 + info_panel_x, default_panel_height + 5 + info_panel_y, info_panel_w, info_panel_h);
				info_panel.write_text(renderer, 0, "Please choose starting point");
				info_panel.write_text(renderer, 1, "Press left click to set the place");
				if (th_mouse.create_mouse(3, 50, 50, &th_maze, &e, mazeWindow, renderer))
				{
				
				   printf("Starting point was set to x : %d  y : %d \n",th_mouse.x_pos,th_mouse.y_pos);
				   STATE = 10;
				   dec_point[0].generate_decision_point(0, 0);
				   dec_point[0].render_point(renderer_2, start_point_pos + default_panel_height + dec_point[0].size);
				   maze_point[0].create_maze_point(0,th_mouse.x_pos,th_mouse.y_pos,&th_maze,0,&dec_point[0]);
				   total_dec_points++;
				   total_maze_points++;
				   th_mouse.current_dec_ID = 0;
				}
				
				
				SDL_RenderPresent(renderer_2);
				SDL_RenderPresent(renderer);
			}

	//       ***********************************************************************
		
	//		 *******************  Intermediate state for mouse debouncing **********
			if (STATE == 10)
			{
				
					if (e.type == SDL_MOUSEBUTTONUP)
					{
						if (e.button.button == SDL_BUTTON_LEFT)
							STATE = 1;
					}
				

			}

	//       ***********************************************************************

	//		 *******************  Choosing end point  ******************************
			if (STATE == 1)
			{
				
				info_panel.render_info_panel(renderer, maze_SCREEN_WIDTH / 2 + info_panel_x, default_panel_height + 5 + info_panel_y, info_panel_w, info_panel_h);
				info_panel.write_text(renderer, 0, "Please choose ending point");
				info_panel.write_text(renderer, 1, "Press left click to set the place");

				if (end_point.create_end_point(3, 100, 100, &th_maze, &e, mazeWindow, renderer))
				{
					STATE = 3;
					printf("End point was set to x : %d  y : %d \n", end_point.x_pos, end_point.y_pos);
				}
				th_mouse.render_mouse(renderer);
				SDL_RenderPresent(renderer_2);
				SDL_RenderPresent(renderer);

			
			}
			
   //       ***********************************************************************

   //		*******************  Simulation in progress  **************************
			if (STATE == 2)
			{

				//th_mouse.render_mouse(renderer);
			}

  //       ***********************************************************************

  //	   *******************  Post simulation mode  ****************************
			if (STATE == 3)
			{
				char text[30];
				info_panel.render_info_panel(renderer, maze_SCREEN_WIDTH / 2 + info_panel_x, default_panel_height + 5 + info_panel_y, info_panel_w, info_panel_h);
				info_panel.write_text(renderer, 0, "Total decision points");
				
				sprintf_s(text, "total collisions : %d", th_mouse.number_of_colisions);
				info_panel.write_text(renderer, 1, text);
				sprintf_s(text,"str Collisions : %d",th_mouse.number_straight_collisions);
				info_panel.write_text(renderer, 2, text);
			    sprintf_s(text, "Total decision points : %d", total_dec_points);
				info_panel.write_text(renderer, 3,text);
				info_panel.write_text(renderer, 4, "Total time :");

			for (int i = 0; i < total_connections; i++)
			{
				connect[i].delete_connection(renderer_2);
			}
		
			for (int i = 0; i < total_dec_points; i++)
			{
				dec_point[i].render_point(renderer_2, dec_point[i].x_pos, dec_point[i].y_pos);
				dec_point[i].decision_point_events(renderer_2, &e, decWindow);
				dec_point[i].move_point_events(renderer_2, &e, decWindow);
				
				if (dec_point[i].is_selected)
				{
					char text[20];
					char text2[20];
					char text3[20];
					sprintf_s(text, "Point ID : %d ", dec_point[i].ID);
					sprintf_s(text2, "Times passed : %d", dec_point[i].times_passed);
					sprintf_s(text3, "Connections : %d", dec_point[i].number_of_connections);
					info_dec.render_info_panel(renderer_2, decision_SCREEN_WIDTH / 24, decision_SCREEN_HEIGHT * 4 / 5, 300, 150);
					info_dec.write_text(renderer_2, 0, text);
					info_dec.write_text(renderer_2, 1, text2);
					info_dec.write_text(renderer_2, 2, text3);
					
				}
				
			}
			
			for (int i = 0; i < total_connections; i++)
			{
				connect[i].render_connection(renderer_2);
			}
	//=============== MANUAL MODE ======================
			if (manual_mode_on == 1)
			{
				info_panel.write_text(renderer,5,"Manual Mode ON");
				if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_RIGHT] || SDL_GetKeyboardState(NULL)[SDL_SCANCODE_LEFT] || SDL_GetKeyboardState(NULL)[SDL_SCANCODE_UP] || SDL_GetKeyboardState(NULL)[SDL_SCANCODE_DOWN])
				{
					th_mouse.delete_mouse(renderer);
					//   Read input from keyboard
					if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_RIGHT])
					{
						th_mouse.x_pos += mouse_default_manual_speed;
						if(th_mouse.colision_mouse_maze())
					    th_mouse.x_pos -= mouse_default_manual_speed;
					}
					if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_LEFT])
					{
						th_mouse.x_pos -= mouse_default_manual_speed;
						if (th_mouse.colision_mouse_maze())
						th_mouse.x_pos += mouse_default_manual_speed;
					}
					if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_UP])
					{
						th_mouse.y_pos -= mouse_default_manual_speed;
						if (th_mouse.colision_mouse_maze())
							th_mouse.y_pos += mouse_default_manual_speed;
						}
					if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_DOWN])
					{
						th_mouse.y_pos += mouse_default_manual_speed;
						if (th_mouse.colision_mouse_maze())
							th_mouse.y_pos -= mouse_default_manual_speed;
					}


					// Check collision with the maze walls 
					if (th_mouse.y_pos > th_mouse.maze->maze_panel_height - th_mouse.size - maze_margin_size-2)
						th_mouse.y_pos -= mouse_default_manual_speed;
					
					if (th_mouse.x_pos > th_mouse.maze->maze_panel_width - th_mouse.size - maze_margin_size-2)
						th_mouse.x_pos -= mouse_default_manual_speed;
					
					if (th_mouse.x_pos < 0 + th_mouse.size + maze_margin_size + 2 )
						th_mouse.x_pos += mouse_default_manual_speed;
					
					if (th_mouse.y_pos < 0 + th_mouse.size + maze_margin_size + 2 )
						th_mouse.y_pos += mouse_default_manual_speed;
						
					th_mouse.render_mouse(renderer);

				}
			
			}



// ======================================================================
			else if (manual_mode_on == false && inteligence_on == true)
			{

				th_mouse.delete_mouse(renderer);
			
				int direction_chosen = 0; // 0 - up , 1 - right, 2- down, 3 - left 
				bool stoppedmoving = false;
				int prevposition_x = th_mouse.x_pos;
				int prevposition_y = th_mouse.y_pos;

				if (th_mouse.y_pos - th_mouse.top_view - th_mouse.size > th_mouse.size && th_mouse.number_of_colisions > 5) //up
				{
					direction_chosen = 0;
				}
				else if (th_mouse.x_pos - th_mouse.right_view - th_mouse.size < -th_mouse.size && th_mouse.number_of_colisions > 5) //right
				{
					direction_chosen = 1;
				}
				else if (th_mouse.y_pos - th_mouse.bottom_view - th_mouse.size < -th_mouse.size && th_mouse.number_of_colisions > 5) //down
				{
					direction_chosen = 2; 
				}
				else if (th_mouse.x_pos - th_mouse.left_view - th_mouse.size > th_mouse.size && th_mouse.number_of_colisions > 5) //left
				{ 
					direction_chosen = 3;
				}


				//Varwindow
				int topview = th_mouse.y_pos - th_mouse.top_view - th_mouse.size;
				int rightview = th_mouse.x_pos - th_mouse.right_view - th_mouse.size;
				int botview = th_mouse.y_pos - th_mouse.bottom_view - th_mouse.size;
				int leftview = th_mouse.x_pos - th_mouse.left_view - th_mouse.size;
				int mouszi = th_mouse.size * 2;



				if (direction_chosen == 0)
				{
					th_mouse.y_pos -= mouse_default_manual_speed;
					if (th_mouse.colision_mouse_maze())
						th_mouse.y_pos += mouse_default_manual_speed;
				}
				if (direction_chosen == 1)
				{
					th_mouse.x_pos += mouse_default_manual_speed;
					if (th_mouse.colision_mouse_maze())
						th_mouse.x_pos -= mouse_default_manual_speed;
				}
				if (direction_chosen == 2)
				{
					th_mouse.y_pos += mouse_default_manual_speed;
					if (th_mouse.colision_mouse_maze())
						th_mouse.y_pos -= mouse_default_manual_speed;
				}
				if (direction_chosen == 3)
				{
					th_mouse.x_pos -= mouse_default_manual_speed;
					if (th_mouse.colision_mouse_maze())
						th_mouse.x_pos += mouse_default_manual_speed;
				}

				if (prevposition_x == th_mouse.x_pos && prevposition_y == th_mouse.y_pos)
				{
				//	th_mouse.x_pos += mouse_default_manual_speed;
				//	if (th_mouse.colision_mouse_maze())
				//		th_mouse.x_pos -= mouse_default_manual_speed;
					stoppedmoving = true;
				}
		
			}
			
			
			
			
			for (int i = 0; i < total_maze_points; i++)
			{
				maze_point[i].maze_point_event(renderer);
			}
			
			// Creation of decision points
			int temp = 0;
			temp = th_mouse.detect_point();
			
			bool collision_with_point = true;
		
			for (int i = 0; i <= total_maze_points; i++)
			{
				collision_with_point = true;
				if (th_mouse.x_pos - th_mouse.size  >  maze_point[i].x_pos + maze_point[i].size)
					collision_with_point = false;
				if (th_mouse.x_pos + th_mouse.size <  maze_point[i].x_pos - maze_point[i].size)
					collision_with_point = false;
				if (th_mouse.y_pos - th_mouse.size  >  maze_point[i].y_pos + maze_point[i].size)
					collision_with_point = false;
				if (th_mouse.y_pos + th_mouse.size  <  maze_point[i].y_pos - maze_point[i].size)
					collision_with_point = false;
			
				if (collision_with_point == true)
				{
					id_last_point_touched = maze_point[i].ID;
					i = total_maze_points + 1;
				}
			}
			

			if (temp == 2 && collision_with_point == false)
			{
				dec_point[total_dec_points].generate_decision_point(2,total_dec_points);
				dec_point[total_dec_points].type = 2;
				//dec_point[total_dec_points].render_point(renderer_2,20+distance_betweeen_points*(total_dec_points+1),20+default_panel_height);
				dec_point[total_dec_points].render_point(renderer_2, dec_point[id_last_point_touched].x_pos + (dec_point[id_last_point_touched].number_of_connections -1) * 20, 20 + dec_point[id_last_point_touched].y_pos);
				bool issame = true;
				while (issame == true)
				{
					for (int i = 0; i < total_dec_points; i++)
					{
						if (dec_point[total_dec_points].x_pos == dec_point[i].x_pos && dec_point[total_dec_points].y_pos == dec_point[i].y_pos)
						{
							dec_point[total_dec_points].x_pos = dec_point[total_dec_points].x_pos + 20;
							i = 0;
						}
						if (i == total_dec_points-1)
						{
							issame = false;
						}
					}
				}
				maze_point[total_maze_points].create_maze_point(total_maze_points,th_mouse.x_pos,th_mouse.y_pos,&th_maze,2,&dec_point[total_dec_points]);
				connect[total_connections].create_connection(&dec_point[total_dec_points], &dec_point[id_last_point_touched], total_connections);
				total_maze_points++;
				total_dec_points++;
				total_connections++;
				th_mouse.current_dec_ID = total_dec_points;
			}
			
	
			if (temp == 1  && collision_with_point == false)
			{
				dec_point[total_dec_points].generate_decision_point(1, total_dec_points);
				//dec_point[total_dec_points].render_point(renderer_2, 20 + distance_betweeen_points*(total_dec_points + 1), 20 + default_panel_height);
				dec_point[total_dec_points].render_point(renderer_2, dec_point[id_last_point_touched].x_pos + (dec_point[id_last_point_touched].number_of_connections - 1) * 20, 20 + dec_point[id_last_point_touched].y_pos);
				bool issame = true;
				while (issame == true)
				{
					for (int i = 0; i < total_dec_points; i++)
					{
						if (dec_point[total_dec_points].x_pos == dec_point[i].x_pos && dec_point[total_dec_points].y_pos == dec_point[i].y_pos)
						{
							dec_point[total_dec_points].x_pos = dec_point[total_dec_points].x_pos + 20;
							i = 0;
						}
						if (i == total_dec_points-1)
						{
							issame = false;
						}
					}		
				}

				maze_point[total_maze_points].create_maze_point(total_maze_points, th_mouse.x_pos, th_mouse.y_pos, &th_maze,1, &dec_point[total_dec_points]);
				connect[total_connections].create_connection(&dec_point[total_dec_points], &dec_point[id_last_point_touched], total_connections);
				total_maze_points++;
				total_dec_points++;
				total_connections++;
				th_mouse.current_dec_ID = total_dec_points;
		
			
			}
		

			th_mouse.render_mouse(renderer);
			end_point.render_end_point(renderer);
	
		
			}
	
	//     ***********************************************************************

	//	   *******************  Manual mode  ***************************************
	
			if (STATE == 4)
			{
			

			}

    //     ***********************************************************************

			th_maze.maze_events(renderer, &e, mazeWindow);
			for (int i = 0; i < maze_buttons; i++)
			{
				if (maze_panel_buttons[i].button_event(&e, renderer, mazeWindow) == true)
				{
					button_actions(i);
				}
			}
			for (int i = 0; i < decision_buttons; i++)
			{
				if (decision_panel_buttons[i].button_event(&e, renderer_2, decWindow) == true)
				{
					button_actions(i + 8);
				}

			}
		
			SDL_RenderPresent(renderer_2);
			SDL_RenderPresent(renderer);
		}
	 
	}


	quit_program();
	TTF_Quit();

	return 0;
}