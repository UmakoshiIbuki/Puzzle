#include"ResourceFactory.h"

std::shared_ptr<Model> ResourceFactory:: GetModel(const std::string& filename)
{
	//filenameのモデルがあるか？
	auto itFound = m_modelMap.find(filename);
	//ない場合
	if (itFound == m_modelMap.end())
	{
		//生成＆読み込み
		auto newModel = std::make_shared<Model>();
		if (newModel->Load(filename) == false)
		{
			//読み込み失敗時はnullを返す
			return nullptr;
		}
		//リスト(map)に登録
		m_modelMap[filename] = newModel;
		//リソースを返す
		return newModel;
	}
	//ある場合
	else
	{
		return (*itFound).second;
	}
}

std::shared_ptr<Texture> ResourceFactory::GetTexture(const std::string& filename)
{
	//filenameのモデルがあるか？
	auto itFound = m_texMap.find(filename);
	//ない場合
	if (itFound == m_texMap.end())
	{
		//生成＆読み込み
		auto newTexture = std::make_shared<Texture>();
		if (newTexture->Load(filename) == false)
		{
			//読み込み失敗時はnullを返す
			return nullptr;
		}
		//リスト(map)に登録
		m_texMap[filename] = newTexture;
		//リソースを返す
		return newTexture;
	}
	//ある場合
	else
	{
		return (*itFound).second;
	}


}

json11::Json ResourceFactory::GetJSON(const std::string& filename)
{
	//filenameのモデルがあるか？
	auto itFound = m_jsonMap.find(filename);
	//ない場合
	if (itFound == m_jsonMap.end())
	{
		//生成＆読み込み
		json11::Json json = LoadJSON(filename);
		if (json.is_null())
		{
			assert(0 && "[GetJSON]jsonファイルが見つからない");
			return nullptr;
		}
		//登録
		m_jsonMap[filename] = json;
		//リソースを返す
		return json;
	}
	//ある場合
	else
	{
		return (*itFound).second;
	}
}

std::shared_ptr<SoundEffect> ResourceFactory::GetSound(const std::string& filename)
{
	//filenameのモデルがあるか？
	auto itFound = m_soundMap.find(filename);
	//ない場合
	if (itFound == m_soundMap.end())
	{
		std::wstring wFilename = sjis_to_wide(filename);
		auto newSound = std::make_shared<SoundEffect>();
		if (!newSound->Load(filename))
		{
			return nullptr;
		}
		m_soundMap[filename] = newSound;
		return newSound;
	}
	//ある場合
	else
	{
		return (*itFound).second;
	}
}

//Json読み込み
inline json11::Json ResourceFactory::LoadJSON(const std::string& filename)
{
	//Jsonファイルを開く
	std::ifstream ifs(filename);
	if (ifs.fail())
	{
		assert(0 && "Jsonファイルが間違ってます");
		return nullptr;
	}

	//文字列として読み込み
	std::string strJson((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());

	//文字列のJsonを解析(パース)する
	std::string err;

	json11::Json jsonObj = json11::Json::parse(strJson, err);
	if (err.size() > 0)
	{
		assert(0 && "読み込んだファイルのJson変換に失敗");
		return nullptr;
	}

	return jsonObj;
}

