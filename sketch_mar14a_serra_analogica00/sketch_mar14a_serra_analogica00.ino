/*
  LiquidCrystal Library - Sensor Air Temperature and Humidity - Sensor Soil Humidity - Photoresistor

*/
// LiquidCrystal I2C - Version: Latest 
#include <LiquidCrystal_I2C.h>

// DHT sensor library - Version: Latest
#include <DHT.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal_I2C lcd(0x27, 20, 4);

// values for showing on lcd
int valueFoto;
float valueTmpAir = 0.0;
float valueHumAir = 0.0;
float valueHumSoil = 0.0;
// photoresistor transducers GL5516
const int fotoResistor = A0;
//soil humidity transducer LM393
const int sensore_umidita_terreno = A1;
//humidity and air temperature transducer DHT11
const int sensore_d11 = 2;
const int DHTTYPE = DHT11;
// button for showing values on lcd
const int button_values = A2;

// text constant
const String testoValGl5516 = "Il valore proveniente dal fotoresistore GL5516 per i LUX  è ";
const String testoValDht11 = "I valori proveniente dal DHT11 sono ";
const String testoValDht11Hum = "per l'umidità ";
const String testoValDht11Tem = "e per la temperatura ";
const String testoValLm393 = "Il valore proveniente dal LM393 per l'umidità del terreno è ";
const String testoHumDHT11 = "";
const String testoTempDHT11 = "Il valore proveniente dal LM393 per l'umidità del terreno è ";


DHT dht(sensore_d11, DHTTYPE);
//#define DHT11_PIN 2
//test per led temperatura
const int ledFreddo = 3;
const int ledMedio = 4;
const int ledCaldo = 5;
//test per led fotoresistore
const int ledFotoresistore = 12;
// number for compare temperature
float baselineTmpAir = 0.0;
// number for compare humidity
float baselineHumAir = 0.0;
//number for compare lightness
int baselineFoto = 0;
// number for compare humidity soil
float baselineHumSoil = 0.0;

// state for managed visualization values on lcd
boolean newState = LOW;
boolean oldState = LOW;
byte state = 0;

