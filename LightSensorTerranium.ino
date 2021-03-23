#include "Adafruit_VEML7700.h"

Adafruit_VEML7700 veml;

#define ALS_LOW_THRESHOLD 10000
#define ALS_HIGH_THRESHOLD 20000

bool als_low_sent = false;
bool als_high_sent =  false;

void setup(){

  Serial.begin(115200);
  delay(5000);

  Serial.println("Adafruit VEML7700 Test");

  if (!veml.begin()){
    Serial.println("Sensor not found");
    while (1);
  }
  Serial.println("Sensor found");
  veml.setGain(VEML7700_GAIN_1);
  veml.setIntegrationTime(VEML7700_IT_800MS);
  Serial.print(F("Gain: "));

  switch (veml.getGain()){
    
    case VEML7700_GAIN_1:
    Serial.println("1");    break;

    case VEML7700_GAIN_2:
    Serial.println("2");    break;

    case VEML7700_GAIN_1_4:
    Serial.println("1/4");  break;

    case VEML7700_GAIN_1_8:
    Serial.println("1/8");  break;
  }

  Serial.print(F("Integration Time (ms): "));

  switch (veml.getIntegrationTime()){

    case VEML7700_IT_25MS:
    Serial.println("25");   break;

    case VEML7700_IT_50MS:
    Serial.println("50");   break;

    case VEML7700_IT_100MS:
    Serial.println("100");  break;

    case VEML7700_IT_200MS:
    Serial.println("200");  break;

    case VEML7700_IT_400MS:
    Serial.println("400");  break;

    case VEML7700_IT_800MS:
    Serial.println("800");  break;
  }
  veml.setLowThreshold(ALS_LOW_THRESHOLD);
  veml.setHighThreshold(ALS_HIGH_THRESHOLD);
  veml.interruptEnable(true);
}

void loop()
{
  Serial.print("Lux: ");
  Serial.println(veml.readLux());

  Serial.print("White: ");
  Serial.println(veml.readWhite());

  uint16_t als = veml.readALS();
  Serial.print("Raw ALS: ");
  Serial.println(als);
  
  String als_s = String(als);

  if((als > ALS_HIGH_THRESHOLD) && (als_high_sent == false)){
      Serial.println("** High threshold");
      Particle.publish("ALS_Level_high", als_s);
      als_high_sent =  true;
      als_low_sent = false;
  }
  else if ((als < ALS_LOW_THRESHOLD) && (als_low_sent == false)){
      Serial.println("** Low threshold");
      Particle.publish("ALS_Level_low", als_s);
      als_low_sent = true;
      als_high_sent = false;
  }
  delay(10000);
}