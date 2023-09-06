//*****************************************************
//
// タイトル処理[title.h]
// Author:髙山桃也
//
//*****************************************************
#ifndef _TITLE_H_
#define _TITLE_H_

//*****************************************************
// インクルード
//*****************************************************
#include "main.h"
#include "scene.h"

//*****************************************************
// クラスの定義
//*****************************************************
class CTitle : public CScene
{
public:
	CTitle();	// コンストラクタ
	~CTitle();	// デストラクタ

	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update();
	virtual void Draw();

private:
};

#endif