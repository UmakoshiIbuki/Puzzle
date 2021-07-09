#pragma once
#include"../GameObject.h"

class StageSelect :public GameObject
{
public:
	virtual void Deserialize(const json11::Json& jsonObj) override;

	void Update()override;

private:

};