#include"../main.h"
#include "Scene.h"
#include "GameObject.h"
#include"../Component/CameraComponent.h"
#include"../Component/InputComponent.h"
#include"Action/StageEditor.h"
#include"EditorCamera.h"

#include"AnimationEffect.h"
#include"DebugLine.h"

//コンストラクタ
Scene::Scene()
{

}

//デストラクタ
Scene::~Scene()
{

}

//初期化
void Scene::Init()
{
	Scene::GetInstance().RequestChangeScene("Data/Scene/Title.json");
	//RequestChangeScene("Data/Scene/TestActionGame.json");
	//Scene::GetInstance().RequestChangeScene("Data/Scene/GameOver.json");

	m_spScreenRT = std::make_shared<Texture>();
	m_spScreenRT->CreateRenderTarget(1280, 720, DXGI_FORMAT_R16G16B16A16_FLOAT);

	m_spScreenZ = std::make_shared<Texture>();
	m_spScreenZ->CreateDepthStencil(1280, 720);

	m_spBlurRT[0] = std::make_shared<Texture>();
	m_spBlurRT[0]->CreateRenderTarget(1280 / 4, 720 / 4);

	m_spBlurRT[1] = std::make_shared<Texture>();
	m_spBlurRT[1]->CreateRenderTarget(1280 / 4, 720 / 4);

	m_spHeightBrightTex = std::make_shared<Texture>();
	m_spHeightBrightTex->CreateRenderTarget(1280, 720, DXGI_FORMAT_R16G16B16A16_FLOAT);

	m_blurTex.Create(1280, 720);

	DirectX::AUDIO_ENGINE_FLAGS eflags = DirectX::AudioEngine_EnvironmentalReverb | DirectX::AudioEngine_ReverbUseFilters;

	m_spStageEditor = std::make_shared<StageEditor>();

	AUDIO.Init();

	Deserialize();
}

void Scene::Deserialize()
{

}

void Scene::Release()
{
	m_spobjects.clear();

	AUDIO.Relese();
}

//更新
void Scene::Update()
{

	m_spStageEditor->Update();

	// 点光の登録をリセットする
	SHADER.ResetPointLight();

	if (m_editorCameraEnable)
	{
		m_camera.Update();
	}

	auto slectObject = m_wpImguiSelectObj.lock();

	for (auto spObj : m_spobjects)
	{
		spObj->Update();
	}

	for (auto spObjectItr = m_spobjects.begin(); spObjectItr != m_spobjects.end();)
	{
		//寿命が尽きていたらリストから除外
		if ((*spObjectItr)->IsAlive() == false)
		{
			spObjectItr = m_spobjects.erase(spObjectItr);
		}
		else
		{
			++spObjectItr;
		}
	}

	if (m_isRequestChangeScene)
	{
		ExecChangeScene();
	}

	AUDIO.Update();

	if (GetAsyncKeyState(VK_F1) & 0x8000)
	{
		ShowCursor(true);
		Scene::GetInstance().RequestChangeScene("Data/Scene/Title.json");
	}

	if (GetAsyncKeyState(VK_F2) & 0x8000)
	{
		ShowImGui = true;
		ShowCursor(true);
	}

	if (GetAsyncKeyState(VK_F3) & 0x8000)
	{
		AUDIO.StopBGM();
		ShowCursor(true);
		Scene::GetInstance().RequestChangeScene("Data/Scene/Result.json");
	}

}

