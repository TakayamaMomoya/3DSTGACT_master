//*****************************************************
//
// ミサイルの処理[enemymissile.cpp]
// Author:��山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "missile.h"
#include "player.h"
#include "game.h"
#include "manager.h"
#include "universal.h"
#include "debugproc.h"
#include "meshfield.h"
#include "particle.h"
#include "collision.h"
#include "orbit.h"
#include "explosion.h"

//*****************************************************
// マクロ定義
//*****************************************************
#define MAX_SPEED	(40.0f)	// 移動速度
#define CHASE_SPEED	(0.7f)	// 追跡速度
#define INITIAL_LIFE	(1)	// 初期体力
#define DEATH_TIME	(300)	// 自滅までの時間
#define SIZE_ORBIT	(50.0f)	// 軌跡のサイズ
#define LENGTH_ORBIT	(50)	// 軌跡の長さ

//=====================================================
// コンストラクタ
//=====================================================
CMissile::CMissile(int nPriority) : CObjectX(nPriority)
{
	m_nDeathTimer = 0;
	m_pCollisionSphere = nullptr;
	m_pOrbit = nullptr;
}

//=====================================================
// デストラクタ
//=====================================================
CMissile::~CMissile()
{

}

//=====================================================
// 生成処理
//=====================================================
CMissile *CMissile::Create(D3DXVECTOR3 pos)
{
	CMissile *pMissile = nullptr;

	pMissile = new CMissile;

	if (pMissile != nullptr)
	{
		pMissile->SetPosition(pos);

		// 初期化処理
		pMissile->Init();
	}

	return pMissile;
}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CMissile::Init(void)
{
	// 継承クラスの初期化
	CObjectX::Init();

	// モデル番号の設定
	int nIdx = CModel::Load("data\\MODEL\\CHARACTER\\missile.x");
	SetIdxModel(nIdx);

	// モデル情報の設定
	BindModel(nIdx);

	if (m_pCollisionSphere == nullptr)
	{// 球の当たり判定生成
		m_pCollisionSphere = CCollisionSphere::Create(CCollision::TAG_MISSILE, CCollision::TYPE_SPHERE, this);

		if (m_pCollisionSphere != nullptr)
		{// 情報の設定
			m_pCollisionSphere->SetPosition(GetPosition());
			m_pCollisionSphere->SetRadius(GetRadius());
		}
	}

	CObjectX::Draw();

	if (m_pOrbit == nullptr)
	{// 軌跡の生成
		m_pOrbit = COrbit::Create(*GetMatrix(), D3DXVECTOR3(0.0f, 0.0f, SIZE_ORBIT), D3DXVECTOR3(0.0f, 0.0f, -SIZE_ORBIT), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), LENGTH_ORBIT);
	}

	return S_OK;
}

//=====================================================
// 死亡処理
//=====================================================
void CMissile::Death(void)
{
	// 自分の爆発
	CParticle::Create(GetPosition(), CParticle::TYPE_EXPLOSION);
	CExplosion::Create(GetPosition())->SetSize(300.0f, 300.0f);

	if (m_pOrbit != nullptr)
	{// 軌跡の切り離し
		m_pOrbit->SetEnd(true);
	}

	// 自身の終了
	Uninit();
}

//=====================================================
// 終了処理
//=====================================================
void CMissile::Uninit(void)
{
	if (m_pCollisionSphere != nullptr)
	{// 当たり判定の消去
		m_pCollisionSphere->Uninit();

		m_pCollisionSphere = nullptr;
	}

	if (m_pOrbit != nullptr)
	{// 軌跡の終了
		m_pOrbit = nullptr;
	}

	// 継承クラスの終了
	CObjectX::Uninit();
}

//=====================================================
// 更新処理
//=====================================================
void CMissile::Update(void)
{
	// 汎用処理取得
	CUniversal *pUniversal = CManager::GetUniversal();

	D3DXVECTOR3 vecMove;
	D3DXMATRIX mtx;

	// 位置保存
	SetPositionOld(GetPosition());

	// 追跡処理
	CMissile::ChasePlayer();

	// 速度制限
	vecMove = GetMove();

	if (D3DXVec3Length(&vecMove) > MAX_SPEED)
	{
		D3DXVec3Normalize(&vecMove, &vecMove);

		vecMove *= MAX_SPEED;

		SetMove(vecMove);
	}

	// メッシュフィールドとの判定
	float fHeight;
	CMeshField *pMesh = nullptr;

	pMesh = CGame::GetMeshField();

	if (pMesh != nullptr)
	{
		fHeight = CGame::GetMeshField()->GetHeight(GetPosition(), nullptr);

		if (fHeight > GetPosition().y)
		{
			Death();
		}
	}

	SetPosition(GetPosition() + GetMove());

	if (m_pCollisionSphere != nullptr)
	{// 球の当たり判定の管理
		m_pCollisionSphere->SetPosition(GetPosition());
	}

	// 目標に近ければ爆発
	if (m_pCollisionSphere != nullptr)
	{
		if (m_pCollisionSphere->SphereCollision(CCollision::TAG_PLAYER))
		{
			CObject *pObj = m_pCollisionSphere->GetOther();

			if (pObj != nullptr)
			{
				// 当たったオブジェクトのヒット処理
				pObj->Hit(10.0f);

				Death();
			}
		}
	}

	// マトリックス初期化
	D3DXMatrixIdentity(&mtx);

	// マトリックスをかけ合わせる
	pUniversal->SetOffSet(&mtx, *GetMatrix(), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	if (m_pOrbit != nullptr)
	{// 軌跡の更新
		m_pOrbit->SetPositionOffset(mtx, 0);
	}

	m_nDeathTimer++;

	if (m_nDeathTimer > DEATH_TIME)
	{
		Death();
	}

	// 傾きの制御
	Tilt();

	// 継承クラスの更新
	CObjectX::Update();
}

//=====================================================
// 追跡処理
//=====================================================
void CMissile::ChasePlayer(void)
{
	// 情報取得
	CPlayer *pPlayer = CGame::GetPlayer();

	// 計算用変数
	D3DXVECTOR3 pos;
	D3DXVECTOR3 vecDiff;

	if (pPlayer == nullptr)
	{// 死亡
		Uninit();

		return;
	}

	pos = pPlayer->GetPosition();

	pos.y += 100.0f;

	// 差分を取得
	vecDiff = pos - GetPosition();

	D3DXVec3Normalize(&vecDiff,&vecDiff);

	vecDiff *= CHASE_SPEED;

	// 移動量設定
	SetMove(GetMove() + vecDiff);
}

//=====================================================
// 傾きの管理
//=====================================================
void CMissile::Tilt(void)
{
	D3DXVECTOR3 rot = { 0.0f,0.0f,0.0f };
	D3DXVECTOR3 move = GetMove();
	float fLength;

	// Y軸平面の角度を取得
	rot.y = atan2f(move.x, move.z);

	// Y軸平面の移動量を取得
	fLength = D3DXVec2Length(&D3DXVECTOR2(move.x,move.z));

	// 横平面の角度を取得
	rot.x = atan2f(fLength,move.y);

	// 回転の設定
	SetRot(rot);
}

//=====================================================
// 描画処理
//=====================================================
void CMissile::Draw(void)
{
	// 継承クラスの描画
	CObjectX::Draw();
}