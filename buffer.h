/*
 *
 * Functions to manipilate a buffer to blit to the
 * Pimoroni Unicorn Hat HD
 *
 * Unicorn Hat HD is 16x16x3, with a leading byte 0x72
 *
 * A S Sharma 2022
 *
 */

#ifndef BUFFER_H
#define BUFFER_H

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <string.h>
#include <time.h>
#include <math.h>

#include <wiringPiSPI.h>


#define SPI_CHANNEL 0
#define SPI_SPEED   9000000

#define BUFFER_WIDTH 16
#define BUFFER_HEIGHT 16


typedef struct {
	uint8_t r;
	uint8_t g;
	uint8_t b;
} col;

typedef struct {
	uint8_t *pixels;
	uint32_t h;
	uint32_t w;
	uint32_t size;
} buffer;


uint8_t clamp(double a);

_Bool compare_col(col a, col b);

int setup();

void bf_init(buffer *buff);

void bf_free(buffer *buff);

void bf_set_pixel(buffer buff, uint32_t x, uint32_t y, col p);

col bf_get_pixel(buffer buff, uint32_t x, uint32_t y);

void bf_print_pixel(buffer buff, uint32_t x, uint32_t y);

void bf_clear(buffer buff);

void bf_fill(buffer buff, col p);

void bf_copy(buffer buff_dest, buffer buff_src);

void bf_superpose(buffer buff_b, buffer buff_a);

void bf_render(buffer buff);

void bf_print(buffer buff);

#endif // BUFFER_H
