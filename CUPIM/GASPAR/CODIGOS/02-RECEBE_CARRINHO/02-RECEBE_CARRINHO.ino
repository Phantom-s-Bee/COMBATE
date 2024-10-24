


int PortaMotorLeftFront      =       22;//22
int PortaMotorLeftBack       =       23;//23
int PortaMotorRightFront     =       12;//12
int PortaMotorRightBack      =       13;//13

int i = 0;

#include "LoRaWan_APP.h"
//#include "Arduino.h"

String buffer = "";

#define RF_FREQUENCY                                915000000 // Hz

#define TX_OUTPUT_POWER                             14        // dBm

#define LORA_BANDWIDTH                              0         // [0: 125 kHz,
                                                              //  1: 250 kHz,
                                                              //  2: 500 kHz,
                                                              //  3: Reserved]
#define LORA_SPREADING_FACTOR                       7         // [SF7..SF12]
#define LORA_CODINGRATE                             1         // [1: 4/5,
                                                              //  2: 4/6,
                                                              //  3: 4/7,
                                                              //  4: 4/8]
#define LORA_PREAMBLE_LENGTH                        8         // Same for Tx and Rx
#define LORA_SYMBOL_TIMEOUT                         0         // Symbols
#define LORA_FIX_LENGTH_PAYLOAD_ON                  false
#define LORA_IQ_INVERSION_ON                        false

#define RX_TIMEOUT_VALUE                            1000
#define BUFFER_SIZE                                 50 // Define the payload size here

char txpacket[BUFFER_SIZE];
char rxpacket[BUFFER_SIZE];

char MotorLeftFront[BUFFER_SIZE];
char MotorLeftBack[BUFFER_SIZE];
char MotorRightFront[BUFFER_SIZE];
char MotorRightBack[BUFFER_SIZE];

static RadioEvents_t RadioEvents;

int16_t txNumber;

int16_t rssi,rxSize;

bool lora_idle = true;

void testeMotores(){
  digitalWrite(PortaMotorLeftFront, HIGH);
  digitalWrite(PortaMotorLeftBack, LOW);
  digitalWrite(PortaMotorRightFront, LOW);
  digitalWrite(PortaMotorRightBack, LOW);
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.println("\nON");
  delay(100);

  digitalWrite(PortaMotorLeftFront, LOW);
  digitalWrite(PortaMotorLeftBack, HIGH);
  digitalWrite(PortaMotorRightFront, LOW);
  digitalWrite(PortaMotorRightBack, LOW);
  digitalWrite(LED_BUILTIN, LOW);
  Serial.println("OFF");
  delay(100);
  
  digitalWrite(PortaMotorLeftFront, LOW);
  digitalWrite(PortaMotorLeftBack, LOW);
  digitalWrite(PortaMotorRightFront, HIGH);
  digitalWrite(PortaMotorRightBack, LOW);
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.println("ON");
  delay(100);

  digitalWrite(PortaMotorLeftFront, LOW);
  digitalWrite(PortaMotorLeftBack, LOW);
  digitalWrite(PortaMotorRightFront, LOW);
  digitalWrite(PortaMotorRightBack, HIGH);
  digitalWrite(LED_BUILTIN, LOW);
  Serial.println("OFF");
  delay(100);

  digitalWrite(PortaMotorLeftFront, HIGH);
  digitalWrite(PortaMotorLeftBack, LOW);
  digitalWrite(PortaMotorRightFront, HIGH);
  digitalWrite(PortaMotorRightBack, LOW);
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.println("ON");
  delay(100);

  digitalWrite(PortaMotorLeftFront, LOW);
  digitalWrite(PortaMotorLeftBack, HIGH);
  digitalWrite(PortaMotorRightFront, LOW);
  digitalWrite(PortaMotorRightBack, HIGH);
  digitalWrite(LED_BUILTIN, LOW);
  Serial.println("OFF");
  delay(200);
  digitalWrite(PortaMotorLeftFront, LOW);
  digitalWrite(PortaMotorLeftBack, LOW);
  digitalWrite(PortaMotorRightFront, LOW);
  digitalWrite(PortaMotorRightBack, LOW);
  digitalWrite(LED_BUILTIN, LOW);
  Serial.println("FIM");
}

