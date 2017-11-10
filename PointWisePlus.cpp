#include "PointWisePlus.h"

using namespace std;
PWiseP::PWiseP(int k, vector<GPoint*> PointCell)
{
	this->PwCount = 0;
	GGroup g = this->PointWiseInit(k, PointCell);
	this->Solve(k,g);
}
GGroup PWiseP::PointWiseInit(int k, vector<GPoint*> PointCell)
{
	GPoint* root = new GPoint();
	for (int i = PointCell.size() - 1; i >= 0; i--) {
		GPoint *p = PointCell[i];
		int p_layer = p->layer ;
		if (p_layer == 0) {//���
			p->firstParent = root;
			root->SimpleCSet.push_front(p);//����root��CS��
		}
		else {
			GPoint *parent = NULL;
			for (vector<GPoint*>::iterator it = p->ParentSet.begin(); it != p->ParentSet.end(); it++) {//��ȡFirstParent����p�����Ӧ��CS
				if ((*it)->layer == p_layer - 1) {
					if (parent == NULL || (*it)->index < parent->index) {
						parent = *it;
					}
				}
			}
			p->firstParent = parent;
			if (parent!=NULL) {
				parent->SimpleCSet.push_front(p);
			}
		}
	}

	GGroup gs;
	gs.pointSet.insert(root);
	gs.Stack.push_back(root);
	return gs;
}

void  PWiseP::Solve(int k,GGroup &g) {
	set<GPoint*> parentPoints;//g��g�����и��ڵ�
	for (vector<GPoint*>::iterator it = g.Stack.begin() + 1; it != g.Stack.end(); it++) {
		parentPoints.insert(*it);
		for (vector<GPoint*>::iterator ij = (*it)->ParentSet.begin(); ij != (*it)->ParentSet.end(); ij++) {
			parentPoints.insert(*ij);
		}
	}

	GPoint* lastPoint = g.Stack[g.Stack.size() - 1];
	for (list<GPoint*>::iterator it = (lastPoint->SimpleCSet).begin(); it != lastPoint->SimpleCSet.end(); it++) {//g��β�ڵ���ӽڵ㼯
		this->JudgeNewElement(k, g, parentPoints.size(), it, parentPoints);
	}
	// iterator all siblings.
	GPoint * parent = lastPoint->firstParent;
	while (parent) {
		if (parent->SimpleCSet.size() > 0) {
			list<GPoint*>::iterator it = parent->SimpleCSet.end();
			it--;
			for (; (*it) != lastPoint;it--){
				this->JudgeNewElement(k, g, parentPoints.size(), it, parentPoints);
			}
		}
		lastPoint = parent;
		parent = parent->firstParent;
	}
}


//�жϽڵ㡢���ڵ��Ƿ���parentPoints�в��ݹ�
int PWiseP::JudgeNewElement(int k,GGroup& g,int cpsize, list<GPoint*>::iterator it, set<GPoint*> &parentPoints) {
	if (parentPoints.find(*it) == parentPoints.end()) {
		cpsize++;//pl.layerΪ0
	}
	for (vector<GPoint*>::iterator ik = (*it)->ParentSet.begin(); ik != (*it)->ParentSet.end(); ik++) {
		if (parentPoints.find(*ik) == parentPoints.end()) {
			cpsize++;//pl.layerΪ1

		}
	}
	if (cpsize > k) {
		return 0;
	}
	else if (cpsize == k && g.Stack.size() == k) {

		GGroup ng;//gskyline(k)
		ng.pointSet.insert(parentPoints.begin(), parentPoints.end());
		ng.pointSet.insert(*it);
		for (vector<GPoint*>::iterator ik = (*it)->ParentSet.begin(); ik != (*it)->ParentSet.end(); ik++) {
			ng.pointSet.insert(*ik);
		}
		this->PwCount++;
		return 0;
	}

	else {
		g.Stack.push_back(*it);//��ʾΪgskyline(k-i)
		this->Solve(k, g);
		g.Stack.pop_back();
	}
	return 0;
}