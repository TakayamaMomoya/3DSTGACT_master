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

//*****************************************************
// マクロ定義
//*****************************************************
#define MOVE_PROGRESS	(120)	// 移動を検出するフレーム数

//=====================================================
// コンストラクタ
//=====================================================
CTutorialManager::CTutorialManager(int nPriority)
{
	m_nCntProgress = 0;
	m_progress = PROGRESS_START;
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
	m_progress = PROGRESS_MOVE;

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CTutorialManager::Uninit(void)
{
	Release();
}

//=====================================================
// 更新処理
//=====================================================
void CTutorialManager::Update(void)
{

}

//=====================================================
// 進行状況の進行
//=====================================================
void CTutorialManager::AddProgress(ACTION action)
{
	switch (m_progress)
	{
	case CTutorialManager::PROGRESS_START:
		break;
	case CTutorialManager::PROGRESS_MOVE:

		if (action == ACTION_MOVE)
		{
			m_nCntProgress++;
		}

		if (m_nCntProgress > MOVE_PROGRESS)
		{// 次の進行状況に移行
			m_nCntProgress = 0;

			m_progress = PROGRESS_SHOT;
		}

		break;
	case CTutorialManager::PROGRESS_SHOT:

		if (action == ACTION_SHOT)
		{// チャージショットへ誘導
			m_progress = PROGRESS_CHARGE;
		}

		break;
	case CTutorialManager::PROGRESS_MAX:
		break;
	default:
		break;
	}
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

		// 初期化処理
		pTutorialManager->Init();
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

	pDebugProc->Print("\n現在の進行[%d]",m_progress);
}