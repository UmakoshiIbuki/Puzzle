#include "SnowManager.h"
#include"../../Component//InputComponent.h"
#include"../Scene.h"
#include"Human.h"

void SnowManager::Deserialize(const json11::Json& jsonObj)
{
}

void SnowManager::Draw2DTex(float f1,float f2)
{	
	m_spSnowGageTex = ResFac.GetTexture("Data/Texture/UITexture/UI_BER.png");

	m_SnowGageMat.CreateScalling(1.0f, 1.0f, 1.0f);
	m_SnowGageMat.SetTranslation(Vec3(-400, -340, 0));
	SHADER.m_spriteShader.SetMatrix(m_SnowGageMat);
	SHADER.m_spriteShader.DrawTex(m_spSnowGageTex.get(), 0, 0);

	m_spSnowGageTex = ResFac.GetTexture("Data/Texture/UITexture/UI_BER1.png");

	m_SnowGageMat.CreateScalling(f1 / 3, 1.0f, 1.0f);
	m_SnowGageMat.SetTranslation(Vec3(-400 + (40 * f2), -340, 0));
	SHADER.m_spriteShader.SetMatrix(m_SnowGageMat);
	SHADER.m_spriteShader.DrawTex(m_spSnowGageTex.get(), 0, 0);
}

void SnowManager::Draw2DHP(float f1,int f2)
{
	m_spBackTex = ResFac.GetTexture("Data/Texture/UITexture/back.png");
	m_spBackMat.CreateScalling(1.5f, 1.0f, 1.0f);
	m_spBackMat.SetTranslation(Vec3(-430, -340, 0));
	SHADER.m_spriteShader.SetMatrix(m_spBackMat);
	SHADER.m_spriteShader.DrawTex(m_spBackTex.get(), 0, 0);

	m_spHpBerTex = ResFac.GetTexture("Data/Texture/UITexture/UI_HP_BER.png");
	m_HpMat.CreateScalling(0.4f, 0.25f, 1.0f);
	m_HpMat.SetTranslation(Vec3(-430, -280, 0));
	SHADER.m_spriteShader.SetMatrix(m_HpMat);
	SHADER.m_spriteShader.DrawTex(m_spHpBerTex.get(), 0, 0);

	m_spHpTex = ResFac.GetTexture("Data/Texture/UITexture/UI_Hp_00.png");
	m_HpMat.CreateScalling(f1 / 100, 0.65f, 1.0f);
	m_HpMat.SetTranslation(Vec3((float)-430 - f2, -293, 0));
	SHADER.m_spriteShader.SetMatrix(m_HpMat);
	SHADER.m_spriteShader.DrawTex(m_spHpTex.get(), 0, 0);
}

void SnowManager::Draw2D()
{
	
}

void SnowManager::Draw2DRemaining(int snow)
{
	m_spSnowRemainingTex = ResFac.GetTexture("Data/Texture/UITexture/UI_SNOWBALL000.png");

	m_SnowRemainingMat.CreateScalling(0.25f, 0.25f, 1.0f);
	m_SnowRemainingMat.SetTranslation((50*snow) - 110, -330, 0);
	SHADER.m_spriteShader.SetMatrix(m_SnowRemainingMat);
	SHADER.m_spriteShader.DrawTex(m_spSnowRemainingTex.get(), 0, 0);
}

void SnowManager::DrawTex(std::string TexFile, Vec3 Pos)
{
	m_spTex = ResFac.GetTexture("Data/Texture/UITexture/"+TexFile+".png");
	Matrix mat;
	mat.SetTranslation(Pos.x-1, Pos.y+2, Pos.z);
	SHADER.m_spriteShader.SetMatrix(mat);
	SHADER.m_spriteShader.DrawTex(m_spTex.get(), 0, 0);
}

void SnowManager::Update()
{
	
}
