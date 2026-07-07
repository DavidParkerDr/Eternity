#include ".\path.h"

/******************************************************************/

Path::Path()
: _pathCounter(0)
{
	_fileName = "";
}

/******************************************************************/

Path::~Path()
{
}

/******************************************************************/

void Path::defaultPathVertical(int width, int height)
{
	_path.clear();
	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			_path.push_back(Coordinate(x,y));
		}
	}
	_fileName = "VERTICAL";
}

/******************************************************************/

void Path::defaultPathHorizontal(int width, int height)
{
	_path.clear();
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			_path.push_back(Coordinate(x,y));
		}
	}
	_fileName = "HORIZONTAL";
}

/******************************************************************/

void Path::loadPath(std::string fileName)
{
	_path.clear();
	ifstream fin(fileName.c_str());
	if (fin)
	{
		_fileName = fileName;
		int x, y;
		fin >> x;
		while (!fin.eof())
		{
			fin >> y;
			_path.push_back(Coordinate(x,y));
			fin >> x;
		}
	}
}

/******************************************************************/
