#pragma once 
#include"../../GameObject.h"

class Human;

class RotObj :public GameObject
{
public:
	void Deserialize(const json11::Json & jsonObj)override;
	void Update()override;
	void AddImguiUpdate()override;

private:
	Matrix m_mStart;
	Vec3 m_vGoal = {};

	float m_speed = 0.0f;
	float m_progress = 0.0f;
	bool  m_goTo = true;
	int   m_sleep = 900;

};