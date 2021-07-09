#pragma once 
#include"../../GameObject.h"

class ElevatorObj :public GameObject
{
public:
	virtual void Deserialize(const json11::Json& jsonObj) override;
	void Update()override;
	void AddImguiUpdate()override;

private:

	Vec3 	m_pos;
	int m_sleep = 120;

};