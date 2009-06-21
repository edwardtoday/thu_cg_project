#include <windows.h>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <sstream>
#include <string>
#include "Triangles.h"
using namespace std;
using namespace Triangles;

int globalfirsttime = 0;							// ���¼���Ϊ�Ƿ����������Ϣ
int toshow = 0;
bool show = false;
bool outputfinal = true;

extern bool ShowFrame;								// ��������λȷ�������ʾģ��
extern bool ShowBlue;
Objects::Objects()									// ���캯�����������������0
{
	this->Clear();	
}
Objects::Objects(const Objects &ano)					// �������캯��
{
	this->points.assign(ano.points.begin(), ano.points.end());
	this->validfaces.assign(ano.validpoints.begin(), ano.validpoints.end());

	this->faces.assign(ano.faces.begin(), ano.faces.end());
	this->normals.assign(ano.normals.begin(), ano.normals.end());
	this->validfaces.assign(ano.validfaces.begin(), ano.validfaces.end());

	this->Q.assign(ano.Q.begin(), ano.Q.end());
	this->costheap.assign(ano.costheap.begin(), ano.costheap.end());
	
	this->linkpoints.assign(ano.linkpoints.begin(), ano.linkpoints.end());
	this->linkfaces.assign(ano.linkfaces.begin(), ano.linkfaces.end());
	this->linkcostheap.assign(ano.linkcostheap.begin(), ano.linkcostheap.end());
	this->sizeofvalidfaces = ano.sizeofvalidfaces;
	this->sizeofvalidpoints = ano.sizeofvalidpoints;
}
Objects::Objects(char *filename)						// ʹ���ļ���Ϊfilename��objģ���ļ���ʼ��
{
	this->Load(filename);
}
void Objects::Clear()								// �����������
{
	points.clear();
	validpoints.clear();

	faces.clear();
	normals.clear();
	validfaces.clear();

	Q.clear();
	costheap.clear();

	linkpoints.clear();

	linkcostheap.clear();
	linkfaces.clear();
	linkvalidpoints.clear();
	sizeofvalidfaces = 0;
	sizeofvalidpoints = 0;
}
Specular::Vector3 Objects::Normailize(int p)			// ���ص�p�������εķ�����
{
	Specular::Vector3 temp;
	Specular::Vector3 v1, v2;
	v1 = points[faces[p].pt1] - points[faces[p].pt2];
	v2 = points[faces[p].pt1] - points[faces[p].pt3];
	temp = v1 ^ v2;							// ʹ�������߶�Ӧ�����������Ϊ������
	temp.Normalize();
	return temp;		
}
void Objects::Load(char *filename)					// �����ļ���Ϊfilename��objģ���ļ�
{
	char c;
	ifstream fin(filename);
	int temppoints, tempfaces;
	this->Clear();							// �����������
	char tempchars[100];

	fin >> c;
	while (c != 'v') {
		fin.getline(tempchars, 100);
		fin >> c;
	}

	while(c == 'v') {
		Specular::Vector3 temp;
		fin >> temp.x >> temp.y >> temp.z;	// ���붥������
		points.push_back(temp);				// ����¶���
		validpoints.push_back(true);		// �����¼Ӷ������Ч
		sizeofvalidpoints++;
		fin >> c;
	}
	cout << "points = " << sizeofvalidpoints << endl;
	while (c != 'f') {
		fin.getline(tempchars, 100);
		fin >> c;
	}
	while (c == 'f') {
		triangles temp;
		fin >> temp.pt1 >> temp.pt2 >> temp.pt3;			// ���������ζ�����
		if (temp.pt1 == 0 || temp.pt2 == 0 || temp.pt3 == 0) break;
		temp.pt1--;
		temp.pt2--;
		temp.pt3--;
		faces.push_back(temp);								// �����������
		validfaces.push_back(true);							// ��������������Ч
		sizeofvalidfaces++;
		fin >> c;
		while (c != 'f' && c != 'g') {						// ����������ĸ��ʾ�ĺ���
			fin.getline(tempchars, 100);
			fin >> c;
		}
		if (c == 'g') 
			break;
	}
	fin.close();
	cout << "faces = " << sizeofvalidfaces << endl;
	cout << "faces.size() = " << faces.size() << endl;
	cout << faces[faces.size() - 1].pt1 << ' ' << faces[faces.size() - 1].pt2 << ' ' << faces[faces.size() - 1].pt3 << endl;
	for (int i = 0; i < faces.size(); i++) {				// ���������η�����
		normals.push_back(Normailize(i));
	}
	cout << "normals.size() = " << normals.size() << endl;
	for (int i = 0; i < normals.size(); i++)				// ��λ�������η�����
		normals[i].Normalize();
}

