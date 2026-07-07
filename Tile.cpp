#include ".\tile.h"

/******************************************************************/

Tile::Tile(int id, char n, char e, char s, char w)
: _fixed(false), _id(id), _numEdges(0), _rotation(0), _used(false), _x(-1), _y(-1)
{
	_patterns[NORTH]	= n;
	_patterns[EAST]		= e;
	_patterns[SOUTH]	= s;
	_patterns[WEST]		= w;

	// Set num edges, set pattern IDs
	for (int i = 0; i < 4; i++) 
	{
		_patternIDs[i] = _patterns[i] - 'A' + 1;

		if (_patterns[i] == 'A') _numEdges ++;
	}
}

/******************************************************************/

Tile::~Tile(void)
{
}

/******************************************************************/