void setup() {
  // set up the LCD's number of columns and rows:
 //lcd.begin(16, 2);
 lcd.init();
 lcd.backlight();
  dht.begin();
    Serial.begin(9600);
  // here I take the values from sensors for temperature and light
  pinMode(fotoResistor, INPUT);
  pinMode(sensore_umidita_terreno, INPUT);
  // pinMode(sensore_temp_e_umidita_aria, INPUT);
  //declaration of led for temperature cold, middle and hot
  pinMode(ledFreddo, OUTPUT);
  pinMode(ledMedio, OUTPUT);
  pinMode(ledCaldo, OUTPUT);
  // declaration of led for lightness
  pinMode(ledFotoresistore, OUTPUT);
}
// method for calculate temperature and humidity from Air here the used library dht
void calcoloTmpAndHumAir() {
  // read values from sensor for temperature
   valueTmpAir = dht.readTemperature();
   valueHumAir = dht.readHumidity();
 // float valueHumAir = dht.readHumidity();
//  float t = dht.readTemperature();
float checkTemp = dht.readTemperature(true);

  if (isnan(valueHumAir) || isnan(valueTmpAir) || isnan(checkTemp)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  float hif = dht.computeHeatIndex(checkTemp, valueHumAir);
  float hic = dht.computeHeatIndex(valueTmpAir, valueHumAir, false);
   
  // Monitoring values from DHT11 Humidity and Temperature
 Serial.println(valueHumAir);
 Serial.println(valueTmpAir);
  
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(valueTmpAir);
  lcd.print((char)223);
  lcd.print("C");
  lcd.setCursor(0, 1);
  lcd.print("Humidity: ");
  lcd.print(valueHumAir);
  lcd.print("%");
  // test per i led
  //if che dichiara che il valore di temperatura è minore di quello base
  if (valueTmpAir <= baselineTmpAir) {
    digitalWrite(ledFreddo, LOW);
    digitalWrite(ledMedio, LOW);
    digitalWrite(ledCaldo, LOW);
  }
  //
  if (valueTmpAir <= baselineTmpAir + 20) {
    digitalWrite(ledFreddo, HIGH);
    digitalWrite(ledMedio, LOW);
    digitalWrite(ledCaldo, LOW);
  }
  if (valueTmpAir >= baselineTmpAir + 21 && valueTmpAir <= baselineTmpAir + 30) {
    digitalWrite(ledFreddo, LOW);
    digitalWrite(ledMedio, HIGH);
    digitalWrite(ledCaldo, LOW);
  }
  if (valueTmpAir >= baselineTmpAir + 31 && valueTmpAir < baselineTmpAir + 50) {
    digitalWrite(ledFreddo, LOW);
    digitalWrite(ledMedio, LOW);
    digitalWrite(ledCaldo, HIGH);
  }
  if (valueTmpAir >= baselineTmpAir + 51) {
    digitalWrite(ledFreddo, HIGH);
    digitalWrite(ledMedio, HIGH);
    digitalWrite(ledCaldo, HIGH);
  }
//  delay(1000);

}
//method for used photoresistor here used analogRead for read from simple transductor
void calcoloLux() {
  // va rivisto con una giornata soleggiata per rilevare il numero esatto da moltiplicareper i lux
  valueFoto = analogRead(fotoResistor);
  //qui va messa la moltiplicazione per restituire il valore esatto 10K
  lcd.setCursor(0, 0);
  lcd.print("Lux:");
  lcd.print(valueFoto);
  // test per vedere a monitor il valueFoto
  Serial.println(testoValGl5516 + valueFoto + " LUX");
  //test per led che si deve accendere per valore dal fotoresistore
  if (valueFoto < baselineFoto || valueFoto <= baselineFoto + 299) {
    digitalWrite(ledFotoresistore, HIGH);
    delay(250);
    digitalWrite(ledFotoresistore, LOW);
  }
  if (valueFoto >= baselineFoto + 300 && valueFoto <= baselineFoto + 500) {
    digitalWrite(ledFotoresistore, HIGH);
    delay(1000);
    digitalWrite(ledFotoresistore, LOW);
  }
  if (valueFoto >= baselineFoto + 501 && valueFoto <= baselineFoto + 700) {
    digitalWrite(ledFotoresistore, HIGH);
  }
}
//method for read values from simple humidity transductor
void calcoloHumSoil() {
   //Legge il valore analogico
  int sensorValue = analogRead(sensore_umidita_terreno);
   //Stampa a schermo il valore
  Serial.println(testoValLm393 + sensorValue);
  lcd.setCursor(0, 0);
  lcd.print("Ground Hum:");
  lcd.print(sensorValue);
//delay(2000); //Attende due secondi
}
void loop() {
 // calcoloTmpAndHumAir();
 // calcoloLux();
//  calcoloHumSoil();
//  delay(1000);
//  lcd.clear();

 newState = analogRead(button_values);
  if ( newState != oldState )
  {
    if ( newState == HIGH )
    {
    Serial.print("Lo stato è ");
    Serial.print(state);
      Serial.print(".");
      state++;
      if (state > 3) {
        state = 1;
      }
      
      if (state == 1) {
        lcd.clear();
        calcoloHumSoil();   // IN QUESTO METODO NON METTERE DELAY. QUELLO CHE DEVI FARE E' COME DA ESEMPIO SOTTO
      }
      else if (state == 2) {
        lcd.clear();
        calcoloLux();      // IN QUESTO METODO NON METTERE DELAY. QUELLO CHE DEVI FARE E' COME DA ESEMPIO SOTTO
      }
      else if (state == 3) {
        lcd.clear();
        calcoloTmpAndHumAir();  // IN QUESTO METODO NON METTERE DELAY. QUELLO CHE DEVI FARE E' COME DA ESEMPIO SOTTO
      }
    }
    oldState = newState;
  }

  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  //  lcd.setCursor(0, 1);
  //  print the number of seconds since reset:
  //  lcd.print(millis() / 1000);
}
