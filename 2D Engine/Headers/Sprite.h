#pragma once
#include "Irrlicht.h"
#include "Engine.h"
#include "CircleTree.h"
#include "Maths.h"
using namespace irr;

class Sprite :
	public scene::ISceneNode
{
	////////////
	// Members
protected:
	bool					_animated;
	Box						_box;
	CircleTree*				_ctree;
	Engine&					_engine;
	int						_height;
	Material				_material;
	Vector					_offset;
	Vector					_scale;
	Texture*				_texture;
	TextureArray			_textureArray; // For animated sprites
	Vertex					_vertices[4];
	int						_width;

	////////////
	// Structors
public:
	Sprite(Engine& engine, Image* image, int xtiles, int ytiles, Vector offset=Vector(0,0,0));
	Sprite(Engine& engine, Texture* texture, Vector offset=Vector(0,0,0));
	Sprite(Engine& engine, Texture* texture, Sprite* parent, Vector relpos, Vector offset=Vector(0,0,0));
	virtual ~Sprite();
protected:
	Sprite(Engine& engine);
	void setup();

	////////////
	// Accessors
public:
	float getAbsoluteAngle()			const	{ return wrapAngle((360-this->getParent()->getRotation().Z) + (360-getRotation().Z)); }
	float getAngle()					const	{ return 360-wrapAngle(getRotation().Z); }
	CircleTree* getCircleTree()			const	{ return _ctree; }
	int	getHeight()						const	{ return _height; }
	const Vector& getOffset()			const	{ return _offset; }
	int getWidth()						const	{ return _width; }

	////////////
	// Mutators
public:
	void setAngle(float angle)					{ setRotation(Vector(0,0,360-wrapAngle(angle))); }
	void setAbsoluteAngle(float angle)			{ setAngle(-wrapAngle(360-this->getParent()->getRotation().Z) + angle); }
	void setCircleTree(CircleTree* tree)		{ _ctree = tree; }
	void setColour(Colour col)					{ for (int i = 0; i < 4; i++) _vertices[i].Color = col; }
	void setOffset(const Vector& offset)		{ _offset = offset; setup(); }
	void setPos(float x, float y, float z=0)	{ setPosition(Vector(x,y,z)); }

	////////////
	// Functions
public:
	float absoluteAngleTo(const Vector& pos)	;
	float absoluteAngleTo(float x, float y)		{ return absoluteAngleTo(Vector(x,y,0)); }
	float absoluteAngleTo(Sprite* other)		{ return absoluteAngleTo(other->getPosition()); }
	bool collidePoint(float x, float y)			;
	bool collidePoint(const Vector& pos)		{ return collidePoint(pos.X, pos.Y); }
	void hide()									{ setVisible(false); }
	void loadCircleTree(char* name)				;
	void move(float amt)						{ Vector newpos(msin(getAngle()), mcos(getAngle()), 0); setPosition(getPosition() + newpos * amt); }
	void pointAt(Vector pos)					{ setAngle(absoluteAngleTo(pos)); }
	void pointAt(Sprite* target)				{ setAngle(absoluteAngleTo(target)); }
	void pointAt(float x, float y)				{ setAngle(absoluteAngleTo(x,y)); }
	float relativeAngleTo(const Vector& pos)	;
	float relativeAngleTo(float x, float y)		{ return relativeAngleTo(Vector(x,y,0)); }
	float relativeAngleTo(Sprite* other)		{ return relativeAngleTo(other->getPosition()); }
	Vector relativePosition(const Vector& pos)	;
	Vector relativePosition(float x, float y)	{ return relativePosition(Vector(x,y,0)); }
	Vector relativePosition(Sprite* other)		{ return relativePosition(other->getPosition()); }
	void scale(float fx, float fy)				{ this->setScale(Vector(fx,fy,1)); }
	void scale(float scale)						{ this->setScale(Vector(scale,scale,1)); }
	void scaleToSize(float size)				{ float height = (float)getHeight(); float factor = size / height; scale(factor); }
	void show()									{ setVisible(true); }
	void turnTowards(float angle, float inc)	;
	void turnTowards(const Vector& p, float i)  { turnTowards(absoluteAngleTo(p),i); }
	void turnTowards(float x, float y, float i)	{ turnTowards(absoluteAngleTo(x,y),i); }
	void turnTowards(Sprite* target, float i)	{ turnTowards(absoluteAngleTo(target),i); }
	void turnLeft(float angle)					{ setAngle(getAngle() - angle); }
	void turnRight(float angle)					{ setAngle(getAngle() + angle); }
	Vertex& vertex(int i)						{ return _vertices[i]; }

private:
	float wrapAngle(float angle)		const	{ if (angle > 360) angle -= 360; if (angle < 0) angle += 360; return angle; }

	///////////////////////
	// Overridden Functions
public:
	virtual void OnRegisterSceneNode();
	virtual void render();
	virtual const Box& getBoundingBox() const;
	virtual u32 getMaterialCount();
	virtual Material& getMaterial(u32 i);

};
