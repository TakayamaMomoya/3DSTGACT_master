//*****************************************************
//
// チュートリアルプレイヤーの処理[tutorialplayer.cpp]
// Author:髙山桃也
//
//*****************************************************

#ifndef _TUTORIALPLAYER_H_
#define _TUTORIALPLAYER_H_

//*****************************************************
// インクルード
//*****************************************************
#include "player.h"

//*****************************************************
// クラスの定義
//*****************************************************
class CTutorialPlayer : public CObject
{
public:
	typedef enum
	{// モーションのタイプ
		MOTION_NEUTRAL = 0,
		MOTION_WALK,
		MOTION_SHOT,
		MOTION_BOOST,
		MOTION_TURNWALK,
		MOTION_TURNBOOST,
		MOTION_MAX,
	}MOTION;

	typedef enum
	{// ブーストの状態
		BOOSTSTATE_NONE = 0,	// 何でもない
		BOOSTSTATE_EMPTY,	// 欠乏状態
		BOOSTSTATE_MAX
	}BOOSTSTATE;

	CTutorialPlayer(int nPriority = 4);	// コンストラクタ
	~CTutorialPlayer();	// デストラクタ

	static CTutorialPlayer *Create(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	HRESULT Load(void);	// 読込
	void Hit(float fDamage);
	bool IsAim(void) { return m_bAim; }
	void SetPosition(D3DXVECTOR3 pos) { m_pos = pos; }
	D3DXVECTOR3 GetPosition(void) { return m_pos; }
	void SetMove(D3DXVECTOR3 move) { m_move = move; }
	D3DXVECTOR3 GetMove(void) { return m_move; }
	void SetPositionOld(D3DXVECTOR3 pos) { m_posOld = pos; }
	D3DXVECTOR3 GetPositionOld(void) { return m_posOld; }
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	void LimitUpperRot(float fRotOld);
	float GetBoost(void) { return m_fBoost; }
	BOOSTSTATE GetBoostState(void) { return m_boostState; }
	int GetAirCounter(void) { return m_nCounterAir; }
	void SetAirCounter(int nCounter) { m_nCounterAir = nCounter; }
	bool IsLand(void) { return m_bLand; }
	int GetLife(void) { return m_nLife; }
	float GetRapid(void) { return m_fParamRapid; }
	float GetPower(void) { return m_fParamPower; }
	float GetCost(void) { return m_fParamCost; }
	int GetNumShot(void) { return m_nNumShot; }
	int GetNumHit(void) { return m_nNumHit; }
	void SetNumHit(int nNumHit);
	void SetNumShot(int nNumShot) { m_nNumShot = nNumShot; }
	CAssess *GetAssess(void) { return m_pAssess; }
	CEnemy *GetLockEnemy(void) { return m_pEnemy; }

private:
	enum STATE
	{
		STATE_NONE = 0,	// 何でもない状態
		STATE_NORMAL,	// 通常状態
		STATE_DEATH,	// 死亡状態
	};

	void Input(void);
	void Turn(D3DXVECTOR3 move, float fRotOld);
	void Lockon(void);
	void ManageState(void);
	void ManageOffset(void);
	void ManageMotion(void);
	void ManageBoost(float fBoostOld);
	void ManageCollision(void);
	bool CollisionField(void);
	void InputMove(void);
	void InputShot(void);
	void InputCamera(void);
	void SetMatrix(void);
	D3DXVECTOR3 LinePridiction(D3DXVECTOR3 pos, float fSpeedBullet, D3DXVECTOR3 posTarget, D3DXVECTOR3 moveTarget);
	float PlusMin(float fData1, float fData2);
	void Boost(void);
	void Death(void);

	int m_nLife;	// 体力
	int m_nCntDeath;	// 死亡までのカウンター
	float m_fBoost;	// ブースト残量
	int m_nTimerRapid;	// 連射するまでの猶予のカウンター
	int m_nTimerShot;	// 単発で撃てる間隔
	int m_nCounterAir;	// 空中カウンター
	int m_nNumShot;	// 射撃回数
	int m_nNumHit;	// 命中回数
	int m_nCntSound;	// サウンドを流すカウンター
	CShadow *m_pShadow;	// 影
	bool m_bAim;	// エイムしているかどうか
	bool m_bLand;
	bool m_bSprint;	// ダッシュ状態かどうか
	bool m_bJump;	// ジャンプしているかどうか
	bool m_bTurnDowner;	// 下半身が回転しているかどうか
	CCollisionSphere *m_pCollisionSphere;
	CCollisionCube *m_pCollisionCube;
	STATE m_state;	// 状態
	D3DXMATRIX m_mtxMazzle[2];	// 銃口のマトリックス
	D3DXVECTOR3 m_offsetMazzle[2];	// 銃口のオフセット位置
	D3DXVECTOR3 m_posMazzle;	// 銃口座標
	D3DXVECTOR3 m_pos;	// 位置
	D3DXVECTOR3 m_posOld;	// 前回の位置
	D3DXVECTOR3 m_move;	// 移動量
	D3DXVECTOR3 m_rot;	// 向き
	D3DXVECTOR3 m_rotDest;	// 向く方向
	D3DXVECTOR3 m_rotUpper;	// 上半身の向き
	D3DXVECTOR3 m_rotDowner;	// 下半身の向き
	D3DXMATRIX m_mtxLockon[2];	// ロックオンのマトリックス
	D3DXMATRIX m_mtxWorld;	// ワールドマトリックス
	D3DXMATRIX m_mtxWaist;	// 腰のマトリックス
	D3DXVECTOR3 m_offsetLockon[2];	// ロックオンのオフセット位置
	D3DXVECTOR3 m_posLockon;	// ロックオン座標のポインタ
	D3DXVECTOR3 m_moveLockon;	// ロックオンしてる敵の移動量
	D3DXVECTOR3 m_posBoost;	// ブースト座標
	D3DXVECTOR3 m_vecBoost;	// ブーストベクトル
	CEnemy *m_pEnemy;	// ロックオンしてる敵のポインタ
	CMotion *m_pBodyUpper;	// 上半身のポインタ
	CMotion *m_pBodyDowner;	// 下半身のポインタ
	BOOSTSTATE m_boostState;	// ブーストの状態
	CCockPit *m_pCockpit;	// コックピットのポインタ
	CRader *m_pRader;	// レーダーのポインタ
	CAssess *m_pAssess;	// 評価ゲージのポインタ
	CLockon *m_pLockon;	// ロックオンのポインタ

	float m_fParamRapid;	// 連射力のパラメーター
	float m_fParamPower;	// 攻撃系のパラメーター
	float m_fParamCost;	// 効率系のパラメーター
	float m_fGrawRapid;	// 連射力成長率
	float m_fGrawPower;	// 攻撃力成長率
	float m_fGrawCost;	// 効率成長率
};

#endif
