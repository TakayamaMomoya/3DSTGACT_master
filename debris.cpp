//*****************************************************
//
// �j�Ђ̏���[debris.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "debris.h"
#include "game.h"
#include "meshfield.h"
#include "particle.h"
#include "universal.h"
#include "manager.h"

//*****************************************************
// �}�N����`
//*****************************************************
#define MODE_FACT	(0.9f)	// �ړ��ʂ̌����W��
#define GRAVITY	(0.3f)	// �d��

//*****************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************
int CDebris::m_nNumAll = 0;	// ����

//=====================================================
// �R���X�g���N�^
//=====================================================
CDebris::CDebris(int nPriority)
{
	m_nLife = 0;
	m_speedRotation = { 0.0f,0.0f,0.0f };

	m_nNumAll++;
}

//=====================================================
// �f�X�g���N�^
//=====================================================
CDebris::~CDebris()
{
	m_nNumAll--;
}

//=====================================================
// ����������
//=====================================================
HRESULT CDebris::Init(void)
{
	// �p���N���X�̏�����
	CObjectX::Init();

	// ���f���ԍ��̐ݒ�
	int nIdx = CModel::Load("data\\MODEL\\debris.x");
	SetIdxModel(nIdx);

	// ���f�����̐ݒ�
	BindModel(nIdx);

	// ���X�s�[�h�̐ݒ�
	for (int nCnt = 0; nCnt < 3; nCnt++)
	{
		m_speedRotation[nCnt] = rand() % 100 * 0.001f;
	}

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CDebris::Uninit(void)
{
	// �p���N���X�̏I��
	CObjectX::Uninit();
}

//=====================================================
// �X�V����
//=====================================================
void CDebris::Update(void)
{
	// �ėp�����̎擾
	CUniversal *pUniversal = CManager::GetUniversal();

	D3DXVECTOR3 move = GetMove();
	D3DXVECTOR3 pos = GetPosition();
	D3DXVECTOR3 rot = GetRot();

	// �p���N���X�̍X�V
	CObjectX::Update();

	// �ړ��ʂ��ʒu�ɉ��Z
	SetPosition(pos + move);

	// �ړ��ʌ���
	move.y -= GRAVITY;

	SetMove(move);

	rot += m_speedRotation;

	if (pUniversal != nullptr)
	{
		// �����̏C��
		pUniversal->LimitRot(&rot.x);
		pUniversal->LimitRot(&rot.y);
		pUniversal->LimitRot(&rot.z);
	}

	SetRot(rot);

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
void CDebris::Draw(void)
{
	// �p���N���X�̕`��
	CObjectX::Draw();
}

//=====================================================
// ��������
//=====================================================
CDebris *CDebris::Create(D3DXVECTOR3 pos, int nLife)
{
	CDebris *pDebris = nullptr;

	if (pDebris == nullptr)
	{// �C���X�^���X����
		pDebris = new CDebris;

		pDebris->m_nLife = nLife;

		pDebris->SetPosition(pos);

		// ����������
		pDebris->Init();
	}

	return pDebris;
}