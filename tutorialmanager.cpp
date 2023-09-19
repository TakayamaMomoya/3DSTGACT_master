//*****************************************************
//
// 評価の処理[assess.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "tutorialmanager.h"

//=====================================================
// コンストラクタ
//=====================================================
CTutorialManager::CTutorialManager(int nPriority)
{

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