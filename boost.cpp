//*****************************************************
//
// ブーストの処理[boost.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "boost.h"
#include "manager.h"
#include "renderer.h"
#include "object2D.h"
#include "player.h"
#include "game.h"

//*****************************************************
// マクロ定義
//*****************************************************
#define NUM_PLACE	(6)	// 桁数
#define GAUGE_WIDTH	(40.0f)	// ゲージの横幅
#define GAUGE_HEIGTH	(250.0f)	// ゲージの縦の長さ
#define GAUGE_POS	(D3DXVECTOR3(1100.0f,650.0f,0.0f))	// ゲージの初期位置

//=====================================================
// コンストラクタ
//=====================================================
CBoost::CBoost(int nPriority) : CObject(nPriority)
{
	m_pObjectGauge = nullptr;
}

//=====================================================
// デストラクタ
//=====================================================
CBoost::~CBoost()
{

}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CBoost::Init(void)
{
	if (m_pObjectGauge == nullptr)
	{// ブーストゲージの生成
		m_pObjectGauge = CObject2D::Create(5);

		if (m_pObjectGauge != nullptr)
		{
			m_pObjectGauge->SetPosition(D3DXVECTOR3(GAUGE_POS.x, GAUGE_POS.y, 0.0f));
			m_pObjectGauge->SetSize(GAUGE_WIDTH, GAUGE_HEIGTH);
			m_pObjectGauge->SetVtx();
		}
	}

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CBoost::Uninit(void)
{
	if (m_pObjectGauge != nullptr)
	{// ゲージの破棄
		m_pObjectGauge = nullptr;
	}

	// 自身の破棄
	Release();
}

//=====================================================
// 更新処理
//=====================================================
void CBoost::Update(void)
{
	// 変数宣言
	float fBoost = 0.0f;
	float fHeight;
	D3DXVECTOR3 pos;
	CPlayer::BOOSTSTATE boostState;

	// プレイヤーの取得
	CPlayer *pPlayer = CGame::GetPlayer();

	if (pPlayer != nullptr)
	{// プレイヤー情報の取得
		fBoost = pPlayer->GetBoost();
		boostState = pPlayer->GetBoostState();
	}
	else
	{
		return;
	}

	// ブーストの割合を算出
	fHeight = fBoost / MAX_BOOST;
	fHeight *= GAUGE_HEIGTH;

	if (m_pObjectGauge != nullptr)
	{// ゲージの設定
		// 位置設定
		pos = { m_pObjectGauge->GetPosition().x,GAUGE_POS.y - fHeight,0.0f };

		// 色設定
		if (boostState == CPlayer::BOOSTSTATE_EMPTY)
		{// 欠乏状態
			m_pObjectGauge->SetCol(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
		}
		else
		{// 通常状態
			m_pObjectGauge->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}

		// サイズ設定
		m_pObjectGauge->SetPosition(pos);
		m_pObjectGauge->SetSize(GAUGE_WIDTH, fHeight);
		m_pObjectGauge->SetVtx();
	}
}

//=====================================================
// 生成処理
//=====================================================
CBoost *CBoost::Create(void)
{
	CBoost *pBoost = nullptr;

	if (pBoost == nullptr)
	{
		pBoost = new CBoost;

		pBoost->Init();
	}

	return pBoost;
}