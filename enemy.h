//*****************************************************
//
// 敵の処理[enemy.cpp]
// Author:髙山桃也
//
//*****************************************************

#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "motion.h"

//*****************************************************
// 前方宣言
//*****************************************************
class CCollisionSphere;
class CCollisionCube;
class CBillboard;
class CShadow;
class CExplSpawner;
class CArrow;

//*****************************************************
// クラスの定義
//*****************************************************
class CEnemy : public CMotion
{
public:
	typedef enum
	{// 敵の種類
		TYPE_NONE = 0,	// 何でもない
		TYPE_MOVE,	// 移動する敵
		TYPE_TANK,	// 戦車敵
		TYPE_ATTACK,	// 攻撃してくる敵
		TYPE_TUTORIAL,	// チュートリアルの敵
		TYPE_MAX
	}TYPE;

	typedef enum
	{// 状態
		STATE_NONE = 0,	// 何でもない状態
		STATE_NORMAL,	// 通常状態
		STATE_DAMAGE,	// ダメージ状態
		STATE_DEATH,	// 死亡状態
		STATE_MAX
	}STATE;

	enum MOVESTATE
	{// 移動状態
		MOVESTATE_NONE = 0,	// 何でもない状態
		MOVESTATE_CHASE,	// 目標追跡状態
		MOVESTATE_AVOID,	// 障害物を避ける状態
		MOVESTATE_MAX
	};

	CEnemy();	// コンストラクタ
	~CEnemy();	// デストラクタ

	static CEnemy *Create(D3DXVECTOR3 pos,TYPE type);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static HRESULT Load(void);	// 読込
	static void Unload(void);
	int GetCntAttack(void) { return m_nCntAttack; }
	void SetCntAttack(int nValue);
	static int GetNumAll(void) { return m_nNumAll; }
	void SetLife(float fLife);
	float GetLife(void) { return m_fLife; }
	void Hit(float fDamage);
	bool IsLockon(void) { return m_bLockon; }
	void SetLockon(bool bLockon) { m_bLockon = bLockon; }
	static CEnemy *GetEnemy(void) { return m_pHead; }
	static void SpawnEnemy(int nNumWave = 1);
	void SetSpherePosition(D3DXVECTOR3 pos);
	void Death(void);
	STATE GetState(void) { return m_state; }
	MOVESTATE GetMoveState(void) { return m_moveState; }
	D3DXVECTOR3 GetPosDest(void) { return m_posDest; }
	void SetPosDest(D3DXVECTOR3 pos) { m_posDest = pos; }
	CCollisionSphere *GetClsnSphere(void) { return m_pCollisionSphere; }
	bool IsFind(void) { return m_bFind; }
	CEnemy *GetNext(void) { return m_pNext; }
	int GetScore(void) { return m_nScore; }
	void SetScore(int nScore) { m_nScore = nScore; }

protected:
	void AvoidCollisions(void);
	void ManagePosDest(void);

private:
	typedef struct
	{// 敵の情報
		TYPE type;	// 種類
		D3DXVECTOR3 pos;	// 位置
	}ENEMYINFO;

	typedef struct
	{// ウェーブの情報
		ENEMYINFO *pEnemyInfo;	// 敵の情報
		int m_nNumEnemy;
	}WAVEINFO;

	void ManageState(void);
	void SearchTarget(void);
	void ManageBonus(void);
	void ManageCollision(void);

	int m_nCntAttack;	// 攻撃をするまでの時間	
	static int m_nNumAll;	// 総数
	int m_nId;	// 番号
	int m_nTimerState;	// 状態遷移タイマー
	float m_fLife;	// 体力
	CCollisionSphere *m_pCollisionSphere;	// 球の当たり判定
	CCollisionCube *m_pCollisionCube;	// 立方体の当たり判定
	bool m_bLockon;	// ロックオンされているかどうか
	bool m_bFind;	// 見つけているかどうか
	static int m_nNumWave;	// ウェーブ数
	static WAVEINFO *m_pWaveInfo;	// ウェーブ情報のポインタ
	CShadow *m_pShadow;	// 影のポインタ
	STATE m_state;	// 状態
	int m_nTimerDeath;	// 死亡までのカウンタ
	CExplSpawner **m_ppExplSpawner;	// 爆発のダブルポインタ
	D3DXVECTOR3 m_posDest;	// 目標位置
	MOVESTATE m_moveState;	// 移動状態
	int m_nScore;	// スコア値
	int m_nCntClose;	// 接近カウンター

	static CEnemy *m_pHead;	// 先頭のアドレス
	static CEnemy *m_pTail;	// 最後尾のアドレス
	CEnemy *m_pPrev;	// 前のアドレス
	CEnemy *m_pNext;	// 次のアドレス
};

#endif
