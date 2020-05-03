#include "Frustum.h"

namespace Soul
{
	Frustum::Frustum()
		:
		mFirstBuild(true),
		mScreenDepth(0)
	{
	}

	//������Ļ����ȣ�ͶӰ�������������������ռ����Ӧ���ӽ����6��ƽ��
	void Frustum::BuildFrustum(float sceneDepth, const Core::SMatrix4x4& projMatrix, const Core::SMatrix4x4& viewMatrix)
	{
		mViewMatrix = viewMatrix;
		mProjMatrix = projMatrix;
		mScreenDepth = sceneDepth;

		// ������ǵ�һ�ι������Һ��ϴεľ�����ͬ�Ͳ�����
		if (!mFirstBuild && (mViewMatrix == viewMatrix || mProjMatrix == projMatrix))
		{
			return;
		}

		float zMinimum, r;
		Core::SMatrix4x4 matrix;
		Core::SMatrix4x4 tempMatrix = mProjMatrix;

		//�����ӽ�����ü���ľ���
		zMinimum = -tempMatrix.mat[3][2] / tempMatrix.mat[2][2];
		r = mScreenDepth / (mScreenDepth - zMinimum);

		tempMatrix.mat[2][2] = r;
		tempMatrix.mat[3][2] = -r * zMinimum;

		//����������ͶӰ��������ӽ������
		matrix = mViewMatrix * tempMatrix;;

		//�����ӽ���Ľ��ü���
		Core::SVector4 nearPlane;
		nearPlane.x = matrix.mat[0][3] + matrix.mat[0][2];//_14 _13
		nearPlane.y = matrix.mat[1][3] + matrix.mat[1][2];//_24 _23
		nearPlane.z = matrix.mat[2][3] + matrix.mat[2][2];//_34 _33
		nearPlane.w = matrix.mat[3][3] + matrix.mat[3][2];//_44 _43
		mPlane[0] = nearPlane;
		mPlane[0].PlaneNormalize();

		//�����ӽ����Զ�ü���
		Core::SVector4 FarPlane;
		FarPlane.x = matrix.mat[0][3] - matrix.mat[0][2];//_14_13
		FarPlane.y = matrix.mat[1][3] - matrix.mat[1][2];//_24_23
		FarPlane.z = matrix.mat[2][3] - matrix.mat[2][2];//_34_33
		FarPlane.w = matrix.mat[3][3] - matrix.mat[3][2];//_44_43
		mPlane[1] = FarPlane;
		mPlane[1].PlaneNormalize();

		//�����ӽ������ü���(XZ��)
		Core::SVector4 LeftPlane;
		LeftPlane.x = matrix.mat[0][3] + matrix.mat[0][0];//_14_11
		LeftPlane.y = matrix.mat[1][3] + matrix.mat[1][0];//_24_21
		LeftPlane.z = matrix.mat[2][3] + matrix.mat[2][0];//_34_31
		LeftPlane.w = matrix.mat[3][3] + matrix.mat[3][0];//_44_41
		mPlane[2] = LeftPlane;
		mPlane[2].PlaneNormalize();

		//�����ӽ�����Ҳü���(XZ��)
		Core::SVector4 RightPlane;
		RightPlane.x = matrix.mat[0][3] - matrix.mat[0][0];//_14_11
		RightPlane.y = matrix.mat[1][3] - matrix.mat[1][0];//_24_21
		RightPlane.z = matrix.mat[2][3] - matrix.mat[2][0];//_34_31
		RightPlane.w = matrix.mat[3][3] - matrix.mat[3][0];//_44_41
		mPlane[3] = RightPlane;
		mPlane[3].PlaneNormalize();

		//�����ӽ���Ķ��ü���(YZ��)
		Core::SVector4 TopPlane;
		TopPlane.x = matrix.mat[0][3] - matrix.mat[0][1];//_14_12
		TopPlane.y = matrix.mat[1][3] - matrix.mat[1][1];//_24_22
		TopPlane.z = matrix.mat[2][3] - matrix.mat[2][1];//_34_32
		TopPlane.w = matrix.mat[3][3] - matrix.mat[3][1];//_44_42
		mPlane[4] = TopPlane;
		mPlane[4].PlaneNormalize();

		//�����ӽ���ĵײü���(YZ��)
		Core::SVector4 BottomPlane;
		BottomPlane.x = matrix.mat[0][3] + matrix.mat[0][1];//_14_12
		BottomPlane.y = matrix.mat[1][3] + matrix.mat[1][1];//_24_22
		BottomPlane.z = matrix.mat[2][3] + matrix.mat[2][1];//_34_32
		BottomPlane.w = matrix.mat[3][3] + matrix.mat[3][1];//_44_42
		mPlane[5] = BottomPlane;
		mPlane[5].PlaneNormalize();
	}

