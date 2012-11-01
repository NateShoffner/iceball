/*
    This file is part of Buld Then Snip.

    Buld Then Snip is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Buld Then Snip is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Buld Then Snip.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "common.h"

SDL_Surface *screen = NULL;
int screen_width = 800;
int screen_height = 600;

int error_sdl(char *msg)
{
	fprintf(stderr, "%s: %s\n", msg, SDL_GetError());
	return 1;
}

int error_perror(char *msg)
{
	perror(msg);
	return 1;
}

int init_platform(void)
{
	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_NOPARACHUTE))
		return error_sdl("SDL_Init");
	
	return 0;
}

int init_video(void)
{
	SDL_WM_SetCaption("buld then snip",NULL);
	
	screen = SDL_SetVideoMode(screen_width, screen_height, 32, 0);
	
	if(screen == NULL)
		return error_sdl("SDL_SetVideoMode");
	
	return 0;
}

void deinit_video(void)
{
	// don't do anything
}

void deinit_sdl(void)
{
	SDL_Quit();
}

void run_game(void)
{
	map_t *map = map_load_aos("mesa.vxl");
	
	model_t tcam;
	tcam.mpx = 256.5f;
	tcam.mpy = 256.5f;
	tcam.mpz = 256.5f;
	tcam.mxx = 1.0f;
	tcam.mxy = 0.0f;
	tcam.mxz = 0.0f;
	tcam.myx = 0.0f;
	tcam.myy = 1.0f;
	tcam.myz = 0.0f;
	tcam.mzx = 0.0f;
	tcam.mzy = 0.0f;
	tcam.mzz = 1.0f;
	
	SDL_LockSurface(screen);
	memset(screen->pixels, 0x51, screen->h*screen->pitch);
	render_vxl(screen->pixels,
		screen->w, screen->h, screen->pitch/4,
		&tcam, map);
	SDL_UnlockSurface(screen);
	SDL_Flip(screen);
	
	SDL_Delay(1000);
	map_free(map);
}

int main(int argc, char *argv[])
{
	if(!init_platform()) {
	if(!init_video()) {
		run_game();
		deinit_video();
	} deinit_sdl();
	}
	
	return 0;
}