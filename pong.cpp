#include <stdio.h>
#include <allegro5/allegro.h>


int main()
{
	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;
	ALLEGRO_BITMAP * rpaddle= NULL;
	ALLEGRO_BITMAP * lpaddle = NULL;
	ALLEGRO_BITMAP *ball = NULL;



	//these two variables hold the x and y positions of the square
	//initalize these variables to where you want your square to start
	float rpaddle_x = 575;
	float rpaddle_y = 50;


	float lpaddle_x = 150;
	float lpaddle_y = 50;

	float ball_x = 150;
	float ball_y = 150;
	//here's our key states. they're all starting as "false" because nothing has been pressed yet.
	//the first slot represents "up", then "down", "left" and "right"
	bool key[4] = { false, false, false, false };
	float ball_dx = -4.0, ball_dy = 4.0;
	//don't redraw until an event happens
	bool redraw = true;

	//this controls our game loop
	bool doexit = false;

	al_init();

	//get the keyboard ready to use
	al_install_keyboard();

	timer = al_create_timer(.02);

	display = al_create_display(640, 480);

	rpaddle = al_create_bitmap(25,50);
	
	rpaddle = al_create_bitmap(25, 50);
	
	ball = al_create_bitmap(32, 32);

	al_set_target_bitmap(rpaddle);
	al_clear_to_color(al_map_rgb(255, 255, 255));

	al_set_target_bitmap(lpaddle);
	al_clear_to_color(al_map_rgb(255, 255, 255));

	al_set_target_bitmap(ball);
	al_clear_to_color(al_map_rgb(255, 255, 255));

	al_set_target_bitmap(al_get_backbuffer(display));

	event_queue = al_create_event_queue();

	//these lines tell teh event source what to look for
	al_register_event_source(event_queue, al_get_display_event_source(display));

	al_register_event_source(event_queue, al_get_timer_event_source(timer));

	//new! tell the event queue that it should take keyboard events, too 
	al_register_event_source(event_queue, al_get_keyboard_event_source());

	al_clear_to_color(al_map_rgb(0, 0, 0));

	al_flip_display();

	al_start_timer(timer);

	//so the game loop is set to act on "ticks" of the timer OR keyboard presses 
	//OR the mouse closing the display
	while (!doexit)
	{
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);

		//////////////////////////////////////////////////////////////////////////////////////////////////
		//here's the movement algorithm

		if (ev.type == ALLEGRO_EVENT_TIMER) {
			//if the up button is pressed AND we're still below the top wall,
			//move the box "up" by 4 pixels
			if (key[0] && rpaddle_y >= 0) {
				rpaddle_y -= 4.0;
			}

			//if the down button is pressed AND we're still above the bottom wall
			//move the box "down" by 4 pixels
			if (key[1] && rpaddle_y <= 480 - 32) {
				rpaddle_y += 4.0;
			}
			//if the left button is pressed AND we're still right of the left wall
			//move the box left by 4 pixels
			if (key[2] && rpaddle_x >= 0) {
				rpaddle_x -= 4.0;
			}

			//if the left button is pressed AND we're still left of the right wall
			//move the box right by 4 pixels
			if (key[3] && rpaddle_x <= 640 - 32) {
				rpaddle_x += 4.0;
			}

			//redraw at every tick of the timer
			redraw = true;
		}

		//left paddle

		if (ev.type == ALLEGRO_EVENT_TIMER) {
			//if the up button is pressed AND we're still below the top wall,
			//move the box "up" by 4 pixels
			if (key[0] && lpaddle_y >= 0) {
				lpaddle_y -= 4.0;
			}

			//if the down button is pressed AND we're still above the bottom wall
			//move the box "down" by 4 pixels
			if (key[1] && lpaddle_y <= 480 - 32) {
				lpaddle_y += 4.0;
			}
			//if the left button is pressed AND we're still right of the left wall
			//move the box left by 4 pixels
			if (key[2] && lpaddle_x >= 0) {
				lpaddle_x -= 4.0;
			}

			//if the left button is pressed AND we're still left of the right wall
			//move the box right by 4 pixels
			if (key[3] && lpaddle_x <= 640 - 32) {
				lpaddle_x += 4.0;
			}

			//redraw at every tick of the timer
			redraw = true;
		}

		//ball

		//sets this part of the game to run on the clicks of the timer 
		if (ev.type == ALLEGRO_EVENT_TIMER) {
			//if the box hits the left wall OR the right wall
			if (ball_x < 0 || ball_x > 640 - 32) {
				//flip the x direction
				ball_dx = -ball_dx;
			}
			//if the box hits the top wall OR the bottom wall
			if (ball_y < 0 || ball_y > 480 - 32) {
				//flip the y direction
				ball_dy = -ball_dy;
			}

			//really important code!
			//move the box in a diagonal
			ball_x += ball_dx;
			ball_y += ball_dy;

			//if an event happened, you better redraw
			redraw = true;
		}




		///////////////////////////////////////////////////////////////////////////////////////////////////////
		else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			break;
		}

		//////////////////////////////////////////////////////////////////////////////////////////////////////
		//here's the algorithm that turns key presses into events
		//a "key down" event is when a key is pushed
		//while a "key up" event is when a key is released

		//has something been pressed on the keyboard?
		else if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {

			//"keycode" holds all the different keys on the keyboard
			switch (ev.keyboard.keycode) {

				//if the up key has been pressed
			case ALLEGRO_KEY_UP:
				key[0] = true;
				break;

				//if the down key has been pressed
			case ALLEGRO_KEY_DOWN:
				key[1] = true;
				break;
			case ALLEGRO_KEY_W:
				key[0] = true;
				break;

				//if the down key has been pressed
			case ALLEGRO_KEY_S:
				key[1] = true;
				break;

			}
		}
		//has something been released on the keyboard?
		else if (ev.type == ALLEGRO_EVENT_KEY_UP) {
			switch (ev.keyboard.keycode) {
			case ALLEGRO_KEY_UP:
				key[0] = false;
				break;

			case ALLEGRO_KEY_DOWN:
				key[1] = false;
				break;

			case ALLEGRO_KEY_W:
				key[0] = false;
				break;

			case ALLEGRO_KEY_S:
				key[1] = false;
				break;

				//kill the program if someone presses escape
			case ALLEGRO_KEY_ESCAPE:
				doexit = true;
				break;
			}
		}


		//RENDER SECTION
		//if the clock ticked but no other events happened, don't bother redrawing
		if (redraw && al_is_event_queue_empty(event_queue)) {
			redraw = false;

			//paint black over the old screen, so the old square dissapears
			al_clear_to_color(al_map_rgb(0, 0, 0));

			//the algorithm above just changes the x and y coordinates
			//here's where the bitmap is actually drawn somewhere else
			al_draw_bitmap(rpaddle, rpaddle_x, rpaddle_y, 0);
			al_draw_bitmap(lpaddle, lpaddle_x, lpaddle_y, 0);
			al_draw_bitmap(ball, ball_x, ball_y, 0);



			al_flip_display();
		}
	}

	al_destroy_bitmap(rpaddle);
	al_destroy_bitmap(lpaddle);
	al_destroy_bitmap(ball);
	al_destroy_timer(timer);
	al_destroy_display(display);
	al_destroy_event_queue(event_queue);

	return 0;
}


