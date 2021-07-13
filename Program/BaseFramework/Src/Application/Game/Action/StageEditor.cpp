#include"StageEditor.h"
#include"Title.h"
#include"Human.h"
#include"../Scene.h"
#include"../../main.h"
#include"../../Component//CameraComponent.h"

StageEditor::StageEditor()
{
	Deserialize();
}

StageEditor::~StageEditor()
{
}

void StageEditor::Deserialize()
{
	m_spReplayTex = ResFac.GetTexture("Data/Texture/CharacterUpdate/Hp.png");
	m_spReturnTex = ResFac.GetTexture("Data/Texture/CharacterUpdate/Return.png");

	m_PoworPos.x = 500;
	m_PoworPos.y = -270;

	m_ReturnPos.x = 0;
	m_ReturnPos.y = 0;

	m_spTitle = std::make_shared<Title>();

	if (m_spCameraComponent)
	{
		m_spCameraComponent->OffsetMatrix().CreateTranslation(0.0f, 0.0f, 0.2f);
		m_spCameraComponent->OffsetMatrix().RotateX(5.0f * ToRadians);
		m_spCameraComponent->OffsetMatrix().RotateY(270.0f * ToRadians);
	}

}

void StageEditor::Update()
{

	//if(m_spTitle->)
	POINT nowMousePos;
	HWND hwnd;
	hwnd = APP.m_window.GetWndHandle();

	GetCursorPos(&nowMousePos);				//マウス現在位置の取得
	ScreenToClient(hwnd, &nowMousePos);		//マウスをクライアント座標に

	nowMousePos.x -= 640;
	nowMousePos.y -= 360;

	MousePos.x = nowMousePos.x;
	MousePos.y = nowMousePos.y;

	auto slectObject = m_wpImguiSelectObj.lock();

	if (ImGui::Begin("Scene"))
	{
		if (ImGui::CollapsingHeader("Object List", ImGuiTreeNodeFlags_DefaultOpen))
		{
			for (auto&& rObj : Scene::GetInstance().GetObjects())
			{
				//選択オブジェクトと一致するオブジェクトかどうか
				bool slected = (rObj == slectObject);

				ImGui::PushID(rObj.get());

				if (ImGui::Selectable(rObj->GetName(), slected))
				{
					m_wpImguiSelectObj = rObj;
				}

				ImGui::PopID();
			}
		}
	}
	ImGui::End();

	//インスペクターウィンドウ
	if (ImGui::Begin("Inspector"))
	{
		if (slectObject)
		{
			slectObject->ImguiUpdate();
			slectObject->AddImguiUpdate();
			m_ObjMat = slectObject->GetMatrix();
			m_name = slectObject->GetName();
			m_RotAngle = slectObject->GetRotation();
			m_force = slectObject->GetForce();
			ImGui::DragFloat3(u8"", &m_RotAngle.x, 0.1f);
		}
	}
	ImGui::End();

	ImGuiUpdate();
}

void StageEditor::ImGuiUpdate()
{
	MakeObject();
	Camera();
}

void StageEditor::Camera()
{
	int flgs = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_OpenOnArrow;
	bool TreeOpen;

	TreeOpen = ImGui::TreeNodeEx((void*)1, flgs, u8"カメラ");
	if (TreeOpen)
	{
		if (ImGui::Button(u8"カメラ1"))
		{
			m_CamMat.CreateRotationZ(0 * ToRadians);
			m_CamMat.SetTranslation(14,15, 0);
			m_spCameraComponent->SetCameraMatrix(m_CamMat);
			Scene::GetInstance().SteTargetCamera(m_spCameraComponent);
		}
		if (ImGui::Button(u8"カメラ2"))
		{
			m_CamMat.CreateRotationZ(90 * ToRadians);
			m_CamMat.SetTranslation(-2.5, 32, 0);
			m_spCameraComponent->SetCameraMatrix(m_CamMat);
			Scene::GetInstance().SteTargetCamera(m_spCameraComponent);
		}
		ImGui::Checkbox("EditorCamera", &EditorCamera);
		{

			Scene::GetInstance().EditorCameraOnOff(EditorCamera);
		}

		ImGui::TreePop();
	}
}

