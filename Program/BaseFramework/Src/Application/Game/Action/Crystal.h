#pragma once
#include "../GameObject.h"

class AinmationEffect;

class Crystal:public GameObject
{
public:
	virtual void Deserialize(const json11::Json& jsonObj) override;
	void UpdateCollision();

	void Update();
	void ParticleEffect();


private:

	int		m_frame;
	float	m_dissolveThreshold;
	float	m_rot;
	float	m_LightPower;
	Vec3	m_pos;
	Matrix	m_rotMat;

	std::shared_ptr<AinmationEffect> fallSnowTex[100];
	Matrix  falleffectMat[100];
	float   falleffectposY;
	Vec3   m_FalleffectPos[100];
};