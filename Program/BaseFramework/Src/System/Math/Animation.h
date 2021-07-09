#pragma once

//アニメーションキー(クォータニオン)
struct AnimKeyQuaternion
{
	float m_time = 0;
	KdQuaternion m_quat;
};

//アニメーションキー(ベクトル)
struct AnimKeyVector3
{
	float m_time = 0;
	Vec3  m_vec;
};


//アニメーションデータ
struct AnimationData
{
	//アニメーション名
	std::string		m_name;
	//アニメの長さ
	float m_maxLength;

	//1ノードのアニメーションデータ
	struct Node
	{
		void Interpolate(Matrix& rDst, float time);
		bool InterpolateTranslations(Vec3& resurt, float time);
		bool InterpolateRotations(KdQuaternion& resurt, float time);
		bool InterpolateScales(Vec3& resurt, float time);

		int m_nodeOffset = -1;

		//各チャンネル
		std::vector<AnimKeyVector3>		m_translations;
		std::vector<AnimKeyQuaternion>	m_rotations;
		std::vector<AnimKeyVector3>		m_scales;

	};

	std::vector<Node> m_nodes;
};

class Animator
{
public:
	inline void SetAnimation(std::shared_ptr<AnimationData>& rData)
	{
		m_spAnimation = rData;

		m_time = 0.0f;
	}

	bool IsAnimationEnd()const
	{
		if (m_spAnimation == nullptr) { return true; }
		if (m_time >= m_spAnimation->m_maxLength) { return true; }
		return false;
	}

	//アニメーションの更新
	void AdvanceTime(std::vector<Model::Node>& rNodes, float speed = 1.0f);

private:
	std::shared_ptr<AnimationData> m_spAnimation = nullptr;	//再生するアニメーションデータ
	float m_time = 0.0f;
	bool  m_isLoop = true;
};