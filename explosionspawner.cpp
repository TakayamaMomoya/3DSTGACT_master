//*****************************************************
//
// �p�[�e�B�N������[particle.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "explosionspawner.h"
#include "explosion.h"

//=====================================================
// �D�揇�ʂ����߂�R���X�g���N�^
//=====================================================
CExplSpawner::CExplSpawner()
{
	m_pos = { 0.0f,0.0f,0.0f };
	m_fRadius = 0.0f;
	m_nLife = 0;
	m_nCntSpawn = 0;
}

//=====================================================
//	�f�X�g���N�^
//=====================================================
CExplSpawner::~CExplSpawner()
{

}

//=====================================================
//	����������
//=====================================================
HRESULT CExplSpawner::Init(void)
{
	return S_OK;
}

//=====================================================
//	�I������
//=====================================================
void CExplSpawner::Uninit(void)
{
	Release();
}

//=====================================================
//	�X�V����
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

		// �J�E���^�[���Z�b�g
		m_nCntSpawn = 0;

		if (m_nLife > 0)
		{
			// �^�C�}�[�Đݒ�
			m_nTimerSpawn = rand() % m_nLife / 2;
		}
	}

	m_nLife--;

	if (m_nLife < 0)
	{
		// ���g�̔j��
		Uninit();
	}
}

//=====================================================
//	�ʒu�ݒ菈��
//=====================================================
void CExplSpawner::SetPosition(D3DXVECTOR3 pos)
{
	m_pos = pos;
}

//=====================================================
//	��������
//=====================================================
CExplSpawner **CExplSpawner::Create(D3DXVECTOR3 pos,float fRadius,int nLife)
{
	CExplSpawner *pSpawner = nullptr;

	if (pSpawner == nullptr)
	{// �C���X�^���X����
		pSpawner = new CExplSpawner;

		pSpawner->m_pos = pos;

		pSpawner->m_nLife = nLife;

		pSpawner->m_fRadius = fRadius;
	}

	return &pSpawner;
}