#ifndef _GGROUP_H_
#define _GGROUP_H_
#include <vector>
#include <set>
#include <stack>
#include"GPoint.h"
#include"Unitgroup.h"
class GGroup
{
public:
	vector<GPoint*> pointStack;
	set<GPoint*> pointSet;
	int tail;//tail set
	set<GPoint*> ChildSet;
	int maxLayer;

public:
	void CalculateCS();
	bool VerifyPoint(GPoint* p);
};
#endif