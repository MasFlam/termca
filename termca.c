#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_FILENAME_LEN 255

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

static inline void displayGrid(State *grid, size_t width, size_t height, bool showSize) {
	State lastVal, val;
	for (size_t y = 1; y < height-1; ++y) {
		lastVal = -1;
		for (size_t x = 1; x < width-1; ++x) {
			val = grid[y*width + x];
			if (lastVal != val)
				printf(
					"\x1b[38;2;%d;%d;%dm",
					(color[val] >> 16) & 0xff,
					(color[val] >> 8) & 0xff,
					color[val] & 0xff
				);
			putchar(symbol[val]);
			lastVal = val;
		}
		printf("\x1b[39m");
		if (showSize) {
			if (y == height-2)
				printf(" %zu", height-2-1);
			else if (y == 1)
				printf(" 0");
		}
		putchar('\n');
	}
	if (showSize) printf("%-*d%zu", (int)width-2-1, 0, width-2);
	putchar('\n');
}

static inline void readGridFromFile(FILE *infp, State *grid, size_t width, size_t height) {
	int val;
	for (size_t y = 1; y < height-1; ++y) {
		for (size_t x = 1; x < width-1; ++x) {
			fscanf(infp, " %d", &val);
			grid[y*width + x] = val;
		}
	}
}

static inline void saveGridToFile(FILE *ofp, State *grid, size_t width, size_t height) {
	fprintf(ofp, "%zu %zu\n", width-2, height-2);
	for (size_t y = 1; y < height-1; ++y) {
		for (size_t x = 1; x < width-1; ++x) {
			fprintf(ofp, " %d", grid[y*width + x]);
		}
		fputc('\n', ofp);
	}
}

static inline void visualize(FILE *infp) {
	size_t width, height;
	
	fscanf(infp, " %zu %zu", &width, &height);
	width += 2;
	height += 2;
	
	State *oldGrid = calloc(width * height, sizeof(State));
	State *newGrid = calloc(width * height, sizeof(State));
	
	readGridFromFile(infp, oldGrid, width, height);
	
	int frameNumber = 0;
	clock_t oldClock, newClock;
	while (true) {
		++frameNumber;
		printf("\x1b[3J\x1b[;f");
		oldClock = clock();
		
		displayGrid(oldGrid, width, height, false);
		for (size_t y = 1; y < height-1; ++y) {
			for (size_t x = 1; x < width-1; ++x) {
				transitionFunction(oldGrid, newGrid, width, height, x, y);
			}
		}
		memcpy(oldGrid, newGrid, width * height * sizeof(State));
		
		newClock = clock();
		float frameTime = (float) (newClock - oldClock) / CLOCKS_PER_SEC * 1000;
		printf(
			"^C - stop    Enter - next    q - next file" "\n"
			"frame number: %d" "\n"
			"frame time: %.1fms" "\n",
			frameNumber,
			frameTime
		);
		int ch = getchar();
		if (ch == 'q')
			goto fin;
		else if (ch == EOF)
			exit(1);
	}
	
fin:
	free(oldGrid);
	free(newGrid);
}

static inline void doEditor() {
	size_t width, height;
	State *grid;
	
	printf(
		"\x1b[3J\x1b[;f"
		"Read from file (.filename, max %d characters) or create new grid (Enter): ",
		MAX_FILENAME_LEN
	);
	
	char filename[MAX_FILENAME_LEN+1];
	fgets(filename, MAX_FILENAME_LEN, stdin);
	if (filename[0] == '.') {
		*strchr(filename, '\n') = '\0';
		
		FILE *ifp = fopen(filename+1, "r");
		if (!ifp) {
			fprintf(stderr, "Error opening file: %s\n", filename+1);
			exit(1);
		}
		
		fscanf(ifp, " %zu %zu", &width, &height);
		width += 2;
		height += 2;
		
		grid = malloc(width * height * sizeof(State));
		readGridFromFile(ifp, grid, width, height);
		
		fclose(ifp);
	} else {
		printf(
			"\x1b[3J\x1b[;f"
			"Enter the grid width: "
		);
		
		scanf(" %zu", &width);
		printf("Enter the grid height: ");
		scanf(" %zu", &height);
		width += 2;
		height += 2;

		grid = calloc(width * height, sizeof(State));
	}
	
	while (true) {
		printf("\x1b[3J\x1b[;f");
		displayGrid(grid, width, height, true);
		printf(
			":x,y - change cell at (x,y)" "\n"
			".filename - save grid to filename (max %d characters)" "\n"
			"q - quit (remember to save first!)" "\n",
			MAX_FILENAME_LEN
		);
		
		int ch = getchar();
		if (ch == ':') {
			size_t x, y;
			scanf("%zu,%zu", &x, &y);
			printf("Chose cell state: (Enter the number)\n");
			#define STATE(name, symbol, _) \
				printf( \
					"\x1b[38;2;%d;%d;%dm(%d, %c)\x1b[39m %s\n", \
					(color[name] >> 16) & 0xff, \
					(color[name] >> 8) & 0xff, \
					color[name] & 0xff, \
					name, symbol, \
					#name \
				);
			#include "states.h"
			#undef STATE
			int state;
			scanf(" %d", &state);
			grid[(y+1)*width + (x+1)] = state;
		} else if (ch == '.') {
			char filename[MAX_FILENAME_LEN+1];
			fgets(filename, MAX_FILENAME_LEN, stdin);
			*strchr(filename, '\n') = '\0';
			FILE *fp = fopen(filename, "w+");
			if (!fp) {
				fprintf(stderr, "Error opening file: %s\n", filename);
				exit(1);
			}
			saveGridToFile(fp, grid, width, height);
			fclose(fp);
		} else if (ch == 'q') {
			printf("Goodbye!\n");
			exit(0);
		} else if (ch == EOF) exit(1);
	}
	
	free(grid);
}

int main(int argc, char **argv) {
	if (argc <= 1) {
		doEditor();
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