void Objects::InitializeLinks()						// ��ʼ��link��ص�����
{
	link null;
	for (int i = 0; i < points.size(); i++)					// ��������
		linkpoints.push_back(null);
	cout << "linkpoints.size() = " << linkpoints.size() << endl;
	cout << "�����������" << OK << endl;
	for (int i = 0; i < faces.size(); i++) {				// �������ӹ�ϵ
		linkpoints[faces[i].pt1].push_back(faces[i].pt2);
		linkpoints[faces[i].pt1].push_back(faces[i].pt3);

		linkpoints[faces[i].pt2].push_back(faces[i].pt1);
		linkpoints[faces[i].pt2].push_back(faces[i].pt3);

		linkpoints[faces[i].pt3].push_back(faces[i].pt1);
		linkpoints[faces[i].pt3].push_back(faces[i].pt2);
	}

	cout << "�������" << OK << endl;
	for (int i = 0; i < points.size(); i++)				// ����ÿһ��������������������������
		sort(linkpoints[i].begin(), linkpoints[i].end());
	cout << "�������" << OK << endl;
	for (int i = 0; i < points.size(); i++) {			// ��Ϊÿһ�����߼������Σ���˶�����������
		for (int j = 0; j < linkpoints[i].size(); j = j + 2) {
			linkpoints[i][j / 2] = linkpoints[i][j];
		}
		int templen = linkpoints[i].size();
		for (int j = 0; j < templen / 2; j++)
			linkpoints[i].pop_back();
	}
	cout << "���������" << OK << endl;

	for (int i = 0; i < points.size(); i++)				// ��������
		linkfaces.push_back(null);
	for (int i = 0; i < faces.size(); i++) {			// ���������������ε����ӹ�ϵ
		linkfaces[faces[i].pt1].push_back(i);
		linkfaces[faces[i].pt2].push_back(i);
		linkfaces[faces[i].pt3].push_back(i);
	}
	cout << "���������" << OK << endl;
}

void Objects::SetR(double r)							// ������ֵΪr						
{
	this->t = t;
}
Matrix::matrix4 Objects::ComputeQ(int p)				// �����p����ľ���Qֵ
{
	Q[p] = Matrix::Zero;								// ��ʼ����Ϊ0����	
	Matrix::matrix4 temp;
	for (int i = 0; i < linkfaces[p].size(); i++) {
		Matrix::Vector4 plane;
		plane.x = normals[linkfaces[p][i]].x;				// ��������Ϊ����ƽ��
		plane.y = normals[linkfaces[p][i]].y;
		plane.z = normals[linkfaces[p][i]].z;
		plane.p = -points[p] * normals[linkfaces[p][i]];
		temp = plane ^ plane;								// ����ƽ���Ӧ����
		Q[p] = Q[p] + temp;
	}
	return Q[p];
}
void Objects::ComputeQ()								// �������е�ľ���Qֵ
{	
	for (int i = 0; i < points.size(); i++) {
		Q.push_back(Matrix::Zero);
	}
	cout << "Q.size() = " << Q.size() << endl;
	for (int i = 0; i < points.size(); i++) {
		ComputeQ(i);
	}
}
double Objects::Det3x3(double *matrix)					// ����һ��3x3�ľ��������ʽֵ
{											// ֱ��������ʽչ������
	int plus[3][3] = {{0, 4, 8}, {1, 5, 6}, {3, 7, 2}};
	int minus[3][3] = {{2, 4, 6}, {1, 3, 8}, {5, 7, 0}};
	double ans = 0;
	for (int i = 0; i < 3; i++) {
		double temp = 1;
		for (int j = 0; j < 3; j++)
			temp *= matrix[plus[i][j]];
		ans += temp;
	}
	for (int i = 0; i < 3; i++) {
		double temp = 1;
		for (int j = 0; j < 3; j++)
			temp *= matrix[minus[i][j]];
		ans -= temp;
	}
	return ans;
}
struct item Objects::GetCost(int pti, int ptj)		// �����pti��ptj����ɵ�pair��costֵ
{
	Matrix::matrix4 m;
	m = Q[pti] + Q[ptj];

	double Qsolve[4][10];									// ���¼���Ϊ��Cramer�������
	memset(Qsolve, 0, sizeof(Qsolve));						// [ matrix00, matrix01, matrix02 ] [ optimalv4_x ] + [ matrix03 ] = [ 0 ]
	for (int k = 0; k < 4; k++) {							// | matrix10, matrix11, matrix12 | | optimalv4_y | + | matrix13 | = | 0 |
		for (int i = 0; i< 3; i++)							// [ matrix20, matrix21, matrix22 ] [ optimalv4_y ] + [ matrix23 ] = [ 0 ]
			for (int j = 0; j < 3; j++)
				Qsolve[k][i * 3 + j] = m.matrix[i][j];
	}
	for (int k = 1; k < 4; k++)
		for (int i = 0; i < 3; i++)
			Qsolve[k][i * 3 + k - 1] = m.matrix[i][3];

	Matrix::Vector4 optimalv4;
	double temp3x3 = Det3x3(Qsolve[0]);
	if (temp3x3 != 0) {											// ��ֹ����Ϊ0
		optimalv4.x = -Det3x3(Qsolve[1]) / Det3x3(Qsolve[0]);
		optimalv4.y = -Det3x3(Qsolve[2]) / Det3x3(Qsolve[0]);
		optimalv4.z = -Det3x3(Qsolve[3]) / Det3x3(Qsolve[0]);
	}
	else {
		optimalv4.x = optimalv4.y = optimalv4.z = 0;
	}
	optimalv4.p = 1;

