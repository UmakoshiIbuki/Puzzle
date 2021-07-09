#pragma once

//====================================================
//
// テクスチャクラス
//
//====================================================
class Texture
{
public:

	//レンダータゲットテクスチャの作成
	bool CreateRenderTarget(int w, int h, DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM,
		UINT arryCnt = 1, const D3D11_SUBRESOURCE_DATA* fileData = nullptr,
		UINT miscFlags = 0);
	//深度ステンシルテクスチャ(Zバッファ)の作成
	bool CreateDepthStencil(int w, int h, DXGI_FORMAT format = DXGI_FORMAT_R24G8_TYPELESS,
		UINT arryCnt = 1, const D3D11_SUBRESOURCE_DATA* fileData = nullptr,
		UINT miscFlags = 0);

	//====================================================
	//
	// 取得
	//
	//====================================================

	// 画像のアスペクト比取得
	float								GetAspectRatio() const { return (float)m_desc.Width / m_desc.Height; }
	// 画像の幅を取得
	UINT								GetWidth() const { return m_desc.Width; }
	// 画像の高さを取得
	UINT								GetHeight() const { return m_desc.Height; }
	// 画像の全情報を取得
	const D3D11_TEXTURE2D_DESC&			GetInfo() const { return m_desc; }
	// ファイルパス取得(Load時のみ)
	const std::string&					GetFilepath() const { return m_filepath; }


	// 画像リソースを取得
	ID3D11Texture2D*					GetResource() const;

	// シェーダリソースビュー取得
	ID3D11ShaderResourceView*			GetSRView() const { return m_srv; }
	// シェーダリソースビューのアドレス取得
	ID3D11ShaderResourceView* const*	GetSRViewAddress() const { return &m_srv; }

	// レンダーターゲットビュー取得
	ID3D11RenderTargetView*				GetRTView() const { return m_rtv; }
	// レンダーターゲットビューのアドレス取得
	ID3D11RenderTargetView* const*		GetRTViewAddress() const { return &m_rtv; }

	// 深度ステンシルビュー取得
	ID3D11DepthStencilView*				GetDSView() const { return m_dsv; }

	bool Load(const std::string& filename, bool renderTarget = false, bool depthStencil = false, bool generateMipmap = true);

	//====================================================
	//
	// テクスチャ作成
	//
	//====================================================

	bool Create(ID3D11Texture2D* pTexture2D);

	bool Create(const D3D11_TEXTURE2D_DESC& desc, const D3D11_SUBRESOURCE_DATA* fillData = nullptr);

	bool Create(int w, int h, DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM, UINT arrayCnt = 1, const D3D11_SUBRESOURCE_DATA* fillData = nullptr);

	//====================================================
	//
	// ビューから作成
	//
	//====================================================

	void SetSRView(ID3D11ShaderResourceView* srv);

	Texture() {}

	Texture(const std::string& filename )
	{
		Load(filename);
	}

	//====================================================
	// 解放
	//====================================================
	void Release();

	~Texture()
	{
		Release();
	}

private:

	ID3D11ShaderResourceView*	m_srv = nullptr;

	ID3D11RenderTargetView*		m_rtv = nullptr;

	ID3D11DepthStencilView*		m_dsv = nullptr;


	D3D11_TEXTURE2D_DESC		m_desc = {};


	std::string					m_filepath;

private:
	// コピー禁止用
	Texture(const Texture& src) = delete;
	void operator=(const Texture& src) = delete;
};
