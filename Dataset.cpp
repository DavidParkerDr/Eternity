#include ".\dataset.h"

/******************************************************************/

Dataset::Dataset(std::string fileName)
: _fileName("NONE")
{
	load(fileName);
}

/******************************************************************/

Dataset::~Dataset()
{
}

/******************************************************************/

void Dataset::load(std::string fileName)
{
	_tiles.clear();
	ifstream fin(fileName.c_str());
	if (fin)
	{
		_fileName = fileName; 
		fin >> _boardWidth >> _boardHeight;
		for (int i = 0; i < _boardWidth * _boardHeight; i++)
		{
			int id = 0;
			char n, e, s, w;
			fin >> id;
			fin >> n >> e >> s >> w;
			Tile* t = new Tile(id, n, e, s, w);
			_tiles.push_back(t);

			if (t->isCorner()) _corners.push_back(t);
			else if (t->isEdge()) _edges.push_back(t);
			else _middlePieces.push_back(t);
		}
		int fixedID = 0;
		int fixedX = 0;
		int fixedY = 0;
		int fixedRot = 0;
		fin >> fixedID;
		while (!fin.eof())
		{
			fin >> fixedRot >> fixedX >> fixedY;
			Tile* fixed = getTile(fixedID-1);
			fixed->setPosition(fixedX, fixedY);
			fixed->setRotation(fixedRot);
			fixed->setFixed(true);
			_fixedPieces.push_back(fixed);
			fin >> fixedID;
		}
	}
	else
	{
		cout << "Cannot find file: " << fileName << ". Dataset not loaded." << endl;
	}
}

/******************************************************************/
