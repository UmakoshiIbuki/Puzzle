#include "Title.h"
#include"../AnimationEffect.h"
#include"../Scene.h"
#include"../../main.h"
#include"../../Component//CameraComponent.h"
#include"../../Component//ModelComponent.h"

void Title::Deserialize(const json11::Json& jsonObj)
{
	GameObject::Deserialize(jsonObj);

	if (m_spCameraComponent)
	{
		m_spCameraComponent->OffsetMatrix().CreateTranslation(7.0f, 5.0f, -30.0f);
		m_spCameraComponent->OffsetMatrix().RotateX(0.0 * ToRadians);
		m_spCameraComponent->OffsetMatrix().RotateY(0 * ToRadians);
	}

	Scene::GetInstance().SteTargetCamera(m_spCameraComponent);

	m_CamMat.RotateZ((0 * ToRadians));
	m_CamMat.RotateY((-180 * ToRadians));
	m_spCameraComponent->SetCameraMatrix(m_CamMat);
	m_spModelComponent->SetModel(ResFac.GetModel(string));

	m_spTitleTex = ResFac.GetTexture("Data/Texture/Title/Title.png");

	//AUDIO.Play("Data/Audio/BGM/loop100302.wav", true);

	for (int j = 0; j < 6; j++)
	{
		for (int i = 0; i < 6; i++)
		{
			m_Matrix.CreateTranslation(-250 + (100 * i), 250 - (100 * j), 1);
			m_Psos.push_back(m_Matrix.GetTranslation());
			m_Matrixs.push_back(m_Matrix);
			m_Scales.push_back(0.7);
		}
	}

	string = "Data/Box/BoxStage.gltf";
	m_spModelComponent->SetModel(ResFac.GetModel(string));
}

void Title::Update()
{

	GetCursorPos(&nowMousePos);				//マウス現在位置の取得
	ScreenToClient(hwnd, &nowMousePos);		//マウスをクライアント座標に

	nowMousePos.x -= 640;
	nowMousePos.y -= 360;

	MousePos.x = nowMousePos.x;
	MousePos.y = nowMousePos.y;

	for (UINT i = 0; i < m_Matrixs.size(); i++)
	{
		if (Collision2D(m_Psos[i], MousePos, 50, 50))
		{
			if (m_Scales[i] < 0.85f) { m_Scales[i] += 0.01f; }
			ImGui::LabelText("", "%d", i);
			std::ostringstream oss;
			oss << i;
			std::string s = "Stage_" + oss.str();
			ImGui::Text(s.c_str());

			if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
			{
				Scene::GetInstance().RequestChangeScene("Data/Scene/Stage/" + s + ".json");
			}
		}
		else
		{
			if (m_Scales[i] > 0.7) { m_Scales[i] -= 0.01f; }
		}
	}

	if (m_TitleBackScale < 1) 
	{ 
		m_TitleBackScale += 0.01f;
		return;
	}
	m_ShowStageSelect = true;

	//ImguiUpdate();

}

void Title::Draw2D()
{
	if (flg) { return; }

	m_spTitleBackTex = ResFac.GetTexture("Data/Texture/Title/TitleBack.png");
	m_TitleBackMat.CreateScalling(1, m_TitleBackScale, 0);
	m_TitleBackMat.SetTranslation(0, 0, 0);
	SHADER.m_spriteShader.SetMatrix(m_TitleBackMat);
	SHADER.m_spriteShader.DrawTex(m_spTitleBackTex.get(), 0, 0);

	if (!m_ShowStageSelect) { return; }
	for (UINT i = 0; i < m_Matrixs.size(); i++)
	{
		m_spTitleTex = ResFac.GetTexture("Data/Texture/Title/lock.png");
		m_Matrixs[i].CreateScalling(m_Scales[i], m_Scales[i], 0);
		m_Matrixs[i].SetTranslation(m_Psos[i].x, m_Psos[i].y, m_Psos[i].z);
		SHADER.m_spriteShader.SetMatrix(m_Matrixs[i]);
		SHADER.m_spriteShader.DrawTex(m_spTitleTex.get(), 0, 0);
	}

	m_spTitleTex = ResFac.GetTexture("Data/Texture/Title/Title.png");
	m_TitleMat.CreateScalling(0.8, 0.8, 0);
	m_TitleMat.SetTranslation(Vec3(-8, -23, 0));
	SHADER.m_spriteShader.SetMatrix(m_TitleMat);
	SHADER.m_spriteShader.DrawTex(m_spTitleTex.get(), 0, 0);

}

void Title::Draw()
{
	if (!flg) { return; }

	if (m_spModelComponent == nullptr) { return; }

	m_spModelComponent->Draw();
}

void Title::ImguiUpdate()
{

	if (ImGui::Begin("MousePos"))
	{
		ImGui::DragFloat2("nowMousePos", &MousePos.x, 0.01f);
	}
	ImGui::End();

}