void AAMotores(){
  digitalWrite(PortaMotorLeftFront, HIGH);
  digitalWrite(PortaMotorLeftBack, LOW);
  digitalWrite(PortaMotorRightFront, HIGH);
  digitalWrite(PortaMotorRightBack, LOW);

  digitalWrite(LED_BUILTIN, HIGH);
  Serial.println("\nNORTE");
}

void VVMotores(){
  digitalWrite(PortaMotorLeftFront, LOW);
  digitalWrite(PortaMotorLeftBack, HIGH);
  digitalWrite(PortaMotorRightFront, LOW);
  digitalWrite(PortaMotorRightBack, HIGH);

  digitalWrite(LED_BUILTIN, HIGH);
  Serial.println("\nSUL");
}

void OAMotores(){
  digitalWrite(PortaMotorLeftFront, LOW);
  digitalWrite(PortaMotorLeftBack, LOW);
  digitalWrite(PortaMotorRightFront, HIGH);
  digitalWrite(PortaMotorRightBack, LOW);

  digitalWrite(LED_BUILTIN, HIGH);
  Serial.println("\nLESTE");
}

void OVMotores(){
  digitalWrite(PortaMotorLeftFront, LOW);
  digitalWrite(PortaMotorLeftBack, LOW);
  digitalWrite(PortaMotorRightFront, LOW);
  digitalWrite(PortaMotorRightBack, HIGH);

  digitalWrite(LED_BUILTIN, HIGH);
  Serial.println("\nOESTE");
}

void AOMotores(){
  digitalWrite(PortaMotorLeftFront, HIGH);
  digitalWrite(PortaMotorLeftBack, LOW);
  digitalWrite(PortaMotorRightFront, LOW);
  digitalWrite(PortaMotorRightBack, LOW);

  digitalWrite(LED_BUILTIN, HIGH);
  Serial.println("\nLESTE");
}

void VOMotores(){
  digitalWrite(PortaMotorLeftFront, LOW);
  digitalWrite(PortaMotorLeftBack, HIGH);
  digitalWrite(PortaMotorRightFront, LOW);
  digitalWrite(PortaMotorRightBack, LOW);

  digitalWrite(LED_BUILTIN, HIGH);
  Serial.println("\nOESTE");
}

void AVMotores(){
  digitalWrite(PortaMotorLeftFront, HIGH);
  digitalWrite(PortaMotorLeftBack, LOW);
  digitalWrite(PortaMotorRightFront, LOW);
  digitalWrite(PortaMotorRightBack, HIGH);

  digitalWrite(LED_BUILTIN, HIGH);
  Serial.println("\nLESTE");
}

void VAMotores(){
  digitalWrite(PortaMotorLeftFront, LOW);
  digitalWrite(PortaMotorLeftBack, HIGH);
  digitalWrite(PortaMotorRightFront, HIGH);
  digitalWrite(PortaMotorRightBack, LOW);

  digitalWrite(LED_BUILTIN, HIGH);
  Serial.println("\nOESTE");
}

void OOMotores(){
  digitalWrite(PortaMotorLeftFront, LOW);
  digitalWrite(PortaMotorLeftBack, LOW);
  digitalWrite(PortaMotorRightFront, LOW);
  digitalWrite(PortaMotorRightBack, LOW);

  digitalWrite(LED_BUILTIN, HIGH);
  Serial.println("\nPARADO");
}

void inerteMotores(){
  digitalWrite(PortaMotorLeftFront, LOW);
  digitalWrite(PortaMotorLeftBack, LOW);
  digitalWrite(PortaMotorRightFront, LOW);
  digitalWrite(PortaMotorRightBack, LOW);

  digitalWrite(LED_BUILTIN, LOW);
  Serial.println("\nINERTE");
}

