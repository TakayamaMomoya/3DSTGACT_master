//*****************************************************
//
// �`���[�g���A���Ǘ�[tutorialmanager.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "tutorialmanager.h"
#include "manager.h"
#include "debugproc.h"
#include "object2D.h"
#include "texture.h"
#include "inputkeyboard.h"
#include "inputjoypad.h"
#include "fade.h"
#include "enemymove.h"

//*****************************************************
// �}�N����`
//*****************************************************
#define MOVE_PROGRESS	(120)	// �ړ������o����t���[����
#define READ_PROGRESS	(240)	// �ǂނ����̃t���[����
#define IN_SPEED	(0.03f)	// �t�F�[�h�C���̃X�s�[�h
#define OUT_SPEED	(0.07f)	// �t�F�[�h�A�E�g�̃X�s�[�h
#define TIME_FADE	(60)	// �t�F�[�h�A�E�g�܂ł̃t���[����
#define RAPID_NUM	(10)	// �A�˂̐i�ނ܂ł̐�
#define FRAME_PATH	"data\\TEXTURE\\UI\\frame05.png"	// �X�L�b�v�̃t���[���̃p�X
#define GAUGE_WIDTH	(136.0f)	// �Q�[�W�̉���
#define GAUGE_HEIGTH	(19.0f)	// �Q�[�W�̏c�̒���
#define GAUGE_POS	(D3DXVECTOR3(10.0f,637.0f,0.0f))	// �Q�[�W�̈ʒu
#define FRAME_WIDTH	(150.0f)	// �t���[���̉���
#define FRAME_HEIGHT	(45.0f)	// �t���[���̏c�̒���
#define FRAME_POS	(D3DXVECTOR3(155.0f,620.0f,0.0f))	// �t���[���̈ʒu
#define TIME_SKIP	(120)	// �X�L�b�v�܂ł̃t���[����

//=====================================================
// �R���X�g���N�^
//=====================================================
CTutorialManager::CTutorialManager(int nPriority)
{
	m_nCntProgress = 0;
	m_nCntSkip = 0;
	m_progress = PROGRESS_START;
	m_pCaption = nullptr;
	m_pFrameSkip = nullptr;
	m_pGaugeSkip = nullptr;
	m_state = STATE_NONE;
}

//=====================================================
// �f�X�g���N�^
//=====================================================
CTutorialManager::~CTutorialManager()
{

}

