#include ".\board.h"
#include "Viewer.h"
#include <assert.h>

/******************************************************************/

Board::Board(int threadID, std::string datasetName, std::string saveName, std::string pathName)
: _dataset(datasetName), _solutions(0), _saveName(saveName), _stopSignal(0), _threadID(threadID),
  _runSilent(false), _furthestPathID(0)
{
	_height = _dataset.getBoardHeight();
	_width = _dataset.getBoardWidth();
	_cornerStart = _currentCorner = 0;
	_cornerEnd = 24;

	// NULL the board
	for (int x = 0; x < _width; x++)
		for (int y = 0; y < _height; y++)
			_tiles[x][y] = NULL;

	if (pathName == "" || pathName == "VERTICAL")
	{
		_path.defaultPathVertical(_dataset.getBoardWidth(), _dataset.getBoardHeight());
	}
	else if (pathName == "HORIZONTAL")
	{
		_path.defaultPathHorizontal(_dataset.getBoardWidth(), _dataset.getBoardHeight());
	}
	else
	{
		loadPath(pathName);
	}

	// Fix fixed pieces!
	for (int i = 0; i < _dataset.numFixedPieces(); i++)
	{
		Tile* fixed = _dataset.getFixed(i);
		setTile(fixed->getX(), fixed->getY(), fixed);
		fixed->setFixed(true);
	}

	buildIndex();
}

/******************************************************************/

// Loading constructor (ignore dataset, it gets overwritten)
Board::Board(int threadID, std::string saveName)
: _dataset("Dataset_eternity2.txt"), _solutions(0), _saveName(saveName), _stopSignal(0), _threadID(threadID),
  _runSilent(false), _furthestPathID(0)
{

	_height = _dataset.getBoardHeight();
	_width = _dataset.getBoardWidth();
	_cornerStart = _currentCorner = 0;
	_cornerEnd = 24;

	// NULL the board
	for (int x = 0; x < _width; x++)
		for (int y = 0; y < _height; y++)
			_tiles[x][y] = NULL;

	// THEN load
	loadState(saveName);

	// Fix fixed pieces!
	for (int i = 0; i < _dataset.numFixedPieces(); i++)
	{
		Tile* fixed = _dataset.getFixed(i);
		setTile(fixed->getX(), fixed->getY(), fixed);
		fixed->setFixed(true);
	}
}

/******************************************************************/

Board::~Board()
{
	stringstream fname;
	fname << "files/Destroyed" << _threadID << ".txt";
	output(fname.str());
}

/******************************************************************/

void Board::buildIndex()
{
	// Make index
	for (int i = 0; i < _dataset.numTiles(); i++)
	{
		Tile* t = _dataset.getTile(i);
		if (!t->isFixed())
		{
			for (int r = 0; r < 4; r++)
			{
				t->setRotation(r);
	
				// Add to index!
				_index[t->northID()][t->eastID()][t->southID()][t->westID()].push_back(Grout(t,r));
				_index[t->northID()][t->eastID()][t->southID()][0].push_back(Grout(t,r));
				_index[t->northID()][t->eastID()][0][t->westID()].push_back(Grout(t,r));
				_index[t->northID()][t->eastID()][0][0].push_back(Grout(t,r));
				_index[t->northID()][0][t->southID()][t->westID()].push_back(Grout(t,r));
				_index[t->northID()][0][t->southID()][0].push_back(Grout(t,r));
				_index[t->northID()][0][0][t->westID()].push_back(Grout(t,r));
				_index[t->northID()][0][0][0].push_back(Grout(t,r));
				_index[0][t->eastID()][t->southID()][t->westID()].push_back(Grout(t,r));
				_index[0][t->eastID()][t->southID()][0].push_back(Grout(t,r));
				_index[0][t->eastID()][0][t->westID()].push_back(Grout(t,r));
				_index[0][t->eastID()][0][0].push_back(Grout(t,r));
				_index[0][0][t->southID()][t->westID()].push_back(Grout(t,r));
				_index[0][0][t->southID()][0].push_back(Grout(t,r));
				_index[0][0][0][t->westID()].push_back(Grout(t,r));
				_index[0][0][0][0].push_back(Grout(t,r));
			}
			t->setRotation(0);
		}
	}
}

/******************************************************************/

void Board::output(std::string fileName)
{
	std::string fname = "files/" + fileName;
	ofstream fout(fname.c_str());
	if (fout)
	{
		fout << "DATASET: " << _dataset.getFileName() << endl;
		fout << "PATHFILE: " << _path.getFileName() << endl;
		fout << "START_CORNER_CONFIG: " << _cornerStart << endl;
		fout << "END_CORNER_CONFIG: " << _cornerEnd << endl;
		fout << "CURRENT_CORNER_CONFIG: " << _currentCorner << endl;
		fout << "FURTHEST: " << _furthestPathID << endl;
		fout << "SOLUTIONS: " << _solutions << endl;
		for (int y = 0; y < _height; y++)
		{
			for (int x = 0; x < _width; x++)
			{
				Tile* t = getTile(x,y);
				if (t)
				{
					fout << t->getID() << " " << t->getRotation() << " " << x << " " << y << " " << t->isFixed() << "\n";
				}
			}
		}
	}
	fout.close();

}

