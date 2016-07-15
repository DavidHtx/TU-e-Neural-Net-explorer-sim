#include "Maze.h"
#include <stdio.h>
#include "menu_panel.h"
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "configurations.h"
#include "SDL2_gfxPrimitives.h"
#include "iostream"
#include "fstream"
using namespace std;
Maze::Maze()
{
	
}


Maze::~Maze()
{
}


void Maze::render_maze(SDL_Renderer* brender, int x, int y, int w, int h)
{

	maze_panel_x_pos = x;
	maze_panel_y_pos = y;

	SDL_Surface* mazefile = SDL_LoadBMP("maze.bmp");
	SDL_Surface* moveicon = SDL_LoadBMP("cursor_move.bmp");
	maze_file_surface = mazefile;
	maze_panel_height = mazefile->h;
	maze_panel_width = mazefile->w;


    SDL_Rect maze_margin_surface = { x, y, mazefile->w+2*maze_margin_size, mazefile->h+2*maze_margin_size };
	margin_surface = maze_margin_surface;
	SDL_Rect maze_main_surface = { x + maze_margin_size, y + maze_margin_size, mazefile->w, mazefile->h };
	main_surface = maze_main_surface;
	SDL_Rect move_icon_pos = { maze_margin_surface.x + maze_margin_surface.w, maze_margin_surface.y - move_icon_size, move_icon_size, move_icon_size};
	
	maze_panel_height = maze_main_surface.h;
	maze_panel_width = maze_main_surface.w;
	
	icon_pos = move_icon_pos;
	SDL_Texture* moveicontexture = SDL_CreateTextureFromSurface(brender, moveicon);
	SDL_Texture* mazetexture = SDL_CreateTextureFromSurface(brender, mazefile);
	maze_txt = mazetexture;
	icon_texture = moveicontexture;
	maze_to_array();
	SDL_SetRenderDrawColor(brender, maze_margin_color);
	SDL_RenderFillRect(brender, &maze_margin_surface);
	SDL_SetRenderDrawColor(brender, maze_background_color);
	SDL_RenderFillRect(brender, &maze_main_surface);
	SDL_RenderPresent(brender);
	SDL_RenderCopy(brender, mazetexture, NULL, &maze_main_surface);
	SDL_RenderCopy(brender, moveicontexture, NULL, &move_icon_pos);
	
}


void Maze::maze_to_array()
{
	std::fstream myfile;
	myfile.open("MazeFile.Walls", ios::out | ios::in | ios::app);
	for (int x = 0; x <= maze_file_surface->w - 1; x++)
	{
		for (int y = 0; y <= maze_file_surface->h - 1; y++)
		{
			
			myfile >> array_m[x][y];


		}
	
	}

	// writes the file in the console 
	/*    
	for (int x = 0; x <= maze_file_surface->w - 1; x++)
	{
		for (int y = 0; y <= maze_file_surface->h - 1; y++)
		{

			printf("%c",array_m[x][y]);


		}
		printf("\n");
	}

	*/

	
}




void Maze::refresh_maze(SDL_Renderer* brender)
{
	margin_surface.x = maze_panel_x_pos;
	margin_surface.y = maze_panel_y_pos;
	main_surface.x = maze_panel_x_pos + maze_margin_size;
	main_surface.y = maze_panel_y_pos + maze_margin_size;
	icon_pos.x = margin_surface.x + margin_surface.w;
	icon_pos.y = margin_surface.y - move_icon_size;

	SDL_SetRenderDrawColor(brender, maze_margin_color);
	SDL_RenderFillRect(brender, &margin_surface);
	SDL_SetRenderDrawColor(brender, maze_background_color);
	SDL_RenderFillRect(brender, &main_surface);
	SDL_RenderCopy(brender, maze_txt, NULL, &main_surface);
	SDL_RenderCopy(brender, icon_texture, NULL, &icon_pos);

}




void Maze::maze_events(SDL_Renderer* brender, SDL_Event *e, SDL_Window*window)
{
	if ((e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEMOTION) && e->window.windowID == SDL_GetWindowID(window))
	{
		int x, y;
		SDL_GetMouseState(&x, &y);
	    mouse_on_icon = true;
		if (x < icon_pos.x - icon_pos.w)
			mouse_on_icon = false;
		else if (x> icon_pos.x + icon_pos.w)
			mouse_on_icon = false;
		else if (y < icon_pos.y - icon_pos.h)
			mouse_on_icon = false;
		else if (y> icon_pos.y + icon_pos.h)
			mouse_on_icon = false;
	
		if (mouse_on_icon == true)
		{
			if (e->type == SDL_MOUSEBUTTONDOWN)
			{
				if (moving == false)
					moving = true;
				else
					moving = false;
			
					
			
			}
			
		}
		if (moving == true)
		{
		
			SDL_Rect clear_area{ maze_panel_x_pos, maze_panel_y_pos, maze_panel_width+2*maze_margin_size, maze_panel_height+2*maze_margin_size };
			SDL_SetRenderDrawColor(brender,background_color);
			SDL_RenderFillRect(brender, &clear_area);
			SDL_RenderFillRect(brender, &icon_pos);
			SDL_GetMouseState(&x, &y);
			if (y < default_panel_height + icon_pos.h/2)
				y = default_panel_height + icon_pos.h/2;
		
			maze_panel_x_pos = x - maze_panel_width - 2 * maze_margin_size - icon_pos.w / 2;
			maze_panel_y_pos = y + icon_pos.h / 2;
			refresh_maze(brender);
		    
		}
	
	
	}
 }

