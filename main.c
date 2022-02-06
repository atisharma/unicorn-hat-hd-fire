#include "buffer.h"
#include "fire.h"


int main( int argc, char *argv[] )
{
	if(setup() == 1) {
		return(EXIT_FAILURE);
	}

	srand(time(NULL));

	buffer flame, seed;
	bf_init(&seed);
	bf_init(&flame);

	fire_seed(seed);

	while(1) {
		bf_superpose(flame, seed);
		for (int j=0; j<15; j++) {
			fire_burn(flame);
		}
		//bf_print(flame);
		bf_render(flame);
	}

	bf_free(&seed);
	bf_free(&flame);

	return(EXIT_SUCCESS);
}
