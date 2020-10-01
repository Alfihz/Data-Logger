#include <DHT.h>
#include <TimeLib.h>
#include <SPI.h>
#include <SD.h>

#define DHTPIN 2
#define DHTTYPE DHT11
#define SDPIN 4

DHT dht(DHTPIN, DHTTYPE);

char string[50];
String str;
char stringSD[500];
int count = 0;
String incomingData;
char incData[50];

File sdFile;

void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);
  dht.begin();
  SD.begin(SDPIN);

  /*incomingData = Serial.readString();
  Serial.println(incomingData);
  incomingData.toCharArray(incData, 50);
  if(strcmp(incData, "TESTING") == 0)
  {
    Serial.print("Pass!");
  }
  else
  {
    Serial.print("Fail");
  }*/
}

void loop() {
  // put your main code here, to run repeatedly:

  setTime(22, 10, 30, 9, 12, 2018);
  //Serial.print(timeStatus());

  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  str = String(day()) + "-" + String(month()) + "-" + String(year()) + "-" + String(hour()) + "-" + String(minute()) + ";" + String(t) + ";" + String(h) + "\n";
  str.toCharArray(string, 50);

  strcat(stringSD, string);

  
  if(count == 5)
  {
    strcat(stringSD, "EOF\n");
    //Serial.write(stringSD); 

    //Write to SD card
    sdFile = SD.open("data.txt", FILE_WRITE);   //opens a file data.txt 
    if(sdFile)                                  //if file is open without errors then only data is written in data.txt
    {
      sdFile.write(stringSD);
      sdFile.close();                           //to close the file
    }

    //Read SD card
    sdFile = SD.open("data.txt");
    if(sdFile)
    {
      while(sdFile.available())
        Serial.write(sdFile.read());
    }
    sdFile.close();
    
    count = 0;
    memset(stringSD, 0, sizeof stringSD);
  }
  //string = char(h) + ";" + char(t);
  //Serial.write(string);
  //Serial.write("\n");

  count++;
  delay(2000);                          //for delay generation of 2sec
}



