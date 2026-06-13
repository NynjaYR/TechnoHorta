#include "DHTesp.h"             //¨¨¨importa a biblioteca do dht22¨¨¨
#include <WiFi.h>               //¨¨¨adiciona o wifi¨¨¨
#include <HTTPClient.h>
#include <PubSubClient.h>       //¨¨¨MQTT¨¨¨
//#include <button.h>             //¨¨¨Botao mqtt¨¨¨
//#include <ESPAsyncWebServer.h>  //¨¨¨Inclui biblioteca para servidor web assíncrono¨¨¨
//#include <AsyncTCP.h>           //¨¨¨Inclui a biblioteca para TCP assíncrono¨¨¨
//#include "html_page.h"          //¨¨¨Inclui o arquivo HTML pagina na web¨¨¨
// (Desmarcar oque for ultilizar)


    const char* ssid = "MALVIMOB";  //WiFi 1
    const char* password = "32336724898";

// espaco para variaveis de teste

String messagemqtt = "";

int cansendmsg_A = 1;
int laststate_A = 0;

int cansendmsg_B = 1;
int laststate_B = 0;

int cansendmsg_C = 1;
int laststate_C = 0;

//
// imaginary sensors

int sensor_A = 0;
int sensor_B = 0;
int sensor_C = 0;

//
// millis time

unsigned long ultimoTempoEnvio = 0;
const long intervaloEnvio = 10000;

unsigned long ultimoTempoEnvio2 = 0;
const long intervaloEnvio2 = 2000; 

//
// MQTT conexao 

const char* mqtt_server_ip = "192.168.3.6";  // <-- Replace with your Broker IP address
const int mqtt_port = 1883;                     // Default MQTT port

WiFiClient wifiClient;              // Create a WiFi client
PubSubClient mqttClient(wifiClient); // Create an MQTT client using WiFi

String currentState = "";       // Store the current state ("on" or "off")

//

int sens_humsolo_A =34;   // ¨¨¨setar posicionamento dos sensores¨¨¨
int sens_humsolo_B =35;
int sens_humsolo_C =32;
int DHT_PIN = 33;

int valor_1=0;            //¨¨¨valores para usar no codigo¨¨¨
int valor_2=0;
int valor_3=0;
int valor_4=0;
int valor_5=0;
int valor_6=0;
int valor_7=0;
int valor_8=0;
int valor_9=0;

float read_sens_humsolo_A=0;   //¨¨¨valores para leitura¨¨¨
float read_sens_humsolo_B=0;
float read_sens_humsolo_C=0;
float read_sens_temp_A=0;
float read_sens_temp_B=0;
float read_sens_temp_C=0;
float read_sens_hum_A=0;
float read_sens_hum_B=0;
float read_sens_hum_C=0;

int valor_anterior_A=0;      //¨¨¨valores para não ficar nada espamando no serial¨¨¨
int valor_anterior_B=0;
int valor_anterior_C=0;

/*int botao_liga_A=19;        //¨¨¨Botões para ligar solenoide representam o site¨¨¨
int botao_liga_B=18;
int botao_liga_C=5;
*/                           // retirado do codigo 

int solenoide_A=0;         //¨¨¨solenoides representadas por leds¨¨¨
int solenoide_B=2;
int solenoide_C=15;

int estado_humsolo_A=0;   //¨¨¨estado para condicional para botão¨¨¨
int estado_humsolo_B=0;
int estado_humsolo_C=0;



const char* serverName = "RESOLVER LOGO ISSO"; // (em progresso)

DHTesp dhtSensor;        //¨¨¨Creio que seta como objeto¨¨¨

void connectToWiFi() {
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);

  // Wait until WiFi is connected
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void mqttCallback(char* topic, byte* payload, unsigned int length) {
  String incomingMessage = "";

  // Convert incoming byte array to String
  for (unsigned int i = 0; i < length; i++) {
    incomingMessage += (char)payload[i];
  }

  incomingMessage.trim(); // Remove trailing whitespace or newline

  if (String(topic) == "pin_state") {
    messagemqtt = incomingMessage;
    }
  // Act only if topic is "pin_state"
/*  if (String(topic) == "pin_state") {
    if (incomingMessage == "rele_on_A" && currentState != "rele_on_A") {
      Serial.println("Rele n_1 turned ON");
      currentState = "rele_on_A";
    }
    else if (incomingMessage == "rele_off_A" && currentState != "rele_off_A") {
      Serial.println("Rele n_1 turned OFF");
      currentState = "rele_off_1";
    }
    // No action needed if state hasn't changed
  }*/
}

