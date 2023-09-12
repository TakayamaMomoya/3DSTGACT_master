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
#define FRAME_PATH	"data\\TEXTURE\\UI\\frame04.png"	// �t���[���̃p�X
#define GAUGE_PATH	"data\\TEXTURE\\UI\\gauge00.png"	// �Q�[�W�̃p�X

//=====================================================
// �R���X�g���N�^
//=====================================================
CAssess::CAssess(int nPriority)
{
	ZeroMemory(&m_apParam[0], PARAM_MAX);
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
		if (m_apParam[nCnt] != nullptr)
		{
			continue;
		}
		else
		{
			m_apParam[nCnt] = new Param;
		}

		if (m_apParam[nCnt]->pGauge == nullptr)
		{// �u�[�X�g�Q�[�W�̐���
			m_apParam[nCnt]->pGauge = CObject2D::Create(7);

			if (m_apParam[nCnt]->pGauge != nullptr)
			{
				// �e�N�X�`���ԍ��擾
				int nIdx = CManager::GetTexture()->Regist(GAUGE_PATH);

				m_apParam[nCnt]->pGauge->SetIdxTexture(nIdx);
				m_apParam[nCnt]->pGauge->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
				m_apParam[nCnt]->pGauge->SetPosition(D3DXVECTOR3(FRAME_WIDTH * 0.5f + FRAME_WIDTH * nCnt, FRAME_HEIGHT, 0.0f));
				m_apParam[nCnt]->pGauge->SetSize(GAUGE_WIDTH, GAUGE_HEIGHT);
				m_apParam[nCnt]->pGauge->SetVtx();
			}
		}

		if (m_apParam[nCnt]->pFrame == nullptr)
		{// �p�����[�^�[�t���[���̐���
			m_apParam[nCnt]->pFrame = CObject2D::Create(7);

			if (m_apParam[nCnt]->pFrame != nullptr)
			{
				// �e�N�X�`���ԍ��擾
				int nIdx = CManager::GetTexture()->Regist(FRAME_PATH);

				m_apParam[nCnt]->pFrame->SetIdxTexture(nIdx);
				m_apParam[nCnt]->pFrame->SetVtx();
				m_apParam[nCnt]->pFrame->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

				m_apParam[nCnt]->pFrame->SetPosition(D3DXVECTOR3(FRAME_WIDTH * 0.5f + FRAME_WIDTH * nCnt, FRAME_HEIGHT, 0.0f));
				m_apParam[nCnt]->pFrame->SetSize(FRAME_WIDTH * 0.5f, FRAME_HEIGHT);
				m_apParam[nCnt]->pFrame->SetVtx();
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
		if (m_apParam[nCnt] != nullptr)
		{
			if (m_apParam[nCnt]->pGauge != nullptr)
			{// �Q�[�W�̔j��
				m_apParam[nCnt]->pGauge->Uninit();

				m_apParam[nCnt]->pGauge = nullptr;
			}

			if (m_apParam[nCnt]->pFrame != nullptr)
			{// �t���[���̔j��
				m_apParam[nCnt]->pFrame->Uninit();

				m_apParam[nCnt]->pFrame = nullptr;
			}

			// �p�����[�^�[���̔j��
			delete m_apParam[nCnt];
		}
	}

	Release();
}

//=====================================================
// �X�V����
//=====================================================
void CAssess::Update(void)
{
	// �Q�[�W�̊Ǘ�
	ManageGauge();
}

//=====================================================
// �Q�[�W�Ǘ�
//=====================================================
void CAssess::ManageGauge(void)
{
	for (int nCnt = 0; nCnt < PARAM_MAX; nCnt++)
	{
		if (m_apParam[nCnt]->pGauge != nullptr)
		{
			float fWidth = m_apParam[nCnt]->fParam * GAUGE_WIDTH * 0.5f;

			// �T�C�Y�ݒ�
			m_apParam[nCnt]->pGauge->SetSize(fWidth, GAUGE_HEIGHT);
			m_apParam[nCnt]->pGauge->SetVtx();
		}
	}
}

//=====================================================
// �Q�[�W�̖���
//=====================================================
void CAssess::PalsingGauge(Param *pParam)
{
	if (pParam->pGauge == nullptr)
	{
		return;
	}

	// �ϐ��錾
	CObject2D *pObj;
	float fWidth;
	float fWidthDest = pParam->fParam * GAUGE_WIDTH * 0.5f;

	pObj = pParam->pGauge;
	
	// ���擾
	fWidth = pObj->GetWidth();
}

//=====================================================
// �ړI�̕������߂鏈��
//=====================================================
void CAssess::SetWidthDest(Param *pParam)
{
	// �ړI�l��ݒ�
	pParam->fParam = pParam->fParam * GAUGE_WIDTH * 0.5f;

	float fWidthDest = pParam->fWidthDest;

	switch (pParam->state)
	{
	case GAUGESTATE_EXTEND:

		// ���߂̈ʒu�ɖړI�l��ݒ�
		fWidthDest += (float)(rand() / 5) * 0.01f;

		break;
	case GAUGESTATE_SHRINK:

		// �Z�߂̈ʒu�ɖړI�l��ݒ�
		fWidthDest -= (float)(rand() / 5) * 0.01f;

		break;
	default:
		break;
	}
}

//=====================================================
// �p�����[�^�[���Z����
//=====================================================
void CAssess::AddParam(float fValue, PARAM param)
{
	if (m_apParam[param] != nullptr)
	{
		m_apParam[param]->fParam += fValue;

		if (m_apParam[param]->fParam >= 1.0f)
		{
			m_apParam[param]->fParam = 1.0f;
		}
		else if (m_apParam[param]->fParam < 0.0f)
		{
			m_apParam[param]->fParam = 0.0f;
		}
	}
}

//=====================================================
// �p�����[�^�[�擾����
//=====================================================
float CAssess::GetParam(PARAM param)
{
	float fParam = 0.0f;

	if (m_apParam[param] != nullptr)
	{
		fParam = m_apParam[param]->fParam;
	}

	return fParam;
}

//=====================================================
// �`�揈��
//=====================================================
void CAssess::Draw(void)
{

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