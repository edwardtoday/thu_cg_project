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
		bool operator>(const item &p) const			// item�Ƚϴ�С
		{
			return cost > p.cost;
		}
		bool operator>=(const item &p) const		// item�Ƚϴ�С
		{
			return cost >= p.cost;
		}
		bool operator<(const item &p) const			// item�Ƚϴ�С
		{
			return cost < p.cost;
		}
		bool operator<=(const item &p) const		// item�Ƚϴ�С
		{
			return cost <= p.cost;
		}
		bool operator==(const item &p) const		// item�ж��Ƿ����
		{
			return cost == p.cost;
		}
		item &operator=(const item &p)				// item��ֵ
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
		vector<Specular::Vector3> points;		// �洢���еĵ�
		vector<bool> validpoints;				// �ж�һ�����Ƿ���

		vector<triangles> faces;				// �洢���е���������������������
		vector<Specular::Vector3> normals;		// �洢���е���������Ķ��㷨����
		vector<bool> validfaces;				// �ж�һ�����������Ƿ���

		vector<Matrix::matrix4> Q;				// �洢���е��Qֵ
		vector<struct item> costheap;			// �洢ÿһ�ԱߵĶ�

		vector<link> linkpoints;				// �洢ÿһ���������ĵ�

		vector<link> linkcostheap;				// �洢ÿһ����������һ�Ա�
		vector<link> linkfaces;					// �洢ÿһ����������������
		vector<int>  linkvalidpoints;			// �����ʱ��¼��Ӧ�ĵ�

		int sizeofvalidpoints;						// ��Ч������
		int sizeofvalidfaces;						// ��Ч����
		double t;
	public:
		Objects();									// ���캯�����������������0
		Objects(const Objects &ano);				// �������캯��
		Objects(char *filename)	;					// ʹ���ļ���Ϊfilename��objģ���ļ���ʼ��
		void Clear();								// �����������
		Specular::Vector3 Normailize(int p);		// ���ص�p�������εķ�����
		void Load(char *filename);					// �����ļ���Ϊfilename��objģ���ļ�
		void SetR(double r);							// ������ֵΪr						
		Matrix::matrix4 ComputeQ(int p);			// �����p����ľ���Qֵ
		void ComputeQ();							// �������е�ľ���Qֵ
		double Det3x3(double *matrix);				// ����һ��3x3�ľ��������ʽֵ
		struct item GetCost(int pti, int ptj);		// �����pti��ptj����ɵ�pair��costֵ
		struct item GetCost(int p);					// ����costheap�еĵ�p��ֵ
		void InitializeLinks();						// ��ʼ��link��ص�����

		template<class Ty>
		void swap(vector<Ty> v, int i, int j);	// ����v�е�i��jԪ��

		void swapcostheap(int i, int j);			// ����costheap�ĵ�i��jԪ��
		void HeapChange(int p);						// ����costheap�е�p��Ԫ�ط����ı�
		void HeapPush(struct item one);				// ��costheap�в���һ��Ԫ��one

		struct item HeapPop(int topop = 0);			// ����costheap�еĵ�totop��Ԫ��

		void MakeHeap();							// ��ʼ��costheap��
		void DeleteVertex();						// ɾ��һ��pair
		void Stop();
		void InitializeAll(char *filename);			// ��ʼ��ȫ������
		void Show();								// ��OpenGL��ʾ��άģ��
		void Output(char *filename);				// д��obj�ļ�	
		bool CheckAll();							// �ж�ȫ���Ƿ���ȷ
		bool CheckLinkcostheap();					// �ж�linkcostheap�������Ƿ����		
		bool CheckLinkfaces();						// �ж�linkface�������Ƿ���ȷ
	};
}

#endif//_TRIANGLES_H_