#include"ElevatorObj.h"
#include"../../Scene.h"

void ElevatorObj::Deserialize(const json11::Json& jsonObj)
{
	GameObject::Deserialize(jsonObj);
	m_colRadius = 1.5; 
}

void ElevatorObj::Update()
{
	GameObject::Update();
	if (m_Hit)
	{
		m_sleep--;
		if (m_sleep < 0) { return; }
		m_pos.Move(m_force);
	}
	else
	{
		m_pos = m_mWorld.GetTranslation();
	}
	m_mWorld.CreateRotationX(m_rot.x);
	m_mWorld.RotateY(m_rot.y * ToRadians);
	m_mWorld.RotateZ(m_rot.z * ToRadians);
	m_mWorld.Move(m_pos);
}

void ElevatorObj::AddImguiUpdate()
{
	if (ImGui::Button(u8"回転"))
	{
		m_rot.y += 90;
	}
	if (ImGui::Button(u8"上行き"))
	{
		m_rot.z = 0;
		m_force.SetPos(0, 0.1, 0);
	}
	if (ImGui::Button(u8"下行き"))
	{
		m_rot.z = 180;
		m_force.SetPos(0, -0.1, 0);
	}
}
