#include <Servo.h>
#include <LiquidCrystal.h>
#define vaga_0_0 0
#define vaga_0_1 1
#define vaga_0_2 2
#define vaga_0_3 3
#define pinServo_0 2
#define pinServo_1 3


//TODO: Adicionar delay entre cada leitura.
//LiquidCrystal lcd(<pino RS>, <pino enable>, <pino D4>, <pino D5>, <pino D6>, <pino D7>)
LiquidCrystal lcd(12, 13, 14, 15, 16, 17);

Servo servo0;
Servo servo1;
String string1 = "";
String string2 = "";
int pinCatracas[] = {A8, A9};
int pinVagas[] = { A0, A1, A2, A3};
int stateVagas[] = { 0, 0, 0, 0};
int pinLedVagas[] = {4, 5, 6, 7};
int ociosoVagas[] = {0, 0, 0, 0};
int ociosoCatracas[] = {0, 0};
int stateCatracas[] = {0, 0};
int contadorCatracas[] = {0, 0};
int vagas = 4;

void checkVaga(int pos) {

  int val = 0;
  for (int i = 0; i < 10; i++) {
    val += analogRead(pinVagas[pos]);
  }
  val = val / 10;
  if (true) { //TODO: Range de aceitação.
    stateVagas[pos] = 1;
  }
  else {
    stateVagas[pos] = 0 ;
  }


}

void checkVagas() {
  vagas = 0;
  for (int i = vaga_0_0 ; i <= vaga_0_3; i++) {
    checkVaga(i);
  }
}
void showVagas() {
  int temp;
  for (int i = vaga_0_0 ; i <= vaga_0_3; i++) {
    if (stateVagas[i] == 1) {
      digitalWrite(pinLedVagas[i], HIGH);
    }
    else {
      digitalWrite(pinLedVagas[i], LOW);
    }
  }
}


void checkCatracas() {

  int val;
  // catraca 0 ENTRADA
  if (stateCatracas[0] == 0) { // Se estiver fechada.
    val = 0;
    for (int i = 0; i < 10; i++) {
      val += analogRead(pinCatracas[0]);
    }
    val = val / 10;
    if ((true) && (vagas > 0)) {
      // TODO: Abre catraca por 10 segundos.
      servo0.write(90);
      vagas --;
    }
  }
  // catraca 1 SAÍDA
  if (stateCatracas[1] == 0) { // Se estiver fechada.
    val = 0;
    for (int i = 0; i < 10; i++) {
      val += analogRead(pinCatracas[1]);
    }
    val = val / 10;
    if ((true) && (vagas > 0)) {
      // TODO: Abre catraca por 10 segundos.
      servo1.write(90);
      vagas++ ;
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
    pinMode(pinLedVagas[i], OUTPUT);
  }

  // Leitura do Sensor das Catracas Ocioso.
  int temp;
  for (int i = 0; i < 2; i++) {
    temp = 0;
    for (int j = 0; j < 10; j++) {
      temp += analogRead(pinCatracas[i]);

    }
    ociosoCatracas[i] = temp / 10;
  }

  // Leitura do Sensor das Vagas
  for (int i = 0; i < 4; i++) {
    temp = 0;
    for (int j = 0; j < 10; j++) {
      temp += analogRead(pinVagas[i]);
    }
    ociosoVagas[i] = temp / 10;
  }

}

void loop() {
  checkVagas();
  showVagas();
  checkCatracas();
  //TODO: Atualiza string1 para  "Ha x vagas" se houver vagas, senão "Não ha vagas."
  lcd.clear();
  lcd.print(string1);
}
