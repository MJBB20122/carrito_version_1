//	Integrantes:
//	- Cindy Janet Nolasco 					NA13005
//	- Ingrid Fabiola Martínez 				MR13051
//	- Mauricio Rigoberto Martinez Romero 	MR13052
//	- Marvín José Benítez Benítez			BB13002

// incluimos la libreria para utilizar el receptor IR
#include <IRremote.h>
//hacemos las definiciones de los pines que vamos a utilizar
#define trigPin 4
#define echoPin 11

#define E1 0
#define E2 1

#define A1 8
#define A2 9
#define B1 12
#define B2 13

#define RP 3
#define LD 6

IRrecv irrecv(RP);
decode_results results;
int encendido = 0;
int direccion = 0;
int cantidad = 0;
long duracion, distancia ;
int giro = 0;
int revoluciones = 0;
// seteamos los pines en input u output asi como asignar la interrupcion a la funcion receptor
void setup()
{
  
    pinMode(E1, OUTPUT);
    pinMode(E2, OUTPUT);

    pinMode(A1, OUTPUT);
    pinMode(A2, OUTPUT);
    pinMode(B1, OUTPUT);
    pinMode(B2, OUTPUT);
    
    pinMode(LD, OUTPUT);


    pinMode(trigPin, OUTPUT);
	pinMode(echoPin, INPUT);
   
    Serial.begin(9600);
    digitalWrite(RP, LOW);
    digitalWrite(2, LOW);
	attachInterrupt(1, receptor, HIGH);
	irrecv.enableIRIn();
     

}
// hilo principal del programa
void loop()
{
	// leemos el sensor de distancia
	digitalWrite(trigPin, LOW);
	delayMicroseconds(10);
    digitalWrite(trigPin, HIGH);
	delayMicroseconds(10);
	digitalWrite(trigPin, LOW);
	duracion = pulseIn(echoPin, HIGH);
	distancia = int(0.017 * duracion);
	// si la distancia del sensor al objeto mas cercano es igual o menor a 15 cm ejecutamos la funcion de reversa condicionada
	if(distancia <= 15)
	{
		if(encendido == 1)
			{
				reversa_2s(); 
			}
		// apagamos el control de interrupciones
		digitalWrite(RP, LOW);
    }
	// encendemos el control de interrupciones
	Serial.println(String(distancia) + " cm.");
	digitalWrite(2, LOW);
	digitalWrite(RP, HIGH);
	delay(500);
}
// funcion controladora para la interrupcion que lee el IR
void receptor()
{
	// leemos el valor que nos retorna el IR
    if(irrecv.decode(&results))
	{
		if(results.value!=4294967295)
			{
				cantidad++;
				Serial.print("Valor RF ");
				Serial.print(cantidad);
				Serial.print(" : ");
				Serial.println(results.value);
				digitalWrite(LD, HIGH);
			}
		// validamos que se desea encender o apagar el carrito
		if(results.value == 551522415 || results.value ==1785214238 || results.value == 4131255956 || results.value ==4129172055)
			{
				// si el carrito esta apagado lo encendemos
				if(encendido == 0)
					{
						Serial.print("Valor encendido: ");
						Serial.println(results.value);
						encendido = 1;
					}
				// si el carrito esta encendido lo apagamos
				else
					{
						Serial.print("Valor Apagado: ");
						Serial.println(results.value);
						encendido = 0;
					}
			}
		// activamos la led que nos permite saber el estado del carrito (encendido o apagado)
		if(encendido == 1)
			{
				digitalWrite(LD, HIGH);
			}
		else
			{
                digitalWrite(LD, LOW);
				apagar();
			}
		// si leemos la instruccion de detener paramos el carrito
		if(results.value == 3102192894 )
			{
				Serial.print("Valor Parar: ");
				Serial.println(results.value);
				parar();
			}
		// si leemos la instruccion de avanzar activamos la potencia y movemos el carrito hacia delante
		if(results.value == 551495895 || results.value == 272201887 ||  results.value == 4222411808 )
			{
				Serial.print("Valor Adelante: ");
				Serial.println(results.value);
				potencia();
				adelante();
			}
		// si leemos la instruccion de reversa activamos la potencia y movemos el carrito hacia atras
		if(results.value == 551528535 || results.value == 3146464926 )
			{
				Serial.print("Valor Reversa: ");
				Serial.println(results.value);
				potencia();
				reversa();
			}
		// si leemos la instruccion de girar a la derecha activamos la potencia y movemos la rueda izquierda para girar a la derecha
		if(results.value == 551544855 || results.value == 4038376259 || results.value == 4277157420 )
			{
				Serial.print("Valor Derecha: ");
				Serial.println(results.value);
				potencia();
				derecha();
			}
		// si leemos la instruccion de girar a la izquierda activamos la potencia y movemos la rueda derecha para girar a la izquierda
		if(results.value == 551491815 || results.value ==  1162352386 ||  results.value == 1726574660 )
			{
				Serial.print("Valor Izquierda: ");
				Serial.println(results.value);
				potencia();
				izquierda();
			}
		// si leemos la instruccion para potencia media establecemos la variable revoluciones a 0
		if(results.value == 551502015 || results.value == 1808161147)
			{
				Serial.println("potencia 0");
				revoluciones = 0;
			}
		// si leemos la instruccion para potencia media establecemos la variable revoluciones a 0
		if(results.value == 551534655 || results.value == 19072879901935374718)
			{
				Serial.println("potencia 1");
				revoluciones = 1;
			}
		// liberamos el lector del IR para que pueda leer una nueva instruccion
		irrecv.resume();
	}
	if(encendido == 0)
		{
			apagar();
		}
}
// funciones especificas
//----------------------

