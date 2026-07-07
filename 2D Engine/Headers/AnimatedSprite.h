#pragma once
#include "sprite.h"
#include <irrlicht.h>
#include "Engine.h"
#include "CircleTree.h"
#include "Maths.h"
using namespace irr;

class AnimatedSprite :
	public Sprite
{
	////////////
	// Members
protected:
	bool					_automatic;			// Whether to cycle automatically
	int						_currentTexture;	// For animated sprites
	int						_maxTextureTimer;	// For animated sprites
	TextureArray			_textureArray; // For animated sprites
	int						_textureTimer;	// For animated sprites

	////////////
	// Structors
public:
	AnimatedSprite(Engine& engine, Image* image, int xtiles, int ytiles, Vector offset=Vector(0,0,0));
	virtual ~AnimatedSprite();

	////////////
	// Accessors
public:
	bool getAutomatic()					const	{ return _automatic; }
	int getFrame()						const	{ return _currentTexture; }
	int numFrames()						const	{ return (int)_textureArray.size(); }

	////////////
	// Mutators
public:
	void setAutomatic(bool v)					{ _automatic = v; }
	void setFrame(int frame)					{ _currentTexture = frame; _material.setTexture(0, _textureArray[_currentTexture]); _material.MaterialType = video::EMT_TRANSPARENT_ALPHA_CHANNEL; }
	void setTextureTimer(int frames)			{ _maxTextureTimer = frames; _textureTimer = frames; }

	///////////////////////
	// Overridden Functions
public:
	virtual void render();
};
