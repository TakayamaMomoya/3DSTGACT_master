//*****************************************************
//
// 背景の処理[bgmulti.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "main.h"
#include "bgmulti.h"
#include "manager.h"
#include "renderer.h"

//*****************************************************
// 静的メンバ変数宣言
//*****************************************************
LPDIRECT3DTEXTURE9 CBgMulti::m_apTexture[MAX_BG] = {};	// テクスチャのポインタ

//=====================================================
// コンストラクタ
//=====================================================
CBgMulti::CBgMulti(int nPriority) : CObject(nPriority)
{
	CBg *pBg;
	
	pBg = CBg::Create();
	pBg->SetScroll(-0.01f, 0.0f);
	
	pBg = CBg::Create();
	pBg->SetScroll(-0.035f, 0.0f);
}

//=====================================================
// デストラクタ
//=====================================================
CBgMulti::~CBgMulti()
{

}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CBgMulti::Init(void)
{


	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CBgMulti::Uninit(void)
{
	Release();
}

//=====================================================
// 更新処理
//=====================================================
void CBgMulti::Update(void)
{

}

//=====================================================
// 描画処理
//=====================================================
void CBgMulti::Draw(void)
{

}

//=====================================================
// 読込処理
//=====================================================
HRESULT CBgMulti::Load(void)
{
	if (m_apTexture[0] == nullptr)
	{
		// ファイル名
		const char *aPathFile[MAX_BG] =
		{
			"data\\TEXTURE\\BG\\bg000.jpg",
			"data\\TEXTURE\\BG\\bg001.png",
		};

		// デバイスの取得
		LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

		for (int nCntTex = 0; nCntTex < MAX_BG; nCntTex++)
		{
			D3DXCreateTextureFromFile
			(
				pDevice,
				aPathFile[nCntTex],
				&m_apTexture[nCntTex]
			);
		}
	}

	return S_OK;
}

//=====================================================
// 破棄処理
//=====================================================
void CBgMulti::Unload(void)
{
	for (int nCntTex = 0; nCntTex < MAX_BG; nCntTex++)
	{
		if (m_apTexture[nCntTex] != nullptr)
		{
			m_apTexture[nCntTex]->Release();
			m_apTexture[nCntTex] = nullptr;
		}
	}
}

//=====================================================
// 生成処理
//=====================================================
CBgMulti *CBgMulti::Create(void)
{
	CBgMulti *pBgMulti = nullptr;

	if (pBgMulti == nullptr)
	{
		pBgMulti = new CBgMulti;
	}

	return pBgMulti;
}