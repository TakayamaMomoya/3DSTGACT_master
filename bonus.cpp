//*****************************************************
//
// ボーナスの表示[bonus.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "bonus.h"
#include "object2D.h"
#include "manager.h"
#include "texture.h"
#include "score.h"
#include "game.h"
#include "player.h"
#include "assess.h"

//*****************************************************
// マクロ定義
//*****************************************************
#define MOVE_FACT	(0.05f)	// 移動係数
#define IN_SPEED	(0.03f)	// フェードインのスピード
#define OUT_SPEED	(0.07f)	// フェードアウトのスピード
#define TIME_FADE	(60)	// フェードアウトまでのフレーム数
#define DOGDE_SCORE	(2000)	// 至近弾回避のスコア
#define MISSILE_SCORE	(500)	// ミサイル撃破のスコア
#define MANUALHIT_SCORE	(1000)	// マニュアル照準のスコア
#define ASSESS_DODGE	(0.07f)	// 回避の評価単価
#define ASSESS_MISSILE	(0.07f)	// ミサイル撃破の評価単価
#define ASSESS_MANUALHIT	(0.1f)	// マニュアルヒットの評価単価

//*****************************************************
// 静的メンバ変数宣言
//*****************************************************
CBonus *CBonus::m_pHead = nullptr;	// 先頭のアドレス
CBonus *CBonus::m_pTail = nullptr;	// 最後尾のアドレス

//=====================================================
// コンストラクタ
//=====================================================
CBonus::CBonus(int nPriority) : CObject(nPriority)
{
	// 値のクリア
	m_pPrev = nullptr;
	m_pNext = nullptr;
	m_pCaption = nullptr;
	m_state = STATE_NONE;
	m_nTimerOut = 0;

	if (m_pHead == nullptr)
	{// 先頭と最後尾アドレスの代入
		m_pHead = this;
		m_pTail = this;

		return;
	}

	// 前のアドレスに最後尾のアドレスを代入する
	m_pPrev = m_pTail;

	// 最後尾のアドレスを自分にする
	m_pTail = this;

	// 前のオブジェクトの次のアドレスを自分にする
	m_pPrev->m_pNext = this;
}

