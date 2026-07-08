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
	std::vector<std::string> candidates;
	candidates.push_back(fileName);
	if (fileName.find("paths/") == std::string::npos && fileName.find("paths\\") == std::string::npos)
	{
		candidates.push_back("paths/" + fileName);
		candidates.push_back("paths\\" + fileName);
	}
	candidates.push_back("files/" + fileName);
	candidates.push_back("files\\" + fileName);

	std::ifstream fin;
	std::string resolvedName = fileName;
	for (size_t i = 0; i < candidates.size(); ++i)
	{
		fin.open(candidates[i].c_str());
		if (fin)
		{
			resolvedName = candidates[i];
			break;
		}
		fin.clear();
	}
	if (fin)
	{
		_fileName = resolvedName;
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
