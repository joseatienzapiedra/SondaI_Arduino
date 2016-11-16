/**
 * Copyright (c) 2016 Jose Atienza  <joseatienzapiedra@gmail.com> http://www.joseatienza.com/
 *                    Juan J. Pardo <dejpardo@gmail.com>
 * All rights reserved.
 *
 * This file is part of SondaI_Arduino.
 * See:  https://github.com/joseatienzapiedra/SondaI_Arduino
 *
 * SondaI_Arduino is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * SondaI_Arduino is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with PicaMesh-server.  If not, see <http://www.gnu.org/licenses/>.
 */


#include <max6675.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

int thermoDO = 4;
int thermoCS = 5;
int thermoCLK = 6;
MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);
int vccPin = 3;
int gndPin = 2;

// make a cute degree symbol
uint8_t degree[8]  = {0x8,0xf4,0x8,0x43,0x4,0x4,0x43,0x0};

void setup() {
    pinMode(vccPin, OUTPUT); digitalWrite(vccPin, HIGH);
  pinMode(gndPin, OUTPUT); digitalWrite(gndPin, LOW);
  lcd.begin();
  lcd.createChar(0, degree);
  lcd.backlight();
  // wait for MAX chip to stabilize
  delay(500);
}

void loop() {
  // basic readout test, just print the current temp
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("MAX6675 test");
  
  // go to line #1
  lcd.setCursor(0,1);
  lcd.print(thermocouple.readCelsius());
#if ARDUINO >= 100
  lcd.write((byte)0);
#else
  lcd.print(0, BYTE);
#endif
  lcd.print("C ");
  lcd.print(thermocouple.readFahrenheit());
#if ARDUINO >= 100
  lcd.write((byte)0);
#else
  lcd.print(0, BYTE);
#endif
  lcd.print('F');
  delay(1000);
}