Mouse::Mouse()
{}
Mouse::~Mouse()
{}

void Mouse::render_mouse(SDL_Renderer* brender)
{
	render_vision(brender);
	filledCircleRGBA(brender,maze->main_surface.x+x_pos,maze->main_surface.y+y_pos,size,mouse_default_color);

}

void Mouse::delete_mouse(SDL_Renderer* brender)
{
	// might be important 
	/*lineRGBA(brender, maze->main_surface.x + bottom_left_x, maze->main_surface.y + bottom_left_y, maze->main_surface.x + top_right_x, maze->main_surface.y + top_right_y,maze_background_color);
	lineRGBA(brender, maze->main_surface.x + top_left_x, maze->main_surface.y + top_left_y, maze->main_surface.x + bottom_right_x, maze->main_surface.y + bottom_right_y, maze_background_color);
	lineRGBA(brender, maze->main_surface.x + x_pos, maze->main_surface.y + top_view, maze->main_surface.x + x_pos, maze->main_surface.y + bottom_view, maze_background_color);
	lineRGBA(brender, maze->main_surface.x + left_view, maze->main_surface.y + y_pos, maze->main_surface.x + right_view, maze->main_surface.y + y_pos, maze_background_color);*/

	filledCircleRGBA(brender, maze->main_surface.x + x_pos, maze->main_surface.y + y_pos, size, maze_base_color);
	maze->refresh_maze(brender);
}

bool Mouse::create_mouse(int id, int x, int y, Maze* ref_source, SDL_Event* e, SDL_Window* window, SDL_Renderer* brender)
{ 
        maze = ref_source;
		char selected = 0;
		ID = id;
		if ((e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEWHEEL) && e->window.windowID == SDL_GetWindowID(window))
	{
		int x, y;
		SDL_GetMouseState(&x,&y);
	
		bool inside = true;
		if (x < maze->main_surface.x  +size )
			inside = false;
		if (x> maze->main_surface.x + maze->main_surface.w - size )
			inside = false;
		if (y < maze->main_surface.y + size )
			inside = false;
		if (y > maze->main_surface.y + maze->main_surface.h - size)
			inside = false;
		if (inside == true )
		{
			
				x_pos = x - maze->main_surface.x;
				y_pos = y - maze->main_surface.y;

				if (colision_mouse_maze() == 0)
				{
				maze->refresh_maze(brender);
				render_mouse(brender);
				//colision_mouse_maze();
				if (e->type == SDL_MOUSEBUTTONDOWN)
				{
					selected = 1;

				}


			}

		}
		
	}
		
		if (selected == 1)
		{
			return 1;
		}

	return 0;

	
	}


bool Mouse::colision_mouse_maze()
{ 
	
		
	for (int j = x_pos - size; j <= x_pos + size; j++)
	{
		for (int i = y_pos - size; i <= y_pos + size; i++)
		{
			if (maze->array_m[i][j] == '1')
			{
				return 1;
			
			}
			
		}
	
	
	}
	
	return 0;
}

