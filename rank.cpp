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
#include "inputkeyboard.h"
#include "player.h"
#include "game.h"
#include "assess.h"
#include "objectmanager.h"

//*****************************************************
// �}�N����`
//*****************************************************
#define FRAME_POS	(D3DXVECTOR3(830.0f,70.0f,0.0f))	// �t���[���̏����ʒu
#define FRAME_WIDTH	(465.0f)	// �t���[���̉���
#define FRAME_HEIGHT	(172.0f)	// �t���[���̏c�̒���
#define FRAME_SCALE	(0.3f)	// �t���[���̃X�P�[��
#define FRAME_PATH	"data\\TEXTURE\\UI\\rank00.png"	// �t���[���̃p�X
#define NUM_PLACE	(2)	// ����
#define RANK_POS	(D3DXVECTOR3(725.0f,70.0f,0.0f))	// �����N�̈ʒu
#define GAUGE_WIDTH	(108.0f)	// �Q�[�W�̉���
#define GAUGE_HEIGHT	(16.0f)	// �Q�[�W�̏c�̒���
#define GAUGE_POS	(D3DXVECTOR3(745.0f,98.0f,0.0f))	// �Q�[�W�̈ʒu
#define INITIAL_RANK	(1)	// ���������N
#define MAX_RANK	(99)	// �����N�̍ő�l
#define GAUGE_COl	(D3DXCOLOR(0.9f,0.9f,0.9f,0.9f))	// �Q�[�W�̐F

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
	m_nRank = INITIAL_RANK;

	m_pGaugeProgress = CObject2D::Create(6);

	if (m_pGaugeProgress != nullptr)
	{
		m_pGaugeProgress->SetPosition(GAUGE_POS);
		m_pGaugeProgress->SetSize(GAUGE_WIDTH, GAUGE_HEIGHT);

		m_pGaugeProgress->SetVtx();
		m_pGaugeProgress->SetCol(GAUGE_COl);
	}

	m_pFrame = CObject2D::Create(7);

	if (m_pFrame != nullptr)
	{
		m_pFrame->SetPosition(FRAME_POS);
		m_pFrame->SetSize(FRAME_WIDTH * FRAME_SCALE, FRAME_HEIGHT * FRAME_SCALE);

		// �e�N�X�`���ԍ��擾
		int nIdx = CManager::GetTexture()->Regist(FRAME_PATH);

		m_pFrame->SetIdxTexture(nIdx);
		m_pFrame->SetVtx();
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
	CInputKeyboard *pKeyboard = CManager::GetKeyboard();

	// �i�s�Q�[�W�̊Ǘ�
	ManageGauge();

	if (pKeyboard != nullptr)
	{
		if (pKeyboard->GetPress(DIK_O))
		{
			AddProgress(1.0f);
		}
	}
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

	float fWidth;
	D3DXVECTOR3 pos;

	fWidth = m_fProgress * GAUGE_WIDTH;

	// �ʒu�ݒ�
	pos = { GAUGE_POS.x + fWidth,GAUGE_POS.y,0.0f };

	// �T�C�Y�ݒ�
	m_pGaugeProgress->SetPosition(pos);
	m_pGaugeProgress->SetSize(fWidth, GAUGE_HEIGHT);
	m_pGaugeProgress->SetVtx();
}

//=====================================================
// �i�s�x�����Z����
//=====================================================
void CRank::AddProgress(float fValue)
{
	// �]���擾
	CAssess *pAssess = nullptr;
	CPlayer *pPlayer = nullptr;
	CObjectManager *pObjManager = CManager::GetObjectManager();

	if (pObjManager != nullptr)
	{// �v���C���[�̓K�p
		pPlayer = pObjManager->GetPlayer();
	}

	if (pPlayer != nullptr)
	{
		pAssess = pPlayer->GetAssess();
 	}

	// �]���̕��ϒl�Z�o
	float fAve = 0.0f;

	if (pAssess != nullptr)
	{
		fAve = (pAssess->GetParam(CAssess::PARAM_ATTACK) + pAssess->GetParam(CAssess::PARAM_DODGE) + pAssess->GetParam(CAssess::PARAM_HIT)) / 3;
	}

	m_fProgress += fValue * fAve + fValue;

	// �l�̐���
	if (m_fProgress < 0.0f)
	{
		m_fProgress = 0.0f;
	}
	if (m_fProgress > 1.0f)
	{// �����N�A�b�v
		m_fProgress = 0.0f;

		m_nRank++;

		// �����N����
		if (m_nRank > MAX_RANK)
		{
			m_nRank = MAX_RANK;

			m_fProgress = 1.0f;
		}
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