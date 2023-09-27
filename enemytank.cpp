//*****************************************************
//
// 戦車敵の処理[enemytank.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "main.h"
#include "enemytank.h"
#include "renderer.h"
#include "manager.h"
#include "player.h"
#include "bullet.h"
#include "defend.h"
#include "universal.h"
#include "debugproc.h"
#include "game.h"
#include "objectmanager.h"
#include "sound.h"

//*****************************************************
// マクロ定義
//*****************************************************
#define SPEED_MOVE	(5.5f)	// 移動速度
#define ATTACK_FREQ	(180)	// 攻撃する頻度
#define BULLET_SPEED	(90.0f)	// 弾のスピード
#define INITIAL_HEIGHT	(300.0f)	// 初期の高さ
#define ATTACK_RANGE	(4000.0f)	// 攻撃する距離
#define DEFAULT_SPEED	(4.0f)	// 基準のスピード
#define DEFAULT_TILT	(-D3DX_PI * 0.1f)	// 基準の傾き
#define TANK_HEIGHT	(150.0f)	// 高さ
#define LIFE_TANK	(25)	// 初期体力
#define INITIAL_SCORE	(4000)	// 初期スコア

//=====================================================
// コンストラクタ
//=====================================================
CEnemyTank::CEnemyTank()
{
	
}

//=====================================================
// デストラクタ
//=====================================================
CEnemyTank::~CEnemyTank()
{

}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CEnemyTank::Init(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// 継承クラスの初期化
	CEnemy::Init();

	// 少し上に上がる
	SetPosition(D3DXVECTOR3(GetPosition().x, GetPosition().y + INITIAL_HEIGHT, GetPosition().z));

	// 体力設定
	SetLife(LIFE_TANK);

	// スコア設定
	SetScore(INITIAL_SCORE);

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CEnemyTank::Uninit(void)
{
	// 継承クラスの終了
	CEnemy::Uninit();
}

//=====================================================
// 更新処理
//=====================================================
void CEnemyTank::Update(void)
{
	if (GetState() == STATE_DEATH)
	{
		if (GetMotion() != 2)
		{
			SetMotion(2);
		}
	}

	// 位置を保存
	SetPositionOld(GetPosition());

	// 追跡処理
	CEnemyTank::ChaseDefend();

	// 移動量減衰
	SetMove(D3DXVECTOR3(GetMove().x * 0.98f, GetMove().y - 0.98f, GetMove().z * 0.98f));

	// 継承クラスの更新
	CEnemy::Update();
}

//=====================================================
// 護衛目標追跡
//=====================================================
void CEnemyTank::ChaseDefend(void)
{
	if (GetState() == STATE_DEATH)
	{
		return;
	}

	// プレイヤー情報取得
	CPlayer *pPlayer = nullptr;
	CObjectManager *pObjManager = CManager::GetObjectManager();

	if (pObjManager != nullptr)
	{// プレイヤーの適用
		pPlayer = pObjManager->GetPlayer();
	}

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

	// 角度補正
	SetRot(D3DXVECTOR3(GetRot().x, GetRot().y + fRotDiff * 0.1f, GetRot().z));

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
			-sinf(GetRot().y) * SPEED_MOVE,
			GetMove().y,
			-cosf(GetRot().y) * SPEED_MOVE
		));
	}
	else
	{// 目標に近づいていたら

		if (GetMoveState() == MOVESTATE_CHASE)
		{
			if (GetCntAttack() >= ATTACK_FREQ)
			{// 一定時間ごとに攻撃
				D3DXVec3Normalize(&vecDest, &vecDest);

				CBullet::Create
				(// 弾の発射
					D3DXVECTOR3(GetPosition().x, GetPosition().y + TANK_HEIGHT, GetPosition().z),
					vecDest * BULLET_SPEED,
					300,
					CBullet::TYPE_ENEMY,
					false,
					20.0f
				);

				SetCntAttack(0);

				// SE再生
				CManager::GetSound()->Play(CSound::LABEL_BIGSHOT);
			}
		}
		else
		{
			SetMove(D3DXVECTOR3
			(
				-sinf(GetRot().y) * SPEED_MOVE,
				GetMove().y,
				-cosf(GetRot().y) * SPEED_MOVE
			));
		}
	}
}

//=====================================================
// 描画処理
//=====================================================
void CEnemyTank::Draw(void)
{
	// 継承クラスの描画
	CEnemy::Draw();
}