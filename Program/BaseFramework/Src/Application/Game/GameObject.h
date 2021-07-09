#pragma once

class CameraComponent;
class InputComponent;
class ModelComponent;

struct SphereInfo;
struct RayInfo;

struct SphereResult;

//タグ定数
enum OBJECT_TAG
{
	TAG_None=0x00000000,			//属性なし
	TAG_Character=0x00000001,		//キャラクター設定
	TAG_Player =0x00000002,			//プレイヤー設定
	TAG_StageObject =0x00000004,	//ステージオブジェクト属性
	TAG_Spring = 0x00000009,		//バネ
	TAG_AttackHit =0x00000010,		//攻撃が当たる属性
	TAG_Mini= 0x00000100
};

class GameObject:public std::enable_shared_from_this<GameObject>
{
public:
	GameObject();
	virtual ~GameObject();

	virtual void Deserialize(const json11::Json& jsonObj);
	virtual void Update();
	virtual void DrawShadowMap();
	virtual void Draw();

	//半透明物の描画
	virtual void DrawEffect() {};

	virtual void Draw2D() { }
	virtual void DebugDraw2D() { }
	
	virtual void ImguiUpdate();
	virtual void AddImguiUpdate();

	inline const Matrix& GetMatrix()const { return m_mWorld; };
	inline void SetMatrix(const Matrix& rMat) { m_mWorld = rMat; }
	inline bool IsAlive() const { return m_alive; }
	inline void Destroy() { m_alive = false; }

	inline void SetTag(UINT tag) { m_tag; }
	inline UINT GetTag() { return m_tag; }
	Matrix GetCam() { return SHADER.m_cb7_Camera.GetWork().mV; }

	inline const std::string& Getname()const { return m_name; }

	//カメラコンポーネント取得
	std::shared_ptr<CameraComponent>GetCameraComponent() { return m_spCameraComponent; }

	std::shared_ptr<ModelComponent>GetModelComponent() { return m_spModelComponent; }

	//球による当たり判定
	bool HitCheckBySphere(const SphereInfo& rInfo);

	//レイによる当たり判定
	bool HitCheckByRay(const RayInfo& rInfo,KdRayResult& rResurt);

	//球による当たり判定(メッシュ)
	bool HitCheckBySphereVsMesh(const SphereInfo& rInfo, SphereResult& rRsult);

	inline const char* GetName()const { return m_name.c_str(); }

	const Matrix& GetPrevMatrix() { return m_mPrev; }
	bool GetBase() { return m_base; }
	void SetBase(bool flg) {m_base=flg; }

	bool GetIsHit() { return m_Hit; }
	void SetHit(bool flg) { m_Hit = flg; }

	Matrix GetOneMove()
	{
		Matrix mPI = m_mPrev;
		mPI.Inverse();
		return mPI * m_mWorld;
	}

	float easeInOut(float progress);
	float easeOutBack(float progress);
	float easeInBack(float progress);
	float easeOutQuart(float progress);

	Vec3 GetRotation() { return m_rot; }
	Vec3 GetForce() { return m_force; }

	float GetRotY() { return m_rot.y; }
protected:

	virtual void Release();

	float m_colRadius = 0.7f;//このキャラクターの半径

	//カメラコンポーネント
	std::shared_ptr<CameraComponent>m_spCameraComponent = std::make_shared<CameraComponent>(*this);
	//インプットコンポーネント
	std::shared_ptr<InputComponent>m_spInputComponent = std::make_shared<InputComponent>(*this);
	//モデルコンポーネント
	std::shared_ptr<ModelComponent>m_spModelComponent = std::make_shared<ModelComponent>(*this);

	Vec3                       MousePos;
	Vec3                       m_RotAngle;
	POINT nowMousePos;
	HWND hwnd;
	Matrix  m_mWorld;
	Matrix  m_mPrev;
	float   m_gravity = 0.01f;
	bool	m_alive = true;
	bool    m_base = false;
	UINT	m_tag = OBJECT_TAG::TAG_None;
	float	m_Movespeed = 0.0f;
	bool    m_LetsMove = true;
	bool	m_Hit=false;
	float m_angleZ = 0;

	Vec3 m_scale;
	Vec3 m_force;
	Vec3 m_pos;
	Vec3 m_rot;
	float m_dissolveThreshold = 0.0f;

	std::string m_name = "GameObject";
};

//クラス名からGameObjectを生成する関数
std::shared_ptr<GameObject>CreateGameObject(const std::string& name);

//球判定に使うデータ
struct SphereInfo
{
	Vec3 m_pos = {};
	float m_radius = 0.0f;
};

struct RayInfo
{
	Vec3 m_pos ;			//レイの発射場所
	Vec3 m_dir;			//レイの発射方向
	float m_maxRange = 0.0f;//レイが届く最大距離
};

//球判定の結果データ
struct SphereResult
{
	Vec3 m_push;		//押し戻すベクトル
	bool m_hit = false;
};

#define RND (rand()/(double)RAND_MAX)