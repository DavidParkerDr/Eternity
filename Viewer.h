#pragma once
#include "Irrlicht.h"
#include "Engine.h"
#include <map>
#include <vector>
#include <string>
#include <sstream>
#include "Board.h"
using namespace std;

class Viewer
{
	//////////
	// Members
private:
	Board*							_board;
	bool							_boardIsOwned;
	map<string,char>				_converter;
	Engine*							_engine;
	vector<map<char,Texture*> >		_textures;

	////////////
	// Structors
public:
	Viewer(Board* board=NULL)		;
	~Viewer()						;

	////////////
	// Accessors
public:
	Board* getBoard()		const	{ return _board; }

	////////////
	// Mutators
public:
	void setBoard(Board* b)			{ _board = b; }

	////////////
	// Functions
public:
	void load()						;
	void run()						;
};
