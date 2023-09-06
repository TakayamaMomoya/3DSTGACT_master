//*****************************************************
//
// 護衛目標の処理[defend.h]
// Author:髙山桃也
//
//*****************************************************

#ifndef _DEFEND_H_
#define _DEFEND_H_

//*****************************************************
// インクルード
//****************************************************
#include "main.h"
#include "object.h"

//*****************************************************
// 前方宣言
//*****************************************************
class CCollisionCube;
class CCollisionSphere;
class CObjectX;

//*****************************************************
// クラス定義
//****************************************************
class CDefend : public CObject
{
public:
	CDefend();	// コンストラクタ
	~CDefend();	// デストラクタ

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	D3DXVECTOR3 GetPosition(void);
	static CDefend *Create(void);
	void Hit(float fDamage);
	int GetLife(void) { return m_nLife; }
	void SetLife(int nLife);

private:
	enum STATE
	{// 状態
		STATE_NONE = 0,	// 何でもない状態
		STATE_NORMAL,	// 通常状態
		STATE_DEATH,	// 死亡状態
		STATE_MAX
	};

	void ManageState(void);	// 状態管理

	CObjectX *m_pObjectX;	// モデルオブジェクトへのポインタ
	CCollisionCube *m_pCollisionCube;	// 立方体の当たり判定
	CCollisionSphere *m_pCollisionSphere;	// 球の当たり判定
	int m_nLife;	// 体力
	int m_nTimerDeath;	// 死亡タイマー
	STATE m_state;	// 状態
};

#endif