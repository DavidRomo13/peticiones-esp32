#include <WiFi.h>
#include <HTTPClient.h>


#include <ArduinoJson.h>


const char* ssid = "INFINITUM6BA4_2.4";
const char* password = "9GdR9pAZwB";


const char* servidor = "https://strangerthings-quotes.vercel.app/api/quotes";


String answer;
const int requestInterval = 10000;


//Elementos del objeto
const char* quote;
const char* author;


int incomingByte = 0;


void setup() {


  Serial.begin(115200);


  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  
  while(WiFi.status() != WL_CONNECTED){
    delay(100);
    Serial.print(".");
  }


  Serial.println("");
  Serial.print("Conectado a la red: ");
  Serial.println(ssid);
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
 
  
}


void loop() {


  incomingByte = serialread();


  if(incomingByte == 's'){


    answer = getRequest(servidor);
    Serial.println("\nRespuesta del servidor: ");
    Serial.println("");
    Serial.println(answer);


    //Inicio: desestructuración
    char json[500];
    answer.remove(0,1);
    int ultimo = answer.length()-1;
    answer.remove(ultimo);
    answer.toCharArray(json, 500);


    StaticJsonDocument<200> doc;
    deserializeJson(doc, json);


    quote = doc["quote"];
    author = doc["author"];
    //Fin: desestructuración


    Serial.println("");
    Serial.println("");
    Serial.println("Atributos del objeto desestructurado:");
    Serial.println("");
    Serial.print("Quote: ");
    Serial.println(quote);
    Serial.print("Author: ");
    Serial.println(author);
    
    delay(requestInterval);
    
  }
  
}




String getRequest(const char* serverName){


  HTTPClient http;
  http.begin(serverName);


  // Enviamos petición HTTP
  int httpResponseCode = http.GET();


  String payload = "...";


  if(httpResponseCode > 0){


    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
    
  }
  else {


    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  
  }


  http.end();
  return payload;
  
}




int serialread() {
  if (Serial.available() > 0) {
    // lee el byte de entrada:
    return Serial.read();
  }
  return 0;
}