	double cost;
	cost = optimalv4 * m * optimalv4;						// ����cost = vbest^T * m * vbest
	item best;
	if ((cost >=2 || cost <= 2) == false) {
		cost = 0.0;
	}
	best.cost = fabs(cost);
	best.optimalv3.x = optimalv4.x;
	best.optimalv3.y = optimalv4.y;
	best.optimalv3.z = optimalv4.z;
	best.validpair.pt1 = pti;
	best.validpair.pt2 = ptj;
	return best;
}
struct item Objects::GetCost(int p)					// ����costheap�еĵ�p��ֵ
{
	costheap[p] = GetCost(costheap[p].validpair.pt1, costheap[p].validpair.pt2);
	return costheap[p];
}

template<class Ty>
void Objects::swap(std::vector<Ty> v, int i, int j)	// ����v�е�i��jԪ��
{
	Ty temp = v[j];
	v[j] = v[i];
	v[i] = temp;			
}

void Objects::swapcostheap(int i, int j)				// ����costheap�ĵ�i��jԪ��
{
	int t = -1;
	
	for (int k = 0; k < linkcostheap[costheap[i].validpair.pt1].size(); k++) {
		if (linkcostheap[costheap[i].validpair.pt1][k] == i)
			linkcostheap[costheap[i].validpair.pt1][k] = t;		// �������ӹ�ϵ
	}
	for (int k = 0; k < linkcostheap[costheap[i].validpair.pt2].size(); k++) {
		if (linkcostheap[costheap[i].validpair.pt2][k] == i)
			linkcostheap[costheap[i].validpair.pt2][k] = t;		// �������ӹ�ϵ
	}
	for (int k = 0; k < linkcostheap[costheap[j].validpair.pt1].size(); k++) {
		if (linkcostheap[costheap[j].validpair.pt1][k] == j)
			linkcostheap[costheap[j].validpair.pt1][k] = i;		// �������ӹ�ϵ
	}
	for (int k = 0; k < linkcostheap[costheap[j].validpair.pt2].size(); k++) {
		if (linkcostheap[costheap[j].validpair.pt2][k] == j)
			linkcostheap[costheap[j].validpair.pt2][k] = i;		// �������ӹ�ϵ
	}
	for (int k = 0; k < linkcostheap[costheap[i].validpair.pt1].size(); k++) {
		if (linkcostheap[costheap[i].validpair.pt1][k] == t)
			linkcostheap[costheap[i].validpair.pt1][k] = j;		// �������ӹ�ϵ
	}
	for (int k = 0; k < linkcostheap[costheap[i].validpair.pt2].size(); k++) {
		if (linkcostheap[costheap[i].validpair.pt2][k] == t)
			linkcostheap[costheap[i].validpair.pt2][k] = j;		// �������ӹ�ϵ
	}
	item temp;
	temp = costheap[i];
	costheap[i] = costheap[j];
	costheap[j] = temp;	
}
void Objects::HeapChange(int p)						// ����costheap�е�p��Ԫ�ط����ı�
{
	if (p >= costheap.size())
		return;
	if (costheap.size() == 1) return;
	if (p != 0 && costheap[p] <= costheap[(p - 1) / 2])	// p���丸�ڵ�С
	{
		while (1) {
			if (p > 0) {
				if (costheap[p] <= costheap[(p - 1) / 2]) {	// ���ÿһ�ζ��ȸ��ڵ�С
					swapcostheap(p, (p - 1) / 2);			// �븸�ڵ㽻��
					p = (p - 1) / 2;
					continue;
				}
			}
			break;
		}
	}
	else {										// p�����ӽڵ��
		if (2 * p + 2 > costheap.size()) 
			return;			
		while (1) {
			if (show) {
				cout << "p = " << p << " cost[p] = " << costheap[p].cost << " cost[p.fa] = " << costheap[(p - 1) / 2].cost << endl;
				if (2 * p + 2 < costheap.size()) 
					cout << "---- cost[p.l_son] = " << costheap[2 * p + 1].cost << " cost[p.r_son] = " << costheap[2 * p + 2].cost << endl;
				else if (2 * p + 1 < costheap.size()) 
					cout << "---- cost[p.l_son] = " << costheap[2 * p + 1].cost << endl;
				cout << "size = " << costheap.size() << endl;
			}
			if (2 * p + 2 < costheap.size()) {	// �����ӽڶ�����
				if ((costheap[2 * p + 1] <= costheap[p]) && (costheap[2 * p + 1] <= costheap[2 * p + 2])) {		// ���ӽڵ���С
					swapcostheap(p, 2 * p + 1);		// �����ӽڵ㽻��				
					p = 2 * p + 1;
					continue;
				}
				else if ((costheap[2 * p + 2] <= costheap[p]) && (costheap[2 * p + 2] <= costheap[2 * p + 1])) {	// ���ӽڵ���С		
					swapcostheap(p, 2 * p + 2);		// �����ӽڵ㽻��
					p = 2 * p + 2;
					continue;
				}
			} 
			else if (2 * p + 1 < costheap.size()) {			// ֻ������ڵ�
				if (costheap[2 * p + 1] <= costheap[p]) {	// �����ӽڵ��
					swapcostheap(p, 2 * p + 1);
					p = 2 * p + 1;
					continue;
				}
			}
			break;
		}
	}
}
void Objects::HeapPush(struct item one)				// ��costheap�в���һ��Ԫ��one
{
	costheap.push_back(one);
	linkcostheap[one.validpair.pt1].push_back(costheap.size() - 1);	// �������㵽costheap�����ӹ�ϵ
	linkcostheap[one.validpair.pt2].push_back(costheap.size() - 1); // �������㵽costheap�����ӹ�ϵ
	HeapChange(costheap.size() - 1);							// �޸Ķ�
}

