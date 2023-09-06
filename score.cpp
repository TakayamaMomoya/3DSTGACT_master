//*****************************************************
//
// スコアの処理[score.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "score.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"
#include "texture.h"

//*****************************************************
// マクロ定義
//*****************************************************
#define NUM_PLACE	(6)	// 桁数
#define SCORE_SPEED	(13)	// スコアの変わるスピード	

//*****************************************************
// 静的メンバ変数宣言
//*****************************************************
LPDIRECT3DTEXTURE9 CScore::m_pTexture = nullptr;	// テクスチャのポインタ

//=====================================================
// コンストラクタ
//=====================================================
CScore::CScore()
{
	m_pObjNumber = nullptr;
	m_nScore = 0;
	m_nSocreDest = 0;
	m_nIdxTexture = -1;
}

//=====================================================
// デストラクタ
//=====================================================
CScore::~CScore()
{

}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CScore::Init(void)
{
	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CScore::Uninit(void)
{
	if (m_pObjNumber != nullptr)
	{
		m_pObjNumber = nullptr;
	}

	CGame::ReleaseScore();
}

//=====================================================
// 更新処理
//=====================================================
void CScore::Update(void)
{
	//スコア値上昇演出==============================
	if (m_nScore < m_nSocreDest)
	{//上昇
		m_nScore += SCORE_SPEED;
		if (m_nScore > m_nSocreDest)
		{//停止
			m_nScore = m_nSocreDest;
		}
	}
	else if (m_nScore > m_nSocreDest)
	{//減少
		m_nScore -= SCORE_SPEED;
		if (m_nScore < m_nSocreDest)
		{//停止
			m_nScore = m_nSocreDest;
		}
	}

	if (m_pObjNumber != nullptr)
	{
		m_pObjNumber->SetValue(m_nScore, NUM_PLACE);
	}
}

//=====================================================
// スコア加算
//=====================================================
void CScore::AddScore(int nValue)
{
	m_nSocreDest += nValue;
}

//=====================================================
// 生成処理
//=====================================================
CScore *CScore::Create(void)
{
	CScore *pScore = nullptr;
	
	CObject2D *pObject2D = CObject2D::Create(7);

	if (pObject2D != nullptr)
	{
		pObject2D->SetPosition(D3DXVECTOR3(940.0f, 70.0f, 0.0f));
		pObject2D->SetSize(120.0f, 60.0f);

		// テクスチャ番号取得
		int nIdx = CManager::GetTexture()->Regist("data\\TEXTURE\\UI\\scoreBack.png");

		pObject2D->SetIdxTexture(nIdx);
		pObject2D->SetVtx();
	}

	if (pScore == nullptr)
	{
		pScore = new CScore;

		if (pScore->m_pObjNumber == nullptr)
		{
			pScore->m_pObjNumber = CNumber::Create(NUM_PLACE, pScore->m_nScore);
			pScore->m_pObjNumber->SetPosition(D3DXVECTOR3(1060.0f, 80.0f, 0.0f));
			pScore->m_pObjNumber->SetSizeAll(20.0f,50.0f);
		}
	}

	return pScore;
}

//=====================================================
// 読込処理
//=====================================================
HRESULT CScore::Load(void)
{
	if (m_pTexture == nullptr)
	{
		// デバイスの取得
		LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

		D3DXCreateTextureFromFile
		(
			pDevice,
			"data\\TEXTURE\\UI\\scoreBack.png",
			&m_pTexture
		);
	}

	return S_OK;
}

//=====================================================
// テクスチャ破棄
//=====================================================
void CScore::Unload(void)
{
	if (m_pTexture != nullptr)
	{
		m_pTexture->Release();
		m_pTexture = nullptr;
	}
}