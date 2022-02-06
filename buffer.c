#include "buffer.h"


// the array that gets sent in rendering
uint8_t matrix[(BUFFER_WIDTH * BUFFER_HEIGHT * 3) + 1];


uint32_t loc(buffer buff, uint32_t x, uint32_t y) {
	return 1 + 3 * ((y * buff.w) + x);
}

uint8_t clamp(double a) {
    // prevent overflow in cast from double
    return (uint8_t)fmin(255, fmax(0, a));
}

_Bool compare_col(col a, col b) {
	return((a.r == b.r) && (a.g == b.g) && (a.b == b.b));
}

int setup() {
	if(wiringPiSPISetup(SPI_CHANNEL, SPI_SPEED) == -1)
	{
		printf("Could not initialise SPI.\n");
		return(1);
	}
	return(0);
}

void bf_init(buffer *buff) {
    // set the buffer size and allocate memory for pixels
    buff->w = BUFFER_WIDTH;
    buff->h = BUFFER_HEIGHT;
    buff->size = buff->h * buff->w;
    buff->pixels = (uint8_t*)calloc(1 + (buff->size * 3), sizeof(uint8_t));
    buff->pixels[0] = 0x72;
}

void bf_free(buffer *buff) {
    free(buff->pixels);
}

void bf_set_pixel(buffer buff, uint32_t x, uint32_t y, col p) {
    if ((x < buff.w) && (y < buff.h)) {
        buff.pixels[loc(buff, x, y)] = p.r;
        buff.pixels[1 + loc(buff, x, y)] = p.g;
        buff.pixels[2 + loc(buff, x, y)] = p.b;
    };
}

col bf_get_pixel(buffer buff, uint32_t x, uint32_t y) {
	col p;
    if ((x < buff.w) && (y < buff.h)) {
        p.r = buff.pixels[0 + loc(buff, x, y)];
        p.g = buff.pixels[1 + loc(buff, x, y)];
        p.b = buff.pixels[2 + loc(buff, x, y)];
		return p;
    } else {
		p.r = 0;
		p.g = 0;
		p.b = 0;
		return p;
	}
}

void bf_print_pixel(buffer buff, uint32_t x, uint32_t y) {
	col p = bf_get_pixel(buff, x, y);
	printf("x=%d y=%d (%d, %d, %d)\n", x, y, p.r, p.g, p.b);
}

void bf_clear(buffer buff) {
    memset(buff.pixels + 1, 0, sizeof(col) * buff.size);
}

void bf_fill(buffer buff, col p) {
	for (uint32_t x=0; x < buff.w; x++) {
		for (uint32_t y=0; y < buff.h; y++) {
			buff.pixels[loc(buff, x, y)] = p.r;
			buff.pixels[1 + loc(buff, x, y)] = p.g;
			buff.pixels[2 + loc(buff, x, y)] = p.b;
		}
	}
}

void bf_copy(buffer buff_dest, buffer buff_src) {
    memcpy(buff_dest.pixels + 1,
			buff_src.pixels + 1,
			sizeof(col) * ((buff_dest.size < buff_src.size) ? buff_dest.size : buff_src.size));
}

void bf_superpose(buffer buff_a, buffer buff_b) {
    // superpose buff_b over buff_a where buff_b is not 0
	for (uint32_t i=0; i<buff_a.size; i++) {
		uint8_t c = buff_b.pixels[1 + i];
		if (c > 0) {
			buff_a.pixels[1 + i] = c;
		}
	}
}

void bf_render(buffer buff) {
    // Push buffer pixels to the LED matrix;
	// the temp buffer is wiped by the push.
    memcpy(matrix,
			buff.pixels,
			sizeof(uint8_t) * (1 + (3 * buff.size)));
	wiringPiSPIDataRW(SPI_CHANNEL, matrix, 1 + (buff.size * 3)) ;
	// fix to 60fps
	struct timespec req = {.tv_sec = 0, .tv_nsec = (1000000000 / 60)};
	nanosleep(&req, NULL);
}

void bf_print(buffer buff) {
	uint8_t r, g, b;
	for (uint32_t y=buff.h; y>0; --y) {
		for (uint32_t x=0; x < buff.w; x++) {
			r = buff.pixels[0 + loc(buff, x, y)];
			g = buff.pixels[1 + loc(buff, x, y)];
			b = buff.pixels[2 + loc(buff, x, y)];
			printf(" %02X%02X%02X", r, g, b);
		}
		printf("\n");
	}
	printf("\n");
}
