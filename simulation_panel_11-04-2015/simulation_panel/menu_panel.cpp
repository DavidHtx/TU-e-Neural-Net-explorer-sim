#include <stdio.h>
#include "menu_panel.h"
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include <string>

menu_panel::menu_panel()
{

}

menu_panel::~menu_panel()
{
}

information_panel::information_panel()
{}
information_panel::~information_panel()
{}

menu_button::menu_button(int b_x, int b_y, int b_h, int b_w,char* text)
{   

	button_height = b_h;
	button_width = b_w;
	button_x_pos = b_x;
	button_y_pos = b_y;
	text_button = text;

}

menu_button::~menu_button()
{
}

void menu_panel::create(SDL_Renderer* renderer, SDL_Window* window, int window_width,int panel_height)
{

	w_width = window_width;
	SDL_Rect panel_background = { 0, 0, window_width, panel_height };
	SDL_Rect panel_background_2 = { 5, 5, window_width - 10, panel_height - 10 };
	SDL_Rect button_background;
	SDL_Rect button_main_color;
	SDL_SetRenderDrawColor(renderer, panel_primary_color);
	SDL_RenderFillRect(renderer, &panel_background);
	SDL_SetRenderDrawColor(renderer, panel_secondary_color);
	SDL_RenderFillRect(renderer, &panel_background_2);

	
}
void menu_button::render_button(SDL_Renderer* brender, char state)
{
	SDL_Color button_col = { default_button_text_color};
	const Uint8 color_r = 30;
	const Uint8 color_g = 153;
	const Uint8 color_b = 190;
	TTF_Font* theFont = TTF_OpenFont("theFont.ttf", 14);
	SDL_Surface* text_surface = TTF_RenderText_Solid(theFont,text_button, button_col);


	SDL_Texture* text_texture = SDL_CreateTextureFromSurface(brender, text_surface);
	int textw = text_surface -> w;
	int texth = text_surface -> h;
	SDL_FreeSurface(text_surface);

	SDL_Rect text_position = { button_x_pos + button_width/5, button_y_pos + button_height/10, textw,texth };

	SDL_Rect button_size = { button_x_pos, button_y_pos,button_width, button_height };
	
	if (state==0)
	SDL_SetRenderDrawColor(brender, button_default_color);
	else if(state==1)
    SDL_SetRenderDrawColor(brender, default_mouse_on_button_color);

	SDL_RenderFillRect(brender, &button_size);

	SDL_RenderCopy(brender, text_texture, NULL, &text_position);
	SDL_DestroyTexture(text_texture);
	//SDL_RenderPresent(brender);
	TTF_CloseFont(theFont);
	
	//SDL_RenderPresent(brender);

}
bool menu_button::button_event(SDL_Event* e, SDL_Renderer* brender, SDL_Window* wind)
{
	bool ref = false;
	if ((e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type==SDL_MOUSEBUTTONUP) && e->window.windowID == SDL_GetWindowID(wind)) //|| e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP)
	{
		int x, y;
	
		SDL_GetMouseState(&x, &y);
		bool inside = true;
		if (x < button_x_pos)
		{
			inside = false;
		}
		else if (x > button_x_pos + button_width)
		{
			inside = false;
		}
		else if (y < button_y_pos)
		{
			inside = false;
		}
		else if (y > button_y_pos + button_height)
		{
			inside = false;
		}
		if (inside==true)
		{
			
			if 	(e->type == SDL_MOUSEMOTION)
			render_button(brender,1);
			
			if (e->type == SDL_MOUSEBUTTONDOWN)
				ref = 1;

		}
		else
		{
			render_button(brender, 0);
			ref = 0;
		}
		

	}
	return ref;
}


void information_panel::initialize_font()
{
	theFont = TTF_OpenFont("theFont.ttf", 18);

}

void information_panel::render_info_panel(SDL_Renderer* prender,int x, int y, int w, int h)
{ 
	 info_panel_width = w;
	 info_panel_height = h;
	 info_panel_x_pos = x;
	 info_panel_y_pos = y;
	SDL_Rect panel_surface = { x, y, w, h };
	SDL_Rect panel_surface_2 = { x + info_panel_margins, y + info_panel_margins, w - 2 * info_panel_margins, h - 2 * info_panel_margins };
	SDL_SetRenderDrawColor(prender, default_info_panel_color);
	SDL_RenderFillRect(prender, &panel_surface);
	SDL_SetRenderDrawColor(prender, secondary_info_panel_color);
	SDL_RenderFillRect(prender, &panel_surface_2);
}

void information_panel::write_text(SDL_Renderer* trender,int line, char* text)
{
	
	SDL_Color info_text_color = {info_panel_text_color};
	SDL_Surface* text_surface = TTF_RenderText_Solid(theFont, text, info_text_color);
	SDL_Texture* text_texture = SDL_CreateTextureFromSurface(trender, text_surface);
	int textw = text_surface->w;
	int texth = text_surface->h;
	SDL_FreeSurface(text_surface);
	SDL_Rect text_position = {info_panel_x_pos+10,info_panel_y_pos+5+line*20,textw,texth};
	SDL_RenderCopy(trender, text_texture, NULL, &text_position);

}



