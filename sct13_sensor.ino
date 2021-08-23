#include "EmonLib.h" 
#include <LiquidCrystal.h>

#define Vrede 129     // define a tensão RMS da rede(valor lido com multimetro)
#define InputSCT 0    // define o canal analógico para o sinal do SCT = pino A2

#define I_calibration 60         // fator de calibração da corrente - leia minhas dicas no tutorial

 
EnergyMonitor EnerMonitor;       //Cria uma instancia da classe EnergyMonitor

double Irms = 0 ;
float Tempo = 0;

void setup() 
{


  
  EnerMonitor.current(InputSCT, I_calibration);   // configura pino SCT e fator de calibração
  Serial.begin(9600);
  lcd.print("Irms(A)=");                    // mostra texto
  lcd.setCursor(0,1);                       // seleciona coluna 0, linha 1
  lcd.print("Ener(Wh)=");                   // mostra texto
} 
 
void loop() 
{ 
  Irms = EnerMonitor.calcIrms(1480);         // calculo da corrente RMS

  Serial.print(Irms);                            // mostra valor da corrente 
  Serial.println("");
  Serial.print(Irms*Vrede*(Tempo/3600));        // calculo de Watt/hora  
  Tempo++;                                   // incrementa Tempo   
  delay(1000);                               // atraso de 1 segundo
}