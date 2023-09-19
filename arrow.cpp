//*****************************************************
//
// ���̏���[arrow.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "arrow.h"
#include "player.h"
#include "game.h"
#include "enemy.h"
#include "objectmanager.h"
#include "manager.h"

//*****************************************************
// �}�N����`
//*****************************************************
#define LENGTH (300.0f)	// �v���C���[����̋���
#define HEIGHT	(100.0f)	// ����

//=====================================================
// �R���X�g���N�^
//=====================================================
CArrow::CArrow(int nPriority)
{
	m_posDest = { 0.0f,0.0f,0.0f };
}

//=====================================================
// �f�X�g���N�^
//=====================================================
CArrow::~CArrow()
{

}

//=====================================================
// ��������
//=====================================================
CArrow *CArrow::Create(void)
{
	CArrow *pDebris = nullptr;

	if (pDebris == nullptr)
	{// �C���X�^���X����
		pDebris = new CArrow;

		// ����������
		pDebris->Init();
	}

	return pDebris;
}

//=====================================================
// ����������
//=====================================================
HRESULT CArrow::Init(void)
{
	// �p���N���X�̏�����
	CObjectX::Init();

	// ���f���ԍ��̐ݒ�
	int nIdx = CModel::Load("data\\MODEL\\arrow.x");

	SetIdxModel(nIdx);

	BindModel(nIdx);

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CArrow::Uninit(void)
{
	CObjectX::Uninit();
}

//=====================================================
// �X�V����
//=====================================================
void CArrow::Update(void)
{
	CObjectX::Update();

	RollPlayer();
}

//=====================================================
// �v���C���[�̎���ɂ��鏈��
//=====================================================
void CArrow::RollPlayer(void)
{
	CPlayer *pPlayer = nullptr;
	CObjectManager *pObjManager = CManager::GetObjectManager();

	if (pObjManager != nullptr)
	{// �v���C���[�̓K�p
		pPlayer = pObjManager->GetPlayer();
	}

	D3DXVECTOR3 posTarget;
	D3DXVECTOR3 vecDiff;
	D3DXVECTOR3 rot = GetRot();

	if (pPlayer == nullptr)
	{
		return;
	}

	// �ړI���W�Ƃ̍������擾
	posTarget = pPlayer->GetPosition();

	vecDiff = posTarget - m_posDest;

	D3DXVec3Normalize(&vecDiff,&vecDiff);

	// ���̋����ɕ␳
	vecDiff *= LENGTH;

	posTarget = D3DXVECTOR3(posTarget.x - vecDiff.x,posTarget.y + HEIGHT, posTarget.z - vecDiff.z);

	SetPosition(posTarget);

	// �����x�N�g���̕���������
	rot.y = atan2f(vecDiff.x, vecDiff.z);

	SetRot(rot);
}

//=====================================================
// �`�揈��
//=====================================================
void CArrow::Draw(void)
{
	CObjectX::Draw();
}