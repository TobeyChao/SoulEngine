#pragma once
#include "../SoulPCH.h"
#include "../Core/Mathematics/SVector2.h"
#include "../Core/Mathematics/SVector3.h"
#include "../Core/Mathematics/SVector4.h"
#include <random>

namespace Soul
{
	class ParticleEmitter
	{
	private:

		struct ParticleType
		{
			ParticleType()
				:
				lifeTime(0.0f),
				active(false)
			{

			}
			Core::SVector3 position;
			Core::SVector4 color;
			Core::SVector3 velocity;
			float lifeTime;
			bool active;
		};

	public:
		ParticleEmitter();
		~ParticleEmitter();
		bool Initialize(SubMesh* particlesMesh, const json& createParameters);
		bool Update(float deltaTime);
	private:

		bool InitializeBuffer();

		void EmitParticles(float);
		void UpdateParticles(float);
		void KillParticles();

		bool UpdateBuffers();

	private:
		// ���ӵķ�������
		Core::SVector3 mParticleEmitCenter;
		// ���ӵķ�������ƫ��
		Core::SVector3 mParticleDeviation;
		// �����ٶȣ�Ĭ��z��������
		Core::SVector3 mParticleVelocity;
		// �����ٶȷ�����xy������Ƕ�
		Core::SVector2 mParticleVelocityVariationAngle;
		// ���������ķ�Χ
		float mParticleLifeTimeMin;
		float mParticleLifeTimeMax;
		// ���ӵĴ�С
		float mParticleSize;
		// ÿ�뷢��������
		float mParticlesPerSecond;
		// ÿ����һ��������Ҫ������
		float mSecondsPerParticle;
		// ���������
		size_t mMaxParticles;
		// ��ǰ�������
		int mCurrentParticleCount;
		// �ۻ�ʱ��
		float mAccumulatedTime;
		// ��������
		std::vector<ParticleType> mParticleList;
		// ���������
		std::mt19937 rng;
		// ���ӵ�Mesh
		SubMesh* mParticlesMesh;
	};
}