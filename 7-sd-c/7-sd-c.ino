
#include <avr/io.h>
#include <util/delay.h>

void escreverDigito(short matriz[][3], int digito){
	
	//digitalWrite(ports[i], digitos[digit][i] == !default_off_value);
	PORTB = matriz[digito][0];
	PORTD |= matriz[digito][1];
	PORTD &= matriz[digito][2];
}

int main (void)
{
	short digitos[10][3] = {{0b11100000,0b00011111,0b01111111},	// 0
							{0b11111100,0b10011111,0xff},		// 1
							{0b11010010,0b00011111,0b01111111},	// 2
							{0b11011000,0b00011111,0b01111111},	// 3
							{0b11001100,0b10011111,0xff},		// 4
							{0b11001001,0b00011111,0b01111111},	// 5
							{0b11000001,0b00011111,0b01111111},	// 6
							{0b11111100,0b00011111,0b01111111},	// 7
							{0b11000000,0b00011111,0b01111111},	// 8
							{0b11001100,0b00011111,0b01111111}};// 9

	//		   gfedcb
	DDRB = 0b00111111;
	//		 a12				
	DDRD = 0b11100000;

	PORTD = 0b01000000;

	for(int i = 0; i < 10; i++){
		escreverDigito(digitos, i);

		_delay_ms(500);
	}

	for(int i = 9; i >= 0; i--){
		escreverDigito(digitos, i);

		_delay_ms(500);
	}
	
	PORTB = 0xff;
	PORTD = 0xff;
	

	return 0;
}