// enciende el carrito
void encender()
{
    digitalWrite(A1, HIGH);
    digitalWrite(A2, LOW);
    digitalWrite(B1, HIGH);
    digitalWrite(B2, LOW);
}

// detiene los motores
void parar()
{
    digitalWrite(A1, LOW);
    digitalWrite(A2, LOW);
    digitalWrite(B1, LOW);
    digitalWrite(B2, LOW);
}

// apaga el carrito
void apagar()
{
    digitalWrite(A1, LOW);
    digitalWrite(A2, LOW);
    digitalWrite(B1, LOW);
    digitalWrite(B2, LOW);
}

// hace girar la rueda izquierda para girar a la derecha
void derecha()
{
	digitalWrite(A1, LOW);
    digitalWrite(A2, LOW);
    digitalWrite(B1, LOW);
    digitalWrite(B2, HIGH); 
}

// hace girar la rueda derecha para girar a la izquierda
void izquierda()
{
	digitalWrite(A1, HIGH);
    digitalWrite(A2, LOW);
    digitalWrite(B1, LOW);
    digitalWrite(B2, LOW);
}

// activa ambos motores dandole sentido hacia adelante
void adelante()
{
    digitalWrite(A1, HIGH);
    digitalWrite(A2, LOW);
    digitalWrite(B1, HIGH);
    digitalWrite(B2, LOW);
}

// activa ambos motores dandole sentido hacia atras
void reversa()
{
    digitalWrite(A1, LOW);
    digitalWrite(A2, HIGH);
    digitalWrite(B1, LOW);
    digitalWrite(B2, HIGH); 
}

// reversa condicionada, detiene ambos motores y luego aplica la reversa por 2 segundos
void reversa_2s()
{
    parar();
    delay(1000);
	Serial.println("Interrupcion atras");
    digitalWrite(A1, LOW);
    digitalWrite(A2, HIGH);
    digitalWrite(B1, LOW);
    digitalWrite(B2, HIGH);
    delay(2000);
    Serial.println("despues delay");
    digitalWrite(A1, LOW);
    digitalWrite(A2, LOW);
    digitalWrite(B1, LOW);
    digitalWrite(B2, LOW);
}

// establece la potencia para los motores en media (40) o total (255)
void potencia()
{
    if(revoluciones == 0)
		{
			analogWrite(E1, 40);
			analogWrite(E2, 40);
		}
    if(revoluciones == 1)
		{
			analogWrite(E1, 255);
			analogWrite(E2, 255);
		}
}