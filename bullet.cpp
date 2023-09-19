//*****************************************************
//
// 弾処理[bullet.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "bullet.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "explosion.h"
#include "effect3D.h"
#include "particle.h"
#include "sound.h"
#include "score.h"
#include "item.h"
#include "enemy.h"
#include "collision.h"
#include "meshfield.h"
#include "orbit.h"
#include "universal.h"
#include "game.h"
#include "player.h"
#include "bonus.h"
#include "debrisspawner.h"
#include "objectmanager.h"

//*****************************************************
// マクロ定義
//*****************************************************
#define SPEED_MOVE	(7.0f)	// 移動速度
#define ROLL_SPEED	(0.1f)	// 回転速度
#define EDGE_ORBIT	(13)	// 軌跡の辺の数

//*****************************************************
// 静的メンバ変数宣言
//*****************************************************
int CBullet::m_nNumAll = 0;	// 総数

//=====================================================
// コンストラクタ
//=====================================================
CBullet::CBullet(int nPriority) : CObject(nPriority)
{
	m_nLife = 0;
	m_nNumHit = 0;
	m_fDamage = 0.0f;
	m_fSize = 0.0f;
	m_type = TYPE_NONE;
	m_bPierce = false;
	m_bLock = false;
	m_move = { 0.0f,0.0f,0.0f };
	m_rot = { 0.0f,0.0f,0.0f };
	m_pos = { 0.0f,0.0f,0.0f };
	m_posOld = { 0.0f,0.0f,0.0f };
	m_col = { 0.0f,0.0f,0.0f,0.0f };
	m_pCollisionSphere = nullptr;
	ZeroMemory(&m_apOrbit[0], sizeof(m_apOrbit));

	// 総数カウントアップ
	m_nNumAll++;
}

