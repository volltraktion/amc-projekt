
#include <LiquidCrystal.h>
LiquidCrystal lcd(8, 9, 10, 11, 12, 13); // LCD-Anschluesse (Ansachluss LCD - Pin Ardunino): RS-8;E-9;D4-10;D5-11;D6-12;D7-13

long measure_distance(int trigger, int echo)
{
  long dauer, entfernung;

  digitalWrite(trigger, LOW);
  delay(5);
  digitalWrite(trigger, HIGH);
  delay(2);
  digitalWrite(trigger, LOW);
  dauer = pulseIn(echo, HIGH);

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

  while ((entfernung1 > 100) && (entfernung2 > 100))
  {
    entfernung1 = measure_distance(trigger1, echo1);

    entfernung2 = measure_distance(trigger2, echo2);
    Serial.print("entfernung1:  ");
    Serial.println(entfernung1);
    Serial.print("entfernung2:  ");
    Serial.println(entfernung2);
  }
  Serial.println("START");
  if (entfernung1 < 100 || entfernung2 < 100)
  {
    runtime1 = millis();
    if (entfernung1 < 100)
    {
      Serial.println("E1");
      digitalWrite(4, HIGH);
      do
      {
        entfernung2 = measure_distance(trigger2, echo2);
      } while (entfernung2 > 100);
      runtime2 = millis();
      digitalWrite(7, HIGH);
    }
    else
    {
      Serial.println("E2");
      digitalWrite(7, HIGH);
      do
      {
        entfernung1 = measure_distance(trigger1, echo1);
      } while (entfernung1 > 100);
      runtime2 = millis();
      digitalWrite(4, HIGH);
    }
    //}
    distance_time = runtime2 - runtime1;
    Serial.print(distance_time);
    Serial.print("ms");
    // v = s/t
    v_speed = distance / (double)distance_time;

    Serial.println(v_speed);
    lcd.setCursor(0, 1);
    lcd.print(v_speed, 2);
    lcd.print("m/s ");
    v_speed *= 313.2;
    lcd.print(v_speed, 0);
    lcd.print("km/h");
    delay(5000);
  }
  digitalWrite(4, LOW);
  digitalWrite(7, LOW);
  entfernung1 = 200;
  entfernung2 = 200;
}
