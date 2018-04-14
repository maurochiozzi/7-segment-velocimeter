/*
	Code for seven segments velocimeter.
	Feel free to use and edit.

	Mauro Chiozzi 
*/

// States definition according hardware sketch; can be diferent if not
// using the same configuration as shown in DDRB and DDRD
// Since I'm using common anode 7s, 1 means off and 0 means on
// Don't confuse with the transistor DDRDxTTxxxxx, they still use 1 as on and 0 as off 
short digits[10][3] = {{0xe0,0b00000000,0x7f},	// 0	{0b11100000,0b00000000,0b01111111},
					   {0xfc,0b10000000,0xff},	// 1	{0b11111100,0b10000000,0xff},
					   {0xd2,0b00000000,0x7f},	// 2	{0b11010010,0b00000000,0b01111111},
					   {0xd8,0b00000000,0x7f},	// 3	{0b11011000,0b00000000,0b01111111},
					   {0xcc,0b10000000,0xff},	// 4	{0b11001100,0b10000000,0xff},
					   {0xc9,0b00000000,0x7f},	// 5	{0b11001001,0b00000000,0b01111111},
					   {0xc1,0b00000000,0x7f},	// 6	{0b11000001,0b00000000,0b01111111},
					   {0xfc,0b00000000,0x7f},	// 7	{0b11111100,0b00000000,0b01111111},
					   {0xc0,0b00000000,0x7f},	// 8	{0b11000000,0b00000000,0b01111111},
					   {0xcc,0b00000000,0x7f}};	// 9	{0b11001100,0b00000000,0b01111111}

// Velocity
const short e = 250; 							// time interval to stop reading velocity sinal and do the math to calculate de average velocity every e-time
long volatile dt_time = 0;						// time difference (delta t) between 2 velocity sinal
long volatile last_signal_time = 0;				// when the last sinal was read
float volatile velKh = 0;						// Velocity in km / h
int average_vel;								// this average velocity is has a too low dt depending on the velocity that can be consired as instant velocity
int volatile count_vel = 0;						// count the ammount of instant velocity calculated in e-time
int volatile velkH_accumulated = 0;				// the sum of all instant velocity calculated in e-time
long int t_e_vel=0;								// when the last average velocity was calculated

const float wheel_radius = (29 * 0.0254) / 2; 	// wheel radius. Converting from inches to meter 
const short number_signals = 18;				// Numbers of signals to complete 1 wheel loop 
const float const_velocity = (2 * 3.14 * wheel_radius / number_signals);
const float meterSecond_to_kilometerHour = 3.6;

void velocityInterrupt(){
	detachInterrupt(1);
	dt_time = millis() - last_signal_time;
	last_signal_time = millis();
	velKh = meterSecond_to_kilometerHour * const_velocity / (dt_time / 1000.0);
	
	count_vel++;
	velkH_accumulated += velKh;
	delay(20);
	attachInterrupt(0, velocityInterrupt, RISING);
}

void calculate(){
	if(millis()- t_e_vel > e && count_vel !=0){
		average_vel = (velkH_accumulated/count_vel);
		count_vel = 0;
		velkH_accumulated = 0;
		t_e_vel = millis();
	}
	else if(millis()-t_e_vel> e && count_vel == 0){
		average_vel = 0;
	}
}

/*
	Function to write a two digits number
*/
void writeNumber(short digitMatrix[][3], int number){
	for(int k = 0; k < 5; k++){
		// Turn on the first transistor
		PORTD |= 0x40;	// 0b01000000
		// Turn off the last one
		PORTD &= 0xdf;	// 0b11011111
		writeDigit(digits, number / 10);
		_delay_ms(15);

		// Turn on the second transistor
		PORTD |= 0x20;	// 0b00100000
		// Turn off the first
		PORTD &= 0xbf;	// 0b10111111
		writeDigit(digits, number % 10);
		_delay_ms(15);
	}
}

/*
	Write a single digit on the 2-seven segment display
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

void setup() {
	Serial.begin(9600);

	// DDRB Ports. 					   
	//		   			   GFEDCB =		7s pins
	DDRB = 0x3f;	// 0b00111111
	//		 			 				Where A is the last remaining pin from the 7s and the 
	//					 A12			1 & 2 referes to the first and second transistor.		
	DDRD = 0xe0;	// 0b1110000
	PIND = 0x00;

	// Velocimeter sensor (digital)
	attachInterrupt(0, velocityInterrupt, RISING);
    t_e_vel = millis();

}

void loop() {
	calculate();
	writeNumber(digits, average_vel);
}