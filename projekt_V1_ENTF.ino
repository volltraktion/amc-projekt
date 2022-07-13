#include <LiquidCrystal.h>
LiquidCrystal lcd(8,9,10,11,12,13);


int trigger1 = 2; 
int echo1 = 3;

int trigger2 = 5;
int echo2 = 6;

long dauer1 = 0, dauer2 = 0;
long entfernung1 = 0, entfernung2 = 0; 
void setup()
{
  Serial.begin (9600); 
  
  pinMode(trigger1, OUTPUT); 
  pinMode(echo1, INPUT); 
  
  pinMode(trigger2, OUTPUT);
  pinMode(echo2, INPUT);

  pinMode(4, OUTPUT);
  pinMode(7, OUTPUT);
  
  lcd.begin(16,2);
  Serial.begin(9600);
  lcd.setCursor(0,0);
  lcd.print("Entfernung:");
}
void loop()
{
  unsigned long runtime = 0, distance_time = 0, runtime2 = 0;
  double v_speed;
  double distance = 154.94; //mm 154.94

  runtime = millis();
  
  lcd.setCursor(0,0);
  lcd.print("Geschwindigkeit:");
  
  digitalWrite(trigger1, LOW);
  delay(5);
  digitalWrite(trigger1, HIGH);
  delay(10);
  digitalWrite(trigger1, LOW);
  dauer1 = pulseIn(echo1, HIGH);  
  
  entfernung1 = (dauer1 / 2) * 0.3432;
  

  if (entfernung1 < 300){
    digitalWrite(4, HIGH);
    do {
        digitalWrite(trigger2, LOW);
        delay(5);
        digitalWrite(trigger2, HIGH);
        delay(10);
        digitalWrite(trigger2, LOW);
        dauer2 = pulseIn(echo2, HIGH);

        entfernung2 = (dauer2 / 2) * 0.3432;
        runtime2 = millis();
    } while(entfernung2 > 300);
    distance_time = runtime2 - runtime;
    Serial.print(distance_time);
    Serial.print("ms");
    digitalWrite(7, HIGH);
    // v = s/t
    v_speed = distance / (double) distance_time;
    // v_speed *= 3.6;

    Serial.println(v_speed);
    lcd.setCursor(0,1);
    lcd.print(v_speed);
    lcd.print(" m/s");
    delay(5000);
  }
  
  digitalWrite(4, LOW);
  digitalWrite(7, LOW);
  
}
