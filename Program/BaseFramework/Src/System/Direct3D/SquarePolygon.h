﻿#pragma once

class SquarePolygon
{
public:

	//四角形ポリゴンの設定
	//w:幅 h:高さ_color:色
	void Init(float w, float h, const Math::Vector4& _color);

	void Draw(int setTextureNo);

	inline void SetTexture(const std::shared_ptr<Texture>& tex) {
		m_texture = tex;
	}

	//アニメーションの分割数を設定
	inline void SetAnimationInfo(int splitX, int splitY) {
		m_animSplitX = splitX;
		m_animSplitY = splitY;
	}

	//分割設定と渡されたコマ数を元にUV座標の計算
	//no:設定したいコマ番号
	void SetAnimationPos(float no);

	//アニメーションを進行させる
	//speed：進行速度(1.0で1コマ進む)
	//loop：ループ再生するかどうか
	void Animation(float speed, bool loop);

	//アニメーションが終わったかどうか
	bool IsAnimationEnd();

private:

	int m_animSplitX = 1;		//横の分割数
	int m_animSplitY = 1;		//縦の分割数

	float m_animPos = 0;		//現在のコマ位置
	//1頂点の情報
	struct VerTex
	{
		Math::Vector3 pos;		//3D空間上の座標
		Math::Vector2 UV;		//テクスチャ座標(*UV に意味はない)
		Math::Vector4 color;	//頂点の色(2点間はは補完される)
	};

	VerTex m_vertex[4];			//四角形の情報

	std::shared_ptr<Texture>m_texture;	//貼り付けるテクスチャの情報

};