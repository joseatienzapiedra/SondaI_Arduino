/*
   Copyright (c) 2016 Jose Atienza  <joseatienzapiedra@gmail.com> http://www.joseatienza.com/
                      Juan J. Pardo <dejpardo@gmail.com>
   All rights reserved.

   This file is part of SondaI_Arduino.
   See:  https://github.com/joseatienzapiedra/SondaI_Arduino

   SondaI_Arduino is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   SondaI_Arduino is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with PicaMesh-server.  If not, see <http://www.gnu.org/licenses/>.
*/

//MISCELLANEOUS RELATED
#include <Wire.h>
unsigned long T_0 = 0;

//SD RELATED
#include <SD.h>
File myfile;
String FILE_NAME;

//SI7021 RELATED
#include <SI7021.h>
SI7021 SI7021_sensor;
float Temp_SI7021, HR_SI7021;

//BMP180 RELATED
#include <SFE_BMP180.h>
SFE_BMP180 BMP180_sensor;
double Temp_BMP180, PressBase_BMP180, PressCurr_BMP180, Alt_BMP180;

//DS3231 RELATED
#include <DS3231.h>
DS3231  rtc(SDA, SCL);
String Date_DS3231, Time_DS3231;
bool RTC_STATUS_FLAG = 0;
int Hour, Minute, Second;

//MPU6050 RELATED
#include <MPU6050.h>
MPU6050 MPU6050_sensor;
Vector Acel_MPU6050;
Vector Giro_MPU6050;

void setup()
{
  //SERIAL RELATED
  Serial.begin(9600);

  //SENSORS CHECK
  Serial.print("\n\n");
  SD_CHECK();
  SI7021_CHECK();
  BMP180_CHECK();
  DS3231_CHECK();
  MPU6050_CHECK();
  Serial.print("\n\n");

  //MISCELLANEOUS RELATED
  pinMode(13, OUTPUT);
}

void loop()
{
  //MISCELLANEOUS RELATED
  T_0 = millis();

  //MISCELLANEOUS RELATED
  digitalWrite(13, HIGH);
  delay(100);
  digitalWrite(13, LOW);

  //SENSORS MEASURE
  SI7021_MEASURE();
  BMP180_MEASURE();
  MPU6050_MEASURE();
  DS3231_MEASURE();

  //SD RELATED
  SD_PRINT_DATA();

  do {} while  ((millis() - T_0) < 1000); //COMPARE CURRENT TIME WITH LOOP INITIAL TIME
}
//_______________________________________________________________________________________________________________________________________________________________________________

void SD_PRINT_DATA()
{
  myfile = SD.open(FILE_NAME, FILE_WRITE);
  if (myfile)
  {
    myfile.print(Time_DS3231);
    myfile.print("\t");
    myfile.print(HR_SI7021);
    myfile.print("\t");
    myfile.print(Temp_SI7021);
    myfile.print("\t");
    myfile.print(Temp_BMP180);
    myfile.print("\t");
    myfile.print(PressBase_BMP180 * 100.000);
    myfile.print("\t");
    myfile.print(PressCurr_BMP180 * 100.000);
    myfile.print("\t");
    myfile.print(Alt_BMP180);
    myfile.print("\t");
    myfile.print(Acel_MPU6050.XAxis);
    myfile.print("\t");
    myfile.print(Acel_MPU6050.YAxis);
    myfile.print("\t");
    myfile.print(Acel_MPU6050.ZAxis);
    myfile.print("\t");
    myfile.print(Giro_MPU6050.XAxis);
    myfile.print("\t");
    myfile.print(Giro_MPU6050.YAxis);
    myfile.print("\t");
    myfile.print(Giro_MPU6050.ZAxis);
    myfile.println();
    myfile.close();

    //PRINT SERIAL

    Serial.print(Time_DS3231);
    Serial.print("\t");
    Serial.print(HR_SI7021);
    Serial.print("\t");
    Serial.print(Temp_SI7021);
    Serial.print("\t");
    Serial.print(Temp_BMP180);
    Serial.print("\t");
    Serial.print(PressBase_BMP180 * 100.000);
    Serial.print("\t");
    Serial.print(PressCurr_BMP180 * 100.000);
    Serial.print("\t");
    Serial.print(Alt_BMP180);
    Serial.print("\t");
    Serial.print(Acel_MPU6050.XAxis);
    Serial.print("\t");
    Serial.print(Acel_MPU6050.YAxis);
    Serial.print("\t");
    Serial.print(Acel_MPU6050.ZAxis);
    Serial.print("\t");
    Serial.print(Giro_MPU6050.XAxis);
    Serial.print("\t");
    Serial.print(Giro_MPU6050.YAxis);
    Serial.print("\t");
    Serial.print(Giro_MPU6050.ZAxis);
    Serial.println();

  }
  else
  {
    Serial.println("\n*****FILE ERROR*****\n");
  }
}

void SD_CHECK()
{
  if (!SD.begin(4))
  {
    Serial.write("SD ERROR\t");
  }
  else
  {
    Serial.write("SD OK\t");
  }
}