//描画
void Scene::Draw()
{
	//エディターカメラをシェーダーにセット
	if (m_editorCameraEnable)
	{
		m_camera.SetToShader();
	}
	else
	{
		std::shared_ptr<CameraComponent>spCamera = m_wpTaegetCamera.lock();
		if (spCamera)
		{
			spCamera->SetToShader();
		}
	}

	{
		RestoreRenderTarget rrt;

		D3D.GetDevContext()->OMSetRenderTargets(1, m_spScreenRT->GetRTViewAddress(), m_spScreenZ->GetDSView());
		D3D.GetDevContext()->ClearRenderTargetView(m_spScreenRT->GetRTView(), Math::Color(0, 0, 0, 1));
		D3D.GetDevContext()->ClearDepthStencilView(m_spScreenZ->GetDSView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);

		//============================
		// シャドウマップ生成描画
		//============================
		SHADER.m_genShadowMapShader.Begin();

		// 全オブジェクトを描画
		for (auto& obj : m_spobjects)
		{
			obj->DrawShadowMap();
		}

		SHADER.m_genShadowMapShader.End();

		//ライトの情報をセット
		SHADER.m_cb8_Light.Write();

		//エフェクトシェーダーを描画デバイスにセット
		SHADER.m_effectShader.SetToDevice();

		//不透明物描画
		// シャドウマップをセット
		D3D.GetDevContext()->PSSetShaderResources(102, 1, SHADER.m_genShadowMapShader.GetDirShadowMap()->GetSRViewAddress());

		//SHADER.m_standardShader.SetToDevice();
		SHADER.m_modelShader.SetToDevice();

		for (auto pObject : m_spobjects)
		{
			pObject->Draw();
		}
		// シャドウマップを解除
		ID3D11ShaderResourceView* nullSRV = nullptr;
		D3D.GetDevContext()->PSSetShaderResources(102, 1, &nullSRV);

		//半透明物描画
		SHADER.m_effectShader.SetToDevice();
		SHADER.m_effectShader.SetTexture(D3D.GetWhiteTex()->GetSRView());

		//Z情報は使うが、Z書き込みoff
		D3D.GetDevContext()->OMSetDepthStencilState(SHADER.m_ds_ZEble_ZWriteDisable, 0);

		//カリングなし(両面描画）
		D3D.GetDevContext()->RSSetState(SHADER.m_rs_CullNone);

		for (auto spObj : m_spobjects)
		{
			spObj->DrawEffect();
		}

		D3D.GetDevContext()->OMSetDepthStencilState(SHADER.m_ds_ZEnable_ZWriteEnable, 0);

		//カリングあり(表面のみ描画）
		D3D.GetDevContext()->RSSetState(SHADER.m_rs_CullBack);

		//2D用のシェーダ開始
		SHADER.m_spriteShader.Begin();
		//全てのオブジェクトの２D描画を行う
		for (auto obj : m_spobjects)
		{
			obj->Draw2D();
		}
		//シェーダの終了
		SHADER.m_spriteShader.End();

		if (ShowImGui)
		{
			DebugLine::GetInstance().DebugDraw();
			for (auto obj : m_spobjects)
			{
				obj->DebugDraw2D();
			}
		}
	}

	// ぼかしていない状況をそのまま表示
	SHADER.m_postProcessShader.ColorDraw(m_spScreenRT.get(),
		DirectX::SimpleMath::Vector4(1, 1, 1, 1));

	// しきい値以上のピクセルを抽出
	SHADER.m_postProcessShader.BrightFiltering(m_spHeightBrightTex.get(), m_spScreenRT.get());

	// 一定以上の明るさを持ったテクスチャを各サイズぼかし画像作成
	SHADER.m_postProcessShader.GenerateBlur(m_blurTex, m_spHeightBrightTex.get());

	// 加算合成に変更
	D3D.GetDevContext()->OMSetBlendState(SHADER.m_bs_Add,
		Math::Color(0, 0, 0, 0), 0xFFFFFFFF);

	// 各サイズの画像を加算合成
	for (int bCnt = 0; bCnt < 5; bCnt++)
	{
		SHADER.m_postProcessShader.ColorDraw(
			m_blurTex.m_rt[bCnt][0].get(), DirectX::SimpleMath::Vector4(1, 1, 1, 1));
	}

	// 合成方法をもとに戻す
	D3D.GetDevContext()->OMSetBlendState(SHADER.m_bs_Alpha,
		Math::Color(0, 0, 0, 0), 0xFFFFFFFF);

	if (whiteness >= 1)
	{
		SHADER.m_postProcessShader.ColorDraw(m_spScreenRT.get(), DirectX::SimpleMath::Vector4(whiteness, whiteness, whiteness, 1));
		whiteness *= 0.7f;
		if (whiteness < 1.0f) { whiteness = 1.0f; }
	}
	else {
		SHADER.m_postProcessShader.ColorDraw(m_spScreenRT.get(), DirectX::SimpleMath::Vector4(1, 1, 1, 1));
	}

}

