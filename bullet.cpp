//*****************************************************
//
// �e����[bullet.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
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
// �}�N����`
//*****************************************************
#define SPEED_MOVE	(7.0f)	// �ړ����x
#define ROLL_SPEED	(0.1f)	// ��]���x
#define EDGE_ORBIT	(13)	// �O�Ղ̕ӂ̐�

//*****************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************
int CBullet::m_nNumAll = 0;	// ����

//=====================================================
// �R���X�g���N�^
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

	// �����J�E���g�A�b�v
	m_nNumAll++;
}

//=====================================================
// �f�X�g���N�^
//=====================================================
CBullet::~CBullet()
{
	m_nNumAll--;
}

//=====================================================
// ����������
//=====================================================
HRESULT CBullet::Init(void)
{
	// �ėp�����擾
	CUniversal *pUniversal = CManager::GetUniversal();

	// �p���N���X�̏�����
	CBillboard::Init();

	// �^�C�v�̐ݒ�
	SetType(TYPE_BULLET);

	Draw();

	if (m_apOrbit[0] == nullptr)
	{// �O�Ղ̐���
		m_apOrbit[0] = COrbit::Create(GetMatrix(), D3DXVECTOR3(GetWidth(), 0.0f, 0.0f), D3DXVECTOR3(-GetWidth(), 0.0f, 0.0f), m_col, EDGE_ORBIT);
	}

	if (m_apOrbit[1] == nullptr)
	{// �O�Ղ̐���
		m_apOrbit[1] = COrbit::Create(GetMatrix(), D3DXVECTOR3(0.0f, GetWidth(), 0.0f), D3DXVECTOR3(0.0f, -GetWidth(), 0.0f), m_col, EDGE_ORBIT);
	}

	if (m_apOrbit[2] == nullptr)
	{// �O�Ղ̐���
		m_apOrbit[2] = COrbit::Create(GetMatrix(), D3DXVECTOR3(GetWidth(), GetWidth(), 0.0f), D3DXVECTOR3(-GetWidth(), -GetWidth(), 0.0f), m_col, EDGE_ORBIT);
	}

	if (m_apOrbit[3] == nullptr)
	{// �O�Ղ̐���
		m_apOrbit[3] = COrbit::Create(GetMatrix(), D3DXVECTOR3(GetWidth(), -GetWidth(), 0.0f), D3DXVECTOR3(-GetWidth(), GetWidth(), 0.0f), m_col, EDGE_ORBIT);
	}

	D3DXMATRIX mtx;

	// �}�g���b�N�X������
	D3DXMatrixIdentity(&mtx);

	// �}�g���b�N�X���������킹��
	pUniversal->SetOffSet(&mtx, GetMatrix(), D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_rot);

	for (int nCnt = 0; nCnt < 6; nCnt++)
	{
		for (int nCntOrbit = 0; nCntOrbit < NUM_ORBIT; nCntOrbit++)
		{// �O�Ղ̍X�V
			if (m_apOrbit[nCntOrbit] != nullptr)
			{
				m_apOrbit[nCntOrbit]->SetPositionOffset(mtx, 0);
			}
		}
	}

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CBullet::Uninit(void)
{
	// �q�b�g���Ǘ�
	ManageHit();

	if (m_pCollisionSphere != nullptr)
	{// �����蔻��̏���
		m_pCollisionSphere->Uninit();

		m_pCollisionSphere = nullptr;
	}

	for (int nCntOrbit = 0; nCntOrbit < NUM_ORBIT; nCntOrbit++)
	{
		if (m_apOrbit[nCntOrbit] != nullptr)
		{// �O�Ղ̏I��
			m_apOrbit[nCntOrbit]->Uninit();
			m_apOrbit[nCntOrbit] = nullptr;
		}
	}

	// �p���N���X�̏I��
	CBillboard::Uninit();
}

//=====================================================
// �q�b�g���Ǘ�
//=====================================================
void CBullet::ManageHit(void)
{
	// �v���C���[�擾
	CPlayer *pPlayer = CGame::GetPlayer();

	switch (m_type)
	{
	case TYPE_PLAYER:

		if (m_nNumHit > 0)
		{
			if (pPlayer != nullptr)
			{// �q�b�g�����Z
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
			{// �q�b�g�����Z
				pPlayer->SetNumHit(pPlayer->GetNumHit() - 1);
			}
		}

		break;
	case TYPE_ENEMY:

		if (m_nNumHit > 0)
		{
			if (pPlayer != nullptr)
			{// �q�b�g�����Z
				pPlayer->SetNumHit(pPlayer->GetNumHit() + 1);
			}
		}

		break;
	default:
		break;
	}
}

//=====================================================
// �X�V����
//=====================================================
void CBullet::Update(void)
{
	// �ėp�����擾
	CUniversal *pUniversal = CManager::GetUniversal();

	// �ϐ��錾
	bool bHit = false;

	// �p���N���X�̍X�V
	CBillboard::Update();

	// ��������
	m_nLife--;

	// �ʒu�̍X�V
	SetPosition(GetPosition() + m_move);

	if (m_pCollisionSphere != nullptr)
	{// �����蔻��̊Ǘ�
		// �^�O
		CCollision::TAG tag = CCollision::TAG_PLAYERBULLET;

		// �����蔻��̈ʒu�ݒ�
		m_pCollisionSphere->SetPositionOld(m_pCollisionSphere->GetPosition());
		m_pCollisionSphere->SetPosition(GetPosition());

		switch (m_type)
		{
		case TYPE_PLAYER:
			tag = CCollision::TAG_ENEMY;

			if (m_pCollisionSphere->SphereCollision(CCollision::TAG_MISSILE))
			{// �ΏۂƂ̓����蔻��
				CObject *pObj = m_pCollisionSphere->GetOther();

				bHit = true;

				// �q�b�g�����Z
				m_nNumHit++;

				if (pObj != nullptr)
				{
					// ���������I�u�W�F�N�g�̃q�b�g����
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
		{// �ΏۂƂ̓����蔻��
			CObject *pObj = m_pCollisionSphere->GetOther();

			bHit = true;

			// �q�b�g�����Z
			m_nNumHit++;

			if (pObj != nullptr)
			{
				// ���������I�u�W�F�N�g�̃q�b�g����
				pObj->Hit(m_fDamage);

				CParticle::Create(GetPosition(), CParticle::TYPE_HIT);
			}
		}

		if (m_pCollisionSphere->TriggerCube(CCollision::TAG_BLOCK))
		{// �u���b�N�Ƃ̓����蔻��
			// �����A�j���[�V����
			CExplosion::Create(GetPosition());

			// �����p�[�e�B�N��
			CParticle::Create(GetPosition(), CParticle::TYPE_HIT);

			// �I������
			Uninit();
		}
	}

	// �O�Ղ̃��[����]
	m_rot.z += ROLL_SPEED;

	// �l�̕␳
	pUniversal->LimitRot(&m_rot.z);

	if (bHit == false)
	{
		CMeshField *pMesh = CGame::GetMeshField();
		float fHeight = 0.0f;

		if (pMesh != nullptr)
		{
			// ���b�V���t�B�[���h�Ƃ̓����蔻��
			fHeight = pMesh->GetHeight(GetPosition(), nullptr);
		}

		if (m_nLife < 0 || fHeight > GetPosition().y)
		{// �����̍폜
			CExplosion::Create(GetPosition());

			CParticle::Create(GetPosition(), CParticle::TYPE_HIT);

			Uninit();
		}
	}
	else
	{
		if (m_bPierce == false)
		{// �ђʂ���e�͏����Ȃ�
			CExplosion::Create(GetPosition());

			CParticle::Create(GetPosition(), CParticle::TYPE_HIT);

			// �I������
			Uninit();
		}
	}

	D3DXMATRIX mtx;

	// �}�g���b�N�X������
	D3DXMatrixIdentity(&mtx);

	// �}�g���b�N�X���������킹��
	pUniversal->SetOffSet(&mtx, GetMatrix(), D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_rot);

	for (int nCntOrbit = 0; nCntOrbit < NUM_ORBIT; nCntOrbit++)
	{// �O�Ղ̍X�V
		if (m_apOrbit[nCntOrbit] != nullptr)
		{
			m_apOrbit[nCntOrbit]->SetPositionOffset(mtx, 0);
		}
	}
}

//=====================================================
// ��ʊO�폜����
//=====================================================
void CBullet::LimitPos(void)
{
	// �ʒu�̓���
	D3DXVECTOR3 pos = GetPosition();

	if (pos.x < 0 || pos.x > SCREEN_WIDTH ||
		pos.y < 0 || pos.y > SCREEN_HEIGHT)
	{
		CExplosion::Create(GetPosition());

		Uninit();
	}
}

//=====================================================
// �`�揈��
//=====================================================
void CBullet::Draw(void)
{
	// �ėp�����擾
	CUniversal *pUniversal = CManager::GetUniversal();

	// �r���{�[�h�̕`��
	CBillboard::SetMatrix();
}

//=====================================================
// ��������
//=====================================================
CBullet *CBullet::Create(D3DXVECTOR3 pos,D3DXVECTOR3 move, int nLife,TYPE type, bool bPierce, float fRadius, float fDamage, D3DXCOLOR col)
{
	CBullet *pBullet = nullptr;

	if (pBullet == nullptr)
	{// �C���X�^���X����
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
		{// �����蔻�萶��
			switch (type)
			{// ��ނ��ƂɃ^�O�̐ݒ�
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

		// ����������
		pBullet->Init();

		// �e�N�X�`���̓Ǎ�
		int nIdx = CManager::GetTexture()->Regist("data\\TEXTURE\\EFFECT\\bullet000.png");
		pBullet->SetIdxTexture(nIdx);
	}

	return pBullet;
}

//=====================================================
// �Ǎ�����
//=====================================================
HRESULT CBullet::Load(void)
{
	return S_OK;
}

//=====================================================
// �e�N�X�`���j��
//=====================================================
void CBullet::Unload(void)
{

}