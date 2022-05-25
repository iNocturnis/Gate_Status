#include <Arduino.h>
#include <TFT_eSPI.h>
#include <SparkFunLSM6DSO.h>
#include <Wire.h>
#include <string>
#include <BluetoothSerial.h>

BluetoothSerial SerialBT;
LSM6DSO accel;

float begin_values [3]= {0.0,0.0,0.0};
float prev_values [3] = {0.0,0.0,0.0};
float min_values [3] = {0.0,0.0,0.0};
float max_values [3] = {0.0,0.0,0.0};
float delta_values [3] = {0.0,0.0,0.0};
float threshold_accel = 0.200;

float begin_values_gyro [3]= {0.0,0.0,0.0};
float prev_values_gyro [3] = {0.0,0.0,0.0};
float min_values_gyro [3] = {0.0,0.0,0.0};
float max_values_gyro [3] = {0.0,0.0,0.0};
float delta_values_gyro [3] = {0.0,0.0,0.0};
float threshold_gyro = 5;

bool was_moving = false;

int status = 0; //-1 moving close, 0 stopped, 1 moving open

void print(auto input){
  Serial.print(input);
  SerialBT.print(input);
}

void println(auto input){
  Serial.println(input);
  SerialBT.println(input);
}

void setup()
{
  Serial.begin(115200);
  SerialBT.begin("ESP32_TEST");
  Wire.begin();

  if (accel.begin())
  {
    println("Accelo online");
  }
  // Initilize settings
  if(accel.initialize(BASIC_SETTINGS)){

  }

  //ACCEL
  begin_values[0] = accel.readFloatAccelX();
  begin_values[1] = accel.readFloatAccelY();
  begin_values[2] = accel.readFloatAccelZ();

  prev_values[0] = accel.readFloatAccelX();
  prev_values[1] = accel.readFloatAccelY();
  prev_values[2] = accel.readFloatAccelZ();

  min_values[0] = accel.readFloatAccelX();
  min_values[1] = accel.readFloatAccelY();
  min_values[2] = accel.readFloatAccelZ();

  max_values[0] = accel.readFloatAccelX();
  max_values[1] = accel.readFloatAccelY();
  max_values[2] = accel.readFloatAccelZ();

  //GYRO
  begin_values_gyro[0] = accel.readFloatGyroX();
  begin_values_gyro[1] = accel.readFloatGyroY();
  begin_values_gyro[2] = accel.readFloatGyroZ();

  prev_values_gyro[0] = accel.readFloatGyroX();
  prev_values_gyro[1] = accel.readFloatGyroY();
  prev_values_gyro[2] = accel.readFloatGyroZ();

  min_values_gyro[0] = accel.readFloatGyroX();
  min_values_gyro[1] = accel.readFloatGyroY();
  min_values_gyro[2] = accel.readFloatGyroZ();

  max_values_gyro[0] = accel.readFloatGyroX();
  max_values_gyro[1] = accel.readFloatGyroY();
  max_values_gyro[2] = accel.readFloatGyroZ();

}

