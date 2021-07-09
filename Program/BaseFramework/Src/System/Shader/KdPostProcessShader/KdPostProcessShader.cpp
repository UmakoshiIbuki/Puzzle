#include "KdPostProcessShader.h"

bool KdPostProcessShader::Init()
{
	{
		#include "KdPostProcess_VS.inc"

		if (FAILED(D3D.GetDev()->CreateVertexShader(compiledBuffer, sizeof(compiledBuffer), nullptr, &m_VS))) {
			assert(0 && "頂点シェーダー作成失敗");
			Release();
			return false;
		}

		std::vector<D3D11_INPUT_ELEMENT_DESC> layout = {
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,		0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,			0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		if (FAILED(D3D.GetDev()->CreateInputLayout(
			&layout[0],
			layout.size(),
			compiledBuffer,
			sizeof(compiledBuffer),
			&m_inputLayout))
			) {
			assert(0 && "CreateInputLayout失敗");
			Release();
			return false;
		}
		{
			#include "KdPostProcess_Blur_PS.inc"

			if (FAILED(D3D.GetDev()->CreatePixelShader(compiledBuffer, sizeof(compiledBuffer), nullptr, &m_blurPS))) {
				assert(0 && "ピクセルシェーダー作成失敗");
				Release();
				return false;
			}
		}
		m_cb0_Blur.Create();

		{
			// コンパイル済みのシェーダーヘッダーファイルをインクルード
			#include "KdPostProcess_HBright_PS.inc"

			if (FAILED(D3D.GetDev()->CreatePixelShader(compiledBuffer,
				sizeof(compiledBuffer), nullptr, &m_HBrightPS))) {
				assert(0 && "ピクセルシェーダー作成失敗");
				Release();
				return false;
			}

		}


	}


	{
#include "KdPostProcess_Color_PS.inc"

		if (FAILED(D3D.GetDev()->CreatePixelShader(compiledBuffer, sizeof(compiledBuffer), nullptr, &m_colorPS))) {
			assert(0 && "ピクセルシェーダー作成失敗");
			Release();
			return false;
		}

	}
	m_cb0_Color.Create();

	return true;

}

void KdPostProcessShader::Release()
{
	SafeRelease(m_VS);
	SafeRelease(m_inputLayout);

	SafeRelease(m_colorPS);
	m_cb0_Color.Release();
}

void KdPostProcessShader::ColorDraw(const Texture* tex, const Math::Vector4& color)
{
	if (tex == nullptr)return;

	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> saveDS;
	UINT saveStencilRef = 0;
	D3D.GetDevContext()->OMGetDepthStencilState(&saveDS, &saveStencilRef);
	D3D.GetDevContext()->OMSetDepthStencilState(SHADER.m_ds_ZDisable_ZWriteDisable, 0);

	m_cb0_Color.Work().Color = color;
	m_cb0_Color.Write();
	D3D.GetDevContext()->VSSetConstantBuffers(0, 1, m_cb0_Color.GetAddress());
	D3D.GetDevContext()->PSSetConstantBuffers(0, 1, m_cb0_Color.GetAddress());

	D3D.GetDevContext()->PSSetShaderResources(0, 1, tex->GetSRViewAddress());
	D3D.GetDevContext()->PSSetSamplers(0, 1, &SHADER.m_ss_Linear_Clamp);

	D3D.GetDevContext()->VSSetShader(m_VS, 0, 0);
	D3D.GetDevContext()->IASetInputLayout(m_inputLayout);
	D3D.GetDevContext()->PSSetShader(m_colorPS, 0, 0);

	Vertex v[4] = {
		{ {-1,-1,0}, {0, 1} },// 0
		{ {-1, 1,0}, {0, 0} },// 1
		{ { 1,-1,0}, {1, 1} },// 2
		{ { 1, 1,0}, {1, 0} } // 3
	};

	D3D.DrawVertices(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,
		4,
		&v[0],
		sizeof(Vertex)
	);

	ID3D11ShaderResourceView* nullSRV = nullptr;
	D3D.GetDevContext()->PSSetShaderResources(0, 1, &nullSRV);
	D3D.GetDevContext()->PSSetSamplers(0, 1, &SHADER.m_ss_Anisotropic_Wrap);
	D3D.GetDevContext()->OMSetDepthStencilState(saveDS.Get(), saveStencilRef);

}

void KdPostProcessShader::BlurDraw(const Texture* tex, const Math::Vector2& dir)
{
	if (tex == nullptr)return;

	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> saveDS;
	UINT saveStencilRef = 0;
	D3D.GetDevContext()->OMGetDepthStencilState(&saveDS, &saveStencilRef);
	D3D.GetDevContext()->OMSetDepthStencilState(SHADER.m_ds_ZDisable_ZWriteDisable, 0);

	Math::Vector2 ts;
	ts.x = 1.0f / tex->GetWidth();
	ts.y = 1.0f / tex->GetHeight();

	const int kMaxSampling = 31;
	float totalWeight = 0;
	const float kDispersion = 3;
	for (int i = 0; i < kMaxSampling; i++)
	{
		int pt = i - kMaxSampling / 2;	// -15 ～ +15
		m_cb0_Blur.Work().Offset[i].x = dir.x * (pt * ts.x);
		m_cb0_Blur.Work().Offset[i].y = dir.y * (pt * ts.y);

		float w = exp(-(pt * pt) / (2 * kDispersion * kDispersion));
		m_cb0_Blur.Work().Offset[i].z = w;
		totalWeight += w;

	}

	for (int i = 0; i < 31; i++)
	{
		m_cb0_Blur.Work().Offset[i].z /= totalWeight;
	}


	m_cb0_Blur.Write();

	D3D.GetDevContext()->VSSetConstantBuffers(0, 1, m_cb0_Blur.GetAddress());
	D3D.GetDevContext()->PSSetConstantBuffers(0, 1, m_cb0_Blur.GetAddress());

	D3D.GetDevContext()->PSSetShaderResources(0, 1, tex->GetSRViewAddress());
	D3D.GetDevContext()->PSSetSamplers(0, 1, &SHADER.m_ss_Linear_Clamp);

	D3D.GetDevContext()->VSSetShader(m_VS, 0, 0);
	D3D.GetDevContext()->IASetInputLayout(m_inputLayout);
	D3D.GetDevContext()->PSSetShader(m_blurPS, 0, 0);

	Vertex v[4] = {
		{ {-1,-1,0}, {0, 1} },// 0
		{ {-1, 1,0}, {0, 0} },// 1
		{ { 1,-1,0}, {1, 1} },// 2
		{ { 1, 1,0}, {1, 0} } // 3
	};

	D3D.DrawVertices(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,
		4,
		&v[0],
		sizeof(Vertex)
	);

	ID3D11ShaderResourceView* nullSRV = nullptr;
	D3D.GetDevContext()->PSSetShaderResources(0, 1, &nullSRV);
	D3D.GetDevContext()->PSSetSamplers(0, 1, &SHADER.m_ss_Anisotropic_Wrap);
	D3D.GetDevContext()->OMSetDepthStencilState(saveDS.Get(), saveStencilRef);
}

void KdPostProcessShader::BrightFiltering(const Texture* destRT, const Texture* srcTex)
{
	RestoreRenderTarget rrt;

	D3D11_VIEWPORT saveVP;
	UINT numVP = 1;
	D3D.GetDevContext()->RSGetViewports(&numVP, &saveVP);

	D3D.GetDevContext()->OMSetRenderTargets(1, destRT->GetRTViewAddress(), nullptr);

	D3D11_VIEWPORT vp = { 0.0f, 0.0f,
		(float)destRT->GetWidth(), (float)destRT->GetHeight(), 0.0f, 1.0f };
	D3D.GetDevContext()->RSSetViewports(1, &vp);


	ID3D11DepthStencilState* saveDS;
	UINT saveStencilRef = 0;
	D3D.GetDevContext()->OMGetDepthStencilState(&saveDS, &saveStencilRef);

	D3D.GetDevContext()->OMSetDepthStencilState(SHADER.m_ds_ZDisable_ZWriteDisable, 0);


	D3D.GetDevContext()->PSSetShaderResources(0, 1, srcTex->GetSRViewAddress());
	D3D.GetDevContext()->PSSetSamplers(0, 1, &SHADER.m_ss_Linear_Clamp);

	D3D.GetDevContext()->VSSetShader(m_VS, 0, 0);
	D3D.GetDevContext()->IASetInputLayout(m_inputLayout);
	D3D.GetDevContext()->PSSetShader(m_HBrightPS, 0, 0);

	Vertex v[4] = {
		{ {-1,-1,0}, {0, 1} },// 0
		{ {-1, 1,0}, {0, 0} },// 1
		{ { 1,-1,0}, {1, 1} },// 2
		{ { 1, 1,0}, {1, 0} } // 3
	};

	D3D.DrawVertices(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP,
		4,
		&v[0],
		sizeof(Vertex)
	);


	ID3D11ShaderResourceView* nullSRV = nullptr;
	D3D.GetDevContext()->PSSetShaderResources(0, 1, &nullSRV);

	D3D.GetDevContext()->PSSetSamplers(0, 1, &SHADER.m_ss_Anisotropic_Wrap);
	D3D.GetDevContext()->OMSetDepthStencilState(saveDS, saveStencilRef);
	saveDS->Release();

	D3D.GetDevContext()->RSSetViewports(1, &saveVP);
}
