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
	CAssess(int nPriority = 3);	// コンストラクタ
	~CAssess();	// デストラクタ

	static CAssess *Create(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void AddHit(float fHit) { m_fAssessHit += fHit; }
	void AddDodge(float fDodge) { m_fAssessDodge += fDodge; }
	void AddAttack(float fAttack) { m_fAssessAttack += fAttack; }
	float GetHit(void) { return m_fAssessHit; }
	float GetDodge(void) { return m_fAssessDodge; }
	float GetAttack(void) { return m_fAssessAttack; }

private:
	enum PARAM
	{
		PARAM_HIT = 0,	// 命中率
		PARAM_DODGE,	// 回避率
		PARAM_ATTACK,	// 攻撃
		PARAM_MAX
	};

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
	};

	void ManageGauge(void);	// ゲージの管理
	void LimitAssess(void);	// 評価の制限

	float m_fAssessHit;	// ヒット率の評価
	float m_fAssessDodge;	// 回避率の評価
	float m_fAssessAttack;	// 攻撃効率の評価
	Param *m_apParam[PARAM_MAX];	// パラメーターの情報
};

#endif