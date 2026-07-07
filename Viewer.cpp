#include ".\viewer.h"

/***************************************************************************************/

Viewer::Viewer(Board* b)
{
	_converter["G"] = 'A';
	_converter["6SOV"] = 'B';
	_converter["XCVY"] = 'C';
	_converter["6SYC"] = 'D';
	_converter["ALGP"] = 'E';
	_converter["ALIO"] = 'F';
	_converter["ALPY"] = 'G';
	_converter["BSCP"] = 'H';
	_converter["BSPY"] = 'I';
	_converter["BSYI"] = 'J';
	_converter["CCOC"] = 'K';
	_converter["CFIY"] = 'L';
	_converter["COGI"] = 'M';
	_converter["DSPC"] = 'N';
	_converter["FSBO"] = 'O';
	_converter["6SBY"] = 'P';
	_converter["PCCP"] = 'Q';
	_converter["PCGO"] = 'R';
	_converter["PCVC"] = 'S';
	_converter["SSIC"] = 'T';
	_converter["SSYG"] = 'U';
	_converter["XCBG"] = 'V';
	_converter["XCIP"] = 'W';

	_textures.push_back(map<char,Texture*>());
	_textures.push_back(map<char,Texture*>());
	_textures.push_back(map<char,Texture*>());
	_textures.push_back(map<char,Texture*>());

	if (b)
	{
		_board = b;
		_boardIsOwned = false;
	}
	else
	{
		_board = new Board(0,"Dataset_Eternity2.txt", "ViewerBoard", "");
		_boardIsOwned = true;
	}

	// Set up engine
	if (Engine::get()) Engine::destroy();
	Engine::init(_board->getWidth()*34,_board->getHeight()*34,32,false,false);
	_engine = Engine::get();
	wstringstream namer;
	namer << _board->getDataset().getFileName().c_str();
	_engine->setCaption(namer.str().c_str());

	// Load textures
	load();
}

/***************************************************************************************/

Viewer::~Viewer()
{
	if (_boardIsOwned) delete _board;
	_engine->destroy();
}

/***************************************************************************************/

void Viewer::load()
{
	for (map<string,char>::iterator i = _converter.begin(); i != _converter.end(); ++i)
	{
		for (int n = 1; n <= 4; n++)
		{
			stringstream fname;
			fname << "Textures/" << i->first << n << ".bmp";
			Texture* texture = _engine->loadTexture(fname.str(), irr::video::SColor(255,255,255,255));
			if (!texture)
			{
				cerr << "Could not load texture: " << fname.str() << endl;
			}
			else
			{
				_textures[n-1][i->second] = texture;
			}
		}
	}
}

/***************************************************************************************/

void Viewer::run()
{
	if (_engine->isRunning())
	{
		_engine->beginDrawing();
		for (int x = 0; x < _board->getWidth(); x++)
		{
			for (int y = 0; y < _board->getHeight(); y++)
			{
				int xpos = x * 34;
				int ypos = y * 34;
				Tile* p = _board->getTile(x,y);
				if (p)
				{
					char dirs[4];
					dirs[0] = p->north();
					dirs[1] = p->east();
					dirs[2] = p->south();
					dirs[3] = p->west();
					for (int n = 0; n < 4; n++)
					{
						Texture* tex = _textures[n][dirs[n]];
						if (tex)
						{
							_engine->drawImage(tex,xpos,ypos);
						}
						else
						{
							tex = _textures[n]['A'];
							_engine->drawImage(tex,xpos,ypos);
						}
					}
				}
			}
		}
		_engine->endDrawing();
	}
}

/***************************************************************************************/
