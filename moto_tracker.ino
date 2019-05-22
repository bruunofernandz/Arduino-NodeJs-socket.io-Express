#include <SoftwareSerial.h>
 
SoftwareSerial esp8266(5,4);
SoftwareSerial gps(2,3);
 
#define DEBUG true
 
void setup()
{
  Serial.begin(4800);
  esp8266.begin(4800);
  gps.begin(4800);

  esp8266.listen();
  sendData("AT+RST\r\n", 2000, DEBUG); // rst
  // Conecta a rede wireless
  sendData("AT+CWJAP=\"wifissid\",\"senhawifi\"\r\n", 2000, DEBUG);
  delay(3000);
  sendData("AT+CWMODE=1\r\n", 1000, DEBUG);
  // Mostra o endereco IP
  sendData("AT+CIFSR\r\n", 1000, DEBUG);
  // Configura para multiplas conexoes
  sendData("AT+CIPMUX=1\r\n", 1000, DEBUG);
  // Inicia o web server na porta 80
  sendData("AT+CIPSERVER=1,80\r\n", 1000, DEBUG);
}
 
void loop()
{
  char entrada = 0;  
  String resposta = "";
  String token[10];
  int pos=0;
  String latitude;
  String longitude;
  String linkGoogle;

  /*// Verifica se o ESP8266 esta enviando dados
  esp8266.listen();
  if(esp8266.isListening()){
    Serial.println("esp82366 listenning...");
    delay(500);
    if (esp8266.available()>0)
    {
      if (esp8266.find("+IPD,"))
      {
        delay(300);
        int connectionId = esp8266.read() - 48;
   
        String webpage = "<head><meta http-equiv=""refresh"" content=""3"">";
        webpage += "</head><h1><u>MOTO TRACKER WEBSERVER</u></h1><br>";
        webpage += "Latitude: ";
        webpage += latitude;
        webpage += " Longitude: ";
        webpage += longitude;
        webpage += "<a href=\"";
        webpage += linkGoogle;
        webpage += "\">link</a>";
   
        String cipSend = "AT+CIPSEND=";
        cipSend += connectionId;
        cipSend += ",";
        cipSend += webpage.length();
        cipSend += "\r\n";
   
        sendData(cipSend, 1000, DEBUG);
        sendData(webpage, 1000, DEBUG);
   
        String closeCommand = "AT+CIPCLOSE=";
        closeCommand += connectionId; // append connection id
        closeCommand += "\r\n";
   
        sendData(closeCommand, 3000, DEBUG);
      }
    }
  }*/
  
  gps.listen();
  if(gps.isListening()){
    Serial.println("gps listenning...");
    while((entrada = gps.read()) != 10) {  
      if(entrada > 0)  
      resposta += entrada;  
    }
    if(!resposta.equals("")){
      Serial.println(resposta);
      String line(resposta);
      for(int i=0; i<10; i++){
        pos=line.indexOf(",");
        if(pos!=-1){
          token[i]=line.substring(0,pos);
          line=line.substring(pos+1,line.length());
          //Serial.println(token[i]);  
        } 
      }
      
      if(token[0]=="$GPGGA"){
        Serial.print("GPS_RESPONSE=");Serial.println(resposta); 
        //Serial.print("LAT=");Serial.print(token[2]);Serial.println(token[3]);
        //Serial.print("LON=");Serial.print(token[4]);Serial.println(token[5]);
        //https://www.google.com/maps/search/?api=1&query=<lat>,<lng>        
        latitude.concat(token[2]);   
        longitude.concat(token[4]);
        linkGoogle.concat("https://www.google.com/maps/search/?api=1&query=");
        linkGoogle.concat(token[2]);
        linkGoogle.concat(",");
        linkGoogle.concat(token[4]);
        Serial.print("LAT=");Serial.println(latitude);
        Serial.print("LON=");Serial.println(longitude);
        Serial.println(linkGoogle);
      }    
    }
  } 
  
}
 
String sendData(String command, const int timeout, boolean debug)
{
  // Envio dos comandos AT para o modulo
  String response = "";
  esp8266.print(command);
  long int time = millis();
  while ( (time + timeout) > millis())
  {
    while (esp8266.available())
    {
      // The esp has data so display its output to the serial window
      char c = esp8266.read(); // read the next character.
      response += c;
    }
  }
  if (debug)
  {
    Serial.println(response);
  }
  return response;
}
