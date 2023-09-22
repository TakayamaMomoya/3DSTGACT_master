//*****************************************************
//
// チュートリアル管理の処理[tutorialmanager.h]
// Author:髙山桃也
//
//*****************************************************

#ifndef _TUTORIALMANAGER_H_
#define _TUTORIALMANAGER_H_

//*****************************************************
// インクルード
//*****************************************************
#include "object.h"

//*****************************************************
// 前方宣言
//*****************************************************
class CObject2D;

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
		PROGRESS_BOOST,	// ブースト
		PROGRESS_SHOT,	// 射撃
		PROGRESS_RAPID,	// 連射
		PROGRESS_CHARGE,	// チャージ射撃
		PROGRESS_PARAM,	// パラメーター
		PROGRESS_ASSESS,	// 評価
		PROGRESS_FREE,	// フリー
		PROGRESS_MAX
	};
	enum ACTION
	{// アクション
		ACTION_MOVE,	// 移動
		ACTION_BOOST,	// ブースト
		ACTION_SHOT,	// 通常射撃
		ACTION_RAPID,	// 連射射撃
		ACTION_CHARGE,	// チャージ射撃
		ACTION_BUTTON,	// 時間経過
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
	enum STATE
	{
		STATE_NONE = 0,	// 何でもない状態
		STATE_IN,	// 出現状態
		STATE_WAIT,	// 待機状態
		STATE_OUT,	// 消滅状態
		STATE_MAX
	};

	void Debug(void);
	void ManageGauge(void);
	void ManageState(void);
	void NextProgress(PROGRESS progress);
	void TimeTutorial(void);
	void EndProgress(PROGRESS progress);
	bool ButtonCheck(void);
	void CreateGauge(void);

	PROGRESS m_progress;	// 進行状況
	PROGRESS m_NextProgress;	// 次の進行状況
	int m_nCntProgress;	// 進行カウンター
	CObject2D *m_pCaption;	// 説明文のポインタ
	CObject2D *m_pFrameSkip;	// スキップ表示のフレーム
	CObject2D *m_pGaugeSkip;	// スキップ表示のゲージ

	D3DXVECTOR3 m_posDest;	// 目標位置
	STATE m_state;	// 状態
	int m_nTimerOut;	// フェードアウトまでのタイマー
	int m_nCntSkip;	// スキップカウンター
};

#endif