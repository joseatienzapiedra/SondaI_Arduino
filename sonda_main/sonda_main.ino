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

//SD RELATED
#include <SD.h>
File myfile;
String header="SI7021 HR [%]\tSI7021 TEMP [ºC]\tBMP180 TEMP [ºC]\tBMP PRESSBASE [Pa]\tBMP180 PRESSCURR [Pa]\tBMP180 ALT [m]\tTime Sensors [ms]\tTime Last Loop [ms]\tFLAG [0/1]\t\n";

int x = 0, i = 0;

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
  Serial.print("\n\n");
  SD_CHECK();
  SI7021_CHECK();
  BMP180_CHECK();
  Serial.print("\n\n");

  //PRINT HEADER RELATED
  SERIAL_PRINT_HEADER();
  SD_PRINT_HEADER();

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

  //SENSORS MEASURE
  T_sensors = millis();
  SI7021_MEASURE();
  BMP180_MEASURE();
  T_sensors = millis() - T_sensors;
  
  //SERIAL RELATED
  SERIAL_PRINT_DATA();

  //SD RELATED
  if (i <= 10)
  {
    SD_PRINT_DATA();
  }
  else if (i==10)
  {
    myfile.close();
    Serial.print("\n\nSD ACABADA!!!!!!!!!\n\n");
  }
  i = i + 1;

  //MISCELLANEOUS RELATED
  T_last_loop = millis() - T_0;
  do {digitalWrite(13, HIGH);} while  ((millis() - T_0) < T); //COMPARE CURRENT TIME WITH LOOP INITIAL TIME
}
//_______________________________________________________________________________________________________________________________________________________________________________

void SERIAL_PRINT_HEADER()
{
  Serial.print(header);
}

void SD_PRINT_HEADER()
{
  if (myfile)
  {
    myfile.print(header);
    myfile.println(); //HAY QUE PROBARA HACERLO EN UNA LINEA CON PRINTLN
  }
  else
  {
    Serial.print("\n*****FILE ERROR*****\n");
  }
}

void SERIAL_PRINT_DATA()
{
  Serial.print(HR_SI7021);
  Serial.print("\t\t");
  Serial.print(Temp_SI7021);
  Serial.print("\t\t\t");
  Serial.print(Temp_BMP180);
  Serial.print("\t\t\t");
  Serial.print(PressBase_BMP180 * 100.000);
  Serial.print("\t\t");
  Serial.print(PressCurr_BMP180 * 100.000);
  Serial.print("\t\t");
  Serial.print(Alt_BMP180);
  Serial.print("\t\t");
  Serial.print(T_sensors);
  Serial.print("\t\t\t");
  Serial.print(T_last_loop);
  Serial.print("\t\t\t");
  Serial.print(x);
  Serial.print("\n");
}

void SD_PRINT_DATA()
{
  if (myfile)
  {
    myfile.print(HR_SI7021);
    myfile.print("\t\t");
    myfile.print(Temp_SI7021);
    myfile.print("\t\t\t");
    myfile.print(Temp_BMP180);
    myfile.print("\t\t\t");
    myfile.print(PressBase_BMP180 * 100.000);
    myfile.print("\t\t");
    myfile.print(PressCurr_BMP180 * 100.000);
    myfile.print("\t\t");
    myfile.print(Alt_BMP180);
    myfile.print("\t\t");
    myfile.print(T_sensors);
    myfile.print("\t\t\t");
    myfile.print(T_last_loop);
    myfile.print("\t\t\t");
    myfile.print(x);
    myfile.println();
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
    myfile = SD.open("textFile.txt", FILE_WRITE);
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
  }
  else
  {
    Serial.write("BMP180 ERROR\t");
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
