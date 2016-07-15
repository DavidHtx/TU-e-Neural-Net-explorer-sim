#include "SDL.h"
#include "stdio.h"
#include <string>
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "configurations.h"
#pragma once




class menu_panel
{
public:
	menu_panel();
	~menu_panel();
	
 	const int height = 30;
	int w_width = 200;

	void create(SDL_Renderer* renderer,SDL_Window* window, int window_width,int panel_height);
	

};

class menu_button : public menu_panel
{
public:
	menu_button(int b_h, int b_w, int b_x, int b_y,char* text);
	~menu_button();
	
	int button_height;
	int button_width;
	int button_x_pos;
	int button_y_pos;
	char* text_button ;
	TTF_Font* theFont ;
	bool button_event(SDL_Event* e, SDL_Renderer* brender,SDL_Window* wind);
	void render_button(SDL_Renderer* brender, char state);



};
class information_panel
{

    public : 
		information_panel();
		~information_panel();
	
		int info_panel_width; 
		int info_panel_height; 
		int info_panel_x_pos;
		int info_panel_y_pos;
		SDL_Surface* text_surface;
		TTF_Font* theFont;
		void render_info_panel(SDL_Renderer* prender,int x, int y, int w, int h);
		void write_text(SDL_Renderer* trender,int line, char* text);
		void initialize_font();


};