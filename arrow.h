//*****************************************************
//
// 矢印の処理[arrow.h]
// Author:髙山桃也
//
//*****************************************************

#ifndef _ARROW_H_
#define _ARROW_H_

//*****************************************************
// インクルード
//*****************************************************
#include "objectX.h"

//*****************************************************
// 前方宣言
//*****************************************************
class CObjectX;
class CNumber;

//*****************************************************
// クラスの定義
//*****************************************************
class CArrow : public CObjectX
{
public:
	CArrow(int nPriority = 3);	// コンストラクタ
	~CArrow();	// デストラクタ

	static CArrow *Create(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetDest(D3DXVECTOR3 pos) { m_posDest = pos; }

private:
	void RollPlayer(void);

	D3DXVECTOR3 m_posDest;	// 目的値
};

#endif
