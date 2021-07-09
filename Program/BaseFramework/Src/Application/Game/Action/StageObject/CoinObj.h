#pragma once 
#include"../../GameObject.h"

class CoinObj :public GameObject
{
public:
	virtual void Deserialize(const json11::Json& jsonObj) override;
	void Update()override;
	void UpdateCollision();
	void AddImguiUpdate()override;

private:

	Vec3 	m_pos;
	Vec3 	m_rot;

};