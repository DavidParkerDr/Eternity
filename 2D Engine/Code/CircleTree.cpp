#include "..\headers\circletree.h"

map<string, CircleTree*> CircleTree::_trees;

/*******************************************************************************/

CircleTree::~CircleTree()
{
	for (int i = 0; i < numPoints(); i++)
	{
		Vector* p = _points[i];
		delete p;
	}
	_points.clear();

	if (_top) delete _top;
}

/*******************************************************************************/

void CircleTree::load(char* filename)
{
	ifstream in(filename);
	if (!in) 
	{
		cerr << "Could not load circle tree file: " << filename << endl;
		return;
	}

	string data;
	int x,y,r,l;


	in >> data; // Filename
	cout << data << endl;
	in >> data >> data; // Width
	in >> data >> data; // Height
	in >> data >> data; // Max levels
	_top = NULL;
	vector<CircleTreeNode*> stack;
	CircleTreeNode* parent = NULL;
	CircleTreeNode* last   = NULL;

	while(!in.eof())
	{
		if (!_top)
		{
			in >> data;	// CIRCLE
			in >> x >> y >> r >> l; // Data
			_top = new CircleTreeNode(x,y,r,l);
			last = _top;
			parent = NULL;
		}
		else
		{
			in >> data; // data could be CIRCLE or { or }
			if (data.compare("{") == 0)
			{
				parent = last;
				stack.push_back(parent);
				in >> data >> x >> y >> r >> l; 
				last = new CircleTreeNode(x,y,r,l);
				parent->addChild(last);
			}
			else if (data.compare("}") == 0)
			{
				stack.pop_back();
				if ((int)stack.size() > 0) 
				{
					last = parent = stack.back();
				}
				else parent = last = NULL;
			}
			else
			{
				in >> x >> y >> r >> l; 
				last = new CircleTreeNode(x,y,r,l);
				if (parent) parent->addChild(last);
			}
		}
	}
	in.close();

	// Collect points
	_top->collectPoints(_points);
}

/*******************************************************************************/

void CircleTree::output(char* filename)
{
	ofstream out(filename);
	if (out)
	{
		_top->output(out);
	}
	out.flush(); out.close();
}

/*******************************************************************************/