void reconnectToMQTT() {
  while (!mqttClient.connected()) {
    Serial.print("Connecting to MQTT Client...");

    // Attempt to connect with a client ID
    if (mqttClient.connect("ESP32_S3_Client")) {
      Serial.println("connected to MQTT broker!");
      mqttClient.subscribe("pin_state"); // Subscribe to topic after successful connection
    }
    else {
      Serial.print("failed with state ");
      Serial.print(mqttClient.state());
      Serial.println(". Retrying in 5 seconds...");
      delay(5000); // Wait before retrying
    }
  }
}

void setup()
{
  Serial.begin(115200);
  /*¨¨¨¨Setando os botões para ligar como entrada¨¨¨¨*/
 /* pinMode(botao_liga_A, INPUT_PULLUP);
  pinMode(botao_liga_B, INPUT_PULLUP);
  pinMode(botao_liga_C, INPUT_PULLUP);*/             //¨¨¨retirado do codigo
/*¨¨¨¨Solenoide que serão representadas por leds¨¨¨¨*/
  pinMode(solenoide_A, OUTPUT);
  pinMode(solenoide_B, OUTPUT);
  pinMode(solenoide_C, OUTPUT);
  /*¨¨¨¨Seta o DHT22¨¨¨¨*/
   dhtSensor.setup(DHT_PIN, DHTesp::DHT22);
/*¨¨¨delay do codigo pego pela internet¨¨¨*/

  connectToWiFi();                          // Connect to WiFi
  mqttClient.setServer(mqtt_server_ip, mqtt_port); // Set MQTT Broker
  mqttClient.setCallback(mqttCallback);     // Set the callback function
}

