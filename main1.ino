//Programa para o hardware de medição de energia AC
//CPU: ESP8266
//WIFI: ESP8266
//Extra: Display LCD I2C
//Extra: Sensor de Corrente
//Extra: Botão para mudança de nível de tensão



//Modules included
#include <ESP8266WiFi.h> //Funções para o ESP8266 WiFi
#include <WiFiClientSecure.h> //Conexão WiFi com ambientes que exigem conexões seguras à clientes
#include <LiquidCrystal_I2C.h> //Fornece as funções necessárias para trabalhar facilmente com um Display LCD que possui módulo I2C
#include <UniversalTelegramBot.h> //Funções para comunicação com um bot Telegram
#include <ArduinoJson.h> //
#include <user_interface.h> //Biblioteca necessaria para acessar os Timer`s.


//OS Timer
os_timer_t tmr0; //Cria o Timer. Maximo de 7 Timer's.
volatile byte task; //Variavel armazenada na RAM para navegar entre as tarefas


//WiFi network credentials
const char* ssid = "CPS_SMART"; //SSID da rede no qual se conectará à internet
const char* password = "smart@2019"; //Senha da rede no qual se conectará à internet


//Thingspeak setting
char api_thingspeak[] = "api.thingspeak.com"; //Endereço da API do ThingSpeak
String api_writekey = "DIGITAR A CHAVE AQUI"; //Chave de acesso para ter direito à escrita no canal criado para este projeto no ThingSpeak
#define send_thingspeak_interval 30000; //Envia informação ao ThingSpeak à cada 30s


//


//Display LCD Parameters
LiquidCrystal_I2C lcd(0x27, 16, 2); //(address I2C, lcdColumns, lcdRows)



void setup(){
    //Display LCD init
    lcd.init();
    lcd.backlight();
    
    //Configuração do Timer
    os_timer_setfn(&tmr0, o_tasks, NULL); //Indica ao Timer qual sera sua Sub rotina.
    os_timer_arm(&tmr0, 1000, true);  //Indica ao Timer seu Tempo em mS e se será executado repetidas vezes ou apenas uma vez (loop = true)
                                     //Desejamos que seja um processo repetido, então configuramos como true
}


void loop(){
    o_tasks(); //Chama função que possui as tarefas a serem executadas
    leitura_sensor(); //Faz a leitura do sensor de Corrente
}


void escreve_Display(){

}

void o_tasks(){
    switch(task){
        case(1):
            //Leitura Sensor
        break;        
        
        case(2):
            //Leitura Sensor    
            //Verifica Telegram
        break;

        case(4):
            //Função envia ThingSpeak
        break;

        case(5):
            //Atualiza o Display            
        break;                
    }
}




//--------------------------------------------------------------------------------------------------
void loop()
{
   leitura();//Sub rotina para processar os LED`s.

   //O codigo irá funcionar normalmente sem que haja delays e acendendo os LED`s no intervalo definido.
   //Para demonstraçao, abra o Serial monitor e voce vera que o codigo continua normalmente.

   Serial.println(x++);//Print para provar que o codigo nao trava. Abra o Serial monitor e veja
                       //que o codigo continua rodando enquanto os LED`s piscam a cada 1 Segundo.
}

void leitura()//Sub rotina para processar os LED`s.
{
   switch (status)
   {
   case(1)://Caso status seja 1, acenda o led 1.
      digitalWrite(D1, 1);
   break;

   case(2)://Caso status seja 2, acenda o led 2.
      digitalWrite(D2, 1);
   break;

   case(3)://Caso status seja 3, acenda o led 3.
      digitalWrite(D3, 1);
   break;

   case(4)://Caso status seja 4, Apague os 3 LED`s.
      digitalWrite(D1, 0);
      digitalWrite(D2, 0);
      digitalWrite(D3, 0);
   break;
   }
}

void led(void*z)//Sub rotina ISR do Timer sera acessada a cada 1 Segundo e mudara o status do LED.
{
   if (status == 4)//Verifica se o maximo foi alcançado para resetar a contagem.
   {
      status = 0;
   }

   status++;
}