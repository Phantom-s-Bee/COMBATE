
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>

// Configurações de Wi-Fi
const char* ssid = "ControleCarro";
const char* password = "20242025";


const byte DNS_PORT = 53;
String ssidd = "CRASH: ";
String nome = "";
String macClient = "";
IPAddress apIP(172, 217, 28, 1);
DNSServer dnsServer;
ESP8266WebServer server(80);

// Pinos da Ponte H (usando GPIO)
const int motorEsq1 = 5;   // D1 = GPIO5
const int motorEsq2 = 4;   // D2 = GPIO4
const int motorDir1 = 14;  // D5 = GPIO14
const int motorDir2 = 12;  // D6 = GPIO12
const int ledPin    = LED_BUILTIN;
const int relayPin  = 13;   // D7 = GPIO13 para o relé

// Estado dos botões de liga/desliga
bool ledState = false;
bool relayState = false;


String responseHTML = ""
                      "<!DOCTYPE html><html lang='pt-br'><head>"
                      "<meta name='viewport' content='width=device-width'>"
                      "<title>Primeiro Acesso</title></head><body>"
                      "<h1>Qual seu nome?</h1><form method='post'><input type='text' placeholder='Nome'><input type='submit'></form>"
                      "</body></html>";




void listarDispositivosConectados() {
  struct station_info *station = wifi_softap_get_station_info();

  Serial.println("Dispositivos conectados:");
  while (station != NULL) {
    Serial.printf("MAC: %02X:%02X:%02X:%02X:%02X:%02X\n",
                  station->bssid[0], station->bssid[1], station->bssid[2],
                  station->bssid[3], station->bssid[4], station->bssid[5]);
    station = STAILQ_NEXT(station, next);
  }
  wifi_softap_free_station_info();
}


String opsPage  = ""
                  "<!DOCTYPE html><html lang='pt-br'><head><meta charset='UTF-8'>"
                  "<meta name='viewport' content='width=device-width, initial-scale=1.0'>"
                  "<title>Bem-vindo</title></head><body style='font-family:Arial, sans-serif; text-align:center; padding:20px;'>"
                  "<h1>Aguarde o , " + nome + " terminar!</h1>"
                  "</body></html>";




  


