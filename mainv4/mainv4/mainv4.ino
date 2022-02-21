//WORKING CODE 11/01/2022

#include <ESP8266WiFi.h>
#include <EmonLib.h>
#include <LiquidCrystal_I2C.h>  //Biblioteca de manipulação do LCD.

#define key_vtensao D0
int key_state = 0;
int estado = 0;
//#define SSID_REDE "CPS_SMART" /* coloque aqui o nome da rede que se deseja conectar */
//#define SENHA_REDE "smart@2019" /* coloque aqui a senha da rede que se deseja conectar */

#define SSID_REDE "JamesESP" /* coloque aqui o nome da rede que se deseja conectar */
#define SENHA_REDE "jamesdamas" /* coloque aqui a senha da rede que se deseja conectar */

//#define SSID_REDE "MDiasPhone" /* coloque aqui o nome da rede que se deseja conectar */
//#define SENHA_REDE "12345678" /* coloque aqui a senha da rede que se deseja conectar */


char endereco_api_thingspeak[] = "api.thingspeak.com";
String chave_escrita_thingspeak = "GQI2SGGWEONXZJO5";  /* Coloque aqui sua chave de escrita do seu canal */
WiFiClient client;


// Liquid Crystal Display ________________________________
int lcdColumns = 16;
int lcdRows = 2;
#define address_i2c 0x27
LiquidCrystal_I2C lcd(address_i2c, lcdColumns, lcdRows);
// Liquid Crystal Display ________________________________


EnergyMonitor EnerMonitor;       //Cria uma instancia da classe EnergyMonitor
double rms = 0.000;
int t_rede = 127;
double potencia = 0.000; //para cada kwh
double kwh = 0.000; //para cada kwh
double preco = 0.000; //valor calculado a partir do consumo
double tarifa = 0.85; //para cada kwh
double acumulado = 0.000; //soma do preço à cada segundo


void def_voltage() {
  key_state = digitalRead(key_vtensao);

  if (key_state == 1) {
    t_rede = 127;
    Serial.println("Tensao da Rede = 127 V");
  }
  if (key_state == 0) {
    t_rede = 220;
    Serial.println("Tensao da Rede = 220 V");
  }
  delay(2000);
}

void wifi_try_toconnect() {
  delay(2000);
  Serial.println("------WI-FI -----");
  Serial.println("Conectando-se a rede: ");
  Serial.println(SSID_REDE);
  Serial.println("\nAguarde...");
  Serial.println("Connecting.. status: " + String(WiFi.status()));


  wifi_connect();
}

void wifi_connect() {
  if (WiFi.status() == WL_CONNECTED) {
    return;
  }

  WiFi.begin(SSID_REDE, SENHA_REDE);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi - Conexão não estabelecida!");
    Serial.println("Connecting.. status: " + String(WiFi.status()));
    digitalWrite(LED_BUILTIN, LOW);
    delay(500);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(5000);
  }

  Serial.print("Conexão estabelecida com Sucesso! Rede: ");
  Serial.println(SSID_REDE);
}

void read_current() {
  rms = EnerMonitor.calcIrms(1480);
  potencia = rms * t_rede / 1000;
  //kwh = kwh+potencia/3600;
  kwh = potencia / 3600;
  preco = kwh * tarifa;
  acumulado = acumulado + kwh * tarifa;

  Serial.print("Corrente: ");
  Serial.println(rms, 3);

  Serial.print("Potencia (kW): ");
  Serial.println(potencia, 3);

  Serial.print("Energia (kWh): ");
  Serial.println(kwh, 3);

  Serial.print("Preco instantaneo (Reais): ");
  Serial.println(preco, 5);

  Serial.print("Preco total (Reais): ");
  Serial.println(acumulado, 5);

  Serial.print("Tensão: ");
  Serial.println(t_rede);
}

void envia_informacoes_thingspeak(String string_dados)
{
  if (client.connect(endereco_api_thingspeak, 80))
  {
    /* faz a requisição HTTP ao ThingSpeak */
    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: " + chave_escrita_thingspeak + "\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(string_dados.length());
    client.print("\n\n");
    client.print(string_dados);
    Serial.println("- Informações enviadas ao ThingSpeak!");
  }
  else {
    Serial.println("Não foi possivel conectar ao ThingSpeak!");
  }
}

void envia_infos_thingspeak() {
  char field_text[100] = {0};

  if (client.connected()) {
    client.stop();
    Serial.println("-Desconectado ThingSpeak");
    Serial.println("  ");
  }
  wifi_connect();

  sprintf(field_text, "field1=%.5f&field2=%.5f&field3=%.5f&field4=%.5f", rms, potencia, kwh, preco);
  envia_informacoes_thingspeak(field_text);

}

void lcd_displaytensao() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("   Ola, Seja");
  lcd.setCursor(0, 1);
  lcd.print("   bem vindo!");

  delay(2000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("    Tensao");
  lcd.setCursor(5, 1);
  lcd.print(t_rede);

  delay(2000);
}

void lcd_display() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("I(RMS):");
  lcd.setCursor(9, 0);
  lcd.print(rms, 3);
  //mostra tensão

  lcd.setCursor(0, 1);
  lcd.print("Acum.:");
  lcd.setCursor(9, 1);
  lcd.print(acumulado, 5);
}

void lcd_displaydois() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("kW:");
  lcd.setCursor(9, 0);
  lcd.print(potencia, 3);
  //mostra tensão

  lcd.setCursor(0, 1);
  lcd.print("Reais:");
  lcd.setCursor(9, 1);
  lcd.print(preco, 5);
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  WiFi.mode(WIFI_STA);
  Serial.begin(115200);
  def_voltage();
  wifi_try_toconnect();
  EnerMonitor.current(0, 13.88);
  delay(3000);
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd_displaytensao();

}

void loop() {
  read_current();
  delay(3000);
  envia_infos_thingspeak();
  if (estado <= 2) {
    lcd_display();
  }
  else if (estado > 2) {
    lcd_displaydois();
  }
  if (estado >= 5) {
    estado = 0;
  }
  estado = estado + 1;
}
