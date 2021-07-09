#include "Model.h"
#include"GLTFLoader.h"

//コンストラクタ
Model::Model()
{

}


Model::~Model()
{
	Relese();
	/*if (m_spMesh)
	{
		m_spMesh.reset();
	}*/
}

//ロード関数
bool Model::Load(const std::string& filename)
{
	//ファイルの完全パスを取得
	std::string fileDir = KdGetDirFromPath(filename);

	//GLTFの読み込み
	std::shared_ptr<GLTFModel>spGltfModel = KdLoadGLTFModel(filename);
	if(spGltfModel == nullptr) { return false; }

	//ノード格納場所のメモリ確保
	m_originalNodes.resize(spGltfModel->Nodes.size());

	//メッシュの受け取り
	for (UINT i = 0; i < spGltfModel->Nodes.size(); i++)
	{
		//入力元ノード
		const GLTFNode& rSrcNode = spGltfModel->Nodes[i];
		
		//出力先ノード
		Node& rDstNone = m_originalNodes[i];

		//ノード情報のセット
		rDstNone.m_name = rSrcNode.Name;
		rDstNone.m_localTransform = rSrcNode.LocalTransform;

		if (rSrcNode.IsMesh)
		{
			//リストにメッシュのインスタンス化
			rDstNone.m_spMesh = std::make_shared<Mesh>();

			//メッシュ情報の作成
			if (rDstNone.m_spMesh)
			{
				rDstNone.m_spMesh->Create(rSrcNode.Mesh.Vertices, rSrcNode.Mesh.Faces, rSrcNode.Mesh.Subsets);
			}
		}
	}

	//マテリアル配列を受け取れるサイズのメモリを確保
	m_materials.resize(spGltfModel->Materials.size());

	for (UINT i = 0; i < m_materials.size(); ++i)
	{
		const GLTFMaterial& rSrcMaterial = spGltfModel->Materials[i];
		Material& rDstMaterial = m_materials[i];

		//名前
		rDstMaterial.Name = rSrcMaterial.Name;

		//基本色
		rDstMaterial.BaseColor = rSrcMaterial.BaseColor;
		rDstMaterial.BaseColorTex = std::make_shared<Texture>();

		rDstMaterial.BaseColorTex = ResFac.GetTexture(fileDir + rSrcMaterial.BaseColorTexture);
		if (rDstMaterial.BaseColorTex == nullptr) {
			rDstMaterial.BaseColorTex = D3D.GetWhiteTex();
		}

		// 金属性・粗さ
		rDstMaterial.Metallic = rSrcMaterial.Metallic;
		rDstMaterial.Roughness = rSrcMaterial.Roughness;
		rDstMaterial.MetallicRoughnessTex = std::make_shared<Texture>();
		
		// 法線マップ
		rDstMaterial.NormalTex = std::make_shared<Texture>();
		if (rDstMaterial.NormalTex->Load(fileDir + rSrcMaterial.NormalTexture) == false)
		{
			// 読み込めなかった場合は、代わりにZ向き法線マップを使用
			rDstMaterial.NormalTex = D3D.GetNormalTex();
		}

		if (rDstMaterial.MetallicRoughnessTex->Load(fileDir + rSrcMaterial.MetallicRoughnessTexture) == false)
		{
			// 読み込めなかった場合は、代わりに白画像を使用
			rDstMaterial.MetallicRoughnessTex = D3D.GetWhiteTex();
		}
	}

	//アニメーションデータ
	m_spAnimations.resize(spGltfModel->Animations.size());

	for (UINT i = 0; i < m_spAnimations.size(); ++i)
	{
		const GLTFAnimationData& rSrcAnimation = *spGltfModel->Animations[i];

		m_spAnimations[i] = std::make_shared<AnimationData>();
		AnimationData& rDstAnimation = *(m_spAnimations[i]);

		rDstAnimation.m_name = rSrcAnimation.m_name;

		rDstAnimation.m_maxLength = rSrcAnimation.m_maxLength;

		rDstAnimation.m_nodes.resize(rSrcAnimation.m_nodes.size());

		for (UINT i = 0; i < rDstAnimation.m_nodes.size(); ++i)
		{
			rDstAnimation.m_nodes[i].m_nodeOffset = rSrcAnimation.m_nodes[i]->m_nodeOffset;
			rDstAnimation.m_nodes[i].m_translations = rSrcAnimation.m_nodes[i]->m_translations;
			rDstAnimation.m_nodes[i].m_rotations= rSrcAnimation.m_nodes[i]->m_rotations;
			rDstAnimation.m_nodes[i].m_scales = rSrcAnimation.m_nodes[i]->m_scales;
		}
	}

	return true;
}

const std::shared_ptr<AnimationData> Model::GetAnimation(const std::string& animName) const
{
	for (auto&& anim : m_spAnimations)
	{
		if (anim->m_name == animName)
		{
			return anim;
		}
		else
		{
			return nullptr;
		}
	}
}

void Model::Relese()
{
	m_materials.clear();
	m_originalNodes.clear();
}
