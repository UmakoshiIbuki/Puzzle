#include"RotObj.h"
#include"../Human.h"
#include"../../Scene.h"
#include"../../DebugLine.h"

void RotObj::Deserialize(const json11::Json& jsonObj)
{
	GameObject::Deserialize(jsonObj);
	//最初の地点を覚える
	m_mStart = m_mWorld;

	//移動先情報の読み込み
	if (jsonObj["MoveTo"].is_array())
	{
		auto& p = jsonObj["MoveTo"].array_items();
		m_vGoal = m_mStart.GetTranslation();
	}

	//移動スピード読み込み
	if (jsonObj["Speed"].is_number())
	{
		m_speed = 0;
	}

}

void RotObj::Update()
{
	DebugLine::GetInstance().AddDebugSphereLine(m_mWorld.GetTranslation(), m_colRadius, { 0.0f,0.0f,1.0f,1.0f });

	if (m_Hit)
	{
		m_mPrev = m_mWorld;

		m_sleep--;
		if (m_sleep < 0) { return; }
		auto& vStart = m_mStart.GetTranslation();
		auto& vGoal = m_mStart.GetTranslation();

		Vec3 vTo = vGoal - vStart;//ゴール地点のベクトル
		Vec3 vNow = vStart + vTo * sin(easeInOut(m_speed));//進行具合を加味して座標を求める
		m_rot.y += 0.1;
		m_mWorld.CreateRotationY(m_rot.x);
		m_mWorld.RotateY(m_rot.y * ToRadians);
		m_mWorld.SetTranslation(vNow);
		//進行具合の更新
		m_progress += m_speed;
		m_speed += 0.009;
	}
	else
	{
		m_pos = m_mWorld.GetTranslation();
		m_mWorld.CreateRotationX(m_rot.x);
		m_mWorld.RotateY(m_rot.y * ToRadians);
		m_mWorld.Move(m_pos);
	}

}

void RotObj::AddImguiUpdate()
{
	if (ImGui::Button(u8"回転"))
	{
		m_rot.y += 90;
	}
	if (ImGui::Button(u8"左回転"))
	{
		m_rot.y = 360;
	}

	if (ImGui::Button(u8"右回転"))
	{
		m_rot.y = 180;
	}

	if (ImGui::Button(u8"上回転"))
	{
		m_rot.y = 90;
	}

	if (ImGui::Button(u8"下回転"))
	{
		m_rot.y = 270;
	}
}
