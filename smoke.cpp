//*****************************************************
//
// ���̏���[smoke.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "smoke.h"
#include "game.h"
#include "meshfield.h"
#include "particle.h"

//*****************************************************
// �}�N����`
//*****************************************************
#define MODE_FACT	(0.9f)	// �ړ��ʂ̌����W��
#define GRAVITY	(0.7f)	// �d��

//*****************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************
int CSmoke::m_nNumAll = 0;	// ����

//=====================================================
// �R���X�g���N�^
//=====================================================
CSmoke::CSmoke(int nPriority)
{
	m_nLife = 0;

	m_nNumAll++;
}

//=====================================================
// �f�X�g���N�^
//=====================================================
CSmoke::~CSmoke()
{
	m_nNumAll--;
}

//=====================================================
// ����������
//=====================================================
HRESULT CSmoke::Init(void)
{
	// �p���N���X�̏�����
	CObjectX::Init();

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CSmoke::Uninit(void)
{
	// �p���N���X�̏I��
	CObjectX::Uninit();
}

//=====================================================
// �X�V����
//=====================================================
void CSmoke::Update(void)
{
	D3DXVECTOR3 move = GetMove();
	D3DXVECTOR3 pos = GetPosition();

	// �p���N���X�̍X�V
	CObjectX::Update();

	CParticle::Create(pos,CParticle::TYPE_SMOKE,D3DXVECTOR3(0.0f,0.0f,0.0f),nullptr,4);

	// �ړ��ʂ��ʒu�ɉ��Z
	SetPosition(pos + move);

	// �ړ��ʌ���
	//move.x *= MODE_FACT;
	//move.z *= MODE_FACT;
	move.y -= GRAVITY;

	SetMove(move);

	// ���b�V���t�B�[���h�Ƃ̓����蔻��
	CMeshField *pMesh = CGame::GetMeshField();

	if (pMesh != nullptr)
	{
		move = GetMove();
		pos = GetPosition();

		// �����̎擾
		float fHeight = CGame::GetMeshField()->GetHeight(pos, &move);

		if (fHeight >= pos.y)
		{// �擾���������ɐݒ�
			pos.y = fHeight;

			SetPosition(pos);
		}
	}

	m_nLife--;

	if (m_nLife <= 0)
	{// �����ɂ��j��
		Uninit();
	}
}

//=====================================================
// �`�揈��
//=====================================================
void CSmoke::Draw(void)
{
	// �p���N���X�̕`��
	CObjectX::Draw();
}

//=====================================================
// ��������
//=====================================================
CSmoke *CSmoke::Create(D3DXVECTOR3 pos, int nLife)
{
	CSmoke *pDebris = nullptr;

	if (pDebris == nullptr)
	{// �C���X�^���X����
		pDebris = new CSmoke;

		pDebris->m_nLife = nLife;

		pDebris->SetPosition(pos);

		// ����������
		pDebris->Init();
	}

	return pDebris;
}