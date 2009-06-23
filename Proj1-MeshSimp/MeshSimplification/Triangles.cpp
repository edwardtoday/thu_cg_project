#include <windows.h>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <sstream>
#include <string>
#include "Triangles.h"
using namespace std;
using namespace Triangles;

int globalfirsttime = 0;							// 以下几行为是否输出调试信息
int toshow = 0;
bool show = false;
bool outputfinal = true;

extern bool ShowFrame;								// 以下两行位确定如何显示模型
extern bool ShowBlue;
Objects::Objects()									// 构造函数，将所有数据清除0
{
	this->Clear();	
}
Objects::Objects(const Objects &ano)					// 拷贝构造函数
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
Objects::Objects(char *filename)						// 使用文件名为filename的obj模型文件初始化
{
	this->Load(filename);
}
void Objects::Clear()								// 清除所有数据
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
Specular::Vector3 Objects::Normailize(int p)			// 返回第p个三角形的法向量
{
	Specular::Vector3 temp;
	Specular::Vector3 v1, v2;
	v1 = points[faces[p].pt1] - points[faces[p].pt2];
	v2 = points[faces[p].pt1] - points[faces[p].pt3];
	temp = v1 ^ v2;							// 使用两条边对应的向量叉积作为法向量
	temp.Normalize();
	return temp;		
}
void Objects::Load(char *filename)					// 加载文件名为filename的obj模型文件
{
	char c;
	ifstream fin(filename);
	int temppoints, tempfaces;
	this->Clear();							// 清除所有数据
	char tempchars[100];

	fin >> c;
	while (c != 'v') {
		fin.getline(tempchars, 100);
		fin >> c;
	}

	while(c == 'v') {
		Specular::Vector3 temp;
		fin >> temp.x >> temp.y >> temp.z;	// 读入顶点坐标
		points.push_back(temp);				// 添加新顶点
		validpoints.push_back(true);		// 设置新加顶点的有效
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
		fin >> temp.pt1 >> temp.pt2 >> temp.pt3;			// 读入三角形顶点标号
		if (temp.pt1 == 0 || temp.pt2 == 0 || temp.pt3 == 0) break;
		temp.pt1--;
		temp.pt2--;
		temp.pt3--;
		faces.push_back(temp);								// 添加新三角形
		validfaces.push_back(true);							// 设置新三角形有效
		sizeofvalidfaces++;
		fin >> c;
		while (c != 'f' && c != 'g') {						// 忽略其他字母表示的含义
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
	for (int i = 0; i < faces.size(); i++) {				// 设置三角形法向量
		normals.push_back(Normailize(i));
	}
	cout << "normals.size() = " << normals.size() << endl;
	for (int i = 0; i < normals.size(); i++)				// 单位化三角形法向量
		normals[i].Normalize();
}

void Objects::InitializeLinks()						// 初始化link相关的数组
{
	link null;
	for (int i = 0; i < points.size(); i++)					// 设置容量
		linkpoints.push_back(null);
	cout << "linkpoints.size() = " << linkpoints.size() << endl;
	cout << "设置容量完成" << OK << endl;
	for (int i = 0; i < faces.size(); i++) {				// 设置连接关系
		linkpoints[faces[i].pt1].push_back(faces[i].pt2);
		linkpoints[faces[i].pt1].push_back(faces[i].pt3);

		linkpoints[faces[i].pt2].push_back(faces[i].pt1);
		linkpoints[faces[i].pt2].push_back(faces[i].pt3);

		linkpoints[faces[i].pt3].push_back(faces[i].pt1);
		linkpoints[faces[i].pt3].push_back(faces[i].pt2);
	}

	cout << "链接完成" << OK << endl;
	for (int i = 0; i < points.size(); i++)				// 将与每一个顶点相连顶点依顶点标号排序
		sort(linkpoints[i].begin(), linkpoints[i].end());
	cout << "排序完成" << OK << endl;
	for (int i = 0; i < points.size(); i++) {			// 因为每一个条边计算两次，因此顶点会出现两次
		for (int j = 0; j < linkpoints[i].size(); j = j + 2) {
			linkpoints[i][j / 2] = linkpoints[i][j];
		}
		int templen = linkpoints[i].size();
		for (int j = 0; j < templen / 2; j++)
			linkpoints[i].pop_back();
	}
	cout << "点链接完成" << OK << endl;

	for (int i = 0; i < points.size(); i++)				// 增加容量
		linkfaces.push_back(null);
	for (int i = 0; i < faces.size(); i++) {			// 建立顶点与三角形的链接关系
		linkfaces[faces[i].pt1].push_back(i);
		linkfaces[faces[i].pt2].push_back(i);
		linkfaces[faces[i].pt3].push_back(i);
	}
	cout << "边链接完成" << OK << endl;
}

void Objects::SetR(double r)							// 设置阈值为r						
{
	this->t = t;
}
Matrix::matrix4 Objects::ComputeQ(int p)				// 计算第p个点的矩阵Q值
{
	Q[p] = Matrix::Zero;								// 初始设置为0矩阵	
	Matrix::matrix4 temp;
	for (int i = 0; i < linkfaces[p].size(); i++) {
		Matrix::Vector4 plane;
		plane.x = normals[linkfaces[p][i]].x;				// 以下四行为计算平面
		plane.y = normals[linkfaces[p][i]].y;
		plane.z = normals[linkfaces[p][i]].z;
		plane.p = -points[p] * normals[linkfaces[p][i]];
		temp = plane ^ plane;								// 计算平面对应矩阵
		Q[p] = Q[p] + temp;
	}
	return Q[p];
}
void Objects::ComputeQ()								// 计算所有点的矩阵Q值
{	
	for (int i = 0; i < points.size(); i++) {
		Q.push_back(Matrix::Zero);
	}
	cout << "Q.size() = " << Q.size() << endl;
	for (int i = 0; i < points.size(); i++) {
		ComputeQ(i);
	}
}
double Objects::Det3x3(double *matrix)					// 计算一个3x3的矩阵的行列式值
{											// 直接用行列式展开计算
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
struct item Objects::GetCost(int pti, int ptj)		// 计算第pti和ptj点组成的pair的cost值
{
	Matrix::matrix4 m;
	m = Q[pti] + Q[ptj];

	double Qsolve[4][10];									// 以下几行为用Cramer法则计算
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
	if (temp3x3 != 0) {											// 防止除数为0
		optimalv4.x = -Det3x3(Qsolve[1]) / Det3x3(Qsolve[0]);
		optimalv4.y = -Det3x3(Qsolve[2]) / Det3x3(Qsolve[0]);
		optimalv4.z = -Det3x3(Qsolve[3]) / Det3x3(Qsolve[0]);
	}
	else {
		optimalv4.x = optimalv4.y = optimalv4.z = 0;
	}
	optimalv4.p = 1;

	double cost;
	cost = optimalv4 * m * optimalv4;						// 计算cost = vbest^T * m * vbest
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
struct item Objects::GetCost(int p)					// 计算costheap中的第p个值
{
	costheap[p] = GetCost(costheap[p].validpair.pt1, costheap[p].validpair.pt2);
	return costheap[p];
}

template<class Ty>
void Objects::swap(std::vector<Ty> v, int i, int j)	// 交换v中的i和j元素
{
	Ty temp = v[j];
	v[j] = v[i];
	v[i] = temp;			
}

void Objects::swapcostheap(int i, int j)				// 交换costheap的第i和j元素
{
	int t = -1;
	
	for (int k = 0; k < linkcostheap[costheap[i].validpair.pt1].size(); k++) {
		if (linkcostheap[costheap[i].validpair.pt1][k] == i)
			linkcostheap[costheap[i].validpair.pt1][k] = t;		// 更改链接关系
	}
	for (int k = 0; k < linkcostheap[costheap[i].validpair.pt2].size(); k++) {
		if (linkcostheap[costheap[i].validpair.pt2][k] == i)
			linkcostheap[costheap[i].validpair.pt2][k] = t;		// 更改链接关系
	}
	for (int k = 0; k < linkcostheap[costheap[j].validpair.pt1].size(); k++) {
		if (linkcostheap[costheap[j].validpair.pt1][k] == j)
			linkcostheap[costheap[j].validpair.pt1][k] = i;		// 更改链接关系
	}
	for (int k = 0; k < linkcostheap[costheap[j].validpair.pt2].size(); k++) {
		if (linkcostheap[costheap[j].validpair.pt2][k] == j)
			linkcostheap[costheap[j].validpair.pt2][k] = i;		// 更改链接关系
	}
	for (int k = 0; k < linkcostheap[costheap[i].validpair.pt1].size(); k++) {
		if (linkcostheap[costheap[i].validpair.pt1][k] == t)
			linkcostheap[costheap[i].validpair.pt1][k] = j;		// 更改链接关系
	}
	for (int k = 0; k < linkcostheap[costheap[i].validpair.pt2].size(); k++) {
		if (linkcostheap[costheap[i].validpair.pt2][k] == t)
			linkcostheap[costheap[i].validpair.pt2][k] = j;		// 更改链接关系
	}
	item temp;
	temp = costheap[i];
	costheap[i] = costheap[j];
	costheap[j] = temp;	
}
void Objects::HeapChange(int p)						// 处理当costheap中第p个元素发生改变
{
	if (p >= costheap.size())
		return;
	if (costheap.size() == 1) return;
	if (p != 0 && costheap[p] <= costheap[(p - 1) / 2])	// p比其父节点小
	{
		while (1) {
			if (p > 0) {
				if (costheap[p] <= costheap[(p - 1) / 2]) {	// 如果每一次都比父节点小
					swapcostheap(p, (p - 1) / 2);			// 与父节点交换
					p = (p - 1) / 2;
					continue;
				}
			}
			break;
		}
	}
	else {										// p比其子节点大
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
			if (2 * p + 2 < costheap.size()) {	// 左右子节都存在
				if ((costheap[2 * p + 1] <= costheap[p]) && (costheap[2 * p + 1] <= costheap[2 * p + 2])) {		// 左子节点最小
					swapcostheap(p, 2 * p + 1);		// 与左子节点交换				
					p = 2 * p + 1;
					continue;
				}
				else if ((costheap[2 * p + 2] <= costheap[p]) && (costheap[2 * p + 2] <= costheap[2 * p + 1])) {	// 右子节点最小		
					swapcostheap(p, 2 * p + 2);		// 与右子节点交换
					p = 2 * p + 2;
					continue;
				}
			} 
			else if (2 * p + 1 < costheap.size()) {			// 只存在左节点
				if (costheap[2 * p + 1] <= costheap[p]) {	// 比左子节点大
					swapcostheap(p, 2 * p + 1);
					p = 2 * p + 1;
					continue;
				}
			}
			break;
		}
	}
}
void Objects::HeapPush(struct item one)				// 向costheap中插入一个元素one
{
	costheap.push_back(one);
	linkcostheap[one.validpair.pt1].push_back(costheap.size() - 1);	// 建立顶点到costheap的链接关系
	linkcostheap[one.validpair.pt2].push_back(costheap.size() - 1); // 建立顶点到costheap的链接关系
	HeapChange(costheap.size() - 1);							// 修改堆
}

struct item Objects::HeapPop(int topop)					// 弹出costheap中的第totop个元素
{
	struct item best = costheap[topop];
	if (show) {											// 输出调试信息
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
	swapcostheap(topop, costheap.size() - 1);	// 设置弹出节点为最后一个元素
	if (show) {									// 输出调试信息
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
	costheap.pop_back();					// 将最后一个元素清除

	HeapChange(topop);						// 修改堆
	return best;
}

void Objects::MakeHeap()								// 初始化costheap堆
{
	bool firsttime = true;
	link null;
	for (int i = 0; i < points.size(); i++)
		linkcostheap.push_back(null);
	for (int i = 0; i < points.size(); i++) {
		for (int j = 0; j < linkpoints[i].size(); j++) 
			if (i < linkpoints[i][j]) {								// 确保每一个pair只计算一次
				struct item temp = GetCost(i, linkpoints[i][j]);	// 获得一个pair

				HeapPush(temp);										// 将pair插入堆中				

			}
	}
	cout << "linkcostheap.size() = " << linkcostheap.size() << endl;
// 以下为测试小根堆的代码
	for (int i = 1; i < costheap.size(); i++) 
		if (costheap[i] < costheap[(i - 1) / 2]) 
			cout << "Heap Error!" << endl;
}

void Objects::InitializeAll(char *filename)				// 初始化全部数据
{
	this->Stop();
	cout << filename << endl;

// 加载文件
	this->Load(filename);
	cout << "File Load OK" << endl;
	cout << "InitializeLinks......" << endl;

// 初始化链接
	this->InitializeLinks();
	cout << "InitializeLinks OK" << endl;
	cout << "ComputeQ......" << endl;

// 生成Q
	this->ComputeQ();
	cout << "ComputeQ OK" << endl;
	cout << "MakeHeap......" << endl;

// 生成堆
	this->MakeHeap();
	cout << "MakeHeap OK" << endl;

// 以下为测试堆与点链接正确性的代码
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
删除pair步骤：
	1、获得并且添加新顶点			对应的数组－points
	2、将删除的顶点设为无效		对应的数组－validpoints
	3、将删除的面设为无效			对应的数组－validfaces

	4、设置新的顶点相连的面		对应的数组－linkfaces
	5、设置修改的面的新顶点		对应的数组－faces
	6、设置修改的面的法向量		对应的数组－normals
	
	7、设置新的顶点的Q值			对应的数组－Q

	8、设置新的顶点连接的pair		对应的数组－linkcostheap
	9、修改堆中的cost并且更新		对应的数组－costheap
**********************************/
void Objects::DeleteVertex()							// 删除一个pair
{
	// 获得删除的pair
	struct item vbest = this->HeapPop();

	// Step 1 - 获得并且添加新顶点	对应的数组－points	
	points.push_back(vbest.optimalv3);
	validpoints.push_back(true);
	sizeofvalidpoints++;

	// Step 2 - 将删除的顶点设为无效	对应的数组－validpoints
	int todelete1 = vbest.validpair.pt1;		// 更换变量名，以简化程序
	int todelete2 = vbest.validpair.pt2;		// 更换变量名，以简化程序
	validpoints[todelete1] = false;
	validpoints[todelete2] = false;	
	sizeofvalidpoints--;
	sizeofvalidpoints--;

	// Step 3 - 将删除的面设为无效	对应的数组－validfaces
	std::vector<int> sharefaces;
	for (int i = 0; i < linkfaces[todelete1].size(); i++)
		for (int j = 0; j < linkfaces[todelete2].size(); j++)
			if (linkfaces[todelete1][i] == linkfaces[todelete2][j]) {	// 如果一个面使用pair中的两个顶点
				validfaces[linkfaces[todelete1][i]] = false;			// 则一定要被删除
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
	// Step 4 - 设置新的顶点相连的面		对应的数组－linkfaces
	// Step 5 - 设置修改的面的新顶点		对应的数组－faces
	// Step 6 - 设置修改的面的法向量		对应的数组－normals	
	link nulllinkface;
	linkfaces.push_back(nulllinkface);
	for (int i = 0; i < linkfaces[todelete1].size(); i++) {					// 处理todelete1的链接关系
		if (validfaces[linkfaces[todelete1][i]]) {							// 如果链接三角形有效
			//cout << "linkfaces[todelete1][" << i << "] = " << linkfaces[todelete1][i] << endl;
			linkfaces.rbegin()->push_back(linkfaces[todelete1][i]);			// 新增顶点链接到此三角形
			if (faces[linkfaces[todelete1][i]].pt1 == todelete1) {			// 以下几行为将有效三角形的顶点更新为新增顶点
				faces[linkfaces[todelete1][i]].pt1 = linkfaces.size() - 1;
			} else if (faces[linkfaces[todelete1][i]].pt2 == todelete1) {
				faces[linkfaces[todelete1][i]].pt2 = linkfaces.size() - 1;
			} else if (faces[linkfaces[todelete1][i]].pt3 == todelete1) {
				faces[linkfaces[todelete1][i]].pt3 = linkfaces.size() - 1;
			}
			normals[linkfaces[todelete1][i]] = Normailize(linkfaces[todelete1][i]);	// 重新设置三角形法向量
		}
	}
	for (int i = 0; i < linkfaces[todelete2].size(); i++) {					// 处理todelete2的链接关系
		if (validfaces[linkfaces[todelete2][i]]) {							// 如果链接三角形有效
			linkfaces.rbegin()->push_back(linkfaces[todelete2][i]);			// 新增顶点链接到此三角形
			if (faces[linkfaces[todelete2][i]].pt1 == todelete2) {			// 以下几行为将有效三角形的顶点更新为新增顶点
				faces[linkfaces[todelete2][i]].pt1 = linkfaces.size() - 1;
			} else if (faces[linkfaces[todelete2][i]].pt2 == todelete2) {
				faces[linkfaces[todelete2][i]].pt2 = linkfaces.size() - 1;
			} else if (faces[linkfaces[todelete2][i]].pt3 == todelete2) {
				faces[linkfaces[todelete2][i]].pt3 = linkfaces.size() - 1;
			}
			normals[linkfaces[todelete2][i]] = Normailize(linkfaces[todelete2][i]);	// 重新设置三角形法向量
		}
	}
	// Step 7 - 设置新的顶点的Q值			对应的数组－Q
	Q.push_back(Matrix::Zero);
	ComputeQ(Q.size() - 1);

	// Step 8 - 设置新的顶点连接的pair	对应的数组－linkcostheap
	// Step 9 - 修改堆中的cost并且更新	对应的数组－costheap	
	link nullpair;
	linkcostheap.push_back(nullpair);
	item temp;
	vector<int> ano;

	for (int i = 0; i < linkcostheap[todelete1].size(); i++) {					// 将与delete1相连的边与新增点相连
		int anopoint = 0;
		if (costheap[linkcostheap[todelete1][i]].validpair.pt1 == todelete1)
			anopoint = costheap[linkcostheap[todelete1][i]].validpair.pt2;
		else 
			anopoint = costheap[linkcostheap[todelete1][i]].validpair.pt1;
		if (validpoints[anopoint]) {
			ano.push_back(anopoint);
		}
	}

	for (int i = 0; i < linkcostheap[todelete2].size(); i++) {					// 将与delete2相连的边与新增点相连
		int anopoint = 0;
		if (costheap[linkcostheap[todelete2][i]].validpair.pt1 == todelete2)
			anopoint = costheap[linkcostheap[todelete2][i]].validpair.pt2;
		else 
			anopoint = costheap[linkcostheap[todelete2][i]].validpair.pt1;
		if (validpoints[anopoint]) {
			ano.push_back(anopoint);
		}
	}

	sort(ano.begin(), ano.end());												// 排序，以消除重边
	int p = 1;
	for (int i = 1; i < ano.size(); i++)
		if (ano[i] != ano[i - 1]) {
			ano[p] = ano[i];
			p++;
		}
	ano.erase(ano.begin() + p, ano.end());

	int tempsize = linkcostheap[todelete1].size();		// 清除与delete1相连的边
	for (int i = 0; i < tempsize; i++) {
		HeapPop(linkcostheap[todelete1][0]);
	}
	tempsize = linkcostheap[todelete2].size();			// 清除与delete2相连的边
	for (int i = 0; i < tempsize; i++) {
		HeapPop(linkcostheap[todelete2][0]);
	}

	for (int i = 0; i < ano.size(); i++) {				// 添加与新增节点相连的边
		temp = GetCost(points.size() - 1, ano[i]);
		HeapPush(temp);
	}

	if (!outputfinal) {
		this->CheckAll();
		cout << "顶点总数为" << sizeofvalidpoints << endl;
		cout << "面总数为" << sizeofvalidfaces << endl;
	}
}
void Objects::Stop() {
	//std::cout << "Run to Stop" << std::endl;
}
bool Objects::CheckLinkfaces() {	// 检测面与点之间的链接关系是否正确

	for (int i = 0; i < linkfaces.size(); i++) {	// 判断点相连的面是否与点相连
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
					cout << "三角形链接错误！" << endl;
					return false;
				}
			}
		}
	}
	for (int i = 0; i < faces.size(); i++) {	// 判断三角形链接的点是否都是有效点
		if (validfaces[i] == false) continue;
		bool ok = true;		
		if (validpoints[faces[i].pt1] == false) ok = false;
		if (validpoints[faces[i].pt2] == false) ok = false;
		if (validpoints[faces[i].pt3] == false) ok = false;
		if (ok == false) {
			cout << i << ' ' << faces[i].pt1 << ' ' << faces[i].pt2 << ' ' << faces[i].pt3 << endl;
			cout << "出现三角形虚链接" << endl;
			return false;
		}
	}
	for (int i = 0; i < linkfaces.size(); i++) {	// 判断点链接的三角形是否都是有效三角形
		if (validpoints[i] == false) continue;
		bool ok = true;
		for (int j = 0; j < linkfaces[i].size(); j++)
			if (validfaces[linkfaces[i][j]] == false) {
				cout << "point = " << i << endl;
				cout << "点虚连接三角形" << endl;
				ok = false;
				return false;
			}
	}

	for (int i = 0; i < faces.size(); i++) {	// 判断三角形链接的点是否链接上三角形
		if (validfaces[i] == false) continue;
		bool ok = false;
		for (int j = 0; j < linkfaces[faces[i].pt1].size(); j++)
			if (linkfaces[faces[i].pt1][j] == i)
				ok = true;
		if (ok == false) {
			cout << "三角形链接点有错误！" << endl;
			return false;
		}
		for (int j = 0; j < linkfaces[faces[i].pt2].size(); j++)
			if (linkfaces[faces[i].pt2][j] == i)
				ok = true;
		if (ok == false) {
			cout << "三角形链接点有错误！" << endl;
			return false;
		}
		for (int j = 0; j < linkfaces[faces[i].pt3].size(); j++)
			if (linkfaces[faces[i].pt3][j] == i)
				ok = true;
		if (ok == false) {
			cout << "三角形链接点有错误！" << endl;
			return false;
		}
	}	

	int left = 0, right = 0;				// 判断三角形与点之间的链接关系总数是否相等
	for (int i = 0; i < faces.size(); i++)
		if (validfaces[i]) left++;
	left = left * 3;	
	for (int i =0; i < linkfaces.size(); i++)
		if (validpoints[i])
			right += linkfaces[i].size();
	if (left != right) {
		cout << "三角形链接数量错误！" << endl;
		return false;
	}

	for (int i = 0; i < linkfaces.size(); i++) {	// 判断一个点是否多次链接同一个三角形
		if (validpoints[i] == false) continue;
		for (int pa = 0; pa < linkfaces[i].size(); pa++)
			for (int pb = 0; pb < pa; pb++)
				if (linkfaces[i][pa] == linkfaces[i][pb]) {
					cout << "点多次连三角形" << endl;
					return false;
				}
	}
	for (int i = 0; i < faces.size(); i++) {		// 判断一个三角形是否多次链接同一个点
		if (faces[i].pt1 == faces[i].pt2) {
			cout << "三角形多次连点" << endl;
			return false;
		}
		if (faces[i].pt1 == faces[i].pt3) {
			cout << "三角形多次连点" << endl;
			return false;
		}
		if (faces[i].pt3 == faces[i].pt2) {
			cout << "三角形多次连点" << endl;
			return false;
		}
	}

	int numberoffaces = 0;							// 判断三角形个数是否正确
	for (int i = 0; i < faces.size(); i++) 
		if (validfaces[i]) 
			numberoffaces++;
	if (numberoffaces != sizeofvalidfaces) {
		cout << "三角形个数错误！" << endl;
		return false;
	}
	cout << "CheckLinkfaces 三角形链接正确" << endl;
	return true;
}

bool Objects::CheckLinkcostheap()						// 判断linkcostheap的链接是否错误
{
	for (int i = 1; i < costheap.size(); i++)			// 判断linkcostheap是否是小根堆
		if (costheap[i] < costheap[(i - 1) / 2]) {
			cout << "i: " << costheap[i].cost << ' ' << "(i - 1) / 2: " << costheap[(i - 1) / 2].cost << endl;
			cout << "i = " << i << endl;
			cout << "小根堆错误！" << endl;
			return false;			
		}

	for (int i = 0; i < linkcostheap.size(); i++) {		// 判断点链接的边是否链接点
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
				cout << "堆链接错误！" << endl;
				return false;
			}
		}
	}
	int icon = 0;
	for (int i = 0; i < costheap.size(); i++)		// 判断边链接的点是否有效
		if (validpoints[costheap[i].validpair.pt1] == false || validpoints[costheap[i].validpair.pt2] == false){
			cout << "pair = " << i << endl;
			if (validpoints[costheap[i].validpair.pt1] == false)
				cout << "point = " << costheap[i].validpair.pt1 << endl;
			if (validpoints[costheap[i].validpair.pt2] == false)
				cout << "point = " << costheap[i].validpair.pt2 << endl;
			icon++;
		}
	if (icon != 0) {
		cout << "出现虚连接！" << icon << "处" << endl;
		return false;
	}
	for (int i = 0; i < costheap.size(); i++) {		// 判断边链接的点是否链接上边
		bool ok = false;
		for (int j = 0; j < linkcostheap[costheap[i].validpair.pt1].size(); j++)
			if (linkcostheap[costheap[i].validpair.pt1][j] == i)
				ok = true;
		if (ok == false) {
			cout << "堆链接点有错误！" << endl;
			return false;
		}
		ok = false;
		for (int j = 0; j < linkcostheap[costheap[i].validpair.pt2].size(); j++)
			if (linkcostheap[costheap[i].validpair.pt2][j] == i)
				ok = true;
		if (ok = false) {
			cout << "堆链接点有错误！" << endl;
			return false;
		}
	}
	int left = costheap.size() * 2;				// 判断点链接边和边链接点的总数是否相等
	int right = 0;
	for (int i = 0; i < linkcostheap.size(); i++) {
		if (validpoints[i] == false) continue;
		right += linkcostheap[i].size();
	}
	if (left != right) {
		cout << "堆链接数量错误！" << endl;
		return false;
	}
	for (int i = 0; i < costheap.size(); i++)		// 判断边多次链接点
		if (costheap[i].validpair.pt1 == costheap[i].validpair.pt2) {
			cout << "pair多次连点" << endl;
			return false;
		}
	for (int i = 0; i < linkcostheap.size(); i++) {	// 判断点多次链接边
		if (validpoints[i] == false)  continue;
		for (int pa = 0; pa < linkcostheap[i].size(); pa++)
			for (int pb = 0; pb < pa; pb++)
				if (linkcostheap[i][pa] == linkcostheap[i][pb]) {
					cout << "点多次连pair" << endl;
					return false;
				}					
	}
	cout << "CheckLinkcostheap 堆链接正确" << endl;
	return true;
}
bool Objects::CheckAll()								// 判断链接是否正确
{
	if (CheckLinkfaces() == false)
		return false;
	if (CheckLinkcostheap() == false)
		return false;
	return true;
}

void Objects::Show()									// 用OpenGL显示三维模型
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
void Objects::Output(char *filename)					// 写入obj文件
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
