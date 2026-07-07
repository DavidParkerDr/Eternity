#pragma once
#include <irrlicht.h>
#include <string>
#include <map>
using namespace irr;

typedef irr::core::aabbox3df				Box;
typedef irr::video::SColor					Colour;
typedef irr::io::IFileList					FileList;
typedef irr::gui::IGUIFont					Font;
typedef irr::gui::IGUIEnvironment			GUI;
typedef irr::video::IImage					Image;
typedef irr::video::SMaterial				Material;
typedef irr::core::position2di				Position;
typedef irr::core::rect<s32>				Rectangle;
typedef irr::video::ITexture				Texture;
typedef irr::core::array<video::ITexture*>	TextureArray;
typedef irr::core::vector3df				Vector;
typedef irr::video::S3DVertex				Vertex;

class Sprite;

class Engine
{
	// Singleton
	static Engine*								_singleton;

	//////////
	// Members
private:
	scene::ICameraSceneNode*					_camera;
	float										_cameraHeight;
	Sprite*										_cameraLocked;
	std::string									_dir; // Main dir (of exe)
	IrrlichtDevice*								_device;
	video::IVideoDriver*						_driver;
	Font*										_font;
	std::map<std::string, Font*>				_fonts;
	gui::IGUIEnvironment*						_gui;
	std::map<std::string, video::IImage*>		_images;
	scene::ISceneManager*						_smgr;
	std::map<std::string, video::ITexture*>		_textures;

	////////////
	// Structors
public:
	static void init(int width=1280, 
					 int height=1024, 
					 int bits=32, 
					 bool fullScreen=true, 
					 bool vsync=true)	
		{ _singleton = new Engine(width, height, bits, fullScreen, vsync); }
	static Engine* get()									{ return _singleton; }
	static void destroy()									{ _singleton->predestruct(); delete _singleton; _singleton = NULL; }
	~Engine();
private:
	Engine(int width=1280, 
		   int height=1024, 
		   int bits=32, 
		   bool fullScreen=true, 
		   bool vsync=true)									;
	void predestruct()										;

	////////////
	// Accessors
public:
	scene::ICameraSceneNode* getCamera()			const	{ return _camera; }
	std::string getCurrentDirectory()				const	{ return _device->getFileSystem()->getWorkingDirectory().c_str(); }
	std::string getDir()							const	{ return _dir; }
	FileList* getFileList(std::string dir)					{ _device->getFileSystem()->changeWorkingDirectoryTo(dir.c_str()); FileList* fs = _device->getFileSystem()->createFileList(); resetDirectory(); return fs; }
	Font* getFont(std::string name)					const	{ std::map<std::string, Font*>::const_iterator i = _fonts.find(name); if (i != _fonts.end()) return i->second; return NULL; }
	int getFPS()									const	{ return _driver->getFPS(); }
	gui::IGUIEnvironment* getGUI()					const	{ return _gui; }
	Image* getImage(std::string name)				const	;
	scene::ISceneManager* getSceneManager()			const	{ return _smgr; }
	Texture* getTexture(std::string name)			const	;
	video::IVideoDriver* getVideoDriver()			const	{ return _driver; }
	bool isRunning()								const	{ return _device->run(); }
	bool isWindowActive()							const	{ return _device->isWindowActive(); }

	////////////
	// Mutators
public:
	void setCaption(std::wstring val)						{ _device->setWindowCaption(val.c_str()); }
	void setEventReceiver(IEventReceiver* er)				{ _device->setEventReceiver(er); }

	////////////
	// Functions
public:
	/// Begins drawing a scene; call "endDrawing" when done. Best to use "draw" instead however.
	void beginDrawing(bool clearBackBuffer=true,
					  bool clearZBuffer=true,
					  Colour clearColour=Colour(255,0,0,0))	{ _driver->beginScene(clearBackBuffer, clearZBuffer, clearColour); }

    /// Changes the font to a new one (must have been loaded first though!)
	void changeFont(std::string newFont)					{ _font = _fonts[newFont]; }

	/// Renders the scene to the screen - call every tick to display the graphics.
	void draw();

	/// Draws a circle thingy with a given amount of vertices
	void drawCircle(int cx, int cy, float rad, int vertices, Colour col=Colour(255,255,255,255));

	/// Draws an image to the screen at the given coords (top-left)
	void drawImage(Texture* img, int x, int y, Colour col=Colour(255,255,255,255), bool useAlpha=true);

	/// Draws a line
	void drawLine(int x1, int y1, int x2, int y2, Colour col=Colour(255,255,255,255));

	/// Draws a rectangle!
	void drawRectangle(int x1, int y1, int x2, int y2, Colour col=Colour(255,255,255,255))	;

	/// Draws some text on the screen
	void drawText(int x, int y,
				  Colour col, 
				  std::wstring text,
				  Rectangle& clip=Rectangle())				;

	/// Draws some text on the screen quickly
	void drawText(int x, int y, std::wstring text)			{ drawText(x,y, Colour(255,255,255,255), text); }

	/// Draws some text on the screen and centre it
	void drawTextCentre(int x, int y,
						Colour col, 
						std::wstring text,
						Rectangle& clip=Rectangle())		;


	/// Finishes drawing a scene; best to use "draw", however.
	void endDrawing()										{ _driver->endScene(); }

	/// Hides the mouse cursor				  
	void hideMouse()										{ _device->getCursorControl()->setVisible(false); }

	void loadFont(std::string fileName, std::string name)	{ Font* font = _gui->getFont(fileName.c_str()); _fonts[name] = font; }

	Image* loadImage(std::string fileName)					;

	/// Loads and stores a texture from a file
	Texture* loadTexture(std::string fileName)				;

	/// Loads and stores a texture with a given colour key - NOTE: This may not scale well under DirectX! Use alpha channel texture instead (or OGL)
	Texture* loadTexture(std::string fileName, Colour colourKey);

	/// Lock camera to a given Sprite (NULL unlocks camera)
	void lockCamera(Sprite* sprite, float distance)			;

	/// Gets the screen coordinates of the given position
	core::position2di screenCoordinatesOf(const Vector& pos)	{ return _smgr->getSceneCollisionManager()->getScreenCoordinatesFrom3DPosition(pos); }

	/// Sets up the camera - must call this at least once before using the camera!
	void setupCamera(const Vector& pos, const Vector& lookAt);

	/// Gets the sprite at the given screen coordinates
	Sprite* spriteAtScreenCoords(int x, int y)				;

	/// Resets the current working directory back to the exe dir
	void resetDirectory()									{ _device->getFileSystem()->changeWorkingDirectoryTo(_dir.c_str()); }

	/// Shows the mouse cursor
	void showMouse()										{ _device->getCursorControl()->setVisible(true); }

	/// Updates the camera position (if camera is locked)
	void updateCamera()										;
};

