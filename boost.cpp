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
#include "texture.h"
#include "renderer.h"
#include "object2D.h"
#include "player.h"
#include "game.h"
#include "inputkeyboard.h"
#include "debugproc.h"
#include "objectmanager.h"

//*****************************************************
// マクロ定義
//*****************************************************
#define NUM_PLACE	(6)	// 桁数
#define GAUGE_WIDTH	(17.0f)	// ゲージの横幅
#define GAUGE_HEIGTH	(215.0f)	// ゲージの縦の長さ
#define GAUGE_POS	(D3DXVECTOR3(1175.0f,643.0f,0.0f))	// ゲージの位置
#define FRAME_WIDTH	(37.0f)	// フレームの横幅
#define FRAME_HEIGHT	(292.0f)	// フレームの縦の長さ
#define FRAME_POS	(D3DXVECTOR3(1178.0f,430.0f,0.0f))	// フレームの位置
#define CAPTION_WIDTH	(15.0f)	// 見出しの横幅
#define CAPTION_HEIGTH	(75.0f)	// 見出しの縦の長さ
#define CAPTION_PATH	"data\\TEXTURE\\UI\\caption00.png"	// 見出しのパス
#define FRAME_PATH	"data\\TEXTURE\\UI\\frame03.png"	// フレームのパス

//=====================================================
// コンストラクタ
//=====================================================
CBoost::CBoost(int nPriority) : CObject(nPriority)
{
	m_pObjectGauge = nullptr;
	m_pCaption = nullptr;
	m_posFrame = { 0.0f,0.0f,0.0f };
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
	m_fWidthFrame = FRAME_WIDTH;
	m_fHeightFrame = FRAME_HEIGHT;
	m_posFrame = FRAME_POS;

	if (m_pObjectGauge == nullptr)
	{// ブーストゲージの生成
		m_pObjectGauge = CObject2D::Create(7);

		if (m_pObjectGauge != nullptr)
		{
			m_pObjectGauge->SetPosition(D3DXVECTOR3(GAUGE_POS.x, GAUGE_POS.y, 0.0f));
			m_pObjectGauge->SetSize(GAUGE_WIDTH, GAUGE_HEIGTH);
			m_pObjectGauge->SetVtx();
		}
	}

	if (m_pCaption == nullptr)
	{// 見出しの生成
		m_pCaption = CObject2D::Create(7);

		if (m_pCaption != nullptr)
		{
			m_pCaption->SetPosition(D3DXVECTOR3(FRAME_POS.x + FRAME_WIDTH + CAPTION_WIDTH, FRAME_POS.y + CAPTION_HEIGTH * 2, 0.0f));
			m_pCaption->SetSize(CAPTION_WIDTH, CAPTION_HEIGTH);
			m_pCaption->SetVtx();

			// テクスチャ番号取得
			int nIdx = CManager::GetTexture()->Regist(CAPTION_PATH);

			m_pCaption->SetIdxTexture(nIdx);
		}
	}

	if (m_pFrame == nullptr)
	{// フレームの生成
		m_pFrame = CObject2D::Create(7);

		if (m_pFrame != nullptr)
		{
			m_pFrame->SetPosition(D3DXVECTOR3(FRAME_POS.x, FRAME_POS.y, 0.0f));
			m_pFrame->SetSize(FRAME_WIDTH, FRAME_HEIGHT);
			m_pFrame->SetVtx();

			// テクスチャ番号取得
			int nIdx = CManager::GetTexture()->Regist(FRAME_PATH);

			m_pFrame->SetIdxTexture(nIdx);
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
		m_pObjectGauge->Uninit();
		m_pObjectGauge = nullptr;
	}

	if (m_pCaption != nullptr)
	{// 見出しの破棄
		m_pCaption->Uninit();
		m_pCaption = nullptr;
	}

	if (m_pFrame != nullptr)
	{// フレームの破棄
		m_pFrame->Uninit();
		m_pFrame = nullptr;
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
	CPlayer *pPlayer = nullptr;
	CObjectManager *pObjManager = CManager::GetObjectManager();

	if (pObjManager != nullptr)
	{// プレイヤーの適用
		pPlayer = pObjManager->GetPlayer();
	}

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
// 描画処理
//=====================================================
void CBoost::Draw(void)
{
	//CManager::GetDebugProc()->Print("\nフレームの位置：[%f,%f,%f]", m_posFrame.x, m_posFrame.y, m_posFrame.z);
	//CManager::GetDebugProc()->Print("\nフレームの大きさ：[%f,%f]", m_fWidthFrame, m_fHeightFrame);
}

//=====================================================
// 位置設定
//=====================================================
void CBoost::Edit(void)
{
	CInputKeyboard *pKeyboard = CManager::GetKeyboard();

	if (pKeyboard == nullptr)
	{
		return;
	}

	if (pKeyboard->GetPress(DIK_LSHIFT))
	{
		if (pKeyboard->GetPress(DIK_UP))
		{
			m_fHeightFrame += 2.0f;
		}
		if (pKeyboard->GetPress(DIK_DOWN))
		{
			m_fHeightFrame -= 2.0f;
		}

		if (pKeyboard->GetPress(DIK_RIGHT))
		{
			m_fWidthFrame += 2.0f;
		}
		if (pKeyboard->GetPress(DIK_LEFT))
		{
			m_fWidthFrame -= 2.0f;
		}
	}
	else
	{
		if (pKeyboard->GetPress(DIK_UP))
		{
			m_posFrame.y -= 2.0f;
		}
		if (pKeyboard->GetPress(DIK_DOWN))
		{
			m_posFrame.y += 2.0f;
		}

		if (pKeyboard->GetPress(DIK_RIGHT))
		{
			m_posFrame.x += 2.0f;
		}
		if (pKeyboard->GetPress(DIK_LEFT))
		{
			m_posFrame.x -= 2.0f;
		}
	}

	if (m_pFrame != nullptr)
	{// フレームの更新
		m_pFrame->SetSize(m_fWidthFrame,m_fHeightFrame);
		m_pFrame->SetPosition(m_posFrame);
		m_pFrame->SetVtx();
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