String formPage = R"rawliteral(

  <!DOCTYPE html>
    <html lang="pt-br">
      <head>
        <meta charset="UTF-8">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <title>Captive Portal</title>
        <style>
          body {
            font-family: Arial, sans-serif;
            text-align: center;
            padding: 20px;
            background-color: #f9f9f9;
          }
          h1 {
            color: #333;
          }
          input[type="text"] {
            width: 80%;
            padding: 10px;
            margin: 10px 0;
            font-size: 16px;
            border: 1px solid #ccc;
            border-radius: 5px;
          }
          input[type="submit"] {
            background-color: #007BFF;
            color: white;
            border: none;
            padding: 10px 20px;
            font-size: 16px;
            border-radius: 5px;
            cursor: pointer;
          }
          input[type="submit"]:hover {
            background-color: #0056b3;
          }
        </style>
        <script>
          function validaNome() {
            var nome = document.getElementById('nome').value;
            if (nome.trim() === '') {
              alert('Por favor, escreva seu nome.');
              return false;
            }
            window.location.href = '/bemvindo?nome=' + encodeURIComponent(nome);
            return false;
          }
        </script>
      </head>
      <body>
        <h1>Qual seu nome?</h1>
        <form onsubmit="return validaNome();">
          <input type="text" id="nome" placeholder="Digite seu nome">
          <input type="submit" value="Enviar">
        </form>
      </body>
      </html>
      )rawliteral";






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
   WiFi.mode(WIFI_AP);
  ssidd +=  WiFi.macAddress();
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0)); // Configura o IP do AP

  WiFi.softAP(ssidd, password);
  Serial.println("Rede Wi-Fi iniciada");
  Serial.print("SSID: ");
  Serial.println(ssidd);
  Serial.println(ssid);
  Serial.print("IP: ");
  Serial.println(WiFi.softAPIP());

  // Configuração do servidor DNS para redirecionamento
  dnsServer.start(DNS_PORT, "*", apIP);

  // Configuração do servidor web
  server.on("/", []() {// Rota para a página inicial
    server.send(200, "text/html", formPage);
    Serial.print("1: ");
    Serial.println(nome);
  });
  // Rota para a página de boas-vindas
  server.on("/bemvindo", handleWelcomePage);
  server.on("/opsPage", handleOpsPage);

  server.on("/controleTank", HTTP_GET, handleRootTank);
  server.on("/controleJoystick", HTTP_GET, handleRootJoystick);
  server.on("/controleBTN", HTTP_GET, handleRootBTN);
  server.on("/move", HTTP_GET, handleMove);
  server.on("/toggleLed", HTTP_GET, handleToggleLed);
  server.on("/toggleRelay", HTTP_GET, handleToggleRelay);
  server.on("/stop", HTTP_GET, handleStop);

  // replay to all requests with same HTML
  server.onNotFound([]() {
    if(nome == ""){
      server.send(200, "text/html", formPage);
      Serial.print("2: ");
      Serial.println(nome);
    }else{
      String opsPage = ""
  "<!DOCTYPE html><html lang='pt-br'><head><meta charset='UTF-8'>"
  "<meta name='viewport' content='width=device-width, initial-scale=1.0'>"
  "<title>Bem-vindo</title>"
  "<style>body { font-family: Arial, sans-serif; text-align: center; padding: 20px; }</style>"
  "<script>"
  "let contador = 5;"  // Contador regressivo de 5 segundos
  "function atualizarContador() {"
  "  document.getElementById('contador').innerText = contador;"
  "  if (contador <= 0) {"
  "    window.location.href = '/controleTank';"  // Redireciona ao zerar
  "  } else {"
  "    contador--;"
  "    setTimeout(atualizarContador, 1000);"
  "  }"
  "}"
  "window.onload = atualizarContador;"
  "</script>"
  "</head><body>"
  "<h1>Aguarde o , " + nome + " terminar!</h1>"
  "<p>Redirecionando em <span id='contador'>5</span>...</p>"
  "</body></html>";


      server.send(200, "text/html", opsPage);
      Serial.print("3: ");
      Serial.println(nome);
    }
  });
  server.begin();
  Serial.println("Servidor iniciado");
  Serial.print("4: ");
  Serial.println(nome);
}

void loop() {
  server.handleClient();
  dnsServer.processNextRequest(); // Processa as solicitações DNS para redirecionamento
  server.handleClient();



  dnsServer.processNextRequest();
  server.handleClient();
  /*
  // Mostra os dispositivos conectados no monitor serial
  static unsigned long ultimaVerificacao = 0;
  if (millis() - ultimaVerificacao > 5000) { // A cada 5 segundos
    listarDispositivosConectados();
    ultimaVerificacao = millis();
  }
  */

}

// Função para parar o carro
void stopCar() {
  digitalWrite(motorEsq1, LOW);
  digitalWrite(motorEsq2, LOW);
  digitalWrite(motorDir1, LOW);
  digitalWrite(motorDir2, LOW);
}


void handleWelcomePage() {
  nome = server.arg("nome");
  String responseHTML = "<!DOCTYPE html><html lang='pt-br'><head><meta charset='UTF-8'>";
  responseHTML += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
  responseHTML += "<title>Bem-vindo</title>";
  responseHTML += "<style>body { font-family: Arial, sans-serif; text-align: center; padding: 20px; }</style>";
  
  // Adiciona o script do contador regressivo
  responseHTML += "<script>";
  responseHTML += "let contador = 3;";  // Inicia o contador em 3
  responseHTML += "function atualizarContador() {";
  responseHTML += "  document.getElementById('contador').innerText = contador;";
  responseHTML += "  if (contador <= 0) {";
  responseHTML += "    window.location.href = '/controleJoystick';";  // Redireciona ao chegar em 0
  responseHTML += "  } else {";
  responseHTML += "    contador--;";  // Decrementa o contador
  responseHTML += "    setTimeout(atualizarContador, 1000);";  // Chama a função a cada 1 segundo
  responseHTML += "  }";
  responseHTML += "}";
  responseHTML += "window.onload = atualizarContador;";  // Inicia o contador ao carregar a página
  responseHTML += "</script>";
  
  responseHTML += "</head><body>";
  responseHTML += "<h1>Bem-vindo, " + nome + "!</h1>";
  responseHTML += "<p>Redirecionando em <span id='contador'>3</span>...</p>";
  responseHTML += "</body></html>";
  
  server.send(200, "text/html", responseHTML);
}


