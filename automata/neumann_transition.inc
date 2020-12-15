TRANSITION_FUNCTION(oldGrid, newGrid, width, height, x, y) {
	State north = GET(x, y-1);
	State south = GET(x, y+1);
	State west = GET(x-1, y);
	State east = GET(x+1, y);
	bool o_in = north == OTRANSSE || south == OTRANSNE || west == OTRANSEE || east == OTRANSWE;
	bool s_in = north == STRANSSE || south == STRANSNE || west == STRANSEE || east == STRANSWE;
	switch (GET(x, y)) {
		case GROUND: {
			if (o_in || s_in)
				SET(x, y, SENS);
			else
				SET(x, y, GROUND);
		} break;
		case SENS: {
			if (o_in || s_in)
				SET(x, y, SENS1);
			else
				SET(x, y, SENS0);
		} break;
		case SENS1: {
			if (o_in || s_in)
				SET(x, y, SENS11);
			else
				SET(x, y, SENS10);
		} break;
		case SENS11: {
			if (o_in || s_in)
				SET(x, y, CFLU00);
			else
				SET(x, y, STRANSSQ);
		} break;
		case SENS10: {
			if (o_in || s_in)
				SET(x, y, STRANSWQ);
			else
				SET(x, y, STRANSNQ);
		} break;
		case SENS0: {
			if (o_in || s_in)
				SET(x, y, SENS01);
			else
				SET(x, y, SENS00);
		} break;
		case SENS01: {
			if (o_in || s_in)
				SET(x, y, STRANSEQ);
			else
				SET(x, y, OTRANSSQ);
		} break;
		case SENS00: {
			if (o_in || s_in)
				SET(x, y, OTRANSWQ);
			else
				SET(x, y, SENS000);
		} break;
		case SENS000: {
			if (o_in || s_in)
				SET(x, y, OTRANSNQ);
			else
				SET(x, y, OTRANSEQ);
		} break;
		case CFLU00:
		case CFLU10: {
			if (s_in)
				SET(x, y, GROUND);
			else if (o_in)
				SET(x, y, CFLU01);
			else
				SET(x, y, CFLU00);
		} break;
		case CFLU01:
		case CFLU11: {
			if (s_in)
				SET(x, y, GROUND);
			else if (o_in)
				SET(x, y, CFLU11);
			else
				SET(x, y, CFLU10);
		} break;
		case OTRANSNE:
		case OTRANSNQ: {
			if (s_in)
				SET(x, y, GROUND);
			else if (
				south == OTRANSNE || west == OTRANSEE || east == OTRANSWE ||
				south == CFLU10   || west == CFLU10   || east == CFLU10   ||
				south == CFLU11   || west == CFLU11   || east == CFLU11
			) SET(x, y, OTRANSNE);
			else SET(x, y, OTRANSNQ);
		} break;
		case OTRANSSE:
		case OTRANSSQ: {
			if (s_in)
				SET(x, y, GROUND);
			else if (
				north == OTRANSSE || west == OTRANSEE || east == OTRANSWE ||
				north == CFLU10   || west == CFLU10   || east == CFLU10   ||
				north == CFLU11   || west == CFLU11   || east == CFLU11
			) SET(x, y, OTRANSSE);
			else SET(x, y, OTRANSSQ);
		} break;
		case OTRANSWE:
		case OTRANSWQ: {
			if (s_in)
				SET(x, y, GROUND);
			else if (
				north == OTRANSSE || south == OTRANSNE || east == OTRANSWE ||
				north == CFLU10   || south == CFLU10   || east == CFLU10   ||
				north == CFLU11   || south == CFLU11   || east == CFLU11
			) SET(x, y, OTRANSWE);
			else SET(x, y, OTRANSWQ);
		} break;
		case OTRANSEE:
		case OTRANSEQ: {
			if (s_in)
				SET(x, y, GROUND);
			else if (
				north == OTRANSSE || south == OTRANSNE || west == OTRANSEE ||
				north == CFLU10   || south == CFLU10   || west == CFLU10   ||
				north == CFLU11   || south == CFLU11   || west == CFLU11
			) SET(x, y, OTRANSEE);
			else SET(x, y, OTRANSEQ);
		} break;
		case STRANSNE:
		case STRANSNQ: {
			if (o_in)
				SET(x, y, GROUND);
			else if (
				south == STRANSNE || west == STRANSEE || east == STRANSWE ||
				south == CFLU10   || west == CFLU10   || east == CFLU10   ||
				south == CFLU11   || west == CFLU11   || east == CFLU11
			) SET(x, y, STRANSNE);
			else SET(x, y, STRANSNQ);
		} break;
		case STRANSSE:
		case STRANSSQ: {
			if (o_in)
				SET(x, y, GROUND);
			else if (
				north == STRANSSE || west == STRANSEE || east == STRANSWE ||
				north == CFLU10   || west == CFLU10   || east == CFLU10   ||
				north == CFLU11   || west == CFLU11   || east == CFLU11
			) SET(x, y, STRANSSE);
			else SET(x, y, STRANSSQ);
		} break;
		case STRANSWE:
		case STRANSWQ: {
			if (o_in)
				SET(x, y, GROUND);
			else if (
				north == STRANSSE || south == STRANSNE || east == STRANSWE ||
				north == CFLU10   || south == CFLU10   || east == CFLU10   ||
				north == CFLU11   || south == CFLU11   || east == CFLU11
			) SET(x, y, STRANSWE);
			else SET(x, y, STRANSWQ);
		} break;
		case STRANSEE:
		case STRANSEQ: {
			if (o_in)
				SET(x, y, GROUND);
			else if (
				north == STRANSSE || south == STRANSNE || west == STRANSEE ||
				north == CFLU10   || south == CFLU10   || west == CFLU10   ||
				north == CFLU11   || south == CFLU11   || west == CFLU11
			) SET(x, y, STRANSEE);
			else SET(x, y, STRANSEQ);
		} break;
	}
}
