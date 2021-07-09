#pragma once

#include "GameObject.h"

class AinmationEffect : public GameObject
{
public:
	AinmationEffect();
	~AinmationEffect();

	//アニメーション情報の設定(初期化)
	void  SetAnimationInfo(const std::shared_ptr<Texture>& tex, float size, int splitX, int splitY, float angle,float animspeed,float animPos);

	//アニメーションの更新
	virtual void Update() override;

	//半透明物の描画
	virtual void DrawEffect() override;

private:

	SquarePolygon m_poly;

	float m_AnimSpeed = 0;
	float m_angleZ = 0;

};