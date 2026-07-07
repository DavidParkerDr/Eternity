#pragma once
//#pragma warning( disable : 4244)
#include "Engine.h"
#include "Maths.h"
#include <vector>
#include <fstream>
using namespace std;

// Represents a node in a circle tree. Note that theoretically we can use one circle tree
// to represent all sprites of the same type.
class CircleTreeNode
{
private:
	int _x;
	int _y;
	int _radius;
	int _radius2;
	int _level;
	vector<CircleTreeNode*> _children;

	////////////
	// Structors
public:
	CircleTreeNode(int x, int y, int r, int l) { _x = x-1; _y = y; _radius = r; _level = l; _radius2 = r*r +1; }
	~CircleTreeNode();

	////////////
	// Accessors
public:
	CircleTreeNode* getChild(int index)		{ return _children.at(index); }
	int getRadius() const					{ return _radius; }
	int getRadius2() const					{ return _radius2; }
	int getLevel() const					{ return _level; }
	int getX() const						{ return _x; }
	int getY() const						{ return _y; }
	int numChildren() const					{ return (int)_children.size(); }

	////////////
	// Mutators
public:
	void addChild(CircleTreeNode* ctn)		{ _children.push_back(ctn); }

	////////////
	// Functions
public:
	// Fills a the list parameter with all the centre points of all nodes
	void collectPoints(vector<Vector*>& v);

	// Checks to see whether a given point is in the circle tree
	bool collidePoint(float x, float y);

	// As above, but scaled to fit
	bool collidePointScaled(float x, float y, float scale);

	// Unused. Meant to collide two circle trees against each other. Replaced by checking
	// the list of points using collidePoint.
	bool collideCircle(CircleTreeNode* ctn, float x, float y);

	// Outputs this circle tree node to an ostream
	void output(ostream& out, int ind=0);
	
	// Indents; called as part of output
	void indent(ostream& out, int amt);

	// Draws this circle tree node on screen given location data
//	void draw(int x, int y, int offX, int offY, float angle);
};

/*******************************************************************************/

inline bool CircleTreeNode::collidePoint(float x, float y)
{
	float d2 = dist2(x,y,(float)_x,(float)_y); 
	if (d2 <= _radius2)
	{
		if (numChildren() > 0)
		{
			for (int i = 0; i < numChildren(); i++)
			{
				CircleTreeNode* child = getChild(i);
				if (child->collidePoint(x,y)) return true;
			}
		}
		else return true;
	}
	return false;
}

/*******************************************************************************/

inline bool CircleTreeNode::collidePointScaled(float x, float y, float scale)
{
	float d2 = dist2(x,y,(float)_x,(float)_y); 
	if (d2 <= _radius2 * scale)
	{
		if (numChildren() > 0)
		{
			for (int i = 0; i < numChildren(); i++)
			{
				CircleTreeNode* child = getChild(i);
				if (child->collidePointScaled(x,y,scale)) return true;
			}
		}
		else return true;
	}
	return false;
}

/*******************************************************************************/

//inline void CircleTreeNode::draw(int x, int y, int offX, int offY, float angle)
//{
//	if (numChildren() == 0)
//	{
//		Vector v(offX-_x,_y-offY);
//		Matrix m;
//		m.rotationMatrix(angle);
//		m.multiply(v);
//		int nx = v.x + x;
//		int ny = v.y + y;
//		dbCircle(nx,ny,_radius);
//	}
//	else
//	{
//		for (int i = 0; i < numChildren(); i++)
//		{
//			CircleTreeNode* child = getChild(i);
//			child->draw(x,y, offX, offY, angle);
//		}
//	}
//}

/*******************************************************************************/

inline bool CircleTreeNode::collideCircle(CircleTreeNode* ctn, float x, float y)
{ 
	int r = _radius2 + ctn->getRadius2(); 
	int ox = ctn->getX() + (int)x - getX();
	int oy = ctn->getY() + (int)y - getY();
	return (dist2((float)ox,(float)oy,0,0) <= r);
}

/*******************************************************************************/
