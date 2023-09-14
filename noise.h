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

	static CNoise *Create(int nLife = 10,float fPeakAlpha = 0.3f);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static int GetNumAll(void) { return m_nNumAll; }

private:
	void ChangeCol(void);

	int m_nLife;	// 寿命
	int m_nAlpha;	// アルファテストの値
	float m_fAddAlpha;	// アルファテストの加算値
	float m_fPeakAlpha;	// α値がピークになるときの割合
	static int m_nNumAll;	// 総数
};

#endif