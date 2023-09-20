//*****************************************************
//
// チュートリアル用敵の処理[enemytutorial.cpp]
// Author:髙山桃也
//
//*****************************************************

#ifndef _ENEMYTUTORIAL_H_
#define _ENEMYTUTORIAL_H_

#include "enemy.h"

//*****************************************************
// クラスの定義
//*****************************************************
class CEnemyTutorial : public CEnemy
{// 拠点に向かう敵
public:
	CEnemyTutorial();	// コンストラクタ
	~CEnemyTutorial();	// デストラクタ

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	void Fall(void);	// 墜落の動き

	float m_fAngleFall;	// 墜落角度
	float m_fRotateFall;	// 墜落時の回転量
	bool m_bFall;	// 墜落するかどうか
};

#endif
