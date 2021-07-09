#include"LiftObj.h"
#include"../../Scene.h"
#include"../../DebugLine.h"

void LiftObj::Deserialize(const json11::Json& jsonObj)
{
	GameObject::Deserialize(jsonObj);
	
	m_colRadius = 1.5;
}

void LiftObj::Update()
{
	DebugLine::GetInstance().AddDebugCoordinateAxisLine(m_pos,3);
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
		//m_rot.y += 1;
	}
	m_mWorld.CreateRotationX(m_rot.x);
	m_mWorld.RotateY(m_rot.y * ToRadians);
	m_mWorld.Move(m_pos);
}

void LiftObj::AddImguiUpdate()
{
	if (ImGui::Button(u8"回転"))
	{
		m_rot.y += 90;
	}

	if (ImGui::Button(u8"左回転"))
	{
		m_rot.y = 360;
		m_force.SetPos(0, 0, -0.1);
	}

	if (ImGui::Button(u8"右回転"))
	{
		m_rot.y = 180;
		m_force.SetPos(0, 0, 0.1);
	}

	if (ImGui::Button(u8"上回転"))
	{
		m_rot.y = 90;
		m_force.SetPos(-0.1,0,0);
	}

	if (ImGui::Button(u8"下回転"))
	{
		m_rot.y = 270;
		m_force.SetPos(0.1, 0, 0);
	}
}
