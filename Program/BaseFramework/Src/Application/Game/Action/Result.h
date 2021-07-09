#pragma once
#include"../GameObject.h"

class AinmationEffect;

class Result :public GameObject
{
public:
	virtual void Deserialize(const json11::Json& jsonObj) override;
	void Update()override;
	void Draw()override;
private:

	std::shared_ptr<Texture>   m_spResultTex;
	Matrix					   m_spResultMat;

	std::shared_ptr<Texture>   m_spCrystalCntTex;
	Matrix					   m_CrystalCnt[99];
	Vec3					   pos = { 200,90,0 };

	Vec3					   m_GoTitlePos;
	Matrix					   m_GoTitleMat;
	std::shared_ptr<Texture>   m_spGoTitleTex;

	Vec3					   m_ReTryPos;
	Matrix					   m_ReTryMat;
	std::shared_ptr<Texture>   m_spReTryTex;

	std::string s;

	int						   m_CrystalCntOne = 0;
	int						   Crystals = 0;

	int						   Weight = 1;
	int						   i = 0;

	float					   m_scale0 = 0.7f;
	float					   m_scale1 = 0.7f;

	bool					   m_CanChange = false;
	Matrix					   m_CamMat;

	std::shared_ptr<AinmationEffect> fallSnowTex[100];
	Matrix							 falleffectMat[100];
	float							 falleffectposY;
	Vec3							 m_FalleffectPos[100];
};