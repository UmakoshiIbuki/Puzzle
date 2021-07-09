#pragma once
#include"../GameObject.h"

class Title;

class StageEditor :GameObject
{
public:
	StageEditor();
	~StageEditor();

	void Update();
	void Deserialize();

	void ImGuiUpdate();
	void Camera();
	void MakeObject();
	void SetInfo();
	void SaveObject(json11::Json::object object, std::string StgName);
	void SaveObject(std::string StgName);
	void WhereStageAdd();

	void Draw2D();
	bool Collision();
	void LoadJson();
private:
	float test = 12;

	std::shared_ptr<Texture>   m_spReplayTex;
	Matrix					   m_PoworMat;
	Vec3					   m_PoworPos;

	std::shared_ptr<Texture>   m_spReturnTex;
	Matrix					   m_ReturnMat;
	Vec3					   m_ReturnPos;

	Vec3                       MousePos;

	std::shared_ptr<Title>	   m_spTitle;

	Vec3					   m_pos;


	std::weak_ptr<GameObject>m_wpImguiSelectObj;
	//std::shared_ptr<CameraComponent>m_spCameraComponent = std::make_shared<CameraComponent>(*this);
	//std::shared_ptr<CameraComponent>m_spCameraY = std::make_shared<CameraComponent>(*this);

	std::string filepath = "";
	std::string m_Object = "";
	std::string path = "";
	std::string SaveName = "";
	std::string m_ObjName;

	Matrix					   m_ObjMat;
	Matrix					   m_CamMat;
	bool EditorCamera=false;


};