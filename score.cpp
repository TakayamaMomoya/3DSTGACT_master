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
#include "rank.h"

//*****************************************************
// マクロ定義
//*****************************************************
#define NUM_PLACE	(6)	// 桁数
#define SCORE_SPEED	(71)	// スコアの変わるスピード
#define RANK_BONUS	(0.04f)	// 1ランクにおけるボーナス倍率
#define RANK_RATE	(0.0004f)	// ランクの上がる倍率

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
	m_nScore += (int)((m_nSocreDest - m_nScore) * 0.1f);

	if (m_pObjNumber != nullptr)
	{
		m_pObjNumber->SetValue(m_nScore, NUM_PLACE);
	}
}

//=====================================================
// スコア加算
//=====================================================
int CScore::AddScore(int nValue)
{
	// ランク取得
	CRank *pRank = CGame::GetRank();

	if (pRank != nullptr)
	{// ランクによるボーナス
		nValue += (int)(nValue * RANK_BONUS * pRank->GetRank());

		pRank->AddProgress(nValue * RANK_RATE);	
	}

	m_nSocreDest += nValue;

	if (m_nSocreDest >1000000)
	{
		m_nSocreDest = 999999;
	}

	return nValue;
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
		pObject2D->SetPosition(D3DXVECTOR3(1030.0f, 80.0f, 0.0f));
		pObject2D->SetSize(70.0f, 35.0f);

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
			pScore->m_pObjNumber->SetPosition(D3DXVECTOR3(1100.0f, 80.0f, 0.0f));
			pScore->m_pObjNumber->SetSizeAll(15.0f,32.5f);
		}
	}

	return pScore;
}