struct item Objects::HeapPop(int topop)					// ����costheap�еĵ�totop��Ԫ��
{
	struct item best = costheap[topop];
	if (show) {											// ���������Ϣ
	cout << "topop = " << topop << endl;
	cout << "costheap[topop].cost = " << costheap[topop].cost << endl;
	cout << "costheap.rbegin()->cost = " << costheap.rbegin()->cost << endl;	

	cout << "topop = " << topop << " costheap.size() - 1 = " << costheap.size() - 1 << endl; 
	for (int i = 0; i < linkcostheap[costheap[topop].validpair.pt1].size(); i++)
		cout << linkcostheap[costheap[topop].validpair.pt1][i] << ' ';
	cout << endl;
	for (int i = 0; i < linkcostheap[costheap[topop].validpair.pt2].size(); i++)
		cout << linkcostheap[costheap[topop].validpair.pt2][i] << ' ';
	cout << endl;
	for (int i = 0; i < linkcostheap[costheap[costheap.size() - 1].validpair.pt1].size(); i++)
		cout << linkcostheap[costheap[costheap.size() - 1].validpair.pt1][i] << ' ';
	cout << endl;
	for (int i = 0; i < linkcostheap[costheap[costheap.size() - 1].validpair.pt2].size(); i++)
		cout << linkcostheap[costheap[costheap.size() - 1].validpair.pt2][i] << ' ';
	cout << endl;
	cout << "topop:" << costheap[topop].validpair.pt1 << ' ' << costheap[topop].validpair.pt2 << endl;
	cout << "costheap.size() - 1:" << costheap[costheap.size() - 1].validpair.pt1 << ' ' << costheap[costheap.size() - 1].validpair.pt2 << endl;
	}
	swapcostheap(topop, costheap.size() - 1);	// ���õ����ڵ�Ϊ���һ��Ԫ��
	if (show) {									// ���������Ϣ
	cout << "***********************swap**********************" << endl;

	cout << "topop = " << topop << " costheap.size() - 1 = " << costheap.size() - 1 << endl; 
	for (int i = 0; i < linkcostheap[costheap[topop].validpair.pt1].size(); i++)
		cout << linkcostheap[costheap[topop].validpair.pt1][i] << ' ';
	cout << endl;
	for (int i = 0; i < linkcostheap[costheap[topop].validpair.pt2].size(); i++)
		cout << linkcostheap[costheap[topop].validpair.pt2][i] << ' ';
	cout << endl;
	for (int i = 0; i < linkcostheap[costheap[costheap.size() - 1].validpair.pt1].size(); i++)
		cout << linkcostheap[costheap[costheap.size() - 1].validpair.pt1][i] << ' ';
	cout << endl;
	for (int i = 0; i < linkcostheap[costheap[costheap.size() - 1].validpair.pt2].size(); i++)
		cout << linkcostheap[costheap[costheap.size() - 1].validpair.pt2][i] << ' ';
	cout << endl;
	cout << "topop:" << costheap[topop].validpair.pt1 << ' ' << costheap[topop].validpair.pt2 << endl;
	cout << "costheap.size() - 1:" << costheap[costheap.size() - 1].validpair.pt1 << ' ' << costheap[costheap.size() - 1].validpair.pt2 << endl;
	cout << topop << ' ' << costheap.size() - 1 << endl;
	}
	for (int i = 0; i < linkcostheap[costheap.rbegin()->validpair.pt1].size(); i++)
		if (linkcostheap[costheap.rbegin()->validpair.pt1][i] == costheap.size() - 1) {
			linkcostheap[costheap.rbegin()->validpair.pt1].erase(linkcostheap[costheap.rbegin()->validpair.pt1].begin() + i);
		}
	for (int i = 0; i < linkcostheap[costheap.rbegin()->validpair.pt2].size(); i++)
		if (linkcostheap[costheap.rbegin()->validpair.pt2][i] == costheap.size() - 1) {
			linkcostheap[costheap.rbegin()->validpair.pt2].erase(linkcostheap[costheap.rbegin()->validpair.pt2].begin() + i);
		}
	costheap.pop_back();					// �����һ��Ԫ�����

	HeapChange(topop);						// �޸Ķ�
	return best;
}

void Objects::MakeHeap()								// ��ʼ��costheap��
{
	bool firsttime = true;
	link null;
	for (int i = 0; i < points.size(); i++)
		linkcostheap.push_back(null);
	for (int i = 0; i < points.size(); i++) {
		for (int j = 0; j < linkpoints[i].size(); j++) 
			if (i < linkpoints[i][j]) {								// ȷ��ÿһ��pairֻ����һ��
				struct item temp = GetCost(i, linkpoints[i][j]);	// ���һ��pair

				HeapPush(temp);										// ��pair�������				

			}
	}
	cout << "linkcostheap.size() = " << linkcostheap.size() << endl;
// ����Ϊ����С���ѵĴ���
	for (int i = 1; i < costheap.size(); i++) 
		if (costheap[i] < costheap[(i - 1) / 2]) 
			cout << "Heap Error!" << endl;
}

