#pragma once
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include "CircleTreeNode.h"
using namespace std;

// The wrapper class for a tree of circle nodes. In theory we need one of these per type
// of sprite we're going to collide with. 
class CircleTree
{
protected:
	CircleTreeNode*							_top;
	vector<Vector*>							_points;
	static map<string, CircleTree*>			_trees;

	////////////
	// Structors
public:
	static CircleTree* loadCircleTree(char* filename)
	{
		string nm(filename);
		map<std::string, CircleTree*>::iterator i = _trees.find(nm);
		if (i != _trees.end())
		{
			return i->second;
		}
		else
		{
			CircleTree* ctree = new CircleTree(filename);
			_trees[nm] = ctree;
			return ctree;
		}
	}
	~CircleTree();
protected:
	CircleTree(char* filename)				{ _top = NULL; load(filename); }
	
	////////////
	// Accessors
public:
	Vector getPoint(int i)			const	{ return *_points.at(i); }
	CircleTreeNode* getTop()		const	{ return _top; }
	int numPoints()					const	{ return (int)_points.size(); }

	////////////
	// Functions
public:
	void output(char* filename);
	bool collideCircle(CircleTree* ct, float x, float y);
	bool collidePoint(float x, float y);
	bool collidePointScaled(float x, float y, float scale);
//	void draw(int x, int y, int offX, int offY, float angle);
protected:
	void load(char* filename);
};

/*******************************************************************************/

inline bool CircleTree::collideCircle(CircleTree* ct, float x, float y)
{ 
	if (_top && ct->getTop()) 
		return _top->collideCircle(ct->getTop(), x, y); 
	else 
		return false; 
}

/*******************************************************************************/

inline bool CircleTree::collidePoint(float x, float y)
{
	if (!_top) return false; 
	else return _top->collidePoint(x,y);
}

/*******************************************************************************/

inline bool CircleTree::collidePointScaled(float x, float y, float scale)
{
	if (!_top) return false; 
	else return _top->collidePointScaled(x,y,scale);
}

/*******************************************************************************/

//inline void CircleTree::draw(int x, int y, int offX, int offY, float angle) 
//{ 
	//if (_top) _top->draw(x, y, offX, offY, angle); 
//}

/*******************************************************************************/
