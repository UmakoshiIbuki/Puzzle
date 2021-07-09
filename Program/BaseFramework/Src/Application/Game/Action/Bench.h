#pragma once
#include"../GameObject.h"

class Bench :public GameObject
{
public:
	virtual void Deserialize(const json11::Json& jsonObj) override;
private:

};