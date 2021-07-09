#pragma once

struct KdRayResult
{
	float m_distance = FLT_MAX;	//当たった時の距離
	bool  m_hit = false;		//当たったかどうか
	Vec3 rHitPos = {};
};

bool RayToMesh(
	const DirectX::XMVECTOR& rRayPos, 
	const DirectX::XMVECTOR& rRayDir, 
	float maxDistance, 
	const Mesh& rMash, 
	const Matrix& rMatrix,
	KdRayResult& rResult
	);

bool KdSphereToMesh(
	const Math::Vector3& rSpherePos,	//球の中心点の場所
	float radius,						//球の半径
	const Mesh& mesh,					//判定するメッシュ情報
	const DirectX::XMMATRIX& matrix,	//判定する相手の行列
	Math::Vector3& rPushedPos			//当たっていた場合、押し返された球の中心点
);

bool Collision2D(Vec3 pos1, Vec3 pos2, float width,float height);

void KdPointToTriangle(const DirectX::XMVECTOR& p, const DirectX::XMVECTOR& a,
	const DirectX::XMVECTOR& b, const DirectX::XMVECTOR& c, DirectX::XMVECTOR& outPt);