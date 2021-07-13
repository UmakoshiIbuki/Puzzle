#include"Human.h"
#include"SnowManager.h"
#include"../Scene.h"
#include"../DebugLine.h"
#include"../../Component//CameraComponent.h"
#include"../../Component//InputComponent.h"
#include"../../Component//ModelComponent.h"

const float Human::s_allowToStepHeight=0.8f;
const float Human::s_landingHeight=0.1f;

void Human::Deserialize(const json11::Json& jsonObj)
{
	GameObject::Deserialize(jsonObj);
	
	if (m_spCameraComponent)
	{
		m_spCameraComponent->OffsetMatrix().CreateTranslation(0.0f, 0.8f, 0.2f);
		m_spCameraComponent->OffsetMatrix().RotateX(5.0f * ToRadians);
		m_spCameraComponent->OffsetMatrix().RotateY(270.0f * ToRadians);
	}

	if ((GetTag() & TAG_Player) != 0)
	{
		Scene::GetInstance().SteTargetCamera(m_spCameraComponent);
		m_spInputComponent = std::make_shared<ActionPlayerInputComponent>(*this);
	}

	SetAnimation("Walk");
	const std::vector<json11::Json>& rRot = jsonObj["Rot"].array_items();

	m_CamMat.RotateZ((-10 * ToRadians));
	m_CamMat.RotateY((0 * ToRadians));
	m_spCameraComponent->SetCameraMatrix(m_CamMat);

	m_gravity = 0.008f;
	m_colRadius = 1;
	m_spReticleTex = ResFac.GetInstance().GetTexture("Data/Texture/UITexture/UI_Reticle.png");

}

void Human::Update()
{

	if (m_spInputComponent) { m_spInputComponent->Update(); }

	m_spInputComponent->SetBase(m_base);
	m_pos = m_mWorld.GetTranslation();
	if (m_base) { return; }
	frame++;

	//移動前の座標を覚える
	m_prevPos = m_pos;

	UpdaetCamera();

	//重力をキャラクターのYの移動力に加える
	m_force.y -= m_gravity;
	
	UpdateCollision();

	UpdateMove();
	//移動力をキャラクターの座標に足しこむ
	m_pos.Move(m_force);
	m_mWorld.CreateScalling(m_scale.x, m_scale.y, m_scale.z);
	m_mWorld.CreateRotationX(m_rot.x);
	m_mWorld.RotateY(m_rot.y);
	m_mWorld.RotateZ(m_rot.z);

	m_mWorld.SetTranslation(m_pos);

	m_animator.AdvanceTime(m_spModelComponent->GetChangeableNodes());
	ImGui::DragFloat3("Position", &m_force.x, 0.01f);
}

void Human::SetAnimation(const char* pAnimName)
{
	//if (m_spModelComponent)
	//{
	//	std::shared_ptr<KdAnimationData> animData = m_spModelComponent->GetAnimation(pAnimName);
	//	m_animator.SetAnimation(animData);
	//}
}

void Human::UpdateMove()
{

	if (!m_spCameraComponent) { return; }

	const Math::Vector2& inputMove = m_spInputComponent->GetAxis(Input::Axes::L);
	Vec3 moveSide = m_spCameraComponent->GetCameraMatrix().GetAxisX() * inputMove.x;
	Vec3 moveForward = m_spCameraComponent->GetCameraMatrix().GetAxisZ() * inputMove.y;

	//上下の移動成分はカット
	moveForward.y = 0.0f;

	Vec3 moveVec = moveSide + moveForward;

	moveVec.Normalize();
	UpdateRotate(moveVec);

	//移動速度補正
	moveVec *= m_movespeed;

	m_force.x = moveVec.x;
	m_force.z = moveVec.z;

	if (!m_isGround && m_spInputComponent->GetButton(Input::Buttons::Y))
	{
		if (m_force.y>0) { return; }
		m_HipDrop = true;
		m_force.y = -0.4;
		sleep = 120;
	}
	else
	{
		m_HipDrop = false;
	}

	if (sleep > 0) { sleep--; return; }

	if (m_spInputComponent->GetButton(Input::Buttons::Y))
	{
		if (!m_jumpflg||!m_isGround) { return; }
		m_force.y=m_jumpPow;
		m_jumpflg = false;
	}
	else
	{
		m_jumpflg = true;
	}
	
}

void Human::UpdateRotate(const Vec3& rMoveDir)
{
	//移動していなければ帰る
	if (rMoveDir.LengthSquared() == 0.0f) { return; }
	//今のキャラクターの方向ベクトル
	Vec3 nowDir = m_mWorld.GetAxisZ();
	nowDir.Normalize();

	//キャラクターの今向いている方向の角度を求める(ラジアン角)
	float nowRadian = atan2(nowDir.x, nowDir.z);

	//移動方向へのベクトルの角度を求める(ラジアン角)
	float targetRadian = atan2(rMoveDir.x, rMoveDir.z);
	float rotateRadian = targetRadian - nowRadian;

	if (rotateRadian > M_PI)
	{
		rotateRadian -= 2 * float(M_PI);
	}
	else if (rotateRadian < -M_PI)
	{
		rotateRadian += 2 * float(M_PI);
	}

	//1回の回転速度をm_m_rotateAngle度内に収める(クランプ)
	rotateRadian = std::clamp(rotateRadian, -m_rotateAngle * ToRadians, m_rotateAngle * ToRadians);

	m_rot.y += rotateRadian;
}

