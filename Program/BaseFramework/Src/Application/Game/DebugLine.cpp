#include "DebugLine.h"


DebugLine::DebugLine()
{
}

DebugLine::~DebugLine()
{
}

void DebugLine::DebugDraw()
{
	////デバッグライン描画
	SHADER.m_effectShader.SetToDevice();
	SHADER.m_effectShader.SetTexture(D3D.GetWhiteTex()->GetSRView());
	{

		AddDebugCoordinateAxisLine(Math::Vector3(0.0f, 0.0f, 0.0f), 2.0f);

		//Zバッファ使用OFF・書き込みOFF
		D3D.GetDevContext()->OMSetDepthStencilState(SHADER.m_ds_ZDisable_ZWriteDisable, 0);

		if (m_debugLines.size() >= 1)
		{
			SHADER.m_effectShader.SetWorldMatrix(Math::Matrix());

			SHADER.m_effectShader.DrawVertices(m_debugLines, D3D_PRIMITIVE_TOPOLOGY_LINELIST);

			m_debugLines.clear();
		}

		D3D.GetDevContext()->OMSetDepthStencilState(SHADER.m_ds_ZEnable_ZWriteEnable, 0);
	}
}

//デバッグライン描画
void DebugLine::AddDebugLine(const Math::Vector3& p1, const Math::Vector3& p2, const Math::Color& color)
{
	//ラインの開始頂点
	EffectShader::Vertex ver;
	ver.Color = color;
	ver.UV = { 0.0f,0.0f };
	ver.Pos = p1;
	m_debugLines.push_back(ver);

	//ラインの終端頂点
	ver.Pos = p2;
	m_debugLines.push_back(ver);
}

//デバッグスフィア描画
void DebugLine::AddDebugSphereLine(const Math::Vector3& pos, float radius, const Math::Color& color)
{
	EffectShader::Vertex ver;
	ver.Color = color;
	ver.UV = { 0.0f,0.0f };

	static constexpr int kDetail = 32;
	for (UINT i = 0; i < kDetail + 1; ++i)
	{
		//XZ平面
		ver.Pos = pos;
		ver.Pos.x += cos((float)i * (360 / kDetail) * ToRadians) * radius;
		ver.Pos.z += sin((float)i * (360 / kDetail) * ToRadians) * radius;
		m_debugLines.push_back(ver);

		ver.Pos = pos;
		ver.Pos.x += cos((float)(i + 1) * (360 / kDetail) * ToRadians) * radius;
		ver.Pos.z += sin((float)(i + 1) * (360 / kDetail) * ToRadians) * radius;
		m_debugLines.push_back(ver);

		//XY平面
		ver.Pos = pos;
		ver.Pos.x += cos((float)i * (360 / kDetail) * ToRadians) * radius;
		ver.Pos.y += sin((float)i * (360 / kDetail) * ToRadians) * radius;
		m_debugLines.push_back(ver);

		ver.Pos = pos;
		ver.Pos.x += cos((float)(i + 1) * (360 / kDetail) * ToRadians) * radius;
		ver.Pos.y += sin((float)(i + 1) * (360 / kDetail) * ToRadians) * radius;
		m_debugLines.push_back(ver);

		//YZ平面
		ver.Pos = pos;
		ver.Pos.y += cos((float)i * (360 / kDetail) * ToRadians) * radius;
		ver.Pos.z += sin((float)i * (360 / kDetail) * ToRadians) * radius;
		m_debugLines.push_back(ver);

		ver.Pos = pos;
		ver.Pos.y += cos((float)(i + 1) * (360 / kDetail) * ToRadians) * radius;
		ver.Pos.z += sin((float)(i + 1) * (360 / kDetail) * ToRadians) * radius;
		m_debugLines.push_back(ver);
	}
}

//デバッグ座標軸描画
void DebugLine::AddDebugCoordinateAxisLine(const Math::Vector3& pos, float scale)
{
	EffectShader::Vertex ver;
	ver.Color = { 1.0f,1.0f,1.0f,1.0f };
	ver.UV = { 0.0f,0.0f };

	//X軸・赤
	ver.Color = { 1.0f,0.0f,0.0f,1.0f };
	ver.Pos = pos;
	m_debugLines.push_back(ver);

	ver.Pos.x += 1.0f * scale;
	m_debugLines.push_back(ver);

	//Y軸・緑
	ver.Color = { 0.0f,1.0f,0.0f,1.0f };
	ver.Pos = pos;
	m_debugLines.push_back(ver);

	ver.Pos.y += 1.0f * scale;
	m_debugLines.push_back(ver);

	//Z軸・青
	ver.Color = { 0.0f,0.0f,1.0f,1.0f };
	ver.Pos = pos;
	m_debugLines.push_back(ver);

	ver.Pos.z += 1.0f * scale;
	m_debugLines.push_back(ver);
}
