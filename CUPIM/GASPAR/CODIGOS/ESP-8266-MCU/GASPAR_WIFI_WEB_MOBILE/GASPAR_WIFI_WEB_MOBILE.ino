#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>

// Configurações de Wi-Fi
const char* ssid = "ControleCarro";
const char* password = "12345678";

const byte DNS_PORT = 53;
DNSServer dnsServer;
ESP8266WebServer server(80);

// Pinos da Ponte H (usando GPIO)
const int motorEsq1 = 5;   // D1 = GPIO5
const int motorEsq2 = 4;   // D2 = GPIO4
const int motorDir1 = 14;  // D5 = GPIO14
const int motorDir2 = 12;  // D6 = GPIO12
const int ledPin = LED_BUILTIN;
const int relayPin = 13;   // D7 = GPIO13 para o relé

// Estado dos botões de liga/desliga
bool ledState = false;
bool relayState = false;

void setup() {
  Serial.begin(115200);
  
  // Configuração dos pinos dos motores e relé
  pinMode(motorEsq1, OUTPUT);
  pinMode(motorEsq2, OUTPUT);
  pinMode(motorDir1, OUTPUT);
  pinMode(motorDir2, OUTPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(relayPin, OUTPUT);

  // Conexão Wi-Fi
  WiFi.softAP(ssid, password);
  Serial.println("Rede Wi-Fi iniciada");
  Serial.print("SSID: ");
  Serial.println(ssid);
  Serial.print("IP: ");
  Serial.println(WiFi.softAPIP());

  // Configuração do servidor DNS para redirecionamento
  dnsServer.start(DNS_PORT, "*", WiFi.softAPIP());

  // Configuração do servidor web
  server.on("/", HTTP_GET, handleRoot);
  server.on("/move", HTTP_GET, handleMove);
  server.on("/toggleLed", HTTP_GET, handleToggleLed);
  server.on("/toggleRelay", HTTP_GET, handleToggleRelay);
  server.on("/stop", HTTP_GET, handleStop);

  server.begin();
  Serial.println("Servidor iniciado");
}

void loop() {
  dnsServer.processNextRequest(); // Processa as solicitações DNS para redirecionamento
  server.handleClient();
}

// Função para parar o carro
void stopCar() {
  digitalWrite(motorEsq1, LOW);
  digitalWrite(motorEsq2, LOW);
  digitalWrite(motorDir1, LOW);
  digitalWrite(motorDir2, LOW);
}

// Controladores das rotas
void handleRoot() {
  String html = "<html>\
                 <head>\
                   <meta name='viewport' content='width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no'>\
                   <style>\
                     body { font-family: Arial, sans-serif; text-align: center; margin: 0; padding: 0; overflow: hidden; -webkit-user-select:none; -moz-user-select:none; -ms-user-select:none; user-select:none; }\
                     h1 { font-size: 32px; margin: 10px 0; }\
                     .button { display: inline-block; width: 300px; height: 200px; font-size: 48px; margin: 5px; cursor: pointer; border-radius: 10px; border: none; color: white; }\
                     .control-grid { display: grid; grid-template-columns: repeat(2, 1fr); gap: 200px; max-width: 400px; margin: 0 60; }\
                     .motor-btn { background-color: green; }\
                     .motor-btn.active { background-color: yellow; }\
                     .device-btn { background-color: red; }\
                     .device-btn.active { background-color: blue; }\
                     @media (max-width: 600px) {\
                       .button { width: 240px; height: 160px; font-size: 40px; }\
                       h1 { font-size: 24px; }\
                     }\
                   </style>\
                 </head>\
                 <body>\
                   <h1>Controle do Carro</h1>\
                   <div class='control-grid'>\
                     <button id='frenteEsq' class='button motor-btn' ontouchstart=\"moveCar('frenteEsq')\" ontouchend=\"stopCar('frenteEsq')\" onmousedown=\"moveCar('frenteEsq')\" onmouseup=\"stopCar('frenteEsq')\">Frente Esq</button>\
                     <button id='frenteDir' class='button motor-btn' ontouchstart=\"moveCar('frenteDir')\" ontouchend=\"stopCar('frenteDir')\" onmousedown=\"moveCar('frenteDir')\" onmouseup=\"stopCar('frenteDir')\">Frente Dir</button>\
                     <button id='trasEsq' class='button motor-btn' ontouchstart=\"moveCar('trasEsq')\" ontouchend=\"stopCar('trasEsq')\" onmousedown=\"moveCar('trasEsq')\" onmouseup=\"stopCar('trasEsq')\">Tras Esq</button>\
                     <button id='trasDir' class='button motor-btn' ontouchstart=\"moveCar('trasDir')\" ontouchend=\"stopCar('trasDir')\" onmousedown=\"moveCar('trasDir')\" onmouseup=\"stopCar('trasDir')\">Tras Dir</button>\
                   </div>\
                   <br>\
                   <button id='ledBtn' class='button device-btn' onclick=\"toggleLed()\">LED</button>\
                   <button id='relayBtn' class='button device-btn' onclick=\"toggleRelay()\">ARMA</button>\
                   <script>\
                     function moveCar(direction) {\
                       document.getElementById(direction).classList.add('active');\
                       fetch('/move?dir=' + direction);\
                     }\
                     function stopCar(direction) {\
                       document.getElementById(direction).classList.remove('active');\
                       fetch('/stop');\
                     }\
                     function toggleLed() {\
                       fetch('/toggleLed').then(response => response.text()).then(state => {\
                         document.getElementById('ledBtn').classList.toggle('active', state.includes('Ligado'));\
                       });\
                     }\
                     function toggleRelay() {\
                       fetch('/toggleRelay').then(response => response.text()).then(state => {\
                         document.getElementById('relayBtn').classList.toggle('active', state.includes('Ligado'));\
                       });\
                     }\
                   </script>\
                 </body>\
                 </html>";
  server.send(200, "text/html", html);
}




void handleMove() {
  String direction = server.arg("dir");
  Serial.print("Movendo para: ");
  Serial.println(direction);

  if (direction == "frenteEsq") {
    digitalWrite(motorEsq1, HIGH);
    digitalWrite(motorEsq2, LOW);
  } else if (direction == "trasEsq") {
    digitalWrite(motorEsq1, LOW);
    digitalWrite(motorEsq2, HIGH);
  } else if (direction == "frenteDir") {
    digitalWrite(motorDir1, HIGH);
    digitalWrite(motorDir2, LOW);
  } else if (direction == "trasDir") {
    digitalWrite(motorDir1, LOW);
    digitalWrite(motorDir2, HIGH);
  }

  server.send(200, "text/plain", "Moving " + direction);
}

void handleStop() {
  Serial.println("Parando o carro");
  stopCar();
  server.send(200, "text/plain", "Car Stopped");
}

void handleToggleLed() {
  ledState = !ledState;
  digitalWrite(ledPin, ledState ? LOW : HIGH);
  Serial.println(ledState ? "LED Ligado" : "LED Desligado");
  server.send(200, "text/plain", "LED " + String(ledState ? "Ligado" : "Desligado"));
}

void handleToggleRelay() {
  relayState = !relayState;
  digitalWrite(relayPin, relayState ? HIGH : LOW);
  Serial.println(relayState ? "Relé Ligado" : "Relé Desligado");
  server.send(200, "text/plain", "Relé " + String(relayState ? "Ligado" : "Desligado"));
}