void SI7021_CHECK()
{
  if (SI7021_sensor.begin())
  {
    Serial.write("SI7021 OK\t");
  }
  else
  {
    Serial.write("SI7021 ERROR\t");
  }
}

void BMP180_CHECK()
{
  if (BMP180_sensor.begin())
  {
    Serial.write("BMP180 OK\t");
    PressBase_BMP180 = BMP180_Pressure_Temp();
  }
  else
  {
    Serial.write("BMP180 ERROR\t");
  }
}

void DS3231_CHECK()
{
  rtc.begin();
  Wire.beginTransmission(87);
  if (0 == Wire.endTransmission())
  {
    Wire.beginTransmission(104);
    if (0 == Wire.endTransmission())
    {
      Serial.write("DS3231 OK\t");
    }
    else
    {
      Serial.write("DS3231 ERROR\t");
    }
  }
  else
  {
    Serial.write("DS3231 ERROR\t");
  }
}

void MPU6050_CHECK()
{
  if (MPU6050_sensor.begin(MPU6050_SCALE_250DPS, MPU6050_RANGE_2G, 105)) //IMPORTANT 105 REFERS TO CUSTOM ADDRESS
  {
    Serial.write("MPU6050 OK\t");
    MPU6050_sensor.calibrateGyro();
    MPU6050_sensor.setThreshold(3);
  }
  else
  {
    Serial.write("MPU6050 ERROR\t");
  }
}

void SI7021_MEASURE()
{
  Wire.beginTransmission(64);
  if (0 == Wire.endTransmission())
  {
    si7021_env data = SI7021_sensor.getHumidityAndTemperature();
    Temp_SI7021 = data.celsiusHundredths / 100.00;
    HR_SI7021 = data.humidityBasisPoints / 100.00;
  }
}

void BMP180_MEASURE()
{
  Wire.beginTransmission(119);
  if (0 == Wire.endTransmission())
  {
    PressCurr_BMP180 = BMP180_Pressure_Temp();
    Alt_BMP180 = BMP180_sensor.altitude(PressCurr_BMP180, PressBase_BMP180);
  }
}

double BMP180_Pressure_Temp()
{
  char status;
  status = BMP180_sensor.startTemperature();
  if (status != 0)
  {
    delay(status);
    status = BMP180_sensor.getTemperature(Temp_BMP180);
    if (status != 0)
    {
      status = BMP180_sensor.startPressure(3); // The parameter is the oversampling setting, from 0 to 3 (highest res, longest wait).
      if (status != 0)
      {
        delay(status);
        status = BMP180_sensor.getPressure(PressCurr_BMP180, Temp_BMP180);
        if (status != 0)
        {
          return (PressCurr_BMP180);
        }
      }
    }
  }
}

void MPU6050_MEASURE()
{
  Wire.beginTransmission(105);
  if (0 == Wire.endTransmission())
  {
    Acel_MPU6050 = MPU6050_sensor.readNormalizeAccel();
    Giro_MPU6050 = MPU6050_sensor.readNormalizeGyro();
  }
}

void DS3231_MEASURE()
{
  Wire.beginTransmission(87);
  if (0 == Wire.endTransmission())
  {
    Wire.beginTransmission(104);
    if (0 == Wire.endTransmission())
    {
      RTC_STATUS_FLAG = 0;
      Time_DS3231 = rtc.getTimeStr();
      Date_DS3231 = rtc.getDateStr();
      FILE_NAME = Date_DS3231.substring(6, 10) + Date_DS3231.substring(3, 5) + Date_DS3231.substring(0, 2) + ".TXT";
    }
    else
    {
      RTC_FAIL_INFER_TIME();
    }
  }
  else
  {
    RTC_FAIL_INFER_TIME();
  }
}

void RTC_FAIL_INFER_TIME()
{
  if (RTC_STATUS_FLAG == 0)
  {
    Hour = Time_DS3231.substring(0, 2).toInt();
    Minute = Time_DS3231.substring(3, 5).toInt();
    Second = Time_DS3231.substring(6, 8).toInt();
    RTC_STATUS_FLAG = 1;
  }

  if (Second < 59)
  {
    Second = Second + 1;
  }
  else
  {
    Second = 0;
    if (Minute < 59)
    {
      Minute = Minute + 1;
    }
    else
    {
      Minute = 0;

      if (Hour < 23)
      {
        Hour = Hour + 1;
      }
      else
      {
        Hour = 0;
        myfile.print("\n\nNEW DAY!\n\n");
      }
    }
  }
  if (Hour < 10)
  {
    Time_DS3231 = "0" + String(Hour);
  }
  else
  {
    Time_DS3231 = String(Hour);
  }
  if (Minute < 10)
  {
    Time_DS3231 = Time_DS3231 + ":" + "0" + String(Minute);
  }
  else
  {
    Time_DS3231 = Time_DS3231 + ":" + String(Minute);
  }
  if (Second < 10)
  {
    Time_DS3231 = Time_DS3231 + ":" + "0" + String(Second) + "*";
  }
  else
  {
    Time_DS3231 = Time_DS3231 + ":" + String(Second) + "*";
  }
}



