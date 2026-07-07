#include "..\headers\animatedsprite.h"
using namespace core;
using namespace scene;
using namespace video;

/*******************************************************************************/

// Animated sprite
AnimatedSprite::AnimatedSprite(Engine& engine, Image* image, int xtiles, int ytiles, Vector offset)
:	Sprite(engine)
{
	_offset = offset;
	_material.Wireframe = false;
	_material.Lighting = false;

	_width = image->getDimension().Width / xtiles;
	_height = image->getDimension().Height / ytiles;

	float halfWidth = _width / 2.0f;
	float halfHeight = _height / 2.0f;
	float x1 = offset.X - halfWidth;
	float x2 = offset.X + halfWidth;
	float y1 = offset.Y - halfHeight;
	float y2 = offset.Y + halfHeight;

	_vertices[0] = Vertex(x1, y1, 0, 0,0,1, Colour(255,255,255,255), 0, 1);
	_vertices[1] = Vertex(x2, y1, 0, 0,0,1, Colour(255,255,255,255), 1, 1);
	_vertices[2] = Vertex(x2, y2, 0, 0,0,1, Colour(255,255,255,255), 1, 0);
	_vertices[3] = Vertex(x1, y2, 0, 0,0,1, Colour(255,255,255,255), 0, 0);

	_box.reset(_offset);
	_box.MinEdge = Vector(x1, y1, -0.1f);
	_box.MaxEdge = Vector(x2, y2, 0.1f);

	_scale = Vector(1,1,0);
	_ctree = NULL;

	// First cut up the image into pieces, convert them into textures, and store them in an array
	core::array<IImage*> images;
	dimension2di tsize(_width, _height);
	for (int y = 0; y < ytiles; y++)
	{
		for (int x = 0; x < xtiles; x++)
		{
			IImage* newImage = image->createImageCopy(position2di(x*_width,y*_height),tsize);
			images.push_back(newImage);
		}
	}
	// Make Textures
	for (int i = 0; i < xtiles*ytiles; i++)
	{
		IImage* img = images[i];
		ITexture* text = _engine.getVideoDriver()->addTexture("AnimatedSpriteImage",img);
		_textureArray.push_back(text);
	}
	// Delete images
	for (int i = 0; i < xtiles*ytiles; i++)
	{
		IImage* img = images[i];
		img->drop();
	}

	// Changed to make compatible with irrlicht 1.4
	_material.setTexture(0, _textureArray[0]);
	_material.MaterialType = EMT_TRANSPARENT_ALPHA_CHANNEL_REF;

//	setMaterialTexture(0, _textureArray[0]);
//	setMaterialType(EMT_TRANSPARENT_ALPHA_CHANNEL_REF);

	_currentTexture = 0;
	_maxTextureTimer = 60;
	_textureTimer = 60;

	_animated = true;
	_automatic = true;
}

/*******************************************************************************/

AnimatedSprite::~AnimatedSprite(void)
{
}

/*******************************************************************************/
// Overridden Functions
/*******************************************************************************/

void AnimatedSprite::render()
{
	// Change texture if animated
	if (_automatic)
	{
		_textureTimer --;
		if (_textureTimer <= 0)
		{
			_textureTimer = _maxTextureTimer;
			_currentTexture ++;
			if (_currentTexture >= (int)_textureArray.size())
			{
				_currentTexture = 0;
			}
			_material.setTexture(0, _textureArray[_currentTexture]);
//			setMaterialTexture(0, _textureArray[_currentTexture]);
//			setMaterialType(EMT_TRANSPARENT_ALPHA_CHANNEL_REF);
		}
	}

	u16 indices[] = {	0,2,1, 0,3,2	};
	video::IVideoDriver* driver = SceneManager->getVideoDriver();

	driver->setMaterial(_material);
	driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);
	driver->drawIndexedTriangleList(_vertices, 4, indices, 2);
}

/*******************************************************************************/