#include<iostream>
#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>


int main(){
        
	ALLEGRO_DISPLAY *display = NULL;
		
		al_init();

		al_init_primitives_addon();

		display = al_create_display(640, 480);

		al_clear_to_color(al_map_rgb(0, 0, 0));
		


		al_draw_triangle(100, 150, 160, 65, 80, 90, al_map_rgb(0, 100, 6), 20);

		al_draw_triangle(160, 120, 180, 40, 70, 75, al_map_rgb(0, 5, 50), 10);

		al_draw_triangle(190, 160, 125, 20, 30, 50, al_map_rgb(0, 50, 90), 10);
		
		
		




		al_flip_display();




		al_rest(10.0);




		al_destroy_display;

		

		return 0;


	}