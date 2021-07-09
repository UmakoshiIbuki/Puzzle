
#pragma once

struct KdBlurTexture
{
	void Create(int w, int h)
	{
		int divideValue = 2;
		for (int i = 0; i < 5; i++)
		{
			m_rt[i][0] = std::make_shared<Texture>();
			m_rt[i][0]->CreateRenderTarget(w / divideValue, h / divideValue,
				DXGI_FORMAT_R16G16B16A16_FLOAT);

			m_rt[i][1] = std::make_shared<Texture>();
			m_rt[i][1]->CreateRenderTarget(w / divideValue, h / divideValue,
				DXGI_FORMAT_R16G16B16A16_FLOAT);
			divideValue *= 2;
		}
	}

	std::shared_ptr<Texture>	m_rt[5][2];
};

class KdPostProcessShader
{
public:

	struct Vertex
	{
		Math::Vector3 Pos;
		Math::Vector2 UV;
	};

	bool Init();

	void Release();
	~KdPostProcessShader()
	{
		Release();
	}

	void ColorDraw(const Texture* tex, const Math::Vector4& color = { 1,1,1,1 });

	void BlurDraw(const Texture* tex, const Math::Vector2& dir);

	void BrightFiltering(const Texture* destRT, const Texture* srcTex);

	void KdPostProcessShader::GenerateBlur(KdBlurTexture& blurTex, const Texture* srcTex)
	{
		RestoreRenderTarget rrt;

		D3D11_VIEWPORT saveVP;
		UINT numVP = 1;
		D3D.GetDevContext()->RSGetViewports(&numVP, &saveVP);

		D3D11_VIEWPORT vp;

		for (int i = 0; i < 5; i++)
		{
			D3D.GetDevContext()->OMSetRenderTargets(
				1, blurTex.m_rt[i][0]->GetRTViewAddress(), nullptr);

			vp = D3D11_VIEWPORT{ 0.0f, 0.0f, (float)blurTex.m_rt[i][0]->GetWidth(),
				(float)blurTex.m_rt[i][0]->GetHeight(), 0.0f, 1.0f };
			D3D.GetDevContext()->RSSetViewports(1, &vp);

			if (i == 0)
			{
				ColorDraw(srcTex);
			}
			else
			{
				ColorDraw(blurTex.m_rt[i - 1][0].get());
			}

			D3D.GetDevContext()->OMSetRenderTargets(1,
				blurTex.m_rt[i][1]->GetRTViewAddress(), nullptr);

			BlurDraw(blurTex.m_rt[i][0].get(), { 1, 0 });

			D3D.GetDevContext()->OMSetRenderTargets(1,
				blurTex.m_rt[i][0]->GetRTViewAddress(), nullptr);

			BlurDraw(blurTex.m_rt[i][1].get(), { 0, 1 });
		}

		D3D.GetDevContext()->RSSetViewports(1, &saveVP);
	}

private:
	ID3D11VertexShader* m_VS = nullptr;
	ID3D11InputLayout* m_inputLayout = nullptr;

	ID3D11PixelShader* m_colorPS = nullptr;
	ID3D11PixelShader* m_HBrightPS = nullptr;

	struct cbColor {
		Math::Vector4	Color = { 1,1,1,1 };
	};

	ID3D11PixelShader* m_blurPS = nullptr;
	struct cbBlur {
		Math::Vector4	Offset[31];
	};
	ConstantBuffer<cbBlur>	m_cb0_Blur;
	ConstantBuffer<cbColor>	m_cb0_Color;
};