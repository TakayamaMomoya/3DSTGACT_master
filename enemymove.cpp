//*****************************************************
//
// 動く敵の処理[enemymove.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "main.h"
#include "enemymove.h"
#include "renderer.h"
#include "manager.h"
#include "player.h"
#include "bullet.h"
#include "defend.h"
#include "universal.h"
#include "debugproc.h"
#include "game.h"
#include "meshfield.h"
#include "particle.h"
#include "debrisspawner.h"
#include "missile.h"

//*****************************************************
// マクロ定義
//*****************************************************
#define SPEED_MOVE	(6.0f)	// 移動速度
#define SPEED_FIGHTER	(30.0f)	// 戦闘機の移動速度
#define ATTACK_FREQ	(180)	// 攻撃する頻度
#define BULLET_SPEED	(30.0f)	// 弾のスピード
#define INITIAL_HEIGHT	(300.0f)	// 初期の高さ
#define HEIGHT_FIGHTER	(900.0f)	// 戦闘機の初期の高さ
#define ATTACK_RANGE	(3500.0f)	// 攻撃する距離
#define DEFAULT_SPEED	(4.0f)	// 基準のスピード
#define DEFAULT_TILT	(-D3DX_PI * 0.1f)	// 基準の傾き
#define ROT_DIFF	(D3DX_PI * 0.2f)	// 基準の向き
#define TURN_ROT	(D3DX_PI * 0.01f)	// ターンするまでの角度
#define LIMIT_TILT	(D3DX_PI * 0.5f)	// 角度の制限
#define LIFE_HELI	(5)	// ヘリの初期体力
#define LIFE_FIGHTER	(10)	// 戦闘機の初期体力
#define FALL_SPEED	(4.0f)	// 墜落速度
#define ROLL_FACT	(0.05f)	// 旋回速度
#define INITIAL_SCORE	(500)	// 初期スコア
#define FALL_POSSIBILITY	(20)	// 墜落するパーセンテージ

//=====================================================
// コンストラクタ
//=====================================================
CEnemyMove::CEnemyMove()
{
	m_fAngleFall = 0.0f;
	m_fRotateFall = 0.0f;
	m_bFall = false;
}

