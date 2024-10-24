
#include "LoRaWan_APP.h"
#include "Arduino.h"
#include <Wire.h>
#define PortaMotorLeftFront             12
#define PortaMotorLeftBack              13
#define PortaMotorRightFront            22
#define PortaMotorRightBack             23
#define OA                                        38
#define AO                                        39
#define RF_FREQUENCY                                915000000 // Hz
#define TX_OUTPUT_POWER                             5        // dBm
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
#define RX_TIMEOUT_VALUE                            100
#define BUFFER_SIZE                                 10
char txpacket[BUFFER_SIZE];
char rxpacket[BUFFER_SIZE];
double txNumber;
bool lora_idle=true;
static RadioEvents_t RadioEvents;
void OnTxDone( void );
void OnTxTimeout( void );
// Defina o tamanho adequado para o buffer txpacket
#define TX_PACKET_SIZE 50
//char txpacket[TX_PACKET_SIZE];

// Variáveis globais
unsigned long previousMillis = 0;
const long interval = RX_TIMEOUT_VALUE; // Intervalo de tempo em milissegundos


void luzes(const char* comando) {
  Serial.print(" : ");
  Serial.print(comando);
  Serial.print(" : ");
  if (strcmp(comando, "AA") == 0) {
    digitalWrite(PortaMotorLeftFront, HIGH);
    digitalWrite(PortaMotorLeftBack, LOW);
    digitalWrite(PortaMotorRightFront, HIGH);
    digitalWrite(PortaMotorRightBack, LOW);  
  } else if (strcmp(comando, "VV") == 0) {
    digitalWrite(PortaMotorLeftFront, LOW);
    digitalWrite(PortaMotorLeftBack, HIGH);
    digitalWrite(PortaMotorRightFront, LOW);
    digitalWrite(PortaMotorRightBack, HIGH); 
  } else if (strcmp(comando, "OA") == 0) {
    digitalWrite(PortaMotorLeftFront, LOW);
    digitalWrite(PortaMotorLeftBack, LOW);
    digitalWrite(PortaMotorRightFront, HIGH);
    digitalWrite(PortaMotorRightBack, LOW); 
  } else if (strcmp(comando, "OV") == 0) {
    digitalWrite(PortaMotorLeftFront, LOW);
    digitalWrite(PortaMotorLeftBack, LOW);
    digitalWrite(PortaMotorRightFront, LOW);
    digitalWrite(PortaMotorRightBack, HIGH); 
  } else if (strcmp(comando, "AO") == 0) {
    digitalWrite(PortaMotorLeftFront, HIGH);
    digitalWrite(PortaMotorLeftBack, LOW);
    digitalWrite(PortaMotorRightFront, LOW);
    digitalWrite(PortaMotorRightBack, LOW); 
  } else if (strcmp(comando, "VO") == 0) {
    digitalWrite(PortaMotorLeftFront, LOW);
    digitalWrite(PortaMotorLeftBack, HIGH);
    digitalWrite(PortaMotorRightFront, LOW);
    digitalWrite(PortaMotorRightBack, LOW); 
  } else if (strcmp(comando, "AV") == 0) {
    digitalWrite(PortaMotorLeftFront, HIGH);
    digitalWrite(PortaMotorLeftBack, LOW);
    digitalWrite(PortaMotorRightFront, LOW);
    digitalWrite(PortaMotorRightBack, HIGH); 
  } else if (strcmp(comando, "VA") == 0) {
    digitalWrite(PortaMotorLeftFront, LOW);
    digitalWrite(PortaMotorLeftBack, HIGH);
    digitalWrite(PortaMotorRightFront, HIGH);
    digitalWrite(PortaMotorRightBack, LOW); 
  }else if (strcmp(comando, "OO") == 0){
    digitalWrite(PortaMotorLeftFront, LOW);
    digitalWrite(PortaMotorLeftBack, LOW);
    digitalWrite(PortaMotorRightFront, LOW);
    digitalWrite(PortaMotorRightBack, LOW); 
    digitalWrite(LED_BUILTIN, HIGH);
  }
}


