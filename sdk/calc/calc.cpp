#include <cstdint>
#include <cstddef>
#include <type_traits>
#include <sdk/calc/calc.h>
#include <sdk/os/lcd.h>
#include <sdk/os/debug.h>

//Draw a line (bresanham line algorithm)
extern "C"
void line(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, uint16_t color) {
	auto dx = (x1 < x2 ? x2 - x1 : x1 - x2);
	auto dy = (y1 < y2 ? y2 - y1 : y1 - y2);

	if (dx >= dy) { //the derivative is less than 1 (not so steep)
		if (x1 > x2) { auto z = x2; x2 = x1; x1 = z; z = y2, y2 = y1, y1 = z; }
		setPixel(x1, y1, color);
		//y1 is the whole number of the y value
		//error is the fractional part (times dx to make it a whole number)
		// y = y1 + (error/dx)
		//if error/dx is greater than 0.5 (error is greater than dx/2) we add 1 to y1 and subtract dx from error (so error/dx is now around -0.5) 
		auto error = dx / 2;
		while (x1 != x2) {
			x1 += 1; //go one step in x direction
			error += dy;//add dy/dx to the y value.
			if (error >= dx){ //If error is greater than dx/2 (error/dx is >=0.5)
				y1 += (y1 < y2 ? 1 : -1);
				error -= dx;
			}
			setPixel(x1, y1, color);
		}
	} else { //the derivative is greater than 1 (very steep)
		if (y1 > y2) { auto z = y2; y2 = y1; y1 = z; z = x2, x2 = x1, x1 = z; }
		setPixel(x1, y1, color);
		auto error = dy / 2;
		while (y1 != y2) { //The same thing, just go up y and look at x
			y1 += 1; //go one step in y direction
			error += dx;//add dx/dy to the x value.
			if (error >= dy){ //If error is greater than dx/2 (error/dx is >=0.5)
				x1 += (x1 < x2 ? 1 : -1);
				error -= dy;
			}
			setPixel(x1, y1, color);
		}
	}
}

extern "C"
void vline(unsigned int x, unsigned int y1, unsigned int y2, uint16_t color) { //vertical line needed for triangle()
	if (y1 > y2) { auto z = y2; y2 = y1; y1 = z; }
	for (auto y = y1; y <= y2; y++)
		setPixel(x, y, color);
}

//Draw a filled triangle.
extern "C"
void triangle(unsigned int x0, unsigned int y0, unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, uint16_t colorFill, uint16_t colorLine){
//Filled triangles are a lot of vertical lines.
/*                                                               -
                        a   ___________----------P3              -
       P0 _________---------              ____---                -
          ---____               _____-----                       -
               b ----___  _-----   c                             -
                        P2                                       -
The triangle has three points P0, P1 and P2 and three lines a, b and c. We go from left to right, calculating the point on a and the point on b or c and then we draw a vertical line connecting these two.
*/

	//Sort the points by x coordinate
	{
		decltype(x0) z;
		if (x0 > x2) { z = x2; x2 = x0; x0 = z; z = y2; y2 = y0; y0 = z; }
		if (x1 > x2) { z = x2; x2 = x1; x1 = z; z = y2; y2 = y1; y1 = z; }
		if (x0 > x1) { z = x1; x1 = x0; x0 = z; z = y1; y1 = y0; y0 = z; }
	}

	auto x = x0; //x is the variable that counts from left to right

	//Values for line a
	auto ay = y0; //The point y for the current x on the line a
	std::make_signed_t<decltype(ay)> aiy; //The direction of line a
	auto adx = (x2 > x0 ? (x2 - x0) : (x0 - x2));
	decltype(ay) ady;
	if (y2 > y0) { aiy = 1; ady = y2 - y0; } else { aiy = -1; ady = y0 - y2; }
	decltype(aiy) aerr = 0; //The y value of a (fractional part). y is actually ay+(aerr/adx)

	//Values for line b
	auto by = y0; //The point y for the current x on the line b
	std::make_signed_t<decltype(by)> biy; //The direction of line b
	auto bdx = (x1>x0 ? (x1 - x0) : (x0 - x1));
	decltype(by) bdy;
	if (y1 > y0) { biy = 1; bdy = y1 - y0; } else { biy = -1; bdy = y0 - y1; }
	decltype(biy) berr = 0;

	//Values for line c
	auto cy = y1; //The point y for the current x on the line y (starting at P1)
	std::make_signed_t<decltype(cy)> ciy; //The direction of line c
	auto cdx = (x2 > x1 ? (x2 - x1) : (x1 - x2));
	decltype(cy) cdy;
	if (y2 > y1) { ciy = 1; cdy = y2 - y1; } else { ciy = -1; cdy = y1 - y2; }
	decltype(ciy) cerr = 0;

	//First draw area between a and b
	while (x < x1) {
		x++;
		aerr += ady;
		while (aerr >= static_cast<decltype(aerr)>(adx / 4)) { //if aerr/adx >= 0.5
			aerr -= adx;
			ay += aiy;
		}
		berr += bdy;
		while (berr >= static_cast<decltype(berr)>(bdx / 4)) { //if berr/bdx >= 0.5
			berr -= bdx;
			by += biy;
		}
		vline(x, ay, by, colorFill);
	}

	//Then draw area between a and c
	while (x < x2 - 1) { //we don't need x=x2, bacause x should already have the right vaue... 
		x++;
		aerr += ady;
		while(aerr >= static_cast<decltype(aerr)>(adx / 4)) { //if aerr/adx >= 0.5
			aerr -= adx;
			ay += aiy;
		}
		cerr += cdy;
		while(cerr >= static_cast<decltype(cerr)>(cdx / 4)) { //if berr/bdx >= 0.5
			cerr -= cdx;
			cy += ciy;
		}
		vline(x, ay, cy, colorFill);
	}

	line(x0, y0, x1, y1, colorLine);
	line(x1, y1, x2, y2, colorLine);
	line(x2, y2, x0, y0, colorLine);
}

extern "C"
void fillScreen(uint16_t color) {
	for(size_t i = 0; i < width * height; i++)
		vram[i] = color;
}

extern "C" void getKey(uint32_t *key1, uint32_t *key2) {
    const auto hw = reinterpret_cast<volatile uint16_t *>(0xa44B0000);
    *key1 = hw[0] << 16 | hw[1];
    *key2 = hw[2] << 16 | hw[3];
    hw[4];
    hw[5];
}

extern "C" __attribute__((constructor(99), weak)) void calcInit(void) {
	LCD_VRAMBackup();
}

extern "C" __attribute__((destructor(99), weak)) void calcExit(void) {
	LCD_VRAMRestore();
	LCD_Refresh();
}