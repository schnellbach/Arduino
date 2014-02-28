
#include "Tx23u.h"

#define TX23_PIN                10

Tx23u    Tx23(TX23_PIN);

void setup() {

  Serial.begin(115200);

  Serial.print("\n\nInitializing...\n");

  pinMode(TX23_PIN, INPUT);
  
  Serial.print("Tx23 sur Pin : ");
  Serial.println(TX23_PIN);
}

void loop() {

    float WindDir = Tx23.GetWindDir();
    
    delay(2000);

    Serial.print ( "Tx23 - Direction : ");
    Serial.print ( WindDir );
    Serial.print ( " Deg");
    Serial.flush();
    
    float WindSpeed = Tx23.GetWindSpeed();
    Serial.print ( " - Vitesse : ");
    Serial.print ( WindSpeed );
    Serial.print ( " m/s (");
    Serial.print ( WindSpeed * 3.6f);
    Serial.print ( " km/h)");
    Serial.println();
    Serial.flush();

}

