//*****************************************************
//
// ランクの処理[rank.h]
// Author:髙山桃也
//
//*****************************************************

#ifndef _RANK_H_
#define _RANK_H_

//*****************************************************
// インクルード
//*****************************************************
#include "object.h"

//*****************************************************
// 前方宣言
//*****************************************************
class CObject2D;
class CNumber;

//*****************************************************
// クラスの定義
//*****************************************************
class CRank : public CObject
{
public:
	CRank(int nPriority = 3);	// コンストラクタ
	~CRank();	// デストラクタ

	static CRank *Create(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void AddProgress(float fValue);

private:
	void ManageGauge(void);	// 進行ゲージの管理

	CObject2D *m_pFrame;	// フレームのポインタ
	CObject2D *m_pGaugeProgress;	// 進行度合いのゲージ
	CNumber *m_pRankNum;	// ランクのポインタ
	float m_fProgress;	// ランクの進行度合
	int m_nRank;	// ランク
};

#endif