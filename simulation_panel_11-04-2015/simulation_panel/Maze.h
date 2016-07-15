
#include <stdio.h>
#include "menu_panel.h"
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "configurations.h"

#pragma once





class Maze
{
public:
	int maze_panel_width;
	int maze_panel_height;
	int maze_panel_x_pos;
	int maze_panel_y_pos;
	char array_m[2000][2000];
	bool moving;
	bool mouse_on_icon;
	SDL_Surface* maze_file_surface;
	SDL_Rect icon_pos;
	SDL_Rect margin_surface;
	SDL_Rect main_surface;
	SDL_Texture* maze_txt;
	SDL_Texture* icon_texture;
	Maze();
	~Maze();
	void render_maze(SDL_Renderer* brender, int x, int y, int w, int h);
	void maze_events(SDL_Renderer* brender, SDL_Event *e, SDL_Window*window);
	void refresh_maze(SDL_Renderer* brender);
	void maze_to_array();
	Uint32 getpixel(SDL_Surface *surface, int x, int y);
};


class Mouse
{
public : 
	Mouse();
	~Mouse();
	int x_pos;
	int y_pos;
	int size = mouse_default_size;
	int current_dec_ID;
	int ID;
	Maze* maze;
	int number_of_colisions;
	int number_straight_collisions;
	int left_view ;
	int right_view;
	int bottom_view;
	int top_view;
	int bottom_left_x;
	int bottom_left_y;
	int bottom_right_x;
	int bottom_right_y;
	int top_left_x;
	int top_left_y;
	int top_right_x;
	int top_right_y;
	
	int maze_points[5];  // 0 - up 1 - right 2 - left 3 - down //   0 - nothing 1 - end point 2 - normal point 3 - finish point 4 - start point
	void colision_vision_points();
	bool create_mouse(int id, int x, int y, Maze* ref_source, SDL_Event* e, SDL_Window* window, SDL_Renderer* brender);
	void render_mouse(SDL_Renderer* brender );
	void delete_mouse(SDL_Renderer* brender);
	bool colision_mouse_maze();
	void render_vision(SDL_Renderer* brender);
	int detect_point();  //  returns type of decision point that has to be created
};

class End_Point
{
public : 
	int x_pos;
	int y_pos;
	int size = end_point_default_size;
	Maze* maze;
	End_Point(); 
	~End_Point();
	void render_end_point(SDL_Renderer* brender);
	bool create_end_point(int id, int x, int y, Maze* ref_source, SDL_Event* e, SDL_Window* window, SDL_Renderer* brender);
	bool colision_endpoint_maze();
}
;
