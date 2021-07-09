#pragma once
#include"../GameObject.h"

class Human;

class RollingBall :public GameObject
{
public:

	void Deserialize(const json11::Json& jsonObj) override;
	void Update()override;
	void CheckBump();

	inline void SetOwner(const std::shared_ptr<GameObject>& spOwner) { m_wpOwner = spOwner; }
private:
	int		m_wallHeight = 0;
	float	m_makeSpeed = 0.0f;
	int     m_Life = 150;
	Vec3    pos;
	Vec3    scale;
	Vec3    m_prevPos = {};
	std::weak_ptr<GameObject>m_wpOwner;
	std::shared_ptr<GameObject> m_sphuman = std::make_shared<GameObject>();

};