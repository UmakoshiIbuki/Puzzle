#include "Tank.h"
#include"../Scene.h"
#include"Human.h"
#include"../../Component//ModelComponent.h"

void Tank::Deserialize(const json11::Json& jsonObj)
{
	GameObject::Deserialize(jsonObj);
	m_spTankTex = ResFac.GetTexture("Data/Texture/UITexture/UI_BER.png");

	m_sphuman = Scene::GetInstance().FindObjectWithName("PlayerHuman");
	
	pos = m_mWorld.GetTranslation();
	pos.y += 1;

	m_dissolveThreshold = 0.5f;
}

void Tank::Update()
{
	this->GetModelComponent()->SetDissolveThreshold(m_dissolveThreshold);
	//this->GetModelComponent()->

	m_hit = false;
	m_rot += 0.5;
	m_mWorld.CreateRotationY(m_rot * ToRadians);
	m_mWorld.Scale(m_scale.x, m_scale.y, m_scale.z);
	m_mWorld.Move(pos);

	UpdateCollision();	

	if (!m_hit) { return; }

	std::shared_ptr<Human> human = std::dynamic_pointer_cast<Human>(m_sphuman);
	if (human)
	{
		m_crystal += human->GetCrystal();
		human->SetCrystal(-human->GetCrystal());
		float f = 0.5f-(float)m_crystal / 10;
		if(m_dissolveThreshold> f)
		m_dissolveThreshold -= 0.001f;
	}

	if (m_crystal >= 5)
	{
		m_frame++;
		if (m_dissolveThreshold > 0) { return; }
		Scene::GetInstance().SetCrystal(m_crystal);
		AUDIO.StopBGM();
		//ShowCursor(true);
		Scene::GetInstance().RequestChangeScene("Data/Scene/Result.json");
	}

}

void Tank::UpdateCollision()
{
	SphereInfo sInfo;
	sInfo.m_pos = m_mWorld.GetTranslation();
	sInfo.m_radius = 1;

	for (auto& obj : Scene::GetInstance().GetObjects())
	{
		//自分自身
		if (obj.get() == this) { continue; }
		{
			//キャラクター以外は無視
			if (!(obj->GetTag() & TAG_Character)) { continue; }

			//当たり判定
			if (obj->HitCheckBySphere(sInfo))
			{
				m_hit = true;
			}
		}
	}
}

void Tank::Draw2D()
{
	/*if (!m_hit) { return; }
	m_TankMat.SetTranslation(0,0,0);
	SHADER.m_spriteShader.SetMatrix(m_TankMat);
	SHADER.m_spriteShader.DrawTex(m_spTankTex.get(), 0, 0);*/
}
