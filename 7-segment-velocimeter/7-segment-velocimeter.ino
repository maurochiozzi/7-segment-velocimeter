/*
	Code for seven segments velocimeter.
	Feel free to use and edit.

	Mauro Chiozzi 
*/

/* 
	Seven segments (7s) display class. Could have used the already made lib,
	but sometimes it's funnier DIY.
*/
class SevenSegmentsDisplay {	
	// Array-sequence to turn up a digit
	//							 	 A  B  C  D  E  F  G  H
	const short numbers[10][8] =   {{1, 1, 1, 1, 1, 1, 0, 0}, 	// 0
									{0, 1, 1, 0, 0, 0, 0, 0},	// 1
									{1, 1, 0, 1, 1, 0, 1, 0},	// 2
									{1, 1, 1, 1, 0, 0, 1, 0},	// 3
									{0, 1, 1, 0, 0, 1, 1, 0},	// 4
									{1, 0, 1, 1, 0, 1, 1, 0},	// 5
									{1, 0, 1, 1, 1, 1, 1, 0},	// 6
									{1, 1, 1, 0, 0, 0, 0, 0},	// 7
									{1, 1, 1, 1, 1, 1, 1, 0},	// 8
									{1, 1, 1, 0, 0, 1, 1, 0}};	// 9

	// There are two kinds of 7s: the anode type and the cathode type
	// Use 0 if your 7s use common anode or 1 if use common cathode.
	boolean default_off_value;

	/**
		Port A 	=>	index 0;
		Port B 	=>	index 1;
		Port C 	=>	index 2;
		Port D 	=>	index 3;
		Port E 	=>	index 4;
		Port F 	=>	index 5;
		Port G 	=>	index 6;
		Port DP =>	index 7; Can be omitted
	*/
	int *ports;
	int ports_state[8];

	public :

		/**
			Constructor. Must have the boolean value saying which type is the 7s
		*/
		SevenSegmentsDisplay(boolean _default_off_value){
			default_off_value = _default_off_value;
		}

		/**
			Set 7s ports according the relation shown above
		*/
		void setPorts(int *_ports){
			ports = _ports;
		}

		/**
			Aftes setting the ports, it's necessary initializate the ports as output
		*/
		void initDisplay(){
			for(int i = 0; i < 7; i++){
		 		pinMode(ports[i], OUTPUT);
		 		digitalWrite(ports[i], default_off_value);
			}
		}

		/**
			Turn off all 7s leds
		*/
		void turnOffDisplay(){
			for(int i = 0; i < 7; i++){
		 		digitalWrite(ports[i], default_off_value);
			}
		}

		/**
			Turn on all 7s leds
		*/
		void turnOnDisplay(){
			for(int i = 0; i < 7; i++){
		 		digitalWrite(ports[i], !default_off_value);
			}
		}

		/**
			Write the number digit
		*/
		void writeDigit(int digit){
			for(int i = 0; i < 7; i++){
				digitalWrite(ports[i], numbers[digit][i] == !default_off_value);
			}
		}
};

// 7s Diplays
SevenSegmentsDisplay displayA(1);
SevenSegmentsDisplay displayB(1);

// Velocity
const int e = 250; 						// time interval to stop reading velocity sinal and do the math to calculate de average velocity every e-time
long volatile dt_time = 0;				// time difference (delta t) between 2 velocity sinal
float volatile last_signal_time = 0;	// when the last sinal was read
float volatile velKh = 0;				// Velocity in km / h
int average_vel;						// this average velocity is has a too low dt depending on the velocity that can be consired as instant velocity
int volatile count_vel = 0;				// count the ammount of instant velocity calculated in e-time
int volatile velkH_accumulated = 0;		// the sum of all instant velocity calculated in e-time
long int t_e_vel=0;						// when the last average velocity was calculated

const long wheel_radius = (29 * 0.0254) / 2; // wheel radius. Converting from inches to meter 
const short number_signals = 18;	// Numbers of signals to complete 1 wheel loop 
const long const_velocity = (2 * 3.14 * wheel_radius / number_signals);
const long meterSecond_to_kilometerHour = 3.6;

void velocidade(){
	detachInterrupt(1);
	dt_time = millis() - last_signal_time;
	last_signal_time = millis();
	velKh = meterSecond_to_kilometerHour * (const_velocity / (dt_time / 1000));
	count_vel++;
	velkH_accumulated += velKh;
	delay(20);
	attachInterrupt(0, velocidade, RISING);
}

void calcular(){
	//VELOCIDADE------------------------------------------------------------------
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

void setup() {
	Serial.begin(9600);

	// Display setup

	int ports_displayA = new int[8]{35, 37, 27, 25, 23, 33, 31, 29};
	int ports_displayB = new int[8]{34, 36, 26, 24, 22, 32, 30, 28};
	
	displayA.setPorts(ports_displayA);
	displayB.setPorts(ports_displayB);

	displayA.initDisplay();
	displayB.initDisplay();

	// Velocimeter sensor (digital)
	attachInterrupt(0, velocidade, RISING);
    t_e_vel = millis();

}

void loop() {
	// put your main code here, to run repeatedly:
	calcular();
	displayA.writeDigit(average_vel / 10);
	displayB.writeDigit(average_vel % 10);
	delay(130);
}

