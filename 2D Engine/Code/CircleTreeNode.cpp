#include "..\headers\circletreenode.h"

/*******************************************************************************/

CircleTreeNode::~CircleTreeNode()
{
	for (int i = 0; i < numChildren(); i++)
	{
		CircleTreeNode* child = _children[i];
		delete child;
	}
	_children.clear();
}

/*******************************************************************************/

void CircleTreeNode::indent(ostream& out, int amt) 
{ 
	for (int i = 0; i < amt; i++) out << " "; 
}

/*******************************************************************************/

void CircleTreeNode::output(ostream& out, int ind)
{
	indent(out,ind);
	out << "CIRCLE " << _x << " " << _y << " " << _radius << " " << _level << endl;
	if (numChildren() > 0)
	{
		indent(out,ind);
		out << "{" << endl;
		for (int i = 0; i < numChildren(); i++)
		{
			CircleTreeNode* ctn = getChild(i);
			ctn->output(out,ind+3);
		}
		indent(out,ind);
		out << "}" << endl;
	}
}

/*******************************************************************************/

void CircleTreeNode::collectPoints(vector<Vector*>& v)
{
	if (numChildren())
	{
		for (int i = 0; i < numChildren(); i++) getChild(i)->collectPoints(v);
	}
	else
	{
		v.push_back(new Vector((float)_x,(float)_y,0));
	}
}

/*******************************************************************************/
