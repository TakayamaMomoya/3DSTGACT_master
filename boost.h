//*****************************************************
//
// ブースト表示の処理[boost.h]
// Author:髙山桃也
//
//*****************************************************

#ifndef _BOOST_H_
#define _BOOST_H_

#include "Object.h"

//*****************************************************
// 前方宣言
//*****************************************************
class CObject2D;

//*****************************************************
// クラスの定義
//*****************************************************
class CBoost : public CObject
{
public:
	CBoost(int nPriority = 6);	// コンストラクタ
	~CBoost();	// デストラクタ

	static CBoost *Create(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void) {};

private:
	CObject2D *m_pObjectGauge;	// ゲージのポインタ
	CObject2D *m_pCaption;	// 見出しのポインタ
};

#endif