void Scene::AddObject(std::shared_ptr<GameObject> spObject)
{
	if (spObject == nullptr) { return; }
	m_spobjects.push_back(spObject);
}

void Scene::ImGuiUpdate()
{
	if (!ShowImGui) { return; }

	ImGuiPrefabFactoryUpdate();

	if (ImGui::Begin("LightSettings"))
	{

		if (ImGui::DragFloat3("Direction", &m_lightDir.x, 0.01f))
		{
			SHADER.m_cb8_Light.Work().DL_Dir = m_lightDir;
			SHADER.m_cb8_Light.Work().DL_Dir.Normalize();
		}
		if (ImGui::DragFloat3("Color", &m_lightColor.x, 0.01f))
		{
			SHADER.m_cb8_Light.Work().DL_Color = m_lightColor;
		}
	}
	ImGui::End();

}

void Scene::ImGuiPrefabFactoryUpdate()
{
	if (ImGui::Begin("PrefabFactory"))
	{
		ImGui::InputText("", &m_jsonPath);
		ImGui::SameLine();
		if (ImGui::Button(u8"Jsonパス取得"))
		{
			//std::string	filepath = "";
			if (APP.m_window.OpenFileDialog(m_jsonPath,
				"ファイルを開く",
				"JSONファイル\0*.json\0"
			))
			{
				//str = filepath;
			}
		}

		if (ImGui::Button("Create"))
		{
			std::ifstream ifs(m_jsonPath);

			if (ifs.fail())
			{
			}
			else {
				json11::Json json = ResFac.GetJSON(m_jsonPath);

				auto newGameObj = CreateGameObject(json["ClassName"].string_value());

				newGameObj->Deserialize(json);

				MergePrefab(json);

				//リストへ追加
				AddObject(newGameObj);

			}
			
			
		}
	}
	ImGui::End();
}

void Scene::Reset()
{
	m_spobjects.clear();
	m_wpImguiSelectObj.reset();
	m_wpTaegetCamera.reset();
	//m_spsky = nullptr;
}

void Scene::LoadScene(const std::string& seneFilename)
{
	Reset();

	//Json読み込み
	json11::Json json = ResFac.GetJSON(seneFilename);
	if (json.is_null())
	{
		assert(0 && "[LoadScene]Jsonファイル読み込み失敗");
		return;
	}

	//オブジェクトリスト取得
	auto& objectDataList = json["GameObjects"].array_items();
	
	//オブジェクト生成ループ
	for (auto&& objJsonData : objectDataList)
	{
		//オブジェクトの作成
		auto newGameObj = CreateGameObject(objJsonData["ClassName"].string_value());

		MergePrefab(objJsonData);

		//オブジェクトのデシりアライズ
		newGameObj->Deserialize(objJsonData);

		//リストへ追加
		AddObject(newGameObj);
	}
}

void Scene::RequestChangeScene(const std::string& filename)
{
	m_nextSceneFilename = filename;
	m_isRequestChangeScene = true;
}
void Scene::ExecChangeScene()
{
	LoadScene(m_nextSceneFilename);
	m_isRequestChangeScene = false;
}

std::shared_ptr<GameObject> Scene::FindObjectWithName(const std::string& name)
{
	// 文字列比較
	for (auto&& obj : m_spobjects)
	{
		if (obj->GetName() == name) { return obj; }
	}
	// 見つからなかったらnullを返す
	return nullptr;
}