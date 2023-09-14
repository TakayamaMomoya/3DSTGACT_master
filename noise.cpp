//*****************************************************
//
// ノイズ処理[noise.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "noise.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//*****************************************************
// マクロ定義
//*****************************************************
#define ANIM_TIME				(6)	//アニメーションの速度
#define NUM_ANIM	(8)	// アニメーションの枚数

//*****************************************************
// 静的メンバ変数宣言
//*****************************************************
int CNoise::m_nNumAll = 0;	// 総数

//=====================================================
// コンストラクタ
//=====================================================
CNoise::CNoise(int nPriority) : CAnim2D(nPriority)
{
	m_nLife = 0;
	m_nAlpha = 0;
	m_nAddAlpha = 0;

	m_nNumAll++;
}

//=====================================================
// デストラクタ
//=====================================================
CNoise::~CNoise()
{
	m_nNumAll--;
}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CNoise::Init(void)
{
	// 継承クラスの初期化
	CAnim2D::Init();

	// 情報設定
	SetInfo(0,6,3,1);

	// サイズ設定
	SetSize(SCREEN_WIDTH * 0.5f,SCREEN_HEIGHT * 0.5f);

	// 位置設定
	SetPosition(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f));

	SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CNoise::Uninit(void)
{
	// 継承クラスの終了
	CAnim2D::Uninit();
}

//=====================================================
// 更新処理
//=====================================================
void CNoise::Update(void)
{
	// 継承クラスの更新
	CAnim2D::Update();

	m_nLife--;

	m_nAlpha += m_nAddAlpha;

	if (m_nLife <= 0)
	{
		Uninit();
	}
}

//=====================================================
// 描画処理
//=====================================================
void CNoise::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// アルファテストの有効化
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, m_nAlpha);

	// 継承クラスの描画
	CAnim2D::Draw();

	// アルファテストの無効化
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
}

//=====================================================
// 生成処理
//=====================================================
CNoise *CNoise::Create(int nLife)
{
	CNoise *pNoise = nullptr;

	pNoise = new CNoise;
	
	if (pNoise != nullptr)
	{
		// 初期化処理
		pNoise->Init();

		pNoise->m_nLife = nLife;

		pNoise->m_nAddAlpha = 255 / nLife;

		// テクスチャの読込
		int nIdx = CManager::GetTexture()->Regist("data\\TEXTURE\\EFFECT\\noise00.png");
		pNoise->SetIdxTexture(nIdx);
	}

	return pNoise;
}