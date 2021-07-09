#include "InputComponent.h"

InputComponent::InputComponent(GameObject& owner):m_owner(owner)
{
	//操作軸初期化
	for (auto& axis : m_axes)
	{
		axis = { 0.0f,0.0f };
	}

	m_buttons.fill(FREE);
}

void InputComponent::PushButton(Input::Buttons no)
{
	assert(no != Input::Buttons::BTN_MAX);

	//押している
	if (m_buttons[no] & STAY)
	{
		m_buttons[no] &= ~ENTER;		//ENTER反転とAND=ENTERだけをOFF
	}
	//押していない
	else {
		m_buttons[no] |= ENTER;			//ENTERをOR=ENTERをON
		m_buttons[no] |= STAY;			//STAYをOR=STAYをON
	}
}

void InputComponent::ReleaseButton(Input::Buttons no)
{
	assert(no != Input::Buttons::BTN_MAX);

	//押している
	if (m_buttons[no] & STAY)
	{
		m_buttons[no] &= ~ENTER;		//ENTER反転とAND=ENTERだけをOFF
		m_buttons[no] &= ~STAY;			//STAY反転とAND=STAYだけをOFF
		m_buttons[no] |=  EXIT;			//EXIT反転とAND=EXITだけをOFF
	}
	//押していない
	else {
		m_buttons[no] &= ~EXIT;			//EXIT反転とAND=EXITをOFF
	}
}

void ActionPlayerInputComponent::Update()
{
	for (auto& rAxis : m_axes)
	{
		rAxis = { 0.0f,0.0f };
	}

	if (GetAsyncKeyState('W') & 0x8000) { m_axes[Input::Axes::L].y = 1.0f;}
	if (GetAsyncKeyState('A') & 0x8000) { m_axes[Input::Axes::L].x = -1.0f; }
	if (GetAsyncKeyState('S') & 0x8000) { m_axes[Input::Axes::L].y = -1.0f; }
	if (GetAsyncKeyState('D') & 0x8000) { m_axes[Input::Axes::L].x = 1.0f; }


	if (GetAsyncKeyState(VK_UP) & 0x8000) { PushButton(Input::Buttons::L1); }
	else { ReleaseButton(Input::Buttons::L1); }
	if (GetAsyncKeyState(VK_DOWN) & 0x8000) { PushButton(Input::Buttons::R1); }
	else { ReleaseButton(Input::Buttons::R1); }

	if (!m_base)
	{
		POINT nowMousePos ;
		GetCursorPos(&nowMousePos);//マウス現在位置の取得
		m_axes[Input::R].x = (float)(m_baseMousePos.x - nowMousePos.x);
		m_axes[Input::R].y = (float)(m_baseMousePos.y - nowMousePos.y);

		m_baseMousePos.x = 640;
		m_baseMousePos.y = 360;
		SetCursorPos(m_baseMousePos.x, m_baseMousePos.y);
	}
	
	//ジャンプ処理
	if (GetAsyncKeyState(VK_SPACE) & 0x8000) 
	{ PushButton(Input::Buttons::Y); }
	else { ReleaseButton(Input::Buttons::Y); }

}
