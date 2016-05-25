// Written by Mohammed Rezwanul Islam
// may 12,2016
// 

#include "monoLCD.h"

colorLCD::colorLCD(void)
{}
void colorLCD::data(byte payload)
{
	cbi(PORTC, 2);

	sbi(PORTC, 1);  //set data bit


	sbi(PORTC, 0);
	cbi(PORTC, 0);
	//write payload 8 bits
	for (int i = 7;i >= 0;i--)
	{
		if (!!(payload & (1 << i)))
		{
			sbi(PORTC, 1);
		}
		else
		{
			cbi(PORTC, 1);
		}

		sbi(PORTC, 0);
		cbi(PORTC, 0);
	}
	sbi(PORTC, 2);
}

void colorLCD::command(byte payload)
{
	cbi(PORTC, 2);

	cbi(PORTC, 1);  //write command bit


	sbi(PORTC, 0);
	cbi(PORTC, 0);
	//write payload 8 bits
	for (int i = 7;i >= 0;i--)
	{
		if (!!(payload & (1 << i)))
		{
			sbi(PORTC, 1);
		}
		else
		{
			cbi(PORTC, 1);
		}

		sbi(PORTC, 0);
		cbi(PORTC, 0);
	}
	sbi(PORTC, 2);
}



void colorLCD::begin()
{
	DDRC = 0b00011111;

	command(0xE2);  //reset
	delay(10);
	command(0xA4);  //power save off
	command(0x2F);  //power control set
	command(0xB0);  //set page address
	command(0x10);  //set col=0 upper 3 bits
	command(0x00);  //set col=0 lower 4 bits

	command(0xAF);  //colorLCD display on
	delay(50);
}

void colorLCD::fontSize(uint8_t x)
{
	_fontSize = x;
}

void colorLCD::setCurser(uint8_t x, uint8_t y)
{
	_x_pos = x;
	_y_pos = y;
}

void colorLCD::displayOff()
{
	command(0xAE);
}

void colorLCD::displayOn()
{
	command(0xAF);
}

void colorLCD::displayNormal()
{
	command(0xA6);
}

void colorLCD::displayInverse()
{
	command(0xA7);
}
void colorLCD::LCDReset()
{
	command(0xE2);  //reset
}
void colorLCD::contrast(byte con)
{
	if (con<0) { con = 0; }
	if (con>32) { con = 32; }

	con = con | 0x80;
	command(con);
}
void colorLCD::displayAllPointsOn()
{
	command(0xA5);
}
void colorLCD::displayAllPointsOff()
{
	command(0xA4);
}

void colorLCD::drawPixel(int x, int y)
{
	if (x > 95) { x = 95; }
	if (y > 67) { x = 67; }
	int   row;
	int  modRow;
	row = y / 8;

	unsigned char buffer = displayRAM[x + 96 * row];

	modRow = y % 8;

	byte pixel = 1 << modRow;
	displayRAM[x + 96 * row] = buffer | pixel;

}
void colorLCD::clearPixel(int x, int y)
{
	int   row;
	int  modRow;

	row = y / 8;

	unsigned char buffer = displayRAM[x + 96 * row];

	modRow = y % 8;

	byte pixel = ~(1 << modRow);

	//  number &= ~(1 << x);
	displayRAM[x + 96 * row] = buffer & pixel;

}

void colorLCD::drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1)
{
	int16_t steep = abs(y1 - y0) > abs(x1 - x0);
	if (steep) {
		swap(x0, y0);
		swap(x1, y1);
	}

	if (x0 > x1) {
		swap(x0, x1);
		swap(y0, y1);
	}

	int16_t dx, dy;
	dx = x1 - x0;
	dy = abs(y1 - y0);

	int16_t err = dx / 2;
	int16_t ystep;

	if (y0 < y1) {
		ystep = 1;
	}
	else {
		ystep = -1;
	}

	for (; x0 <= x1; x0++) {
		if (steep) {
			drawPixel(y0, x0);
		}
		else {
			drawPixel(x0, y0);
		}
		err -= dy;
		if (err < 0) {
			y0 += ystep;
			err += dx;
		}
	}
}

