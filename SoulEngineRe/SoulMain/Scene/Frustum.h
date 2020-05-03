#pragma once
#include "../Core/Mathematics/SMatrix4x4.h"
#include "../Core/Mathematics/SVector4.h"
#include "../Core/Mathematics/SVector3.h"
namespace Soul
{
	class Frustum
	{
	public:
		Frustum();

		//������Ļ����ȣ�ͶӰ�����������������Ӧ���ӽ����6��ƽ��
		void BuildFrustum(const Core::SMatrix4x4& projMatrix, const Core::SMatrix4x4& viewMatrix);

		//�ж�һ�����Ƿ����ӽ�����
		bool CheckPoint(float x, float y, float z);

		//�ж�һ���������Ƿ����ӽ����ڣ��õ��ǰ�Χ��İ취
		bool CheckCube(float xCenter, float yCenter, float zCenter, float radius);

		//�ж�һ�������Ƿ����ӽ�����,�õ��ǰ�Χ��İ취
		bool CheckSphere(float xCenter, float yCenter, float zCenter, float radius);

		//�ж�һ���������Ƿ����ӽ����ڣ��õ�Ҳ�ǰ�Χ��ĵķ���
		bool CheckRectangle(float xCenter, float yCenter, float zCenter, float xSize, float ySize, float zSize);

		//�ж�һ���������Ƿ����ӽ����ڣ��õ���8��ķ���
		bool CheckRectangle2(float, float, float, float, float, float);

	private:
		bool mFirstBuild;
		Core::SMatrix4x4 mCacheViewMatrix;
		Core::SMatrix4x4 mCacheProjMatrix;
		Core::SVector4 mPlane[6];
	};
}