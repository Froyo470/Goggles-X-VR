#include "FastIMU.h"
#include "Madgwick.h"
#include <Wire.h>

#define IMU_ADDRESS 0x68    //Change to the address of the IMU
#define PERFORM_CALIBRATION //Comment to disable startup calibration
MPU6500 IMU;               //Change to the name of any supported IMU!

// Currently supported IMUS: MPU9255 MPU9250 MPU6886 MPU6500 MPU6050 ICM20689 ICM20690 BMI055 BMX055 BMI160 LSM6DS3 LSM6DSL QMI8658

calData calib = { 0 };  //Calibration data
AccelData IMUAccel;    //Sensor data
GyroData IMUGyro;
MagData IMUMag;
Madgwick filter;

void setup() {
  Wire.begin();
  Wire.setClock(400000); //400khz clock

  Serial.begin(115200);
  while (!Serial) {
    ;
  }

  int err = IMU.init(calib, IMU_ADDRESS);
  if (err != 0) {
    Serial.print("Error initializing IMU: ");
    Serial.println(err);
    while (true) {
      ;
    }
  }

  if (err != 0) {
    Serial.print("Error Setting range: ");
    Serial.println(err);
    while (true) {
      ;
    }
  }

#ifdef PERFORM_CALIBRATION
  Serial.println("FastIMU Calibrated Quaternion example");
  if (IMU.hasMagnetometer()) {
    delay(1000);
    Serial.println("Move IMU in figure 8 pattern until done.");
    delay(3000);
    IMU.calibrateMag(&calib);
    Serial.println("Magnetic calibration done!");
  }
  else {
    delay(1000);
  }
  Serial.println("Keep IMU level.");
  delay(5000);
  IMU.calibrateAccelGyro(&calib);
  Serial.println("Calibration done!");
  Serial.println("Accel biases X/Y/Z: ");
  Serial.print(calib.accelBias[0]);
  Serial.print(", ");
  Serial.print(calib.accelBias[1]);
  Serial.print(", ");
  Serial.println(calib.accelBias[2]);
  Serial.println("Gyro biases X/Y/Z: ");
  Serial.print(calib.gyroBias[0]);
  Serial.print(", ");
  Serial.print(calib.gyroBias[1]);
  Serial.print(", ");
  Serial.println(calib.gyroBias[2]);
  if (IMU.hasMagnetometer()) {
    Serial.println("Mag biases X/Y/Z: ");
    Serial.print(calib.magBias[0]);
    Serial.print(", ");
    Serial.print(calib.magBias[1]);
    Serial.print(", ");
    Serial.println(calib.magBias[2]);
    Serial.println("Mag Scale X/Y/Z: ");
    Serial.print(calib.magScale[0]);
    Serial.print(", ");
    Serial.print(calib.magScale[1]);
    Serial.print(", ");
    Serial.println(calib.magScale[2]);
  }
  delay(5000);
  IMU.init(calib, IMU_ADDRESS);

  filter.begin(0.2f);
#endif
}

void loop() {
  IMU.update();
  IMU.getAccel(&IMUAccel);
  IMU.getGyro(&IMUGyro);

  if (IMU.hasMagnetometer()) {
    IMU.getMag(&IMUMag);
    filter.update(IMUGyro.gyroX, IMUGyro.gyroY, IMUGyro.gyroZ,
                  IMUAccel.accelX, IMUAccel.accelY, IMUAccel.accelZ,
                  IMUMag.magX, IMUMag.magY, IMUMag.magZ);
  } else {
    filter.updateIMU(IMUGyro.gyroX, IMUGyro.gyroY, IMUGyro.gyroZ,
                     IMUAccel.accelX, IMUAccel.accelY, IMUAccel.accelZ);
  }

  // Convert quaternion to Euler angles (yaw, pitch, roll)
  float q0 = filter.getQuatW();
  float q1 = filter.getQuatX();
  float q2 = filter.getQuatY();
  float q3 = filter.getQuatZ();

  float yaw = atan2(2.0f * (q1 * q2 + q0 * q3),
                    q0 * q0 + q1 * q1 - q2 * q2 - q3 * q3) * 180.0f / PI;
  float pitch = -asin(2.0f * (q1 * q3 - q0 * q2)) * 180.0f / PI;
  float roll = atan2(2.0f * (q0 * q1 + q2 * q3),
                     q0 * q0 - q1 * q1 - q2 * q2 + q3 * q3) * 180.0f / PI;

/*  float qw = filter.getQuatW();
  float qx = filter.getQuatX();
  float qy = filter.getQuatY();
  float qz = filter.getQuatZ();

  // Yaw (Z), Pitch (Y), Roll (X) in radians
  float ysqr = qy * qy;

  float t0 = +2.0f * (qw * qx + qy * qz);
  float t1 = +1.0f - 2.0f * (qx * qx + ysqr);
  float roll = atan2(t0, t1);

  float t2 = +2.0f * (qw * qy - qz * qx);
  t2 = t2 > 1.0f ? 1.0f : t2;
  t2 = t2 < -1.0f ? -1.0f : t2;
  float pitch = asin(t2);

  float t3 = +2.0f * (qw * qz + qx * qy);
  float t4 = +1.0f - 2.0f * (ysqr + qz * qz);
  float yaw = atan2(t3, t4);

  // Convert to degrees
  yaw *= 180.0f / PI;
  pitch *= 180.0f / PI;
  roll *= 180.0f / PI; */


  // Output format expected by OpenTrack: "yaw,pitch,roll\n"
  Serial.print(yaw); Serial.print(",");
  Serial.print(pitch); Serial.print(",");
  Serial.println(roll);


  delay(10); // ~100 Hz output
}
