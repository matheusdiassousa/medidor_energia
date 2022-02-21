#include <LiquidCrystal_I2C.h>  //Biblioteca de manipulação do LCD.
#include <user_interface.h>     //Biblioteca necessaria para acessar os Timer`s.
#include "EmonLib.h"            //Biblioteca de calculo da corrente RMS.
#include <FS.h>                 //Biblioteca da memória flash ESP8266


// Sensor de Corrente ________________________________
#define Vrede 127    // define a tensão RMS da rede(valor lido com multimetro)
#define InputSCT 0    // define o canal analógico para o sinal do SCT = pino A2
#define I_calibration 45.48         // fator de calibração da corrente - leia minhas dicas no tutorial
//29.48
EnergyMonitor EnerMonitor;       //Cria uma instancia da classe EnergyMonitor
double rms = 0;
double Irms2 = 0;
String string = "";
double watthora = 0;
float Tempo = 0;
// Sensor de Corrente ________________________________


// Timer ________________________________
os_timer_t tmr0;//Cria o Timer. Maximo de 7 Timer's.
volatile byte status;//Variavel armazenada na RAM para Status do LED.
// Timer ________________________________


// Liquid Crystal Display ________________________________
int lcdColumns = 16;
int lcdRows = 2;
#define address_i2c 0x27
LiquidCrystal_I2C lcd(address_i2c, lcdColumns, lcdRows);
// Liquid Crystal Display ________________________________


void setup()
{
// Sensor de Corrente ________________________________    
    EnerMonitor.current(InputSCT, I_calibration);   // configura pino SCT e fator de calibração
// Sensor de Corrente ________________________________


// Timer ________________________________    
   os_timer_setfn(&tmr0, contador, NULL); //Indica ao Timer qual sera sua Sub rotina.
   os_timer_arm(&tmr0, 1000, true);  //Indica ao Timer seu Tempo em mS e se sera repetido ou apenas uma vez (loop = true)
// Timer ________________________________   


// Liquid Crystal Display ________________________________
    lcd.init();
    lcd.backlight();
    lcd.clear();
// Liquid Crystal Display ________________________________

Serial.begin(115200);
//conecta WiFi

delay(1000);
}

void loop()
{

    lcd_display();
    //envia_thingspeak():
    //checa_telegram();
}

void lcd_display()
{
    switch (status)
    {
    case (1):
        lcd.setCursor(0,0);
        lcd.print("Tensao:");
        lcd.setCursor(10,0);
        lcd.print(Vrede);
        //mostra tensão
        
        lcd.setCursor(0,1);
        lcd.print("Irms:");
        lcd.setCursor(10,1);
        Irms2 = ler_corrente();
        lcd.print(Irms2);
        //mostra corrente
        break;
        
    case (9):

        //mostra potencia instantanea
        //mostra previsão kwh (1h)
        lcd.clear();
        break;
        
    case (10):
        lcd.setCursor(0,0);
        lcd.print("Pot. kW:");
        lcd.setCursor(10,0);
        lcd.print(Vrede*rms/1000);
        //mostra potencia

        lcd.setCursor(0,1);
        lcd.print("Irms:");
        lcd.setCursor(10,1);
        Irms2 = ler_corrente();
        lcd.print(Irms2);
        //mostra corrente        
        
        break;

    case (19):
        
        //mostra previsão kwh (1m)
        //mostra previsão acumulada
        lcd.clear();
        break;

    case (20):
        lcd.setCursor(0,0);
        lcd.print("kWh:");
        watthora = (Vrede*rms/1000)*0.92;
        lcd.setCursor(10,1);
        lcd.print(watthora);
        //mostra potencia

        lcd.setCursor(0,1);
        lcd.print("Irms:");
        lcd.setCursor(10,1);
        Irms2 = ler_corrente();
        lcd.print(Irms2);
        //mostra corrente        
        

        //mostra corrente
        //mostra potencia instantanea
        //mostra previsão kwh (1h)
        break;

    }

}


void contador(void*z)//Sub rotina ISR do Timer sera acessada a cada 1 Segundo e mudara o status do LED.
{
   if (status == 20)//Verifica se o maximo foi alcançado para resetar a contagem.
   {
      status = 0;
   }

   status++;
   ler_corrente();
}

double ler_corrente()
{
    rms = EnerMonitor.calcIrms(1480);         // calculo da corrente RMS
    //string = String(rms,3);
    //Irms2 = string.toDouble();
    watthora = rms*Vrede*(Tempo/3600);
    Tempo++;
    return rms;
}
