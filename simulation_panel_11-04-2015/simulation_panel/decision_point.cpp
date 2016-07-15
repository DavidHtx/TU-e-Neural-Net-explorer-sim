#include "decision_point.h"
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "SDL2_gfxPrimitives.h"
#include <stdio.h>

decision_point::decision_point()
{
}


decision_point::~decision_point()
{
}

connection::connection()
{
	
}

connection::~connection()
{
}


void connection::create_connection(decision_point* p1, decision_point* p2,int id)
{
	point1 = p1;
	point2 = p2;
	x1 = p1->x_pos;
	y1 = p1->y_pos;
	x2 = p2->x_pos;
	y2 = p2->y_pos;
	p1->number_of_connections++;
	p2->number_of_connections++;
	ID_point1 = p1->ID;
	ID_point2 = p2->ID;
	times_passed = 0;
	ID = id;
	size = 1;
	printf("Connection with ID : %d created between point %d and point %d\n",ID,ID_point1,ID_point2);
}

void connection::render_connection(SDL_Renderer* brender)
{
	x1 = point1->x_pos;
	y1 = point1->y_pos;
	x2 = point2->x_pos;
	y2 = point2->y_pos;
	SDL_SetRenderDrawColor(brender, 0,0,0,255);
	SDL_RenderDrawLine(brender,x1,y1,x2,y2);
	//thickLineRGBA(brender, x1,y1,x2,y2,5,0,255,255,255);
}

void connection::delete_connection(SDL_Renderer* brender)
{
	x1 = point1->x_pos;
	y1 = point1->y_pos;
	x2 = point2->x_pos;
	y2 = point2->y_pos;
	SDL_SetRenderDrawColor(brender, background_color);
	SDL_RenderDrawLine(brender, x1, y1, x2, y2);

}

void decision_point::generate_decision_point(char type_dec,int id)
{
	type = type_dec;
	ID = id;
	times_passed = 0;
	number_of_connections = 0;
	is_selected = false;
	size = 5;
	printf("Decision point created with ID : %d type: ", ID);
	if (type == 0)
		printf("starting point\n");
	else if (type == 1)
		printf("normal point\n");
	else if (type ==2)
		printf("dead end\n");
	else if (type == 3)
		printf("final point\n");

}

void decision_point::delete_point(SDL_Renderer* brender, int x, int y)
{
	filledCircleRGBA(brender, x_pos, y_pos, size + 3, background_color);
	
}

void decision_point::render_point(SDL_Renderer* brender, int x, int y)
{  
	x_pos = x;
	y_pos = y;
  	filledCircleRGBA(brender, x_pos, y_pos, size + 3,background_color);
	if (is_selected)
	{
		filledCircleRGBA(brender, x_pos, y_pos, size + 3, point_selected_color);
	}
	if      (type == 0)
		filledCircleRGBA(brender, x_pos, y_pos, size, start_point_color);
	else if (type == 1)
		filledCircleRGBA(brender, x_pos, y_pos, size, default_point_color);
	else if (type == 2)
		filledCircleRGBA(brender, x_pos, y_pos, size, dead_point_color);
	else if (type == 3)
		filledCircleRGBA(brender, x_pos, y_pos, size, end_point_color);

	
}

void decision_point::move_point_events(SDL_Renderer* brender, SDL_Event* e, SDL_Window* window)
{ 
	
	if (is_selected)// && mouse_over)
	{
		int x;
		int y;
		filledCircleRGBA(brender, x_pos, y_pos, size + 3, background_color);
			 SDL_GetMouseState(&x, &y);
			 x_pos = x;
			 y_pos = y; 

			 if (y_pos < 50)
				 y_pos = 50;
                         
			 render_point(brender, x_pos, y_pos);
			
			
	
	}
 
}

bool decision_point::decision_point_events(SDL_Renderer* brender, SDL_Event* e, SDL_Window* window)
{ 
	
	if ((e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEMOTION) && e->window.windowID == SDL_GetWindowID(window))
	{   
		int x, y;
     	SDL_GetMouseState(&x, &y);
		bool inside = true;
		if (x < x_pos - size)
		{
			inside = false;
		}
		else if (x> x_pos + size)
		{
			inside = false;
		}
		else if (y < y_pos - size)
		{
			inside = false;
		}
	
		else if (y>y_pos + size)
		{ 
			inside = false;
		}
		if (inside == true)
		{
			mouse_over = true;
			if (e->type == SDL_MOUSEBUTTONDOWN)
			{   
				if (is_selected == false)
					is_selected = true;
				else
					is_selected = false;

				render_point(brender,x_pos,y_pos);
				return 1;
				}
					
		
		}
		if (inside != true)
			mouse_over = false;
		if (inside != true && e->type == SDL_MOUSEBUTTONDOWN)
		{
			is_selected = false;
			render_point(brender, x_pos, y_pos);
						return 0;
		}


	}
 
}


void maze_points::create_maze_point(int id,int x, int y, Maze* m, char typ,decision_point *dec_ref)
{
	ID = id;
	x_pos = x;
	y_pos = y;
	maze = m;
	type = typ;
	dec_point = dec_ref;
	size = maze_point_size;
}

void maze_points::render_maze_point(SDL_Renderer* brender)
{
	if (is_selected == 1)
		filledCircleRGBA(brender, x_pos + maze->main_surface.x, y_pos + maze->main_surface.y, size + 2, maze_point_selected);
	else 
		filledCircleRGBA(brender, x_pos + maze->main_surface.x, y_pos + maze->main_surface.y, size + 2, maze_background_color);

	if (type == 0)
	filledCircleRGBA(brender, x_pos+maze->main_surface.x, y_pos+maze->main_surface.y, size, start_point_color);
	else if (type == 1) 
    filledCircleRGBA(brender, x_pos + maze->main_surface.x, y_pos + maze->main_surface.y, size, default_point_color);
	else if (type == 2)
	filledCircleRGBA(brender, x_pos + maze->main_surface.x, y_pos + maze->main_surface.y, size, dead_point_color);
	else if (type == 3)
	filledCircleRGBA(brender, x_pos + maze->main_surface.x, y_pos + maze->main_surface.y, size, end_point_color);

}

void maze_points::maze_point_event(SDL_Renderer* brender)
{ 
	if (dec_point->is_selected == 1)
		is_selected = 1;
	else is_selected = 0;
	render_maze_point(brender);


}


