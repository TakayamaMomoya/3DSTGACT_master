//*****************************************************
//
// ゲーム処理[game.h]
// Author:髙山桃也
//
//*****************************************************
#ifndef _GAME_H_
#define _GAME_H_

//*****************************************************
// インクルード
//*****************************************************
#include "main.h"
#include "scene.h"

//*****************************************************
// 前方宣言
//*****************************************************
class CPlayer;
class CScore;
class CTimer;
class CGame;
class CDefend;
class CEdit;
class CMeshWall;
class CPause;
class CRank;

//*****************************************************
// クラスの定義
//*****************************************************
class CGame : public CScene
{
public:
	enum STATE
	{// 状態
		STATE_NONE,	// 何もしていない状態
		STATE_NORMAL,	// 通常状態
		STATE_END,	// 終了状態
		STATE_MAX
	};

	CGame();	// コンストラクタ
	~CGame();	// デストラクタ

	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update();
	virtual void Draw();
	static CScore *GetScore(void) { return m_pScore; }
	static CTimer *GetTimer(void) { return m_pTimer; }
	static CDefend *GetDefend(void) { return m_pDefend; }
	static CEdit *GetEdit(void) { return m_pEdit; }
	static CRank *GetRank(void) { return m_pRank; }
	static void ReleaseScore(void);
	static void ReleaseTimer(void);
	static void ReleaseDefend(void);
	bool IsPhoto(void) { return m_bPhoto; }
	static void EnablePause(bool bPause) { m_bPause = bPause; }
	static bool IsPause(void) { return m_bPause; }
	static void TogglePause(CPause *pPause);

private:
	enum WALLTYPE
	{
		WALLTYPE_FRONT = 0,	// 前方の壁
		WALLTYPE_RIGHT,	// 右の壁
		WALLTYPE_LEFT,	// 左の壁
		WALLTYPE_REAR,	// 後ろの壁
		WALLTYPE_MAX
	};

	void Debug(void);	// デバッグ処理
	void ManageState(void);	// 状態管理
	void LimitPlayerPos(void);	// プレイヤー移動制限
	void ManageWall(void);	// 壁の管理
	void TimeBonus(void);	// タイムボーナスの管理

	static CScore *m_pScore;	// スコアのポインタ
	static CTimer *m_pTimer;	// タイマーのポインタ
	static CDefend *m_pDefend;	// 護衛対象へのポインタ
	static CEdit *m_pEdit;	// エディターへのポインタ
	static CRank *m_pRank;	// ランクへのポインタ
	static STATE m_state;	// 状態
	CMeshWall *m_apMeshWall[WALLTYPE_MAX];	// 壁のポインタ
	bool m_bPhoto;	// フォトモードかどうか
	static bool m_bPause;	// ポーズかどうか
	static CPause *m_pPause;	// ポーズのアドレス
	int m_nTimerWave;	// ウェーブタイマー
	int m_nCntSecond;	// 一秒のカウンター
};

#endif