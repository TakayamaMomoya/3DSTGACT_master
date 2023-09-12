//*****************************************************
//
// 評価の処理[assess.h]
// Author:髙山桃也
//
//*****************************************************

#ifndef _ASSESS_H_
#define _ASSESS_H_

//*****************************************************
// インクルード
//*****************************************************
#include "object.h"

//*****************************************************
// 前方宣言
//*****************************************************
class CObject2D;

//*****************************************************
// クラスの定義
//*****************************************************
class CAssess : public CObject
{
public:
	enum PARAM
	{
		PARAM_HIT = 0,	// 命中率
		PARAM_DODGE,	// 回避率
		PARAM_ATTACK,	// 攻撃
		PARAM_MAX
	};

	CAssess(int nPriority = 3);	// コンストラクタ
	~CAssess();	// デストラクタ

	static CAssess *Create(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void AddParam(float fValue, PARAM param);
	float GetParam(PARAM param);

private:
	enum GAUGESTATE
	{
		GAUGESTATE_EXTEND = 0,	// 伸びる状態
		GAUGESTATE_SHRINK,	// 縮む状態
		GAUGESTATE_MAX
	};

	struct Param
	{
		CObject2D *pGauge;	// ゲージのポインタ
		CObject2D *pFrame;	// フレームのポインタ
		GAUGESTATE state;	// 状態
		float fParam;	// パラメーター値
		float fWidthDest;	// 幅の目的値
	};

	void ManageGauge(void);	// ゲージの管理
	void PalsingGauge(Param *pParam);	// ゲージの脈動
	void SetWidthDest(Param *pParam);	// 目的の幅設定

	Param *m_apParam[PARAM_MAX];	// パラメーターの情報
};

#endif