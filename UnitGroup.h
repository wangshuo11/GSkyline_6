#ifndef _UNITGROUP_H_
#define _UNITGROUP_H_
#include<set>
#include"GPoint.h"

struct classcomp
{
	bool operator()(GPoint* a, GPoint* b) {
		return a->id < b->id;
	}
};

class UnitGroup {
public:
	set<GPoint*> unitSet;
	set<GPoint*> allParentSet;
	set<GPoint*, classcomp> merge;
	int tail;
	int size;
public:
	UnitGroup();
	UnitGroup(const UnitGroup& u);
	UnitGroup(vector<GPoint*>);
	~UnitGroup();
	void clearUnitGroup();
	void insert(GPoint* p);

	int allPointSize();
};
#endif