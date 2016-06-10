#include <Servo.h>
Servo myservo; 
#define P7 7
#define P6 6
#define P5 5
int angulo = 0;
void setup() {
	// put your setup code here, to run once:
	Serial.begin(9600);
	myservo.attach(9); 
	myservo.write(0);
        pinMode(P7, OUTPUT);
        pinMode(P6, OUTPUT);
        pinMode(P5, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
	int valor=myservo.read();
	myservo.write(45);
        if(valor == 45)
        {
            digitalWrite(P7, HIGH);
        }
	if(valor == 90)
        {
            digitalWrite(P6, HIGH);
        }
        if(valor == 180)
        {
            digitalWrite(P5, HIGH);
        }
        Serial.println(valor);
        angulo = angulo + 45;
        if(angulo == 180)
          {
            angulo = 0;
            digitalWrite(P7, LOW);
            digitalWrite(P6, LOW);
            digitalWrite(P5, LOW);
            delay(1500);
          }
        delay(2000);
}
