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
#include "texture.h"
#include "renderer.h"
#include "object2D.h"
#include "player.h"
#include "game.h"
#include "inputkeyboard.h"
#include "debugproc.h"
#include "objectmanager.h"

//*****************************************************
// �}�N����`
//*****************************************************
#define NUM_PLACE	(6)	// ����
#define GAUGE_WIDTH	(17.0f)	// �Q�[�W�̉���
#define GAUGE_HEIGTH	(215.0f)	// �Q�[�W�̏c�̒���
#define GAUGE_POS	(D3DXVECTOR3(1175.0f,643.0f,0.0f))	// �Q�[�W�̈ʒu
#define FRAME_WIDTH	(37.0f)	// �t���[���̉���
#define FRAME_HEIGHT	(292.0f)	// �t���[���̏c�̒���
#define FRAME_POS	(D3DXVECTOR3(1178.0f,430.0f,0.0f))	// �t���[���̈ʒu
#define CAPTION_WIDTH	(15.0f)	// ���o���̉���
#define CAPTION_HEIGTH	(75.0f)	// ���o���̏c�̒���
#define CAPTION_PATH	"data\\TEXTURE\\UI\\caption00.png"	// ���o���̃p�X
#define FRAME_PATH	"data\\TEXTURE\\UI\\frame03.png"	// �t���[���̃p�X

//=====================================================
// �R���X�g���N�^
//=====================================================
CBoost::CBoost(int nPriority) : CObject(nPriority)
{
	m_pObjectGauge = nullptr;
	m_pCaption = nullptr;
	m_posFrame = { 0.0f,0.0f,0.0f };
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
	m_fWidthFrame = FRAME_WIDTH;
	m_fHeightFrame = FRAME_HEIGHT;
	m_posFrame = FRAME_POS;

	if (m_pObjectGauge == nullptr)
	{// �u�[�X�g�Q�[�W�̐���
		m_pObjectGauge = CObject2D::Create(7);

		if (m_pObjectGauge != nullptr)
		{
			m_pObjectGauge->SetPosition(D3DXVECTOR3(GAUGE_POS.x, GAUGE_POS.y, 0.0f));
			m_pObjectGauge->SetSize(GAUGE_WIDTH, GAUGE_HEIGTH);
			m_pObjectGauge->SetVtx();
		}
	}

	if (m_pCaption == nullptr)
	{// ���o���̐���
		m_pCaption = CObject2D::Create(7);

		if (m_pCaption != nullptr)
		{
			m_pCaption->SetPosition(D3DXVECTOR3(FRAME_POS.x + FRAME_WIDTH + CAPTION_WIDTH, FRAME_POS.y + CAPTION_HEIGTH * 2, 0.0f));
			m_pCaption->SetSize(CAPTION_WIDTH, CAPTION_HEIGTH);
			m_pCaption->SetVtx();

			// �e�N�X�`���ԍ��擾
			int nIdx = CManager::GetTexture()->Regist(CAPTION_PATH);

			m_pCaption->SetIdxTexture(nIdx);
		}
	}

	if (m_pFrame == nullptr)
	{// �t���[���̐���
		m_pFrame = CObject2D::Create(7);

		if (m_pFrame != nullptr)
		{
			m_pFrame->SetPosition(D3DXVECTOR3(FRAME_POS.x, FRAME_POS.y, 0.0f));
			m_pFrame->SetSize(FRAME_WIDTH, FRAME_HEIGHT);
			m_pFrame->SetVtx();

			// �e�N�X�`���ԍ��擾
			int nIdx = CManager::GetTexture()->Regist(FRAME_PATH);

			m_pFrame->SetIdxTexture(nIdx);
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
		m_pObjectGauge->Uninit();
		m_pObjectGauge = nullptr;
	}

	if (m_pCaption != nullptr)
	{// ���o���̔j��
		m_pCaption->Uninit();
		m_pCaption = nullptr;
	}

	if (m_pFrame != nullptr)
	{// �t���[���̔j��
		m_pFrame->Uninit();
		m_pFrame = nullptr;
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
	CPlayer *pPlayer = nullptr;
	CObjectManager *pObjManager = CManager::GetObjectManager();

	if (pObjManager != nullptr)
	{// �v���C���[�̓K�p
		pPlayer = pObjManager->GetPlayer();
	}

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
// �`�揈��
//=====================================================
void CBoost::Draw(void)
{
	//CManager::GetDebugProc()->Print("\n�t���[���̈ʒu�F[%f,%f,%f]", m_posFrame.x, m_posFrame.y, m_posFrame.z);
	//CManager::GetDebugProc()->Print("\n�t���[���̑傫���F[%f,%f]", m_fWidthFrame, m_fHeightFrame);
}

//=====================================================
// �ʒu�ݒ�
//=====================================================
void CBoost::Edit(void)
{
	CInputKeyboard *pKeyboard = CManager::GetKeyboard();

	if (pKeyboard == nullptr)
	{
		return;
	}

	if (pKeyboard->GetPress(DIK_LSHIFT))
	{
		if (pKeyboard->GetPress(DIK_UP))
		{
			m_fHeightFrame += 2.0f;
		}
		if (pKeyboard->GetPress(DIK_DOWN))
		{
			m_fHeightFrame -= 2.0f;
		}

		if (pKeyboard->GetPress(DIK_RIGHT))
		{
			m_fWidthFrame += 2.0f;
		}
		if (pKeyboard->GetPress(DIK_LEFT))
		{
			m_fWidthFrame -= 2.0f;
		}
	}
	else
	{
		if (pKeyboard->GetPress(DIK_UP))
		{
			m_posFrame.y -= 2.0f;
		}
		if (pKeyboard->GetPress(DIK_DOWN))
		{
			m_posFrame.y += 2.0f;
		}

		if (pKeyboard->GetPress(DIK_RIGHT))
		{
			m_posFrame.x += 2.0f;
		}
		if (pKeyboard->GetPress(DIK_LEFT))
		{
			m_posFrame.x -= 2.0f;
		}
	}

	if (m_pFrame != nullptr)
	{// �t���[���̍X�V
		m_pFrame->SetSize(m_fWidthFrame,m_fHeightFrame);
		m_pFrame->SetPosition(m_posFrame);
		m_pFrame->SetVtx();
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