void StageEditor::MakeObject()
{
	if (ImGui::Begin("Factory"))
	{
		if (ImGui::Button(u8"Stage"))
		{
			SaveName = "TestActionGame";
		}
		ImGui::InputText("StageName", &SaveName);
		ImGuiShowHelp(u8"ステージの名前:Stage_＋数字\nAddStage:新規登録\nReLoad:StageNameに記したステージに移動", false);

		if (ImGui::Button(u8"AddStage"))
		{
			SaveObject("Data/Scene/Stage/" + SaveName + ".json");
		}

		if (ImGui::Button("ReLoad"))
		{
			ResFac.Clear();
			Scene::GetInstance().RequestChangeScene("Data/Scene/Stage/"+SaveName+".json");
		}

		ImGui::InputText("ObjectName", &m_ObjName);
		ImGuiShowHelp(u8"オブジェクトの名前\AddObject:ステージに出してみる\nLoadObject:Json書き込み", false);

		if (ImGui::Button(u8"AddObject"))
		{
			m_Object = "Data/Scene/" + m_ObjName + ".json";
			WhereStageAdd();
		}

		if (ImGui::Button("LoadObject"))
		{
			SetInfo();
		}
		int flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_OpenOnArrow;
		bool ObjList;
		ObjList = ImGui::TreeNodeEx((void*)1, flags, "ObjectList");
		if (ObjList)
		{
			if (ImGui::Button(u8"NormalObj"))
			{
				m_ObjName = "NormalObj";
			}
			if (ImGui::Button(u8"LiftObj"))
			{
				m_ObjName = "LiftObj";
			}
			if (ImGui::Button(u8"RotObj"))
			{
				m_ObjName = "RotObj";
			}
			if (ImGui::Button(u8"ElevatorObj"))
			{
				m_ObjName = "ElevatorObj";
			}
			ImGui::TreePop();
		}
	}
	ImGui::End();

}

void StageEditor::SetInfo()
{
	json11::Json::object object;
	json11::Json json = ResFac.GetJSON(m_Object);

	json11::Json::array pos(3);
	pos[0] = m_ObjMat.GetTranslation().x;
	pos[1] = m_ObjMat.GetTranslation().y;
	pos[2] = m_ObjMat.GetTranslation().z;

	json11::Json::array scale(3);
	scale[0] = m_ObjMat.GetAxisX().Length();
	scale[1] = m_ObjMat.GetAxisY().Length();
	scale[2] = m_ObjMat.GetAxisZ().Length();

	json11::Json::array rot(3);
	rot[0] = m_RotAngle.x;
	rot[1] = m_RotAngle.y;
	rot[2] = m_RotAngle.z;

	json11::Json::array force(3);
	force[0] = m_force.x;
	force[1] = m_force.y;
	force[2] = m_force.z;

	object["ClassName"] = json["ClassName"].string_value();
	object["ModelFileName"] = json["ModelFileName"].string_value();
	object["Name"] = m_name;
	object["Pos"] = pos;
	object["Scale"] = scale;
	object["Rot"] = rot;
	object["Tag"] = json["Tag"].int_value();
	object["Force"] = force;

	SaveObject(object, "Data/Scene/Stage/" + SaveName + ".json");

}

void StageEditor::SaveObject(json11::Json::object object, std::string StgName)
{
	path = StgName;

	std::ofstream ofs(path, std::ios::in | std::ios::ate);
	if (ofs)
	{
		json11::Json jsn = object;

		std::string strJson = jsn.dumpDebug();

		ofs.seekp(-5, std::ios::end);

		std::string str = ",\n";
		ofs.write(str.c_str(), str.size());

		ofs.write(strJson.c_str(), strJson.size());

		str = "\n]\n}";
		ofs.write(str.c_str(), str.size());
	}
}

