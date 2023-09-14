//*****************************************************
//
// ノイズの処理[noise.h]
// Author:髙山桃也
//
//*****************************************************

#ifndef _NOISE_H_
#define _NOISE_H_

//*****************************************************
// インクルード
//*****************************************************
#include "animation2D.h"

//*****************************************************
// クラスの定義
//*****************************************************
class CNoise : public CAnim2D
{
public:
	CNoise(int nPriority = 7);	// コンストラクタ
	~CNoise();	// デストラクタ

	static CNoise *Create(int nLife = 10);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static int GetNumAll(void) { return m_nNumAll; }

private:
	int m_nLife;	// 寿命
	static int m_nNumAll;	// 総数
};

#endif