#include "ws2811.h"
#include "board_info.h"
#include "anim.h"

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <signal.h>

/* Unicorn Hat */
#define TARGET_FREQ	WS2811_TARGET_FREQ
#define GPIO_PIN	18
#define DMA		5
#define WIDTH		8
#define HEIGHT		8
#define LED_COUNT	(WIDTH * HEIGHT)

/* ANSI Colors */
#define COLOR_GREEN     "\x1b[1;32m\x1b[42m" /* intense green on green */
#define COLOR_GRAY      "\x1b[1;30m" /* intense black, codename for gray */
#define COLOR_RESET     "\x1b[0m"


/* Init matrix */
ws2811_t ledstring =
{
	.freq = TARGET_FREQ,
	.dmanum = DMA,
	.channel =
	{
		[0] =
		{
			.gpionum    = GPIO_PIN,
			.count      = LED_COUNT,
			.invert     = 0,
			.brightness = 10,
		}
	}
};


/* Returns pixel id (integer) */
int getPixelPosition(int x, int y){

	int map[8][8] = {
		{7 ,6 ,5 ,4 ,3 ,2 ,1 ,0 },
		{8 ,9 ,10,11,12,13,14,15},
		{23,22,21,20,19,18,17,16},
		{24,25,26,27,28,29,30,31},
		{39,38,37,36,35,34,33,32},
		{40,41,42,43,44,45,46,47},
		{55,54,53,52,51,50,49,48},
		{56,57,58,59,60,61,62,63}
	};

	return map[x][y];
}

/* Draw pixel */
void setPixelColorRGB(int pixel, int r, int g, int b)
{
	ledstring.channel[0].leds[pixel] = (r << 16) | (g << 8) | b;
	return;
}

void clearLEDBuffer(void){
	int i;
	for(i=0; i<LED_COUNT; i++){
		setPixelColorRGB(i,0,0,0);
	}
}


/* Clear the display and exit gracefully */
void unicorn_exit(int status){
	int i;
	for (i=0; i<64; i++){
		setPixelColorRGB(i,0,0,0);
	}
	ws2811_render(&ledstring);
	ws2811_fini(&ledstring);
        fputs(COLOR_RESET, stdout);
	exit(status);
}


int main() {
	int i;
	for (i = 0; i < 64; i++) {
		struct sigaction sa;
		memset(&sa, 0, sizeof(sa));
		sa.sa_handler = unicorn_exit;
		sigaction(i, &sa, NULL);
	}

	setvbuf(stdout, NULL, _IONBF, 0);

	if (board_info_init() < 0)
	{
		return -1;
	}

	if(ws2811_init(&ledstring))
	{
		return -1;
	}

	  uint8_t mask, y; /* these fit in 1 byte */ 

	  while (1) {
                i = 0;
	        /* loop for each byte in anim[] (anim.h) */
	      while(i<sizeof(anim)) {

		  /* clear screen and LED matrix */
		  printf("\033[2J\033[H");
		  clearLEDBuffer();

		  for(y=0; y<8; y++) {

		      /* for each bit in our byte */
                      for(mask = 1<<7; mask != 0; mask >>= 1) {
                        if(anim[i] & mask) {
                           fputs(COLOR_GREEN "1" COLOR_RESET, stdout);
                           switch(mask) {
                              case 128 : setPixelColorRGB(getPixelPosition(0,y), 166, 166, 42); break; /* yellow */
                              case 64  : setPixelColorRGB(getPixelPosition(1,y),  52, 128, 42); break; /* green */
                              case 32  : setPixelColorRGB(getPixelPosition(2,y),  52, 128, 42); break;
                              case 16  : setPixelColorRGB(getPixelPosition(3,y),  52, 128, 42); break;
                              case 8   : setPixelColorRGB(getPixelPosition(4,y),  52, 128, 42); break;
                              case 4   : setPixelColorRGB(getPixelPosition(5,y),  52, 128, 42); break;
                              case 2   : setPixelColorRGB(getPixelPosition(6,y),  52, 128, 42); break;
                              case 1   : setPixelColorRGB(getPixelPosition(7,y), 166, 166, 42); break; /* yellow */
                           }
                        }  else fputs(COLOR_GRAY "0" COLOR_RESET, stdout);
                      }

                      fputs("\n", stdout);
                      i++; /* move to next byte in anim[] */
                  } /* we have composited one full frame */

                  /* light up the LEDs with ws2811 lib and render frame */
		  ws2811_render(&ledstring);
                  /* every 9th byte is our frame duration */
		  usleep(anim[i++]*6000);
	      }
	  }
	  unicorn_exit(0);
	  return 0;
}
