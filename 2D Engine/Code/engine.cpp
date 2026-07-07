#include "..\headers\engine.h"
#include "..\headers\sprite.h"
using namespace video;
using namespace core;
using namespace scene;
using namespace std;

Engine* Engine::_singleton = NULL;

/*******************************************************************************/

Engine::Engine(int width, int height, int bits, bool fullScreen, bool vsync)
{
	_device = NULL;
	_driver = NULL;
	_smgr	= NULL;
	_gui	= NULL;
	_camera = NULL;
	_cameraLocked = NULL;

	// Attempt to load Irrlicht
	video::E_DRIVER_TYPE driverType = video::EDT_SOFTWARE;//EDT_OPENGL;
	_device = createDevice(driverType, core::dimension2d<s32>(width, height), bits, fullScreen, false, vsync);

	if (_device == NULL)
	{
		throw exception("Could not initialise OpenGL!");
	}

	_dir		= _device->getFileSystem()->getWorkingDirectory();
	_driver		= _device->getVideoDriver();
	_smgr		= _device->getSceneManager();
	_gui		= _device->getGUIEnvironment();

	_driver->setTextureCreationFlag(video::ETCF_ALWAYS_32_BIT, true);

//	_soundEngine = irrklang::createIrrKlangDevice();

	_font = _gui->getBuiltInFont();
}

/*******************************************************************************/

Engine::~Engine(void)
{
	if (_device) _device->drop();
}

/*******************************************************************************/

void Engine::predestruct()
{
	for (std::map<std::string,Image*>::iterator i = _images.begin(); i != _images.end(); ++i)
	{
		Image* img = i->second;
		img->drop();
	}
//	_soundEngine->drop();
	_soundEngine = NULL;
	_device->drop();
	_device = NULL;
}

/*******************************************************************************/
// Accessors
/*******************************************************************************/

Image* Engine::getImage(std::string name) const
{
	std::map<std::string, Image*>::const_iterator i = _images.find(name);
	if (i != _images.end())
	{
		return i->second;
	}
	else
	{
		return NULL;
	}
}

/*******************************************************************************/

ITexture* Engine::getTexture(std::string name) const
{
	std::map<std::string, ITexture*>::const_iterator i = _textures.find(name);
	if (i != _textures.end())
	{
		return i->second;
	}
	else
	{
		return NULL;
	}
}

/*******************************************************************************/
// Functions
/*******************************************************************************/

void Engine::draw()
{
	if (isWindowActive())
	{
		_driver->beginScene(true, true, 0);

		_smgr->drawAll();
		_gui->drawAll();

		_driver->endScene();
	}
}

/*******************************************************************************/

void Engine::drawCircle(int cx, int cy, float rad, int vertices, Colour col)
{
	_driver->draw2DPolygon(Position(cx,cy), rad, col, vertices);
}

/*******************************************************************************/

void Engine::drawImage(Texture* img, int x, int y, Colour col, bool useAlpha)
{
	rect<s32> sourceRect(0,0,img->getSize().Width,img->getSize().Height);
	_driver->draw2DImage(img, position2d<s32>(x,y), sourceRect, NULL, col, useAlpha);
}

/*******************************************************************************/

void Engine::drawLine(int x1, int y1, int x2, int y2, Colour col)
{
	_driver->draw2DLine(Position(x1,y1), Position(x2,y2), col);
}

/*******************************************************************************/

void Engine::drawRectangle(int x1, int y1, int x2, int y2, Colour col)
{
	_driver->draw2DLine(Position(x1,y1), Position(x2,y1), col);
	_driver->draw2DLine(Position(x2,y1), Position(x2,y2), col);
	_driver->draw2DLine(Position(x2,y2), Position(x1,y2), col);
	_driver->draw2DLine(Position(x1,y2), Position(x1,y1), col);
}

/*******************************************************************************/

void Engine::drawText(int x, int y, Colour col, std::wstring text, Rectangle& clip)
{
	if (clip.getHeight() > 0)
		_font->draw(text.c_str(),Rectangle(x,y,x,y),col,false,false,&clip);
	else
		_font->draw(text.c_str(),Rectangle(x,y,x,y),col);
}

