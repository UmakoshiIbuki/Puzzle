#pragma once

class DebugLine
{
public:
	DebugLine();
	~DebugLine();

	static DebugLine& GetInstance()
	{
		static DebugLine inctance;
		return inctance;
	}

	void DebugDraw();
	void AddDebugLine(const Math::Vector3& p1, const Math::Vector3& p2, const Math::Color& color);
	void AddDebugSphereLine(const Math::Vector3& pos, float radius, const Math::Color& color);
	void AddDebugCoordinateAxisLine(const Math::Vector3& pos, float scale);
private:
	
	//デバッグライン描画用の頂点配列
	std::vector<EffectShader::Vertex> m_debugLines;

};