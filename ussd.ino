//code from: https://forum.arduino.cc/index.php?topic=426006.0
  
#include <SoftwareSerial.h>
#include <String.h>
//SIM800 TX is connected to Arduino D8
#define SIM800_TX_PIN 10

//SIM800 RX is connected to Arduino D7
#define SIM800_RX_PIN 11

//Create software serial object to communicate with SIM800
SoftwareSerial sim800l(SIM800_TX_PIN, SIM800_RX_PIN);
boolean creditDA;
boolean creditMo;
void initialiser();
void verifieCredit();
void recharger();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  while (!Serial);
  sim800l.begin(9600);

  recharger();
  delay(20000);
}

void loop() {
  // put your main code here, to run repeatedly:
  initialiser();
  if (creditMo)localiser();
}

void localiser() {}

void initialiser() {
  verifieCredit();
  delay(50000);
  if (creditDA)recharger();
  delay(500);
  Serial.println("initialiser");
}

void verifieCredit() {
  unsigned int i, j ;

  String s = "", l = "";
  char ch[300];
  creditMo = false;
  creditDA = false;
  sim800l.println("AT+CUSD=1");
  delay(1000);
  while (!sim800l.available()) {}
  Serial.println(sim800l.readString());
  while (!Serial.available()) {}
  sim800l.println("AT+CUSD=1,\"*222#\"");
  while (!sim800l.available()) {}
  Serial.print(sim800l.readString());
  while (!Serial.available()) {}
  Serial.print(sim800l.readString());
  while (!sim800l.available()) {}
  readSerial(ch);

  i = 0;

  delay(5000);

  for (int k = 1; k < i; k++) {
    while (!Serial.available()) {};
    Serial.print(ch[k]);
  }
  delay(5000);
  while (!Serial.available()) {}
  Serial.println("s:" + s.length()); // just to know what happning

  delay(1000);
  j = 1;
  while (j <= i) {
    if (ch[j] == 'M')
      if (ch[j + 1] == 'o')
      {

        j = j - 2;
        while (ch[j] != ' ') {
          if (ch[j] != '0')
          {
            creditMo = true;
            break;
            while (Serial.available() == 0) {}
            Serial.println("trouve");
          }
          else j = j - 1;
        }
        j = i + 1;
      }
      else {
        j = j + 1;
        while (Serial.available() == 0) {}
      }
  }
  Serial.println("non trouve Mo");

  if (!creditMo)
  {
    j = 1;
    while (j <= i) {
      if (ch[j] == 's')
        if (ch[j + 1] == 'o')
          if (ch[j + 2] == 'l')
            if (ch[j + 3] == 'd')
              if (ch[j + 4] == 'e')
              {
                j = j + 6;
                if ((ch[j] != '0') && (ch[j + 1] != ',') && (ch[j + 2] != ','))
                {
                  creditDA = true;
                }
                j = i + 1;
              }
              else
                j = j + 1;
    }
  }
  Serial.println("creditDA,creditMo");
}

void recharger() {

  sim800l.print("AT+CMGF=0\r");
  delay(300);
  sim800l.println("AT+CUSD=1,\"*143#\"");
  delay(1000);
  sim800l.println("AT+CUSD=1,\"10\"");
  delay(1000);
  sim800l.println("AT+CUSD=1,\"2\"");
  delay(1000);
  sim800l.println("AT+CUSD=1,\"1\"");
  delay(1000);
  Serial.println("Recherger");
}
/*
  Serial.println("Get data HTTP");
  //set up GPRS, connection type
  sim800l.print("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"\r");
  delay(1000);
  // set the APN
  sim800l.print("AT+SAPBR=3,1,\"internet\",\"internet\"\r");
  delay(1000);
  //tries to connect to GPRS
  sim800l.print("AT+SAPBR=1,1\r");
  delay(2000);
  // initialise embedded HTTP ruttine
  sim800l.print("AT+HTTPINIT\r");
  delay(2000);
  // set the HTTP session
  sim800l.print("AT+HTTPPARA=\"CID\",1\r");
  delay(1000);
  Serial.println("Enter the URL of the website: ");
  char url[100];
  readSerial(url);  // votre URL
  Serial.println(url);
  // set the HTTP URL
  sim800l.print("AT+HTTPPARA=\"URL\",\"");
  sim800l.print(url);
  sim800l.print("\"\r");
  delay(1000);
  // start the session
  sim800l.print("AT+HTTPACTION=0\r");
  delay(10000);
  // read the data of the HTTP server
  sim800l.print("AT+HTTPREAD\r");
  delay(1000);
  // terminate the HTTP server
  sim800l.print("AT+HTTPTERM\r");
  delay(100);
  sim800l.println();
  }*/

int readSerial(char result[]) {
  int i = 0;
  while (1) {
    while (sim800l.available() > 0) {
      char inChar = sim800l.read();
      if (inChar == '\n') {
        result[i] = '\0';
        sim800l.flush();
        return 0;
      }
      if (inChar != '\r') {
        result[i] = inChar;
        i++;
      }
    }
  }
}