//=====================================================
// ����������
//=====================================================
HRESULT CTutorialManager::Init(void)
{
	m_NextProgress = m_progress;
	m_state = STATE_IN;

	// �Q�[�W�̐���
	CreateGauge();

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CTutorialManager::Uninit(void)
{
	if (m_pCaption != nullptr)
	{
		m_pCaption->Uninit();
		m_pCaption = nullptr;
	}

	if (m_pFrameSkip != nullptr)
	{
		m_pFrameSkip->Uninit();
		m_pFrameSkip = nullptr;
	}

	if (m_pGaugeSkip != nullptr)
	{
		m_pGaugeSkip->Uninit();
		m_pGaugeSkip = nullptr;
	}

	Release();
}

//=====================================================
// �X�V����
//=====================================================
void CTutorialManager::Update(void)
{
	// �X�L�b�v�Q�[�W�Ǘ�
	ManageGauge();

	// ��ԊǗ�
	ManageState();

	// ���Ԍo�߂ɂ��`���[�g���A���̐i�s
	TimeTutorial();
	
	if (m_progress >= PROGRESS_SHOT && CEnemy::GetNumAll() == 0)
	{// �ˌ��ȍ~�͓G���o��
		CEnemy::Create(D3DXVECTOR3(500.0f, 0.0f, 0.0f), CEnemy::TYPE_TUTORIAL);
		CEnemy::Create(D3DXVECTOR3(-500.0f, 0.0f, 0.0f), CEnemy::TYPE_TUTORIAL);
	}
}

//=====================================================
// �Q�[�W�Ǘ�
//=====================================================
void CTutorialManager::ManageGauge(void)
{
	if (m_pGaugeSkip == nullptr)
	{
		return;
	}

	CInputKeyboard *pKeyboard = CManager::GetKeyboard();
	CInputJoypad *pJoypad = CManager::GetJoypad();

	if (pKeyboard == nullptr || pJoypad == nullptr)
	{
		return;
	}

	if (m_nCntSkip != TIME_SKIP)
	{
		if (pKeyboard->GetPress(DIK_TAB) ||
			pJoypad->GetPress(CInputJoypad::PADBUTTONS_BACK, 0))
		{// �X�L�b�v�J�E���^�[
			m_nCntSkip++;

			if (m_nCntSkip >= TIME_SKIP)
			{
				m_nCntSkip = TIME_SKIP;

				CFade *pFade = CManager::GetFade();

				if (pFade != nullptr)
				{
					pFade->SetFade(CScene::MODE_GAME);
				}
			}
		}
		else
		{
			m_nCntSkip = 0;
		}
	}

	float fWidth = (float)m_nCntSkip / (float)TIME_SKIP * GAUGE_WIDTH;

	// �ʒu�ݒ�
	D3DXVECTOR3 pos = { GAUGE_POS.x + fWidth,GAUGE_POS.y,0.0f };

	// �T�C�Y�ݒ�
	m_pGaugeSkip->SetPosition(pos);
	m_pGaugeSkip->SetSize(fWidth, GAUGE_HEIGTH);
	m_pGaugeSkip->SetVtx();
}

//=====================================================
// ��ԊǗ�
//=====================================================
void CTutorialManager::ManageState(void)
{
	if (m_pCaption == nullptr)
	{
		return;
	}

	// �F�̎擾
	D3DXCOLOR col = m_pCaption->GetCol();

	switch (m_state)
	{
	case CTutorialManager::STATE_IN:
		col.a += IN_SPEED;

		if (col.a > 1.0f)
		{
			m_state = STATE_WAIT;
		}

		break;
	case CTutorialManager::STATE_WAIT:

		break;
	case CTutorialManager::STATE_OUT:
		col.a -= OUT_SPEED;

		if (col.a <= 0.0f)
		{// ���̃L���v�V�����ݒ�
			NextProgress(m_NextProgress);
		}

		break;
	default:
		break;
	}

	if (m_pCaption != nullptr)
	{
		m_pCaption->SetCol(col);
	}
}

//=====================================================
// ���Ԍo�߂ɂ��`���[�g���A���̐i�s
//=====================================================
void CTutorialManager::TimeTutorial(void)
{
	CInputKeyboard *pKeyboard = CManager::GetKeyboard();
	CInputJoypad *pJoypad = CManager::GetJoypad();
	CFade *pFade = CManager::GetFade();

	if (m_progress == PROGRESS_START ||
		m_progress == PROGRESS_PARAM ||
		m_progress == PROGRESS_ASSESS)
	{
		if (ButtonCheck())
		{// �{�^������������i�s
			AddProgress(ACTION_BUTTON);
		}
	}

	if (pKeyboard != nullptr && pJoypad != nullptr && m_progress == PROGRESS_FREE)
	{
		if (pKeyboard->GetTrigger(DIK_RETURN))
		{// ��ʑJ��
			if (pFade != nullptr)
			{
				pFade->SetFade(CScene::MODE_GAME);
			}
		}
	}
}

//=====================================================
// �i�s�󋵂̐i�s
//=====================================================
void CTutorialManager::AddProgress(ACTION action)
{
	switch (m_progress)
	{
	case CTutorialManager::PROGRESS_START:	// �X�^�[�g���

		if (action == ACTION_BUTTON)
		{// ���̐i�s�󋵂Ɉڍs
			EndProgress(PROGRESS_MOVE);
		}

		break;
	case CTutorialManager::PROGRESS_MOVE:	// �ړ��`���[�g���A��

		if (action == ACTION_MOVE)
		{
			m_nCntProgress++;
		}

		if (m_nCntProgress > MOVE_PROGRESS)
		{// ���̐i�s�󋵂Ɉڍs
			EndProgress(PROGRESS_BOOST);
		}

		break;
	case CTutorialManager::PROGRESS_BOOST:	// �u�[�X�g�`���[�g���A��

		if (action == ACTION_BOOST)
		{
			m_nCntProgress++;
		}

		if (m_nCntProgress > MOVE_PROGRESS)
		{// ���̐i�s�󋵂Ɉڍs
			EndProgress(PROGRESS_SHOT);
		}

		break;
	case CTutorialManager::PROGRESS_SHOT:	// �ˌ��`���[�g���A��

		if (action == ACTION_SHOT)
		{// �`���[�W�V���b�g�֗U��
			EndProgress(PROGRESS_RAPID);
		}
		break;
	case CTutorialManager::PROGRESS_RAPID:	// �A�ˎˌ��`���[�g���A��

		if (action == ACTION_RAPID)
		{
			m_nCntProgress++;
		}

		if (m_nCntProgress > RAPID_NUM)
		{// ���̐i�s�󋵂Ɉڍs
			EndProgress(PROGRESS_CHARGE);
		}
		break;
	case CTutorialManager::PROGRESS_CHARGE:	// �`���[�W�ˌ��`���[�g���A��

		if (action == ACTION_CHARGE)
		{// �`���[�W�V���b�g�֗U��
			EndProgress(PROGRESS_PARAM);
		}
		break;
	case CTutorialManager::PROGRESS_PARAM:	// �p�����[�^�[

		if (action == ACTION_BUTTON)
		{// ���̐i�s�󋵂Ɉڍs
			EndProgress(PROGRESS_ASSESS);
		}

		break;
	case CTutorialManager::PROGRESS_ASSESS:	// �]��

		if (action == ACTION_BUTTON)
		{// ���̐i�s�󋵂Ɉڍs
			EndProgress(PROGRESS_FREE);
		}

		break;
	case CTutorialManager::PROGRESS_MAX:
		break;
	default:
		break;
	}
}

//=====================================================
// �i�s�̏I��
//=====================================================
void CTutorialManager::EndProgress(PROGRESS progress)
{
	m_nCntProgress = 0;

	m_NextProgress = progress;

	m_state = STATE_OUT;
}

//=====================================================
// �L���v�V�����̐؂�ւ�
//=====================================================
void CTutorialManager::NextProgress(PROGRESS progress)
{
	if (m_pCaption == nullptr)
	{
		return;
	}

	char *aPath[PROGRESS_MAX] =
	{
		"data\\TEXTURE\\UI\\tutorial00.png",
		"data\\TEXTURE\\UI\\tutorial01.png",
		"data\\TEXTURE\\UI\\tutorial02.png",
		"data\\TEXTURE\\UI\\tutorial03.png",
		"data\\TEXTURE\\UI\\tutorial04.png",
		"data\\TEXTURE\\UI\\tutorial05.png",
		"data\\TEXTURE\\UI\\tutorial06.png",
		"data\\TEXTURE\\UI\\tutorial07.png",
		"data\\TEXTURE\\UI\\tutorial08.png",
	};

	// �e�N�X�`���ԍ��擾
	int nIdx = CManager::GetTexture()->Regist(aPath[progress]);

	m_pCaption->SetIdxTexture(nIdx);

	m_progress = m_NextProgress;

	// �C����Ԃɂ���
	m_state = STATE_IN;
}

//=====================================================
// �{�^�������������̃`�F�b�N
//=====================================================
bool CTutorialManager::ButtonCheck(void)
{
	CInputKeyboard *pKeyboard = CManager::GetKeyboard();
	CInputJoypad *pJoypad = CManager::GetJoypad();

	if (pKeyboard == nullptr || pJoypad == nullptr)
	{
		return false;
	}

	if (pKeyboard->GetTrigger(DIK_F) ||
		pJoypad->GetTrigger(CInputJoypad::PADBUTTONS_START, 0))
	{// �`���[�g���A����i�߂�
		return true;
	}

	return false;
}

//=====================================================
// ��������
//=====================================================
CTutorialManager *CTutorialManager::Create(void)
{
	CTutorialManager *pTutorialManager = nullptr;

	if (pTutorialManager == nullptr)
	{// �C���X�^���X����
		pTutorialManager = new CTutorialManager;

		if (pTutorialManager != nullptr)
		{
			// ����������
			pTutorialManager->Init();

			pTutorialManager->m_pCaption = CObject2D::Create(6);

			if (pTutorialManager->m_pCaption != nullptr)
			{// ���o���̐���
				pTutorialManager->m_pCaption->SetPosition(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 600.0f, 0.0f));
				pTutorialManager->m_pCaption->SetSize(340.0f, 80.0f);

				// �����̖ڕW�ʒu�Ɠ�������
				pTutorialManager->m_posDest = pTutorialManager->m_pCaption->GetPosition();

				pTutorialManager->m_pCaption->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));

				pTutorialManager->NextProgress(PROGRESS_START);

				pTutorialManager->m_pCaption->SetVtx();
			}
		}
	}

	return pTutorialManager;
}

