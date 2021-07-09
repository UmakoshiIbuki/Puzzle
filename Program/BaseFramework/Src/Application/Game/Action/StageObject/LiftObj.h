#pragma once 
#include"../../GameObject.h"

class LiftObj:public GameObject
{
public:
	virtual void Deserialize(const json11::Json& jsonObj) override;
	void Update()override;
	void AddImguiUpdate()override;
private:

	Vec3 	m_pos;
	Vec3 	m_AxisZ;
	Vec3 v;
	int m_sleep = 35;
};