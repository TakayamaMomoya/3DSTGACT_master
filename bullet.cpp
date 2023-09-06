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
CBullet::CBullet(int nPriority) : CBillboard(nPriority)
{
	m_nLife = 0;
	m_nNumHit = 0;
	m_fDamage = 0.0f;
	m_type = TYPE_NONE;
	m_bPierce = false;
	m_bLock = false;
	m_move = { 0.0f,0.0f,0.0f };
	m_rot = { 0.0f,0.0f,0.0f };
	m_col = { 0.0f,0.0f,0.0f,0.0f };
	m_pCollisionSphere = nullptr;
	m_apOrbit[0] = nullptr;
	m_apOrbit[1] = nullptr;

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

	// 継承クラスの初期化
	CBillboard::Init();

	// タイプの設定
	SetType(TYPE_BULLET);

	Draw();

	if (m_apOrbit[0] == nullptr)
	{// 軌跡の生成
		m_apOrbit[0] = COrbit::Create(GetMatrix(), D3DXVECTOR3(GetWidth(), 0.0f, 0.0f), D3DXVECTOR3(-GetWidth(), 0.0f, 0.0f), m_col, EDGE_ORBIT);
	}

	if (m_apOrbit[1] == nullptr)
	{// 軌跡の生成
		m_apOrbit[1] = COrbit::Create(GetMatrix(), D3DXVECTOR3(0.0f, GetWidth(), 0.0f), D3DXVECTOR3(0.0f, -GetWidth(), 0.0f), m_col, EDGE_ORBIT);
	}

	if (m_apOrbit[2] == nullptr)
	{// 軌跡の生成
		m_apOrbit[2] = COrbit::Create(GetMatrix(), D3DXVECTOR3(GetWidth(), GetWidth(), 0.0f), D3DXVECTOR3(-GetWidth(), -GetWidth(), 0.0f), m_col, EDGE_ORBIT);
	}

	if (m_apOrbit[3] == nullptr)
	{// 軌跡の生成
		m_apOrbit[3] = COrbit::Create(GetMatrix(), D3DXVECTOR3(GetWidth(), -GetWidth(), 0.0f), D3DXVECTOR3(-GetWidth(), GetWidth(), 0.0f), m_col, EDGE_ORBIT);
	}

	D3DXMATRIX mtx;

	// マトリックス初期化
	D3DXMatrixIdentity(&mtx);

	// マトリックスをかけ合わせる
	pUniversal->SetOffSet(&mtx, GetMatrix(), D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_rot);

	for (int nCnt = 0; nCnt < 6; nCnt++)
	{
		for (int nCntOrbit = 0; nCntOrbit < NUM_ORBIT; nCntOrbit++)
		{// 軌跡の更新
			if (m_apOrbit[nCntOrbit] != nullptr)
			{
				m_apOrbit[nCntOrbit]->SetPositionOffset(mtx, 0);
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

	// 継承クラスの終了
	CBillboard::Uninit();
}

//=====================================================
// ヒット数管理
//=====================================================
void CBullet::ManageHit(void)
{
	// プレイヤー取得
	CPlayer *pPlayer = CGame::GetPlayer();

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

	// 継承クラスの更新
	CBillboard::Update();

	// 寿命減衰
	m_nLife--;

	// 位置の更新
	SetPosition(GetPosition() + m_move);

	if (m_pCollisionSphere != nullptr)
	{// 当たり判定の管理
		// タグ
		CCollision::TAG tag = CCollision::TAG_PLAYERBULLET;

		// 当たり判定の位置設定
		m_pCollisionSphere->SetPositionOld(m_pCollisionSphere->GetPosition());
		m_pCollisionSphere->SetPosition(GetPosition());

		switch (m_type)
		{
		case TYPE_PLAYER:
			tag = CCollision::TAG_ENEMY;

			if (m_pCollisionSphere->SphereCollision(CCollision::TAG_MISSILE))
			{// 対象との当たり判定
				CObject *pObj = m_pCollisionSphere->GetOther();

				bHit = true;

				// ヒット数加算
				m_nNumHit++;

				if (pObj != nullptr)
				{
					// 当たったオブジェクトのヒット処理
					pObj->Hit(m_fDamage);

					CParticle::Create(GetPosition(), CParticle::TYPE_HIT);

					CBonus::Create(CBonus::TYPE_MISSILE);
				}
			}
			break;
		case TYPE_ENEMY:
			tag = CCollision::TAG_PLAYER;
			break;
		default:
			break;
		}

		if (m_pCollisionSphere->SphereCollision(tag))
		{// 対象との当たり判定
			CObject *pObj = m_pCollisionSphere->GetOther();

			bHit = true;

			// ヒット数加算
			m_nNumHit++;

			if (pObj != nullptr)
			{
				// 当たったオブジェクトのヒット処理
				pObj->Hit(m_fDamage);

				CParticle::Create(GetPosition(), CParticle::TYPE_HIT);
			}
		}

		if (m_pCollisionSphere->TriggerCube(CCollision::TAG_BLOCK))
		{// ブロックとの当たり判定
			// 爆発アニメーション
			CExplosion::Create(GetPosition());

			// 爆発パーティクル
			CParticle::Create(GetPosition(), CParticle::TYPE_HIT);

			// 終了処理
			Uninit();
		}
	}

	// 軌跡のロール回転
	m_rot.z += ROLL_SPEED;

	// 値の補正
	pUniversal->LimitRot(&m_rot.z);

	if (bHit == false)
	{
		CMeshField *pMesh = CGame::GetMeshField();
		float fHeight = 0.0f;

		if (pMesh != nullptr)
		{
			// メッシュフィールドとの当たり判定
			fHeight = pMesh->GetHeight(GetPosition(), nullptr);
		}

		if (m_nLife < 0 || fHeight > GetPosition().y)
		{// 自分の削除
			CExplosion::Create(GetPosition());

			CParticle::Create(GetPosition(), CParticle::TYPE_HIT);

			Uninit();
		}
	}
	else
	{
		if (m_bPierce == false)
		{// 貫通する弾は消えない
			CExplosion::Create(GetPosition());

			CParticle::Create(GetPosition(), CParticle::TYPE_HIT);

			// 終了処理
			Uninit();
		}
	}

	D3DXMATRIX mtx;

	// マトリックス初期化
	D3DXMatrixIdentity(&mtx);

	// マトリックスをかけ合わせる
	pUniversal->SetOffSet(&mtx, GetMatrix(), D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_rot);

	for (int nCntOrbit = 0; nCntOrbit < NUM_ORBIT; nCntOrbit++)
	{// 軌跡の更新
		if (m_apOrbit[nCntOrbit] != nullptr)
		{
			m_apOrbit[nCntOrbit]->SetPositionOffset(mtx, 0);
		}
	}
}

//=====================================================
// 画面外削除処理
//=====================================================
void CBullet::LimitPos(void)
{
	// 位置の入手
	D3DXVECTOR3 pos = GetPosition();

	if (pos.x < 0 || pos.x > SCREEN_WIDTH ||
		pos.y < 0 || pos.y > SCREEN_HEIGHT)
	{
		CExplosion::Create(GetPosition());

		Uninit();
	}
}

//=====================================================
// 描画処理
//=====================================================
void CBullet::Draw(void)
{
	// 汎用処理取得
	CUniversal *pUniversal = CManager::GetUniversal();

	// ビルボードの描画
	CBillboard::SetMatrix();
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
		pBullet->SetPosition(pos);
		pBullet->SetSize(fRadius, fRadius);
		pBullet->m_nLife = nLife;
		pBullet->m_type = type;
		pBullet->m_bPierce = bPierce;
		pBullet->m_fDamage = fDamage;
		pBullet->m_col = col;

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

			pBullet->m_pCollisionSphere->SetPosition(pBullet->GetPosition());

			pBullet->m_pCollisionSphere->SetRadius(fRadius * 1.5f);
		}

		// 初期化処理
		pBullet->Init();

		// テクスチャの読込
		int nIdx = CManager::GetTexture()->Regist("data\\TEXTURE\\EFFECT\\bullet000.png");
		pBullet->SetIdxTexture(nIdx);
	}

	return pBullet;
}

//=====================================================
// 読込処理
//=====================================================
HRESULT CBullet::Load(void)
{
	return S_OK;
}

//=====================================================
// テクスチャ破棄
//=====================================================
void CBullet::Unload(void)
{

}