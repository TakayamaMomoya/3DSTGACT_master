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
#define FRAME_PATH	"data\\TEXTURE\\UI\\frame05.png"	// スキップのフレームのパス
#define GAUGE_WIDTH	(136.0f)	// ゲージの横幅
#define GAUGE_HEIGTH	(19.0f)	// ゲージの縦の長さ
#define GAUGE_POS	(D3DXVECTOR3(10.0f,637.0f,0.0f))	// ゲージの位置
#define FRAME_WIDTH	(150.0f)	// フレームの横幅
#define FRAME_HEIGHT	(45.0f)	// フレームの縦の長さ
#define FRAME_POS	(D3DXVECTOR3(155.0f,620.0f,0.0f))	// フレームの位置
#define TIME_SKIP	(120)	// スキップまでのフレーム数

//=====================================================
// コンストラクタ
//=====================================================
CTutorialManager::CTutorialManager(int nPriority)
{
	m_nCntProgress = 0;
	m_nCntSkip = 0;
	m_progress = PROGRESS_START;
	m_pCaption = nullptr;
	m_pFrameSkip = nullptr;
	m_pGaugeSkip = nullptr;
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

	// ゲージの生成
	CreateGauge();

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

	if (m_pFrameSkip != nullptr)
	{
		m_pFrameSkip->Uninit();
		m_pFrameSkip = nullptr;
	}

	if (m_pGaugeSkip != nullptr)
	{
		m_pGaugeSkip->Uninit();
		m_pGaugeSkip = nullptr;
	}

	Release();
}

//=====================================================
// 更新処理
//=====================================================
void CTutorialManager::Update(void)
{
	// スキップゲージ管理
	ManageGauge();

	// 状態管理
	ManageState();

	// 時間経過によるチュートリアルの進行
	TimeTutorial();
	
	if (m_progress >= PROGRESS_SHOT && CEnemy::GetNumAll() == 0)
	{// 射撃以降は敵を出す
		CEnemy::Create(D3DXVECTOR3(500.0f, 0.0f, 0.0f), CEnemy::TYPE_TUTORIAL);
		CEnemy::Create(D3DXVECTOR3(-500.0f, 0.0f, 0.0f), CEnemy::TYPE_TUTORIAL);
	}
}

//=====================================================
// ゲージ管理
//=====================================================
void CTutorialManager::ManageGauge(void)
{
	if (m_pGaugeSkip == nullptr)
	{
		return;
	}

	CInputKeyboard *pKeyboard = CManager::GetKeyboard();
	CInputJoypad *pJoypad = CManager::GetJoypad();

	if (pKeyboard == nullptr || pJoypad == nullptr)
	{
		return;
	}

	if (m_nCntSkip != TIME_SKIP)
	{
		if (pKeyboard->GetPress(DIK_TAB) ||
			pJoypad->GetPress(CInputJoypad::PADBUTTONS_BACK, 0))
		{// スキップカウンター
			m_nCntSkip++;

			if (m_nCntSkip >= TIME_SKIP)
			{
				m_nCntSkip = TIME_SKIP;

				CFade *pFade = CManager::GetFade();

				if (pFade != nullptr)
				{
					pFade->SetFade(CScene::MODE_GAME);
				}
			}
		}
		else
		{
			m_nCntSkip = 0;
		}
	}

	float fWidth = (float)m_nCntSkip / (float)TIME_SKIP * GAUGE_WIDTH;

	// 位置設定
	D3DXVECTOR3 pos = { GAUGE_POS.x + fWidth,GAUGE_POS.y,0.0f };

	// サイズ設定
	m_pGaugeSkip->SetPosition(pos);
	m_pGaugeSkip->SetSize(fWidth, GAUGE_HEIGTH);
	m_pGaugeSkip->SetVtx();
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
	{
		if (ButtonCheck())
		{// ボタンを押したら進行
			AddProgress(ACTION_BUTTON);
		}
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

		if (action == ACTION_BUTTON)
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

		if (action == ACTION_BUTTON)
		{// 次の進行状況に移行
			EndProgress(PROGRESS_ASSESS);
		}

		break;
	case CTutorialManager::PROGRESS_ASSESS:	// 評価

		if (action == ACTION_BUTTON)
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
// ボタンを押したかのチェック
//=====================================================
bool CTutorialManager::ButtonCheck(void)
{
	CInputKeyboard *pKeyboard = CManager::GetKeyboard();
	CInputJoypad *pJoypad = CManager::GetJoypad();

	if (pKeyboard == nullptr || pJoypad == nullptr)
	{
		return false;
	}

	if (pKeyboard->GetTrigger(DIK_F) ||
		pJoypad->GetTrigger(CInputJoypad::PADBUTTONS_START, 0))
	{// チュートリアルを進める
		return true;
	}

	return false;
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
// ゲージの生成
//=====================================================
void CTutorialManager::CreateGauge(void)
{
	if (m_pGaugeSkip == nullptr)
	{// ゲージの生成
		m_pGaugeSkip = CObject2D::Create(7);

		if (m_pGaugeSkip != nullptr)
		{
			m_pGaugeSkip->SetPosition(D3DXVECTOR3(GAUGE_POS.x, GAUGE_POS.y, 0.0f));
			m_pGaugeSkip->SetSize(GAUGE_WIDTH, GAUGE_HEIGTH);
			m_pGaugeSkip->SetCol(D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f));
			m_pGaugeSkip->SetVtx();
		}
	}

	if (m_pFrameSkip == nullptr)
	{// フレームの生成
		m_pFrameSkip = CObject2D::Create(7);

		if (m_pFrameSkip != nullptr)
		{
			m_pFrameSkip->SetPosition(D3DXVECTOR3(FRAME_POS.x, FRAME_POS.y, 0.0f));
			m_pFrameSkip->SetSize(FRAME_WIDTH, FRAME_HEIGHT);
			m_pFrameSkip->SetVtx();

			// テクスチャ番号取得
			int nIdx = CManager::GetTexture()->Regist(FRAME_PATH);

			m_pFrameSkip->SetIdxTexture(nIdx);
		}
	}
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
	pDebugProc->Print("\n次の進行[%d]", m_NextProgress);
	pDebugProc->Print("\nスキップカウンター[%d]",m_nCntSkip);
}