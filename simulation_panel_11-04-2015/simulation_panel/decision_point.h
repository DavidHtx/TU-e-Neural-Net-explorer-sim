#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "configurations.h"
#include "maze.h"

//#define background_color 169,191,230,255

#pragma once
class decision_point
{
public:
	
	int x_pos;
	int y_pos;
	
	int ID;
	int size;
	int times_passed;
	int number_of_connections; 
	char type;   // type  0 -> starting point 1 -> normal point 2 -> dead end 3 -> final point
	bool is_selected;
	bool mouse_over;
	bool is_moved;
	bool directions_used[4] ;
	void generate_decision_point(char type_dec,int id);
	void render_decision_point();
	bool decision_point_events(SDL_Renderer* brender, SDL_Event* e,SDL_Window* window);
	void move_point_events(SDL_Renderer* brender, SDL_Event* e, SDL_Window* window);
	void render_point(SDL_Renderer* brender, int x, int y);
	void delete_point(SDL_Renderer* brender, int x, int y);
	bool point_selected();
	decision_point();
	~decision_point();
};


class connection
{
public:
	int x1;
	int y1;
	int x2;
	int y2;
	int ID;
	int size;
	int times_passed;
	int ID_point1;
	int ID_point2;
	decision_point* point1;
	decision_point* point2;
	connection();
	~connection();

	void create_connection(decision_point* p1, decision_point* p2,int id);
	void render_connection(SDL_Renderer* brender);
	void delete_connection(SDL_Renderer* brender);
};



class maze_points
{
public :
	char type;  //  0 - start point  1 - normal   2 - dead end  3 - end
	bool is_selected;
	int x_pos;
	int y_pos;
	int ID;
	int size;
	Maze *maze;
	decision_point *dec_point;

	void create_maze_point(int id,int x, int y, Maze* m, char typ, decision_point *dec_ref);
	void render_maze_point(SDL_Renderer* brender);
	void maze_point_event(SDL_Renderer* brender);



};