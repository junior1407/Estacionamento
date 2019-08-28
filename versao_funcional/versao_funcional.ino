#include <Servo.h>
#include <LiquidCrystal.h>

#define pinServo_0 2
#define pinServo_1 3

//LiquidCrystal lcd(<pino RS>, <pino enable>, <pino D4>, <pino D5>, <pino D6>, <pino D7>)
LiquidCrystal lcd(12, 13, 14, 15, 16, 17);

//Variáveis
Servo servo_entrada;
Servo servo_saida;
String string1 = "";
String string2 = "";
bool timer0 = 0, timer1 = 0;
unsigned long int countTimer0, countTimer1;
float lastReading[] = {0, 0, 0, 0, 0, 0};
int pinSensores[] = { A0, A1, A9, A2};
int stateVagas[] = { 0, 0, 0, 0};
int pinLedVagasVerde[] = {4, 5, 6, 7};
int pinLedVagasVermelho[] = {8, 9, 11, 10};
float ocioso[] = {0, 0, 0, 0, 0, 0};
int stateCatracas[] = {0, 0};
int vagas = 3;

void readSensores() {

  for (int i = 0; i < 10 ; i++) {
    for (int j = 0; j < 4; j++) { //4 sensores - 2 vagas e 2 servos
      if (i == 0)
      {
        lastReading[j] = 0;
      }
      delay(7);
      lastReading[j] += analogRead(pinSensores[j]);
    }
  }
  for (int i = 0; i < 4; i++) {
    lastReading[i] = lastReading[i] / 10;
    lastReading[i] = (lastReading[i] * 5) / 1023; //Transformando escala em volts
  }
}

void processLeitura() {
  float temp;
  //Leitura de Vagas
  for (int i = 0; i < 2; i++) {
    temp = abs(ocioso[i] - lastReading[i]);  //Vendo diferença na leitura
    stateVagas[i] = 0;
    if (temp > 0.1) { //Se variar 0.1v, há um carro
      stateVagas[i] = 1;
    }
  }
}

void showVagas() {
  for (int i = 0 ; i < 4; i++) {
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
    if ((abs(ocioso[2] - lastReading[2]) > 0.1) && (vagas > 0)) {  //Se a leitura diz que tem carro e há vaga
      timer0 = true;
      stateCatracas[0] = 1;
      countTimer0 = millis();
      servo_entrada.write(110);  //Aberto
      vagas --;
    }
  }
  else if (timer0 == true) {
    if (millis() - countTimer0 >= 3000)
    {
      servo_entrada.write(25); //Fechado
      timer0 = false;
      stateCatracas[0] = 0;
    }
  }


  // catraca 1 SAÍDA
  if (stateCatracas[1] == 0) { // Se estiver fechada.
    if (abs(ocioso[3] - lastReading[3]) > 0.1) {
      timer1 = true;
      stateCatracas[1] = 1;
      countTimer1 = millis();
      servo_saida.write(95); //ABERTA
      vagas++ ;
    }
  }
  else if (timer1 == true) {
    //Serial.println(millis() - countTimer1);
    if (millis() - countTimer1 >= 3000)
    {
      stateCatracas[1] = 0;
      servo_saida.write(0); //ABERTO
      timer1 = false;
    }
  }

}


void setup() {
  Serial.begin(9600);
  delay(200);

  lcd.begin(16, 2);
  lcd.clear();

  servo_entrada.attach(pinServo_0);
  servo_saida.attach(pinServo_1);
  servo_entrada.write(25);
  servo_saida.write(0);

  for (int i = 0; i < 4; i++ ) {
    pinMode(pinLedVagasVerde[i], OUTPUT);
    pinMode(pinLedVagasVermelho[i], OUTPUT);
  }
  stateVagas[2] = 0;
  stateVagas[3] = 1;

  readSensores();
  Serial.println("-----");
  for (int i = 0; i < 4; i++) {
    ocioso[i] = lastReading[i];
    Serial.println(ocioso[i]);
  }
}


void loop() {
  readSensores();
  processLeitura();
  checkCatracas();
  delay(100);
  showVagas();

  string1 = "Ha ";
  string2 = string1 + vagas;
  string1 = string2 + " vagas.";
  lcd.clear();
  lcd.print(string1);
}
