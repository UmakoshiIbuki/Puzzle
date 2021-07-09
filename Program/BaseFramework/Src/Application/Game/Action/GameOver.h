#pragma once
#include"../GameObject.h"

class AinmationEffect;

class GameOver :public GameObject
{
public:
	virtual void Deserialize(const json11::Json& jsonObj) override;
	void Update()override;
	void Draw()override;

private:

	Matrix					   m_ResultMat;
	std::shared_ptr<Texture>   m_spResultTex;

	Vec3					   m_GoTitlePos;
	Matrix					   m_GoTitleMat;
	std::shared_ptr<Texture>   m_spGoTitleTex;

	Vec3					   m_ReTryPos;
	Matrix					   m_ReTryMat;
	std::shared_ptr<Texture>   m_spReTryTex;

	bool   m_CanChange = false;
	Matrix m_CamMat;

	float							 m_FallEffectposY;
	Vec3							 m_FallEffectPos[100];
	Matrix							 m_FallEffectMat[100];
	std::shared_ptr<AinmationEffect> m_FallSnowTex[100];

	float							 m_scale0 = 0.7f;
	float							 m_scale1 = 0.7f;

};