#include "GameOver.h"
#include"../Scene.h"
#include"../AnimationEffect.h"
#include"../../main.h"
#include"../../Component//CameraComponent.h"

void GameOver::Deserialize(const json11::Json& jsonObj)
{
	if (m_spCameraComponent)
	{
		m_spCameraComponent->OffsetMatrix().CreateTranslation(10.0f, 5.0f, -30.0f);
		m_spCameraComponent->OffsetMatrix().RotateX(0.0 * ToRadians);
		m_spCameraComponent->OffsetMatrix().RotateY(0 * ToRadians);
	}

	Scene::GetInstance().SteTargetCamera(m_spCameraComponent);

	m_CamMat.RotateZ((0 * ToRadians));
	m_CamMat.RotateY((-180 * ToRadians));
	m_spCameraComponent->SetCameraMatrix(m_CamMat);

	m_spResultTex = ResFac.GetTexture("Data/Texture/Result/GameOver.png");
	m_spGoTitleTex = ResFac.GetTexture("Data/Texture/Result/UI_GoTitle.png");
	m_spReTryTex = ResFac.GetTexture("Data/Texture/Result/UI_ReTry.png");

	m_GoTitlePos.x = -200;
	m_GoTitlePos.y = -100;
	m_GoTitleMat.SetTranslation(m_GoTitlePos);

	m_ReTryPos.x = 200;
	m_ReTryPos.y = -100;
	m_ReTryMat.SetTranslation(m_ReTryPos);

	for (UINT i = 0; i < 100; i++)
	{
		m_FallSnowTex[i] = std::make_shared< AinmationEffect>();
		m_FallEffectMat[i].SetTranslation(RND * 30 - 15, RND * 20, RND * 30 - 15);

		m_FallSnowTex[i]->SetAnimationInfo(ResFac.GetTexture("Data/Texture/SnowCrystal.png"), 0.4f, 1, 1, 0, 0, 0);
		m_FallSnowTex[i]->SetMatrix(m_FallEffectMat[i]);
		m_FallEffectPos[i] = m_FallEffectMat[i].GetTranslation();
		Scene::GetInstance().AddObject(m_FallSnowTex[i]);
	}
	m_FallEffectposY = 0.02f;
}

void GameOver::Update()
{
	
	GetCursorPos(&nowMousePos);				//マウス現在位置の取得
	ScreenToClient(hwnd, &nowMousePos);		//マウスをクライアント座標に

	nowMousePos.x -= 640;
	nowMousePos.y -= 360;

	MousePos.x = nowMousePos.x;
	MousePos.y = nowMousePos.y;

	if (Collision2D(MousePos, m_GoTitlePos, 200, 100))
	{
		if (m_scale0 < 0.85f) { m_scale0 += 0.01f; }

		if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
		{
			if (m_CanChange)
			{
				Scene::GetInstance().RequestChangeScene("Data/Scene/Title.json");
				m_CanChange = false;
			}
		}
		else
		{
			m_CanChange = true;
		}
	}
	else
	{
		if (m_scale0 > 0.7) { m_scale0 -= 0.01f; }
	}

	if (Collision2D(MousePos, m_ReTryPos, 200, 100))
	{
		if (m_scale1 < 0.85f) { m_scale1 += 0.01f; }

		if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
		{
			if (m_CanChange)
			{
				Scene::GetInstance().RequestChangeScene("Data/Scene/TestActionGame.json");
				m_CanChange = false;
			}
		}
		else
		{
			m_CanChange = true;
		}
	}
	else
	{
		if (m_scale1 > 0.7) { m_scale1 -= 0.01f; }
	}

	for (UINT i = 0; i < 100; i++)
	{
		m_FallEffectPos[i].y -= m_FallEffectposY;
		m_FallEffectMat[i].Scale(0.99, 0.99, 0.99);
		m_FallEffectMat[i].SetTranslation(m_FallEffectPos[i]);
		m_FallSnowTex[i]->SetMatrix(m_FallEffectMat[i]);
		if (m_FallEffectMat[i].GetTranslation().y < 0)
		{
			m_FallEffectMat[i].CreateScalling(1, 1, 1);
			m_FallEffectPos[i].Move(RND * 30 - 15, RND * 20, RND * 30 - 15);
		}
	}
}

void GameOver::Draw()
{
	m_ResultMat.SetTranslation(Vec3(0,0,0));
	SHADER.m_spriteShader.SetMatrix(m_ResultMat);
	SHADER.m_spriteShader.DrawTex(m_spResultTex.get(), 0, 0);

	m_GoTitleMat.CreateScalling(m_scale0, m_scale0, 0);
	m_GoTitleMat.SetTranslation(m_GoTitlePos);
	SHADER.m_spriteShader.SetMatrix(m_GoTitleMat);
	SHADER.m_spriteShader.DrawTex(m_spGoTitleTex.get(), 0, 0);

	m_ReTryMat.CreateScalling(m_scale1, m_scale1, 0);
	m_ReTryMat.SetTranslation(m_ReTryPos);
	SHADER.m_spriteShader.SetMatrix(m_ReTryMat);
	SHADER.m_spriteShader.DrawTex(m_spReTryTex.get(), 0, 0);
}