//*****************************************************
//
// コックピットの処理[cockpit.h]
// Author:髙山桃也
//
//*****************************************************

#ifndef _COCKPIT_H_
#define _COCKPIT_H_

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
class CCockPit : public CObject
{
public:
	CCockPit(int nPriority = 3);	// コンストラクタ
	~CCockPit();	// デストラクタ

	static CCockPit *Create(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

private:
	enum PARAM
	{
		PARAM_RAPID = 0,	// 連射力
		PARAM_POWER,	// 攻撃力
		PARAM_COST,	// 効率
		PARAM_MAX
	};
	void ManageLife(void);
	void ManageParam(void);

	CObject2D *m_pFrame;	// フレームのポインタ
	CObject2D *m_apGauge[PARAM_MAX];	// ゲージのポインタ
	CObject2D *m_apFrame[PARAM_MAX];	// フレームのポインタ
	CObject2D *m_apIcon[PARAM_MAX];	// アイコンのポインタ
	CNumber *m_pLife;	// 残り体力表示
};

#endif
