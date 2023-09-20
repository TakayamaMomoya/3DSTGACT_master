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

//=====================================================
// �R���X�g���N�^
//=====================================================
CTutorialManager::CTutorialManager(int nPriority)
{
	m_nCntProgress = 0;
	m_progress = PROGRESS_START;
	m_pCaption = nullptr;
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

	Release();
}

//=====================================================
// �X�V����
//=====================================================
void CTutorialManager::Update(void)
{
	// ��ԊǗ�
	ManageState();

	// ���Ԍo�߂ɂ��`���[�g���A���̐i�s
	TimeTutorial();
	
	if (m_progress >= PROGRESS_SHOT && CEnemy::GetNumAll() == 0)
	{// �ˌ��ȍ~�͓G���o��
		CEnemy::Create(D3DXVECTOR3(500.0f, 0.0f, 0.0f), CEnemy::TYPE_MOVE);
		CEnemy::Create(D3DXVECTOR3(-500.0f, 0.0f, 0.0f), CEnemy::TYPE_MOVE);
	}
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
	{// ���Ԍo��
		AddProgress(ACTION_TIME);
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

		if (action == ACTION_TIME)
		{
			m_nCntProgress++;
		}

		if (m_nCntProgress > READ_PROGRESS)
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

		if (action == ACTION_TIME)
		{
			m_nCntProgress++;
		}

		if (m_nCntProgress > READ_PROGRESS)
		{// ���̐i�s�󋵂Ɉڍs
			EndProgress(PROGRESS_ASSESS);
		}

		break;
	case CTutorialManager::PROGRESS_ASSESS:	// �]��

		if (action == ACTION_TIME)
		{
			m_nCntProgress++;
		}

		if (m_nCntProgress > READ_PROGRESS)
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
	pDebugProc->Print("\n���̐i�s[%d]",m_NextProgress);
}