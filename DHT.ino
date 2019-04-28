#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <DHT.h>


ESP8266WebServer server(80);

DHT dht(0, DHT11);

void setup()
{
 
  Serial.begin(115200);

  WiFi.begin("SSID", "Password" );
  //Feedback caso esteja usando o Monitor Serial 
  Serial.println("");
  Serial.print("Conectando");

  
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  
  
  IPAddress ip(192, 168, 1, 11); // Vostro IP vedi da ipconfig 
  IPAddress gateway(192, 168, 1, 1);
  IPAddress subnet(255, 255, 255, 0);
  Serial.print("Configurato IP : ");
  Serial.println(ip);
 
  
  
  Serial.println("");
  Serial.println("Connessione");
  Serial.print ("IP: ");
  Serial.println(WiFi.localIP());

  server.on("/temperature", HTTP_GET, getTemperature);

 
  server.on("/humidity", HTTP_GET, getHumidity);

  
  server.on("/monitor", HTTP_GET, showMonitor);

  
  server.onNotFound(onNotFound);

  
  server.begin();
  Serial.println("Servidor HTTP iniciado");
}

void loop() 
{
  
  server.handleClient();
}


void onNotFound() 
{
  server.send(404, "text/plain", "Not Found" );
}


void getTemperature()
{
 
  float t = dht.readTemperature();
  String json = "{\"temperature\":"+String(t)+"}";
  server.send (200, "application/json", json);
}


void getHumidity()
{
 
  float h = dht.readHumidity();  
  String json = "{\"humidity\":"+String(h)+"}";  
  server.send(200, "application/json", json);
}

void showMonitor()
{
  String html =
  "<html>"
  "<head>"
    "<meta name='viewport' content='width=device-width, initial-scale=1, user-scalable=no'/>"
    "<title>DHT Monitor</title>"
    "<style type='text/css'>"
      "body{"
            "padding: 35px;"
            "background-color: #222222;"
      "}"
      "h1{"
        "color: #FFFFFF;"
        "font-family: sans-serif;"
      "}"
      "p{"
        "color: #EEEEEE;"
        "font-family: sans-serif;"
            "font-size:18px;"
      "}"
   "</style>"
  "</head>"
  "<body>"
    "<h1>DHT Monitor</h1>"
    "<p id='temperature'>Temperature: </p>"
    "<p id='humidity'>Humidity: </p>"
  "</body>"
  "<script type='text/javascript'>"
    "refresh();"  
    "setInterval(refresh, 5000);"
    "function refresh()"
    "{"
      "refreshTemperature();"
      "refreshHumidity();"
    "}"
    "function refreshTemperature()"
    "{"
      "var xmlhttp = new XMLHttpRequest();"
      "xmlhttp.onreadystatechange = function() {"
        "if (xmlhttp.readyState == XMLHttpRequest.DONE && xmlhttp.status == 200){"
          "document.getElementById('temperature').innerHTML = 'Temperature: ' + JSON.parse(xmlhttp.responseText).temperature + 'C';"
        "}"
      "};"
      "xmlhttp.open('GET', 'http://192.168.1.11/temperature', true);"
      "xmlhttp.send();"
    "}"
    "function refreshHumidity()"
    "{"
      "var xmlhttp = new XMLHttpRequest();"
      "xmlhttp.onreadystatechange = function() {"
        "if (xmlhttp.readyState == XMLHttpRequest.DONE && xmlhttp.status == 200){"
          "document.getElementById('humidity').innerHTML = 'Humidity: ' + JSON.parse(xmlhttp.responseText).humidity + '%';"
        "}"
      "};"
      "xmlhttp.open('GET', 'http://192.168.1.11/humidity', true);"
      "xmlhttp.send();"
    "}"
  "</script>"
  "</html>";
 
  server.send(200, "text/html", html);
}
