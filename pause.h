//*****************************************************
//
// ポーズの処理[pause.h]
// Author:髙山桃也
//
//*****************************************************

#ifndef _PAUSE_H_
#define _PAUSE_H_

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
class CPause : CObject
{
public:
	CPause();	// コンストラクタ
	~CPause();	// デストラクタ

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CPause *Create(void);

private:
	enum MENU
	{// 項目の種類
		MENU_RESUME = 0,	// 再開
		MENU_RETRY,	// リトライ
		MENU_QUIT,	// 終了
		MENU_MAX
	};

	void Input(void);
	void Fade(MENU menu);

	MENU m_menu;	// 選択項目

	CObject2D *m_apMenu[MENU_MAX];	// メニュー項目の２Dオブジェクト
	CObject2D *m_pBg;	// 背景のポインタ
};

#endif