//=====================================================
// デストラクタ
//=====================================================
CBonus::~CBonus()
{

}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CBonus::Init(void)
{
	m_state = STATE_IN;

	// 他の見出しを押し上げる
	CBonus *pBonus = m_pHead;

	while (pBonus != nullptr)
	{
		CBonus *pBonusNext = pBonus->m_pNext;

		if (pBonus != this)
		{
			pBonus->m_posDest.y -= 70.0f;
		}

		pBonus = pBonusNext;
	}

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CBonus::Uninit(void)
{
	if (m_pCaption != nullptr)
	{
		m_pCaption->Uninit();

		m_pCaption = nullptr;
	}

	if (m_pHead == this)
	{// 先頭アドレスの破棄
	 // 先頭アドレスを次のアドレスに引き継ぐ
		m_pHead = m_pNext;

		if (m_pNext != nullptr)
		{
			m_pNext->m_pPrev = nullptr;
		}
	}
	else if (m_pTail == this)
	{// 最後尾アドレスの破棄
		if (m_pPrev != nullptr)
		{// 最後尾アドレスを前のアドレスに引き継ぐ
			m_pTail = m_pPrev;

			m_pPrev->m_pNext = nullptr;
		}
	}
	else
	{// 真ん中のアドレスの破棄
	 // 前のアドレスから次のアドレスをつなぐ
		m_pPrev->m_pNext = m_pNext;

		// 次のアドレスから前のアドレスをつなぐ
		m_pNext->m_pPrev = m_pPrev;
	}

	Release();
}

//=====================================================
// 更新処理
//=====================================================
void CBonus::Update(void)
{
	// 状態管理
	ManageState();

	// 目標位置に向かう処理
	MoveToDest();
}

//=====================================================
// 状態管理
//=====================================================
void CBonus::ManageState(void)
{
	if (m_pCaption == nullptr)
	{
		return;
	}

	if (m_pCaption->GetPosition().y <= SCREEN_WIDTH * 0.4f)
	{
		m_state = STATE_OUT;
	}

	// 色の取得
	D3DXCOLOR col = m_pCaption->GetCol();

	switch (m_state)
	{
	case CBonus::STATE_IN:
		col.a += IN_SPEED;

		if (col.a > 1.0f)
		{
			m_state = STATE_WAIT;
		}

		break;
	case CBonus::STATE_WAIT:

		m_nTimerOut++;

		if (m_nTimerOut >= TIME_FADE)
		{
			m_state = STATE_OUT;
		}

		break;
	case CBonus::STATE_OUT:
		col.a -= OUT_SPEED;

		if (col.a <= 0.0f)
		{
			Uninit();
		}

		break;
	default:
		break;
	}

	if (m_pCaption != nullptr)
	{
		m_pCaption->SetCol(col);
	}
}

//=====================================================
// 目標に向かう処理
//=====================================================
void CBonus::MoveToDest(void)
{
	// 位置の取得
	D3DXVECTOR3 pos;
	D3DXVECTOR3 vecDiff;

	if (m_pCaption != nullptr)
	{
		pos = m_pCaption->GetPosition();
	}
	else
	{
		return;
	}

	// 差分ベクトル取得
	vecDiff = m_posDest - pos;

	pos += vecDiff * MOVE_FACT;

	// 位置の設定
	m_pCaption->SetPosition(pos);

	m_pCaption->SetVtx();
}

//=====================================================
// 描画処理
//=====================================================
void CBonus::Draw(void)
{

}

//=====================================================
// 生成処理
//=====================================================
CBonus *CBonus::Create(TYPE type)
{
	char *aPath[TYPE_MAX] =
	{
		"data\\TEXTURE\\UI\\message00.png",
		"data\\TEXTURE\\UI\\message01.png",
		"data\\TEXTURE\\UI\\message02.png"
	};

	CBonus *pBonus = nullptr;

	pBonus = new CBonus;

	if (pBonus != nullptr)
	{
		pBonus->m_pCaption = CObject2D::Create(6);

		if (pBonus->m_pCaption != nullptr)
		{
			// ボーナス付与
			pBonus->BonusScore(type);

			pBonus->m_pCaption->SetPosition(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 600.0f, 0.0f));
			pBonus->m_pCaption->SetSize(170.0f, 30.0f);

			// 初期の目標位置と同期する
			pBonus->m_posDest = pBonus->m_pCaption->GetPosition();

			pBonus->m_pCaption->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));

			// テクスチャ番号取得
			int nIdx = CManager::GetTexture()->Regist(aPath[type]);

			pBonus->m_pCaption->SetIdxTexture(nIdx);
			pBonus->m_pCaption->SetVtx();
		}

		// 初期化処理
		pBonus->Init();
	}

	return pBonus;
}

//=====================================================
// スコアボーナス付与
//=====================================================
void CBonus::BonusScore(TYPE type)
{
	// スコア取得
	CScore *pScore = CGame::GetScore();

	// プレイヤーの取得
	CPlayer *pPlayer = CGame::GetPlayer();

	CAssess *pAssess = nullptr;

	if (pPlayer != nullptr)
	{// 評価システムの取得
		pAssess = pPlayer->GetAssess();
	}

	int aScore[TYPE_MAX] = 
	{
		DOGDE_SCORE,
		MISSILE_SCORE,
		MANUALHIT_SCORE
	};

	if (pScore != nullptr)
	{
		pScore->AddScore(aScore[type]);
	}

	if (pAssess == nullptr)
	{
		return;
	}

	switch (type)
	{
	case CBonus::TYPE_DOGDE:

		pAssess->AddParam(ASSESS_DODGE,CAssess::PARAM_DODGE);

		break;
	case CBonus::TYPE_MISSILE:

		pAssess->AddParam(ASSESS_MISSILE, CAssess::PARAM_HIT);

		break;
	case CBonus::TYPE_MANUALHIT:

		pAssess->AddParam(ASSESS_MANUALHIT, CAssess::PARAM_HIT);

		break;
	default:
		break;
	}
}