void Mouse::render_vision(SDL_Renderer* brender)
{
	 left_view = x_pos - size - vision_distance;
	 right_view = x_pos + size + vision_distance;
	 bottom_view = y_pos + size + vision_distance;
	 top_view = y_pos - size - vision_distance;
	 bottom_left_x = left_view;
	 bottom_left_y = bottom_view;
	 bottom_right_x = right_view;
	 bottom_right_y = bottom_view;
	 top_left_x = left_view; 
	 top_left_y = top_view;
	 top_right_x = right_view;
	 top_right_y = top_view;
	
	for (int i = x_pos - vision_distance - size; i <= x_pos ; i++)
	{
		if (maze->array_m[y_pos][i] == '1') 
	    left_view = i;
	}
	
	for (int i = x_pos + size + vision_distance; i >= x_pos ; i--)
	{
		if (maze->array_m[y_pos][i] == '1')
			right_view = i;
	}
	
	for (int i = y_pos - size - vision_distance; i <= y_pos; i++)
	{
		if (maze->array_m[i][x_pos] == '1')
			top_view = i;
	}
	
	for (int i = y_pos + size + vision_distance; i >= y_pos; i--)
	{
		if (maze->array_m[i][x_pos] == '1')
			bottom_view = i;
	}
	
	for (int i = vision_distance; i >= 0 ; i--)
	{
			if (maze->array_m[y_pos - size - i][x_pos - size - i] == '1')
			{
				top_left_x = x_pos - size - i;
				top_left_y = y_pos -size - i;
			}
		
	}

	for (int i = vision_distance; i >= 0; i--)
	{
		if (maze->array_m[y_pos + size + i][x_pos + size + i] == '1')
		{
			bottom_right_x = x_pos + size + i;
			bottom_right_y = y_pos + size + i;
		}

	}

	for (int i = vision_distance; i >= 0; i--)
	{
		if (maze->array_m[y_pos - size - i][x_pos + size + i] == '1')
		{
			top_right_x = x_pos + size + i;
			top_right_y = y_pos - size - i;
		}

	}
	for (int i = vision_distance; i >= 0; i--)
	{
		if (maze->array_m[y_pos + size + i][x_pos - size - i] == '1')
		{
			bottom_left_x = x_pos - size - i;
			bottom_left_y = y_pos + size + i;
			
		}

	}

	
	lineRGBA(brender, maze->main_surface.x + bottom_left_x, maze->main_surface.y + bottom_left_y, maze->main_surface.x + top_right_x, maze->main_surface.y + top_right_y, vision_color);
	lineRGBA(brender,maze->main_surface.x + top_left_x,maze->main_surface.y+top_left_y,maze->main_surface.x+bottom_right_x,maze->main_surface.y+bottom_right_y,vision_color);
	lineRGBA(brender, maze->main_surface.x + x_pos, maze->main_surface.y+top_view , maze->main_surface.x + x_pos, maze->main_surface.y + bottom_view,vision_color);
	lineRGBA(brender, maze->main_surface.x + left_view, maze->main_surface.y + y_pos, maze->main_surface.x + right_view, maze->main_surface.y + y_pos, vision_color);
	//filledCircleRGBA(brender, maze->main_surface.x + x_pos, maze->main_surface.y + y_pos, mouse_default_size+vision_distance, vision_color);
	




}


int Mouse::detect_point()
{
	int prev_colisions = number_of_colisions;
	int prev_straight_colisions = number_straight_collisions;
	number_of_colisions = 0;
	number_straight_collisions = 0;
	if (x_pos - left_view - size < size * 2)
	{
		number_straight_collisions++;
		number_of_colisions++;
	}
	if (y_pos - top_view - size < size * 2)
	{
		number_straight_collisions++;
		number_of_colisions++;
	}
	if (right_view - x_pos - size < size * 2)
	{
		number_straight_collisions++;
		number_of_colisions++;
	}
	if (bottom_view - y_pos - size < size * 2)
	{
		number_straight_collisions++;
		number_of_colisions++;
	}


	if ((top_right_x - x_pos) - size < size * 2  ) 
		number_of_colisions++;
	if ((x_pos -bottom_left_x) - size < size * 2 )
		number_of_colisions++;
	if ((x_pos - top_left_x) - size < size * 2)
		number_of_colisions++;
	if ((bottom_right_x- x_pos)- size < size * 2 ) 
		number_of_colisions++;

	if (number_of_colisions == 7 && prev_colisions != number_of_colisions)
		return 2;


	if (prev_colisions != number_of_colisions && number_straight_collisions <2 && (number_of_colisions-number_straight_collisions)==4)
	return 1;


	
	
	else 
	return 0;
}



End_Point::End_Point()
{}
End_Point::~End_Point()
{}

bool End_Point::colision_endpoint_maze()
{
	for (int j = x_pos - size; j <= x_pos + size; j++)
	{
		for (int i = y_pos - size; i <= y_pos + size; i++)
		{
			if (maze->array_m[i][j] == '1')
			{
				return 1;

			}

		}


	}

	return 0;


}

bool End_Point::create_end_point(int id, int x, int y, Maze* ref_source,SDL_Event* e, SDL_Window* window, SDL_Renderer* brender)
{ 
	
	
	maze = ref_source;
	char selected = 0;
	if ((e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEWHEEL) && e->window.windowID == SDL_GetWindowID(window))
	{
	
		int x, y;
		SDL_GetMouseState(&x, &y);

		bool inside = true;
		if (x < maze->main_surface.x + size)
			inside = false;
		if (x> maze->main_surface.x + maze->main_surface.w - size)
			inside = false;
		if (y < maze->main_surface.y + size)
			inside = false;
		if (y > maze->main_surface.y + maze->main_surface.h - size)
			inside = false;
		if (inside == true)
		{
			
			x_pos = x - maze->main_surface.x;
			y_pos = y - maze->main_surface.y;
			

			if (!colision_endpoint_maze())
			{

				maze->refresh_maze(brender);
				render_end_point(brender);


				if (e->type == SDL_MOUSEBUTTONDOWN)
				{
					selected = 1;

				}

			}
		}
	
	
	}

	if (selected == 1)
	{
		return 1;
	}

	return 0;

 }

void End_Point::render_end_point(SDL_Renderer* brender)
{
	filledCircleRGBA(brender, maze->main_surface.x + x_pos, maze->main_surface.y + y_pos, size, end_point_color);
}


