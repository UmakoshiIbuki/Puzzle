#include "Lamp.h"
#include"../Scene.h"

void Lamp::Deserialize(const json11::Json& jsonObj)
{
	GameObject::Deserialize(jsonObj);
	const std::vector<json11::Json>& rColor = jsonObj["Color"].array_items();
	m_color = { (float)rColor[0].number_value(), (float)rColor[1].number_value(), (float)rColor[2].number_value() };
	m_light = jsonObj["LightPosY"].int_value();
	m_lightPower = jsonObj["LightPower"].int_value();

}

void Lamp::Update()
{
	Vec3 pos = m_mWorld.GetTranslation();
	pos.y += m_light;
	SHADER.AddPointLight(pos, m_lightPower, m_color);
	//ImGuiUpdate();
}

void Lamp::ImGuiUpdate()
{

	if (ImGui::Begin("LightSettings"))
	{

		if (ImGui::DragFloat("Light", &m_light, 0.01f))
		{
			m_light=m_light;
		}
		if (ImGui::DragFloat("LightSize", &m_lightPower, 0.01f))
		{
			m_lightPower = m_lightPower;
		}
	}
	ImGui::End();
}
