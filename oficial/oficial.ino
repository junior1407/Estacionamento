#include <Servo.h>
#include <LiquidCrystal.h>
#define vaga_0_0 0
#define vaga_0_1 1
#define vaga_0_2 2
#define vaga_0_3 3s
#define catraca_0 4
#define catraca_1 5
#define pinServo_0 2
#define pinServo_1 3

//TODO: Adicionar delay entre cada leitura.
//LiquidCrystal lcd(<pino RS>, <pino enable>, <pino D4>, <pino D5>, <pino D6>, <pino D7>)
LiquidCrystal lcd(12, 13, 14, 15, 16, 17);

Servo servo0;
Servo servo1;
String string1 = "";
String string2 = "";
bool timer0=0, timer1=0;
int countTimer0, countTimer1;
int lastReading[] = {0,0,0,0,0,0};
int pinSensores[] = { A0, A1, A2, A3, A8, A9};
int stateVagas[] = { 0, 0, 0, 0}; 
int pinLedVagasVerde[] = {4, 5, 6, 7};
int pinLedVagasVermelho[] = {8, 9, 10, 11};
int ocioso[] = {0, 0, 0, 0,0,0};
int stateCatracas[] = {0, 0};
int contadorCatracas[] = {0, 0};
int vagas = 4;

void checkSensores() {

  for (int i = 0; i < 10 ; i++){
      for (int j = 0; j < 6; j++) {
        if (i ==0)
        {
          lastReading[j] =0;
        }
        lastReading[j] += analogRead(pinSensores[j]);
      }
  }
  for (int i = 0; i < 6; i++) {
     lastReading[i] = lastReading[i] / 10;
    
  }
/*
  if (true) { //TODO: Range de aceitação.
    stateVagas[pos] = 1;
  }
  else {
    stateVagas[pos] = 0 ;
  }
*/

}


void showVagas() {
  int temp;
  for (int i = vaga_0_0 ; i <= vaga_0_3; i++) {
    if (stateVagas[i] == 1) {
      digitalWrite(pinLedVagasVermelho[i], HIGH);
      digitalWrite(pinLedVagasVerde[i], LOW);
    }
    else {
      digitalWrite(pinLedVagasVermelho[i], LOW);
      digitalWrite(pinLedVagasVerde[i], HIGH);
    }
  }
}


void checkCatracas() {

  // catraca 0 ENTRADA
  if (stateCatracas[0] == 0) { // Se estiver fechada.
    if ((true) && (vagas > 0)) {
      timer0=true;
      countTimer0 = millis();
      servo0.write(90);
      vagas --;
    }
  }
  else if (timer0==true){
    if (millis()-countTimer0 >=10000)
    {
      servo1.write(0);
      timer0=false;
    }
  }

  
  // catraca 1 SAÍDA
  if (stateCatracas[1] == 0) { // Se estiver fechada.
    if ((true) && (vagas > 0)) {
      timer1=true;
      countTimer1 = millis();
      servo1.write(90);
      vagas++ ;
    }
  }
    else if (timer1==true){
    if (millis()-countTimer1 >=10000)
    {
      servo1.write(0);
      timer1=false;
    }
  }

}


void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.clear();
  servo0.attach(pinServo_0);
  servo1.attach(pinServo_1);
  servo0.write(0); //TODO: Conferir se 0 graus é fechado.
  servo1.write(0);
  for (int i = vaga_0_0; i <= vaga_0_3; i++ ) {
    pinMode(pinLedVagasVerde[i], OUTPUT);
    pinMode(pinLedVagasVermelho[i], OUTPUT);
  }
  checkSensores();
  for(int i =0; i<6; i++){
    ocioso[i] = lastReading[i];
  }
  int temp;
  

}


void loop() {
  checkSensores();
  showVagas();
  checkCatracas();
  string1 = "Ha ";
  string2 = string1 + vagas;
  string1= string2 +" vagas.";
  //TODO: Atualiza string1 para  "Ha x vagas" se houver vagas, senão "Não ha vagas."
  lcd.clear();
  lcd.print(string1);
}
