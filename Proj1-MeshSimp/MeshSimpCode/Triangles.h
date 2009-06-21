#ifndef _TRIANGLES_H_
#define _TRIANGLES_H_

#include <vector>
#include <algorithm>
#include <GL/gl.h>
#include <GL/glu.h>
#include "Vector3.h"
#include "Vector4.h"

#pragma warning(disable : 4018)
#pragma warning(disable : 4267)

#define OK " OK "

namespace Triangles {
	using namespace std;
	struct triangles {
		int pt1, pt2, pt3;
		triangles() {
			pt1 = pt2 = pt3 = 0;
		}
		triangles(int p1, int p2, int p3) {
			pt1 = p1;
			pt2 = p2;
			pt3 = p3;
		}
	};

	struct pairs {
		int pt1, pt2;
		pairs() {
			pt1 = pt2 = 0;
		}
		pairs(int p1, int p2) {
			pt1 = p1;
			pt2 = p2;
		}
	};

	struct item {
		pairs validpair;
		Specular::Vector3 optimalv3;
		double cost;
		bool operator>(const item &p) const			// item比较大小
		{
			return cost > p.cost;
		}
		bool operator>=(const item &p) const		// item比较大小
		{
			return cost >= p.cost;
		}
		bool operator<(const item &p) const			// item比较大小
		{
			return cost < p.cost;
		}
		bool operator<=(const item &p) const		// item比较大小
		{
			return cost <= p.cost;
		}
		bool operator==(const item &p) const		// item判断是否相等
		{
			return cost == p.cost;
		}
		item &operator=(const item &p)				// item赋值
		{
			validpair = p.validpair;
			optimalv3 = p.optimalv3;
			cost = p.cost;
			return *this;
		}
	};
	typedef vector<int> link;

	class Objects {	
	public:
		vector<Specular::Vector3> points;		// 存储所有的点
		vector<bool> validpoints;				// 判断一个点是否保留

		vector<triangles> faces;				// 存储所有的三角形面的三个顶点序号
		vector<Specular::Vector3> normals;		// 存储所有的三角形面的顶点法向量
		vector<bool> validfaces;				// 判断一个三角形面是否保留

		vector<Matrix::matrix4> Q;				// 存储所有点的Q值
		vector<struct item> costheap;			// 存储每一对边的堆

		vector<link> linkpoints;				// 存储每一个点相连的点

		vector<link> linkcostheap;				// 存储每一个点相连的一对边
		vector<link> linkfaces;					// 存储每一个点相连的三角形
		vector<int>  linkvalidpoints;			// 在输出时记录对应的点

		int sizeofvalidpoints;						// 有效顶点数
		int sizeofvalidfaces;						// 有效面数
		double t;
	public:
		Objects();									// 构造函数，将所有数据清除0
		Objects(const Objects &ano);				// 拷贝构造函数
		Objects(char *filename)	;					// 使用文件名为filename的obj模型文件初始化
		void Clear();								// 清除所有数据
		Specular::Vector3 Normailize(int p);		// 返回第p个三角形的法向量
		void Load(char *filename);					// 加载文件名为filename的obj模型文件
		void SetR(double r);							// 设置阈值为r						
		Matrix::matrix4 ComputeQ(int p);			// 计算第p个点的矩阵Q值
		void ComputeQ();							// 计算所有点的矩阵Q值
		double Det3x3(double *matrix);				// 计算一个3x3的矩阵的行列式值
		struct item GetCost(int pti, int ptj);		// 计算第pti和ptj点组成的pair的cost值
		struct item GetCost(int p);					// 计算costheap中的第p个值
		void InitializeLinks();						// 初始化link相关的数组

		template<class Ty>
		void swap(vector<Ty> v, int i, int j);	// 交换v中的i和j元素

		void swapcostheap(int i, int j);			// 交换costheap的第i和j元素
		void HeapChange(int p);						// 处理当costheap中第p个元素发生改变
		void HeapPush(struct item one);				// 向costheap中插入一个元素one

		struct item HeapPop(int topop = 0);			// 弹出costheap中的第totop个元素

		void MakeHeap();							// 初始化costheap堆
		void DeleteVertex();						// 删除一个pair
		void Stop();
		void InitializeAll(char *filename);			// 初始化全部数据
		void Show();								// 用OpenGL显示三维模型
		void Output(char *filename);				// 写入obj文件	
		bool CheckAll();							// 判断全部是否正确
		bool CheckLinkcostheap();					// 判断linkcostheap的链接是否错误		
		bool CheckLinkfaces();						// 判断linkface的链接是否正确
	};
}

#endif//_TRIANGLES_H_