#pragma once
#include"../GameObject.h"

class Lamp:public GameObject
{
public:
	virtual void Deserialize(const json11::Json& jsonObj) override;

	void Update()override;
	void ImGuiUpdate();

private:
	float m_light = 0;
	float m_lightPower = 0;

	Vec3  m_color = { 0.0f,0.0f,0.0f };
};