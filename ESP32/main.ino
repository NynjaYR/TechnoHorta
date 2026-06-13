#include "DHTesp.h"             //¨¨¨importa a biblioteca do dht22¨¨¨
#include <WiFi.h>               //¨¨¨adiciona o wifi¨¨¨
#include <HTTPClient.h>
//#include <PubSubClient.h>       //¨¨¨MQTT¨¨¨
//#include <button.h>             //¨¨¨Botao mqtt¨¨¨
//#include <ESPAsyncWebServer.h>  //¨¨¨Inclui biblioteca para servidor web assíncrono¨¨¨
//#include <AsyncTCP.h>           //¨¨¨Inclui a biblioteca para TCP assíncrono¨¨¨
//#include "html_page.h"          //¨¨¨Inclui o arquivo HTML pagina na web¨¨¨
// (Desmarcar oque for ultilizar)


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


const char* ssid = "Сомнительный Wi-Fi";    //¨¨¨nome variavel yehudi se vira¨¨¨ (isso é o nome da rede e não uma variavel qualquer, explique melhor suas anotações.)
const char* password =  "244466666"; //¨¨¨senha da rede yehudi se vira¨¨¨ (...)


const char* serverName = "RESOLVER LOGO ISSO"; // (em progresso)


DHTesp dhtSensor;        //¨¨¨Creio que seta como objeto¨¨¨


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


  WiFi.begin(ssid, password);
  Serial.print("Conectando");


  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }


  Serial.println("\nWiFi conectado!");
}


void loop()
{
/* ¨¨¨¨ Valores para umidade de solo para os três setores¨¨¨¨*/
  valor_7= analogRead(sens_humsolo_A);      
  read_sens_humsolo_A= map(valor_7,0,4095,0,100);
  valor_8= analogRead(sens_humsolo_B);
  read_sens_humsolo_B= map(valor_8,0,4095,0,100);
  valor_9= analogRead(sens_humsolo_C);
  read_sens_humsolo_C= map(valor_9,0,4095,0,100);






/*¨¨¨¨condicionais para valores analógicos de humsolo¨¨¨¨*/
if(read_sens_humsolo_A <=30)
{


  estado_humsolo_A=1;


}else
{


  estado_humsolo_A=0;


}
if(read_sens_humsolo_B <=40)
{


estado_humsolo_B=1;


}else
{


estado_humsolo_B=0;


}
if(read_sens_humsolo_C <=60)
{


estado_humsolo_C=1;


}else
{


estado_humsolo_C=0;


}








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
  Serial.println("Temp: " + String(data.temperature, 2) + "°C");
  Serial.println("Humidity: " + String(data.humidity, 1) + "%");
  Serial.println("---");
  delay(2000);


if((read_sens_humsolo_A <=30) || (read_sens_humsolo_B <=40) || (read_sens_humsolo_C <=60))
{
  digitalWrite(solenoide_A,estado_humsolo_A);
  digitalWrite(solenoide_B,estado_humsolo_B);
  digitalWrite(solenoide_C,estado_humsolo_C);
 
/*¨¨¨¨Valores no Serial referentes a umidade do solo em %¨¨¨¨*/
  Serial.println("A umidade do solo está em ");
  Serial.print(read_sens_humsolo_A);
  Serial.print("%");
 
  Serial.println("A umidade do solo está em ");
  Serial.print(read_sens_humsolo_B);
  Serial.print("%");


  Serial.println("A umidade do solo está em ");
  Serial.print(read_sens_humsolo_C);
  Serial.print("%");


/*¨¨¨¨Copiado do exemplo da Biblioteca, feito para temperatura e umidade do ar¨¨¨¨*/
 TempAndHumidity  data = dhtSensor.getTempAndHumidity();
  Serial.println("Temp: " + String(data.temperature, 2) + "°C");
  Serial.println("Humidity: " + String(data.humidity, 1) + "%");
  delay(2000);
  Serial.println(valor_7);
}


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


  delay(10000);
}
