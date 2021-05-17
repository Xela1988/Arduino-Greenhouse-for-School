/*
  LiquidCrystal Library - Sensor Air Temperature and Humidity - Sensor Soil Humidity - Photoresistor - Button
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
// photoresistor transductor GL5516
const int fotoResistor = A0;
//soil humidity transductor LM393
const int sensore_umidita_terreno = A1;
//humidity and air temperature transducer DHT11
const int sensore_d11 = 2;
const int DHTTYPE = DHT11;
// button for showing values on lcd
const int button_values = A2;

// text constant for print on lcd or serial monitor of Ide

// text for print on lcd and serial monitor values from DHT11 transductor
const String testoFailedDHT11 = "Failed to read from DHT sensor!";
const String testoValDHT11 = "I valori proveniente dal DHT11 sono ";
const String testoValDHT11Hum = "% per l'umidità e ";
const String testoValDHT11Tem = "° per la temperatura ";
const String lcdPrintDHT11Temp = "Temp: ";
const String lcdPrintC = "C";
const String lcdPrintDHT11Hum = "Humidity: ";
// text for print on lcd and serial monitor values from Soil transductor GL5516
const String testoValGl5516 = "Il valore proveniente dal fotoresistore GL5516 per i LUX  è ";
const String lux = "Lux: " ;
const String luxCapitalize = " LUX";
// text for print on lcd and serial monitor values from Soil transductor LM393
const String testoValLM393 = "Il valore proveniente dal LM393 per l'umidità del terreno è ";
const String lcdPrintLM393Hum = "Ground Hum:";
const String tenere = "Tenere";
const String premuto = "premuto";
const String ilPulsante = "il pulsante";
const String per = "per";
const String visualizzare = "visualizzare";
const String iValori = "i valori";
const String loStatoE = "Lo stato è ";


DHT dht(sensore_d11, DHTTYPE);
//constant for led temperature from DHT11
const int ledFreddo = 3;
const int ledMedio = 4;
const int ledCaldo = 5;
//test for led photoresistor GL5516
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
  // set up the different LCD's number of columns and rows:
  // lcd.begin(16, 2);
  // start the ICQUANZX Display LCD serial IIC I2C TWI 1602
  lcd.init();
  // lcd light activated
  lcd.backlight();
  // start DHT11 transductor
  dht.begin();
  // Start listening port 9960
  Serial.begin(9600);
  // here set pin for different component
  pinMode(fotoResistor, INPUT);
  pinMode(sensore_umidita_terreno, INPUT);
  //declaration of led for temperature cold, middle and hot
  pinMode(ledFreddo, OUTPUT);
  pinMode(ledMedio, OUTPUT);
  pinMode(ledCaldo, OUTPUT);
  // declaration of led for lightness
  pinMode(ledFotoresistore, OUTPUT);
}
// method for calculate temperature and humidity from Air here the used library dht
void calcoloTmpAndHumAir() {

  // read values from sensor for temperature and humidity
  valueTmpAir = dht.readTemperature();
  valueHumAir = dht.readHumidity();
  float checkTemp = dht.readTemperature(true);

  // control if valueHumAir valueTmpAir and checkTemp is Not a Null
  if (isnan(valueHumAir) || isnan(valueTmpAir) || isnan(checkTemp)) {
    Serial.println(testoFailedDHT11);
    return;
  }

  float hif = dht.computeHeatIndex(checkTemp, valueHumAir);
  float hic = dht.computeHeatIndex(valueTmpAir, valueHumAir, false);

  // Monitoring values from DHT11 Humidity and Temperature QUI VA MESSO IL LOG PER IL MONITOR
  Serial.println(testoValDHT11 + valueHumAir + testoValDHT11Hum + valueTmpAir +testoValDHT11Tem);

  lcd.setCursor(0, 0);
  lcd.print(lcdPrintDHT11Temp);
  lcd.print(valueTmpAir);
  lcd.print((char)223);
  lcd.print(lcdPrintC);
  lcd.setCursor(0, 1);
  lcd.print(lcdPrintDHT11Hum);
  lcd.print(valueHumAir);
  lcd.print("%");
  // led test
  //control if valueTmpAir is less than base temperature
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
}
//method for used photoresistor here used analogRead for read from simple transductor
void calcoloLux() {
  // the sunny day with the the maximum lightness the GL5516 detect the valuesabout 970-1000
  valueFoto = analogRead(fotoResistor);
  //recall method for read the values from button
  newState = digitalRead(button_values);
  //qui va messa la moltiplicazione per restituire il valore esatto 10K
  lcd.setCursor(0, 0);
  lcd.print(lux);
  lcd.print(valueFoto);
  // test per vedere a monitor il valueFoto
  Serial.println(testoValGl5516 + valueFoto + luxCapitalize);
  //test for turn on and turn off the led yellow with values from GL5516
  if (newState == LOW) {
    if (valueFoto < baselineFoto || valueFoto <= baselineFoto + 400) {
      digitalWrite(ledFotoresistore, HIGH);
      delay(1000);
      digitalWrite(ledFotoresistore, LOW);
    }
    if (valueFoto >= baselineFoto + 401 && valueFoto <= baselineFoto + 599) {
      digitalWrite(ledFotoresistore, HIGH);
      delay(3000);
      digitalWrite(ledFotoresistore, LOW);
    }
    if (valueFoto >= baselineFoto + 600) {
      digitalWrite(ledFotoresistore, LOW);
    }
  }
}
//method for read values from simple ground humidity transductor LM393
void calcoloHumSoil() {
  // turn off the led from DHTL11
  turnOffLedDHT11();
  //reading analog value from soil transductor LM393
  int sensorValue = analogRead(sensore_umidita_terreno);
  //Print in serial monitor
  Serial.println(testoValLM393 + sensorValue);
  //print on lcd Ground humidity and his value
  lcd.setCursor(0, 0);
  lcd.print(lcdPrintLM393Hum);
  lcd.print(sensorValue);
}

//beginning method for user instruction
void welcome() {
  newState = digitalRead(button_values);
  if (newState == LOW) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(tenere);
    lcd.setCursor(0, 1);
    lcd.print(premuto);
    delay(1500);
  }
  newState = digitalRead(button_values);
  if (newState == LOW) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(ilPulsante);   lcd.setCursor(0, 1);
    lcd.print(per);
    delay(1500);
  }
  newState = digitalRead(button_values);
  if (newState == LOW) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(visualizzare);
    lcd.setCursor(0, 1);
    lcd.print(iValori);
    delay(1500);
  }
}

// method for closed light from led of the DHT11
void turnOffLedDHT11() {
  digitalWrite(ledFreddo, LOW);
  digitalWrite(ledMedio, LOW);
  digitalWrite(ledCaldo, LOW);
}

void loop() {
  newState = digitalRead(button_values);
  if ( newState != oldState )
  {
    if ( newState == HIGH )
    {
      Serial.println(loStatoE +state);
      state++;
      if (state > 3) {
        state = 1;
      }

      if (state == 1) {
        lcd.clear();
        calcoloHumSoil();
      }
      else if (state == 2) {
        lcd.clear();
        calcoloLux();
      }
      else if (state == 3) {
        lcd.clear();
        calcoloTmpAndHumAir();
      }
    }
   oldState = newState;
  } else {
    if (state == 0)
    {
      welcome();
    }

    if (state > 3) {
      state = 1;
    }
    if (state == 1) {
      calcoloHumSoil();
    }
    else if (state == 2) {
      calcoloLux();
    }
    else if (state == 3) {
      calcoloTmpAndHumAir();
    }
  }
}
