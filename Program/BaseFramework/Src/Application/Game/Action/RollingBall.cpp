#include"RollingBall.h"
#include"../Scene.h"


void RollingBall::Deserialize(const json11::Json& jsonObj)
{

	GameObject::Deserialize(jsonObj);
	m_sphuman = Scene::GetInstance().FindObjectWithName("PlayerHuman");

	scale.x = 1.001f;
	scale.y = 1;
	scale.z = 1.001f;
	m_colRadius = jsonObj["Rad"].number_value();

}
void RollingBall::Update()
{

	SphereInfo sinfo;
	sinfo.m_pos = m_sphuman->GetMatrix().GetTranslation();
	sinfo.m_radius = 5;
	if (!HitCheckBySphere(sinfo)) { return; }

	pos = m_mWorld.GetTranslation();

	Vec3 move;

	if (--m_Life <= 0)
	{
		//m_makeSpeed = -0.01f;
		//move.y = m_makeSpeed;
		//m_mWorld.Move(move);
		//if(m_mWorld.GetTranslation().y<-1){Destroy();}
	}


	m_gravity += 0.01f;
	CheckBump();
	pos.y -= m_gravity;

	if (m_Life >= 0)
	{
		if (m_wallHeight < 100)
		{
			/*m_wallHeight++;
			m_makeSpeed = 0.01f;
			move.y = m_makeSpeed;*/
			//scale.x += 0.01;
		}
	}

	move.y = 0;

	m_mWorld.SetTranslation(pos);

}

void RollingBall::CheckBump()
{
	SphereInfo info;

	info.m_pos = pos;
	info.m_radius = m_colRadius;

	for (auto& obj : Scene::GetInstance().GetObjects())
	{
		if (obj.get() == this) { continue; }

		SphereResult Result;
		if (obj->HitCheckBySphereVsMesh(info, Result))
		{
			{
				//m_mWorld.Scale(scale.x, scale.y, scale.z);
				pos.Move(Result.m_push);
				m_gravity = 0.0f;
			}
		}
	}

}