void loop()
{
    if (!mqttClient.connected()) {
    reconnectToMQTT();
  }
  
  mqttClient.loop(); // Process incoming MQTT messages

/* ¨¨¨¨ Valores para umidade de solo para os três setores¨¨¨¨*/
  valor_7= analogRead(sens_humsolo_A);      
  read_sens_humsolo_A= map(valor_7,0,4095,0,100);
  valor_8= analogRead(sens_humsolo_B);
  read_sens_humsolo_B= map(valor_8,0,4095,0,100);
  valor_9= analogRead(sens_humsolo_C);
  read_sens_humsolo_C= map(valor_9,0,4095,0,100);

//

unsigned long tempoAtual = millis();
unsigned long tempoAtual2 = millis();

//TESTE
//nao estou suportando mais, ja sao 1:45 da manha e eu aqui ainda

/*¨¨¨¨condicionais para valores analógicos de humsolo¨¨¨¨*/
if(read_sens_humsolo_A <= 30 && cansendmsg_A == 1 || messagemqtt == "rele_on_A" && laststate_A != 1) {
  estado_humsolo_A = 1;
  laststate_A = 1;
  cansendmsg_A = 0;
  Serial.println("Setor_A_Ligado");
} else if (read_sens_humsolo_A > 30 && cansendmsg_A == 0 || messagemqtt == "rele_off_A" && laststate_A != 0) {
    estado_humsolo_A=0;
    laststate_A = 0;
    cansendmsg_A = 1;
    Serial.println("Setor_A_Desligado");
}

if(read_sens_humsolo_B <= 40 && cansendmsg_B == 1 || messagemqtt == "rele_on_B" && laststate_B != 1) {
  estado_humsolo_B = 1;
  laststate_B = 1;
  cansendmsg_B = 0;
  Serial.println("Setor_B_Ligado");
} else if (read_sens_humsolo_B > 40 && cansendmsg_B == 0 || messagemqtt == "rele_off_B" && laststate_B != 0) {
    estado_humsolo_B=0;
    laststate_B = 0;
    cansendmsg_B = 1;
    Serial.println("Setor_B_Desligado");
}

if(read_sens_humsolo_C <= 40 && cansendmsg_C == 1 || messagemqtt == "rele_on_C" && laststate_C != 1) {
  estado_humsolo_C = 1;
  laststate_C = 1;
  cansendmsg_C = 0;
  Serial.println("Setor_C_Ligado");
} else if (read_sens_humsolo_C > 30 && cansendmsg_C == 0 || messagemqtt == "rele_off_C" && laststate_C != 0) {
    estado_humsolo_C = 0;
    laststate_C = 0;
    cansendmsg_C = 1;
    Serial.println("Setor_C_Desligado");
}

//comparativos sensor em ordem
/*











*/

/*if (read_sens_humsolo_C <=60) {
    estado_humsolo_C=1;
} else {
    estado_humsolo_C=0;
}*/

/////

/*¨¨¨¨Consições para botões liga em else if e setar variaveis para tal tarefa¨¨¨¨*/
/*int valor_10= digitalRead(botao_liga_A);
int valor_11= digitalRead(botao_liga_B);
int valor_12= digitalRead(botao_liga_C);*/                     //retirado do codigo
/*¨¨¨¨Condições para ligar considerando cada setor com diferentes tempos¨¨¨¨*/
/*if(valor_10 == 0 )
{
  digitalWrite(solenoide_A, HIGH); // deixa a solenoide A ligada por 5s
  delay(5000);
  digitalWrite(solenoide_A, LOW);  // desliga a solenoide


  valor_10 = 0;                    //seta o valor para 0 para não ficar doudo tvz não precisse


}else if (valor_11 == 0)
//Ta certo o valor estar em 0???

{
  digitalWrite(solenoide_B, HIGH); // deixa a solenoide B ligada por 8s
  delay(8000);
  digitalWrite(solenoide_B, LOW);  // desliga a solenoide


  valor_11 = 0;                    // seta o valor para 0


}else if(valor_12 == 0)


{
  digitalWrite(solenoide_C, HIGH); // deixa a solenoide C ligada por 10s
  delay(10000);
  digitalWrite(solenoide_C, LOW);  // desliga a solenoide


  valor_12 = 0;                    // seta o valor para 0


}else
*/

{
  digitalWrite(solenoide_A, LOW);
  digitalWrite(solenoide_B, LOW);
  digitalWrite(solenoide_C, LOW);
}
/*¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨talvez não haja como fazer duas possibilidades de uma vez¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨¨*/

/*¨¨¨¨Mostrar valores no serial referente a umidade e temperatura¨¨¨¨*/
 TempAndHumidity data = dhtSensor.getTempAndHumidity();
 /* Serial.println("Temp: " + String(data.temperature, 2) + "°C");
  Serial.println("Humidity: " + String(data.humidity, 1) + "%");
  Serial.println("---");
  */

if((read_sens_humsolo_A <=30) || (read_sens_humsolo_B <=40) || (read_sens_humsolo_C <=60))
{
  digitalWrite(solenoide_A,estado_humsolo_A);
  digitalWrite(solenoide_B,estado_humsolo_B);
  digitalWrite(solenoide_C,estado_humsolo_C);

/*¨¨¨¨Valores no Serial referentes a umidade do solo em %¨¨¨¨*/
/*  Serial.println("A umidade do solo está em ");
  Serial.print(read_sens_humsolo_A);
  Serial.print("%");
 
  Serial.println("A umidade do solo está em ");
  Serial.print(read_sens_humsolo_B);
  Serial.print("%");

  Serial.println("A umidade do solo está em ");
  Serial.print(read_sens_humsolo_C);
  Serial.print("%");
*/
/*¨¨¨¨Copiado do exemplo da Biblioteca, feito para temperatura e umidade do ar¨¨¨¨*/
 TempAndHumidity  data = dhtSensor.getTempAndHumidity();
/*  Serial.println("Temp: " + String(data.temperature, 2) + "°C");
  Serial.println("Humidity: " + String(data.humidity, 1) + "%");
  Serial.println(valor_7);
*/
}

if (tempoAtual - ultimoTempoEnvio >= intervaloEnvio) {
  ultimoTempoEnvio = tempoAtual;
  if (WiFi.status() == WL_CONNECTED) {

      HTTPClient http;

      http.begin(serverName);
      http.addHeader("Content-Type", "application/json");

      String json = "{";
      json += "\"temperatura\":" + String(data.temperature, 2)    + ",";
      json += "\"umidade_solo_A\":" + String(read_sens_humsolo_A) + ",";
      json += "\"umidade_solo_B\":" + String(read_sens_humsolo_B) + ",";
      json += "\"umidade-solo_C\":" + String(read_sens_humsolo_C) + ",";
      json += "\"umidade\":" + String(data.humidity, 1)  ;
      json += "}";
      Serial.print(json);
      int httpResponseCode = http.POST(json); //final,, envio

      Serial.print("Código HTTP: ");
      Serial.println(httpResponseCode);

      http.end();
  }
}
}
