#pragma once
#include"../GameObject.h"

class AinmationEffect;

class Title :public GameObject
{
public:
	virtual void Deserialize(const json11::Json& jsonObj) override;

	void Update()override;

	void Draw2D()override;
	void Draw()override;

	virtual void ImguiUpdate()override;

	bool							 m_isTitleScene = true;

private:

	int frame=0;

	bool					   m_ShowStageSelect = false;

	std::shared_ptr<Texture>   m_spTitleTex;
	Matrix					   m_TitleMat;

	std::shared_ptr<Texture>   m_spTitleBackTex;
	Matrix					   m_TitleBackMat;
	float					   m_TitleBackScale =0;

	bool					   m_canChange = false;
	float                      m_black = 0.0f;
	bool                       flg=false;
	Vec3                       MousePos;
	Matrix m_mStart002;
	Vec3 m_vGoal002 = {};

	int						   flgs = 1;
	std::shared_ptr<Texture>   m_spStageWindowTex;
	Matrix					   m_StageWindowMat;

	std::shared_ptr<Texture>   m_spClickToStartTex;
	Matrix					   m_ClickToStartMat;
	float                      m_CposY=0;
	float                      m_CspeedY;

	std::vector<Matrix>		   m_Matrixs;
	std::vector<Vec3>		   m_Psos;
	std::vector<float>		   m_Scales;
	Matrix					   m_Matrix;


	int						   m_stageclearcnt=0;

	std::string				   m_stagename;

	Matrix m_CamMat;
	std::string string = "Data/StageMap/StageMap.gltf";

};