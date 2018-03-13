#include <avr/io.h>
#include <util/delay.h>

/**
	Function that will turn on/off the 7s leds. Receive the 10 by 3 matrix
	which refers to bit state to turn the leds and represent the second parameter
	in the function, the "digit".
*/
void writeDigit(short digitMatrix[][3], int digit){
	// The first column will have only the PORTB states, so we can just put like this
	PORTB  = digitMatrix[digit][0];
	// However, PORTD contains one bit information that still represent one of the leds,
	// and others states from the transistors. In this case, we don't know which state
	// the transistors are, thus we can't simply put the bit values in the PORTD.
	// The second matrix's column have the bits which need to be turned on. Using
	// OR operation, we can turn on the led we want and leave the others just as they where before
	// Here, 0 means: leave the same value as before, and 1 means to change
	PORTD |= digitMatrix[digit][1];
	// The second expression, using a AND operator, turn off all wanted leds
	// Here, 1 means: leave the same, and 0: to change
	PORTD &= digitMatrix[digit][2];
}

int main (void)
{	
	// States definition according hardware sketch; can be diferent if not
	// using the same configuration as shown in DDRB and DDRD
	// Since I'm using common anode 7s, 1 means off and 0 means on
	// Don't confuse with the transistor DDRDxTTxxxxx, they still use 1 as on and 0 as off 
	short digits[10][3] = {{0xe0,0x1f,0x7f},	// 0	{0b11100000,0b00011111,0b01111111},
						   {0xfc,0x9f,0xff},	// 1	{0b11111100,0b10011111,0xff},
						   {0xd2,0x1f,0x7f},	// 2	{0b11010010,0b00011111,0b01111111},
						   {0xd8,0x1f,0x7f},	// 3	{0b11011000,0b00011111,0b01111111},
						   {0xcc,0x9f,0xff},	// 4	{0b11001100,0b10011111,0xff},
						   {0xc9,0x1f,0x7f},	// 5	{0b11001001,0b00011111,0b01111111},
						   {0xc1,0x1f,0x7f},	// 6	{0b11000001,0b00011111,0b01111111},
						   {0xfc,0x1f,0x7f},	// 7	{0b11111100,0b00011111,0b01111111},
						   {0xc0,0x1f,0x7f},	// 8	{0b11000000,0b00011111,0b01111111},
						   {0xcc,0x1f,0x7f}};	// 9	{0b11001100,0b00011111,0b01111111}

	// DDRB Ports. 					   
	//		   GFEDCB =		7s pins
	DDRB = 0x3f;	// 0b00111111
	//		 A12			Where A is the last remaining pin from the 7s and the 
	//						1 & 2 referes to the first and second transistor.		
	DDRD = 0xe0;	// 0b11100000

	// Counting from 0 to 99 test
	for(int i = 0; i < 100; i++){
		// To keep the number showing for a litte time
		for(int k = 0; k < 5; k++){ // should be millis() - lastChange, but at this time I still don't know how to implemet millis() :)
			// Turn on the first transistor
			PORTD |= 0x40;	// 0b01000000
			// Turn off the last one
			PORTD &= 0xdf;	// 0b11011111
			writeDigit(digits, i / 10);
			_delay_ms(30);

			// Turn on the second transistor
			PORTD |= 0x20;	// 0b00100000
			// Turn off the first
			PORTD &= 0xbf;	// 0b10111111
			writeDigit(digits, i % 10);
			_delay_ms(30);
		}
	}
	
	// Finish :) turn off all leds; Not a elegant why, but...
	PORTB = 0xff;
	PORTD = 0xff;
	
	return 0;
}
