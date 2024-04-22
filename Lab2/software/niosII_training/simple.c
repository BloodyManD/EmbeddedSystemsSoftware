#include <stdio.h>
#include <unistd.h>
#include "altera_avalon_pio_regs.h"
#include "system.h"
#include "alt_types.h"
#include "sys/alt_irq.h"
#include "altera_avalon_performance_counter.h"

#define NONE_PRESSED 0xF  // Value read from button PIO when no buttons pressed
#define NONE_PRESSED_IRQ 0x0
#define DEBOUNCE 30000  // Time in microseconds to wait for switch debounce
#define ARRAY_SIZE 510

/* Declare a global variable to hold the edge capture value. */
volatile int edge_capture = 0x0; // set to not enter until interrupt is handled


#ifdef ALT_ENHANCED_INTERRUPT_API_PRESENT
static void handle_button_interrupts(void* context) __attribute__ ((section (".interrupt_handler_section")));
#else
static void handle_button_interrupts(void* context, alt_u32 id) __attribute__ ((section (".interrupt_handler_section")));
#endif


#ifdef ALT_ENHANCED_INTERRUPT_API_PRESENT
static void handle_button_interrupts(void* context)
#else
static void handle_button_interrupts(void* context, alt_u32 id)
#endif
{
PERF_BEGIN(PERFORMANCE_COUNTER_0_BASE, 1);

/* Cast context to edge_capture's type. It is important that this
be declared volatile to avoid unwanted compiler optimization. */
volatile int* edge_capture_ptr = (volatile int*) context;
/*
* Read the edge capture register on the button PIO.
* Store value.
*/
*edge_capture_ptr =
IORD_ALTERA_AVALON_PIO_EDGE_CAP(BUTTONS_BASE);
/* Write to the edge capture register to reset it. */
IOWR_ALTERA_AVALON_PIO_EDGE_CAP(BUTTONS_BASE, 0);
/* Read the PIO to delay ISR exit. This is done to prevent a
spurious interrupt in systems with high processor -> pio
latency and fast interrupts. */
IORD_ALTERA_AVALON_PIO_EDGE_CAP(BUTTONS_BASE);
PERF_END(PERFORMANCE_COUNTER_0_BASE, 1);
}

/* Initialize the button_pio. */
static void init_button_pio()
{
/* Recast the edge_capture pointer to match the
alt_irq_register() function prototype. */
void* edge_capture_ptr = (void*) &edge_capture;
/* Enable all 4 button interrupts. */
IOWR_ALTERA_AVALON_PIO_IRQ_MASK(BUTTONS_BASE, 0xf);
/* Reset the edge capture register. */
IOWR_ALTERA_AVALON_PIO_EDGE_CAP(BUTTONS_BASE, 0x0);
/* Register the ISR. */
#ifdef ALT_ENHANCED_INTERRUPT_API_PRESENT
alt_ic_isr_register(BUTTONS_IRQ_INTERRUPT_CONTROLLER_ID, BUTTONS_IRQ, handle_button_interrupts, edge_capture_ptr, 0x0);
#else
alt_irq_register( BUTTON_PIO_IRQ, edge_capture_ptr, handle_button_interrupts );
#endif
}

int same_adjacent(int* array, int size){
	for(int i = 0; i < size - 1; i++){
		if(array[i] == array[i+1]){
			return i;
		}
	}
	return -1;
}

int same_adjacent4x(int* array, int size){
	int limlong = size >> 2;
	int limshort = size - (size & 0x03);
	for(int i = 0; i < limlong; i++){
		int j = i * 4;
		if(array[j] == array[j + 1]) return j;
		if(array[j + 1] == array[j + 2]) return j + 1;
		if(array[j + 2] == array[j + 3]) return j + 2;
		if(array[j + 3] == array[j + 4]) return j + 3;
	}
	for(int i = limshort; i < size - 1; i++){
		if(array[i] == array[i + 1]) return i;
	}
	return -1;
}




