//*****************************************************
//
// リザルトの処理[result.h]
// Author:��山桃也
//
//*****************************************************

#ifndef _RESULT_H_
#define _RESULT_H_

//*****************************************************
// インクルード
//*****************************************************
#include "object.h"

//*****************************************************
// 前方宣言
//*****************************************************
class CObject2D;

//*****************************************************
// クラス定義
//*****************************************************
class CResult : CObject
{
public:
	CResult();	// コンストラクタ
	~CResult();	// デストラクタ

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CResult *Create(bool bWin = false);

private:
	enum MENU
	{// 項目の種類
		MENU_REPLAY = 0,	// リプレイ
		MENU_RANKING,	// ランキング
		MENU_MAX
	};

	enum STATE
	{// 状態
		STATE_NONE = 0,	// 何でもない状態
		STATE_WAIT,	// 入力待機状態
		STATE_CURRENT,	// 入力している状態
		STATE_MAX
	};

	void Input(void);
	void Fade(MENU menu);
	void Create2D(bool bWin);

	MENU m_menu;	// 選択項目

	CObject2D *m_apMenu[MENU_MAX];	// メニュー項目の２Dオブジェクト
	CObject2D *m_pBg;	// 背景のポインタ
	CObject2D *m_pCaption;	// 見出しのポインタ
	STATE m_state;	// 状態
};

#endif