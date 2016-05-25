#include "monoLCD.h"

graphicsLCD lcd;
void setup()
{

	pinMode(9, OUTPUT);
	analogWrite(9, 128);


	lcd.begin();
	lcd.clearDisplay();
	lcd.drawDisplay();

	lcd.setCurser(1, 1);
	lcd.fontSize(1);
	lcd.print("This is font size 1");
	lcd.drawDisplay();
	delay(3000);
	lcd.clearDisplay();
	
	lcd.setCurser(1, 1);
	lcd.fontSize(2);
	lcd.print("This is font size 2");
	lcd.drawDisplay();
	delay(3000);
	lcd.clearDisplay();


	lcd.setCurser(1, 1);
	lcd.fontSize(3);
	lcd.print("Font");
	lcd.setCurser(1, 25);
	lcd.print("3");
	lcd.drawDisplay();
	delay(3000);
	lcd.clearDisplay();


	for (int i = 0;i <= 22;i++)
	{
		lcd.drawLine(0, 0, i * 6, 96);
		lcd.drawDisplay();
		delay(200);
	}

	for (int i = 0;i<22;i++)
	{
		lcd.drawLine(i * 6, 64, 96, 0);
		lcd.drawDisplay();
		delay(200);
	}

	
	for (int i = 0;i <= 22;i++)
	{
		lcd.xdrawLine(0, 0, i * 6, 96);
		lcd.drawDisplay();
		delay(200);
	}

	for (int i = 0;i<22;i++)
	{
		lcd.xdrawLine(i * 6, 64, 96, 0);
		lcd.drawDisplay();
		delay(200);
	}


	for (int i = 0;i<16;i++)
	{
		lcd.drawCircle(48, 32, i * 3);
		lcd.drawDisplay();
		delay(200);
	}

	for (int i = 0;i<16;i++)
	{
		lcd.xdrawCircle(48, 32, i * 3);
		lcd.drawDisplay();
		delay(200);
	}

	for (int i = 0;i<30;i++)
	{
		lcd.fillCircle(random(96), random(64), random(10));
		lcd.drawDisplay();
		delay(200);
	}

	lcd.clearDisplay();

	for (int i = 0;i<60;i += 6)
	{
		lcd.drawRect(48 - i / 2, 32 - i / 2, i, i);
		lcd.drawDisplay();
		delay(200);
	}

	for (int i = 0;i<60;i += 6)
	{
		lcd.xdrawRect(48 - i / 2, 32 - i / 2, i, i);
		lcd.drawDisplay();
		delay(200);
	}


	for (int i = 0;i<60;i += 6)
	{
		lcd.fillRect(48 - i / 2, 32 - i / 2, i, i);
		lcd.drawDisplay();
		delay(200);
	}

	for (int i = 0;i<30;i += 6)
	{
		lcd.xfillRect(48 - i / 2, 32 - i / 2, i, i);
		lcd.drawDisplay();
		delay(200);
	}

	lcd.clearDisplay();
	for (int i = 0;i<30;i++)
	{
		lcd.drawPixel(random(96), random(64));
		lcd.drawDisplay();
		delay(200);
	}

	lcd.clearDisplay();

	lcd.setCurser(20, 25);
	lcd.fontSize(2);
	lcd.print("END!");
	lcd.drawDisplay();
	

	}

void loop()
{

  /* add main program code here */

}
