#pragma once
#include"../GameProcess.h"

class CharacterUpdate;
class AinmationEffect;

class ActionGameProcess :public GameProcess
{
public:

	void Deserialize(const json11::Json& jsonObj);

	void Draw2D();

	void Update() override;

	void CrystaCount(int one , int ten);
	int GetTen() { return m_CrystalsTenthPlace; }
	int GetOne() { return m_CrystalsOncePlace ; }

private:

	Matrix m_TexMat;	//テクスチャの行列
	Vec3   m_TexPos;	//テクスチャの座標

	std::shared_ptr<Texture> m_spExpTex;			//操作説明画面テクスチャ

	float m_rotate;
	float m_angle = 0;
	float scale = 1;


	std::shared_ptr<GameObject> m_sphuman;
	std::shared_ptr<GameObject> m_sptank;

	std::shared_ptr<Texture> m_spCrystalsOncePlaceTex;

	std::shared_ptr<Texture> m_spCrystalsTenthPlaceTex;

	std::string m_number;
	
	bool m_canChange = false;
	std::shared_ptr<Texture>   m_spPauseTex;
	bool					   m_GoPause = false;

	Matrix					   m_ReplayMat;
	Vec3					   m_ReplayPos;

	Matrix					   m_GoTitleMat;
	Vec3					   m_GoTitlePos;

	Matrix					   m_PauseMat;
	Matrix					   m_StopMat;

	Vec3                       MousePos;

	bool					   m_OneTouch=true;

	Math::Vector3 copy;

	float					   m_scale0 = 0.7f;
	float					   m_scale1 = 0.7f;
	///////////////////////////////////////////////////////////////////////////////////////
	int  m_CrystalsTenthPlace = 0;
	int  m_CrystalsOncePlace = 0;
	int  AttackCnt = 0;
	//////////////////////////////////////////////////////////////////////////////////////
};