//=====================================================
// �Q�[�W�̐���
//=====================================================
void CTutorialManager::CreateGauge(void)
{
	if (m_pGaugeSkip == nullptr)
	{// �Q�[�W�̐���
		m_pGaugeSkip = CObject2D::Create(7);

		if (m_pGaugeSkip != nullptr)
		{
			m_pGaugeSkip->SetPosition(D3DXVECTOR3(GAUGE_POS.x, GAUGE_POS.y, 0.0f));
			m_pGaugeSkip->SetSize(GAUGE_WIDTH, GAUGE_HEIGTH);
			m_pGaugeSkip->SetCol(D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f));
			m_pGaugeSkip->SetVtx();
		}
	}

	if (m_pFrameSkip == nullptr)
	{// �t���[���̐���
		m_pFrameSkip = CObject2D::Create(7);

		if (m_pFrameSkip != nullptr)
		{
			m_pFrameSkip->SetPosition(D3DXVECTOR3(FRAME_POS.x, FRAME_POS.y, 0.0f));
			m_pFrameSkip->SetSize(FRAME_WIDTH, FRAME_HEIGHT);
			m_pFrameSkip->SetVtx();

			// �e�N�X�`���ԍ��擾
			int nIdx = CManager::GetTexture()->Regist(FRAME_PATH);

			m_pFrameSkip->SetIdxTexture(nIdx);
		}
	}
}

//=====================================================
// �`�揈��
//=====================================================
void CTutorialManager::Draw(void)
{
#ifdef  _DEBUG
	Debug();
#endif //  _DEBUG
}

//=====================================================
// �f�o�b�O�\��
//=====================================================
void CTutorialManager::Debug(void)
{
	CDebugProc *pDebugProc = CManager::GetDebugProc();

	if (pDebugProc == nullptr)
	{
		return;
	}

	pDebugProc->Print("\n���݂̐i�s[%d]", m_progress);
	pDebugProc->Print("\n���̐i�s[%d]", m_NextProgress);
	pDebugProc->Print("\n�X�L�b�v�J�E���^�[[%d]",m_nCntSkip);
}