void Objects::InitializeAll(char *filename)				// ��ʼ��ȫ������
{
	this->Stop();
	cout << filename << endl;

// �����ļ�
	this->Load(filename);
	cout << "File Load OK" << endl;
	cout << "InitializeLinks......" << endl;

// ��ʼ������
	this->InitializeLinks();
	cout << "InitializeLinks OK" << endl;
	cout << "ComputeQ......" << endl;

// ����Q
	this->ComputeQ();
	cout << "ComputeQ OK" << endl;
	cout << "MakeHeap......" << endl;

// ���ɶ�
	this->MakeHeap();
	cout << "MakeHeap OK" << endl;

// ����Ϊ���Զ����������ȷ�ԵĴ���
	cout << linkcostheap.size() << endl;
	for (int i = 0; i < linkcostheap.size(); i++) {
		for (int j = 0; j < linkcostheap[i].size(); j++) {
			bool ok = false;		
			if (costheap[linkcostheap[i][j]].validpair.pt1 == i) 
				ok = true;
			if (costheap[linkcostheap[i][j]].validpair.pt2 == i) 
				ok = true;
			if (ok == false) 
				cout << "Link Error!" << endl;
		}
	}

	this->CheckAll();
	cout << "*****************************************************************" << endl;
}

/**********************************
ɾ��pair���裺
	1����ò�������¶���			��Ӧ�����飭points
	2����ɾ���Ķ�����Ϊ��Ч		��Ӧ�����飭validpoints
	3����ɾ��������Ϊ��Ч			��Ӧ�����飭validfaces

	4�������µĶ�����������		��Ӧ�����飭linkfaces
	5�������޸ĵ�����¶���		��Ӧ�����飭faces
	6�������޸ĵ���ķ�����		��Ӧ�����飭normals
	
	7�������µĶ����Qֵ			��Ӧ�����飭Q

	8�������µĶ������ӵ�pair		��Ӧ�����飭linkcostheap
	9���޸Ķ��е�cost���Ҹ���		��Ӧ�����飭costheap
**********************************/
void Objects::DeleteVertex()							// ɾ��һ��pair
{
	// ���ɾ����pair
	struct item vbest = this->HeapPop();

	// Step 1 - ��ò�������¶���	��Ӧ�����飭points	
	points.push_back(vbest.optimalv3);
	validpoints.push_back(true);
	sizeofvalidpoints++;

	// Step 2 - ��ɾ���Ķ�����Ϊ��Ч	��Ӧ�����飭validpoints
	int todelete1 = vbest.validpair.pt1;		// �������������Լ򻯳���
	int todelete2 = vbest.validpair.pt2;		// �������������Լ򻯳���
	validpoints[todelete1] = false;
	validpoints[todelete2] = false;	
	sizeofvalidpoints--;
	sizeofvalidpoints--;

	// Step 3 - ��ɾ��������Ϊ��Ч	��Ӧ�����飭validfaces
	std::vector<int> sharefaces;
	for (int i = 0; i < linkfaces[todelete1].size(); i++)
		for (int j = 0; j < linkfaces[todelete2].size(); j++)
			if (linkfaces[todelete1][i] == linkfaces[todelete2][j]) {	// ���һ����ʹ��pair�е���������
				validfaces[linkfaces[todelete1][i]] = false;			// ��һ��Ҫ��ɾ��
				int c =  faces[linkfaces[todelete1][i]].pt1 ^ faces[linkfaces[todelete1][i]].pt2 ^ faces[linkfaces[todelete1][i]].pt3 ^ todelete1 ^ todelete2;
				for (int k = 0; k < linkfaces[c].size(); k++)
					if (linkfaces[c][k] == linkfaces[todelete1][i]) {
						linkfaces[c].erase(linkfaces[c].begin() + k);
					}
				sizeofvalidfaces--;
			}
			if (!outputfinal) {
				cout << "Step 2~3" << OK << endl;	
			}
	// Step 4 - �����µĶ�����������		��Ӧ�����飭linkfaces
	// Step 5 - �����޸ĵ�����¶���		��Ӧ�����飭faces
	// Step 6 - �����޸ĵ���ķ�����		��Ӧ�����飭normals	
	link nulllinkface;
	linkfaces.push_back(nulllinkface);
	for (int i = 0; i < linkfaces[todelete1].size(); i++) {					// ����todelete1�����ӹ�ϵ
		if (validfaces[linkfaces[todelete1][i]]) {							// ���������������Ч
			//cout << "linkfaces[todelete1][" << i << "] = " << linkfaces[todelete1][i] << endl;
			linkfaces.rbegin()->push_back(linkfaces[todelete1][i]);			// �����������ӵ���������
			if (faces[linkfaces[todelete1][i]].pt1 == todelete1) {			// ���¼���Ϊ����Ч�����εĶ������Ϊ��������
				faces[linkfaces[todelete1][i]].pt1 = linkfaces.size() - 1;
			} else if (faces[linkfaces[todelete1][i]].pt2 == todelete1) {
				faces[linkfaces[todelete1][i]].pt2 = linkfaces.size() - 1;
			} else if (faces[linkfaces[todelete1][i]].pt3 == todelete1) {
				faces[linkfaces[todelete1][i]].pt3 = linkfaces.size() - 1;
			}
			normals[linkfaces[todelete1][i]] = Normailize(linkfaces[todelete1][i]);	// �������������η�����
		}
	}
	for (int i = 0; i < linkfaces[todelete2].size(); i++) {					// ����todelete2�����ӹ�ϵ
		if (validfaces[linkfaces[todelete2][i]]) {							// ���������������Ч
			linkfaces.rbegin()->push_back(linkfaces[todelete2][i]);			// �����������ӵ���������
			if (faces[linkfaces[todelete2][i]].pt1 == todelete2) {			// ���¼���Ϊ����Ч�����εĶ������Ϊ��������
				faces[linkfaces[todelete2][i]].pt1 = linkfaces.size() - 1;
			} else if (faces[linkfaces[todelete2][i]].pt2 == todelete2) {
				faces[linkfaces[todelete2][i]].pt2 = linkfaces.size() - 1;
			} else if (faces[linkfaces[todelete2][i]].pt3 == todelete2) {
				faces[linkfaces[todelete2][i]].pt3 = linkfaces.size() - 1;
			}
			normals[linkfaces[todelete2][i]] = Normailize(linkfaces[todelete2][i]);	// �������������η�����
		}
	}
	// Step 7 - �����µĶ����Qֵ			��Ӧ�����飭Q
	Q.push_back(Matrix::Zero);
	ComputeQ(Q.size() - 1);

	// Step 8 - �����µĶ������ӵ�pair	��Ӧ�����飭linkcostheap
	// Step 9 - �޸Ķ��е�cost���Ҹ���	��Ӧ�����飭costheap	
	link nullpair;
	linkcostheap.push_back(nullpair);
	item temp;
	vector<int> ano;

	for (int i = 0; i < linkcostheap[todelete1].size(); i++) {					// ����delete1�����ı�������������
		int anopoint = 0;
		if (costheap[linkcostheap[todelete1][i]].validpair.pt1 == todelete1)
			anopoint = costheap[linkcostheap[todelete1][i]].validpair.pt2;
		else 
			anopoint = costheap[linkcostheap[todelete1][i]].validpair.pt1;
		if (validpoints[anopoint]) {
			ano.push_back(anopoint);
		}
	}

	for (int i = 0; i < linkcostheap[todelete2].size(); i++) {					// ����delete2�����ı�������������
		int anopoint = 0;
		if (costheap[linkcostheap[todelete2][i]].validpair.pt1 == todelete2)
			anopoint = costheap[linkcostheap[todelete2][i]].validpair.pt2;
		else 
			anopoint = costheap[linkcostheap[todelete2][i]].validpair.pt1;
		if (validpoints[anopoint]) {
			ano.push_back(anopoint);
		}
	}

	sort(ano.begin(), ano.end());												// �����������ر�
	int p = 1;
	for (int i = 1; i < ano.size(); i++)
		if (ano[i] != ano[i - 1]) {
			ano[p] = ano[i];
			p++;
		}
	ano.erase(ano.begin() + p, ano.end());

	int tempsize = linkcostheap[todelete1].size();		// �����delete1�����ı�
	for (int i = 0; i < tempsize; i++) {
		HeapPop(linkcostheap[todelete1][0]);
	}
	tempsize = linkcostheap[todelete2].size();			// �����delete2�����ı�
	for (int i = 0; i < tempsize; i++) {
		HeapPop(linkcostheap[todelete2][0]);
	}

	for (int i = 0; i < ano.size(); i++) {				// ����������ڵ������ı�
		temp = GetCost(points.size() - 1, ano[i]);
		HeapPush(temp);
	}

	if (!outputfinal) {
		this->CheckAll();
		cout << "��������Ϊ" << sizeofvalidpoints << endl;
		cout << "������Ϊ" << sizeofvalidfaces << endl;
	}
}
void Objects::Stop() {
	//std::cout << "Run to Stop" << std::endl;
}
bool Objects::CheckLinkfaces() {	// ��������֮������ӹ�ϵ�Ƿ���ȷ

	for (int i = 0; i < linkfaces.size(); i++) {	// �жϵ����������Ƿ��������
		if (validpoints[i]) {
			for (int j = 0; j < linkfaces[i].size(); j++) {
				bool ok = false;
				if (faces[linkfaces[i][j]].pt1 == i)
					ok = true;
				if (faces[linkfaces[i][j]].pt2 == i)
					ok = true;
				if (faces[linkfaces[i][j]].pt3 == i)
					ok = true;
				if (ok == false) {
					cout << "���������Ӵ���" << endl;
					return false;
				}
			}
		}
	}
	for (int i = 0; i < faces.size(); i++) {	// �ж����������ӵĵ��Ƿ�����Ч��
		if (validfaces[i] == false) continue;
		bool ok = true;		
		if (validpoints[faces[i].pt1] == false) ok = false;
		if (validpoints[faces[i].pt2] == false) ok = false;
		if (validpoints[faces[i].pt3] == false) ok = false;
		if (ok == false) {
			cout << i << ' ' << faces[i].pt1 << ' ' << faces[i].pt2 << ' ' << faces[i].pt3 << endl;
			cout << "����������������" << endl;
			return false;
		}
	}
	for (int i = 0; i < linkfaces.size(); i++) {	// �жϵ����ӵ��������Ƿ�����Ч������
		if (validpoints[i] == false) continue;
		bool ok = true;
		for (int j = 0; j < linkfaces[i].size(); j++)
			if (validfaces[linkfaces[i][j]] == false) {
				cout << "point = " << i << endl;
				cout << "��������������" << endl;
				ok = false;
				return false;
			}
	}

	for (int i = 0; i < faces.size(); i++) {	// �ж����������ӵĵ��Ƿ�������������
		if (validfaces[i] == false) continue;
		bool ok = false;
		for (int j = 0; j < linkfaces[faces[i].pt1].size(); j++)
			if (linkfaces[faces[i].pt1][j] == i)
				ok = true;
		if (ok == false) {
			cout << "���������ӵ��д���" << endl;
			return false;
		}
		for (int j = 0; j < linkfaces[faces[i].pt2].size(); j++)
			if (linkfaces[faces[i].pt2][j] == i)
				ok = true;
		if (ok == false) {
			cout << "���������ӵ��д���" << endl;
			return false;
		}
		for (int j = 0; j < linkfaces[faces[i].pt3].size(); j++)
			if (linkfaces[faces[i].pt3][j] == i)
				ok = true;
		if (ok == false) {
			cout << "���������ӵ��д���" << endl;
			return false;
		}
	}	

	int left = 0, right = 0;				// �ж����������֮������ӹ�ϵ�����Ƿ����
	for (int i = 0; i < faces.size(); i++)
		if (validfaces[i]) left++;
	left = left * 3;	
	for (int i =0; i < linkfaces.size(); i++)
		if (validpoints[i])
			right += linkfaces[i].size();
	if (left != right) {
		cout << "������������������" << endl;
		return false;
	}

	for (int i = 0; i < linkfaces.size(); i++) {	// �ж�һ�����Ƿ�������ͬһ��������
		if (validpoints[i] == false) continue;
		for (int pa = 0; pa < linkfaces[i].size(); pa++)
			for (int pb = 0; pb < pa; pb++)
				if (linkfaces[i][pa] == linkfaces[i][pb]) {
					cout << "������������" << endl;
					return false;
				}
	}
	for (int i = 0; i < faces.size(); i++) {		// �ж�һ���������Ƿ�������ͬһ����
		if (faces[i].pt1 == faces[i].pt2) {
			cout << "�����ζ������" << endl;
			return false;
		}
		if (faces[i].pt1 == faces[i].pt3) {
			cout << "�����ζ������" << endl;
			return false;
		}
		if (faces[i].pt3 == faces[i].pt2) {
			cout << "�����ζ������" << endl;
			return false;
		}
	}

	int numberoffaces = 0;							// �ж������θ����Ƿ���ȷ
	for (int i = 0; i < faces.size(); i++) 
		if (validfaces[i]) 
			numberoffaces++;
	if (numberoffaces != sizeofvalidfaces) {
		cout << "�����θ�������" << endl;
		return false;
	}
	cout << "CheckLinkfaces ������������ȷ" << endl;
	return true;
}

