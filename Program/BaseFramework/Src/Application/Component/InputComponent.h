#pragma once
#include"../Game/GameObject.h"

namespace Input
{
	enum Axes  //axisの複数形
	{
		L,
		R,
		AXS_MAX
	};

	enum Buttons
	{
		A,
		B,
		X,
		Y,
		L1,
		R1,
		SHIFT,
		BTN_MAX
	};
}

class InputComponent
{
public:
	
	//ボタン状態
	enum 
	{
		FREE=0x00000000,
		ENTER=0x00000001,
		STAY=0x00000002,
		EXIT=0x00000004,
	};

	InputComponent(GameObject& owner);
	
	virtual~InputComponent() {};

	virtual void Update() {};

	//操作軸取得
	inline const Math::Vector2& GetAxis(Input::Axes no)const
	{
		assert(no != Input::Axes::AXS_MAX);
		return m_axes[no];
	}

	//ボタンフラグ取得
	inline int GetButton(Input::Buttons no)const
	{
		assert(no != Input::Buttons::BTN_MAX);
		return m_buttons[no];
	}
	//ボタン押した
	void PushButton(Input::Buttons no);
	//ボタン離す
	void ReleaseButton(Input::Buttons no);

	void SetBase(bool base) { m_base=base; }

	bool m_base = false;

protected:

	//軸操作
	std::array<Math::Vector2, Input::Axes::AXS_MAX>m_axes;
	//軸操作
	std::array<int, Input::Buttons::BTN_MAX>m_buttons;
	//持ち主
	GameObject& m_owner;
};

///////////////////////////////////////////////////////////////
//キーボード用入力コンポーネント
///////////////////////////////////////////////////////////////
class PlayerInputComponent : public InputComponent
{
public:
	PlayerInputComponent(GameObject& owner) :InputComponent(owner) {}
	virtual void Update()override;

};

class ActionPlayerInputComponent:public InputComponent
{
public:
	ActionPlayerInputComponent(GameObject& rOwner) :InputComponent(rOwner)
	{
		m_baseMousePos.x = 640;
		m_baseMousePos.y = 360;
	}
	virtual void Update()override;

	POINT m_prevMousePos;
	POINT m_baseMousePos;

private:

};