/******************************************************************/

void Board::outputGrid(std::string fileName)
{
	std::string fname = "files/" + fileName;
	ofstream fout(fname.c_str());
	if (fout)
	{
		for (int y = 0; y < _height; y++)
		{
			for (int x = 0; x < _width; x++)
			{
				Tile* t = getTile(x,y);
				if (t)
				{
					fout << t->getID() << "-" << t->getRotation() << "\t";
				}
				else
				{
					fout << "/-/\t";
				}
			}
			fout << endl;
		}
	}
	else
	{
		cerr << "Could not open file " << fileName << " for output." << endl;
	}
	fout.close();
}

/******************************************************************/

void Board::resetBoard()
{
	for (int x = 0; x < _width; x++)
	{
		for (int y = 0; y < _height; y++)
		{
			Tile* t = getTile(x,y);
			if (t && !t->isFixed())
			{
				resetTile(x,y);
			}
		}
	}
}

/******************************************************************/

void Board::loadState(std::string fileName)
{
	std::string fname = "files/" + fileName;
	ifstream fin(fname.c_str());
	if (fin)
	{
		string datasetName;
		string pathName;
		string junk;
		fin >> junk >> datasetName;
		fin >> junk >> pathName;

		loadDataset(datasetName);
		buildIndex();
		_width = _dataset.getBoardWidth();
		_height = _dataset.getBoardHeight();
		if (pathName == "HORIZONTAL")
		{
			_path.defaultPathHorizontal(_width, _height);
		}
		else if (pathName == "VERTICAL")
		{
			_path.defaultPathVertical(_dataset.getBoardWidth(), _dataset.getBoardHeight());
		}
		else
		{
			loadPath(pathName);
		}

		// Load config info
		int cornerConfig;
		int cornerStart;
		int cornerFinish;
		fin >> junk >> cornerStart;
		fin >> junk >> cornerFinish;
		fin >> junk >> cornerConfig;

		// Load furthest path
		fin >> junk;
		if (junk == "FURTHEST:")
		{
			fin >> _furthestPathID;
			
			// Load # solns
			fin >> junk >> _solutions;
		}
		else
		{
			_furthestPathID = 0;
			// Load # solns
			fin >> _solutions;
		}

		// Load saved pieces
		int tileID;
		int tileX;
		int tileY;
		int tileRot;
		bool tileFixed;
		fin >> tileID;
		while (!fin.eof())
		{
			fin >> tileRot >> tileX >> tileY >> tileFixed;
			Tile* t = _dataset.getTile(tileID-1);
			t->setRotation(tileRot);
			setTile(tileX, tileY, t);
			if (tileFixed) t->setFixed(true);
			fin >> tileID;
		}
		if (!_runSilent) cout << "* Thread " << _threadID << " - Loaded state " << fileName << " successfully!" << endl;
		_cornerStart = cornerStart;
		_cornerEnd = cornerFinish;
		_currentCorner = cornerConfig;
	}
	else
	{
		cerr << "* Thread " << _threadID << " - Could not load state: " << fileName << endl;
	}
}

/******************************************************************/

void Board::solve(int cornerStart, int cornerEnd, int current, bool loaded)
{
	// We only want to reset the first time if we are starting from scratch;
	// if we're loading, we want to leave the board intact.
	bool resetFirst = !loaded;

	// Setup corner configs
	vector<vector<int> > cornerConfigurations;
	for (int i = 0; i < 4; i ++)
	{
		for (int j = 0; j < 4; j++)
		{
			for (int k = 0; k < 4; k++)
			{
				for (int l = 0; l < 4; l++)
				{
					if (i != j && i != k && i != l && j != k && j != l && k != l)
					{
						vector<int> config;
						config.push_back(i);
						config.push_back(j);
						config.push_back(k);
						config.push_back(l);
						cornerConfigurations.push_back(config);
					}
				}
			}
		}
	}

	for (_currentCorner = max(cornerStart,current); _currentCorner < _cornerEnd; _currentCorner++)
	{
		// Reset board
		if (resetFirst) 
		{
			resetBoard();
			_path.resetPath();

			vector<int>& cornerConfig = cornerConfigurations[_currentCorner];
			setTile(0,			0,			_dataset.getCorner(cornerConfig[0]));	// Top left
			setTile(_width-1,   0,			_dataset.getCorner(cornerConfig[1]));	// Top right
			setTile(0,			_height-1,	_dataset.getCorner(cornerConfig[2]));	// Bottom left
			setTile(_width-1,	_height-1,	_dataset.getCorner(cornerConfig[3]));	// Bottom right

			// Rotate them into the correct position
			Tile* t = getTile(0,0);
			t->setFixed(true);
			while (t->northID() != 1 || t->westID() != 1) 
				t->incRotation();

			t = getTile(_width-1,0);
			t->setFixed(true);
			while (t->northID() != 1 || t->eastID() != 1) 
				t->incRotation();

			t = getTile(0,_height-1);
			t->setFixed(true);
			while (t->southID() != 1 || t->westID() != 1) 
				t->incRotation();

			t = getTile(_width-1,_height-1);
			t->setFixed(true);
			while (t->southID() != 1 || t->eastID() != 1) 
				t->incRotation();
		}
		else resetFirst = true;

		int solns = _solutions;
		if (!_runSilent) cout << "* Thread " << _threadID << " - Using corner configuration: " << _currentCorner << endl;

		// Solve using path
		Coordinate* coords = _path.nextCoordinates();	
		solver(coords);

		if (!_runSilent) cout << "* Thread " << _threadID << " - Total solutions found thus far: " << _solutions << endl;

		// Should we stop?
		if (_stopSignal != 0) return;
	}

	if (!_runSilent) cout << "* Thread " << _threadID << " - Final count: " << _solutions << " solutions" << endl;
}