bool Objects::CheckLinkcostheap()						// �ж�linkcostheap�������Ƿ����
{
	for (int i = 1; i < costheap.size(); i++)			// �ж�linkcostheap�Ƿ���С����
		if (costheap[i] < costheap[(i - 1) / 2]) {
			cout << "i: " << costheap[i].cost << ' ' << "(i - 1) / 2: " << costheap[(i - 1) / 2].cost << endl;
			cout << "i = " << i << endl;
			cout << "С���Ѵ���" << endl;
			return false;			
		}

	for (int i = 0; i < linkcostheap.size(); i++) {		// �жϵ����ӵı��Ƿ����ӵ�
		if (validpoints[i] == false) continue;
		for (int j = 0; j < linkcostheap[i].size(); j++) {
			bool ok = false;		
			if (costheap[linkcostheap[i][j]].validpair.pt1 == i) {				
				ok = true;
			}
			if (costheap[linkcostheap[i][j]].validpair.pt2 == i) {				
				ok = true;
			}
			if (ok == false) {
				cout << "�����Ӵ���" << endl;
				return false;
			}
		}
	}
	int icon = 0;
	for (int i = 0; i < costheap.size(); i++)		// �жϱ����ӵĵ��Ƿ���Ч
		if (validpoints[costheap[i].validpair.pt1] == false || validpoints[costheap[i].validpair.pt2] == false){
			cout << "pair = " << i << endl;
			if (validpoints[costheap[i].validpair.pt1] == false)
				cout << "point = " << costheap[i].validpair.pt1 << endl;
			if (validpoints[costheap[i].validpair.pt2] == false)
				cout << "point = " << costheap[i].validpair.pt2 << endl;
			icon++;
		}
	if (icon != 0) {
		cout << "���������ӣ�" << icon << "��" << endl;
		return false;
	}
	for (int i = 0; i < costheap.size(); i++) {		// �жϱ����ӵĵ��Ƿ������ϱ�
		bool ok = false;
		for (int j = 0; j < linkcostheap[costheap[i].validpair.pt1].size(); j++)
			if (linkcostheap[costheap[i].validpair.pt1][j] == i)
				ok = true;
		if (ok == false) {
			cout << "�����ӵ��д���" << endl;
			return false;
		}
		ok = false;
		for (int j = 0; j < linkcostheap[costheap[i].validpair.pt2].size(); j++)
			if (linkcostheap[costheap[i].validpair.pt2][j] == i)
				ok = true;
		if (ok = false) {
			cout << "�����ӵ��д���" << endl;
			return false;
		}
	}
	int left = costheap.size() * 2;				// �жϵ����ӱߺͱ����ӵ�������Ƿ����
	int right = 0;
	for (int i = 0; i < linkcostheap.size(); i++) {
		if (validpoints[i] == false) continue;
		right += linkcostheap[i].size();
	}
	if (left != right) {
		cout << "��������������" << endl;
		return false;
	}
	for (int i = 0; i < costheap.size(); i++)		// �жϱ߶�����ӵ�
		if (costheap[i].validpair.pt1 == costheap[i].validpair.pt2) {
			cout << "pair�������" << endl;
			return false;
		}
	for (int i = 0; i < linkcostheap.size(); i++) {	// �жϵ������ӱ�
		if (validpoints[i] == false)  continue;
		for (int pa = 0; pa < linkcostheap[i].size(); pa++)
			for (int pb = 0; pb < pa; pb++)
				if (linkcostheap[i][pa] == linkcostheap[i][pb]) {
					cout << "������pair" << endl;
					return false;
				}					
	}
	cout << "CheckLinkcostheap ��������ȷ" << endl;
	return true;
}
bool Objects::CheckAll()								// �ж������Ƿ���ȷ
{
	if (CheckLinkfaces() == false)
		return false;
	if (CheckLinkcostheap() == false)
		return false;
	return true;
}

