TRANSITION_FUNCTION(oldGrid, newGrid, width, height, x, y) {
	int sum =
		GET(x+1, y+1) + GET(x+1, y) + GET(x+1, y-1) + GET(x, y-1) +
		GET(x-1, y-1) + GET(x-1, y) + GET(x-1, y+1) + GET(x, y+1);
	switch (GET(x, y)) {
		case ALIVE: {
			if (sum < 2 || sum > 3)
				SET(x, y, DEAD);
			else
				SET(x, y, ALIVE);
		} break;
		case DEAD: {
			if (sum == 3)
				SET(x, y, ALIVE);
			else
				SET(x, y, DEAD);
		} break;
	}
}
