#include "AnimationEffect.h"

AinmationEffect::AinmationEffect(){}

AinmationEffect::~AinmationEffect(){}

void AinmationEffect::SetAnimationInfo(const std::shared_ptr<Texture>& rTex, float size, int splitX, int splitY, float angle,float animspeed,float animPos)
{
	m_poly.Init(size, size, { 1,1,1,1 });
	m_poly.SetAnimationInfo(splitX, splitY);
	m_poly.SetAnimationPos(animPos);
	m_poly.SetTexture(rTex);
    m_AnimSpeed = animspeed;
	m_angleZ = angle;
}

void AinmationEffect::Update()
{
	m_poly.Animation(m_AnimSpeed, true);

	if (m_poly.IsAnimationEnd())
	{
		Destroy();
	}
}

void AinmationEffect::DrawEffect()
{
//各軸方向の拡大率を取得
		float scaleX = m_mWorld.GetAxisX().Length();
		float scaleY = m_mWorld.GetAxisY().Length();
		float scaleZ = m_mWorld.GetAxisZ().Length();

		//ビルボード処理
		Matrix drawMat;
		drawMat.CreateScalling(scaleX, scaleY, scaleZ);		//拡大率を設定
		drawMat.RotateZ(m_angleZ * ToRadians);			//Z軸の回転角度を加える		常に同じ画像だったら嫌なのでZ軸回転させる

		drawMat.SetBillboard(SHADER.m_cb7_Camera.GetWork().mV);		//カメラ行列の取得

		//座標は自分のものを使う
		drawMat.SetTranslation(m_mWorld.GetTranslation());
		//Matrix camMat = SHADER.m_cb7_Camera.GetWork().mV;
		//camMat.Inverse();//カメラの逆行列を計算			カメラの真正面に来る

		//カメラの逆行列を描画する行列と合成
		//drawMat *= camMat;

		SHADER.m_effectShader.SetWorldMatrix(drawMat);
		SHADER.m_effectShader.WriteToCB();
		m_poly.Draw(0);
}
