#pragma once

////////////////////////////////////////////////////////////
//リソース管理クラス
//・デザインパターンのFlyweightパターンを採用
///////////////////////////////////////////////////////////

class ResourceFactory
{
public:
	//モデルデータ取得
	std::shared_ptr<Model> GetModel(const std::string& filename);

	//テクスチャデータ取得
	std::shared_ptr<Texture>GetTexture(const std::string& filename);

	//Json取得
	json11::Json GetJSON(const std::string& filename);

	std::shared_ptr<SoundEffect>GetSound(const std::string& filename);

	//管理を破棄する
	void Clear()
	{
		m_modelMap.clear();
		m_texMap.clear();
		m_jsonMap.clear();

		m_soundMap.clear();
	}

private:

	//Json読み込み
	json11::Json LoadJSON(const std::string& filename);

	std::unordered_map<std::string, std::shared_ptr<SoundEffect>>m_soundMap;

	//モデルデータ管理マップ
	std::unordered_map<std::string, std::shared_ptr<Model>>m_modelMap;
	
	//テクスチャー管理マップ
	std::unordered_map<std::string, std::shared_ptr<Texture>>m_texMap;

	//Json管理マップ
	std::unordered_map < std::string, json11::Json>m_jsonMap;

private:
	//////////////////////////////////////////////////////
	//シングルトン
	/////////////////////////////////////////////////////
public:
	static ResourceFactory& GetInstance()
	{
		static ResourceFactory inctance;
		return inctance;
	}
};

#define ResFac ResourceFactory::GetInstance()