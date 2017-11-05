#include "PointWisePlus.h"

using namespace std;
PWiseP::PWiseP(int k, vector<GPoint*> PointCell)
{
	this->PwCount = 0;
	this->Solve(k,this->PointWiseInit(k, PointCell));
}
GGroup PWiseP::PointWiseInit(int k, vector<GPoint*> PointCell)
{
	GPoint* root = new GPoint();
	for (int i = PointCell.size() - 1; i >= 0; i--) {
		GPoint *p = PointCell[i];
		int p_layer = p->layer - 1;
		if (p_layer == -1) {//���
			p->firstParent = root;
			root->SimpleCSet.push_front(p);//����root��CS��
		}
		else {
			GPoint *parent = NULL;
			for (vector<GPoint*>::iterator it = p->ParentSet.begin(); it != p->ParentSet.end(); it++) {//��ȡFirstParent����p�����Ӧ��CS
				if ((*it)->layer == p_layer) {
					if (parent == NULL) {
						parent = *it;
					}
					else if ((*it)->index < parent->index) {
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

	GGroup g;
	g.pointSet.insert(root);
	g.Stack.push_back(root);
}

void  PWiseP::Solve(int k,GGroup &g) {
	set<GPoint*> parentPoints;//g��g�����и��ڵ�
	for (vector<GPoint*>::iterator it = g.Stack.begin() + 1; it != g.Stack.end(); it++) {
		parentPoints.insert(*it);
		for (vector<GPoint*>::iterator ij = (*it)->ParentSet.begin(); ij != (*it)->ParentSet.end(); ij++) {
			parentPoints.insert(*ij);
		}
	}

	int psize = parentPoints.size();
	GPoint* lastPoint = g.Stack[g.Stack.size() - 1];
	for (list<GPoint*>::iterator it = (lastPoint->SimpleCSet).begin(); it != lastPoint->SimpleCSet.end(); it++) {//g��β�ڵ���ӽڵ㼯
		int cpsize = psize;
		if (parentPoints.find(*it) == parentPoints.end()) {
			cpsize++;
		}
		for (vector<GPoint*>::iterator ik = (*it)->ParentSet.begin(); ik != (*it)->ParentSet.end(); ik++) {
			if (parentPoints.find(*ik) == parentPoints.end()) {
				cpsize++;

			}
		}
		if (cpsize > k) {
			continue;
		}
		else if (cpsize == k && g.Stack.size() == k) {

			GGroup ng;
			ng.pointSet.insert(parentPoints.begin(), parentPoints.end());
			ng.pointSet.insert(*it);
			for (vector<GPoint*>::iterator ik = (*it)->ParentSet.begin(); ik != (*it)->ParentSet.end(); ik++) {
				ng.pointSet.insert(*ik);
			}
			PwCount++;
			continue;

		}

		else {
			g.Stack.push_back(*it);
			Solve(k,g);
			g.Stack.pop_back();
		}
	}
	// iterator all siblings.
	GPoint * parent = lastPoint->firstParent;
	while (parent) {
		if (parent->SimpleCSet.size() > 0) {
			list<GPoint*>::reverse_iterator it = parent->SimpleCSet.rbegin();

			while ((*it) != lastPoint) {
				//
				int cpsize = psize;
				if (parentPoints.find(*it) == parentPoints.end()) {
					cpsize++;
				}
				for (vector<GPoint*>::iterator ik = (*it)->ParentSet.begin(); ik != (*it)->ParentSet.end(); ik++) {
					if (parentPoints.find(*ik) == parentPoints.end()) {
						cpsize++;
					}
				}
				if (cpsize > k) {
					it++;
					continue;
				}
				else if (cpsize == k && g.Stack.size() == k) {
					GGroup ng;
					ng.pointSet.insert(parentPoints.begin(), parentPoints.end());
					ng.pointSet.insert(*it);
					for (vector<GPoint*>::iterator ik = (*it)->ParentSet.begin(); ik != (*it)->ParentSet.end(); ik++) {
						ng.pointSet.insert(*ik);
					}
					PwCount++;
					it++;
					continue;

				}

				else {
					g.Stack.push_back(*it);
					Solve(k,g);
					g.Stack.pop_back();
				}
				it++;
			}
		}
		lastPoint = parent;
		parent = parent->firstParent;
	}
}