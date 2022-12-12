#include <SoftwareSerial.h>
#include <ThingerESP8266.h>
#include <ESP8266WiFi.h>

SoftwareSerial DataSerial(12, 13);

unsigned long previousMillis = 0;
const long interval = 3000;

String arrData[4];

#define USERNAME "AldiPutra"
#define DEVICE_ID "Tugas_Akhir_CecepAldi"
#define DEVICE_CREDENTIAL "PJTTd1?2CrB7!GB2"

ThingerESP8266 thing(USERNAME, DEVICE_ID, DEVICE_CREDENTIAL);

const char* ssid = "Tokyooo";
const char* password = "12345678";

int kelembaban, hujan, cahaya;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  DataSerial.begin(9600);

  WiFi.begin(ssid, password);

  thing.add_wifi(ssid, password);

  thing["Data"] >> [](pson & out){
    out["Kelembaban"] = kelembaban;
    out["Hujan"] = hujan;
    out["Cahaya"] = cahaya;
  };
}

void loop() {
  // put your main code here, to run repeatedly:
  unsigned long currentMillis = millis();
  if(currentMillis - previousMillis >= interval)
  {
    previousMillis = currentMillis;

    String data = "";
    while(DataSerial.available()>0)
    {
      data += char(DataSerial.read());
    }

    data.trim();
    
    if(data != "")
    {
      int index = 0;
      for(int i=0; i<= data.length(); i++)
      {
        char delimiter = '#';
        if(data[i] != delimiter)
          arrData[index] += data[i];
        else
          index++;
      }
      if(index ==2 )
      {
        Serial.println("Kelembaban  : " + arrData[0]);
        Serial.println("Hujan       : " + arrData[1]);
        Serial.println("Cahaya      : " + arrData[2]);
        Serial.println();
      }
      
      kelembaban   = arrData[0].toInt();
      hujan        = arrData[1].toInt();
      cahaya       = arrData[2].toInt();

      thing.handle();
      
      arrData[0] = "";
      arrData[1] = "";
      arrData[2] = "";
     
    }

    DataSerial.println("Ya");
  }
}
