//*****************************************************
//
// 煙の処理[smoke.cpp]
// Author:髙山桃也
//
//*****************************************************

#ifndef _SMOKE_H_
#define _SMOKE_H_

#include "ObjectX.h"

//*****************************************************
// クラスの定義
//*****************************************************
class CSmoke : public CObjectX
{
public:
	CSmoke(int nPriority = 3);	// コンストラクタ
	~CSmoke();	// デストラクタ

	static CSmoke *Create(D3DXVECTOR3 pos,int nLife);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	int m_nLife;	// 寿命
	static int m_nNumAll;	// 総数
};

#endif
