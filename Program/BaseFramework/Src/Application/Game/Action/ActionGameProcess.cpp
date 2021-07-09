#include"Tank.h"
#include"Human.h"
#include"ActionGameProcess.h"
#include"StageEditor.h"
#include"../Scene.h"
#include"../AnimationEffect.h"
#include"../../main.h"
#include"../../Component//InputComponent.h"



void ActionGameProcess::Deserialize(const json11::Json& jsonObj)
{
	ShowCursor(false);

	m_spExpTex = ResFac.GetTexture("Data/Texture/UITexture/UI_Exp.png");

	m_sphuman = Scene::GetInstance().FindObjectWithName("PlayerHuman");
	m_sptank = Scene::GetInstance().FindObjectWithName("Tank");

	m_GoTitlePos.x = 2;
	m_GoTitlePos.y = -45;

	m_ReplayPos.x = 0;
	m_ReplayPos.y = 50;

	m_StopMat.SetTranslation(0, 150, 0);

	if (jsonObj["Name"].is_null() == false)
	{
		m_name = jsonObj["Name"].string_value();
	}

	//KD_AUDIO.Play("Data/Audio/BGM/loop100315.wav", true);

}

void ActionGameProcess::Draw2D()
{
	//ゲーム中のクリスタル獲得UI(1の位と10の位）
	m_TexMat.CreateScalling(0.25f, 0.25f, 1);
	m_TexMat.SetTranslation(560, 300, 0);
	SHADER.m_spriteShader.SetMatrix(m_TexMat);
	SHADER.m_spriteShader.DrawTex(m_spCrystalsOncePlaceTex.get(), 0, 0);

	m_TexMat.CreateScalling(0.25f, 0.25f, 1);
	m_TexMat.SetTranslation(500, 300, 0);
	SHADER.m_spriteShader.SetMatrix(m_TexMat);
	SHADER.m_spriteShader.DrawTex(m_spCrystalsTenthPlaceTex.get(), 0, 0);

	m_TexMat.CreateScalling(0.5, 0.5, 1);
	m_TexMat.SetTranslation(m_TexPos);
	SHADER.m_spriteShader.SetMatrix(m_TexMat);
	SHADER.m_spriteShader.DrawTex(m_spExpTex.get(), 0, 0);

	if (m_GoPause)
	{
		m_spPauseTex = ResFac.GetTexture("Data/Texture/Stop.png");
		SHADER.m_spriteShader.SetMatrix(m_StopMat);
		SHADER.m_spriteShader.DrawTex(m_spPauseTex.get(), 0, 0);

		m_spPauseTex = ResFac.GetTexture("Data/Texture/GoTitle.png");
		m_PauseMat.CreateScalling(m_scale1, m_scale1, 0);
		m_PauseMat.SetTranslation(m_GoTitlePos);
		SHADER.m_spriteShader.SetMatrix(m_PauseMat);
		SHADER.m_spriteShader.DrawTex(m_spPauseTex.get(), 0, 0);

		m_spPauseTex = ResFac.GetTexture("Data/Texture/Replay.png");
		m_ReplayMat.CreateScalling(m_scale0, m_scale0, 0);
		m_ReplayMat.SetTranslation(m_ReplayPos);
		SHADER.m_spriteShader.SetMatrix(m_ReplayMat);
		SHADER.m_spriteShader.DrawTex(m_spPauseTex.get(), 0, -0);
	}
}

