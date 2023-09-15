//*****************************************************
//
// ���X�|�i�[����[smokespawner.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "smokespawner.h"
#include "smoke.h"

//=====================================================
// �D�揇�ʂ����߂�R���X�g���N�^
//=====================================================
CSmokeSpawner::CSmokeSpawner()
{
	m_pos = { 0.0f,0.0f,0.0f };
	m_fSpeed = 0.0f;
	m_nLife = 0;
	m_nNumDebris = 0;
}

//=====================================================
// �f�X�g���N�^
//=====================================================
CSmokeSpawner::~CSmokeSpawner()
{

}

//=====================================================
// ����������
//=====================================================
HRESULT CSmokeSpawner::Init(void)
{
	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CSmokeSpawner::Uninit(void)
{
	Release();
}

//=====================================================
// �X�V����
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
		// ���g�̔j��
		Uninit();
	}
}

//=====================================================
// �ʒu�ݒ菈��
//=====================================================
void CSmokeSpawner::SetPosition(D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//=====================================================
// ��������
//=====================================================
CSmokeSpawner *CSmokeSpawner::Create(D3DXVECTOR3 pos,float fSpeed,int nLife, int nNumDebris)
{
	CSmokeSpawner *pSpawner = nullptr;

	if (pSpawner == nullptr)
	{// �C���X�^���X����
		pSpawner = new CSmokeSpawner;

		pSpawner->m_pos = pos;

		pSpawner->m_nLife = nLife;

		pSpawner->m_fSpeed = fSpeed;

		pSpawner->m_nNumDebris = nNumDebris;
	}

	return pSpawner;
}