void Human::UpdaetCamera()
{
	if (!m_spCameraComponent) { return; }

	const Math::Vector2& inputRotate = m_spInputComponent->GetAxis(Input::R);

	Vec3 moveVec = { -inputRotate.x,0.0f,-inputRotate.y };
	moveVec *= m_movespeed;

	float DeltaX = moveVec.x;
	float DeltaY = moveVec.z;

	Matrix mRotY, mRotX;
	mRotY.RotateY(DeltaX * ToRadians);
	m_CamMat *= mRotY;

	Vec3 AxisX = m_CamMat.GetAxisZ();
	AxisX.Normalize();
	mRotX = DirectX::XMMatrixRotationAxis(AxisX, DeltaY * ToRadians);
	m_CamMat *= mRotX;

	m_CamMat.SetTranslation(m_pos.x, m_pos.y , m_pos.z);
	m_spCameraComponent->SetCameraMatrix(m_CamMat);
}

void Human::UpdateCollision()
{
	float distanceFromGround = FLT_MAX;

	CheckBump();

	//下方向への判定を行い、着地した
	if (CheckGround(distanceFromGround))
	{
		//地面の上にy座標を移動
		m_pos.y += s_allowToStepHeight - distanceFromGround;
		//地面があるので、y方向への移動量は0に
		m_force.y = 0.0f;
		return;
	}

}
void Human::CheckBump()
{
	SphereInfo info;
	
	info.m_pos = m_pos;		//中心点キャラクターの位置
	info.m_pos.y += 0.15f;	//キャラクターのぶつかり判定をするので、ちょっと上に持ち上げる
	info.m_radius = 0.2f;	//キャラクターの大きさに合わせて半径サイズもいい感じに設定する

	DebugLine::GetInstance().AddDebugSphereLine(info.m_pos, info.m_radius, { 0.0f,0.0f,1.0f,1.0f });

	for (auto& obj : Scene::GetInstance().GetObjects())
	{
		if (obj.get() == this) { continue; }

		SphereResult Result;
	
		if (obj->HitCheckBySphereVsMesh(info, Result))
		{
			m_pos.Move(Result.m_push);
		}
	}


}

bool Human::CheckGround(float& rDstDistance)
{

	RayInfo rayInfo;
	rayInfo.m_pos = m_pos;

	//キャラの足元からレイを発射すると地面と当たらないので少し持ち上げる(乗り越えられる段差の高さ分だけ)
	rayInfo.m_pos.y += s_allowToStepHeight;

	//落下中かもしれないので、1フレーム前の座標分も持ち上げる
	rayInfo.m_pos.y += m_prevPos.y - m_pos.y;

	//地面方向へのレイ
	rayInfo.m_dir = { 0.0f,-1.0f,0.0f };
	rayInfo.m_maxRange = FLT_MAX;

	KdRayResult finalRayResult;

	//当たったキャラクター
	std::shared_ptr<GameObject>hitObj = nullptr;

	//全員とレイ判定
	for (auto& obj : Scene::GetInstance().GetObjects())
	{
		if (obj.get() == this) { continue; }

		//ステージと当たり判定(背景オブジェクト以外に乗るときは変更の可能性あり)
		if (!(obj->GetTag() & (TAG_StageObject))) { continue; }
		KdRayResult rayResult;

		if (obj->HitCheckByRay(rayInfo, rayResult))
		{
			//最も当たったところまでの距離が近いものを保持する
			if (rayResult.m_distance < finalRayResult.m_distance)
			{
				finalRayResult = rayResult;
				hitObj = obj;
			}
		}
	}

	//補正分の長さを結果に反映＆着地判定

	float distanceFromGround = FLT_MAX;

	//足元にステージオブジェクトがあった
	if (finalRayResult.m_hit)
	{
		//地面との距離を算出
		distanceFromGround = finalRayResult.m_distance - (m_prevPos.y - m_pos.y);
	}

	if (m_force.y > 0.0f)
	{
		m_isGround = false;
	}
	else
	{
		//地面からの距離が(歩いて乗り越えられる高さ＋地面から足が離れていても着地判定する高さ)未満であれば着地とみなす
		m_isGround = (distanceFromGround < (s_allowToStepHeight + s_landingHeight));
	}

	//地面との距離を格納
	rDstDistance = distanceFromGround;

	//動くものの上に着地した時の判定
	if (hitObj)
	{
		/*x' = x * cosθ - y * sinθ
		  y' = x * sinθ + y * cosθ*/
		auto mOneMove = hitObj->GetOneMove();
		auto vOneMove = mOneMove.GetTranslation();
			
		if (hitObj->Getname() == "RotObj")
		{
			vOneMove *= 0;
		}

		ImGui::Text(hitObj->GetName());
		ImGui::DragFloat3(u8"", &vOneMove.x, 0.1f);
		m_pos.Move(vOneMove);
		Vec3 dir = m_mWorld.GetTranslation() - hitObj->GetMatrix().GetTranslation();
		
		if(m_HipDrop){hitObj->SetHit(true);}

	}

	//着地したかどうかを返す
	return m_isGround;

}

void Human::Draw2D()
{

	m_ReticleMat.CreateScalling(m_Scale, m_Scale, m_Scale);
	SHADER.m_spriteShader.SetMatrix(m_ReticleMat);
	SHADER.m_spriteShader.DrawTex(m_spReticleTex.get(), 0, 0);

}