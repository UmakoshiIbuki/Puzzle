#pragma once
#include"GameObject.h"

struct PARTICLE
{
	float mPosX, mPosY,mPosZ;
	float mMoveX, mMoveY;
	float mSize;
	int   mLifeSpan;
	Matrix mMat;
};

class Particle:public GameObject
{
public:
	virtual void Deserialize(const json11::Json& jsonObj) override;
	void Update()override;

	void EmitParticle(struct PARTICLE* p, float aPosX, float aPosY,float aPosZ, float aMoveX, float aMoveY, float size, int LifeSpan);
	void UpdateParticle(struct PARTICLE* p);
	void DrawParticle(struct PARTICLE* p);
	void DrawEffect()override;
	void SetTextureFile(std::string FileName) { filename = FileName; }

	void SetShowTime(int time) { showtime = time; }
	void SetMove(float moveX, float moveY,float speedX,float speedY) { m_MoveX = moveX, m_MoveY = moveY, m_MoveSpeedX=speedX, m_MoveSpeedY=speedY; }
	void SetSize(float size) { m_Size = size; }
private:
	static const int particleNum = 35;//粒子の個数
	struct PARTICLE smoke[particleNum];//煙用

	std::shared_ptr<Texture> m_spParticleTex;
	int showtime = 0;
	SquarePolygon m_poly;

	float m_MoveX;
	float m_MoveY;
	float m_MoveSpeedX;
	float m_MoveSpeedY;
	float m_Size;
	std::string filename;

	float m_g=0.0f;

};