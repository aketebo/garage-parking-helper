#include <IRremote.h>

enum Color {
  redColor,
  greenColor,
  noColor
};

struct Light {
  bool on;
  Color color;
};

// Distance pins
int distanceEchoPin = 12;
int distanceTriggerPin = 10;

// Codes for bluetooth light
long red = 0xFF906F;
long green = 0xFF10EF;
long turnOn = 0xFFE01F;

IRsend irsend;
Light light;
int dataLength = 32; // bits
int distanceLimit = 10; // in centimeters

void setup() {
  // Setup output
  Serial.begin(9600);

  // Setup pins
  pinMode(distanceEchoPin, INPUT);
  pinMode(distanceTriggerPin, OUTPUT);

  // Setup state of light
  light.on = false;
  light.color = noColor;
}

void loop() {
  int carCurrentDistance = 0;

  // Read distance
  carCurrentDistance = readDistance();
  Serial.print("Car distance");
  Serial.println(carCurrentDistance);

  // Change light based on distance
  if (carCurrentDistance < distanceLimit) {
    makeLight(redColor);
  } else {
    makeLight(greenColor);
  }

  delay(200);
}

int readDistance() {
  int duration = 0; // in microseconds
  int distance = 0;
  
  // Clear trigger pin
  digitalWrite(distanceTriggerPin, LOW);
  delayMicroseconds(2);
  
  // Hold high for 10 microseconds
  digitalWrite(distanceTriggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(distanceTriggerPin, LOW);

  // Read
  duration = pulseIn(distanceEchoPin, HIGH);
  Serial.print("Duration");
  Serial.println(duration);

  Serial.println();
  distance = (duration * 0.034) / 2;

  Serial.print("Distance ");
  Serial.println(distance);
  
  return distance;
}

void makeLight(Color color) {
  // Turn on light if needed
  if (light.on == false) {
    turnOnLight();
    light.on == true;
  }

  // Delay between sending of codes; won't work without this delay
  delay(100);

  // Change light if the color isn't current
  if (color != light.color) {
    if (color == redColor) {
       irsend.sendNEC(red, dataLength);
    } else if (color == greenColor) {
      irsend.sendNEC(green, dataLength);
    }
  }

  light.color = color;
}

void turnOnLight() {
  irsend.sendNEC(turnOn, dataLength);
}

