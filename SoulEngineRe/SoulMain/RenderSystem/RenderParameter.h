#pragma once
#include "GPUBuffer.h"
namespace Soul
{
	enum class PrimitiveTopology
	{
		PT_TRIANGLELIST,			// �����δ�
		PT_LINELIST,				// �ߴ�
		PT_POINTLIST,				// ���
	};

	enum class BlendType
	{
		BT_DEFAULT,
		BT_TRANSPARENT,	// ͸�����
		BT_ADD,			//��ɫ�ӷ�
	};

	enum class RasterizerType
	{
		RT_DEFAULT,
		RT_CULL_NONE,				// ���޳�
		RT_CULL_CLOCKWISE,			// ����Ϊ˳ʱ�����ߣ��޳����棨˳ʱ�룩
		RT_CULL_COUNTERCLOCKWISE,	// ����Ϊ˳ʱ�����ߣ��޳����棨��ʱ�룩
		RT_WIREFRAME,				// ����Ϊ˳ʱ�����ߣ��߿�ģʽ�����޳�
	};

	enum class DepthStencilType
	{
		DST_DEFAULT,
		DST_LESS_EQUAL,		// ��Ȳ���С�ڵ��ڣ�������պ�
		DST_NO_DEPTH_WRITE,	// ��Ȳ��Կ��������ǲ�д�����ֵ
		DST_NO_DOUBLE_BLEND,// �޶��λ��
	};

	class RenderParameter
	{
	public:
		RenderParameter()
			:
			mPrimitiveTopology(PrimitiveTopology::PT_TRIANGLELIST),
			mSizePerVertex(0),
			mIndicesCount(0),
			mVertexCount(0),
			mVertexBuffer(nullptr),
			mIndexBuffer(nullptr)
		{
		}
		PrimitiveTopology mPrimitiveTopology;
		//Vertex Size
		unsigned int mSizePerVertex;
		//Index Count
		unsigned int mIndicesCount;
		//Vertex Count
		unsigned int mVertexCount;
		//Vertex Buffer
		GPUBuffer* mVertexBuffer;
		//Index Buffer
		GPUBuffer* mIndexBuffer;
	};
}