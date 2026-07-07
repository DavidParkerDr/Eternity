#pragma once
#include <vector>
#include <fstream>
using namespace std;

struct Coordinate
{
	Coordinate()				{ x = y = 0; }
	Coordinate(int ix, int iy)	{ x = ix; y = iy; }
	int x;
	int y;
};

class Path
{
	// Members
private:
	std::string						_fileName;
	std::vector<Coordinate>			_path;
	int								_pathCounter;

	// Structors
public:
	Path();
	~Path();

	// Accessors
public:
	const Coordinate& getCoordinates(int i)	const	{ return _path[i]; }
	std::string getFileName()				const	{ return _fileName; } 
	int getPathCounter()					const	{ return _pathCounter; }
	int getPathSize()						const	{ return (int)_path.size(); }

	// Functions
public:
	void incPathCounter()							{ _pathCounter ++; }
	void decPathCounter()							{ _pathCounter --; }
	void defaultPathHorizontal(int width, int height);
	void defaultPathVertical(int width, int height)	;
	void loadPath(std::string fileName)				;
	void resetPath()								{ _pathCounter = 0; }

	// Gets current coordinates but does NOT increment the path counter - you must do this manually!
	Coordinate* nextCoordinates()					{ if (_pathCounter < getPathSize()) return &_path[_pathCounter]; return NULL; }
};
