#pragma once

//3Dベクトル
class Vec3 :public DirectX::XMFLOAT3
{
public:

	// XMFLOAT3から代入してきた時
	Vec3(const DirectX::XMFLOAT3& V)
	{
		x = V.x;
		y = V.y;
		z = V.z;
	}

	inline void Complement(const Vec3& vTo, float rot)
	{
		Vec3  vRotAxis = Vec3::Cross(*this, vTo);
		if (vRotAxis.LengthSquared() != 0)
		{
			//自分のZ方向とベクトルと自身からターゲットへ向かうベクトルの内積
			float d = Vec3::Dot(*this, vTo);

			//誤差で‐1～1以外になる可能性大なので、クランプする
			d = std::min(std::max(d, -1.0f), 1.0f);

			//自分の前方向ベクトルと自身からターゲットへ向かうベクトル間の角度(radian)
			float radian = acos(d);

			radian = std::min(radian, rot * ToRadians);

			*this=TransformNormal(DirectX::XMMatrixRotationAxis(vRotAxis, radian));
		}
	}

	//指定行列でVectorを変換する
	Vec3& TransformCoord(const DirectX::XMMATRIX& m)
	{
		*this = XMVector3TransformCoord(*this, m);
		return *this;
	}
	
	//指定(回転)行列でVectorを変換する
	Vec3& TransformNormal(const DirectX::XMMATRIX& m)
	{
		*this = XMVector3TransformNormal(*this, m);
		return *this;
	}

	//デフォルトコンストラクタは座標の0クリアを行う
	Vec3()
	{
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
	}

	Vec3(float _x, float _y, float _z)
	{
		x = _x;
		y = _y;
		z = _z;
	}

	//XMVECTORから代入してきたとき
	Vec3(const DirectX::XMVECTOR& v)
	{
		//変換して代入
		DirectX::XMStoreFloat3(this, v);
	}
	//XMVECTORへ変換
	operator DirectX::XMVECTOR() const { return DirectX::XMLoadFloat3(this); }

	//Math::Vector3と互換性を持つための関数
	operator Math::Vector3& () { return *(Math::Vector3*)this; }

	//算術演算子 乗算(*)
	Vec3& operator*=(float s)
	{
		*this = DirectX::XMVectorScale(*this, s);
		return *this;
	}

	//自分を正規化
	Vec3& Normalize()
	{
		*this = DirectX::XMVector3Normalize(*this);
		return *this;
	}

	//長さ
	float Length() const
	{
		return DirectX::XMVector3Length(*this).m128_f32[0];
	}

	//長さの2乗(高速なので判定用に使用することが多い)
	float LengthSquared() const
	{
		return DirectX::XMVector3LengthSq(*this).m128_f32[0];
	}

	//内積
	static float Dot(const Vec3 v1, const Vec3& v2)
	{
		return DirectX::XMVector3Dot(v1, v2).m128_f32[0];
	}
	//外積
	static Vec3 Cross(const Vec3 v1, const Vec3& v2)
	{
		return DirectX::XMVector3Cross(v1, v2);
	}

	Vec3 Move(const Vec3 v1)
	{
		this->x += v1.x;
		this->y += v1.y;
		this->z += v1.z;
		return *this;
	}

	Vec3 Move(float x,float y,float z)
	{
		this->x += x;
		this->y += y;
		this->z += z;
		return *this;
	}

	Vec3 SetPos(Vec3 v1)
	{
		this->x = v1.x;
		this->y = v1.y;
		this->z = v1.z;
		return *this;
	}

	Vec3 SetPos(float x, float y, float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		return *this;
	}
};

//4*4の行列
class Matrix :public DirectX::XMFLOAT4X4
{
public:

	//デフォルトコンストラクタ単位行列化
	Matrix()
	{
		*this = DirectX::XMMatrixIdentity();
	}

	//XMMATRIXから代入してきた
	Matrix(const DirectX::XMMATRIX& m)
	{
		DirectX::XMStoreFloat4x4(this, m);
	}

	//XMFLOAT4x4,Math::Matrixから代入してきた
	Matrix(const DirectX::XMFLOAT4X4& m)
	{
		memcpy_s(this, sizeof(float) * 16, &m, sizeof(XMFLOAT4X4));
	}

	//XMMATRIXへ変換
	operator DirectX::XMMATRIX() const
	{
		return DirectX::XMLoadFloat4x4(this);
	}

	//Math::Matrixと互換性を持つための関数
	operator Math::Matrix& () { return *(Math::Matrix*)this; }

	//代入演算子　乗算
	Matrix& operator*=(const Matrix& m)
	{
		*this = DirectX::XMMatrixMultiply(*this, m);
		return *this;
	}

	//作成========================================================

	//移動行列作成
	void CreateTranslation(float x, float y, float z)
	{
		*this = DirectX::XMMatrixTranslation(x, y, z);
	}

	void CreateTranslation(const Vec3& v)
	{
		*this = DirectX::XMMatrixTranslation(v.x, v.y, v.z);
	}

	void CreateRotationX(float angle)
	{
		*this = DirectX::XMMatrixRotationX(angle);
	}

	void CreateRotationY(float angle)
	{
		*this = DirectX::XMMatrixRotationY(angle);
	}

	void CreateRotationZ(float angle)
	{
		*this = DirectX::XMMatrixRotationZ(angle);
	}
	//拡縮行列作成
	void CreateScalling(float x, float y, float z)
	{
		*this = DirectX::XMMatrixScaling(x, y, z);
	}

	void CreateScalling(const Vec3& v)
	{
		*this = DirectX::XMMatrixScaling(v.x, v.y, v.z);
	}

