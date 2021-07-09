#include "Crystal.h"
#include "Human.h"
#include "../Scene.h"
#include"../Particle.h"
#include"../../Component//ModelComponent.h"
#include"../AnimationEffect.h"

void Crystal::Deserialize(const json11::Json& jsonObj)
{
	GameObject::Deserialize(jsonObj);
	m_pos = m_mWorld.GetTranslation();
	m_pos.y += 1;

	m_colRadius = 1;
	m_dissolveThreshold = 0;
	
	for (UINT i = 0; i < 100; i++)
	{
		fallSnowTex[i] = std::make_shared< AinmationEffect>();
		falleffectMat[i].SetTranslation((m_mWorld.GetTranslation().x-1.5)+RND * 3 , m_mWorld.GetTranslation().y+ RND, (m_mWorld.GetTranslation().z - 1.5 )+RND * 3 );

		fallSnowTex[i]->SetAnimationInfo(ResFac.GetTexture("Data/Texture/White1.png"), 0.4f, 1, 1, 0, 0, 0);
		fallSnowTex[i]->SetMatrix(falleffectMat[i]);
		m_FalleffectPos[i] = falleffectMat[i].GetTranslation();
		Scene::GetInstance().AddObject(fallSnowTex[i]);
	}
	falleffectposY = 0.02f;

}

void Crystal::UpdateCollision()
{
	SphereInfo Info;
	Info.m_pos = m_mWorld.GetTranslation();		
	Info.m_radius = m_colRadius;				//動いた方向に向かって

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
				Scene::GetInstance().RequestChangeScene("Data/Scene/Result.json");
			}			
		}
	}
}

void Crystal::Update()
{
	m_pos = m_mWorld.GetTranslation();
	m_rot += 0.5;
	m_mWorld.CreateRotationY(m_rot*ToRadians);
	m_mWorld.Move(m_pos);

	for (UINT i = 0; i < 100; i++)
	{
		m_FalleffectPos[i].y -= falleffectposY;
		falleffectMat[i].Scale(0.99, 0.99, 0.99);
		falleffectMat[i].SetTranslation(m_FalleffectPos[i]);
		fallSnowTex[i]->SetMatrix(falleffectMat[i]);
		if (falleffectMat[i].GetTranslation().y < 0)
		{
			falleffectMat[i].CreateScalling(1, 1, 1);
			m_FalleffectPos[i].Move(0, m_mWorld.GetTranslation().y + RND, 0);
		}
	}

	//ParticleEffect();

	UpdateCollision();
}

void Crystal::ParticleEffect()
{
	static const std::string filename = "Data/Texture/White.png";
	std::shared_ptr<Particle> particle = std::make_shared< Particle>();
	particle->SetTextureFile(filename);
	particle->SetShowTime(10);
	particle->SetSize(0.5f);
	particle->SetMove(0.3f, 0.3f, 0.2f, 0.2f);
	particle->Deserialize(ResFac.GetJSON("Data/Scene/Particle.json"));
	m_pos = m_mWorld.GetTranslation();
	m_mWorld.SetTranslation(m_pos);
	particle->SetMatrix(m_mWorld);

	Scene::GetInstance().AddObject(particle);
}