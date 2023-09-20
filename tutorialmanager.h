//*****************************************************
//
// チュートリアル管理の処理[tutorialmanager.h]
// Author:��山桃也
//
//*****************************************************

#ifndef _TUTORIALMANAGER_H_
#define _TUTORIALMANAGER_H_

//*****************************************************
// インクルード
//*****************************************************
#include "object.h"

//*****************************************************
// クラスの定義
//*****************************************************
class CTutorialManager : public CObject
{
public:
	enum PROGRESS
	{// 進行状況
		PROGRESS_START,	// 開始
		PROGRESS_MOVE,	// 移動
		PROGRESS_SHOT,	// 射撃
		PROGRESS_CHARGE,	// チャージ射撃
		PROGRESS_MAX
	};
	enum ACTION
	{// アクション
		ACTION_MOVE,	// 移動
		ACTION_SHOT,	// 通常射撃
		ACTION_RAPID,	// 連射射撃
		ACTION_CHARGE,	// チャージ射撃
		ACTION_MAX
	};

	CTutorialManager(int nPriority = 3);	// コンストラクタ
	~CTutorialManager();	// デストラクタ

	static CTutorialManager *Create(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	void AddProgress(ACTION action);
	PROGRESS GetProgress(void) { return m_progress; }

private:
	void Debug(void);

	PROGRESS m_progress;	// 進行状況
	int m_nCntProgress;	// 進行カウンター
};

#endif