#include"ModelComponent.h"

void ModelComponent::SetModel(const std::shared_ptr<Model>& rModel)
{
	//使用しているモデルをセット
	m_spModel = rModel;

	//念のためコピー用配列のクリア
	m_coppiedNodes.clear();

	//ノードのコピー
	if (rModel)
	{
		m_coppiedNodes = rModel->GetOriginalNodes();
	}
}

void ModelComponent::Draw()
{
	//有効じゃないときはスキップ
	if (m_enable == false) { return; }

	//モデルがないときはスキップ
	if (m_spModel == false) { return; }

	// 描画設定
	SHADER.m_modelShader.SetDissolveThreshold(m_dissolveThreshold);

	//全てのノードを一つ一つ描画
	for (UINT i = 0; i < m_coppiedNodes.size(); i++)
	{
		auto& rNode = m_coppiedNodes[i];
		if (rNode.m_spMesh == nullptr) { continue; }
	
		// 行列セット
		SHADER.m_modelShader.SetWorldMatrix(rNode.m_localTransform * m_owner.GetMatrix());

		// 描画
		SHADER.m_modelShader.DrawMesh(rNode.m_spMesh.get(), m_spModel->GetMaterials());
	}

}

void ModelComponent::DrawShadowMap()
{
	// 有効じゃないときはスキップ
	if (m_enable == false)return;
	// モデルがないときはスキップ
	if (m_spModel == nullptr)return;


	// 全ノード(メッシュ)を描画
	for (UINT i = 0; i < m_coppiedNodes.size(); i++)
	{
		auto& rNode = m_coppiedNodes[i];

		// メッシュがない場合はスキップ
		if (rNode.m_spMesh == nullptr) { continue; }

		// 行列セット
		SHADER.m_genShadowMapShader.SetWorldMatrix(rNode.m_localTransform * m_owner.GetMatrix());

		// 描画
		SHADER.m_genShadowMapShader.DrawMeshDepth(rNode.m_spMesh.get(), m_spModel->GetMaterials());
	}
}
