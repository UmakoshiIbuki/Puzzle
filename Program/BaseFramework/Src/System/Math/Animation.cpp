#include "Animation.h"

template<class T>
int BinarySearchNextAnimKey(const std::vector<T>& list, float time)
{
	int low = 0;
	int high = (int)list.size();
	while (low < high)
	{
		int mid = (low + high) / 2;
		float midTime = list[mid].m_time;

		if (midTime <= time)low = mid + 1;
		else high = mid;
	}
	return low;
}

void AnimationData::Node::Interpolate(Matrix& rDst, float time)
{
	//クォータニオンによる回転補完
	Matrix rotate;
	KdQuaternion resultQuat;
	if (InterpolateRotations(resultQuat, time))
	{
		rotate.CreateFromQuaternion(resultQuat);
	}

	Matrix scale;
	Vec3 resultRot;
	if (InterpolateScales(resultRot, time))
	{
		scale.CreateScalling(resultRot);
	}

	//ベクターによる座標補完
	Matrix trans;
	Vec3 resultVec;
	if (InterpolateTranslations(resultVec, time))
	{
		trans.CreateTranslation(resultVec);
	}
	rDst = scale * rotate * trans;
}

bool AnimationData::Node::InterpolateTranslations(Vec3& resurt, float time)
{
	if (m_translations.size() == 0) return false;

	UINT keyIdx = BinarySearchNextAnimKey(m_translations, time);

	if (keyIdx == 0)
	{
		resurt = m_translations.front().m_vec;
		return true;
	}
	else if (keyIdx >= m_translations.size())
	{
		resurt = m_translations.back().m_vec;
		return true;
	}
	else
	{
		auto& prev = m_translations[keyIdx - 1];
		auto& next = m_translations[keyIdx];

		float f = (time - prev.m_time) / (next.m_time - prev.m_time);

		resurt = DirectX::XMVectorLerp(prev.m_vec, next.m_vec, f);
	}
	return true;
}

bool AnimationData::Node::InterpolateRotations(KdQuaternion& resurt, float time)
{
	if (m_rotations.size() == 0) return false;

	UINT keyIdx = BinarySearchNextAnimKey(m_rotations, time);

	if (keyIdx == 0)
	{
		resurt = m_rotations.front().m_quat;
	}
	else if (keyIdx >= m_rotations.size())
	{
		resurt = m_rotations.back().m_quat;
	}
	else
	{
		auto& prev = m_rotations[keyIdx - 1];
		auto& next = m_rotations[keyIdx];

		float f = (time - prev.m_time) / (next.m_time - prev.m_time);

		resurt = DirectX::XMQuaternionSlerp(prev.m_quat, next.m_quat, f);
	}
	return true;
}

bool AnimationData::Node::InterpolateScales(Vec3& resurt, float time)
{
	if (m_scales.size() == 0) return false;

	UINT keyIdx = BinarySearchNextAnimKey(m_scales, time);

	if (keyIdx == 0)
	{
		resurt = m_scales.front().m_vec;
		return true;
	}
	else if (keyIdx >= m_scales.size())
	{
		resurt = m_scales.back().m_vec;
		return true;
	}
	else
	{
		auto& prev = m_scales[keyIdx - 1];
		auto& next = m_scales[keyIdx];

		float f = (time - prev.m_time) / (next.m_time - prev.m_time);

		resurt = DirectX::XMVectorLerp(prev.m_vec, next.m_vec, f);
	}
	return true;
}

void Animator::AdvanceTime(std::vector<Model::Node>& rNodes, float speed)
{
	if (!m_spAnimation) { return; }
		for (auto& rAnimNode : m_spAnimation->m_nodes)
		{
			UINT idx = rAnimNode.m_nodeOffset;
			rAnimNode.Interpolate(rNodes[idx].m_localTransform, m_time);
		}
		m_time += speed;

		if (m_time >= m_spAnimation->m_maxLength)
		{
			if (m_isLoop)
			{
				m_time = 0.0f;
			}
			else
			{
				m_time = m_spAnimation->m_maxLength;
			}
		}
}
