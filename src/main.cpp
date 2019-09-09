#include <Arduino.h>

#define TINY_GSM_MODEM_SIM800      // Modem is SIM800
#define TINY_GSM_RX_BUFFER   1024  // Set RX buffer to 1Kb

#include <TinyGsmClient.h>




#define MODEM_RST             5
#define MODEM_PWKEY           4
#define MODEM_POWER_ON       23
#define MODEM_TX             27
#define MODEM_RX             26


TinyGsm modem(Serial1);
TinyGsmClient client(modem);

boolean IsConnected = false ;
boolean IsGprsConnected = false ;
int RetriesConnections = 100 ; 
int RetriesGprsConnections = 100 ; 

const char apn[]      = "FNETNRJ"; // APN (example: internet.vodafone.pt) use https://wiki.apnchanger.org
const char gprsUser[] = ""; // GPRS User
const char gprsPass[] = ""; // GPRS Password


void setup() {
  Serial.begin(115200); 
  Serial.println("Starting Arduino tracker.") ;
  
 // Set modem reset, enable, power pins
  pinMode(MODEM_PWKEY, OUTPUT);
  pinMode(MODEM_RST, OUTPUT);
  pinMode(MODEM_POWER_ON, OUTPUT);
  digitalWrite(MODEM_PWKEY, LOW);
  digitalWrite(MODEM_RST, HIGH);
  digitalWrite(MODEM_POWER_ON, HIGH);

  // Set GSM module baud rate and UART pins
  Serial1.begin(115200, SERIAL_8N1, MODEM_RX, MODEM_TX);
  delay(3000);

  Serial.println("Initializing modem...");
  modem.restart();


  Serial.print("IMEI : ");
  Serial.println (modem.getIMEI());

String modemInfo = modem.getModemInfo();
  Serial.print("Modem: ");
  Serial.println(modemInfo);

  Serial.print("Connecting to APN: ");
  Serial.println(apn);
  modem.gprsConnect(apn, gprsUser, gprsPass); 
  
  
  do {
    IsConnected = modem.waitForNetwork() ; 
    if (!IsConnected) {
    Serial.println(" Not Connected");
    delay(5000) ; 
  }
  else {
    Serial.println(" Connected");
  }

  if (modem.isNetworkConnected()) {
    Serial.println ("Network connected");
  }

  } while ((!IsConnected) && (RetriesConnections-- > 0)) ; 
  
  Serial.print ("GPRS status:" );
  do {
     IsGprsConnected = modem.isGprsConnected();
    if (!IsGprsConnected) {
    Serial.println(" No GPRS Connection");
    delay(5000) ; 
  }
  else {
    Serial.println("GPRS Connection");
  }
} while ((!IsGprsConnected) && (RetriesGprsConnections-- > 0)) ;
  
  String modemOperator = modem.getOperator() ; 
  Serial.print("Operator: ");
  Serial.println(modemOperator);
 
  IPAddress local = modem.localIP();
  Serial.print ("Local IP: ");
  Serial.println ( local);

  int csq = modem.getSignalQuality();
  Serial.print ("Signal quality: ");
  Serial.println ( csq);

 String modemDate = modem.getGSMDateTime (DATE_DATE) ; 
  Serial.print("Date: ");
  Serial.println(modemDate);

 String modemTime = modem.getGSMDateTime (DATE_TIME) ; 
  Serial.print("Time: ");
  Serial.println(modemTime);

  String modemLocation = modem.getGsmLocation () ; 
  Serial.print("location: ");
  Serial.println(modemLocation);

    modem.poweroff();


}

void loop() {
  // put your main code here, to run repeatedly:
}