//=====================================================
// デストラクタ
//=====================================================
CEnemyMove::~CEnemyMove()
{

}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CEnemyMove::Init(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// 継承クラスの初期化
	CEnemy::Init();

	// 少し上に上がる
	SetPosition(D3DXVECTOR3(GetPosition().x, GetPosition().y + INITIAL_HEIGHT, GetPosition().z));

	// 初期の体力設定
	SetLife(LIFE_HELI);
	
	// 墜落時の向き誤差を設定
	m_fAngleFall = (float)(rand() % (int)(3.14 * 100.0f) - 3.14 * 50.0f) / 100.0f;

	// 墜落時の回転量を設定
	m_fRotateFall = (float)(rand() % (int)(0.15f * 50.0f) + 0.15f * 50.0f) / 100.0f;

	// スコア設定
	SetScore(INITIAL_SCORE);

	// 墜落するかどうかの設定
	int nRand = rand() % 100;

	if (nRand < FALL_POSSIBILITY)
	{
		m_bFall = true;
	}

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CEnemyMove::Uninit(void)
{
	// 継承クラスの終了
	CEnemy::Uninit();
}

//=====================================================
// 更新処理
//=====================================================
void CEnemyMove::Update(void)
{
	// 位置を保存
	SetPositionOld(GetPosition());

	if (GetState() != STATE_DEATH)
	{
		// 追跡処理
		CEnemyMove::ChaseTarget();

		// 傾きの制御
		Tilt();
	}
	else
	{
		if (m_bFall)
		{
			// 墜落
			Fall();
		}
		else
		{
			Death();
		}
	}

	if (GetState() == STATE_DEATH)
	{
		// 移動量減衰
		SetMove(GetMove() *= 0.98f);
	}
	else
	{
		// 移動量減衰
		SetMove(D3DXVECTOR3(GetMove().x * 0.98f,0.0f, GetMove().z * 0.98f));
	}

	// 継承クラスの更新
	CEnemy::Update();
}

//=====================================================
// 傾きの制御処理
//=====================================================
void CEnemyMove::Tilt(void)
{
	// 汎用処理取得
	CUniversal *pUniversal = CManager::GetUniversal();

	// 変数宣言
	D3DXVECTOR3 move, rot;
	float fSpeed,fTilt;

	// 情報取得
	move = GetMove();
	rot = GetRot();

	// 速度を取得
	fSpeed = D3DXVec3Length(&move);

	// 傾き具合を算出
	fTilt = -D3DX_PI - fSpeed / DEFAULT_SPEED * DEFAULT_TILT;

	if (pUniversal != nullptr)
	{// 傾く処理
		pUniversal->FactingRot(&rot.x, fTilt, 0.98f);
	}
	
	// 向きの反映
	SetRot(rot);
}

//=====================================================
// 目標追跡
//=====================================================
void CEnemyMove::ChaseTarget(void)
{
	// プレイヤー情報取得
	CPlayer *pPlayer = CGame::GetPlayer();

	if (pPlayer != nullptr)
	{
		if (GetMoveState() == MOVESTATE_CHASE)
		{
			SetPosDest(pPlayer->GetPosition());
		}
	}

	// 変数宣言
	D3DXVECTOR3 vecDest;

	// 差分を取得
	vecDest = GetPosDest() - GetPosition();

	float length = D3DXVec3Length(&vecDest);

	// 目標角度を取得
	float fRotDest = atan2f(vecDest.x, vecDest.z);

	float fRotDiff = fRotDest - GetRot().y;

	// 角度の修正
	if (fRotDiff > D3DX_PI)
	{
		fRotDiff -= 6.28f;
	}
	if (fRotDiff < -D3DX_PI)
	{
		fRotDiff += 6.28f;
	}

	// 角度補正
	SetRot(D3DXVECTOR3(GetRot().x, GetRot().y + fRotDiff * ROLL_FACT, GetRot().z));

	// 角度の修正
	if (GetRot().y > D3DX_PI)
	{
		// 角度補正
		SetRot(D3DXVECTOR3(GetRot().x, GetRot().y - 6.28f, GetRot().z));
	}
	if (GetRot().y < -D3DX_PI)
	{
		// 角度補正
		SetRot(D3DXVECTOR3(GetRot().x, GetRot().y + 6.28f, GetRot().z));
	}

	if (length > ATTACK_RANGE)
	{// 目標から離れていたら
		// 目標方向へ進む
		SetMove(D3DXVECTOR3
		(
			sinf(GetRot().y) * SPEED_MOVE,
			GetMove().y,
			cosf(GetRot().y) * SPEED_MOVE
		));
	}
	else
	{// 目標に近づいていたら
		if (GetMoveState() == MOVESTATE_CHASE)
		{
			if (GetCntAttack() >= ATTACK_FREQ)
			{// 一定時間ごとに攻撃
				CMissile::Create(GetPosition());

				SetCntAttack(0);
			}
		}
		else
		{
			SetMove(D3DXVECTOR3
			(
				sinf(GetRot().y) * SPEED_MOVE,
				GetMove().y,
				cosf(GetRot().y) * SPEED_MOVE
			));
		}

	}

}

//=====================================================
// 墜落処理
//=====================================================
void CEnemyMove::Fall(void)
{
	// 汎用処理取得
	CUniversal *pUniversal = CManager::GetUniversal();
	D3DXVECTOR3 move,rot;

	// 移動方向取得
	m_fAngleFall = atan2f(GetMove().x, GetMove().z) + m_fAngleFall;

	move =
	{
		sinf(m_fAngleFall) * FALL_SPEED,
		-0.2f,
		cosf(m_fAngleFall) * FALL_SPEED
	};

	SetMove(GetMove() + move);

	// 回転させる
	rot = GetRot();

	rot.y += m_fRotateFall;

	// 値の補正
	pUniversal->LimitRot(&rot.y);

	SetRot(rot);

	// メッシュフィールドとの当たり判定
	CMeshField *pMesh = CGame::GetMeshField();

	CParticle::Create(GetPosition(), CParticle::TYPE_SMOKE);

	if (pMesh != nullptr)
	{
		float  fHeight = CGame::GetMeshField()->GetHeight(GetPosition(), &move);

		if (fHeight >= GetPosition().y)
		{// メッシュフィールドに当たったら爆発
			CDebrisSpawner::Create(GetPosition(), 15.0f, 1, 3);

			CParticle::Create(GetPosition(), CParticle::TYPE_SAND);

			Death();
		}
	}
}

//=====================================================
// 描画処理
//=====================================================
void CEnemyMove::Draw(void)
{
	// 継承クラスの描画
	CEnemy::Draw();
}

//=====================================================
// コンストラクタ
//=====================================================
CEnemyAttack::CEnemyAttack()
{
	m_state = STATE_NONE;
	m_nTimerLeave = 0;
}

//=====================================================
// デストラクタ
//=====================================================
CEnemyAttack::~CEnemyAttack()
{

}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CEnemyAttack::Init(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// 継承クラスの初期化
	CEnemy::Init();

	// 少し上に上がる
	SetPosition(D3DXVECTOR3(GetPosition().x, GetPosition().y + HEIGHT_FIGHTER, GetPosition().z));

	// 追跡状態に設定
	m_state = STATE_CHASE;

	// 初期の体力設定
	SetLife(LIFE_FIGHTER);

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CEnemyAttack::Uninit(void)
{
	// 継承クラスの終了
	CEnemy::Uninit();
}

//=====================================================
// 更新処理
//=====================================================
void CEnemyAttack::Update(void)
{
	// 位置を保存
	SetPositionOld(GetPosition());

	// 追跡処理
	CEnemyAttack::ChasePlayer();

	// 移動量減衰
	SetMove(GetMove() *= 0.99f);

	// 継承クラスの更新
	CEnemy::Update();
}

//=====================================================
// 傾きの制御処理
//=====================================================
void CEnemyAttack::Tilt(float fRotDiff)
{
	// 汎用処理取得
	CUniversal *pUniversal = CManager::GetUniversal();

	// 変数宣言
	float fTilt;
	float fRot = GetRot().y;

	// 割合を算出
	fTilt = fRotDiff / ROT_DIFF;

	// 向きの設定
	fRot = fTilt * ROT_DIFF;

	// 値の補正
	pUniversal->LimitRot(&fRot);

	if (LIMIT_TILT < fRot)
	{
		fRot = LIMIT_TILT;
	}
	else if (-LIMIT_TILT > fRot)
	{
		fRot = -LIMIT_TILT;
	}

	if (m_state == STATE_LEAVE)
	{
		// 本体を回す
		pUniversal->FactingRot(&fRot, 0.0f, 0.10f);
	}
	else
	{
		// 本体を回す
		pUniversal->FactingRot(&fRot, fTilt, 0.10f);
	}

	SetRot(D3DXVECTOR3(GetRot().x, GetRot().y, fRot));
}

//=====================================================
// 状態管理
//=====================================================
void CEnemyAttack::ManageState(float fLength,float fRotDiff)
{
	switch (m_state)
	{
	case STATE_CHASE:
		// 追跡状態
		if (fLength > 7000.0f)
		{// ターンする
			m_state = STATE_TURN;
		}

		if (fLength < 2000.0f)
		{
			m_state = STATE_LEAVE;
		}

		break;
	case STATE_TURN:
		// ターン状態
		if (fRotDiff * fRotDiff < TURN_ROT * TURN_ROT)
		{// 追跡にうつる
			m_state = STATE_CHASE;
		}
		else
		{
			if (fLength < 2000.0f)
			{
				m_state = STATE_LEAVE;
			}
		}

		break;
	case STATE_LEAVE:
		// 離脱状態
		m_nTimerLeave++;

		if (m_nTimerLeave > 300)
		{
			m_state = STATE_TURN;
			m_nTimerLeave = 0;
		}

		break;
	default:
		break;
	}
}

//=====================================================
// 目標追跡
//=====================================================
void CEnemyAttack::ChasePlayer(void)
{
	// プレイヤー情報取得
	CPlayer *pPlayer = CGame::GetPlayer();

	if (pPlayer != nullptr)
	{
		// 変数宣言
		D3DXVECTOR3 vecDest;

		// 差分を取得
		vecDest = pPlayer->GetPosition() - GetPosition();

		float length = D3DXVec3Length(&vecDest);

		// 目標角度を取得
		float fRotDest = atan2f(vecDest.x, vecDest.z) + D3DX_PI;

		float fRotDiff = fRotDest - GetRot().y;

		// 角度の修正
		if (fRotDiff > D3DX_PI)
		{
			fRotDiff -= 6.28f;
		}
		if (fRotDiff < -D3DX_PI)
		{
			fRotDiff += 6.28f;
		}

		// 傾く処理
		Tilt(fRotDiff);

		// 状態管理
		ManageState(length, fRotDiff);

		if (m_state != STATE_LEAVE)
		{
			if (m_state == STATE_TURN)
			{
				// 角度補正
				SetRot(D3DXVECTOR3(GetRot().x, GetRot().y + fRotDiff * 0.02f, GetRot().z));
			}
			else
			{
				// 角度補正
				SetRot(D3DXVECTOR3(GetRot().x, GetRot().y + fRotDiff * 0.001f, GetRot().z));
			}
		}

		// 角度の修正
		if (GetRot().y > D3DX_PI)
		{
			// 角度補正
			SetRot(D3DXVECTOR3(GetRot().x, GetRot().y - 6.28f, GetRot().z));
		}
		if (GetRot().y < -D3DX_PI)
		{
			// 角度補正
			SetRot(D3DXVECTOR3(GetRot().x, GetRot().y + 6.28f, GetRot().z));
		}

		SetMove(D3DXVECTOR3
		(
			-sinf(GetRot().y) * SPEED_FIGHTER,
			GetMove().y,
			-cosf(GetRot().y) * SPEED_FIGHTER
		));

		if (length > ATTACK_RANGE)
		{// 目標から離れていたら
		 // 目標方向へ進む
			
		}
		else
		{// 目標に近づいていたら
			if (GetCntAttack() >= ATTACK_FREQ)
			{// 一定時間ごとに攻撃
				D3DXVec3Normalize(&vecDest, &vecDest);

				CBullet::Create
				(// 弾の発射
					D3DXVECTOR3(GetPosition().x, GetPosition().y, GetPosition().z),
					vecDest * BULLET_SPEED,
					300,
					CBullet::TYPE_ENEMY,
					false,
					20.0f
				);

				SetCntAttack(0);
			}
		}
	}
}

//=====================================================
// 描画処理
//=====================================================
void CEnemyAttack::Draw(void)
{
	// 継承クラスの描画
	CEnemy::Draw();
}