//=====================================================
// デストラクタ
//=====================================================
CBullet::~CBullet()
{
	m_nNumAll--;
}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CBullet::Init(void)
{
	// 汎用処理取得
	CUniversal *pUniversal = CManager::GetUniversal();

	m_rot.y = atan2f(m_move.x,m_move.z);

	// タイプの設定
	SetType(TYPE_BULLET);

	Draw();

	if (m_apOrbit[0] == nullptr)
	{// 軌跡の生成
		m_apOrbit[0] = COrbit::Create(m_mtxWorld, D3DXVECTOR3(m_fSize, 0.0f, 0.0f), D3DXVECTOR3(-m_fSize, 0.0f, 0.0f), m_col, EDGE_ORBIT);
	}

	if (m_apOrbit[1] == nullptr)
	{// 軌跡の生成
		m_apOrbit[1] = COrbit::Create(m_mtxWorld, D3DXVECTOR3(0.0f, m_fSize, 0.0f), D3DXVECTOR3(0.0f, -m_fSize, 0.0f), m_col, EDGE_ORBIT);
	}

	if (m_apOrbit[2] == nullptr)
	{// 軌跡の生成
		m_apOrbit[2] = COrbit::Create(m_mtxWorld, D3DXVECTOR3(m_fSize, m_fSize, 0.0f), D3DXVECTOR3(-m_fSize, -m_fSize, 0.0f), m_col, EDGE_ORBIT);
	}

	if (m_apOrbit[3] == nullptr)
	{// 軌跡の生成
		m_apOrbit[3] = COrbit::Create(m_mtxWorld, D3DXVECTOR3(m_fSize, -m_fSize, 0.0f), D3DXVECTOR3(-m_fSize, m_fSize, 0.0f), m_col, EDGE_ORBIT);
	}

	for (int nCnt = 0; nCnt < 6; nCnt++)
	{
		for (int nCntOrbit = 0; nCntOrbit < NUM_ORBIT; nCntOrbit++)
		{// 軌跡の更新
			if (m_apOrbit[nCntOrbit] != nullptr)
			{
				m_apOrbit[nCntOrbit]->SetPositionOffset(m_mtxWorld, 0);
			}
		}
	}

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CBullet::Uninit(void)
{
	// ヒット数管理
	ManageHit();

	if (m_pCollisionSphere != nullptr)
	{// 当たり判定の消去
		m_pCollisionSphere->Uninit();

		m_pCollisionSphere = nullptr;
	}

	for (int nCntOrbit = 0; nCntOrbit < NUM_ORBIT; nCntOrbit++)
	{
		if (m_apOrbit[nCntOrbit] != nullptr)
		{// 軌跡の終了
			m_apOrbit[nCntOrbit]->Uninit();
			m_apOrbit[nCntOrbit] = nullptr;
		}
	}

	Release();
}

//=====================================================
// ヒット数管理
//=====================================================
void CBullet::ManageHit(void)
{
	// プレイヤー取得
	CPlayer *pPlayer = nullptr;
	CObjectManager *pObjManager = CManager::GetObjectManager();

	if (pObjManager != nullptr)
	{// プレイヤーの適用
		pPlayer = pObjManager->GetPlayer();
	}

	switch (m_type)
	{
	case TYPE_PLAYER:

		if (m_nNumHit > 0)
		{
			if (pPlayer != nullptr)
			{// ヒット数加算
				pPlayer->SetNumHit(pPlayer->GetNumHit() + 1);

				if (m_bLock == false)
				{
					CBonus::Create(CBonus::TYPE_MANUALHIT);
				}
			}
		}
		else
		{
			if (pPlayer != nullptr)
			{// ヒット数加算
				pPlayer->SetNumHit(pPlayer->GetNumHit() - 1);
			}
		}

		break;
	case TYPE_ENEMY:

		if (m_nNumHit > 0)
		{
			if (pPlayer != nullptr)
			{// ヒット数加算
				pPlayer->SetNumHit(pPlayer->GetNumHit() + 1);
			}
		}

		break;
	default:
		break;
	}
}

//=====================================================
// 更新処理
//=====================================================
void CBullet::Update(void)
{
	// 汎用処理取得
	CUniversal *pUniversal = CManager::GetUniversal();

	// 変数宣言
	bool bHit = false;

	// 寿命減衰
	m_nLife--;

	m_posOld = m_pos;

	// 位置の更新
	m_pos += m_move;

	if (m_pCollisionSphere != nullptr)
	{// 当たり判定の管理
		// タグ
		CCollision::TAG tag = CCollision::TAG_PLAYERBULLET;

		// 当たり判定の位置設定
		m_pCollisionSphere->SetPositionOld(m_pCollisionSphere->GetPosition());
		m_pCollisionSphere->SetPosition(m_pos);

		switch (m_type)
		{
		case TYPE_PLAYER:
			tag = CCollision::TAG_ENEMY;

			// ミサイルとの当たり判定
			bHit = BulletHit(CCollision::TAG_MISSILE);

			break;
		case TYPE_ENEMY:
			tag = CCollision::TAG_PLAYER;
			break;
		default:
			break;
		}

		// キャラクターとの当たり判定
		bHit = BulletHit(tag);

		if (m_pCollisionSphere->TriggerCube(CCollision::TAG_BLOCK))
		{// ブロックとの当たり判定
			Death();
		}
	}

	// 軌跡のロール回転
	m_rot.z += ROLL_SPEED;

	// 値の補正
	pUniversal->LimitRot(&m_rot.z);

	if (bHit == false)
	{
		// メッシュフィールドとの当たり判定
		CMeshField *pMesh = nullptr;
		CObjectManager *pObjManager = CManager::GetObjectManager();

		if (pObjManager != nullptr)
		{// メッシュフィールドの取得
			pMesh = pObjManager->GetMeshField();
		}

		float fHeight = 0.0f;

		if (pMesh != nullptr)
		{
			// メッシュフィールドとの当たり判定
			fHeight = pMesh->GetHeight(m_pos, nullptr);

			if (fHeight > m_pos.y)
			{
				CParticle::Create(m_pos, CParticle::TYPE_SAND);

				Death();
			}
		}

		if (m_nLife < 0)
		{// 自分の削除
			Death();
		}
	}
	else
	{
		if (m_bPierce == false)
		{// 貫通しない弾は消える
			Death();
		}
	}

	for (int nCntOrbit = 0; nCntOrbit < NUM_ORBIT; nCntOrbit++)
	{// 軌跡の更新
		if (m_apOrbit[nCntOrbit] != nullptr)
		{
			m_apOrbit[nCntOrbit]->SetPositionOffset(m_mtxWorld, 0);
		}
	}
}

//=====================================================
// 命中したか確認する処理
//=====================================================
bool CBullet::BulletHit(CCollision::TAG tag)
{
	bool bHit = false;
	bool bSame = false;

	if (m_pCollisionSphere->IsTriggerEnter(tag))
	{// 対象との当たり判定
		CObject *pObj = m_pCollisionSphere->GetOther();

		if (pObj != nullptr)
		{
			bHit = true;

			// 当たったオブジェクトのヒット処理
			pObj->Hit(m_fDamage);

			CParticle::Create(m_pos, CParticle::TYPE_HIT);

			if (tag == CCollision::TAG_MISSILE)
			{
				CBonus::Create(CBonus::TYPE_MISSILE);
			}

			CDebrisSpawner::Create(GetPosition(), 7.0f, 1, 3);

			// ヒット数加算
			m_nNumHit++;
		}
	}

	return bHit;
}

//=====================================================
// 死亡処理
//=====================================================
void CBullet::Death(void)
{
	// パーティクル生成
	CParticle::Create(m_pos, CParticle::TYPE_HIT);

	// 終了処理
	Uninit();
}

//=====================================================
// 描画処理
//=====================================================
void CBullet::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	D3DXMATRIX mtxRot, mtxTrans;

	//ワールドマトリックス初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot,
		m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans,
		m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	//ワールドマトリックス設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);
}

//=====================================================
// 生成処理
//=====================================================
CBullet *CBullet::Create(D3DXVECTOR3 pos,D3DXVECTOR3 move, int nLife,TYPE type, bool bPierce, float fRadius, float fDamage, D3DXCOLOR col)
{
	CBullet *pBullet = nullptr;

	if (pBullet == nullptr)
	{// インスタンス生成
		pBullet = new CBullet;
		pBullet->m_move = move;
		pBullet->m_pos = pos;
		pBullet->m_posOld = pos;
		pBullet->m_nLife = nLife;
		pBullet->m_type = type;
		pBullet->m_bPierce = bPierce;
		pBullet->m_fDamage = fDamage;
		pBullet->m_col = col;
		pBullet->m_fSize = fRadius;

		if (pBullet->m_pCollisionSphere == nullptr)
		{// 当たり判定生成
			switch (type)
			{// 種類ごとにタグの設定
			case TYPE_ENEMY:
				pBullet->m_pCollisionSphere = CCollisionSphere::Create(CCollision::TAG_ENEMYBULLET, CCollision::TYPE_SPHERE, pBullet);
					break;
			case TYPE_PLAYER:
				pBullet->m_pCollisionSphere = CCollisionSphere::Create(CCollision::TAG_PLAYERBULLET, CCollision::TYPE_SPHERE, pBullet);
				break;
			default:
				break;
			}
		}

		if (pBullet->m_pCollisionSphere != nullptr)
		{
			pBullet->m_pCollisionSphere->SetPosition(pBullet->m_pos);

			pBullet->m_pCollisionSphere->SetRadius(fRadius * 1.5f);
		}

		// 初期化処理
		pBullet->Init();
	}

	return pBullet;
}