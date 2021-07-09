#include "System/KdSystem.h"

#include "Utility.h"

bool CoolTime(int Time)
{
	Time--;
	if (Time < 0) { return true;}
	return false;
}

// viewから画像情報を取得する
void GetTextureInfo(ID3D11View* view, D3D11_TEXTURE2D_DESC& outDesc)
{
	outDesc = {};

	ID3D11Resource* res;
	view->GetResource(&res);

	ID3D11Texture2D* tex2D;
	if (SUCCEEDED(res->QueryInterface<ID3D11Texture2D>(&tex2D)))
	{
		tex2D->GetDesc(&outDesc);
		tex2D->Release();
	}
	res->Release();
}

void MergePrefab(json11::Json& rSrcJson)
{
	//プレハブありの場合は、プレハブ側のものをベースにこのJsonをマージする
	std::string prefabFilename = rSrcJson["Prefab"].string_value();
	if (prefabFilename.size() > 0)
	{
		//Json読み込み
		json11::Json prefJson = ResFac.GetJSON(prefabFilename);

		if (prefJson.is_null() == false)
		{
			json11::Json copyPrefab = prefJson.object_items();
			//マージする
			for (auto&& n : rSrcJson.object_items())
			{
				copyPrefab[n.first] = n.second;
			}

			//マージしたものに差し替え
			rSrcJson = copyPrefab;
		}
	}
}

RestoreRenderTarget::RestoreRenderTarget()
{
	D3D.GetDevContext()->OMGetRenderTargets(1, &m_pSaveRT, &m_pSaveZ);
}

RestoreRenderTarget::~RestoreRenderTarget()
{
	D3D.GetDevContext()->OMSetRenderTargets(1, &m_pSaveRT, m_pSaveZ);
	m_pSaveRT->Release();
	m_pSaveZ->Release();
}

float RestoreRenderTarget::easeIn(float progress)
{
	return 1.0f - cos((progress * M_PI) * 0.5f);
}

float RestoreRenderTarget::easeInOut(float progress)
{
	return -(cos(M_PI * progress) - 1) * 0.5f;
}