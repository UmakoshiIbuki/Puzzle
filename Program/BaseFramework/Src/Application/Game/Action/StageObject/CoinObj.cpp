#include"CoinObj.h"
#include"../Human.h"
#include"../../Scene.h"
#include"../../DebugLine.h"

void CoinObj::Deserialize(const json11::Json& jsonObj)
{
	GameObject::Deserialize(jsonObj);
	m_colRadius = 1.5;
}

void CoinObj::Update()
{
	DebugLine::GetInstance().AddDebugCoordinateAxisLine(m_pos, 3);

	GameObject::Update();
	m_rot.y += 1;
	m_pos = m_mWorld.GetTranslation();

	m_mWorld.CreateRotationY(m_rot.x);
	m_mWorld.RotateY(m_rot.y * ToRadians);
	m_mWorld.Move(m_pos);
	UpdateCollision();
}

void CoinObj::UpdateCollision()
{
	SphereInfo Info;
	Info.m_pos = m_mWorld.GetTranslation();
	Info.m_radius = m_colRadius;

	for (auto& obj : Scene::GetInstance().GetObjects())
	{
		//自分自身は無視
		if (obj.get() == this) { continue; }

		bool hit = false;

		//球判定
		if (obj->GetTag() & TAG_Character)
		{
			if (obj->HitCheckBySphere(Info))
			{
				hit = true;
			}
		}

		if (hit)
		{
			std::shared_ptr<Human> human = std::dynamic_pointer_cast<Human>(obj);
			if (human)
			{
				human->SetCrystal(1);
			}
			Destroy();
		}
	}
}

void CoinObj::AddImguiUpdate()
{
	if (ImGui::Button(u8"回転"))
	{
		m_rot.y += 90;
	}
}
