//includes basicos

#include <math.h>                                                         // Incluimos a biblioteca Math

// Configuração do DHT
#include <DHT.h>                                                          // Incluimos a biblioteca DHT
#define DHTPIN A1                                                         // Pino analógico do DHT11
#define DHTTYPE DHT11                                                     // DHT 11
DHT dht(DHTPIN, DHTTYPE);                                                 // Declarando passando as funções da biblioteca para a variavel   

// Configuração do MQ135
#include "SoftwareSerial.h"                                               // Incluimos a biblioteca SoftwareSerial
SoftwareSerial mySerial(8,9);                                            // RX(9) y TX(8) Para o Gsm
#define MQ_analog A5                                                      // Pino do MQ135 analógico 
#define MQ_dig 7                                                          // Pino do MQ135 digital 

// Declarações de variaveis
int valor_analog;                                                         // declaração para o valor analógico do MQ135 
int valor_dig;                                                            // declaração para o valor digital do MQ135

void setup() {
  Serial.begin(9600);                                                     // Iniciamos a comunicação serial
  mySerial.begin(9600);                                                   // Iniciamos uma segunda comunicação serial
  dht.begin();                                                            // Iniciamos o dht   
  pinMode(MQ_analog, INPUT);                                              // Declaramos o pino analogico do Mq135 como input
  pinMode(MQ_dig, INPUT);                                                 // Declaramos o pino digital do Mq135 como input
}
 
void loop() {
  if (mySerial.available()){                                              // Se a comunicação SoftwareSerial é viável
    Serial.write(mySerial.read());                                        // Obtem-se os dados
  }
  if (Serial.available()){                                                // Se a comunicação serie normal tem dados
    while(Serial.available()) {                                           // e enquanto tenha dados para mostrar 
      mySerial.write(Serial.read());                                      // Obtemos pela comunicação SoftwareSerial
    } 
    mySerial.println();                                                   // Enviamos um fim de linha
  }
  float t = dht.readTemperature();                                        // Obtemos o valor da temperatura numa variável t
  valor_analog = analogRead(MQ_analog);                                   // Obtemos o valor analógico do MQ135 
  valor_dig = digitalRead(MQ_dig);                                        // Obtemos o valor digital do MQ135
  EnviaSMS(); 
  Serial.print("Foi");
  delay(60000);                                                            // Espera por 8 segundos
}

void EnviaSMS(){            
  float t = dht.readTemperature();                                    // Valor da temperatura do DHT11
  valor_analog = analogRead(MQ_analog);                               // Valor analógico do MQ135
  mySerial.println("AT+CMGF=1");                                      // Ativamos a função de envio de SMS
  delay(100);                                                         // Pausa
  mySerial.println("AT+CMGS=\"+5547989162853\"");                     // Definimos o número do destinatário em formato internacional
  delay(100);                                                         // Pausa
  mySerial.print("Risco de incendio, temperatura: "+(String)t+", partes por milhao de CO2 : "+(String)valor_analog);                                           // Texto do SMS 
  delay(500);                                                         // Pausa
  mySerial.print(char(26));                                           // Enviamos o equivalente a Control+Z 
  delay(100);                                                         // Pausa
  mySerial.println("");                                               // Enviamos um fim de linha
  delay(100);                                                         // Pausa
}
