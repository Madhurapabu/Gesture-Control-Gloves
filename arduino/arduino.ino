#include <Wire.h>
#include <MPU6050.h>

#define TOUCH_SENS 2
#define TOUCH_LED 12
#define LED_PIN 13

// class default I2C address is 0x68
const MPU6050 accelgyro;

int16_t ax, ay, az;
int16_t gx, gy, gz;
int8_t threshold, count;

float temp;
bool zero_detect;
bool TurnOnZI = false;

bool XnegMD, XposMD, YnegMD, YposMD, ZnegMD, ZposMD;

// to hold data in app()
struct ProcessedData
{
  byte x;
  byte y;
};
ProcessedData data;

bool blinkState = false;
bool gyroState = false;
const bool isPlotting = false;

void setup()
{
  // configure touch sensor
  pinMode(TOUCH_SENS, INPUT);

  // configure LEDs
  pinMode(TOUCH_LED, OUTPUT);
  pinMode(LED_PIN, OUTPUT);

  // join I2C bus
  Wire.begin();

  // initialize serial communication
  Serial.begin(38400);

  // initialize device
  accelgyro.initialize();
  Serial.println("debug-initialized");

  // verify connection
  gyroState = accelgyro.testConnection();

  if (gyroState)
  {
    // on connection success
    Serial.println("debug-mpu-success");
  }
  else
  {
    // on connection failed
    Serial.println("debug-mpu-failed");
  }

  // some settings, idk what happens
  accelgyro.setAccelerometerPowerOnDelay(3);
  accelgyro.setIntZeroMotionEnabled(TurnOnZI);
  accelgyro.setMotionDetectionDuration(40);
  accelgyro.setZeroMotionDetectionDuration(1);
}

// output data as serial output line to use in arduino plotter
void plotting()
{
  // read raw accel/gyro measurements from device
  accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  XnegMD = accelgyro.getXNegMotionDetected();
  XposMD = accelgyro.getXPosMotionDetected();
  YnegMD = accelgyro.getYNegMotionDetected();
  YposMD = accelgyro.getYPosMotionDetected();
  ZnegMD = accelgyro.getZNegMotionDetected();
  ZposMD = accelgyro.getZPosMotionDetected();

  zero_detect = accelgyro.getIntMotionStatus();
  threshold = accelgyro.getZeroMotionDetectionThreshold();

  // get internal temperature
  // temp = (accelgyro.getTemperature() / 340.) + 36.53;

  // accelerometer reading
  Serial.print(ax / 16384.);
  Serial.print(" ");
  Serial.print(ay / 16384.);
  Serial.print(" ");
  Serial.print(az / 16384.);
  Serial.print(" ");

  // gyroscope reading
  // Serial.print(gx / 131.072);
  // Serial.print(" ");
  // Serial.print(gy / 131.072);
  // Serial.print(" ");
  // Serial.print(gz / 131.072);
  // Serial.print(" ");

  Serial.print(zero_detect);
  Serial.print(" ");

  Serial.print(XnegMD);
  Serial.print(" ");

  Serial.println(XposMD);

  // sleep 100ms
  delay(100);

  // toggle LED
  blinkState = !blinkState;
  digitalWrite(LED_PIN, blinkState);
}

// pass to python
void app()
{
  // read raw accel/gyro measurements from device
  accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  data.x = map(ax, -17000, 17000, 0, 255); // X axis data
  data.y = map(ay, -17000, 17000, 0, 255); // Y axis data

  // delay 250ms
  delay(250);

  if (data.y < 100)
  {
    Serial.println("gesture-right");
  }
  if (data.y > 175)
  {
    Serial.println("gesture-left");
  }
  if (data.x > 175)
  {
    // Serial.println("gesture-skip");
  }
  if (data.x < 70)
  {
    Serial.println("gesture-down");
  }
  if (data.x > 70 && data.x < 170 && data.y > 80 && data.y < 130)
  {
    Serial.println("gesture-up");
  }
}

void loop()
{
  // avoid looping if gyro sensor isn't connected
  if (!gyroState)
  {
    // keep LED on to display something is wrong
    digitalWrite(LED_PIN, true);
  }
  else
  {
    if (digitalRead(TOUCH_SENS) == 1)
    {
      // run application
      digitalWrite(TOUCH_LED, true);
      app();
    }
    else
    {
      digitalWrite(TOUCH_LED, false);

      Serial.println("no-touch");
      delay(250);
    }
  }
}