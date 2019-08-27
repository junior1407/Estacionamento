#include <Servo.h>
#include <LiquidCrystal.h>

#define pinServo_0 2 //Entrada
#define pinServo_1 3 //Saida
Servo servo_entrada;
Servo servo_saida;

//LiquidCrystal lcd(<pino RS>, <pino enable>, <pino D4>, <pino D5>, <pino D6>, <pino D7>)
LiquidCrystal lcd(12, 13, 14, 15, 16, 17);
float lastReading[] = {0,0,0,0,0,0};
int pinSensores[] = { A0, A1, A9, A2};
float ocioso[] = {0, 0, 0, 0,0,0};
int pinLedVerde[] = {4, 5, 6, 7};
int pinLedVermelho[] = {8, 9, 11, 10};
String teste1="", teste2="";

void checkSensores() {

  for (int i = 0; i < 10 ; i++){
      for (int j = 0; j < 4; j++) {
        if (i ==0)
        {
          lastReading[j] =0;
        }
        delay(7);
        lastReading[j] += analogRead(pinSensores[j]);
      }
  }
  for (int i = 0; i < 4; i++) {
     lastReading[i] = lastReading[i] / 10;
     lastReading[i] = (lastReading[i]*5)/1023;
  }
}

void setup() {
  Serial.begin(9600);
  delay(200);

   lcd.clear();
   lcd.print("oi");
  servo_entrada.attach(pinServo_0);
  servo_saida.attach(pinServo_1);
  for (int i = 0; i < 4; i++ ) {
    pinMode(pinLedVerde[i], OUTPUT);
    pinMode(pinLedVermelho[i], OUTPUT);
  }
  
  servo_entrada.write(25); //TODO: Conferir se 0 graus é fechado.
  servo_saida.write(0);
 /* delay(2000);
 servo_entrada.write(115); //TODO: Conferir se 0 graus é fechado.
servo_saida.write(95); 
delay(2000);
  servo_entrada.write(25); //TODO: Conferir se 0 graus é fechado.
  servo_saida.write(0);*/
  
  checkSensores();
  Serial.println("-----");
  for(int i =0; i<4; i++){
    ocioso[i] = lastReading[i];
    Serial.println(ocioso[i]);
  }
   Serial.println("-----");
   digitalWrite(pinLedVerde[2], HIGH);
   digitalWrite(pinLedVermelho[3], HIGH);
}
float temp;
void loop() {
  delay(5000);
  checkSensores();
  Serial.println("-----");
  for(int i =0; i<4; i++){
    temp = abs(ocioso[i] - lastReading[i]);
    if (i<=1){
      if (temp>0.1){
        digitalWrite(pinLedVerde[i], LOW);
        digitalWrite(pinLedVermelho[i], HIGH);
      }
      else{
                digitalWrite(pinLedVerde[i], HIGH);
        digitalWrite(pinLedVermelho[i], LOW);
        
      }
    }
    if (i>=2){
         if ((i==2)&&(temp>0.2)){
             servo_entrada.write(110);
             delay(3000);
             servo_entrada.write(25);
         }
         if ((i==3)&&(temp>0.2)){
             servo_saida.write(95);
             delay(3000);
             servo_saida.write(0);
         }
      
    }
    Serial.println(temp);
  }
   Serial.println("-----");
  
}