void testeMotores() {
  unsigned long currentMillis = millis(); // Obtém o tempo atual

  // Verifica se o intervalo de tempo passou
  if (lora_idle == true && currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis; // Atualiza o tempo anterior
    txNumber += 0.01;

    const char* commands[] = {"AA", "VV", "OA", "OV", "AO", "VO", "AV", "VA", "OO"};
    for (int i = 0; i < 9; i++) {
      // Prepara o pacote
      snprintf(txpacket, TX_PACKET_SIZE, "%s", commands[i]); 
      Serial.printf("\r\nsending packet \"%s\" , length %d\r\n", txpacket, strlen(txpacket));

      // Verifique se o tamanho do pacote não excede o buffer
      size_t packetLength = strlen(txpacket);
      if (packetLength < TX_PACKET_SIZE) {
        // Envia o pacote
        Radio.Send((uint8_t *)txpacket, packetLength);
      } else {
        Serial.println("Erro: Pacote excede o tamanho do buffer");
      }

      if (strcmp(commands[i], "AA") == 0) {
        digitalWrite(PortaMotorLeftFront, HIGH);
        digitalWrite(PortaMotorLeftBack, LOW);
        digitalWrite(PortaMotorRightFront, HIGH);
        digitalWrite(PortaMotorRightBack, LOW);  
      } else if (strcmp(commands[i], "VV") == 0) {
        digitalWrite(PortaMotorLeftFront, LOW);
        digitalWrite(PortaMotorLeftBack, HIGH);
        digitalWrite(PortaMotorRightFront, LOW);
        digitalWrite(PortaMotorRightBack, HIGH); 
      } else if (strcmp(commands[i], "OA") == 0) {
        digitalWrite(PortaMotorLeftFront, LOW);
        digitalWrite(PortaMotorLeftBack, LOW);
        digitalWrite(PortaMotorRightFront, HIGH);
        digitalWrite(PortaMotorRightBack, LOW); 
      } else if (strcmp(commands[i], "OV") == 0) {
        digitalWrite(PortaMotorLeftFront, LOW);
        digitalWrite(PortaMotorLeftBack, LOW);
        digitalWrite(PortaMotorRightFront, LOW);
        digitalWrite(PortaMotorRightBack, HIGH); 
      } else if (strcmp(commands[i], "AO") == 0) {
        digitalWrite(PortaMotorLeftFront, HIGH);
        digitalWrite(PortaMotorLeftBack, LOW);
        digitalWrite(PortaMotorRightFront, LOW);
        digitalWrite(PortaMotorRightBack, LOW); 
      } else if (strcmp(commands[i], "VO") == 0) {
        digitalWrite(PortaMotorLeftFront, LOW);
        digitalWrite(PortaMotorLeftBack, HIGH);
        digitalWrite(PortaMotorRightFront, LOW);
        digitalWrite(PortaMotorRightBack, LOW); 
      } else if (strcmp(commands[i], "AV") == 0) {
        digitalWrite(PortaMotorLeftFront, HIGH);
        digitalWrite(PortaMotorLeftBack, LOW);
        digitalWrite(PortaMotorRightFront, LOW);
        digitalWrite(PortaMotorRightBack, HIGH); 
      } else if (strcmp(commands[i], "VA") == 0) {
        digitalWrite(PortaMotorLeftFront, LOW);
        digitalWrite(PortaMotorLeftBack, HIGH);
        digitalWrite(PortaMotorRightFront, HIGH);
        digitalWrite(PortaMotorRightBack, LOW); 
      } else if (strcmp(commands[i], "OO") == 0) {
        digitalWrite(PortaMotorLeftFront, LOW);
        digitalWrite(PortaMotorLeftBack, LOW);
        digitalWrite(PortaMotorRightFront, LOW);
        digitalWrite(PortaMotorRightBack, LOW); 
        digitalWrite(LED_BUILTIN, HIGH); 
      }

      delay(RX_TIMEOUT_VALUE);
    }
    lora_idle = false;
  }
  digitalWrite(PortaMotorLeftFront, LOW);
  digitalWrite(PortaMotorLeftBack, LOW);
  digitalWrite(PortaMotorRightFront, LOW);
  digitalWrite(PortaMotorRightBack, LOW); 
  Radio.IrqProcess();
}



