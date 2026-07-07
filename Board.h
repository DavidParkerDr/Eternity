#pragma once
#include "Dataset.h"
#include "Path.h"
#include "Tile.h"
#include <list>

class Board
{
	// Members
private:
	Dataset					_dataset;
	Path					_path;
	int						_width;
	int						_height;
	Tile*					_tiles[16][16];
	int						_solutions;
	std::vector<Grout>		_index[24][24][24][24];
	int						_cornerStart;
	int						_cornerEnd;
	int						_currentCorner;
	int						_stopSignal;
	int						_threadID;
	std::string				_saveName;
	bool					_runSilent;
	int						_furthestPathID;

	// Structors
public:
	Board(int threadID, std::string datasetName, 
						std::string saveName, 
						std::string pathName);		// Normal constructor
	Board(int threadID, std::string saveName);		// Loading constructor
	~Board();

	// Accessors
public:
	int getWidth()					const	{ return _width; }
	int getHeight()					const	{ return _height; }
	const Path& getPath()			const	{ return _path; }
	const Dataset& getDataset()		const	{ return _dataset; }
	std::string getSaveName()		const	{ return _saveName; }
	Tile* getTile(int x, int y)		const	{ if (x >= 0 && x < _width && y >= 0 && y < _height) return _tiles[x][y]; return NULL; }
	int getThreadID()				const	{ return _threadID; }
	int numSolutions()				const	{ return _solutions; }
	bool isRunningSilent()			const	{ return _runSilent; }
	int getFurthestPathID()			const	{ return _furthestPathID; }

	// Mutators
public:
	void setRunSilent(bool rs)				{ _runSilent = rs; }
	void setTile(int x, int y, Tile* t)		{ if (t) { _tiles[x][y] = t; t->setPosition(x,y); t->setUsed(true); } }
	void resetTile(int x, int y)			{ if (_tiles[x][y]) { _tiles[x][y]->reset(); } _tiles[x][y] = NULL; }

	// Functions
public:
	void buildIndex()						;
	void loadDataset(std::string fileName)	{ _dataset.load(fileName); }
	void loadPath(std::string fileName)		{ _path.loadPath(fileName); }
	void loadState(std::string fileName)	;
	void output(std::string fileName)		;
	void outputGrid(std::string fileName)	;
	void resetBoard()						;
	void solve(int start, int end)			{ solve(start, end, start, false); }
	void solveFromLoad()					{ solve(_cornerStart, _cornerEnd, _currentCorner, true); }
	void solver(Coordinate* coord)			;

	// Thread control signals
	void stop()								{ _stopSignal = 1; }

private:
	// Main solve function: do not call this publicly, use one of the inline starts above
	void solve(int start, 
			   int end,
			   int current,
			   bool loaded=false)			;
};
