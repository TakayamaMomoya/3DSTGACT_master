//*****************************************************
//
// 戦車敵の処理[enemytank.cpp]
// Author:髙山桃也
//
//*****************************************************

#ifndef _ENEMYTANK_H_
#define _ENEMYTANK_H_

#include "enemy.h"

//*****************************************************
// クラスの定義
//*****************************************************
class CEnemyTank : public CEnemy
{
public:
	CEnemyTank();	// コンストラクタ
	~CEnemyTank();	// デストラクタ

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	void ChaseDefend(void);	// 追跡処理
};

#endif
