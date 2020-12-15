#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define RGB(r, g, b) (((r) << 16) | ((g) << 8) | (b))

typedef enum {
	#define STATE(name, symbol, color) name,
	#include "states.h"
	#undef STATE
} State;

static const int32_t color[] = {
	#define STATE(name, symbol, color) color,
	#include "states.h"
	#undef STATE
};

static const char symbol[] = {
	#define STATE(name, symbol, color) symbol,
	#include "states.h"
	#undef STATE
};

#define GET(x, y) (oldGrid[(y) * width + (x)])
#define SET(x, y, val) (newGrid[(y) * width + (x)] = (val))
#define TRANSITION_FUNCTION(oldGrid, newGrid, width, height, x, y) \
	static inline void transitionFunction(State *oldGrid, State *newGrid, size_t width, size_t height, size_t x, size_t y)
#include "transition.inc"
#undef GET
#undef SET
#undef TRANSITION_FUNCTION

static inline void displayGrid(State *grid, size_t width, size_t height) {
	State lastVal, val;
	for (size_t y = 1; y < height-1; ++y) {
		lastVal = -1;
		for (size_t x = 1; x < width-1; ++x) {
			val = grid[y*width + x];
			if (lastVal != val)
				printf("\x1b[38;2;%d;%d;%dm", color[val] >> 16, color[val] >> 8, color[val]);
			putchar(symbol[val]);
			lastVal = val;
		}
		printf("\x1b[39m\n");
	}
	putchar('\n');
}

static inline void readInitial(FILE *infp, State *grid, size_t width, size_t height) {
	int val;
	for (size_t y = 1; y < height-1; ++y) {
		for (size_t x = 1; x < width-1; ++x) {
			fscanf(infp, " %d", &val);
			grid[y*width + x] = val;
		}
	}
}

static inline void visualize(FILE *infp) {
	size_t width, height;
	
	fscanf(infp, " %zu %zu", &width, &height);
	width += 2;
	height += 2;
	
	State *oldGrid = calloc(width * height, sizeof(State));
	State *newGrid = calloc(width * height, sizeof(State));
	
	readInitial(infp, oldGrid, width, height);
	
	int frameNumber = 0;
	clock_t oldClock, newClock;
	while (true) {
		++frameNumber;
		printf("\x1b[3J\x1b[;f");
		oldClock = clock();
		
		displayGrid(oldGrid, width, height);
		for (size_t y = 1; y < height-1; ++y) {
			for (size_t x = 1; x < width-1; ++x) {
				transitionFunction(oldGrid, newGrid, width, height, x, y);
			}
		}
		memcpy(oldGrid, newGrid, width * height * sizeof(State));
		
		newClock = clock();
		float frameTime = (float) (newClock - oldClock) / CLOCKS_PER_SEC * 1000;
		printf(
			"^C - stop    ^D - sonic    Enter - next" "\n"
			"frame number: %d" "\n"
			"frame time: %.1fms" "\n",
			frameNumber,
			frameTime
		);
		getchar();
	}
	
	free(oldGrid);
	free(newGrid);
}

int main(int argc, char **argv) {
	if (argc <= 1) {
		fprintf(stderr, "Usage: termca <in-files...>\n");
		return 2;
	} else {
		for (int i = 1; i < argc; ++i) {
			char *name = argv[i];
			FILE *fp = fopen(name, "r");
			if (!fp) {
				fprintf(stderr, "Error opening file: %s\n", name);
				return 1;
			}
			visualize(fp);
			fclose(fp);
		}
	}
	
	return 0;
}
