#include "..\headers\sprite.h"
#include "..\headers\Engine.h"
using namespace core;
using namespace scene;
using namespace video;

/*******************************************************************************/

Sprite::Sprite(Engine& engine)
:	ISceneNode(engine.getSceneManager()->getRootSceneNode(), 
			   engine.getSceneManager(), 
			   0), 
	_engine(engine)
{
}

/*******************************************************************************/

Sprite::Sprite(Engine& engine, Texture* texture, Vector offset)
:	ISceneNode(engine.getSceneManager()->getRootSceneNode(), 
			   engine.getSceneManager(), 
			   0), 
	_engine(engine),
	_offset(offset),
	_texture(texture)
{
	setup();
}

/*******************************************************************************/

Sprite::Sprite(Engine& engine, Texture* texture, Sprite* parent, Vector relpos, Vector offset)
:	ISceneNode(parent, 
			   engine.getSceneManager(), 
			   0), 
	_engine(engine),
	_offset(offset),
	_texture(texture)
{
	setup();
	setPosition(relpos);
}

/*******************************************************************************/

void Sprite::setup()
{
	_material.Wireframe = false;
	_material.Lighting = false;

	_width = _texture->getSize().Width;
	_height = _texture->getSize().Height;

	float halfWidth = _width / 2.0f;
	float halfHeight = _height / 2.0f;
	float x1 = _offset.X - halfWidth;
	float x2 = _offset.X + halfWidth;
	float y1 = _offset.Y - halfHeight;
	float y2 = _offset.Y + halfHeight;

	_vertices[0] = Vertex(x1, y1, 0, 0,0,1, Colour(255,255,255,255), 0, 1);
	_vertices[1] = Vertex(x2, y1, 0, 0,0,1, Colour(255,255,255,255), 1, 1);
	_vertices[2] = Vertex(x2, y2, 0, 0,0,1, Colour(255,255,255,255), 1, 0);
	_vertices[3] = Vertex(x1, y2, 0, 0,0,1, Colour(255,255,255,255), 0, 0);

	_box.reset(_offset);
	_box.MinEdge = Vector(x1, y1, -0.1f);
	_box.MaxEdge = Vector(x2, y2, 0.1f);

	_material.setTexture(0, _texture);
	_material.MaterialType = EMT_TRANSPARENT_ALPHA_CHANNEL_REF;

//	setMaterialTexture(0, _texture);
//	setMaterialType(EMT_TRANSPARENT_ALPHA_CHANNEL_REF);

	_scale = Vector(1,1,0);
	_ctree = NULL;
	_animated = false;
}

/*******************************************************************************/

Sprite::~Sprite(void)
{
	remove();
}

/*******************************************************************************/
// Functions
/*******************************************************************************/

float Sprite::absoluteAngleTo(const Vector& otherPos)
{
	Vector diff = otherPos - this->getAbsolutePosition();
	
	// Straight up or down
	if (diff.X == 0)
	{
		if (diff.Y < 0) // We're below
		{
			return 180;
		}
		else // We're above
		{
			return 0;
		}
	}
	// Otherwise
	else
	{
		double angle = atan(diff.Y/diff.X) * 180 / irr::core::PI;
		if (diff.X > 0)
		{
			angle = 90 - angle;
			//if (diff.Y > 0)
			//{
			//	// 90 - 180
			//	angle = 90 - angle;
			//}
			//else // diff.Y < 0
			//{
			//	// 0 - 90
			//	angle = 90 - angle;
			//}
		}
		else // diff.X < 0
		{
			angle = 270 - angle;
		//	if (diff.Y > 0)
		//	{
		//		// 180 - 270
		//		angle = 270 - angle;
		//	}
		//	else // diff.Y < 0
		//	{
		//		// 270 - 360
		//		angle = 270 - angle;
		//	}
		}
		return wrapAngle((float)angle);
	}
}

/*******************************************************************************/

bool Sprite::collidePoint(float x, float y)
{
	if (!_ctree) return false;

	// Get relative position
    Vector relPos = relativePosition(x,y);

	return _ctree->collidePointScaled(relPos.X, relPos.Y, _scale.X);
}

/*******************************************************************************/

void Sprite::loadCircleTree(char* name)
{
	_ctree = CircleTree::loadCircleTree(name);
}

/*******************************************************************************/

float Sprite::relativeAngleTo(const Vector& otherPos)
{
	float angle = absoluteAngleTo(otherPos);
	float myAngle = getAngle();
	float diff = angle - myAngle;
	return wrapAngle(diff);
}

/*******************************************************************************/

Vector Sprite::relativePosition(const Vector& pos)
{
	// Move point to relative position (rotate around us)
	float relAngle = (float)toRad(relativeAngleTo(pos));
	float dist = getPosition().getDistanceFrom(pos);
	float relX = dist * sin(relAngle) + (_width/2);
	float relY = dist * -cos(relAngle) + (_height/2);
	return Vector(relX, relY, 0);
}

/*******************************************************************************/

void Sprite::turnTowards(float angle, float inc)
{
	float currentAngle = getAngle();
	float diff = angle - currentAngle;

	diff = wrapAngle(diff);

	// Turn by increment
	if (diff <= inc || diff >= (360-inc))
	{
		setAngle(angle);
	}
	else
	{
		if (diff <= 180)
		{
			turnRight(inc);
		}
		else
		{
			turnLeft(inc);
		}
	}
}

/*******************************************************************************/
// Overridden Functions
/*******************************************************************************/

void Sprite::OnRegisterSceneNode()
{
	if (IsVisible)
		SceneManager->registerNodeForRendering(this);

	ISceneNode::OnRegisterSceneNode();
}

/*******************************************************************************/

void Sprite::render()
{
	u16 indices[] = {	0,2,1, 0,3,2	};
	video::IVideoDriver* driver = SceneManager->getVideoDriver();

	driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);
	driver->setMaterial(_material);
	driver->drawIndexedTriangleList(_vertices, 4, indices, 2);
}

/*******************************************************************************/

const Box& Sprite::getBoundingBox() const
{
	return _box;
}

/*******************************************************************************/

u32 Sprite::getMaterialCount()
{
	return 1;
}

/*******************************************************************************/

Material& Sprite::getMaterial(u32 i)
{
	return _material;
}	

/*******************************************************************************/