void colorLCD::xdrawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1)
{
	int16_t steep = abs(y1 - y0) > abs(x1 - x0);
	if (steep) {
		swap(x0, y0);
		swap(x1, y1);
	}

	if (x0 > x1) {
		swap(x0, x1);
		swap(y0, y1);
	}

	int16_t dx, dy;
	dx = x1 - x0;
	dy = abs(y1 - y0);

	int16_t err = dx / 2;
	int16_t ystep;

	if (y0 < y1) {
		ystep = 1;
	}
	else {
		ystep = -1;
	}

	for (; x0 <= x1; x0++) {
		if (steep) {
			clearPixel(y0, x0);
		}
		else {
			clearPixel(x0, y0);
		}
		err -= dy;
		if (err < 0) {
			y0 += ystep;
			err += dx;
		}
	}
}

void colorLCD::drawFastVLine(int16_t x, int16_t y, int16_t h) {
	drawLine(x, y, x, y + h - 1);
}

void colorLCD::xdrawFastVLine(int16_t x, int16_t y, int16_t h) {
	xdrawLine(x, y, x, y + h - 1);
}

void colorLCD::fillRect(int16_t x, int16_t y, int16_t w, int16_t h) {
	for (int16_t i = x; i<x + w; i++) {
		drawFastVLine(i, y, h);
	}
}

void colorLCD::xfillRect(int16_t x, int16_t y, int16_t w, int16_t h) {
	for (int16_t i = x; i<x + w; i++) {
		xdrawFastVLine(i, y, h);
	}
}

void colorLCD::drawFontPixel(uint8_t x, uint8_t y, uint8_t pixelSize)
{
	fillRect(x, y, pixelSize, pixelSize);
}

void colorLCD::xdrawFontPixel(uint8_t x, uint8_t y, uint8_t pixelSize)
{
	xfillRect(x, y, pixelSize, pixelSize);

}

void colorLCD::character(char character)
{
	if (_x_pos < _fontSize) { _x_pos = _fontSize; }


	for (uint8_t i = 0;i<5;i++)
	{
		for (uint8_t j = 0;j<8;j++)
		{
			byte myByte = 0;
			myByte = pgm_read_byte(&(ASCII[(byte)character - 0x20][i]));
			//myByte = (ASCII[(byte)character - 0x20][i]);

			if (((myByte >> j) & 0b00000001) == 0b00000001)

				//  if((pgm_read_byte_near(ASCII[character][i]  >> j)  & 0b00000001) == 0b00000001)
			{
				//drawPixel(_x_pos, _y_pos);
				drawFontPixel(_x_pos, _y_pos, _fontSize);
			}
			else
			{//draw back color
			 //       clearPixel(_x_pos, _y_pos);
				xdrawFontPixel(_x_pos, _y_pos, _fontSize);
			}

			_y_pos = _y_pos + _fontSize;
		}
		_y_pos = _y_pos - _fontSize * 8;
		_x_pos = _x_pos + _fontSize;

	}
	_x_pos = _x_pos + _fontSize;

	if (_x_pos > _LCD_x)
	{
		_x_pos = 0;
		_y_pos = _y_pos + _fontSize * 8;
	}


}

void colorLCD::print(char *characters)
{
	while (*characters)
	{
		character(*characters++);
	}
}

