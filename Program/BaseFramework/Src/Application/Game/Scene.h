#pragma once

#include"EditorCamera.h"

//前方宣言
class GameObject;
class CameraComponent;
class Bridge;
class StageEditor;

class Scene
{
public:
	static Scene& GetInstance()
	{
		static Scene inctance;
		return inctance;
	}

	//デストラクタ
	~Scene();

	
	void Init();         //初期化関数
	void Deserialize();
	void Release();      //解放関数 
	void Update();       //更新関数   
	void Draw();         //描画関数   

	void RequestChangeScene(const std::string& filename);

	void AddObject(std::shared_ptr<GameObject> spObject);

	inline void SteTargetCamera(std::shared_ptr<CameraComponent>spCamera) { m_wpTaegetCamera = spCamera; }

	void ImGuiUpdate();

	void ImGuiPrefabFactoryUpdate();

	//GameObjectのリストを返す
	const std::list<std::shared_ptr<GameObject>>GetObjects()const { return m_spobjects; }
	
	std::shared_ptr<GameObject> FindObjectWithName(const std::string& name);

	void SetCrystal(int crystal) { m_crystals= crystal; };
	int GetCrystal() { return m_crystals; }

	void SetHitCnt(int one, int ten) { m_hitCntOne = one, m_hitCntTen = ten; }
	int GetPlayerHitCntOne() { return m_hitCntOne; }
	int GetPlayerHitCntTen() { return m_hitCntTen; }

	void SetWhiteIn(float f) { whiteness = f; }
	float GetWhite() { return whiteness; }

	void SetHitCnt(int one) { HitCnt += one; }
	int GetHitCnt() { return HitCnt; }

	void SetShotCnt(int one) { ShotCnt += one; }
	int GetShotCnt() { return ShotCnt; }

	void EditorCameraOnOff(bool flg) { m_editorCameraEnable = flg; }

	void LoadScene(const std::string& sneneFilename);

private:
	int HitCnt = 0;
	int ShotCnt = 0;

	bool ShowImGui = false;

	bool m_canPlaySE = false;

	float whiteness = 0.0f;

	std::shared_ptr<Texture> m_spHeightBrightTex = nullptr;

	std::shared_ptr<Texture> m_spBlurRT[2] = { nullptr, nullptr };

	KdBlurTexture m_blurTex;

	int m_hitCntOne = 0;
	int m_hitCntTen = 0;

	int m_crystals = 0;

	bool b = false;

	std::shared_ptr<Texture> m_spScreenRT = nullptr;
	std::shared_ptr<Texture> m_spScreenZ = nullptr;

	//-------------------------
	// 平行光
	//-------------------------
	Vec3 m_lightDir = { -1,-1,-1 };		//方向
	Vec3 m_lightColor = { 0.5f,0.5f,0.5f };		//光の色

	std::string m_jsonPath = "";

	std::string m_nextSceneFilename = "";	//次のシーンのJsonファイル名
	bool m_isRequestChangeScene = false;	//シーン遷移のリクエストがあったか

	void ExecChangeScene();					//シーンを実際に変更するところ
	void Reset();							//シーンをまたぐときにリセットする処理

	//コンストラクタ						
	Scene();

	EditorCamera	m_camera;
	bool			m_editorCameraEnable = false;

	bool			pauseFlag = false;

	std::list<std::shared_ptr<GameObject>> m_spobjects;

	std::weak_ptr<GameObject>m_wpImguiSelectObj;

	//ターゲットのカメラ
	std::weak_ptr<CameraComponent>m_wpTaegetCamera;

	SquarePolygon m_poly;

	std::shared_ptr<StageEditor> m_spStageEditor;

};
