class DisplaySeteSegmentos {	
	//							 	 A  B  C  D  E  F  G  H
	const short numeros[10][8] =    {{1, 1, 1, 1, 1, 1, 0, 0}, 	// 0
									{0, 1, 1, 0, 0, 0, 0, 0},	// 1
									{1, 1, 0, 1, 1, 0, 1, 0},	// 2
									{1, 1, 1, 1, 0, 0, 1, 0},	// 3
									{0, 1, 1, 0, 0, 1, 1, 0},	// 4
									{1, 0, 1, 1, 0, 1, 1, 0},	// 5
									{1, 0, 1, 1, 1, 1, 1, 0},	// 6
									{1, 1, 1, 0, 0, 0, 0, 0},	// 7
									{1, 1, 1, 1, 1, 1, 1, 0},	// 8
									{1, 1, 1, 0, 0, 1, 1, 0}};	// 9

	boolean default_value;
	/**
	Porta A 	=>	indice 0;
	Porta B 	=>	indice 1;
	Porta C 	=>	indice 2;
	Porta D 	=>	indice 3;
	Porta E 	=>	indice 4;
	Porta F 	=>	indice 5;
	Porta G 	=>	indice 6;
	Porta DP 	=>	indice 7;
	*/
	int *portas;
	int estado[8];

	public :

		DisplaySeteSegmentos(boolean _default_value){
			default_value = _default_value;
		}

		void setPortas(int *_portas){
			portas = _portas;
		}

		void initDisplay(){
			for(int i = 0; i < 7; i++){
		 		pinMode(portas[i], OUTPUT);
		 		digitalWrite(portas[i], default_value);
			}
		}

		void apagarDisplay(){
			for(int i = 0; i < 7; i++){
		 		digitalWrite(portas[i], default_value);
			}
		}

		void ligarDisplay(){
			for(int i = 0; i < 7; i++){
		 		digitalWrite(portas[i], !default_value);
			}
		}

		void escreverDigito(int digito){
			for(int i = 0; i < 7; i++){
				digitalWrite(portas[i], numeros[digito][i] == !default_value);
			}
		}
};

DisplaySeteSegmentos displayA(1);
DisplaySeteSegmentos displayB(1);

void setup() {
	// put your setup code here, to run once:
	Serial.begin(9600);

	int portas_displayA = new int[8]{35, 37, 27, 25, 23, 33, 31, 29};
	int portas_displayB = new int[8]{34, 36, 26, 24, 22, 32, 30, 28};
	
	displayA.setPortas(portas_displayA);
	displayB.setPortas(portas_displayB);

	displayA.initDisplay();
	displayA.ligarDisplay();
	
	displayB.initDisplay();
	displayB.ligarDisplay();
	
}

void loop() {
	// put your main code here, to run repeatedly:
	for(int i = 0; i < 100 ; i++){
 		displayA.escreverDigito(i / 10);
 		displayB.escreverDigito(i % 10);
 		delay(50);
	}
}

