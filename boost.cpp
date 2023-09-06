//*****************************************************
//
// �u�[�X�g�̏���[boost.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "boost.h"
#include "manager.h"
#include "renderer.h"
#include "object2D.h"
#include "player.h"
#include "game.h"

//*****************************************************
// �}�N����`
//*****************************************************
#define NUM_PLACE	(6)	// ����
#define GAUGE_WIDTH	(40.0f)	// �Q�[�W�̉���
#define GAUGE_HEIGTH	(250.0f)	// �Q�[�W�̏c�̒���
#define GAUGE_POS	(D3DXVECTOR3(1100.0f,650.0f,0.0f))	// �Q�[�W�̏����ʒu

//=====================================================
// �R���X�g���N�^
//=====================================================
CBoost::CBoost(int nPriority) : CObject(nPriority)
{
	m_pObjectGauge = nullptr;
}

//=====================================================
// �f�X�g���N�^
//=====================================================
CBoost::~CBoost()
{

}

//=====================================================
// ����������
//=====================================================
HRESULT CBoost::Init(void)
{
	if (m_pObjectGauge == nullptr)
	{// �u�[�X�g�Q�[�W�̐���
		m_pObjectGauge = CObject2D::Create(5);

		if (m_pObjectGauge != nullptr)
		{
			m_pObjectGauge->SetPosition(D3DXVECTOR3(GAUGE_POS.x, GAUGE_POS.y, 0.0f));
			m_pObjectGauge->SetSize(GAUGE_WIDTH, GAUGE_HEIGTH);
			m_pObjectGauge->SetVtx();
		}
	}

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CBoost::Uninit(void)
{
	if (m_pObjectGauge != nullptr)
	{// �Q�[�W�̔j��
		m_pObjectGauge = nullptr;
	}

	// ���g�̔j��
	Release();
}

//=====================================================
// �X�V����
//=====================================================
void CBoost::Update(void)
{
	// �ϐ��錾
	float fBoost = 0.0f;
	float fHeight;
	D3DXVECTOR3 pos;
	CPlayer::BOOSTSTATE boostState;

	// �v���C���[�̎擾
	CPlayer *pPlayer = CGame::GetPlayer();

	if (pPlayer != nullptr)
	{// �v���C���[���̎擾
		fBoost = pPlayer->GetBoost();
		boostState = pPlayer->GetBoostState();
	}
	else
	{
		return;
	}

	// �u�[�X�g�̊������Z�o
	fHeight = fBoost / MAX_BOOST;
	fHeight *= GAUGE_HEIGTH;

	if (m_pObjectGauge != nullptr)
	{// �Q�[�W�̐ݒ�
		// �ʒu�ݒ�
		pos = { m_pObjectGauge->GetPosition().x,GAUGE_POS.y - fHeight,0.0f };

		// �F�ݒ�
		if (boostState == CPlayer::BOOSTSTATE_EMPTY)
		{// ���R���
			m_pObjectGauge->SetCol(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
		}
		else
		{// �ʏ���
			m_pObjectGauge->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}

		// �T�C�Y�ݒ�
		m_pObjectGauge->SetPosition(pos);
		m_pObjectGauge->SetSize(GAUGE_WIDTH, fHeight);
		m_pObjectGauge->SetVtx();
	}
}

//=====================================================
// ��������
//=====================================================
CBoost *CBoost::Create(void)
{
	CBoost *pBoost = nullptr;

	if (pBoost == nullptr)
	{
		pBoost = new CBoost;

		pBoost->Init();
	}

	return pBoost;
}