void loop()
{
  
  float current_values [3]= {accel.readFloatAccelX(),accel.readFloatAccelY(),accel.readFloatAccelZ()};
  float current_values_gyro [3]= {accel.readFloatGyroX(),accel.readFloatGyroY(),accel.readFloatGyroZ()};

  /*
  std::string X_string = std::to_string(X);
  X_string = std::string(("X: ")).append(X_string);

  std::string Y_string = std::to_string(Y);
  Y_string = std::string(("Y: ")).append(Y_string);

  std::string Z_string = std::to_string(Z);
  Z_string = std::string(("Z: ")).append(Z_string);

  tft.drawString(X_string.c_str(),10,10);
  tft.drawString(Y_string.c_str(),10,20);
  tft.drawString(Z_string.c_str(),10,30);

  times_reported++;
  avg[0] = (X + avg[0])/times_reported;
  avg[1] = (Y + avg[1])/times_reported;
  avg[2] = (Z + avg[2])/times_reported;

  peak[0] = std::max(peak[0],X);
  peak[1] = std::max(peak[1],Y);
  peak[2] = std::max(peak[2],Z);

  //Drawing avg
  std::string X_avg = std::to_string(avg[0]);
  X_avg = std::string(("Average X: ")).append(X_avg);

  std::string Y_avg = std::to_string(avg[1]);
  Y_avg = std::string(("Average Y: ")).append(Y_avg);

  std::string Z_avg = std::to_string(avg[2]);
  Z_avg = std::string(("Average Z: ")).append(Z_avg);

  tft.drawString(X_avg.c_str(),10,40);
  tft.drawString(Y_avg.c_str(),10,50);
  tft.drawString(Z_avg.c_str(),10,60);

  //Drawing peaks
  std::string X_peak = std::to_string(peak[0]);
  X_peak = std::string(("Peak X: ")).append(X_peak);

  std::string Y_peak = std::to_string(peak[1]);
  Y_peak = std::string(("Peak Y: ")).append(Y_peak);

  std::string Z_peak = std::to_string(peak[2]);
  Z_peak = std::string(("Peak Z: ")).append(Z_peak);

  tft.drawString(X_peak.c_str(),10,70);
  tft.drawString(Y_peak.c_str(),10,80);
  tft.drawString(Z_peak.c_str(),10,90);
  */

  if(std::abs(prev_values[0] - current_values[0]) > threshold_accel || 
  std::abs(prev_values[1] - current_values[1]) > threshold_accel || 
  std::abs(prev_values[2] - current_values[2]) > threshold_accel){
    println("-------------WE HAVE MOTION --------------");
    for(int i = 0; i < 3; i ++){
      print("Initial X,Y,Z :" );
      print(begin_values[0]);
      print(",");
      print(begin_values[1]);
      print(",");
      println(begin_values[2]);
      
      print("Max X,Y,Z :" );
      print(max_values[0]);
      print(",");
      print(max_values[1]);
      print(",");
      println(max_values[2]);
      
      print("Min X,Y,Z :" );
      print(min_values[0]);
      print(",");
      print(min_values[1]);
      print(",");
      println(min_values[2]);

      if(std::abs(prev_values[0] - current_values[0]) > threshold_accel){
        print("DELTA X: ");
        println(prev_values[0]-current_values[0]);
        delta_values[0]+= prev_values[0]-current_values[0];
      }
      if(std::abs(prev_values[1] - current_values[1]) > threshold_accel){
        print("DELTA Y: ");
        println(prev_values[1]-current_values[1]);
        delta_values[1]+= prev_values[1]-current_values[1];
      }
      if(std::abs(prev_values[2] - current_values[2]) > threshold_accel){
        print("DELTA Z: ");
        println(prev_values[2]-current_values[2]);
        delta_values[2]+= prev_values[2]-current_values[2];
      }
    }
  }
  prev_values[0] = current_values[0];
  prev_values[1] = current_values[1];
  prev_values[2] = current_values[2];

  max_values[0] = std::max(max_values[0],current_values[0]);
  max_values[1] = std::max(max_values[1],current_values[1]);
  max_values[2] = std::max(max_values[2],current_values[2]);

  min_values[0] = std::min(min_values[0],current_values[0]);
  min_values[1] = std::min(min_values[1],current_values[1]);
  min_values[2] = std::min(min_values[2],current_values[2]);

  bool moving = false;
  for(int i = 0 ;i  < 3; i ++){
    if(std::abs(prev_values_gyro[i] - current_values_gyro[i]) > threshold_gyro){
      moving = true;
    }
  }

  prev_values_gyro[0] = current_values_gyro[0];
  prev_values_gyro[1] = current_values_gyro[1];
  prev_values_gyro[2] = current_values_gyro[2];

  if(moving){
    println("WE ARE MOVING");
    for(int i = 0 ;i < 3; i ++){
      print(delta_values[i]);
      print(",");
    }
    println("");
    //Check if closing or opening
    if(delta_values[0] > 0){
      status = 1;
    }
    if(delta_values[0] < 0){
      status = -1;
    }
    if(delta_values[0] == 0){
      status = 0;
    }
    was_moving = true;
  }
  if(was_moving && !moving){
    println("We stopped moving");
    was_moving = false;
    delta_values[0] = 0;
    delta_values[1] = 0;
    delta_values[2] = 0;
  }
  if(was_moving){
    println(status);
  }
  delay(100);
}