void ActionGameProcess::Update()
{

	//改善//////////////////////////////////////////////////////////////////
	POINT nowMousePos;
	HWND hwnd;
	hwnd = APP.m_window.GetWndHandle();

	GetCursorPos(&nowMousePos);				//マウス現在位置の取得
	ScreenToClient(hwnd, &nowMousePos);		//マウスをクライアント座標に

	nowMousePos.x -= 640;
	nowMousePos.y -= 360;

	MousePos.x = nowMousePos.x;
	MousePos.y = nowMousePos.y;
	//改善//////////////////////////////////////////////////////////////////

	if (GetAsyncKeyState('M') )
	{
		if (m_OneTouch)
		{
			m_OneTouch = false;
			m_sphuman->SetBase(true);
			ShowCursor(true);
		}
	}
	else
	{
		m_OneTouch = true;
	}

	if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
	{
		if (m_OneTouch)
		{
			m_OneTouch = false;
			m_GoPause = true;
			m_sphuman->SetBase(true);
			ShowCursor(true);
			m_ReplayMat.SetTranslation(m_ReplayPos);
			m_GoTitleMat.SetTranslation(m_GoTitlePos);
		}
	}
	else
	{
		m_OneTouch = true;
	}
	
	if (m_GoPause)
	{
		if (Collision2D(m_ReplayPos, MousePos, 300, 50))
		{
			if (m_scale0 < 0.85f) { m_scale0 += 0.01f; }
			if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
			{
				ShowCursor(false);
				m_sphuman->SetBase(false);
				m_GoPause = false;
				return;
			}
		}
		else
		{
			if (m_scale0 > 0.7) { m_scale0 -= 0.01f; }
		}

		if (Collision2D(m_GoTitlePos, MousePos, 300, 50))
		{
			if (m_scale1 < 0.85f) { m_scale1 += 0.01f; }
			if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
			{
				m_sphuman->SetBase(false);
				Scene::GetInstance().RequestChangeScene("Data/Scene/Title.json");
			}
		}
		else
		{
			if (m_scale1 > 0.7) { m_scale1 -= 0.01f; }
		}
	}

	if (GetAsyncKeyState('Q'))
	{
		m_TexPos.SetPos(0, 0, 0);
		m_spExpTex = ResFac.GetTexture("Data/Texture/Expall.png");
	}
	else
	{
		m_TexPos.SetPos(600, -300, 0);
		m_spExpTex = ResFac.GetTexture("Data/Texture/UITexture/UI_Exp.png");
	}

	std::shared_ptr<Human> human = std::dynamic_pointer_cast<Human>(m_sphuman);
	std::shared_ptr<Tank> tank = std::dynamic_pointer_cast<Tank>(m_sptank);

	if (human)
	{
		m_CrystalsOncePlace = human->GetCrystal();
		Scene::GetInstance().SetCrystal(m_CrystalsOncePlace);
		human->SetCrystal(0);
	}

	CrystaCount(m_CrystalsOncePlace, m_CrystalsTenthPlace);

}

void ActionGameProcess::CrystaCount(int one, int ten)
{
	switch (one)
	{
	case 0:
		m_number = "0";
		m_spCrystalsOncePlaceTex = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + m_number + ".png");
		break;
	case 1:
		m_number = "1";
		m_spCrystalsOncePlaceTex = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + m_number + ".png");
		break;
	case 2:
		m_number = "2";
		m_spCrystalsOncePlaceTex = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + m_number + ".png");
		break;
	case 3:
		m_number = "3";
		m_spCrystalsOncePlaceTex = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + m_number + ".png");
		break;
	case 4:
		m_number = "4";
		m_spCrystalsOncePlaceTex = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + m_number + ".png");
		break;
	case 5:
		m_number = "5";
		m_spCrystalsOncePlaceTex = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + m_number + ".png");
		break;
	case 6:
		m_number = "6";
		m_spCrystalsOncePlaceTex = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + m_number + ".png");
		break;
	case 7:
		m_number = "7";
		m_spCrystalsOncePlaceTex = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + m_number + ".png");
		break;
	case 8:
		m_number = "8";
		m_spCrystalsOncePlaceTex = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + m_number + ".png");
		break;
	case 9:
		m_number = "9";
		m_spCrystalsOncePlaceTex = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + m_number + ".png");
		break;
	}

	switch (ten)
	{
	case 0:
		m_number = "0";
		m_spCrystalsTenthPlaceTex = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + m_number + ".png");
		break;
	case 1:
		m_number = "1";
		m_spCrystalsTenthPlaceTex = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + m_number + ".png");
		break;
	case 2:
		m_number = "2";
		m_spCrystalsTenthPlaceTex = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + m_number + ".png");
		break;
	case 3:
		m_number = "3";
		m_spCrystalsTenthPlaceTex = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + m_number + ".png");
		break;
	case 4:
		m_number = "4";
		m_spCrystalsTenthPlaceTex = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + m_number + ".png");
		break;
	case 5:
		m_number = "5";
		m_spCrystalsTenthPlaceTex = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + m_number + ".png");
		break;
	case 6:
		m_number = "6";
		m_spCrystalsTenthPlaceTex = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + m_number + ".png");
		break;
	case 7:
		m_number = "7";
		m_spCrystalsTenthPlaceTex = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + m_number + ".png");
		break;
	case 8:
		m_number = "8";
		m_spCrystalsTenthPlaceTex = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + m_number + ".png");
		break;
	case 9:
		m_number = "9";
		m_spCrystalsTenthPlaceTex = ResFac.GetTexture("Data/Texture/UITexture/UI_One_" + m_number + ".png");
		break;
	}
}