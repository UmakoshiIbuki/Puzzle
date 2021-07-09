#include"CameraComponent.h"

//コンストラクター
CameraComponent::CameraComponent(GameObject& owner): m_owner(owner)
{
	m_mProj.CreateProjectionPerspacetiveFov(60 * ToRadians,
		D3D.GetBackBuffer()->GetAspectRatio(), 0.01f, 5000.0f);
}

CameraComponent::~CameraComponent()
{

}

void CameraComponent::SetCameraMatrix(const Matrix& m)
{
	//カメラ行列セット
	m_mCam = m_mOffset*m;

	//カメラ行列からビュー行列を算出
	m_mView = m_mCam;
	m_mView.Inverse();
}

void CameraComponent::SetToShader()
{
	//追従カメラ座標をシェーダーにセット
	SHADER.m_cb7_Camera.Work().CamPos = m_mCam.GetTranslation();

	//追従カメラのview行列をシェーダーにセット
	SHADER.m_cb7_Camera.Work().mV = m_mView;

	//追従カメラの射影行列をシェーダーにセット
	SHADER.m_cb7_Camera.Work().mP = m_mProj;

	//カメラ情報(view行列、射影行列)を、シェーダーの定数バッファへセット
	SHADER.m_cb7_Camera.Write();
}