void handleOpsPage() {
  nome = server.arg("nome");
  String responseHTML = "<!DOCTYPE html><html lang='pt-br'><head><meta charset='UTF-8'>";
  responseHTML += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
  responseHTML += "<title>Bem-vindo</title></head><body style='font-family:Arial, sans-serif; text-align:center; padding:20px;'>";
  responseHTML += "<h1>Aguarde o , " + nome + " terminar!</h1>";
  responseHTML += "</body></html>";
  server.send(200, "text/html", opsPage);
}

void handleRootTank() {
  String html = "<html>\
                 <head>\
                   <meta name='viewport' content='width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no'>\
                   <style>\
                     body { font-family: Arial, sans-serif; text-align: center; margin: 0; padding: 0; overflow: hidden; height: 100vh; display: flex; flex-direction: column; justify-content: center; }\
                     h1 { font-size: 5vw; margin: 10px 0; }\
                     .button { display: inline-block; width: 40vw; height: 20vw; font-size: 5vw; margin: 5px; cursor: pointer; border-radius: 10px; border: none; color: white; }\
                     .button { -webkit-user-select: none; -moz-user-select: none; -ms-user-select: none;  user-select: none; -webkit-touch-callout: none; }\
                     .control-grid { display: grid; grid-template-columns: repeat(2, 1fr); gap: 5vw; max-width: 90vw; margin: auto; }\
                     .motor-btn { background-color: green; }\
                     .motor-btn.active { background-color: yellow; }\
                     .device-btn { background-color: red; }\
                     .device-btn.active { background-color: blue; }\
                     @media (max-width: 600px) {\
                       .button { width: 80px; height: 60px; font-size: 12px; }\
                       h1 { font-size: 20px; }\
                     }\
                   </style>\
                 </head>\
                 <body>\
                   <h1>Controle do Carro - Tanque</h1>\
                   <h2><a href='\controleJoystick'>Trocar de controle</a></h2>\
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

void handleRootJoystick() {
  const char* html = R"rawliteral(
  <!DOCTYPE html>
  <html lang="pt">
  <head>
    <meta name='viewport' content='width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no'>
    <style>
      body {
        font-family: Arial, sans-serif; text-align: center; margin: 0; padding: 0; 
        height: 100vh; display: flex; flex-direction: column; justify-content: center; 
        overflow: hidden;
      }
      h1 { font-size: 5vw; margin: 10px 0; }
      #joystick-container {
        width: 50vw; height: 50vw; margin: auto; background-color: #f0f0f0;
        border-radius: 50%; position: relative; user-select: none;
      }
      #joystick {
        width: 30%; height: 30%; background-color: gray; border-radius: 50%;
        position: absolute; top: 35%; left: 35%; touch-action: none; user-select: none;
      }
      .button {
        width: 40vw; height: 20vw; font-size: 5vw; margin: 5px; cursor: pointer;
        border: none; border-radius: 10px; background-color: green; color: white;
      }
      .button:active { background-color: yellow; }
      .active { background-color: blue !important; }
      @media (max-width: 600px) {
        .button { width: 80px; height: 60px; font-size: 12px; }
        h1 { font-size: 20px; }
      }
    </style>
  </head>
  <body>
    <h1>Controle do Carro - Joystick</h1>
    <h2><a href='/controleBTN'>Trocar de controle</a></h2>
    <div id='joystick-container'>
      <div id='joystick'></div>
    </div>
    <br>
    <button class='button' id='ledBtn' onclick="toggleLed()">LED</button>
    <button class='button' id='relayBtn' onclick="toggleRelay()">ARMA</button>
    <br>
    <button class='button' onclick="rotateCar('esquerda')">Girar Esquerda</button>
    <button class='button' onclick="rotateCar('direita')">Girar Direita</button>

    <script>
      const joystick = document.getElementById('joystick');
      const container = document.getElementById('joystick-container');
      const ledBtn = document.getElementById('ledBtn');
      const relayBtn = document.getElementById('relayBtn');

      const centerX = container.offsetWidth / 2;
      const centerY = container.offsetHeight / 2;
      let dragging = false;

      container.addEventListener('touchstart', () => dragging = true);
      container.addEventListener('touchmove', dragJoystick);
      container.addEventListener('touchend', stopJoystick);

      function dragJoystick(event) {
        if (!dragging) return;
        const touch = event.touches[0];
        const rect = container.getBoundingClientRect();
        let x = touch.clientX - rect.left;
        let y = touch.clientY - rect.top;

        // Ensure joystick stays within bounds
        x = Math.max(0, Math.min(x, container.offsetWidth));
        y = Math.max(0, Math.min(y, container.offsetHeight));

        joystick.style.left = (x - joystick.offsetWidth / 2) + 'px';
        joystick.style.top = (y - joystick.offsetHeight / 2) + 'px';

        sendDirection(x - centerX, y - centerY);
      }

      function stopJoystick() {
        dragging = false;
        joystick.style.left = '35%';
        joystick.style.top = '35%';
        fetch('/stop');
      }

      function sendDirection(dx, dy) {
        const threshold = 20;
        if (Math.abs(dy) > Math.abs(dx)) {
          fetch(`/move?dir=${dy < -threshold ? 'frente' : 'tras'}`);
        } else if (Math.abs(dx) > threshold) {
          fetch(`/move?dir=${dx > 0 ? 'Dir' : 'Esq'}`);
        }
      }

      function toggleLed() {
        fetch('/toggleLed')
          .then(response => response.text())
          .then(state => ledBtn.classList.toggle('active', state.includes('Ligado')));
      }

      function toggleRelay() {
        fetch('/toggleRelay')
          .then(response => response.text())
          .then(state => relayBtn.classList.toggle('active', state.includes('Ligado')));
      }

      function rotateCar(direction) {
        fetch(`/rotate?dir=${direction}`);
      }
    </script>
  </body>
  </html>
  )rawliteral";

  server.send(200, "text/html", html);
}

