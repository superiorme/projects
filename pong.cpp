#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5\allegro_audio.h>
#include <allegro5\allegro_acodec.h>
#include<allegro5\allegro_ttf.h>
#include<allegro5\allegro_font.h>
#include<iostream>
#include<string>
using namespace std;
int lscore = 0;
int rscore = 0;

bool collision(int b1_x, int b1_y, int b2_x, int b2_y);



int main()
{



	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;
	ALLEGRO_BITMAP * rpaddle = NULL;
	ALLEGRO_BITMAP * lpaddle = NULL;
	ALLEGRO_BITMAP *ball = NULL;
	ALLEGRO_FONT*font = NULL;
	ALLEGRO_SAMPLE *sample3 = NULL;
	ALLEGRO_SAMPLE *sample2 = NULL;
	ALLEGRO_SAMPLE_INSTANCE *instance3 = NULL;
	ALLEGRO_SAMPLE_INSTANCE *instance2 = NULL;
	float rpaddle_x = 575;
	float rpaddle_y = 50;
	float lpaddle_x = 50;
	float lpaddle_y = 50;
	
	float ball_x = 150;
	float ball_y = 150;
	bool keyl[2] = { false, false };//lpaddle
	bool keyr[2] = { false, false };//rpaddle

	float ball_dx = -4.0, ball_dy = 6.0;
	al_install_audio();
	al_init_acodec_addon();
	al_reserve_samples(10);

	sample3 = al_load_sample("pong_music.wav");

	instance3 = al_create_sample_instance(sample3);

	al_set_sample_instance_playmode(instance3, ALLEGRO_PLAYMODE_LOOP);

	al_attach_sample_instance_to_mixer(instance3, al_get_default_mixer());

	al_play_sample_instance(instance3);



	//don't redraw until an event happens
	bool redraw = true;

	//this controls our game loop
	bool doexit = false;


	al_init();
	al_init_font_addon(); // initialize the font addon
	al_init_ttf_addon();// initialize the ttf (True Type Font) addon

	//get the keyboard ready to use
	al_install_keyboard();
	


	sample2 = al_load_sample("Jump.wav");

	instance2 = al_create_sample_instance(sample2);

	al_set_sample_instance_playmode(instance2, ALLEGRO_PLAYMODE_ONCE);

	al_attach_sample_instance_to_mixer(instance2, al_get_default_mixer());

	timer = al_create_timer(.02);

	display = al_create_display(640, 480);
	rpaddle = al_create_bitmap(25, 50);
	lpaddle = al_create_bitmap(25, 50);
	font = al_create_builtin_font();
	ball = al_create_bitmap(32, 32);

	al_set_target_bitmap(rpaddle);
	al_clear_to_color(al_map_rgb(255, 255, 255));

	al_set_target_bitmap(lpaddle);
	al_clear_to_color(al_map_rgb(255, 255, 255));

	al_set_target_bitmap(ball);
	al_clear_to_color(al_map_rgb(255, 255, 255));

	al_set_target_bitmap(al_get_backbuffer(display));

	event_queue = al_create_event_queue();

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

			if (keyr[0] && rpaddle_y >= 0) {
				rpaddle_y -= 4.0;
			}
			if (keyr[1] && rpaddle_y <= 480 - 32) {
				rpaddle_y += 4.0;
			}
			
			//left paddle
			//if the up button is pressed AND we're still below the top wall,
			//move the box "up" by 4 pixels
			//if (keyl[0] && lpaddle_y >= 0) {
				//lpaddle_y -= 4.0;
			//}

			//if the down button is pressed AND we're still above the bottom wall

			//move the box "down" by 4 pixels
			//if (keyl[1] && lpaddle_y <= 480 - 32) {
			//	lpaddle_y += 4.0;
			//}

			if (lpaddle_y < ball_y) {
				cout << "adjusting lpaddle down";
				lpaddle_y += 5.0;
			}
			else if (lpaddle_y > ball_y) {
				cout << "adjusting lpaddle up";
				lpaddle_y -= 5.0;
			}


			//if the box hits the left wall OR the right wall
			if (ball_x < 0) {
				ball_dx = -ball_dx;
				rscore++;
			}
			else if (ball_x > 640 - 32) {
				//flip the x direction
				ball_dx = -ball_dx;
				lscore++;

			}

			//if the box hits the top wall OR the bottom wall
			if (ball_y < 0 || ball_y > 480 - 32) {
				//flip the y direction
				ball_dy = -ball_dy;
			}

			//check collision with left paddle
			//cout << ball_x << " " << ball_y << " " << lpaddle_x << " " << lpaddle_y << endl;

			if (collision(ball_x, ball_y, lpaddle_x, lpaddle_y) == 1) {
				ball_dx = -ball_dx;
				//add sound effect
			

				al_play_sample_instance(instance2);
				//add to score

			}






			//check with right paddle

			if (collision(ball_x, ball_y, rpaddle_x, rpaddle_y) == 1) {
				ball_dx = -ball_dx;

				al_play_sample_instance(instance2);

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
				keyr[0] = true;
				break;

			case ALLEGRO_KEY_DOWN:
				keyr[1] = true;
				break;
				//if the up key has been pressed
			case ALLEGRO_KEY_W:
				keyl[0] = true;
				break;

				//if the down key has been pressed
			case ALLEGRO_KEY_S:
				keyl[1] = true;
				break;
			}
		}
		else if (ev.type == ALLEGRO_EVENT_KEY_UP) {
			switch (ev.keyboard.keycode) {
			case ALLEGRO_KEY_UP:
				keyr[0] = false;
				break;
			case ALLEGRO_KEY_DOWN:
				keyr[1] = false;
				break;
			case ALLEGRO_KEY_W:
				keyl[0] = false;
				break;
			case ALLEGRO_KEY_S:
				keyl[1] = false;
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
			al_draw_textf(font, ALLEGRO_COLOR(al_map_rgb(255, 255, 255)), 320, 40, 1, "score   : %d ", rscore);
			al_draw_textf(font, ALLEGRO_COLOR(al_map_rgb(255, 255, 255)), 320, 40, 1, "  : %d ", lscore);

			al_flip_display();
		}
		
		}
		
	}
	

bool collision(int b1_x, int b1_y, int b2_x, int b2_y) {
		if ((b1_x + 32 < b2_x) || //ball is to the left of the paddle
			(b1_x > b2_x + 25) || //ball is to the right of the paddle
			(b1_y > b2_y + 50) ||  //ball is below paddle
			(b1_y+32 < b2_y))	//ball is above paddle
			return 0;//no collision
		else
			cout << "collision!";
			return 1;//collision!
	}