	//�ж�һ�����Ƿ����ӽ�����
	//һ���������ӽ���6��������棬��õ����ӽ�����
	bool Frustum::CheckPoint(float x, float y, float z)
	{
		Core::SVector3 Point = Core::SVector3(x, y, z);
		for (int i = 0; i < 6; ++i)
		{
			//��˽��
			float DotEnd = mPlane[i].PlaneEquationGetResultFrom(Point);
			if (DotEnd < 0.0f)
			{
				return false;
			}
		}
		return true;
	}

	//�ж�һ���������Ƿ����ӽ����ڣ�radius=�����α߳�/2
	//��һ�����㷨��ֻҪ�����������8���㶼λ���ӽ���6�����е�����һ����ĸ��棬��ô�������λ���ӽ�����
	//�������һ�������ӽ�����(�ཻ������ȫλ���ӽ���)
	bool Frustum::CheckCube(float xCenter, float yCenter, float zCenter, float radius)
	{
		Core::SVector3 Point[8];
		float DotEnd;
		Point[0] = Core::SVector3(xCenter - radius, yCenter - radius, zCenter - radius);
		Point[1] = Core::SVector3(xCenter + radius, yCenter - radius, zCenter - radius);
		Point[2] = Core::SVector3(xCenter - radius, yCenter + radius, zCenter - radius);
		Point[3] = Core::SVector3(xCenter - radius, yCenter - radius, zCenter + radius);
		Point[4] = Core::SVector3(xCenter + radius, yCenter + radius, zCenter - radius);
		Point[5] = Core::SVector3(xCenter + radius, yCenter - radius, zCenter + radius);
		Point[6] = Core::SVector3(xCenter - radius, yCenter + radius, zCenter + radius);
		Point[7] = Core::SVector3(xCenter + radius, yCenter + radius, zCenter + radius);

		for (int i = 0; i < 6; ++i)
		{
			/*�ֱ�����������ÿ�����Ƿ����ӽ�����*/

			/*һ*/
			DotEnd = mPlane[i].PlaneEquationGetResultFrom(Point[0]);
			if (DotEnd >= 0)
			{
				continue;
			}
			/*��*/
			DotEnd = mPlane[i].PlaneEquationGetResultFrom(Point[1]);
			if (DotEnd >= 0)
			{
				continue;
			}
			/*��*/
			DotEnd = mPlane[i].PlaneEquationGetResultFrom(Point[2]);
			if (DotEnd >= 0)
			{
				continue;
			}
			/*��*/
			DotEnd = mPlane[i].PlaneEquationGetResultFrom(Point[3]);
			if (DotEnd >= 0)
			{
				continue;
			}
			/*��*/
			DotEnd = mPlane[i].PlaneEquationGetResultFrom(Point[4]);
			if (DotEnd >= 0)
			{
				continue;
			}
			/*��*/
			DotEnd = mPlane[i].PlaneEquationGetResultFrom(Point[5]);
			if (DotEnd >= 0)
			{
				continue;
			}
			/*��*/
			DotEnd = mPlane[i].PlaneEquationGetResultFrom(Point[6]);
			if (DotEnd >= 0)
			{
				continue;
			}
			/*��*/
			DotEnd = mPlane[i].PlaneEquationGetResultFrom(Point[7]);
			if (DotEnd >= 0)
			{
				continue;
			}
			return false;
		}
		return true;
	}



	//�ж�һ�������Ƿ����ӽ�����,�õ��ǰ�Χ��İ취
	//���ڹ����ӽ��������6�����ǵ�λ�������������������ƽ��ĵ��Ϊ���ĵ�ƽ��ľ���
	//��������c���ӽ���6��ƽ���е�����һƽ��ľ���Ϊk,���-r>k,������λ�ڶ�Ӧƽ��ķ���֮�⣬��������ȫλ���ӽ���֮�⣬��������������ӽ����ཻ(�����ཻ������ȫλ���ӽ���)
	bool Frustum::CheckSphere(float xCenter, float yCenter, float zCenter, float radius)
	{
		Core::SVector3 Point = Core::SVector3(xCenter, yCenter, zCenter);
		float DotEnd;
		for (int i = 0; i < 6; ++i)
		{
			DotEnd = mPlane[i].PlaneEquationGetResultFrom(Point);
			if (-radius > DotEnd)
			{
				return false;
			}
		}
		return true;
	}

