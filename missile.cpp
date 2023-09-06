//*****************************************************
//
// �~�T�C���̏���[enemymissile.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
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
// �}�N����`
//*****************************************************
#define MAX_SPEED	(30.0f)	// �ړ����x
#define CHASE_SPEED	(1.0f)	// �ǐՑ��x
#define INITIAL_LIFE	(1)	// �����̗�
#define DEATH_TIME	(300)	// ���ł܂ł̎���
#define SIZE_ORBIT	(50.0f)	// �O�Ղ̃T�C�Y
#define LENGTH_ORBIT	(50)	// �O�Ղ̒���

//=====================================================
// �R���X�g���N�^
//=====================================================
CMissile::CMissile()
{
	m_nDeathTimer = 0;
	m_pCollisionSphere = nullptr;
	m_pOrbit = nullptr;
}

//=====================================================
// �f�X�g���N�^
//=====================================================
CMissile::~CMissile()
{

}

//=====================================================
// ��������
//=====================================================
CMissile *CMissile::Create(D3DXVECTOR3 pos)
{
	CMissile *pMissile = nullptr;

	pMissile = new CMissile;

	if (pMissile != nullptr)
	{
		pMissile->SetPosition(pos);

		// ����������
		pMissile->Init();
	}

	return pMissile;
}

//=====================================================
// ����������
//=====================================================
HRESULT CMissile::Init(void)
{
	// �p���N���X�̏�����
	CObjectX::Init();

	// ���f���ԍ��̐ݒ�
	int nIdx = CModel::Load("data\\MODEL\\CHARACTER\\missile.x");
	SetIdxModel(nIdx);

	// ���f�����̐ݒ�
	BindModel(nIdx);

	if (m_pCollisionSphere == nullptr)
	{// ���̓����蔻�萶��
		m_pCollisionSphere = CCollisionSphere::Create(CCollision::TAG_MISSILE, CCollision::TYPE_SPHERE, this);

		if (m_pCollisionSphere != nullptr)
		{// ���̐ݒ�
			m_pCollisionSphere->SetPosition(GetPosition());
			m_pCollisionSphere->SetRadius(GetRadius());
		}
	}

	CObjectX::Draw();

	if (m_pOrbit == nullptr)
	{// �O�Ղ̐���
		m_pOrbit = COrbit::Create(*GetMatrix(), D3DXVECTOR3(0.0f, 0.0f, SIZE_ORBIT), D3DXVECTOR3(0.0f, 0.0f, -SIZE_ORBIT), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), LENGTH_ORBIT);
	}

	return S_OK;
}

//=====================================================
// ���S����
//=====================================================
void CMissile::Death(void)
{
	// �����̔���
	CParticle::Create(GetPosition(), CParticle::TYPE_EXPLOSION);
	CExplosion::Create(GetPosition())->SetSize(300.0f, 300.0f);

	if (m_pOrbit != nullptr)
	{// �O�Ղ̐؂藣��
		m_pOrbit->SetEnd(true);
	}

	// ���g�̏I��
	Uninit();
}

//=====================================================
// �I������
//=====================================================
void CMissile::Uninit(void)
{
	if (m_pCollisionSphere != nullptr)
	{// �����蔻��̏���
		m_pCollisionSphere->Uninit();

		m_pCollisionSphere = nullptr;
	}

	if (m_pOrbit != nullptr)
	{// �O�Ղ̏I��
		m_pOrbit = nullptr;
	}

	// �p���N���X�̏I��
	CObjectX::Uninit();
}

//=====================================================
// �X�V����
//=====================================================
void CMissile::Update(void)
{
	// �ėp�����擾
	CUniversal *pUniversal = CManager::GetUniversal();

	D3DXVECTOR3 vecMove;
	D3DXMATRIX mtx;

	// �ʒu�ۑ�
	SetPositionOld(GetPosition());

	// �ǐՏ���
	CMissile::ChasePlayer();

	// ���x����
	vecMove = GetMove();

	if (D3DXVec3Length(&vecMove) > MAX_SPEED)
	{
		D3DXVec3Normalize(&vecMove, &vecMove);

		vecMove *= MAX_SPEED;

		SetMove(vecMove);
	}

	// ���b�V���t�B�[���h�Ƃ̔���
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
	{// ���̓����蔻��̊Ǘ�
		m_pCollisionSphere->SetPosition(GetPosition());
	}

	// �ڕW�ɋ߂���Δ���
	if (m_pCollisionSphere != nullptr)
	{
		if (m_pCollisionSphere->SphereCollision(CCollision::TAG_PLAYER))
		{
			CObject *pObj = m_pCollisionSphere->GetOther();

			if (pObj != nullptr)
			{
				// ���������I�u�W�F�N�g�̃q�b�g����
				pObj->Hit(10.0f);

				Death();
			}
		}
	}

	// �}�g���b�N�X������
	D3DXMatrixIdentity(&mtx);

	// �}�g���b�N�X���������킹��
	pUniversal->SetOffSet(&mtx, *GetMatrix(), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	if (m_pOrbit != nullptr)
	{// �O�Ղ̍X�V
		m_pOrbit->SetPositionOffset(mtx, 0);
	}

	m_nDeathTimer++;

	if (m_nDeathTimer > DEATH_TIME)
	{
		Death();
	}

	// �X���̐���
	Tilt();

	// �p���N���X�̍X�V
	CObjectX::Update();
}

//=====================================================
// �ǐՏ���
//=====================================================
void CMissile::ChasePlayer(void)
{
	// ���擾
	CPlayer *pPlayer = CGame::GetPlayer();

	// �v�Z�p�ϐ�
	D3DXVECTOR3 pos;
	D3DXVECTOR3 vecDiff;

	if (pPlayer == nullptr)
	{// ���S
		Uninit();

		return;
	}

	pos = pPlayer->GetPosition();

	pos.y += 100.0f;

	// �������擾
	vecDiff = pos - GetPosition();

	D3DXVec3Normalize(&vecDiff,&vecDiff);

	vecDiff *= CHASE_SPEED;

	// �ړ��ʐݒ�
	SetMove(GetMove() + vecDiff);
}

//=====================================================
// �X���̊Ǘ�
//=====================================================
void CMissile::Tilt(void)
{
	D3DXVECTOR3 rot = { 0.0f,0.0f,0.0f };
	D3DXVECTOR3 move = GetMove();
	float fLength;

	// Y�����ʂ̊p�x���擾
	rot.y = atan2f(move.x, move.z);

	// Y�����ʂ̈ړ��ʂ��擾
	fLength = D3DXVec2Length(&D3DXVECTOR2(move.x,move.z));

	// �����ʂ̊p�x���擾
	rot.x = atan2f(fLength,move.y);

	// ��]�̐ݒ�
	SetRot(rot);
}

//=====================================================
// �`�揈��
//=====================================================
void CMissile::Draw(void)
{
	// �p���N���X�̕`��
	CObjectX::Draw();
}