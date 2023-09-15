//*****************************************************
//
// 煙スポナー処理[smokespawner.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "smokespawner.h"
#include "smoke.h"

//=====================================================
// 優先順位を決めるコンストラクタ
//=====================================================
CSmokeSpawner::CSmokeSpawner()
{
	m_pos = { 0.0f,0.0f,0.0f };
	m_fSpeed = 0.0f;
	m_nLife = 0;
	m_nNumDebris = 0;
}

//=====================================================
// デストラクタ
//=====================================================
CSmokeSpawner::~CSmokeSpawner()
{

}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CSmokeSpawner::Init(void)
{
	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CSmokeSpawner::Uninit(void)
{
	Release();
}

//=====================================================
// 更新処理
//=====================================================
void CSmokeSpawner::Update(void)
{
	D3DXVECTOR3 move;
	float fRot,fSpeed;
	int nLife;
	CSmoke *pDebris;

	for (int nCnt = 0; nCnt < m_nNumDebris; nCnt++)
	{
		nLife = rand() % 50 + 10;
		fRot = rand() % 628 - 314 / 100.0f;
		fSpeed = (float)(rand() % (int)(m_fSpeed * 50)) * 0.01f + m_fSpeed * 0.5f;

		move.x = sinf(fRot) * fSpeed;
		move.y = fSpeed;
		move.z = cosf(fRot) * fSpeed;

		pDebris = CSmoke::Create(m_pos, nLife);

		pDebris->SetMove(move);
	}

	m_nLife--;

	if (m_nLife < 0)
	{
		// 自身の破棄
		Uninit();
	}
}

//=====================================================
// 位置設定処理
//=====================================================
void CSmokeSpawner::SetPosition(D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//=====================================================
// 生成処理
//=====================================================
CSmokeSpawner *CSmokeSpawner::Create(D3DXVECTOR3 pos,float fSpeed,int nLife, int nNumDebris)
{
	CSmokeSpawner *pSpawner = nullptr;

	if (pSpawner == nullptr)
	{// インスタンス生成
		pSpawner = new CSmokeSpawner;

		pSpawner->m_pos = pos;

		pSpawner->m_nLife = nLife;

		pSpawner->m_fSpeed = fSpeed;

		pSpawner->m_nNumDebris = nNumDebris;
	}

	return pSpawner;
}