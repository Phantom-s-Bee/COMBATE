#include <NetworkConfig.hpp>
#include <RoteadorWiFi.hpp>
#include <Servidor.hpp>


const NetworkConfig networkConfig(
	IPAddress(172, 217, 28, 1),
	IPAddress(172, 217, 28, 1),
	IPAddress(255, 255, 255, 0));

RoteadorWiFi roteadorWiFi(networkConfig);

Servidor servidor(networkConfig);

// Pinos da Ponte H (usando GPIO)
const int pinMotorEsquerdo1 = 5;   // D1 = GPIO5
const int pinMotorEsquerdo2 = 4;   // D2 = GPIO4
const int pinMotorDireito1 = 14;  // D5 = GPIO14
const int pinMotorDireito2 = 12;  // D6 = GPIO12
const int pinLed    = LED_BUILTIN;
const int pinRelay  = 13;   // D7 = GPIO13 para o relé

// Estado dos botões de liga/desliga
bool ledState = false;
bool relayState = false;




void setup() 
{
	Serial.begin(115200);
	
	
	roteadorWiFi.begin();
	servidor.begin();
	

	// // Configuração do servidor web
	// server.on("/", []() {// Rota para a página inicial
	// 	server.send(200, "text/html", responseHTML);
	// });

	// // replay to all requests with same HTML
	// server.onNotFound([]() {
	// 	server.send(200, "text/html", responseHTML);
	// });

	// server.begin();
}

void loop() 
{
	// server.handleClient();
	servidor.update();
} 