#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Set the LCD address to 0x3F for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup()
{
lcd.begin();
lcd.backlight();
lcd.setCursor(0,0);
lcd.print("I2C LCD with ");
lcd.setCursor(0,1);
lcd.print(" STM32F103C8T6 ");
}

void loop()
{
lcd.setCursor(0,0);
lcd.print("I2C LCD with ");
lcd.setCursor(0,1);
lcd.print(" STM32F103C8T6 ");
}