void setup() {
  Serial.begin(115200);
  Mcu.begin(HELTEC_BOARD,SLOW_CLK_TPYE);
  pinMode(PortaMotorLeftFront, OUTPUT);
  pinMode(PortaMotorLeftBack, OUTPUT);
  pinMode(PortaMotorRightFront, OUTPUT);
  pinMode(PortaMotorRightBack, OUTPUT);

  pinMode(OA, INPUT);
  pinMode(AO, INPUT);

  txNumber=0;

  RadioEvents.TxDone = OnTxDone;
  RadioEvents.TxTimeout = OnTxTimeout;
    
  Radio.Init( &RadioEvents );
  Radio.SetChannel( RF_FREQUENCY );
  Radio.SetTxConfig( MODEM_LORA, TX_OUTPUT_POWER, 0, LORA_BANDWIDTH,
                                 LORA_SPREADING_FACTOR, LORA_CODINGRATE,
                                 LORA_PREAMBLE_LENGTH, LORA_FIX_LENGTH_PAYLOAD_ON,
                                 true, 0, 0, LORA_IQ_INVERSION_ON, 3000 ); 

  Radio.IrqProcess( );
  testeMotores();
}


void loop()
{

  Serial.println();
  Serial.println();
  Serial.println();
  Serial.println();

// OA
// OV
// AO
// VO

  
  Serial.print(OA);
  Serial.print(" : ");
  Serial.println(analogRead(OA));
  
  Serial.print(AO);
  Serial.print(" : ");
  Serial.println(analogRead(AO));
  
  Serial.println(" ");
  Serial.print("BOTÃO ACIONADO: ");

if(analogRead(OA) == 0 && analogRead(AO)){
  sprintf(txpacket,"OO");
  luzes(txpacket);
  Radio.Send( (uint8_t *)txpacket, strlen(txpacket) ); //send the package out	
}
//delay(10); // Pequeno delay para garantir a estabilização do pino
  
  if(analogRead(OA) >= 50 && analogRead(OA) <= 1000 ){
    if(analogRead(AO) >= 50 && analogRead(AO) <= 1000 ){
      sprintf(txpacket,"VV");  //start a package
      luzes(txpacket);
      Radio.Send( (uint8_t *)txpacket, strlen(txpacket) ); //send the package out	
      lora_idle = false;
      Serial.print("BTN: ");
      Serial.print(analogRead(AO));
      Serial.println(" VV: ON");
    }else if(analogRead(AO) > 1000){
      sprintf(txpacket,"AV");  //start a package
      luzes(txpacket);
      Radio.Send( (uint8_t *)txpacket, strlen(txpacket) ); //send the package out	
      lora_idle = false;
      Serial.print("BTN: ");
      Serial.print(analogRead(AO));
      Serial.println(" AV: ON");
    }else if(lora_idle == true){
      sprintf(txpacket,"OV");  //start a package
      luzes(txpacket);
      Radio.Send( (uint8_t *)txpacket, strlen(txpacket) ); //send the package out	
      lora_idle = false;
      Serial.print("BTN: ");
      Serial.print(analogRead(OA));
      Serial.println(" OV: ON");
    }else{
      Serial.print("BTN: ");
      Serial.print(analogRead(OA));
      Serial.println(" OV: OFF");
    }
  }
  //delay(10); // Pequeno delay para garantir a estabilização do pino
  if(analogRead(AO) >= 50 && analogRead(AO) <= 1000 ){
    if(analogRead(OA) >= 50 && analogRead(OA) <= 1000 ){
      sprintf(txpacket,"VV");  //start a package
      luzes(txpacket);
      Radio.Send( (uint8_t *)txpacket, strlen(txpacket) ); //send the package out	
      lora_idle = false;
      Serial.print("BTN: ");
      Serial.print(analogRead(OA));
      Serial.println(" VV: ON");
    }else if(analogRead(OA) > 1000){
      sprintf(txpacket,"VA");  //start a package
      luzes(txpacket);
      Radio.Send( (uint8_t *)txpacket, strlen(txpacket) ); //send the package out	
      lora_idle = false;
      Serial.print("BTN: ");
      Serial.print(analogRead(OA));
      Serial.println(" VA: ON");
    }else if(lora_idle == true){
      sprintf(txpacket,"VO");  //start a package
      luzes(txpacket);
      Radio.Send( (uint8_t *)txpacket, strlen(txpacket) ); //send the package out	
      lora_idle = false;
      Serial.print("BTN: ");
      Serial.print(analogRead(AO));
      Serial.println(" VO: ON");
    }else{
      Serial.print("BTN: ");
      Serial.print(analogRead(OA));
      Serial.println(" OV: OFF");
    }
  }


  if(analogRead(OA) > 1000){
    if(analogRead(AO) > 1000){
      sprintf(txpacket,"AA");  //start a package
      luzes(txpacket);
      Radio.Send( (uint8_t *)txpacket, strlen(txpacket) ); //send the package out	
      lora_idle = false;
      Serial.print("BTN: ");
      Serial.print(analogRead(AO));
      Serial.println(" AA: ON");
    }else if(analogRead(AO) >= 50 && analogRead(AO) <= 1000 ){
      sprintf(txpacket,"VA");  //start a package
      luzes(txpacket);
      Radio.Send( (uint8_t *)txpacket, strlen(txpacket) ); //send the package out	
      lora_idle = false;
      Serial.print("BTN: ");
      Serial.print(analogRead(AO));
      Serial.println(" VA: ON");
    }else if(lora_idle == true){
      sprintf(txpacket,"OA");  //start a package
      luzes(txpacket);
      Radio.Send( (uint8_t *)txpacket, strlen(txpacket) ); //send the package out	
      lora_idle = false;
      Serial.print("BTN: ");
      Serial.print(analogRead(OA));
      Serial.println(" OV: ON");
    }else{
      Serial.print("BTN: ");
      Serial.print(analogRead(OA));
      Serial.println(" OV: OFF");
    }
  }

//delay(10); // Pequeno delay para garantir a estabilização do pino
  if(analogRead(AO) > 1000 ){
    if(analogRead(OA) > 1000){
      sprintf(txpacket,"AA");  //start a package
      luzes(txpacket);
      Radio.Send( (uint8_t *)txpacket, strlen(txpacket) ); //send the package out	
      lora_idle = false;
      Serial.print("BTN: ");
      Serial.print(analogRead(OA));
      Serial.println(" AA: ON");
    }else if(analogRead(OA) >= 50 && analogRead(OA) <= 1000 ){
      sprintf(txpacket,"AV");  //start a package
      luzes(txpacket);
      Radio.Send( (uint8_t *)txpacket, strlen(txpacket) ); //send the package out	
      lora_idle = false;
      Serial.print("BTN: ");
      Serial.print(analogRead(OA));
      Serial.println(" AV: ON");
    }else if(lora_idle == true){
      sprintf(txpacket,"AO");  //start a package
      luzes(txpacket);
      Radio.Send( (uint8_t *)txpacket, strlen(txpacket) ); //send the package out	
      lora_idle = false;
      Serial.print("BTN: ");
      Serial.print(analogRead(AO));
      Serial.println(" AO: ON");
    }else{
      Serial.print("BTN: ");
      Serial.print(analogRead(OA));
      Serial.println(" OV: OFF");
    }
  }
  
if(analogRead(AO) == 0 && analogRead(OA) == 0){
  sprintf(txpacket,"OO");  //start a package
      luzes(txpacket);
      Radio.Send( (uint8_t *)txpacket, strlen(txpacket) ); //send the package out	
      lora_idle = false;
      
}
  //delay(10); // Pequeno delay para garantir a estabilização do pino

  delay(100); // Ajuste o delay conforme necessário para a taxa de leitura desejada

  /*
    if(lora_idle == true)
	  {
      delay(1000);
      //txNumber ++;
      //sprintf(txpacket,"Hello world number %0.2f",txNumber);  //start a package
      sprintf(txpacket,"AA");  //start a package
    
    
      Serial.printf("\r\nsending packet \"%s\" , length %d\r\n",txpacket, strlen(txpacket));

      Radio.Send( (uint8_t *)txpacket, strlen(txpacket) ); //send the package out	
      lora_idle = false;
    }
    //Radio.IrqProcess( );
  
  */
  
  
/*
	if(lora_idle == true)
	{
    delay(1000);
		txNumber += 0.01;
		//sprintf(txpacket,"Hello world number %0.2f",txNumber);  //start a package
		sprintf(txpacket,"A");  //start a package
   
		Serial.printf("\r\nsending packet \"%s\" , length %d\r\n",txpacket, strlen(txpacket));

		Radio.Send( (uint8_t *)txpacket, strlen(txpacket) ); //send the package out	
    lora_idle = false;
	}
  */


////digitalWrite(PortaMotorLeftFront, LOW);
        ////digitalWrite(PortaMotorLeftBack, LOW);
        ////digitalWrite(PortaMotorRightFront, LOW);
        ////digitalWrite(PortaMotorRightBack, LOW); 
  //testeMotores();
  Radio.IrqProcess( );
}

void OnTxDone( void )
{
	Serial.println("TX done......");
	lora_idle = true;
}

void OnTxTimeout( void )
{
    Radio.Sleep( );
    Serial.println("TX Timeout......");
    lora_idle = true;
}