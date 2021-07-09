#pragma once

#include"../Game/GameObject.h"

////////////////////////////////////////////////
//カメラコンポーネントクラス
////////////////////////////////////////////////

class CameraComponent
{
public:
	CameraComponent(GameObject& owner);
	~CameraComponent();

	//オフセット行列取得
	inline Matrix& OffsetMatrix() { return m_mOffset; }

	//カメラ行列
	inline const Matrix& GetCameraMatrix() { return m_mCam; }

	//ビュー行列
	inline const Matrix& GetViewMatrix() { return m_mView; }
	
	//カメラ行列、ビュー行列設定(行列ｍと行列Offsetが合成され、最終的なカメラ行列になる)
	void SetCameraMatrix(const Matrix& m);

	//カメラ情報(ビューz射影行列など)をシェーダにセット
	void SetToShader();

protected:

	//オフセット行列
	Matrix  m_mOffset;
	//カメラ行列
	Matrix  m_mCam;
	//ビュー行列
	Matrix  m_mView;
	//射影行列
	Matrix  m_mProj;

	GameObject& m_owner;
};