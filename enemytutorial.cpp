//*****************************************************
//
// チュートリアル敵の処理[enemytutorial.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "main.h"
#include "enemytutorial.h"
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
#include "smokespawner.h"
#include "missile.h"
#include "objectmanager.h"

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
CEnemyTutorial::CEnemyTutorial()
{
	m_fAngleFall = 0.0f;
	m_fRotateFall = 0.0f;
	m_bFall = false;
}

//=====================================================
// デストラクタ
//=====================================================
CEnemyTutorial::~CEnemyTutorial()
{

}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CEnemyTutorial::Init(void)
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
void CEnemyTutorial::Uninit(void)
{
	// 継承クラスの終了
	CEnemy::Uninit();
}

//=====================================================
// 更新処理
//=====================================================
void CEnemyTutorial::Update(void)
{
	// 位置を保存
	SetPositionOld(GetPosition());

	if (GetState() != STATE_DEATH)
	{

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

	// 継承クラスの更新
	CEnemy::Update();
}

//=====================================================
// 墜落処理
//=====================================================
void CEnemyTutorial::Fall(void)
{
	// 汎用処理取得
	CUniversal *pUniversal = CManager::GetUniversal();
	D3DXVECTOR3 move, rot;

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
	CMeshField *pMesh = nullptr;
	CObjectManager *pObjManager = CManager::GetObjectManager();

	if (pObjManager != nullptr)
	{// メッシュフィールドの取得
		pMesh = pObjManager->GetMeshField();
	}

	CParticle::Create(GetPosition(), CParticle::TYPE_SMOKE);

	if (pMesh != nullptr)
	{
		float  fHeight = pMesh->GetHeight(GetPosition(), &move);

		if (fHeight >= GetPosition().y)
		{// メッシュフィールドに当たったら爆発
			CSmokeSpawner::Create(GetPosition(), 15.0f, 1, 3);

			CParticle::Create(GetPosition(), CParticle::TYPE_SAND);

			Death();
		}
	}
}

//=====================================================
// 描画処理
//=====================================================
void CEnemyTutorial::Draw(void)
{
	// 継承クラスの描画
	CEnemy::Draw();
}