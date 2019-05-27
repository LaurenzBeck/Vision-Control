// I2Cdev and MPU6050 must be installed as libraries, or else the .cpp/.h files
// for both classes must be in the include path of your project
#include "I2Cdev.h"
#include "MPU6050.h"

// Arduino Wire library is required if I2Cdev I2CDEV_ARDUINO_WIRE implementation
// is used in I2Cdev.h
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif


MPU6050 accelgyro;

volatile int16_t ax, ay, az = 0;
volatile int16_t rawx, rawy, rawz = 0;
volatile double  phi_hat, theta_hat;

volatile unsigned long dTu;
volatile double dT;
volatile unsigned long exec;

const float Pi = 3.14159;

void setup() {
    // join I2C bus (I2Cdev library doesn't do this automatically)
    #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
        Wire.begin();
    #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
        Fastwire::setup(400, true);
    #endif

    // initialize serial communication
    Serial.begin(115200);

    // initialize device
    Serial.println("Initializing I2C devices...");
    accelgyro.initialize();

    // verify connection
    Serial.println("Testing device connections...");
    Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");

    // Setup MPU6050 to 200 Hz and activate Digital Low Pass Filter in Mode 4 (DLPF)
    accelgyro.setRate(4);
    accelgyro.setDLPFMode(4);

    // Initialize phi_hat and theta_hat
    phi_hat = 0.0;
    theta_hat = 0.0;
}

void loop() {
  dTu = micros() - exec;
  if (dTu >= 5000) // Reading IMU at 200 Hz
  {
    dT = (double)dTu / 1000000;
    exec = micros();
    // read raw accel/gyro measurements from device
    accelgyro.getMotion6(&ax, &ay, &az, &rawx, &rawy, &rawz);

    // call filter
    filter();
    
    // display tab-separated accel/gyro x/y/z values
    Serial.print((int)(dT*1000)); Serial.print(",");
    Serial.print(ax); Serial.print(",");
    Serial.print(ay); Serial.print(",");
    Serial.print(az); Serial.print(",");
    Serial.print(rawx); Serial.print(",");
    Serial.print(rawy); Serial.print(",");
    Serial.print(rawz); Serial.print(",");
    Serial.print((int)(phi_hat*1000)); Serial.print(",");
    Serial.println((int)(theta_hat*1000));
  }
}

void filter()
{
  // Use the predefined global variables ax, ay, az and gx, gy, gz, as well as dT which is also already calculated as a global variable
  // Store the results of your algorithm into the following variables: phi_hat, theta_hat 
}