/*******************************************************************************/

void Engine::drawTextCentre(int x, int y, Colour col, std::wstring text, Rectangle& clip)
{
	if (clip.getHeight() > 0)
		_font->draw(text.c_str(),Rectangle(x,y,x,y),col,true,true,&clip);
	else
		_font->draw(text.c_str(),Rectangle(x,y,x,y),col,true,true);
}

/*******************************************************************************/

Image* Engine::loadImage(std::string fileName)
{
	std::map<std::string, Image*>::const_iterator i = _images.find(fileName);
	if (i != _images.end())
	{
		return i->second;
	}
	else
	{
		Image* image = _driver->createImageFromFile(fileName.c_str());
		_images[fileName] = image;
		return image;
	}
}

/*******************************************************************************/

Sound* Engine::loadSound(std::string fileName)					
{
	std::map<std::string, Sound*>::const_iterator i = _sounds.find(fileName);
	if (i != _sounds.end())
	{
		return i->second;
	}
	else
	{
		Sound* sound = _soundEngine->addSoundSource(fileName.c_str());
		_sounds[fileName] = sound;
		return sound;
	}	
}

/*******************************************************************************/

ITexture* Engine::loadTexture(std::string fileName)
{
	std::map<std::string, Texture*>::const_iterator i = _textures.find(fileName);
	if (i != _textures.end())
	{
		return i->second;
	}
	else
	{
		ITexture* texture = _driver->getTexture(fileName.c_str());
		_textures[fileName] = texture;
		return texture;
	}
}

/*******************************************************************************/

ITexture* Engine::loadTexture(std::string fileName, Colour colourKey)
{
	std::map<std::string, ITexture*>::const_iterator i = _textures.find(fileName);
	if (i != _textures.end())
	{
		return i->second;
	}
	else
	{
		ITexture* texture = _driver->getTexture(fileName.c_str());
		_driver->makeColorKeyTexture(texture, colourKey);
		texture->regenerateMipMapLevels();
		_textures[fileName] = texture;
		return texture;
	}
}

/*******************************************************************************/

void Engine::lockCamera(Sprite* sprite, float distance)
{
	distance = abs(distance);
	_cameraHeight = distance;
	if (_camera)
	{
		_cameraLocked = sprite;
		if (sprite)
		{
			_camera->setPosition(_cameraLocked->getPosition()+Vector(0,0,-_cameraHeight));
			_camera->setTarget(_cameraLocked->getPosition());
		}
		else
		{
			_camera->setPosition(Vector(0,0,-_driver->getScreenSize().Width/2.0f));
			_camera->setTarget(Vector(0,0,0));
		}
	}
}

/*******************************************************************************/

void Engine::setupCamera(const Vector& position, const Vector& lookAt)
{
	// USE MINUS Z-axis position otherwise everything will be mirrored L-R
	_camera = _smgr->addCameraSceneNode(NULL, position, lookAt);
	_camera->setUpVector(Vector(0,1,0));
}

/*******************************************************************************/

Sprite* Engine::spriteAtScreenCoords(int x, int y)
{
	ISceneNode* node = _smgr->getSceneCollisionManager()->getSceneNodeFromScreenCoordinatesBB(position2d<s32>(x,y));
	if (node)
	{
		return (Sprite*)node;
	}
	return NULL;
}

/*******************************************************************************/

void Engine::positionListener(const Vector& pos, const Vector& lookAt)
{
	if (lookAt.getLengthSQ() == 0)
	{
		_soundEngine->setListenerPosition(pos, Vector(pos.X, pos.Y, 0), Vector(0,0,-1));
	}
	else
	{
		_soundEngine->setListenerPosition(pos, lookAt, Vector(0,0,-1));
	}
}

/*******************************************************************************/

void Engine::updateCamera()
{
	if (_cameraLocked)
	{
		_camera->setPosition(_cameraLocked->getPosition()+Vector(0,0,-_cameraHeight));
		_camera->setTarget(_cameraLocked->getPosition());
	}
}

/*******************************************************************************/
