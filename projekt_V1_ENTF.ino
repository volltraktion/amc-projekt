
#include <LiquidCrystal.h>
LiquidCrystal lcd(8, 9, 10, 11, 12, 13); // LCD-Anschluesse (Ansachluss LCD - Pin Ardunino): RS-8;E-9;D4-10;D5-11;D6-12;D7-13

long measure_distance(int trigger, int echo)
{
  long dauer, entfernung;

  digitalWrite(trigger, LOW); // Rücksetzen des Senders 
  delay(5);
  digitalWrite(trigger, HIGH); // Aussenden des neuen Signals vom Ultraschallsensor
  delay(2);
  digitalWrite(trigger, LOW);
  dauer = pulseIn(echo, HIGH);  // Rückgabe in ms

  entfernung = (dauer / 2) * 0.3432;
  return entfernung;
}

int trigger1 = 2; // Anschluss Trigger des Ultraschallsensors 1 auf Pin 2
int echo1 = 3;    // Anschluss Echo des Ultraschallsensors 1 auf Pin 3

int trigger2 = 5; // Anschluss Trigger des Ultraschallsensors 2 auf Pin 5
int echo2 = 6;    // Anschluss Trigger des Ultraschallsensors 2 auf Pin 6

void setup()
{
  Serial.begin(9600);

  pinMode(trigger1, OUTPUT);
  pinMode(echo1, INPUT);

  pinMode(trigger2, OUTPUT);
  pinMode(echo2, INPUT);

  pinMode(4, OUTPUT);
  pinMode(7, OUTPUT);

  lcd.begin(16, 2);
  Serial.begin(9600);
}
void loop()
{
  unsigned long runtime1 = 0, distance_time = 0, runtime2 = 0;
  double v_speed;
  double distance = 154.94; // mm 154.94
  long entfernung1, entfernung2;

  lcd.setCursor(0, 0);
  lcd.print("Geschwindigkeit:");

  do
  {
    entfernung1 = measure_distance(trigger1, echo1); // Messen des ersten Sensors

    entfernung2 = measure_distance(trigger2, echo2); // Messen des zweiten Sensors
    Serial.print("entfernung1:  "); // Ausgabe der Entfernungen, welche durch Funktion zurückgegeben werden.
    Serial.println(entfernung1);
    Serial.print("entfernung2:  "); 
    Serial.println(entfernung2);

  } while ((entfernung1 > 100) && (entfernung2 > 100)); // Überprüfung ob Objekt Messstrecke befährt oder nicht, wenn ja Aussprung
  Serial.println("START");
  if (entfernung1 < 100 || entfernung2 < 100) // Prüfen ob Sensoren wirklich ausgelöst wurden (2. Ebene)
  {
    runtime1 = millis(); // Speichern einer Startzeit
    if (entfernung1 < 100) // Wenn Sensor 1 ausgelöst wurde, starte dieses Programm
    {
      Serial.println("E1");
      lcd.setCursor(13, 1);
      lcd.print(">>>"); // Ausgabe der Bewegungsrichtung auf dem LCD
      digitalWrite(4, HIGH);  // Schalte LED ein
      do
      {
        entfernung2 = measure_distance(trigger2, echo2); // Messen der zweiten Enfernung, solange die Entfernung größer 100mm ist.
      } while (entfernung2 > 100);
      runtime2 = millis(); // Speichern des Zeitpunktes ab dem der zweite Sensor ausgelöst wurde
      digitalWrite(7, HIGH); // Schalte LED ein.
    }
    else // Starte diese Funktionen, wenn der Sensor 2 ausgelöst wurde
    {
      Serial.println("E2");
      lcd.setCursor(13, 1);
      lcd.print("<<<"); // Ausgabe der Richtung
      digitalWrite(7, HIGH); // Einschalten der LED an zuvor ausgelöstem Sensor
      do
      {
        entfernung1 = measure_distance(trigger1, echo1);
      } while (entfernung1 > 100); // Warte bis zweiter Sensor kleiner ists
      runtime2 = millis(); // Speichere den Zeitpunkt der zweiten Auslösung
      digitalWrite(4, HIGH); // Schalte LED 2 ein.
    }
    //}
    distance_time = runtime2 - runtime1; // Berechne Zeitdifferenz zwischen Auslösepunkten
    Serial.print(distance_time);
    Serial.print("ms"); // USB Ausgabe zum Debugging

    // v = s/t
    v_speed = distance / (double)distance_time; // Berechne Geschwindigkeit

    Serial.println(v_speed);
    lcd.setCursor(0, 1);
    lcd.print(v_speed, 2);
    lcd.print("m/s "); // Ausgabe der Geschwindigkeit
    delay(5000);
  }
  digitalWrite(4, LOW);
  digitalWrite(7, LOW);
  entfernung1 = 200; // Zurücksetzen der Distanzen
  entfernung2 = 200;
}
