#pragma once
#include"../Game/GameObject.h"

/////////////////////////////////////////////////
//モデルコンポーネント
////////////////////////////////////////////////

class ModelComponent
{
public:
	ModelComponent(GameObject& owner) : m_owner(owner) {}

	//有効フラグ
	bool IsEnable()const { return m_enable; }
	void SetEnable(bool enable) { m_enable = enable; }

	//モデル取得
	//inline const std::shared_ptr<KdModel>GetModel()const { return m_spModel; }
	const std::vector<Model::Node>& GetNodes()const { return m_coppiedNodes; }

	//メッシュ取得
	inline const std::shared_ptr<Mesh>GetMesh(UINT index)const
	{
		if (index>= m_coppiedNodes.size()) { return nullptr; }
		return m_coppiedNodes[index].m_spMesh;
	}

	inline Model::Node* FindNode(const std::string& name)
	{
		/*if (!m_spModel) { return nullptr; }
		return m_spModel->FindNode(name);*/

		for (auto&& node : m_coppiedNodes)
		{
			if (node.m_name == name)
			{
				return &node;
			}
		}
		return nullptr;
	}

	const std::shared_ptr<AnimationData>GetAnimation(const std::string& animName)const
	{
		if (!m_spModel) { return nullptr; }
		return m_spModel->GetAnimation(animName);
	}

	std::vector<Model::Node>& GetChangeableNodes() { return m_coppiedNodes; }

	//モデルセット
	//inline void SetModel(const std::shared_ptr<KdModel> model) { m_spModel = model; }

	void SetModel(const std::shared_ptr<Model>& rModel);
	//StsndardShaderで描画
	void Draw();
	// シャドウマップ生成描画
	void DrawShadowMap();

	// ディゾルブの閾値設定
	void SetDissolveThreshold(float threshold)
	{
		m_dissolveThreshold = threshold;
	}


private:

	std::vector<Model::Node>m_coppiedNodes;	//個別管理のためオリジナルからコピーして保持する配列

	//有効
	bool m_enable = true;

	//モデルデータの参照
	std::shared_ptr<Model>m_spModel;
	GameObject& m_owner;

	// 描画設定
	float			m_dissolveThreshold = 0.0f;
};