void StageEditor::SaveObject(std::string StgName)
{
	std::ofstream ofs(StgName);
	if (ofs)
	{
		json11::Json::array objects;
		json11::Json::object object;
		object["GameObjects"];

		json11::Json jsn = object;

		std::string strJson = jsn.dumpDebug();

		ofs.write(strJson.c_str(), strJson.size());

		ofs.seekp(-7, std::ios::end);

		json11::Json::object obj;

		obj["ClassName"] = "GameObject";
		obj["ModelFileName"] = "Data/Box/BoxStage.gltf";
		obj["Name"] = "StageMap";
		obj["Pos"] = "[0,0,0 ]";
		obj["Scale"] = "[1,1,1]";
		obj["Tag"] = 4;
		objects.push_back(obj);

		json11::Json::object obj0;
		json11::Json::array pos(3);
		pos[0] = 14;
		pos[1] = 10.5;
		pos[2] = 0;
		obj0["ClassName"] = "GameObject";
		obj0["ModelFileName"] = "Data/Box/NormalObj.gltf";
		obj0["Name"] = "NormalObj";
		obj0["Pos"] = pos;
		obj0["Tag"] = 4;
		objects.push_back(obj0);

		json11::Json::object obj1;
		obj1["ClassName"] = "Human";
		obj1["Name"] = "PlayerHuman";
		obj1["Prefab"] = "Data/Scene/Human.json";
		obj1["Tag"] = 3;
		objects.push_back(obj1);

		json11::Json::object obj2;
		obj2["ClassName"] = "ActionGameProcess" ;
		obj2["Name"] = "GameProcess" ;
		obj2["Time"] ="0" ;
		obj2["TimeShow"] = "[0, 3, 0]" ;
		objects.push_back(obj2);

		json11::Json json = objects;
		std::string Json = json.dumpDebug();
		ofs.write(Json.c_str(), Json.size());

		std::string str;

		str = "\n}";

		ofs.write(str.c_str(), str.size());
	}
}

void StageEditor::WhereStageAdd()
{
	json11::Json json = ResFac.GetJSON(m_Object);

	auto newGameObj = CreateGameObject(json["ClassName"].string_value());

	newGameObj->Deserialize(json);

	Scene::GetInstance().AddObject(newGameObj);
}



void StageEditor::Draw2D()
{
	m_PoworMat.SetTranslation(m_PoworPos);
	SHADER.m_spriteShader.SetMatrix(m_PoworMat);
	SHADER.m_spriteShader.DrawTex(m_spReplayTex.get(), 0, 0);

	m_ReturnMat.SetTranslation(m_ReturnPos);
	SHADER.m_spriteShader.SetMatrix(m_ReturnMat);
	SHADER.m_spriteShader.DrawTex(m_spReturnTex.get(), 0, 0);
}

bool StageEditor::Collision()
{
	if (Collision2D(MousePos, m_PoworPos, 200, 100))
	{
		if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
		{
			test = 1;

			return false;
		}
	}

	if (Collision2D(m_ReturnPos, m_ReturnPos, 100, 100))
	{
		if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
		{
			return true;
		}
	}

	return false;

}

void StageEditor::LoadJson()
{
	std::ifstream ifs("Data/Scene/Human.json");
	if (ifs.fail()) { assert(0 && "jsonパスが違う"); };

	std::string strjson((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());

	std::string err;
	json11::Json jsonobj = json11::Json::parse(strjson, err);
	if (err.size() > 0) { assert(0 && "json変換失敗"); };

	std::vector<json11::Json>& rPos = jsonobj["Pos"].array_items();
	m_pos.x = (float)rPos[0].number_value();

}
