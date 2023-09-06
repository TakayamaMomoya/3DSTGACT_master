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

//*****************************************************
// �}�N����`
//*****************************************************
#define GAUGE_POS	(D3DXVECTOR3(50.0f,550.0f,0.0f))	// �Q�[�W�̏����ʒu
#define GAUGE_WIDTH	(20.0f)	// �Q�[�W�̉���
#define GAUGE_HEIGHT	(125.0f)	// �Q�[�W�̏c�̒���
#define BETWEEN_HEIGHT	(50.0f)	// �Q�[�W�Ԃ̕�

//=====================================================
// �R���X�g���N�^
//=====================================================
CAssess::CAssess(int nPriority)
{
	m_fAssessHit = 0.0f;
	m_fAssessDodge = 0.0f;
	m_fAssessAttack = 0.0f;
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
				m_apGauge[nCnt]->SetPosition(D3DXVECTOR3(GAUGE_POS.x + BETWEEN_HEIGHT * nCnt, GAUGE_POS.y, 0.0f));
				m_apGauge[nCnt]->SetSize(GAUGE_WIDTH, GAUGE_HEIGHT);
				m_apGauge[nCnt]->SetVtx();
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
			float fHeight = aParam[nCnt] * GAUGE_HEIGHT;

			// �ʒu�ݒ�
			D3DXVECTOR3 pos = { GAUGE_POS.x + BETWEEN_HEIGHT * nCnt,GAUGE_POS.y - fHeight,0.0f };
 
			// �T�C�Y�ݒ�
			m_apGauge[nCnt]->SetPosition(pos);
			m_apGauge[nCnt]->SetSize(GAUGE_WIDTH, fHeight);
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