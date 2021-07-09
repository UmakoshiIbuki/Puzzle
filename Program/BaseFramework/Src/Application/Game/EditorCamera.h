#pragma once

class EditorCamera
{
public:
	EditorCamera();		//コンストラクタ
	~EditorCamera();	//デストラクタ

	void Update();		//更新
	void SetToShader();	//view行列・射影表列をシェーダーセット

private:
	Math::Vector3	m_viewPos;
	Math::Matrix	m_mCam;
	Math::Matrix	m_mProj;

	POINT			m_prevMousePos = { 0,0 };//前フレームのマウス座標
};