void colorLCD::drawCircle(int16_t x0, int16_t y0, int16_t r) {
	int16_t f = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x = 0;
	int16_t y = r;

	drawPixel(x0, y0 + r);
	drawPixel(x0, y0 - r);
	drawPixel(x0 + r, y0);
	drawPixel(x0 - r, y0);

	while (x<y) {
		if (f >= 0) {
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;

		drawPixel(x0 + x, y0 + y);
		drawPixel(x0 - x, y0 + y);
		drawPixel(x0 + x, y0 - y);
		drawPixel(x0 - x, y0 - y);
		drawPixel(x0 + y, y0 + x);
		drawPixel(x0 - y, y0 + x);
		drawPixel(x0 + y, y0 - x);
		drawPixel(x0 - y, y0 - x);
	}
}

void colorLCD::xdrawCircle(int16_t x0, int16_t y0, int16_t r) {
	int16_t f = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x = 0;
	int16_t y = r;

	clearPixel(x0, y0 + r);
	clearPixel(x0, y0 - r);
	clearPixel(x0 + r, y0);
	clearPixel(x0 - r, y0);

	while (x<y) {
		if (f >= 0) {
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;

		clearPixel(x0 + x, y0 + y);
		clearPixel(x0 - x, y0 + y);
		clearPixel(x0 + x, y0 - y);
		clearPixel(x0 - x, y0 - y);
		clearPixel(x0 + y, y0 + x);
		clearPixel(x0 - y, y0 + x);
		clearPixel(x0 + y, y0 - x);
		clearPixel(x0 - y, y0 - x);
	}
}

void colorLCD::drawCircleHelper(int16_t x0, int16_t y0,
	int16_t r, uint8_t cornername) {
	int16_t f = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x = 0;
	int16_t y = r;

	while (x<y) {
		if (f >= 0) {
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;
		if (cornername & 0x4) {
			drawPixel(x0 + x, y0 + y);
			drawPixel(x0 + y, y0 + x);
		}
		if (cornername & 0x2) {
			drawPixel(x0 + x, y0 - y);
			drawPixel(x0 + y, y0 - x);
		}
		if (cornername & 0x8) {
			drawPixel(x0 - y, y0 + x);
			drawPixel(x0 - x, y0 + y);
		}
		if (cornername & 0x1) {
			drawPixel(x0 - y, y0 - x);
			drawPixel(x0 - x, y0 - y);
		}
	}
}

void colorLCD::xdrawCircleHelper(int16_t x0, int16_t y0,
	int16_t r, uint8_t cornername) {
	int16_t f = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x = 0;
	int16_t y = r;

	while (x<y) {
		if (f >= 0) {
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;
		if (cornername & 0x4) {
			clearPixel(x0 + x, y0 + y);
			clearPixel(x0 + y, y0 + x);
		}
		if (cornername & 0x2) {
			clearPixel(x0 + x, y0 - y);
			clearPixel(x0 + y, y0 - x);
		}
		if (cornername & 0x8) {
			clearPixel(x0 - y, y0 + x);
			clearPixel(x0 - x, y0 + y);
		}
		if (cornername & 0x1) {
			clearPixel(x0 - y, y0 - x);
			clearPixel(x0 - x, y0 - y);
		}
	}
}

void colorLCD::drawDisplay()
{
	for (int index = 0; index < 96 * 9; index++)
	{
		{data(displayRAM[index]);}
	}
}

void colorLCD::clearDisplay()
{
	memset(displayRAM, 0, 864);

	drawDisplay();
}


void  colorLCD::drawFastHLine(int16_t x, int16_t y, int16_t w) {
	drawLine(x, y, x + w - 1, y);
}

void  colorLCD::xdrawFastHLine(int16_t x, int16_t y, int16_t w) {
	xdrawLine(x, y, x + w - 1, y);
}


void  colorLCD::drawRect(int16_t x, int16_t y, int16_t w, int16_t h)
{
	drawFastHLine(x, y, w);
	drawFastHLine(x, y + h - 1, w);
	drawFastVLine(x, y, h);
	drawFastVLine(x + w - 1, y, h);
}
void  colorLCD::xdrawRect(int16_t x, int16_t y, int16_t w, int16_t h)
{
	xdrawFastHLine(x, y, w);
	xdrawFastHLine(x, y + h - 1, w);
	xdrawFastVLine(x, y, h);
	xdrawFastVLine(x + w - 1, y, h);
}

void  colorLCD::fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta)
{

	int16_t f = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x = 0;
	int16_t y = r;

	while (x<y) {
		if (f >= 0) {
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;

		if (cornername & 0x1) {
			drawFastVLine(x0 + x, y0 - y, 2 * y + 1 + delta);
			drawFastVLine(x0 + y, y0 - x, 2 * x + 1 + delta);
		}
		if (cornername & 0x2) {
			drawFastVLine(x0 - x, y0 - y, 2 * y + 1 + delta);
			drawFastVLine(x0 - y, y0 - x, 2 * x + 1 + delta);
		}
	}
}
void  colorLCD::xfillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta)
{

	int16_t f = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x = 0;
	int16_t y = r;

	while (x<y) {
		if (f >= 0) {
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;

		if (cornername & 0x1) {
			xdrawFastVLine(x0 + x, y0 - y, 2 * y + 1 + delta);
			xdrawFastVLine(x0 + y, y0 - x, 2 * x + 1 + delta);
		}
		if (cornername & 0x2) {
			xdrawFastVLine(x0 - x, y0 - y, 2 * y + 1 + delta);
			xdrawFastVLine(x0 - y, y0 - x, 2 * x + 1 + delta);
		}
	}
}

void  colorLCD::fillCircle(int16_t x0, int16_t y0, int16_t r)
{
	drawFastVLine(x0, y0 - r, 2 * r + 1);
	fillCircleHelper(x0, y0, r, 3, 0);
}

void  colorLCD::xfillCircle(int16_t x0, int16_t y0, int16_t r)
{
	xdrawFastVLine(x0, y0 - r, 2 * r + 1);
	xfillCircleHelper(x0, y0, r, 3, 0);
}

void  colorLCD::fillRoundRect(int16_t x, int16_t y, int16_t w,
	int16_t h, int16_t r) {
	// smarter version
	fillRect(x + r, y, w - 2 * r, h);

	// draw four corners
	fillCircleHelper(x + w - r - 1, y + r, r, 1, h - 2 * r - 1);
	fillCircleHelper(x + r, y + r, r, 2, h - 2 * r - 1);
}

void  colorLCD::xfillRoundRect(int16_t x, int16_t y, int16_t w,
	int16_t h, int16_t r) {
	// smarter version
	xfillRect(x + r, y, w - 2 * r, h);

	// draw four corners
	xfillCircleHelper(x + w - r - 1, y + r, r, 1, h - 2 * r - 1);
	xfillCircleHelper(x + r, y + r, r, 2, h - 2 * r - 1);
}


void  colorLCD::drawRoundRect(int16_t x, int16_t y, int16_t w,
	int16_t h, int16_t r) {
	// smarter version
	drawFastHLine(x + r, y, w - 2 * r); // Top
	drawFastHLine(x + r, y + h - 1, w - 2 * r); // Bottom
	drawFastVLine(x, y + r, h - 2 * r); // Left
	drawFastVLine(x + w - 1, y + r, h - 2 * r); // Right
												// draw four corners
	drawCircleHelper(x + r, y + r, r, 1);
	drawCircleHelper(x + w - r - 1, y + r, r, 2);
	drawCircleHelper(x + w - r - 1, y + h - r - 1, r, 4);
	drawCircleHelper(x + r, y + h - r - 1, r, 8);
}

void  colorLCD::xdrawRoundRect(int16_t x, int16_t y, int16_t w,
	int16_t h, int16_t r) {
	// smarter version
	xdrawFastHLine(x + r, y, w - 2 * r); // Top
	xdrawFastHLine(x + r, y + h - 1, w - 2 * r); // Bottom
	xdrawFastVLine(x, y + r, h - 2 * r); // Left
	xdrawFastVLine(x + w - 1, y + r, h - 2 * r); // Right
												 // draw four corners
	xdrawCircleHelper(x + r, y + r, r, 1);
	xdrawCircleHelper(x + w - r - 1, y + r, r, 2);
	xdrawCircleHelper(x + w - r - 1, y + h - r - 1, r, 4);
	xdrawCircleHelper(x + r, y + h - r - 1, r, 8);
}

