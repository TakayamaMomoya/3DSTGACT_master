//*****************************************************
//
// ��q�ڕW�̏���[defend.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
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
// �}�N����`
//*****************************************************
#define MAX_LIFE	(600)	// �̗�
#define TIME_DEATH	(200)	// ���S����܂ł̃t���[����

//=====================================================
// �R���X�g���N�^
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
// �f�X�g���N�^
//=====================================================
CDefend::~CDefend()
{

}

//=====================================================
// ����������
//=====================================================
HRESULT CDefend::Init(void)
{
	// �l�̏����ݒ�
	m_nLife = MAX_LIFE;

	if (m_pObjectX == nullptr)
	{
		m_pObjectX = CObjectX::Create();

		if (m_pObjectX != nullptr)
		{
			// ���f���ԍ��̐ݒ�
			int nIdx = CModel::Load("data\\MODEL\\BLOCK\\bill00.x");
			m_pObjectX->SetIdxModel(nIdx);

			// ���f�����̐ݒ�
			m_pObjectX->BindModel(nIdx);

			if (m_pCollisionCube == nullptr)
			{// �����蔻�萶��
				m_pCollisionCube = CCollisionCube::Create(CCollision::TAG_BLOCK, this);

				D3DXVECTOR3 pos = GetPosition();

				if (m_pCollisionCube != nullptr)
				{
					m_pCollisionCube->SetPosition(GetPosition());

					m_pCollisionCube->SetVtx(m_pObjectX->GetVtxMax(), m_pObjectX->GetVtxMin());
				}
			}

			if (m_pCollisionSphere == nullptr)
			{// ���̓����蔻�萶��
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
// �I������
//=====================================================
void CDefend::Uninit(void)
{
	CGame::ReleaseDefend();

	if (m_pObjectX != nullptr)
	{// ���f���̔j��
		m_pObjectX->Uninit();
		m_pObjectX = nullptr;
	}

	if (m_pCollisionCube != nullptr)
	{// �����̂̓����蔻��̔j��
		m_pCollisionCube->Uninit();
		m_pCollisionCube = nullptr;
	}

	if (m_pCollisionSphere != nullptr)
	{// ���̓����蔻��̔j��
		m_pCollisionSphere->Uninit();
		m_pCollisionSphere = nullptr;
	}

	Release();
}

//=====================================================
// �X�V����
//=====================================================
void CDefend::Update(void)
{
	// ��ԊǗ�
	ManageState();
}

//=====================================================
// ��ԊǗ�
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
		{// ���g�̔j��
			Uninit();
		}

		break;
	default:
		break;
	}
}

//=====================================================
// �q�b�g����
//=====================================================
void CDefend::Hit(float fDamage)
{
	if (m_state == STATE_DEATH)
	{
		return;
	}

	// �̗͌���
	SetLife(GetLife() - (int)fDamage);

    if (GetLife() == 0)
	{
		// �����̔���
		CParticle::Create(GetPosition(), CParticle::TYPE_EXPLOSION);
		CExplosion::Create(GetPosition())->SetSize(220.0f, 220.0f);

		CExplSpawner::Create(GetPosition(),m_pObjectX->GetRadius(), TIME_DEATH);

		m_state = STATE_DEATH;
	}
}

//=====================================================
// �`�揈��
//=====================================================
void CDefend::Draw(void)
{
	
}

//=====================================================
// �ʒu�擾����
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
// ��������
//=====================================================
CDefend *CDefend::Create(void)
{
	CDefend *pDefend = nullptr;

	if (pDefend == nullptr)
	{// �C���X�^���X����
		pDefend = new CDefend;

		// ����������
		pDefend->Init();
	}

	return pDefend;
}

//=====================================================
// ���C�t�ݒ�
//=====================================================
void CDefend::SetLife(int nLife)
{
	m_nLife = nLife;

	if (m_nLife < 0)
	{// �O��艺�ɂ����Ȃ��悤�ɕ␳
		m_nLife = 0;
	}
}