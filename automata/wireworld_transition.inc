TRANSITION_FUNCTION(oldGrid, newGrid, width, height, x, y) {
	int headCount = 0;
	for (int i = -1; i <= 1; ++i) {
		for (int j = -1; j <= 1; ++j) {
			if ((i != 0 || j != 0) && GET(x+i, y+j) == HEAD) {
				++headCount;
			}
		}
	}
	switch (GET(x, y)) {
		case EMPTY: SET(x, y, EMPTY); break;
		case HEAD:  SET(x, y, TAIL); break;
		case TAIL:  SET(x, y, TRACE); break;
		case TRACE: {
			if (headCount == 1 || headCount == 2)
				SET(x, y, HEAD);
			else
				SET(x, y, TRACE);
		} break;
	}
}
