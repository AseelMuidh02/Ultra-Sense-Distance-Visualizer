#include <LedControl.h>

// Define ultrasonic sensor pins
#define TRIG_PIN A5         // TRIG is connected to analog pin A5 (used as digital output)
#define ECHO_PIN A6         // ECHO is connected to analog pin A6 (used as digital input)

// Initialize the MAX7219 LED Dot Matrix module
// DIN is on pin 51 (MOSI), CLK on pin 52 (SCK), CS on pin 53 (SS)
LedControl lc = LedControl(51, 52, 53, 1);

void setup() {
  // Set up ultrasonic sensor pins
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  
  // Initialize the LED matrix
  lc.shutdown(0, false);  // Wake up the display (device 0)
  lc.setIntensity(0, 8);  // Set brightness level (0-15)
  lc.clearDisplay(0);     // Clear the display
  
  // Start serial communication for debugging
  Serial.begin(9600);
  Serial.println("Ultrasonic Sensor and LED Matrix Display Starting...");
}

void loop() {
  long duration;
  float distance;
  
  // Ensure TRIG is LOW to start clean
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  
  // Send a 10-microsecond pulse to TRIG
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  
  // Read the duration of the echo pulse (with a timeout of 30000 microseconds)
  duration = pulseIn(ECHO_PIN, HIGH, 30000);
  
  // If no echo is received, set a high distance value
  if (duration == 0) {
    distance = 999;  // No object detected within range
  } else {
    // Calculate distance in centimeters:
    // distance = (duration * 0.0343) / 2
    distance = (duration * 0.0343) / 2;
  }
  
  // Print the measured distance to the Serial Monitor
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
  
  // Determine the bar graph value:
  // If distance is 100 cm or more (or no echo), turn off LEDs.
  int bar;
  if (distance >= 100 || duration == 0) {
    bar = 0;
  } else {
    // Map distance (0 - 100 cm) to a bar value (8 - 0 columns)
    bar = map((int)distance, 0, 100, 8, 0);
    if (bar > 8) bar = 8;
    if (bar < 0) bar = 0;
  }
  
  // Update the LED matrix display: clear and then light columns based on 'bar'
  lc.clearDisplay(0);
  for (int col = 0; col < 8; col++) {
    if (col < bar) {
      lc.setColumn(0, col, 0xFF);  // Light all 8 LEDs in this column
    } else {
      lc.setColumn(0, col, 0x00);  // Turn off this column
    }
  }
  
  // Delay before taking the next measurement
  delay(500);
}