void handleRootBTN() {
  String html = "<html>\
                 <head>\
                   <meta name='viewport' content='width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no'>\
                   <style>\
                     body { font-family: Arial, sans-serif; text-align: center; margin: 0; padding: 0; overflow: hidden; height: 100vh; display: flex; flex-direction: column; justify-content: center; align-items: center; }\
                     h1 { font-size: 5vw; margin: 10px 0; }\
                     .button { display: inline-block; width: 40vw; height: 20vw; font-size: 5vw; margin: 5px; cursor: pointer; border-radius: 10px; border: none; color: white; }\
                     .button { -webkit-user-select: none; -moz-user-select: none; -ms-user-select: none;  user-select: none; -webkit-touch-callout: none; }\
                     .control-grid { display: flex; justify-content: center; align-items: center; margin: auto; }\
                     table { margin: auto; }\
                     .motor-btn { background-color: green; }\
                     .motor-btn.active { background-color: yellow; }\
                     .device-btn { background-color: red; }\
                     .device-btn.active { background-color: blue; }\
                     .footer { position: fixed; bottom: 0; width: 100%; display: flex; justify-content: center; gap: 5vw; padding: 10px; background-color: #f1f1f1; }\
                     @media (max-width: 600px) {\
                       .button { width: 80px; height: 60px; font-size: 12px; }\
                       h1 { font-size: 20px; }\
                     }\
                   </style>\
                 </head>\
                 <body>\
                   <h1>Controle do Carro - BTN</h1>\
                   <h2><a href='/controleTank'>Trocar de controle</a></h2>\
                   <div class='control-grid'>\
                     <table>\
                       <tr><th>-</th><th><button id='frente' class='button motor-btn' ontouchstart=\"moveCar('frente')\" ontouchend=\"stopCar('frente')\" onmousedown=\"moveCar('frente')\" onmouseup=\"stopCar('frente')\">Frente</button></th><th>-</th></tr>\
                       <tr><th><button id='Esq' class='button motor-btn' ontouchstart=\"moveCar('Esq')\" ontouchend=\"stopCar('Esq')\" onmousedown=\"moveCar('Esq')\" onmouseup=\"stopCar('Esq')\">Esquerda</button></th><th>-</th><th><button id='Dir' class='button motor-btn' ontouchstart=\"moveCar('Dir')\" ontouchend=\"stopCar('Dir')\" onmousedown=\"moveCar('Dir')\" onmouseup=\"stopCar('Dir')\">Direita</button></th></tr>\
                       <tr><th>-</th><th><button id='tras' class='button motor-btn' ontouchstart=\"moveCar('tras')\" ontouchend=\"stopCar('tras')\" onmousedown=\"moveCar('tras')\" onmouseup=\"stopCar('tras')\">Tras</button></th><th>-</th></tr>\
                       <tr><th><button id='SpinEsq' class='button motor-btn' ontouchstart=\"moveCar('SpinEsq')\" ontouchend=\"stopCar('SpinEsq')\" onmousedown=\"moveCar('SpinEsq')\" onmouseup=\"stopCar('SpinEsq')\">Girar Esquerda</button></th><th>-</th><th><button id='SpinDir' class='button motor-btn' ontouchstart=\"moveCar('SpinDir')\" ontouchend=\"stopCar('SpinDir')\" onmousedown=\"moveCar('SpinDir')\" onmouseup=\"stopCar('SpinDir')\">Girar Direita</button></th></tr>\
                     </table>\
                   </div>\
                   <div class='footer'>\
                     <button id='ledBtn' class='button device-btn' onclick=\"toggleLed()\">LED</button>\
                     <button id='relayBtn' class='button device-btn' onclick=\"toggleRelay()\">ARMA</button>\
                   </div>\
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

  //Modo BTN
  else if (direction == "frente") {
    digitalWrite(motorEsq1, HIGH);
    digitalWrite(motorDir1, HIGH);
    digitalWrite(motorDir2, LOW);
    digitalWrite(motorEsq2, LOW);
  } else if (direction == "tras") {
    digitalWrite(motorEsq1, LOW);
    digitalWrite(motorDir1, LOW);
    digitalWrite(motorDir2, HIGH);
    digitalWrite(motorEsq2, HIGH);
  } else if (direction == "Esq") {
    digitalWrite(motorEsq1, LOW);
    digitalWrite(motorDir1, HIGH);
    digitalWrite(motorDir2, LOW);
    digitalWrite(motorEsq2, LOW);
  } else if (direction == "Dir") {
    digitalWrite(motorEsq1, HIGH);
    digitalWrite(motorDir1, LOW);
    digitalWrite(motorDir2, LOW);
    digitalWrite(motorEsq2, LOW);
  } else if (direction == "SpinEsq") {
    digitalWrite(motorEsq1, LOW);
    digitalWrite(motorDir1, HIGH);
    digitalWrite(motorDir2, LOW);
    digitalWrite(motorEsq2, HIGH);
  } else if (direction == "SpinDir") {
    digitalWrite(motorEsq1, HIGH);
    digitalWrite(motorDir1, LOW);
    digitalWrite(motorDir2, HIGH);
    digitalWrite(motorEsq2, LOW);
  }//

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
