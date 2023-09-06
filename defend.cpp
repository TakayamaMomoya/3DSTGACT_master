//*****************************************************
//
// 護衛目標の処理[defend.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "defend.h"
#include "manager.h"
#include "collision.h"
#include "objectX.h"
#include "particle.h"
#include "explosion.h"
#include "explosionspawner.h"
#include "game.h"

//*****************************************************
// マクロ定義
//*****************************************************
#define MAX_LIFE	(600)	// 体力
#define TIME_DEATH	(200)	// 死亡するまでのフレーム数

//=====================================================
// コンストラクタ
//=====================================================
CDefend::CDefend()
{
	m_pObjectX = nullptr;
	m_pCollisionCube = nullptr;
	m_pCollisionSphere = nullptr;
	m_nLife = 0;
	m_state = STATE_NONE;
}

//=====================================================
// デストラクタ
//=====================================================
CDefend::~CDefend()
{

}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CDefend::Init(void)
{
	// 値の初期設定
	m_nLife = MAX_LIFE;

	if (m_pObjectX == nullptr)
	{
		m_pObjectX = CObjectX::Create();

		if (m_pObjectX != nullptr)
		{
			// モデル番号の設定
			int nIdx = CModel::Load("data\\MODEL\\BLOCK\\bill00.x");
			m_pObjectX->SetIdxModel(nIdx);

			// モデル情報の設定
			m_pObjectX->BindModel(nIdx);

			if (m_pCollisionCube == nullptr)
			{// 当たり判定生成
				m_pCollisionCube = CCollisionCube::Create(CCollision::TAG_BLOCK, this);

				D3DXVECTOR3 pos = GetPosition();

				if (m_pCollisionCube != nullptr)
				{
					m_pCollisionCube->SetPosition(GetPosition());

					m_pCollisionCube->SetVtx(m_pObjectX->GetVtxMax(), m_pObjectX->GetVtxMin());
				}
			}

			if (m_pCollisionSphere == nullptr)
			{// 球の当たり判定生成
				m_pCollisionSphere = CCollisionSphere::Create(CCollision::TAG_DEFEND, CCollision::TYPE_SPHERE, this);

				if (m_pCollisionSphere != nullptr)
				{
					m_pCollisionSphere->SetRadius(m_pObjectX->GetRadius());
				}
			}
		}
	}

	m_state = STATE_NONE;

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CDefend::Uninit(void)
{
	CGame::ReleaseDefend();

	if (m_pObjectX != nullptr)
	{// モデルの破棄
		m_pObjectX->Uninit();
		m_pObjectX = nullptr;
	}

	if (m_pCollisionCube != nullptr)
	{// 立方体の当たり判定の破棄
		m_pCollisionCube->Uninit();
		m_pCollisionCube = nullptr;
	}

	if (m_pCollisionSphere != nullptr)
	{// 球の当たり判定の破棄
		m_pCollisionSphere->Uninit();
		m_pCollisionSphere = nullptr;
	}

	Release();
}

//=====================================================
// 更新処理
//=====================================================
void CDefend::Update(void)
{
	// 状態管理
	ManageState();
}

//=====================================================
// 状態管理
//=====================================================
void CDefend::ManageState(void)
{
	switch (m_state)
	{
	case CDefend::STATE_NORMAL:
		break;
	case CDefend::STATE_DEATH:

		m_nTimerDeath++;

		if (m_nTimerDeath >= TIME_DEATH)
		{// 自身の破棄
			Uninit();
		}

		break;
	default:
		break;
	}
}

//=====================================================
// ヒット処理
//=====================================================
void CDefend::Hit(float fDamage)
{
	if (m_state == STATE_DEATH)
	{
		return;
	}

	// 体力減少
	SetLife(GetLife() - (int)fDamage);

    if (GetLife() == 0)
	{
		// 自分の爆発
		CParticle::Create(GetPosition(), CParticle::TYPE_EXPLOSION);
		CExplosion::Create(GetPosition())->SetSize(220.0f, 220.0f);

		CExplSpawner::Create(GetPosition(),m_pObjectX->GetRadius(), TIME_DEATH);

		m_state = STATE_DEATH;
	}
}

//=====================================================
// 描画処理
//=====================================================
void CDefend::Draw(void)
{
	
}

//=====================================================
// 位置取得処理
//=====================================================
D3DXVECTOR3 CDefend::GetPosition(void)
{
	if (m_pObjectX != nullptr)
	{
		return m_pObjectX->GetPosition();
	}
	else
	{
		return D3DXVECTOR3();
	}
}

//=====================================================
// 生成処理
//=====================================================
CDefend *CDefend::Create(void)
{
	CDefend *pDefend = nullptr;

	if (pDefend == nullptr)
	{// インスタンス生成
		pDefend = new CDefend;

		// 初期化処理
		pDefend->Init();
	}

	return pDefend;
}

//=====================================================
// ライフ設定
//=====================================================
void CDefend::SetLife(int nLife)
{
	m_nLife = nLife;

	if (m_nLife < 0)
	{// ０より下にいかないように補正
		m_nLife = 0;
	}
}