# Arduino alapú szónikus radar
A projekt céla, hogy egy mikrokontrollerrel vezérelt önműködő rendszert hozzunk létre, amiben valamilyen bemenet változás hatására automatikus beavatkozás történjen.

A mi megvalósításunkban egy ultrahangos távolságérzékelő figyeli 180°-ban a területet, s ha egy megadott távolságon belül tereptárgyat érzékel, akkor hangjelzést ad és kiírást tesz a kijelzőre; ezzel értesítve a tulajdonost.

[<img src="pics/project.png" width="350" style="display: block; margin: auto; filter: drop-shadow(5px 5px 5px #999); border-radius: 5px;" />](pics/project.png)

---
## Tartalom
<!-- TOC -->
- [Arduino alapú szónikus radar](#arduino-alapú-szónikus-radar)
  - [Tartalom](#tartalom)
  - [Feladat](#feladat)
    - [Követelmények](#követelmények)
  - [Megvalósítás](#megvalósítás)
    - [Szimulációs környezet](#szimulációs-környezet)
    - [Fizikai eszközök](#fizikai-eszközök)
      - [Áramellátás](#áramellátás)
      - [I2C LCD (kijelző)](#i2c-lcd-kijelző)
      - [Micro Servo (SG90 / MG996)](#micro-servo-sg90--mg996)
      - [Ultrasonic Sensor (HC-SR04)](#ultrasonic-sensor-hc-sr04)
      - [Active buzzer](#active-buzzer)
      - [LED (állapotjelző)](#led-állapotjelző)
    - [Programozás](#programozás)
      - [A kód](#a-kód)
      - [A működés rövid leírása](#a-működés-rövid-leírása)
      - [A működés folyamatábrája](#a-működés-folyamatábrája)
    - [Tesztelés](#tesztelés)
  - [Zárszó](#zárszó)
<!-- TOC -->

## Feladat
A _**Nyíregyházi Egyetem - [BMI1504L] - IoT alapok** (2025. őszi félév)_ elnevezésű tantárgy követelményekényt megfogalmazott automatizált rendszer megvalósítása.
A projekt megvalósítói:
- Kátai Attila
- Suller György
- Oláh Zsolt

### Követelmények
A projekt feladatnak mindenképpen tartalmaznia kell:
- egy mikrokontrollert,
- egy érzékelőt,
- egy kijelzőt,
- egy beavatkozót (aktuátort),
- egy futtatható kódot,
- egy github oldalt, ahol az eredmények elérhetők,
- dokumentációt.

## Megvalósítás

### Szimulációs környezet
A projekt megvalósításához felhasználtunk a [WOKWI szimulációs oldal](https://wokwi.com) ingyenes felületét, ahol különböző mikrokontrollereket és egyéb, hozzájuk kapcsolható eszközöket, programkönyvtáraket egybe tudtunk rendezni még azelőtt, mielőtt a fizikai eszközöket megvásároltuk volna.
Ennek előnye, hogy nagyon sok mindent le tudtunk tesztelni előre, s így ki tudtuk választani a projekt megvalósításához szükséges eszközöket.
A szimulációt az alábbi oldalon lehet elérni: [link](https://wokwi.com/projects/449700746274259969)

### Fizikai eszközök
A projekt kivitelezéséhez a következő eszközök kerültek beszerzésre:
 - [x] [Arduino Uno R3](pics/arduino.png) | 1db
 - [x] [Potenciométer (10k)](pics/potmeter.png) | 1db
 - [x] [Micro Servo (SG90)](pics/servo.png) | 1db
 - [x] [I2C LCD (16x2)](pics/lcd.png) | 1db
 - [x] [Ultrasonic Sensor (HC-SR04)](pics/sensor.png) | 1db
 - [x] [Active buzzer](pics/buzzer.png) | 1db
 - [x] [LED (piros)](pics/led.png) | 1db
 - [x] [Ellenállás (220-330 Ohm)](pics/ellenallas.png) | 1db
 - [x] [Breadboard](pics/breadboard.png) | 1db
 - [x] [Kábelezés](pics/cords.png) | 1 köteg
- Komplett szett, ami minden szükséges alkotóelemet tartalmazta: [hestore.hu: RL-SET-SUPS](https://www.hestore.hu/prod_10045701.html)

#### Áramellátás
Központi áram elosztó elem a Breadbord, ahová az Arduino kimeneti lábait kötöttük, s minden csatlakoztatott eszköz innen vételezheti a működéséhez szükséges tápellátást.
- Arduino **5V** kimenet: **+** (breadbord pozitív sín)
- Arduino **GND** kimenet: **-** (breadbord negatív sín)

#### I2C LCD (kijelző)
- **VCC**: Breadboard +5V (pozitív)
- **GND**: Breadboard GND (negatív)
- **SDA**: Arduino SDA (A4)
- **SCL**: Arduino SCL (A5)

#### Micro Servo (SG90 / MG996)
- **Piros**: Breadboard +5V (pozitív)
- **Barna/Fekete**: Breadboard GND (negatív)
- **Narancs/Fehér**: Arduino **D8** digitális pin

#### Ultrasonic Sensor (HC-SR04)
- **VCC**: Breadboard +5V (pozitív)
- **GND**: Breadboard GND (negatív)
- **Trigger**: Arduino **D2** pin
- **Echo**: Arduino **D3** pin

#### Active buzzer
- **Pozitív (+)**: Arduino **D7** pin
- **Negatív (–)**: Breadboard GND (negatív)

#### LED (állapotjelző)
- **Anód (+)**: Arduino **D6** pin
- **Katód (–)**: Breadboardra kötve → sorosan az ellenállással → GND

### Programozás
A projekt szoftveres kivitelezéséhez az Arduino IDE szolgált, ami ingyenesen letölthető, s a mindenkor szükséges, a mikrokontrollernek és kapcsolt eszközeiknek megfelelő függvénykönyvtárakkal bővíthető.
- Arduino IDE: [letöltés](https://docs.arduino.cc/software/ide-v2/tutorials/getting-started/ide-v2-downloading-and-installing/)
- LiquidCrystal I2C forráskönyvtár: [letöltés](https://www.arduinolibraries.info/libraries/liquid-crystal-i2-c)
- Micro servo forráskönyvtár: [letöltés](https://www.arduinolibraries.info/libraries/servo)

#### A kód
A futtatható kód, ami a működést biztosítja:

```c
/*
 * Arduino Radar Projekt - V2.4
 * - Ultrahangos erzekeles fix tavolsagon belul
 * - Szonar mozgatas ciklikusan szervo motor segitsegevel
 * - Aktualis ertekek megjelenitese kijelzon
 * - Opcionalis soros porti visszairas
 * - Tereptargy erzekeles eseten riasztas (hang, szoveg)
 */

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

// Wokwi szimulacio eseten 0x27,
// egyebkent altalaban 0x3C
LiquidCrystal_I2C Lcd(0x27, 16, 2); 
Servo RadarServo;

// Pin kiosztasok
const int TrigPin = 2;
const int EchoPin = 3;
const int LedPin = 6;
const int BuzzerPin = 7;
const int ServoPin = 8;
const int PotPin = A1; 

// Erzekelesi tavolsag (cm)
int RiasztasiHatarek = 50; 

// Opcionalis soros visszairas
// true  = Adatok kuldese a szamitogep fele (lassabb)
// false = Nincs adatkuldes (gyorsabb mukodes)
bool SorosKimenetKell = true; 

// Valtozok
float Tavolsag = 0;
int AktualisSzog = 0;
int LepesIrany = 2;
byte PasztazasKesleltetes; 

void setup() {
  // A soros portot mindig elinditjuk, hogy hiba eseten elerheto legyen,
  // de irni csak akkor fogunk ra, ha a fenti valtozo engedi.
  Serial.begin(9600);

  pinMode(TrigPin, OUTPUT);
  pinMode(EchoPin, INPUT);
  pinMode(LedPin, OUTPUT);
  pinMode(BuzzerPin, OUTPUT);

  RadarServo.attach(ServoPin);

  Lcd.init();
  Lcd.backlight();
  Lcd.clear();

  Lcd.setCursor(0, 0);
  Lcd.print("Arduino radar");
  Lcd.setCursor(0, 1);
  Lcd.print("Betoltes...");
  delay(2000);
  Lcd.clear();
}

float TavolsagMeres() {
  digitalWrite(TrigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(TrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(TrigPin, LOW);

  long Idotartam = pulseIn(EchoPin, HIGH, 30000); 

  if (Idotartam == 0) return 0; 
  return (Idotartam * 0.0343) / 2;
}

void loop() {
  
  // --- SEBESSEG ---
  // Minel nagyobb a potmeter erteke, annal lassabb lesz a mozgatas
  int PotErtek = analogRead(PotPin);
  PasztazasKesleltetes = map(PotErtek, 0, 1023, 120, 20);

  // --- MOZGATAS ES MERES ---
  RadarServo.write(AktualisSzog);
  delay(30); 
  Tavolsag = TavolsagMeres();

  // --- SOROS MONITOR ---
  // Csak akkor fut le, ha korabban engedelyezve volt
  if (SorosKimenetKell) {
    Serial.print("Fok: ");
    Serial.print(AktualisSzog);
    Serial.print(" | Kesleltetes: ");
    Serial.print(PasztazasKesleltetes);
    Serial.print("ms | Tav: ");
    Serial.print(Tavolsag);
    
    if (Tavolsag <= RiasztasiHatarek && Tavolsag > 0) {
       Serial.println("cm  !!! RIASZTAS !!!");
    } else {
       Serial.println("cm");
    }
  }

  // --- LCD KIJELZES ---
  Lcd.setCursor(0, 0);
  Lcd.print("Fok:");
  Lcd.print(AktualisSzog);
  Lcd.print(" "); 
  if(AktualisSzog < 100) Lcd.print(" "); 
  if(AktualisSzog < 10) Lcd.print(" ");

  Lcd.setCursor(8, 0); 
  Lcd.print("Kesl:");
  Lcd.print(PasztazasKesleltetes);
  Lcd.print(" "); 

  Lcd.setCursor(0, 1);

  if (Tavolsag <= RiasztasiHatarek && Tavolsag > 0) {
    digitalWrite(LedPin, HIGH);
    digitalWrite(BuzzerPin, HIGH);
    Lcd.print("! BEHATOLAS !   ");
  } else {
    digitalWrite(LedPin, LOW);
    digitalWrite(BuzzerPin, LOW);
    Lcd.print("Erz.tav: ");
    Lcd.print(Tavolsag, 1);
    Lcd.print("cm   ");
  }

  // Szervo ciklikus leptetese
  AktualisSzog += LepesIrany;
  if (AktualisSzog >= 180 || AktualisSzog <= 0) {
    LepesIrany = -LepesIrany;
  }

  delay(PasztazasKesleltetes);
}
```

#### A működés rövid leírása
Ez az Arduino-alapú radarrendszer környezetének folyamatos pásztázását végzi egy szervómotorra szerelt ultrahangos távolságérzékelő segítségével, amely ciklikusan, 0 és 180 fok között mozog oda-vissza. A rendszer működés közben valós időben jeleníti meg az LCD kijelző felső sorában az aktuális pásztázási szöget és a mozgás sebességét (melyet egy potméterrel szabályozhatunk: feltekerve csökken a késleltetés, így gyorsul a radar), az alsó sorban pedig az érzékelt tárgy távolságát mutatja. A biztonsági logika folyamatosan figyeli a mért adatokat: amennyiben a beállított 50 cm-es határértéken belül akadályt észlel, a készülék azonnal riasztási üzemmódba vált, melyet a LED felvillanása, a hangjelző (buzzer) sípolása és a kijelzőn megjelenő „! BEHATOLAS !” felirat jelez; nyugalmi állapotban pedig a pontos távolságot írja ki, miközben opcionálisan a számítógép felé is továbbítja az adatokat diagnosztikai célból.

#### A működés folyamatábrája
![pics/radar-flow.png](pics/radar-flow.png)

### Tesztelés
![1. teszt video](test1.mp4)
![2. teszt video](test2.mp4)

## Zárszó
Az ismeretlen eszközök, az újnak ható programozási nyelv és logika, a kezdeti technikai útvesztők leküzdése igazi elszántságot követelt, de a befektetett munka megtérült. A folyamat során rengeteget tanultunk, és a működő rendszer látványa nemcsak a siker édes ízét hozta el, hanem kaput nyitott a jövőbeni, szerteágazó és még komolyabb fejlesztések lehetősége előtt is.