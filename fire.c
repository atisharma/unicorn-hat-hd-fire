#include "fire.h"

const col red = {255, 0, 0};
const col green = {0, 255, 0};
const col blue = {0, 0, 255};

const col black = {0, 0, 0};
const col grey = {128, 128, 128};
const col white = {255, 255, 255};

const col wood = {80, 60, 35};

const col fire_0 = {180, 40, 20};
const col fire_1 = {128, 9, 9};
const col fire_2 = {162, 22, 9};
const col fire_3 = {190, 53, 2};
const col fire_4 = {240, 127, 19};
const col fire_5 = {242, 125, 12};
const col fire_6 = {153, 100, 12};
const col fire_7 = {140, 60, 10};

const col ash_0 = {120, 50, 10};
const col ash_1 = {100, 50, 10};
const col ash_2 = {80, 40, 5};
const col ash_3 = {40, 20, 0};


col fire_cols[14] = {
	wood,
	fire_0,
	fire_1,
	fire_2,
	fire_3,
	fire_4,
	fire_5,
	fire_6,
	fire_7,
	ash_0,
	ash_1,
	ash_2,
	ash_3,
	black
};


void fire_seed(buffer buff) {
	for (uint32_t x=6; x<buff.w - 6; x++) {
		bf_set_pixel(buff, x, 0, wood);
	}
}

void fire_burn(buffer buff) {
	col p;
	uint32_t x = rand() % 16;
	uint32_t y = rand() % 16;
	uint32_t dx = rand() % 3;
	// random pixel rises to left, right, or middle
	p = bf_get_pixel(buff, x, y);
	bf_set_pixel(buff, x+dx-1, y+1, p);
	// old pixels progress colour
	for (int i=0; i<14; i++) {
		if (compare_col(p, fire_cols[i])) {
			bf_set_pixel(buff, x, y, fire_cols[i+1]);
			continue;
		}
	}
}

