#pragma once
#include"../GameObject.h"

class SnowManager :public GameObject
{
public:

	virtual void Deserialize(const json11::Json& jsonObj) override;
	void Draw2DTex(float f1,float f2);
	void Draw2DHP(float f1,int f2);
	void Draw2D();
	void Draw2DRemaining(int snow);
	void DrawTex(std::string TexFile, Vec3	Pos);
	void Update()override;
	
private:
	std::shared_ptr<Texture> m_spSnowRemainingTex;	//雪玉の残弾数
	Matrix					 m_SnowRemainingMat;		//描画位置

	std::shared_ptr<Texture> m_spBackTex;
	Matrix					 m_spBackMat;
	
	std::shared_ptr<Texture> m_spSnowGageTex;
	Matrix					 m_SnowGageMat; 
	std::shared_ptr<Texture> m_spSnowGageTex2;
	Matrix					 m_SnowGageMat2;

	std::shared_ptr<Texture> m_spHpTex;
	Matrix					 m_HpMat;

	std::shared_ptr<Texture> m_spHpBerTex;

	std::shared_ptr<Texture> m_spParticleTex;

	std::shared_ptr<Texture> m_spTex;

};

#define RND (rand()/(double)RAND_MAX)