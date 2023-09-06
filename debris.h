//*****************************************************
//
// 破片の処理[debris.cpp]
// Author:髙山桃也
//
//*****************************************************

#ifndef _DEBRIS_H_
#define _DEBRIS_H_

#include "ObjectX.h"

//*****************************************************
// クラスの定義
//*****************************************************
class CDebris : public CObjectX
{
public:
	CDebris(int nPriority = 3);	// コンストラクタ
	~CDebris();	// デストラクタ

	static CDebris *Create(D3DXVECTOR3 pos,int nLife);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	int m_nLife;	// 寿命
	static int m_nNumAll;	// 総数
};

#endif
