//*****************************************************
//
// �]���̏���[assess.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "assess.h"
#include "object2D.h"
#include "manager.h"
#include "debugproc.h"
#include "texture.h"

//*****************************************************
// �}�N����`
//*****************************************************
#define GAUGE_POS	(D3DXVECTOR3(50.0f,550.0f,0.0f))	// �Q�[�W�̏����ʒu
#define GAUGE_WIDTH	(SCREEN_WIDTH / 3)	// �Q�[�W�̉���
#define GAUGE_HEIGHT	(9.0f)	// �Q�[�W�̏c�̒���
#define BETWEEN_HEIGHT	(50.0f)	// �Q�[�W�Ԃ̕�
#define FRAME_HEIGHT	(10.0f)	// �t���[���̏c�̒���
#define FRAME_WIDTH	(SCREEN_WIDTH / 3)	// �t���[���̉���

//=====================================================
// �R���X�g���N�^
//=====================================================
CAssess::CAssess(int nPriority)
{
	m_fAssessHit = 0.0f;
	m_fAssessDodge = 0.0f;
	m_fAssessAttack = 0.0f;
	ZeroMemory(&m_apGauge[0], PARAM_MAX);
	ZeroMemory(&m_apFrame[0], PARAM_MAX);
}

//=====================================================
// �f�X�g���N�^
//=====================================================
CAssess::~CAssess()
{

}

//=====================================================
// ����������
//=====================================================
HRESULT CAssess::Init(void)
{
	for (int nCnt = 0; nCnt < PARAM_MAX; nCnt++)
	{
		if (m_apGauge[nCnt] == nullptr)
		{// �u�[�X�g�Q�[�W�̐���
			m_apGauge[nCnt] = CObject2D::Create(7);

			if (m_apGauge[nCnt] != nullptr)
			{
				m_apGauge[nCnt]->SetPosition(D3DXVECTOR3(FRAME_WIDTH * 0.5f + FRAME_WIDTH * nCnt, FRAME_HEIGHT, 0.0f));
				m_apGauge[nCnt]->SetSize(GAUGE_WIDTH, GAUGE_HEIGHT);
				m_apGauge[nCnt]->SetVtx();
			}
		}

		if (m_apFrame[nCnt] == nullptr)
		{// �p�����[�^�[�t���[���̐���
			m_apFrame[nCnt] = CObject2D::Create(7);

			if (m_apFrame[nCnt] != nullptr)
			{
				// �e�N�X�`���ԍ��擾
				int nIdx = CManager::GetTexture()->Regist("data\\TEXTURE\\UI\\frame04.png");

				m_apFrame[nCnt]->SetIdxTexture(nIdx);
				m_apFrame[nCnt]->SetVtx();
				m_apFrame[nCnt]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

				m_apFrame[nCnt]->SetPosition(D3DXVECTOR3(FRAME_WIDTH * 0.5f + FRAME_WIDTH * nCnt, FRAME_HEIGHT, 0.0f));
				m_apFrame[nCnt]->SetSize(FRAME_WIDTH * 0.5f, FRAME_HEIGHT);
				m_apFrame[nCnt]->SetVtx();
			}
		}
	}

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CAssess::Uninit(void)
{
	for (int nCnt = 0; nCnt < PARAM_MAX; nCnt++)
	{
		if (m_apGauge[nCnt] != nullptr)
		{
			m_apGauge[nCnt]->Uninit();

			m_apGauge[nCnt] = nullptr;
		}

		if (m_apFrame[nCnt] != nullptr)
		{
			m_apFrame[nCnt]->Uninit();

			m_apFrame[nCnt] = nullptr;
		}
	}

	Release();
}

//=====================================================
// �X�V����
//=====================================================
void CAssess::Update(void)
{
	// �]���̐���
	LimitAssess();

	// �Q�[�W�̊Ǘ�
	ManageGauge();
}

//=====================================================
// �Q�[�W�Ǘ�
//=====================================================
void CAssess::ManageGauge(void)
{
	float aParam[PARAM_MAX] =
	{
		m_fAssessHit,
		m_fAssessDodge,
		m_fAssessAttack
	};

	for (int nCnt = 0; nCnt < PARAM_MAX; nCnt++)
	{
		if (m_apGauge[nCnt] != nullptr)
		{
			float fWidth = aParam[nCnt] * GAUGE_WIDTH * 0.5f;

			// �T�C�Y�ݒ�
			m_apGauge[nCnt]->SetSize(fWidth, GAUGE_HEIGHT);
			m_apGauge[nCnt]->SetVtx();
		}
	}
}

//=====================================================
// �]���̐���
//=====================================================
void CAssess::LimitAssess(void)
{
	if (m_fAssessAttack >= 1.0f)
	{
		m_fAssessAttack = 1.0f;
	}
	else if (m_fAssessAttack < 0.0f)
	{
		m_fAssessAttack = 0.0f;
	}

	if (m_fAssessHit >= 1.0f)
	{
		m_fAssessHit = 1.0f;
	}
	else if (m_fAssessHit < 0.0f)
	{
		m_fAssessHit = 0.0f;
	}

	if (m_fAssessDodge >= 1.0f)
	{
		m_fAssessDodge = 1.0f;
	}
	else if (m_fAssessDodge < 0.0f)
	{
		m_fAssessDodge = 0.0f;
	}
}

//=====================================================
// �`�揈��
//=====================================================
void CAssess::Draw(void)
{
	CManager::GetDebugProc()->Print("\n�������F[%f]", m_fAssessHit);
}

//=====================================================
// ��������
//=====================================================
CAssess *CAssess::Create(void)
{
	CAssess *pAssess = nullptr;

	if (pAssess == nullptr)
	{// �C���X�^���X����
		pAssess = new CAssess;

		// ����������
		pAssess->Init();
	}

	return pAssess;
}