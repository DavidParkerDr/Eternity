#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "Tile.h"
using namespace std;

class Dataset
{
	// Members
private:
	std::string			_fileName;
	int					_boardWidth;
	int					_boardHeight;
	std::vector<Tile*>	_tiles;
	std::vector<Tile*>	_corners;
	std::vector<Tile*>	_edges;
	std::vector<Tile*>	_middlePieces;
	std::vector<Tile*>	_fixedPieces;
	
	// Structors
public:
	Dataset(std::string fileName);
	~Dataset();

	// Accessors
public:
	int	getBoardHeight()		const	{ return _boardHeight; }
	int	getBoardWidth()			const	{ return _boardWidth; }
	Tile* getTile(int i)		const	{ return _tiles[i]; }
	Tile* getCorner(int i)		const	{ return _corners[i]; }
	Tile* getEdge(int i)		const	{ return _edges[i]; }
	Tile* getMiddle(int i)		const	{ return _middlePieces[i]; }
	Tile* getFixed(int i)		const	{ return _fixedPieces[i]; }
	std::string getFileName()	const	{ return _fileName; } 
	int numTiles()				const	{ return (int)_tiles.size(); }
	int numMiddlePieces()		const	{ return (int)_middlePieces.size(); }
	int numEdges()				const	{ return (int)_edges.size(); }
	int numCorners()			const	{ return (int)_corners.size(); }
	int numFixedPieces()		const	{ return (int)_fixedPieces.size(); }

	// Functions
public:
	void load(std::string fileName)		;
};
