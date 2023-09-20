//*****************************************************
//
// チュートリアル管理[tutorialmanager.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "tutorialmanager.h"
#include "manager.h"
#include "debugproc.h"
#include "object2D.h"
#include "texture.h"
#include "inputkeyboard.h"
#include "inputjoypad.h"
#include "fade.h"
#include "enemymove.h"

//*****************************************************
// マクロ定義
//*****************************************************
#define MOVE_PROGRESS	(120)	// 移動を検出するフレーム数
#define READ_PROGRESS	(240)	// 読むだけのフレーム数
#define IN_SPEED	(0.03f)	// フェードインのスピード
#define OUT_SPEED	(0.07f)	// フェードアウトのスピード
#define TIME_FADE	(60)	// フェードアウトまでのフレーム数
#define RAPID_NUM	(10)	// 連射の進むまでの数

//=====================================================
// コンストラクタ
//=====================================================
CTutorialManager::CTutorialManager(int nPriority)
{
	m_nCntProgress = 0;
	m_progress = PROGRESS_START;
	m_pCaption = nullptr;
	m_state = STATE_NONE;
}

//=====================================================
// デストラクタ
//=====================================================
CTutorialManager::~CTutorialManager()
{

}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CTutorialManager::Init(void)
{
	m_NextProgress = m_progress;
	m_state = STATE_IN;

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CTutorialManager::Uninit(void)
{
	if (m_pCaption != nullptr)
	{
		m_pCaption->Uninit();
		m_pCaption = nullptr;
	}

	Release();
}

//=====================================================
// 更新処理
//=====================================================
void CTutorialManager::Update(void)
{
	// 状態管理
	ManageState();

	// 時間経過によるチュートリアルの進行
	TimeTutorial();
	
	if (m_progress >= PROGRESS_SHOT && CEnemy::GetNumAll() == 0)
	{// 射撃以降は敵を出す
		CEnemy::Create(D3DXVECTOR3(500.0f, 0.0f, 0.0f), CEnemy::TYPE_MOVE);
		CEnemy::Create(D3DXVECTOR3(-500.0f, 0.0f, 0.0f), CEnemy::TYPE_MOVE);
	}
}

//=====================================================
// 状態管理
//=====================================================
void CTutorialManager::ManageState(void)
{
	if (m_pCaption == nullptr)
	{
		return;
	}

	// 色の取得
	D3DXCOLOR col = m_pCaption->GetCol();

	switch (m_state)
	{
	case CTutorialManager::STATE_IN:
		col.a += IN_SPEED;

		if (col.a > 1.0f)
		{
			m_state = STATE_WAIT;
		}

		break;
	case CTutorialManager::STATE_WAIT:

		break;
	case CTutorialManager::STATE_OUT:
		col.a -= OUT_SPEED;

		if (col.a <= 0.0f)
		{// 次のキャプション設定
			NextProgress(m_NextProgress);
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
// 時間経過によるチュートリアルの進行
//=====================================================
void CTutorialManager::TimeTutorial(void)
{
	CInputKeyboard *pKeyboard = CManager::GetKeyboard();
	CInputJoypad *pJoypad = CManager::GetJoypad();
	CFade *pFade = CManager::GetFade();

	if (m_progress == PROGRESS_START ||
		m_progress == PROGRESS_PARAM ||
		m_progress == PROGRESS_ASSESS)
	{// 時間経過
		AddProgress(ACTION_TIME);
	}

	if (pKeyboard != nullptr && pJoypad != nullptr && m_progress == PROGRESS_FREE)
	{
		if (pKeyboard->GetTrigger(DIK_RETURN))
		{// 画面遷移
			if (pFade != nullptr)
			{
				pFade->SetFade(CScene::MODE_GAME);
			}
		}
	}
}

//=====================================================
// 進行状況の進行
//=====================================================
void CTutorialManager::AddProgress(ACTION action)
{
	switch (m_progress)
	{
	case CTutorialManager::PROGRESS_START:	// スタート状態

		if (action == ACTION_TIME)
		{
			m_nCntProgress++;
		}

		if (m_nCntProgress > READ_PROGRESS)
		{// 次の進行状況に移行
			EndProgress(PROGRESS_MOVE);
		}

		break;
	case CTutorialManager::PROGRESS_MOVE:	// 移動チュートリアル

		if (action == ACTION_MOVE)
		{
			m_nCntProgress++;
		}

		if (m_nCntProgress > MOVE_PROGRESS)
		{// 次の進行状況に移行
			EndProgress(PROGRESS_BOOST);
		}

		break;
	case CTutorialManager::PROGRESS_BOOST:	// ブーストチュートリアル

		if (action == ACTION_BOOST)
		{
			m_nCntProgress++;
		}

		if (m_nCntProgress > MOVE_PROGRESS)
		{// 次の進行状況に移行
			EndProgress(PROGRESS_SHOT);
		}

		break;
	case CTutorialManager::PROGRESS_SHOT:	// 射撃チュートリアル

		if (action == ACTION_SHOT)
		{// チャージショットへ誘導
			EndProgress(PROGRESS_RAPID);
		}
		break;
	case CTutorialManager::PROGRESS_RAPID:	// 連射射撃チュートリアル

		if (action == ACTION_RAPID)
		{
			m_nCntProgress++;
		}

		if (m_nCntProgress > RAPID_NUM)
		{// 次の進行状況に移行
			EndProgress(PROGRESS_CHARGE);
		}
		break;
	case CTutorialManager::PROGRESS_CHARGE:	// チャージ射撃チュートリアル

		if (action == ACTION_CHARGE)
		{// チャージショットへ誘導
			EndProgress(PROGRESS_PARAM);
		}
		break;
	case CTutorialManager::PROGRESS_PARAM:	// パラメーター

		if (action == ACTION_TIME)
		{
			m_nCntProgress++;
		}

		if (m_nCntProgress > READ_PROGRESS)
		{// 次の進行状況に移行
			EndProgress(PROGRESS_ASSESS);
		}

		break;
	case CTutorialManager::PROGRESS_ASSESS:	// 評価

		if (action == ACTION_TIME)
		{
			m_nCntProgress++;
		}

		if (m_nCntProgress > READ_PROGRESS)
		{// 次の進行状況に移行
			EndProgress(PROGRESS_FREE);
		}
		break;
	case CTutorialManager::PROGRESS_MAX:
		break;
	default:
		break;
	}
}

//=====================================================
// 進行の終了
//=====================================================
void CTutorialManager::EndProgress(PROGRESS progress)
{
	m_nCntProgress = 0;

	m_NextProgress = progress;

	m_state = STATE_OUT;
}

//=====================================================
// キャプションの切り替え
//=====================================================
void CTutorialManager::NextProgress(PROGRESS progress)
{
	if (m_pCaption == nullptr)
	{
		return;
	}

	char *aPath[PROGRESS_MAX] =
	{
		"data\\TEXTURE\\UI\\tutorial00.png",
		"data\\TEXTURE\\UI\\tutorial01.png",
		"data\\TEXTURE\\UI\\tutorial02.png",
		"data\\TEXTURE\\UI\\tutorial03.png",
		"data\\TEXTURE\\UI\\tutorial04.png",
		"data\\TEXTURE\\UI\\tutorial05.png",
		"data\\TEXTURE\\UI\\tutorial06.png",
		"data\\TEXTURE\\UI\\tutorial07.png",
		"data\\TEXTURE\\UI\\tutorial08.png",
	};

	// テクスチャ番号取得
	int nIdx = CManager::GetTexture()->Regist(aPath[progress]);

	m_pCaption->SetIdxTexture(nIdx);

	m_progress = m_NextProgress;

	// イン状態にする
	m_state = STATE_IN;
}

//=====================================================
// 生成処理
//=====================================================
CTutorialManager *CTutorialManager::Create(void)
{
	CTutorialManager *pTutorialManager = nullptr;

	if (pTutorialManager == nullptr)
	{// インスタンス生成
		pTutorialManager = new CTutorialManager;

		if (pTutorialManager != nullptr)
		{
			// 初期化処理
			pTutorialManager->Init();

			pTutorialManager->m_pCaption = CObject2D::Create(6);

			if (pTutorialManager->m_pCaption != nullptr)
			{// 見出しの生成
				pTutorialManager->m_pCaption->SetPosition(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 600.0f, 0.0f));
				pTutorialManager->m_pCaption->SetSize(340.0f, 80.0f);

				// 初期の目標位置と同期する
				pTutorialManager->m_posDest = pTutorialManager->m_pCaption->GetPosition();

				pTutorialManager->m_pCaption->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));

				pTutorialManager->NextProgress(PROGRESS_START);

				pTutorialManager->m_pCaption->SetVtx();
			}
		}
	}

	return pTutorialManager;
}

//=====================================================
// 描画処理
//=====================================================
void CTutorialManager::Draw(void)
{
#ifdef  _DEBUG
	Debug();
#endif //  _DEBUG
}

//=====================================================
// デバッグ表示
//=====================================================
void CTutorialManager::Debug(void)
{
	CDebugProc *pDebugProc = CManager::GetDebugProc();

	if (pDebugProc == nullptr)
	{
		return;
	}

	pDebugProc->Print("\n現在の進行[%d]", m_progress);
	pDebugProc->Print("\n次の進行[%d]",m_NextProgress);
}