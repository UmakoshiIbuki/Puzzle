#include "Particle.h"

void Particle::Deserialize(const json11::Json& jsonObj)
{
	GameObject::Deserialize(jsonObj);
		
	m_poly.SetTexture(ResFac.GetTexture(filename));
	m_poly.Init(1.0f, 1.0f, { 1,1,1,1 });
}

void Particle::Update()
{
	showtime--;
	for (int i = 0; i < particleNum; i++)
	{
		UpdateParticle(&smoke[i]);
	}

	if (showtime < 0)
	{
		Destroy();
	}
}

void Particle::EmitParticle(PARTICLE* p, float aPosX, float aPosY,float aPosZ, float aMoveX, float aMoveY, float size, int LifeSpan)
{
	p->mPosX = aPosX;
	p->mPosY = aPosY;
	p->mPosZ = aPosZ;
	p->mMoveX = aMoveX;
	p->mMoveY = aMoveY;
	p->mSize = size;
	p->mLifeSpan = LifeSpan;
}

void Particle::UpdateParticle(PARTICLE* p)
{
	//座標更新
	p->mPosX += p->mMoveX;
	p->mPosY += p->mMoveY;
	m_g += m_gravity;

	p->mPosY -= m_g/100;
	//行列作成
	Matrix matPos, matSize;

	//サイズ変更
	p->mSize *= 0.90f;
	
	//有効期間
	p->mLifeSpan--;
	if (p->mLifeSpan <= 0)
	{
		EmitParticle(p, m_mWorld.GetTranslation().x, m_mWorld.GetTranslation().y+0.5, m_mWorld.GetTranslation().z, RND * m_MoveX - m_MoveSpeedX, RND * m_MoveY - m_MoveSpeedY, RND * m_Size, showtime+5);
	}

	p->mMat.CreateScalling(p->mSize, p->mSize, 0);
	p->mMat.SetTranslation(p->mPosX, p->mPosY, p->mPosZ);
}

void Particle::DrawParticle(PARTICLE* p)
{
	/*SHADER.m_spriteShader.SetMatrix(p->mMat);
	SHADER.m_spriteShader.DrawTex(m_spParticleTex.get(), 0, 0);*/

	Matrix m;
	m.SetTranslation(p->mMat.GetTranslation());
	p->mMat.SetBillboard(SHADER.m_cb7_Camera.GetWork().mV);
	p->mMat.SetTranslation(m.GetTranslation());

	SHADER.m_effectShader.SetWorldMatrix(p->mMat);
	SHADER.m_effectShader.WriteToCB();
	m_poly.Draw(0);
}

void Particle::DrawEffect()
{

	//Matrix drawMat;
	//drawMat.CreateScalling(scaleX, scaleY, scaleZ);		//拡大率を設定
	//drawMat.RotateZ(m_angleZ * KdToRadians);			//Z軸の回転角度を加える		常に同じ画像だったら嫌なのでZ軸回転させる

	//drawMat.SetBillboard(SHADER.m_cb7_Camera.GetWork().mV);		//カメラ行列の取得

	////座標は自分のものを使う
	//drawMat.SetTranslation(m_mWorld.GetTranslation());

	for (int i = 0; i < particleNum; i++)
	{
		DrawParticle(&smoke[i]);
	}
}
