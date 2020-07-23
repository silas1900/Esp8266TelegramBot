/*==============================================================================================================================================================================
 * Automação simples utilizando um bot do Telegram
 * É necessário ter a biblioteca ArduinoJson versão 5.13.5
 * 
 */
//====== Bibliotecas ===================================================================================================================================================== 

#include "CTBot.h"
#include <SimpleDHT.h>

//============================================================================================================================================================================ 

#define led1 5 //D1
#define led2 4 // D2
#define ledWifi 12 // D6

//============================================================================================================================================================================ 

int pinDHT11 = 14; //Sensor conectado ao pino D5
SimpleDHT11 dht11; //Criação do objeto para a biblioteca

// Variáveis  do estado dos leds
bool estadoLed1 = false; 
bool estadoLed2 = false;


//============================================================================================================================================================================ 

CTBot myBot;
//Configurações da rede wifi e conexao
String ssid  = "CNET TELECOM_FAMILIA ALMEIDA"; //Nome da sua rede wifi
String pass  = "almeida87"; //Senha da sua rede wifi
String token = "1111697630:AAEeqrSOJFmWD-xkytopDRe4TjPbfPbsfxk"; //Token bot Telegram

//====== protótipo da função d3e teste da conexão ===================================================================================================================================================== 
void testaCon();

//============================================================================================================================================================================   
void setup()
{
  
  Serial.begin(115200);
  Serial.println("Inicializando bot Telegram...");
  //Conexao na rede wifi
  myBot.wifiConnect(ssid, pass);
  //Define o token
  myBot.setTelegramToken(token);
  //Verifica a conexao
  if (myBot.testConnection())
    Serial.println("Conexao com Telegram Ok!");
  else
    Serial.println("Falha na conexao!");
  //Define os pinos dos leds como saida e apaga os leds
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(ledWifi, OUTPUT);
  
  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
  digitalWrite(ledWifi, LOW);
  
}
//===================================================================================================================================================================================
void loop(){

  testaCon();
  //Variavel que armazena a mensagem recebida
  TBMessage msg;
  //Verifica se chegou alguma mensagem
  if (myBot.getNewMessage(msg))
  {
    //Verifica se foi recebia a mensagem "led1 on"
    if (msg.text.equalsIgnoreCase("led1 on"))
    {
      
      estadoLed1 = true;
      digitalWrite(led1, HIGH);
      //Retorna uma mensagem informando que o led foi aceso
      myBot.sendMessage(msg.sender.id, "Led1 Aceso");
    }
    else if (msg.text.equalsIgnoreCase("led1 off"))
    {
      estadoLed1 = false;
      digitalWrite(led1, LOW);
      myBot.sendMessage(msg.sender.id, "Led 1 desligado!");
    }
    else if (msg.text.equalsIgnoreCase("led2 on"))
    {
      estadoLed2 = true;
      digitalWrite(led2, HIGH);
      myBot.sendMessage(msg.sender.id, "LED 2 Aceso!");
    }
    else if (msg.text.equalsIgnoreCase("led2 off"))
    {
      estadoLed2 = false;
      digitalWrite(led2, LOW);
      myBot.sendMessage(msg.sender.id, "Led 2 desligado!");
    }
    else if  (msg.text.equalsIgnoreCase("th"))
    {
        
        byte temperature = 0;
        byte humidity = 0;
        dht11.read(pinDHT11, &temperature, &humidity, NULL);

        float t = temperature;
        float h = humidity;
        
        myBot.sendMessage(msg.sender.id, "A temperatura é de " + String(t) + " graus celsius.\n" + "E a umidade de" +  String(h)+ "%.\n\n");
    }
     else if  (msg.text.equalsIgnoreCase("lum"))
    {
        float l = analogRead(A0);
        myBot.sendMessage(msg.sender.id, "A luminosidade é de " + String(l)+ "\n\n");
    }
     else if  (msg.text.equalsIgnoreCase("status")) 
    {
        myBot.sendMessage(msg.sender.id, "O led1 está ");
        if(estadoLed1){ // verifica o estado atual do led1
           myBot.sendMessage(msg.sender.id, "Ligado " );
        }else {
           myBot.sendMessage(msg.sender.id, "Desligado\n");
        }
        
        myBot.sendMessage(msg.sender.id, "\n" );
        myBot.sendMessage(msg.sender.id, "O led2 está");
        if(estadoLed2){// verifica o estado atual do led2
           myBot.sendMessage(msg.sender.id, "Ligado " );
        }else{
          myBot.sendMessage(msg.sender.id, "Desligado\n");
        }
    }
    else if  (msg.text.equalsIgnoreCase("start"))
    {
        String ms; // menu
        ms =   (String)"seja bem vindo(a) " + msg.sender.username + (String)".\n" +
               (String)"led1 on :  Para ligar o Led 1.\n" +
               (String)"led1 off : Para desligar o Led 1.\n" +
               (String)"led2 on :  Para ligar o Led 2.\n" +
               (String)"led2 off : Para desligar o Led 2.\n" +
               (String)"th :     Para obter temperatura e umidade.\n" +
               (String)"lum :      Para obter luminosidade.\n" +
               (String)"status :   Para obter o estado atual dos Leds.\n" +
               (String)"Muito obrigado Volte sempre!!";
        myBot.sendMessage(msg.sender.id,ms);
    }
    else
    {
      //Caso receba qualquer outro comando, envia uma de aviso
      String reply;
      reply = (String)"Ola " + msg.sender.username + (String)"! Comando inválido\n Digite start para obter o menu ";
      myBot.sendMessage(msg.sender.id, reply);
    }
  }
  delay(500);
}// end loop
//=========== Função de teste da conexão================================================================================================================================================================= 
void  testaCon(){
  if (myBot.testConnection()){ // se conectado liga o ledWifi
     digitalWrite(ledWifi, HIGH);
  }else  digitalWrite(ledWifi, LOW);
}// end loop