void setup() {
    Serial.begin(115200);
    Mcu.begin(HELTEC_BOARD,SLOW_CLK_TPYE);
    
    pinMode(PortaMotorLeftFront, OUTPUT);
    pinMode(PortaMotorLeftBack, OUTPUT);
    pinMode(PortaMotorRightFront, OUTPUT);
    pinMode(PortaMotorRightBack, OUTPUT);
    pinMode(LED_BUILTIN, OUTPUT);

    txNumber=0;
    rssi=0;
  
    RadioEvents.RxDone = OnRxDone;
    Radio.Init( &RadioEvents );
    Radio.SetChannel( RF_FREQUENCY );
    Radio.SetRxConfig( MODEM_LORA, LORA_BANDWIDTH, LORA_SPREADING_FACTOR,
                               LORA_CODINGRATE, 0, LORA_PREAMBLE_LENGTH,
                               LORA_SYMBOL_TIMEOUT, LORA_FIX_LENGTH_PAYLOAD_ON,
                               0, true, 0, 0, LORA_IQ_INVERSION_ON, true );

    testeMotores();
}

unsigned long lastCommandTime = millis();
const unsigned long timeout = 100; // Tempo em milissegundos para considerar inatividade

void loop() {
  if (lora_idle) {
    lora_idle = false;
    //Serial.println("into RX mode");
    Radio.Rx(0);
  }
  
  Radio.IrqProcess();

  // Verifica se nenhum comando de rádio foi recebido e executa a função inerteMotores()
  if (millis() - lastCommandTime >= timeout) {
    i++;
      if(i > 1000){
        inerteMotores();
        i = 0;
      }
    
    
  }
}

void OnRxDone(uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr) {
  rssi = rssi;
  rxSize = size;
  memcpy(rxpacket, payload, size);
  rxpacket[size] = '\0';
  //rxpacket[size] = '\0';
  Radio.Sleep();
  Serial.printf("\r\nIntensidade do sinal(rssi): %d , length: %d\r\n", rxpacket, rssi, rxSize);

  // Atualiza o tempo do último comando recebido
  lastCommandTime = millis();

  if (strcmp((char*)rxpacket, "Teste") == 0) {
    Serial.printf("\r\nComando recebido \"%s\"", rxpacket);
    digitalWrite(LED_BUILTIN, HIGH);
    testeMotores();
  } else if (strcmp((char*)rxpacket, "AA") == 0) {
    Serial.printf("\r\nComando recebido \"%s\"", rxpacket);
    AAMotores();
  } else if (strcmp((char*)rxpacket, "VV") == 0) {
    Serial.printf("\r\nComando recebido \"%s\"", rxpacket);
    VVMotores();
  } else if (strcmp((char*)rxpacket, "OA") == 0) {
    Serial.printf("\r\nComando recebido \"%s\"", rxpacket);
    OAMotores();
  } else if (strcmp((char*)rxpacket, "OV") == 0) {
    Serial.printf("\r\nComando recebido \"%s\"", rxpacket);
    OVMotores();
  } else if (strcmp((char*)rxpacket, "AO") == 0) {
    Serial.printf("\r\nComando recebido \"%s\"", rxpacket);
    AOMotores();
  } else if (strcmp((char*)rxpacket, "VO") == 0) {
    Serial.printf("\r\nComando recebido \"%s\"", rxpacket);
    VOMotores();
  } else if (strcmp((char*)rxpacket, "AV") == 0) {
    Serial.printf("\r\nComando recebido \"%s\"", rxpacket);
    AVMotores();
  } else if (strcmp((char*)rxpacket, "VA") == 0) {
    Serial.printf("\r\nComando recebido \"%s\"", rxpacket);
    VAMotores();
  } else if (strcmp((char*)rxpacket, "OO") == 0){
    digitalWrite(LED_BUILTIN, HIGH);
    OOMotores();
  } /*else {
    digitalWrite(LED_BUILTIN, LOW);
    Serial.printf("\r\nComando recebido \"%s\"", rxpacket);
    inerteMotores();
    Serial.printf("\r\nComandos Recebidos:");
    Serial.printf("\nMotorLeftFront - \"%s\"", PortaMotorLeftFront);
    Serial.printf("\nMotorLeftBack - \"%s\"", PortaMotorLeftBack);
    Serial.printf("\nMotorRightFront - \"%s\"", PortaMotorRightFront);
    Serial.printf("\nMotorRightBack - \"%s\"", PortaMotorRightBack);
  }*/

  lora_idle = true;
}

