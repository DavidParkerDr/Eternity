#pragma once
#include <map>
using namespace std;

static const int NORTH	= 0;
static const int EAST	= 1;
static const int SOUTH	= 2;
static const int WEST	= 3;

class Tile
{
	// Public Members
public:

	// Members
private:
	int		_id;
	int		_numEdges;
	int		_patternIDs[4];
	char	_patterns[4];
	int		_rotation;
	bool	_used;
	bool	_fixed;
	int		_x, _y;

	// Structors
public:
	Tile(int id, char n, char e, char s, char w);
	~Tile();

	// Accessors
public:
	int getID()					const	{ return _id; }
	int getRotation()			const	{ return _rotation; }
	char getPattern(int i)		const	{ return _patterns[i]; }
	int getPatternID(int i)		const	{ return _patternIDs[i]; }
	char getFixedNorth()		const	{ return _patterns[NORTH]; }
	char getFixedEast()			const	{ return _patterns[EAST]; }
	char getFixedSouth()		const	{ return _patterns[SOUTH]; }
	char getFixedWest()			const	{ return _patterns[WEST]; }
	char north()				const	{ return _patterns[wrap(NORTH-_rotation)]; }
	char east()					const	{ return _patterns[wrap(EAST-_rotation)]; }
	char south()				const	{ return _patterns[wrap(SOUTH-_rotation)]; }
	char west()					const	{ return _patterns[wrap(WEST-_rotation)]; }
	int northID()				const	{ return _patternIDs[wrap(NORTH-_rotation)]; }
	int eastID()				const	{ return _patternIDs[wrap(EAST-_rotation)]; }
	int southID()				const	{ return _patternIDs[wrap(SOUTH-_rotation)]; }
	int westID()				const	{ return _patternIDs[wrap(WEST-_rotation)]; }
	int getX()					const	{ return _x; }
	int getY()					const	{ return _y; }
	int numEdges()				const	{ return _numEdges; }
	bool isUsed()				const	{ return _used; }
	bool isFixed()				const	{ return _fixed; }
	bool isEdge()				const	{ return _numEdges == 1; }
	bool isCorner()				const	{ return _numEdges == 2; }
	bool isMutant()				const	{ return _numEdges == 3; }
	bool isFreak()				const	{ return _numEdges == 4; }
	bool isFreakishMonster()	const	{ return _numEdges > 4; }
	bool isImpossible()			const	{ return _numEdges < 0; }

	// Mutators
public:
	void setFixed(bool v)				{ _fixed = v; }
	void setPosition(int x, int y)		{ _x = x; _y = y; }
	void setRotation(int r)				{ _rotation = r; }
	void setUsed(bool v)				{ _used = v; }

	// Functions
public:
	void decRotation()					{ _rotation = wrap(_rotation - 1); }
	void incRotation()					{ _rotation++; if (_rotation == 4) _rotation = 0; }
	void reset()						{ _x = -1; _y = -1; _rotation = 0; _used = false; }
	
	// Private Functions
private:
	static int wrap(int x)				{ if (x < 0) x += 4; return x; }
};

struct Grout
{
	Grout()					{ tile = NULL; rotation = 0; }
	Grout(Tile* t, int r)	{ tile = t; rotation = r; }
	Tile* tile;
	int   rotation;
};
