//*****************************************************
//
// チュートリアル処理[tutorial.h]
// Author:髙山桃也
//
//*****************************************************
#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_

//*****************************************************
// インクルード
//*****************************************************
#include "main.h"
#include "scene.h"

//*****************************************************
// 前方宣言
//*****************************************************
class CPlayer;
class CMeshField;

//*****************************************************
// クラスの定義
//*****************************************************
class CTutorial : public CScene
{
public:
	enum PROGRESS
	{// 進行状況
		PROGRESS_START,	// 開始
		PROGRESS_SHOT,	// 射撃
		PROGRESS_MAX
	};
	enum ACTION
	{// アクション
		ACTION_SHOT,	// 通常射撃
		ACTION_RAPID,	// 連射射撃
		ACTION_CHARGE,	// チャージ射撃
		ACTION_MAX
	};

	CTutorial();	// コンストラクタ
	~CTutorial();	// デストラクタ

	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update();
	virtual void Draw();

	void AddProgress(ACTION action);

private:
	void Debug(void);	// デバッグ処理
	void ManageState(void);	// 状態管理
	void LimitPlayerPos(void);	// プレイヤー移動制限

	PROGRESS m_progress;	// 進行状況
};

#endif