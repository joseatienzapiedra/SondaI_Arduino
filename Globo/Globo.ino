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

#include <LiquidCrystal_I2C.h>
#include <Wire.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
uint8_t degree[8]  = {0x8,0xf4,0x8,0x43,0x4,0x4,0x43,0x0}; // make a degree symbol for I2C LCD1602 Screen

void setup() {
  lcd.begin();
  lcd.createChar(0, degree);
  lcd.backlight();
}

void loop() {
  lcd.clear();
  lcd.setCursor(0, 0);   // go to line #0
  lcd.print("Text0");
  lcd.setCursor(0,1);    // go to line #1
  lcd.print("Text1");
}
