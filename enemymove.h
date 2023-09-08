//*****************************************************
//
// 動く敵の処理[enemymove.cpp]
// Author:髙山桃也
//
//*****************************************************

#ifndef _ENEMYMOVE_H_
#define _ENEMYMOVE_H_

#include "enemy.h"

//*****************************************************
// クラスの定義
//*****************************************************
class CEnemyMove : public CEnemy
{// 拠点に向かう敵
public:
	CEnemyMove();	// コンストラクタ
	~CEnemyMove();	// デストラクタ

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	void Tilt(void);	// 傾きの制御
	void ChaseDefend(void);	// 追跡処理
	void Fall(void);	// 墜落の動き

	float m_fAngleFall;	// 墜落角度
	float m_fRotateFall;	// 墜落時の回転量
	bool m_bFall;	// 墜落するかどうか
};

class CEnemyAttack : public CEnemy
{// プレイヤーに向かう敵
public:
	CEnemyAttack();	// コンストラクタ
	~CEnemyAttack();	// デストラクタ

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	typedef enum
	{// 状態
		STATE_NONE = 0,	// 何もしてない状態
		STATE_CHASE,	// 追跡状態
		STATE_TURN,	// ターン状態
		STATE_LEAVE,	// 離れる状態
		STATE_MAX
	}STATE;

	void Tilt(float fRotDiff);	// 傾きの制御
	void ChasePlayer(void);	// 追跡処理
	void ManageState(float fLength, float fRotDiff);	// 状態管理処理

	STATE m_state;	// 状態
	int m_nTimerLeave;	// 去るタイマー
};

#endif