/******************************************************************/

void Board::solver(Coordinate* coords)
{
	_path.incPathCounter();

	// Check stop signal
	if (_stopSignal != 0)
	{
		if (_stopSignal == 1)
		{
			// Save
			if (!_runSilent) output(_saveName);
			if (!_runSilent) outputGrid("Grid_" + _saveName);
			_stopSignal = 2;
			_path.decPathCounter();
			return;
		}
		else 
		{
			_path.decPathCounter();
			return;
		}
	}

	// Check finishing conditions
	if (coords == NULL) // Nothing left in the path
	{
		// We've finished the path! It's complete
		_solutions ++;
		if (!_runSilent) cout << "* Thread " << _threadID << " - Jubilation! Solution found: #" << _solutions << endl;
		if (_solutions == 1) 
		{
			stringstream solnName;
			solnName << "Solutions/" << _saveName << "-thread" << _threadID << "_solution";
			if (!_runSilent) output(solnName.str() + ".txt");
			if (!_runSilent) outputGrid(solnName.str() + "Grid.txt");
		}
		_path.decPathCounter();
		return;
	}

	// Check furthest path ID
	if (_path.getPathCounter() > _furthestPathID)
	{
		_furthestPathID = _path.getPathCounter();
		stringstream solnName;
		solnName << "Furthest/" << _saveName << "-thread" << _threadID << "_furthest";
		if (!_runSilent) output(solnName.str() + ".txt");
		if (!_runSilent) outputGrid(solnName.str() + "Grid.txt");
	}

	// Otherwise get int coordinates
	int x = coords->x;
	int y = coords->y;

	// Check for an existing tile at those coords
	Tile* existing = getTile(x, y);

	// If one exists, and it's a fixed one, recurse
	if (existing && existing->isFixed())
	{
		// Recurse immediately
		solver(_path.nextCoordinates());
		// But if we return from this, dec the counter
		_path.decPathCounter();
		return;
	}

	// If there is no existing tile, or if there is but it isn't fixed - we need to place one/check it

	// First get pattern constraints
	Tile* northTile	= getTile(x,y-1);
	Tile* eastTile	= getTile(x+1,y);
	Tile* southTile	= getTile(x,y+1);
	Tile* westTile	= getTile(x-1,y);
	//		 Does tile exist? Then get its pattern  : or set it to 0 (open/any pattern)
	int north	= northTile ? northTile->southID()	: 0;
	int east	= eastTile  ? eastTile->westID()	: 0;
	int south	= southTile ? southTile->northID()	: 0;
	int west	= westTile  ? westTile->eastID()	: 0;
	
	// But if the coords are at the edge, set to edge pattern (i.e. 1)
	int numEdges = 0;
	if (x == 0)		
	{
		west = 1;
		numEdges++;
	}
	else if (x == _width -1)
	{
		east = 1;
		numEdges++;
	}
	if (y == 0)	
	{
		north = 1;
		numEdges++;
	}
	else if (y == _height-1)
	{
		south = 1;
		numEdges++;
	}

	// Fetch the appropriate vector
	vector<Grout>& tiles = _index[north][east][south][west];

	// If it contains tiles:
	if (!tiles.empty())
	{
		// Loop through them
		for (int i = 0; i < (int)tiles.size(); i++)
		{
			Tile* tile = tiles[i].tile;

			// If we have an existing tile already (see earlier)...
			if (existing)
			{
				// ... then loop through until we reach it. This is for loading purposes and enables us to reach
				// the same point in the backtrack path.
				if (tile == existing)
				{
					solver(_path.nextCoordinates());
					// Remember to reset so we can backtrack from it
					resetTile(x,y);
					existing = NULL; // This is necessary to avoid a pernicious bug leading to gaps in the board
				}
			}
			// Otherwise, if the current tile isn't used:
			else if (!tile->isUsed()&& tile->numEdges()== numEdges)
			{
				// Set the rotation appropriately...
				tile->setRotation(tiles[i].rotation);
				// ... place it ...
				setTile(x,y,tile);
				// ... and recurse
				solver(_path.nextCoordinates());
				// Then reset it for backtracking
				resetTile(x,y);
			}
		}
	}
	// Decrement the path counter when we backtrack
	_path.decPathCounter();
}

/******************************************************************/