	//�㷨����������㷨һ��
	//xSize,ySize,zSize�ֱ�Ϊ�����峤��ߵ�һ��
	bool Frustum::CheckRectangle(float xCenter, float yCenter, float zCenter, float xSize, float ySize, float zSize)
	{
		Core::SVector3 Point[8];
		float DotEnd;
		Point[0] = Core::SVector3(xCenter - xSize, yCenter - ySize, zCenter - zSize);
		Point[1] = Core::SVector3(xCenter + xSize, yCenter - ySize, zCenter - zSize);
		Point[2] = Core::SVector3(xCenter - xSize, yCenter + ySize, zCenter - zSize);
		Point[3] = Core::SVector3(xCenter - xSize, yCenter - ySize, zCenter + zSize);
		Point[4] = Core::SVector3(xCenter + xSize, yCenter + ySize, zCenter - zSize);
		Point[5] = Core::SVector3(xCenter + xSize, yCenter - ySize, zCenter + zSize);
		Point[6] = Core::SVector3(xCenter - xSize, yCenter + ySize, zCenter + zSize);
		Point[7] = Core::SVector3(xCenter + xSize, yCenter + ySize, zCenter + zSize);

		for (int i = 0; i < 6; ++i)
		{
			/*�ֱ�����������ÿ�����Ƿ����ӽ�����*/

			/*һ*/
			DotEnd = mPlane[i].PlaneEquationGetResultFrom(Point[0]);
			if (DotEnd >= 0.0f)
			{
				continue;
			}

			/*��*/
			DotEnd = mPlane[i].PlaneEquationGetResultFrom(Point[1]);
			if (DotEnd >= 0.0f)
			{
				continue;
			}

			/*��*/
			DotEnd = mPlane[i].PlaneEquationGetResultFrom(Point[2]);
			if (DotEnd >= 0.0f)
			{
				continue;
			}

			/*��*/
			DotEnd = mPlane[i].PlaneEquationGetResultFrom(Point[3]);
			if (DotEnd >= 0.0f)
			{
				continue;
			}

			/*��*/
			DotEnd = mPlane[i].PlaneEquationGetResultFrom(Point[4]);
			if (DotEnd >= 0.0f)
			{
				continue;
			}

			/*��*/
			DotEnd = mPlane[i].PlaneEquationGetResultFrom(Point[5]);
			if (DotEnd >= 0.0f)
			{
				continue;
			}

			/*��*/
			DotEnd = mPlane[i].PlaneEquationGetResultFrom(Point[6]);
			if (DotEnd >= 0.0f)
			{
				continue;
			}

			/*��*/
			DotEnd = mPlane[i].PlaneEquationGetResultFrom(Point[7]);
			if (DotEnd >= 0.0f)
			{
				continue;
			}

			return false;
		}
		return true;
	}

	bool Frustum::CheckRectangle2(float maxWidth, float maxHeight, float maxDepth, float minWidth, float minHeight, float minDepth)
	{
		Core::SVector3 Point[8];
		float DotEnd;
		Point[0] = Core::SVector3(minWidth, minHeight, minDepth);
		Point[1] = Core::SVector3(maxWidth, minHeight, minDepth);
		Point[2] = Core::SVector3(minWidth, maxHeight, minDepth);
		Point[3] = Core::SVector3(maxWidth, maxHeight, minDepth);
		Point[4] = Core::SVector3(minWidth, minHeight, maxDepth);
		Point[5] = Core::SVector3(maxWidth, minHeight, maxDepth);
		Point[6] = Core::SVector3(minWidth, maxHeight, maxDepth);
		Point[7] = Core::SVector3(maxWidth, maxHeight, maxDepth);

		for (int i = 0; i < 6; ++i)
		{
			/*�ֱ�����������ÿ�����Ƿ����ӽ�����*/

			/*һ*/
			DotEnd = mPlane[i].PlaneEquationGetResultFrom(Point[0]);
			if (DotEnd >= 0)
			{
				continue;
			}

			/*��*/
			DotEnd = mPlane[i].PlaneEquationGetResultFrom(Point[1]);
			if (DotEnd >= 0)
			{
				continue;
			}

			/*��*/
			DotEnd = mPlane[i].PlaneEquationGetResultFrom(Point[2]);
			if (DotEnd >= 0)
			{
				continue;
			}

			/*��*/
			DotEnd = mPlane[i].PlaneEquationGetResultFrom(Point[3]);
			if (DotEnd >= 0)
			{
				continue;
			}

			/*��*/
			DotEnd = mPlane[i].PlaneEquationGetResultFrom(Point[4]);
			if (DotEnd >= 0)
			{
				continue;
			}

			/*��*/
			DotEnd = mPlane[i].PlaneEquationGetResultFrom(Point[5]);
			if (DotEnd >= 0)
			{
				continue;
			}

			/*��*/
			DotEnd = mPlane[i].PlaneEquationGetResultFrom(Point[6]);
			if (DotEnd >= 0)
			{
				continue;
			}

			/*��*/
			DotEnd = mPlane[i].PlaneEquationGetResultFrom(Point[7]);
			if (DotEnd >= 0)
			{
				continue;
			}

			return false;
		}
		return true;
	}
}