//*****************************************************
//
// パーティクル処理[particle.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "explosionspawner.h"
#include "explosion.h"

//=====================================================
// 優先順位を決めるコンストラクタ
//=====================================================
CExplSpawner::CExplSpawner()
{
	m_pos = { 0.0f,0.0f,0.0f };
	m_fRadius = 0.0f;
	m_nLife = 0;
	m_nCntSpawn = 0;
}

//=====================================================
//	デストラクタ
//=====================================================
CExplSpawner::~CExplSpawner()
{

}

//=====================================================
//	初期化処理
//=====================================================
HRESULT CExplSpawner::Init(void)
{
	return S_OK;
}

//=====================================================
//	終了処理
//=====================================================
void CExplSpawner::Uninit(void)
{
	Release();
}

//=====================================================
//	更新処理
//=====================================================
void CExplSpawner::Update(void)
{
	D3DXVECTOR3 pos;
	float fSize;

	m_nCntSpawn++;

	if (m_nCntSpawn >= m_nTimerSpawn)
	{
		CExplosion *pExplosion;

		pos.x = (float)(rand() % (int)(m_fRadius * 100)) * 0.01f - m_fRadius * 0.5f;
		pos.y = (float)(rand() % (int)(m_fRadius * 100)) * 0.01f - m_fRadius * 0.5f;
		pos.z = (float)(rand() % (int)(m_fRadius * 100)) * 0.01f - m_fRadius * 0.5f;

		fSize = (float)(rand() % 200) + 50;

		pExplosion = CExplosion::Create(m_pos + pos);

		pExplosion->SetSize(fSize, fSize);

		// カウンターリセット
		m_nCntSpawn = 0;

		if (m_nLife > 0)
		{
			// タイマー再設定
			m_nTimerSpawn = rand() % m_nLife / 2;
		}
	}

	m_nLife--;

	if (m_nLife < 0)
	{
		// 自身の破棄
		Uninit();
	}
}

//=====================================================
//	位置設定処理
//=====================================================
void CExplSpawner::SetPosition(D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//=====================================================
//	生成処理
//=====================================================
CExplSpawner **CExplSpawner::Create(D3DXVECTOR3 pos,float fRadius,int nLife)
{
	CExplSpawner *pSpawner = nullptr;

	if (pSpawner == nullptr)
	{// インスタンス生成
		pSpawner = new CExplSpawner;

		pSpawner->m_pos = pos;

		pSpawner->m_nLife = nLife;

		pSpawner->m_fRadius = fRadius;
	}

	return &pSpawner;
}