	void CreateFromQuaternion(const Math::Quaternion& quat)
	{
		*this = DirectX::XMMatrixRotationQuaternion(quat);
	}

	//指定回転行列作成
	void CreateRotationAxis(const Vec3& axis, float angle)
	{
		*this = DirectX::XMMatrixRotationAxis(axis, angle);
	}

	//透視影行列の作成
	Matrix& CreateProjectionPerspacetiveFov(float fovAngleY, float aspectRatio, float nearZ, float farZ)
	{
		*this = DirectX::XMMatrixPerspectiveFovLH(fovAngleY, aspectRatio, nearZ, farZ);
		return *this;
	}

	// 正射影行列作成
	Matrix& CreateProjection_Orthographic(float viewWidth, float viewHeight, float nearZ, float farZ)
	{
		*this = DirectX::XMMatrixOrthographicLH(viewWidth, viewHeight, nearZ, farZ);
		return *this;
	}

	//操作========================================================

	void RotateX(float angle)
	{
		*this *= DirectX::XMMatrixRotationX(angle);
	}


	//Y軸回転
	void RotateY(float angle)
	{
		*this *= DirectX::XMMatrixRotationY(angle);
	}
	
	//Z軸回転
	void RotateZ(float angle)
	{
		*this *= DirectX::XMMatrixRotationZ(angle);
	}

	
	//拡縮
	void Scale(float x, float y, float z)
	{
		*this *= DirectX::XMMatrixScaling(x, y, z);
	}


	//逆行列にする
	void Inverse()
	{
		*this = DirectX::XMMatrixInverse(nullptr, *this);
	}

	//ビルボード処理
	inline void SetBillboard(const Matrix& mat)
	{		
		Matrix a = mat;
		a.SetTranslation({ 0,0,0 });
		a.Inverse();
		a.SetTranslation(mat.GetTranslation());
		*this *= a;
	}

	//移動関数
	void Move(const Vec3& v)
	{
		_41 += v.x;
		_42 += v.y;
		_43 += v.z;
	}

	void Move(float x,float y,float z)
	{
		_41 += x;
		_42 += y;
		_43 += z;
	}
	//プロパティ==================================================

	//Z軸取得
	Vec3 GetAxisX()const { return{ _11,_12,_13 }; }

	Vec3 GetAxisY()const { return{ _21,_22,_23 }; }

	Vec3 GetAxisZ()const { return{ _31,_32,_33 }; }

	//Z軸セット
	void SetAxisX(const Vec3& v)
	{
		_11 = v.x;
		_12 = v.y;
		_13 = v.z;
	}

	void SetAxisY(const Vec3& v)
	{
		_21 = v.x;
		_22 = v.y;
		_23 = v.z;
	}

	void SetAxisZ(const Vec3& v)
	{
		_31 = v.x;
		_32 = v.y;
		_33 = v.z;
	}

	//座標取得
	Vec3 GetTranslation()const { return{ _41,_42,_43 }; }

	void SetTranslation(const Vec3& v)
	{
		_41 = v.x;
		_42 = v.y;
		_43 = v.z;
	}

	void SetTranslation(float x,float y,float z)
	{
		_41 = x;
		_42 = y;
		_43 = z;
	}

	Vec3 GetAngles()const
	{
		Matrix mat = *this;

		//各軸を取得
		Vec3 axisX = mat.GetAxisX();
		Vec3 axisY = mat.GetAxisY();
		Vec3 axisZ = mat.GetAxisZ();

		//各軸を正規化
		axisX.Normalize();
		axisY.Normalize();
		axisZ.Normalize();

		//マトリックスを正規化
		mat.SetAxisX(axisX);
		mat.SetAxisY(axisY);
		mat.SetAxisZ(axisZ);

		Vec3 angles;
		angles.x = atan2f(mat.m[1][2], mat.m[2][2]);
		angles.y = atan2f(-mat.m[0][2], sqrt( mat.m[1][2]*mat.m[1][2]+mat.m[2][2]*mat.m[2][2]));
		angles.z = atan2f(mat.m[0][1], mat.m[0][0]);

		return angles;
	}

	// Z軸を指定方向に向ける
	Matrix& LookTo(const Vec3& dir, const Vec3& up)
	{
		Vec3 vZ = dir;
		vZ.Normalize();
		Vec3 vX = Vec3::Cross(up, vZ).Normalize();
		if (vX.LengthSquared() == 0)
		{
			vX = { 1,0,0 };
		}
		Vec3 vY = Vec3::Cross(vZ, vX).Normalize();

		float scaleX = GetAxisX().Length();
		float scaleY = GetAxisY().Length();
		float scaleZ = GetAxisZ().Length();

		SetAxisX(vX * scaleX);
		SetAxisY(vY * scaleY);
		SetAxisZ(vZ * scaleZ);

		return *this;
	}


};

//Matrix同士の合成
inline Matrix operator*(const Matrix& m1, const Matrix& m2)
{
	using namespace DirectX;
	return XMMatrixMultiply(m1, m2);
}

/////////////////////////////////////
//クォータニオン
/////////////////////////////////////
class KdQuaternion :public DirectX::XMFLOAT4
{
public:
	//コンストラクターで初期化代入
	KdQuaternion()
	{
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
		w = 1.0f;
	}

	//XMVectorから代入してきたとき
	KdQuaternion(const DirectX::XMVECTOR& v)
	{
		//変換して代入
		DirectX::XMStoreFloat4(this, v);

	}
	//XMVectorへ変換
	operator DirectX::XMVECTOR()const { return DirectX::XMLoadFloat4(this); }

};