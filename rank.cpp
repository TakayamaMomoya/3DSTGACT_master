//*****************************************************
//
// ランクの処理[rank.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "rank.h"
#include "manager.h"
#include "debugproc.h"
#include "object2D.h"
#include "texture.h"
#include "number.h"
#include "inputkeyboard.h"
#include "player.h"
#include "game.h"
#include "assess.h"
#include "objectmanager.h"

//*****************************************************
// マクロ定義
//*****************************************************
#define FRAME_POS	(D3DXVECTOR3(830.0f,70.0f,0.0f))	// フレームの初期位置
#define FRAME_WIDTH	(465.0f)	// フレームの横幅
#define FRAME_HEIGHT	(172.0f)	// フレームの縦の長さ
#define FRAME_SCALE	(0.3f)	// フレームのスケール
#define FRAME_PATH	"data\\TEXTURE\\UI\\rank00.png"	// フレームのパス
#define NUM_PLACE	(2)	// 桁数
#define RANK_POS	(D3DXVECTOR3(725.0f,70.0f,0.0f))	// ランクの位置
#define GAUGE_WIDTH	(108.0f)	// ゲージの横幅
#define GAUGE_HEIGHT	(16.0f)	// ゲージの縦の長さ
#define GAUGE_POS	(D3DXVECTOR3(745.0f,98.0f,0.0f))	// ゲージの位置
#define INITIAL_RANK	(1)	// 初期ランク
#define MAX_RANK	(99)	// ランクの最大値
#define GAUGE_COl	(D3DXCOLOR(0.9f,0.9f,0.9f,0.9f))	// ゲージの色

//=====================================================
// コンストラクタ
//=====================================================
CRank::CRank(int nPriority)
{
	m_fProgress = 0.0f;
	m_nRank = 0;
	m_pFrame = nullptr;
	m_pRankNum = nullptr;
	m_pGaugeProgress = nullptr;
}

//=====================================================
// デストラクタ
//=====================================================
CRank::~CRank()
{

}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CRank::Init(void)
{
	m_nRank = INITIAL_RANK;

	m_pGaugeProgress = CObject2D::Create(6);

	if (m_pGaugeProgress != nullptr)
	{
		m_pGaugeProgress->SetPosition(GAUGE_POS);
		m_pGaugeProgress->SetSize(GAUGE_WIDTH, GAUGE_HEIGHT);

		m_pGaugeProgress->SetVtx();
		m_pGaugeProgress->SetCol(GAUGE_COl);
	}

	m_pFrame = CObject2D::Create(7);

	if (m_pFrame != nullptr)
	{
		m_pFrame->SetPosition(FRAME_POS);
		m_pFrame->SetSize(FRAME_WIDTH * FRAME_SCALE, FRAME_HEIGHT * FRAME_SCALE);

		// テクスチャ番号取得
		int nIdx = CManager::GetTexture()->Regist(FRAME_PATH);

		m_pFrame->SetIdxTexture(nIdx);
		m_pFrame->SetVtx();
	}

	if (m_pRankNum == nullptr)
	{
		m_pRankNum = CNumber::Create(NUM_PLACE, m_nRank);

		if (m_pRankNum != nullptr)
		{
			m_pRankNum->SetPosition(RANK_POS);
			m_pRankNum->SetSizeAll(15.0f, 32.5f);
		}
	}

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CRank::Uninit(void)
{
	if (m_pFrame != nullptr)
	{
		m_pFrame->Uninit();

		m_pFrame = nullptr;
	}

	if (m_pGaugeProgress != nullptr)
	{
		m_pGaugeProgress->Uninit();

		m_pGaugeProgress = nullptr;
	}

	if (m_pRankNum != nullptr)
	{
		m_pRankNum->Uninit();

		m_pRankNum = nullptr;
	}

	Release();
}

//=====================================================
// 更新処理
//=====================================================
void CRank::Update(void)
{
	CInputKeyboard *pKeyboard = CManager::GetKeyboard();

	// 進行ゲージの管理
	ManageGauge();

	if (pKeyboard != nullptr)
	{
		if (pKeyboard->GetPress(DIK_O))
		{
			AddProgress(1.0f);
		}
	}
}

//=====================================================
// 進行ゲージの管理
//=====================================================
void CRank::ManageGauge(void)
{
	if (m_pGaugeProgress == nullptr)
	{
		return;
	}

	float fWidth;
	D3DXVECTOR3 pos;

	fWidth = m_fProgress * GAUGE_WIDTH;

	// 位置設定
	pos = { GAUGE_POS.x + fWidth,GAUGE_POS.y,0.0f };

	// サイズ設定
	m_pGaugeProgress->SetPosition(pos);
	m_pGaugeProgress->SetSize(fWidth, GAUGE_HEIGHT);
	m_pGaugeProgress->SetVtx();
}

//=====================================================
// 進行度合加算処理
//=====================================================
void CRank::AddProgress(float fValue)
{
	// 評価取得
	CAssess *pAssess = nullptr;
	CPlayer *pPlayer = nullptr;
	CObjectManager *pObjManager = CManager::GetObjectManager();

	if (pObjManager != nullptr)
	{// プレイヤーの適用
		pPlayer = pObjManager->GetPlayer();
	}

	if (pPlayer != nullptr)
	{
		pAssess = pPlayer->GetAssess();
 	}

	// 評価の平均値算出
	float fAve = 0.0f;

	if (pAssess != nullptr)
	{
		fAve = (pAssess->GetParam(CAssess::PARAM_ATTACK) + pAssess->GetParam(CAssess::PARAM_DODGE) + pAssess->GetParam(CAssess::PARAM_HIT)) / 3;
	}

	m_fProgress += fValue * fAve + fValue;

	// 値の制限
	if (m_fProgress < 0.0f)
	{
		m_fProgress = 0.0f;
	}
	if (m_fProgress > 1.0f)
	{// ランクアップ
		m_fProgress = 0.0f;

		m_nRank++;

		// ランク制限
		if (m_nRank > MAX_RANK)
		{
			m_nRank = MAX_RANK;

			m_fProgress = 1.0f;
		}
	}

	if (m_pRankNum != nullptr)
	{
		m_pRankNum->SetValue(m_nRank,NUM_PLACE);
	}
}

//=====================================================
// 描画処理
//=====================================================
void CRank::Draw(void)
{

}

//=====================================================
// 生成処理
//=====================================================
CRank *CRank::Create(void)
{
	CRank *pRank = nullptr;

	if (pRank == nullptr)
	{// インスタンス生成
		pRank = new CRank;

		// 初期化処理
		pRank->Init();
	}

	return pRank;
}