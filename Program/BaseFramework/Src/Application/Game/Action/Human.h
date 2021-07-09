#pragma once
#include"../GameObject.h"

class Human:public GameObject
{
public:

	virtual void Deserialize(const json11::Json& jsonObj) override;
	virtual void Update()override;

	bool IsGround() { return m_isGround; }

	void SetAnimation(const char* pAnimName);

	void SetCrystal(int crystal) { m_crystal += crystal; }
	int  GetCrystal() { return m_crystal; }

private:

	//キャラクターの移動関係//////////////////////////////////////////////////////////////////
	void UpdateMove();							//移動更新処理
	void UpdateRotate(const Vec3& rMoveDir);
	float		m_movespeed = 0.04f;				//移動スピード
	Vec3		m_pos;
	Vec3		m_force;						//キャラクターにかかる力
	Vec3		m_rot;						    //ワールド行列の回転角度 
	float		m_rotateAngle = 10.0f;			//キャラクターの回転速度
	Vec3		m_prevPos;						//1フレーム前のキャラクターのPos
	float       m_jumpPow = 0.1f;
	bool        m_HipDrop = false;
	bool		m_jumpflg = true;

	//カメラ/////////////////////////////////////////////////////////////////////////////////
	void UpdaetCamera();
	float		m_camRotSpeed = 0.2f;
	
	//衝突判定///////////////////////////////////////////////////////////////////////////////
	void UpdateCollision();
	void CheckBump();
	bool CheckGround(float& rDistance);
	bool        m_hit = false;					//球に当たったか？
	bool		m_isGround;
	UINT		m_tag = OBJECT_TAG::TAG_None;

	void Draw2D();
	std::shared_ptr<Texture>   m_spReticleTex;		//十字のレティクル
	Matrix					   m_ReticleMat;		//描画位置
	float					   m_Scale = 0.6;
	
	bool m_canmGather =false;//壁を作った時やリロードの間動かなくする用

	Animator m_animator;

	static const float s_allowToStepHeight;		//歩いて乗り越えられる段差の高さ
	static const float s_landingHeight;			//地面から足が離れていても着地しているとする高さ(坂道などを下るときに宙に浮くのを避ける)
	
	int frame = 0;

	int m_crystal = 0;

	Matrix m_CamMat;

	int sleep = 0;

};