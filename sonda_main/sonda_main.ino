/**
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
unsigned long T_0 = 0, T_sensors = 0, T_last_loop = 0, T = 1000;
int x = 0;

//0 FOR IGNORE, 1 TO PRINT ON SERIAL PORT
bool SERIAL_NAMES = 0;
bool CHECK_PRINT_ON_SERIAL = 0;
bool SI7021_HR_ACTIVE = 1;
bool SI7021_TEMP_ACTIVE = 1;
bool BMP180_TEMP_ACTIVE = 1;
bool BMP180_PRESSBASE_ACTIVE = 0;
bool BMP180_PRESSCURR_ACTIVE = 0;
bool BMP180_ALT_ACTIVE = 1;
bool TIMES_ACTIVE = 0;
bool FLAG_ACTIVE = 0;


//SI7021 RELATED
#include <SI7021.h>
SI7021 SI7021_sensor;
float Temp_SI7021, HR_SI7021;

//BMP180 RELATED
#include <SFE_BMP180.h>
SFE_BMP180 BMP180_sensor;
double Temp_BMP180, PressBase_BMP180, PressCurr_BMP180, Alt_BMP180;

void setup()
{
  //SERIAL RELATED
  Serial.begin(9600);

  //SENSORS CHECK
    if (CHECK_PRINT_ON_SERIAL)
  {
    Serial.print("\n\n");
  }
  SI7021_CHECK();
  BMP180_CHECK();
    if (CHECK_PRINT_ON_SERIAL)
  {
    Serial.print("\n\n");
  }

  //MISCELLANEOUS RELATED
  pinMode(13, OUTPUT);
  //BMP180RELATED
  PressBase_BMP180 = BMP180_Pressure_Temp();
}

void loop()
{
  //MISCELLANEOUS RELATED
  T_0 = millis();
  x = !x;
  digitalWrite(13, LOW);
  T_sensors = millis();

  //SENSORS MEASURE
  SI7021_MEASURE();
  BMP180_MEASURE();

  //MISCELLANEA RELATED
  T_sensors = millis() - T_sensors;

  //SERIAL RELATED
  SERIAL_PRINT();

  //MISCELLANEOUS RELATED
  T_last_loop = millis() - T_0;
  do {
    digitalWrite(13, HIGH);
  } while  ((millis() - T_0) < T); //COMPARE CURRENT TIME WITH LOOP INITIAL TIME
}

void SERIAL_PRINT()
{
  if (SERIAL_NAMES)
  {
    if (SI7021_HR_ACTIVE)
    {
      Serial.print("SI7021 HR [%]\t");
    }
    if (SI7021_TEMP_ACTIVE)
    {
      Serial.print("SI7021 TEMP [ºC]\t");
    }
    if (BMP180_TEMP_ACTIVE)
    {
      Serial.print("BMP180 TEMP [ºC]\t");
    }
    if (BMP180_PRESSBASE_ACTIVE)
    {
      Serial.print("BMP PRESSBASE [Pa]\t");
    }
    if (BMP180_PRESSCURR_ACTIVE)
    {
      Serial.print("BMP180 PRESSCURR [Pa]\t");
    }
    if (BMP180_ALT_ACTIVE)
    {
      Serial.print("BMP180 ALT [m]\t");
    }
    if (TIMES_ACTIVE)
    {
      Serial.print("Time Sensors [ms]\tTime Last Loop [ms]\t");
    }
    if (FLAG_ACTIVE)
    {
      Serial.print("FLAG [0/1]\t");
    }
    Serial.print("\n");
  }
  //VARIABLES-------------------------------------------------------
  if (SI7021_HR_ACTIVE)
  {
    Serial.print(HR_SI7021);
    Serial.print("\t\t");
  }
  if (SI7021_TEMP_ACTIVE)
  {
    Serial.print(Temp_SI7021);
    Serial.print("\t\t\t");
  }
  if (BMP180_TEMP_ACTIVE)
  {
    Serial.print(Temp_BMP180);
    Serial.print("\t\t\t");
  }
  if (BMP180_PRESSBASE_ACTIVE)
  {
    Serial.print(PressBase_BMP180);
    Serial.print("\t\t\t");
  }
  if (BMP180_PRESSCURR_ACTIVE)
  {
    Serial.print(PressCurr_BMP180);
    Serial.print("\t\t\t");
  }
  if (BMP180_ALT_ACTIVE)
  {
    Serial.print(Alt_BMP180);
    Serial.print("\t\t");
  }
  if (TIMES_ACTIVE)
  {
    Serial.print(T_sensors);
    Serial.print("\t\t\t");
    Serial.print(T_last_loop);
    Serial.print("\t\t\t");
  }
  if (FLAG_ACTIVE)
  {
    Serial.print(x);
  }
  Serial.print("\n");
}

void SI7021_CHECK()
{
  if (SI7021_sensor.begin())
  {
    if (CHECK_PRINT_ON_SERIAL)
    {
      Serial.write("SI7021 OK\t");
    }
  }
  else
  {
    if (CHECK_PRINT_ON_SERIAL)
    {
      Serial.write("SI7021 ERROR\t");
    }
  }
}

void BMP180_CHECK()
{
  if (BMP180_sensor.begin())
  {
    if (CHECK_PRINT_ON_SERIAL)
    {
      Serial.write("BMP180 OK\t");
    }
  }
  else
  {
    if (CHECK_PRINT_ON_SERIAL)
    {
      Serial.write("BMP180 ERROR\t");
    }
  }
}

void SI7021_MEASURE()
{
  si7021_env data = SI7021_sensor.getHumidityAndTemperature();
  Temp_SI7021 = data.celsiusHundredths / 100.00;
  HR_SI7021 = data.humidityBasisPoints / 100.00;
}

void BMP180_MEASURE()
{
  PressCurr_BMP180 = BMP180_Pressure_Temp();
  Alt_BMP180 = BMP180_sensor.altitude(PressCurr_BMP180, PressBase_BMP180);
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
