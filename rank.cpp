//*****************************************************
//
// �����N�̏���[rank.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "rank.h"
#include "manager.h"
#include "debugproc.h"
#include "object2D.h"
#include "texture.h"
#include "number.h"

//*****************************************************
// �}�N����`
//*****************************************************
#define FRAME_POS	(D3DXVECTOR3(830.0f,60.0f,0.0f))	// �t���[���̏����ʒu
#define FRAME_WIDTH	(465.0f)	// �t���[���̉���
#define FRAME_HEIGHT	(172.0f)	// �t���[���̏c�̒���
#define FRAME_SCALE	(0.3f)	// �t���[���̃X�P�[��
#define FRAME_PATH	"data\\TEXTURE\\UI\\rank00.png"	// �t���[���̃p�X
#define NUM_PLACE	(2)	// ����
#define RANK_POS	(D3DXVECTOR3(725.0f,60.0f,0.0f))	// �����N�̈ʒu
#define GAUGE_WIDTH	(100.0f)	// �Q�[�W�̉���
#define GAUGE_HEIGHT	(16.0f)	// �Q�[�W�̏c�̒���
#define GAUGE_POS	(D3DXVECTOR3(745.0f,88.0f,0.0f))	// �Q�[�W�̈ʒu

//=====================================================
// �R���X�g���N�^
//=====================================================
CRank::CRank(int nPriority)
{
	m_fProgress = 0.0f;
	m_nRank = 0;
	m_pFrame = nullptr;
	m_pRankNum = nullptr;
	m_pGaugeProgress = nullptr;
}

//=====================================================
// �f�X�g���N�^
//=====================================================
CRank::~CRank()
{

}

//=====================================================
// ����������
//=====================================================
HRESULT CRank::Init(void)
{
	m_pFrame = CObject2D::Create(6);

	if (m_pFrame != nullptr)
	{
		m_pFrame->SetPosition(FRAME_POS);
		m_pFrame->SetSize(FRAME_WIDTH * FRAME_SCALE, FRAME_HEIGHT * FRAME_SCALE);

		// �e�N�X�`���ԍ��擾
		int nIdx = CManager::GetTexture()->Regist(FRAME_PATH);

		m_pFrame->SetIdxTexture(nIdx);
		m_pFrame->SetVtx();
	}

	m_pGaugeProgress = CObject2D::Create(6);

	if (m_pGaugeProgress != nullptr)
	{
		m_pGaugeProgress->SetPosition(GAUGE_POS);
		m_pGaugeProgress->SetSize(GAUGE_WIDTH, GAUGE_HEIGHT);

		m_pGaugeProgress->SetVtx();
	}


	if (m_pRankNum == nullptr)
	{
		m_pRankNum = CNumber::Create(NUM_PLACE, m_nRank);

		if (m_pRankNum != nullptr)
		{
			m_pRankNum->SetPosition(RANK_POS);
			m_pRankNum->SetSizeAll(15.0f, 32.5f);
		}
	}

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CRank::Uninit(void)
{
	if (m_pFrame != nullptr)
	{
		m_pFrame->Uninit();

		m_pFrame = nullptr;
	}

	if (m_pGaugeProgress != nullptr)
	{
		m_pGaugeProgress->Uninit();

		m_pGaugeProgress = nullptr;
	}

	if (m_pRankNum != nullptr)
	{
		m_pRankNum->Uninit();

		m_pRankNum = nullptr;
	}

	Release();
}

//=====================================================
// �X�V����
//=====================================================
void CRank::Update(void)
{
	// �i�s�Q�[�W�̊Ǘ�
	ManageGauge();
}

//=====================================================
// �i�s�Q�[�W�̊Ǘ�
//=====================================================
void CRank::ManageGauge(void)
{
	if (m_pGaugeProgress == nullptr)
	{
		return;
	}
}

//=====================================================
// �i�s�x�����Z����
//=====================================================
void CRank::AddProgress(float fValue)
{
	m_fProgress += fValue;

	// �l�̐���
	if (m_fProgress < 0.0f)
	{
		m_fProgress = 0.0f;
	}
	if (m_fProgress > 1.0f)
	{// �����N�A�b�v
		m_fProgress = 0.0f;

		m_nRank++;
	}

	if (m_pRankNum != nullptr)
	{
		m_pRankNum->SetValue(m_nRank,NUM_PLACE);
	}
}

//=====================================================
// �`�揈��
//=====================================================
void CRank::Draw(void)
{

}

//=====================================================
// ��������
//=====================================================
CRank *CRank::Create(void)
{
	CRank *pRank = nullptr;

	if (pRank == nullptr)
	{// �C���X�^���X����
		pRank = new CRank;

		// ����������
		pRank->Init();
	}

	return pRank;
}