void Objects::Show()									// ��OpenGL��ʾ��άģ��
{
	glEnable(GL_LIGHTING);	
	if (ShowFrame == false) {
		glDisable(GL_LIGHTING);
		if (ShowBlue) {
			glColor3f(0.0, 0.0, 1.0);
			for (int i = 0; i < faces.size(); i++) {
				if (validfaces[i]) {
					glBegin(GL_LINE_LOOP);				
					glVertex3dv(&points[faces[i].pt1][0]);
					glVertex3dv(&points[faces[i].pt2][0]);
					glVertex3dv(&points[faces[i].pt3][0]);
					glEnd();
				}
			}
		}
		glEnable(GL_LIGHTING);
		glBegin(GL_TRIANGLES);
		for (int i = 0; i < faces.size(); i++) {
			if (validfaces[i]) {
				glNormal3dv(&normals[i][0]);
				glVertex3dv(&points[faces[i].pt1][0]);
				glVertex3dv(&points[faces[i].pt2][0]);
				glVertex3dv(&points[faces[i].pt3][0]);
			}
		}
		glEnd();
	}
	else {
		glDisable(GL_LIGHTING);
		glColor3f(1.0, 0.0, 0.0);
		glBegin(GL_LINES);
		glLineWidth(5);
			glVertex3dv(&points[costheap[0].validpair.pt1][0]);
			glVertex3dv(&points[costheap[0].validpair.pt2][0]);
		glEnd();
		glColor3f(1.0, 1.0, 1.0);
		glLineWidth(1);
		for (int i = 0; i < faces.size(); i++) {
			if (validfaces[i]) {
				glBegin(GL_LINE_LOOP);				
				glVertex3dv(&points[faces[i].pt1][0]);
				glVertex3dv(&points[faces[i].pt2][0]);
				glVertex3dv(&points[faces[i].pt3][0]);
				glEnd();
			}
		}
		glEnable(GL_LIGHTING);
	}
	glDisable(GL_LIGHTING);
}
void Objects::Output(char *filename)					// д��obj�ļ�
{
	char space = ' ';
	int current = 0;
	for (int i = 0; i < points.size(); i++) {
		linkvalidpoints.push_back(-1);
		if (validpoints[i]) {
			*linkvalidpoints.rbegin() = ++current;
		}
	}
	this->CheckAll();
	std::ofstream fout(filename);
	fout << '#' << space << sizeofvalidpoints << space << sizeofvalidfaces << std::endl;
	for (int i = 0; i < points.size(); i++)
		if (validpoints[i]) 
			fout << 'v' << space << points[i].x << space << points[i].y << space << points[i].z << std::endl;
	for (int i = 0; i < faces.size(); i++)
		if (validfaces[i])
			fout << 'f' << space << linkvalidpoints[faces[i].pt1] << space << linkvalidpoints[faces[i].pt2] << space << linkvalidpoints[faces[i].pt3] << std::endl;
	fout.close();
}
