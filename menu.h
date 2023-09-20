//*****************************************************
//
// メニューの処理[menu.h]
// Author:髙山桃也
//
//*****************************************************

#ifndef _MENU_H_
#define _MENU_H_

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
class CMenu : CObject
{
public:
	CMenu();	// コンストラクタ
	~CMenu();	// デストラクタ

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CMenu *Create(void);

private:
	enum MENU
	{// 項目の種類
		MENU_GAME = 0,	// 再開
		MENU_TUTORIAL,	// チュートリアル
		MENU_MAX
	};

	void Input(void);
	void Fade(MENU menu);

	MENU m_menu;	// 選択項目

	CObject2D *m_apMenu[MENU_MAX];	// メニュー項目の２Dオブジェクト
	CObject2D *m_pCaption;	// 見出しのポインタ
};

#endif