int main(void)
{
	int arr[510] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255, 256, 257, 258, 259, 260, 261, 262, 263, 264, 265, 266, 267, 268, 269, 270, 271, 272, 273, 274, 275, 276, 277, 278, 279, 280, 281, 282, 283, 284, 285, 286, 287, 288, 289, 290, 291, 292, 293, 294, 295, 296, 297, 298, 299, 300, 301, 302, 303, 304, 305, 306, 307, 308, 309, 310, 311, 312, 313, 314, 315, 316, 317, 318, 319, 320, 321, 322, 323, 324, 325, 326, 327, 328, 329, 330, 331, 332, 333, 334, 335, 336, 337, 338, 339, 340, 341, 342, 343, 344, 345, 346, 347, 348, 349, 350, 351, 352, 353, 354, 355, 356, 357, 358, 359, 360, 361, 362, 363, 364, 365, 366, 367, 368, 369, 370, 371, 372, 373, 374, 375, 376, 377, 378, 379, 380, 381, 382, 383, 384, 385, 386, 387, 388, 389, 390, 391, 392, 393, 394, 395, 396, 397, 398, 399, 400, 401, 402, 403, 404, 405, 406, 407, 408, 409, 410, 411, 412, 413, 414, 415, 416, 417, 418, 419, 420, 421, 422, 423, 424, 425, 426, 427, 428, 429, 430, 431, 432, 433, 434, 435, 436, 437, 438, 439, 440, 441, 442, 443, 444, 445, 446, 447, 448, 449, 450, 451, 452, 453, 454, 455, 456, 457, 458, 459, 460, 461, 462, 463, 464, 465, 466, 467, 468, 469, 470, 471, 472, 473, 474, 475, 476, 477, 478, 479, 480, 481, 482, 483, 484, 485, 486, 487, 488, 489, 490, 491, 492, 493, 494, 495, 496, 497, 498, 499, 500, 501, 502, 503, 504, 505, 506, 507, 508, 508};
	alt_icache_flush_all();
	alt_dcache_flush_all();
	int buttons;  // Use to hold button pressed value
	int led = 0x01;  // Use to write to led
	int number_of_runs = 10;

	int accumulator = 0;
	printf("Simple\n");   // print a message to show that program is running

	IOWR_ALTERA_AVALON_PIO_DATA(GREEN_LED_BASE,led); // write initial value to pio
	init_button_pio();

	PERF_RESET(PERFORMANCE_COUNTER_0_BASE);
	PERF_START_MEASURING (PERFORMANCE_COUNTER_0_BASE);
	for(int i = 0; i < 100; i++){
		PERF_BEGIN(PERFORMANCE_COUNTER_0_BASE, 2);
		accumulator += same_adjacent(arr, ARRAY_SIZE);
		PERF_END(PERFORMANCE_COUNTER_0_BASE, 2);
	}
	printf("%d\n", accumulator);
	alt_icache_flush_all();
	alt_dcache_flush_all();
	accumulator = 0;
	for(int i = 0; i < 100; i++){
		PERF_BEGIN(PERFORMANCE_COUNTER_0_BASE, 3);
		accumulator += same_adjacent4x(arr, ARRAY_SIZE);
		PERF_END(PERFORMANCE_COUNTER_0_BASE, 3);
	}
	printf("%d\n", accumulator);

	PERF_STOP_MEASURING(PERFORMANCE_COUNTER_0_BASE);
	perf_print_formatted_report(PERFORMANCE_COUNTER_0_BASE, ALT_CPU_FREQ, 3, "interrupt", "same_adjacent", "same_adjacent4x");

	exit(0);
//	printf("%d", same_adjacent(arr, ARRAY_SIZE));
//	printf("%d", same_adjacent4x(arr, ARRAY_SIZE));
	while (1)
	{
//		buttons = IORD_ALTERA_AVALON_PIO_DATA(BUTTONS_BASE); // read buttons via pio
		buttons = edge_capture; // set buttons to current value of edge capture
		if (buttons != NONE_PRESSED_IRQ)  // if button pressed
		{
			number_of_runs--;
			switch(buttons){
				case 0x1:{ // if KEY0 is pressed
					led = 0x01; // turn on rightmost LED
					break;
				}
				case 0x2:{ // if KEY1 is pressed
					if (led == 0x01) led = 0x80; // if rightmost LED is on move to leftmost
					else led = led >> 1; // else move right
					break;
				}
				case 0x4:{ // if KEY2 is pressed
					if (led == 0x80) led = 0x01; // if leftmost LED is on move to rightmost
					else led = led << 1; // else move left
					break;
				}
				case 0x8:{ // if KEY3 is pressed
					led = 0x80; // turn on leftmost LED
					break;
				}
			}
			if(!number_of_runs){
				PERF_STOP_MEASURING(PERFORMANCE_COUNTER_0_BASE);
				perf_print_formatted_report(PERFORMANCE_COUNTER_0_BASE, ALT_CPU_FREQ, 3, "interrupt", "TBD1", "TBD2");
			}

//			if (buttons == 0x7) led = 0x80; // if KEY3 is pressed turn on leftmost LED
//			if (buttons == 0xb){ // if KEY2 is pressed
//				if (led == 0x80) led = 0x01; // if leftmost LED is on move to rightmost LED
//				else led = led << 1; // else move left
//			}
//			if (buttons == 0xd){ // if KEY1 is pressed
//				if (led == 0x01) led = 0x80; // if rightmost LED is on move to leftmost LED
//				else led = led >> 1; // else move right
//			}
//			if (buttons == 0xe) led = 0x01; // if KEY0 is pressed turn on leftmost LED



//			if (led >= 0x80)  // if pattern is 00000001 on board (leds in reverse order)
//				led = 0x01;  // reset pattern
//			else
//				led = led << 1;  // shift right on board (led0 is far left)

			IOWR_ALTERA_AVALON_PIO_DATA(GREEN_LED_BASE,led);  // write new value to pio

			/* Switch debounce routine
			   Wait for small delay after intial press for debouncing
			   Wait for release of key
			   Wait for small delay after release for debouncing */

//			usleep (DEBOUNCE);
//			while (buttons != NONE_PRESSED)  // wait for button release
//				buttons = IORD_ALTERA_AVALON_PIO_DATA(BUTTONS_BASE);   // update
//			usleep (DEBOUNCE);
			edge_capture = 0x0; // reset edge_capture until next interrupt
		}

	}

}  // end

