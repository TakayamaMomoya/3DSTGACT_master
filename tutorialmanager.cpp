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

//*****************************************************
// �}�N����`
//*****************************************************
#define MOVE_PROGRESS	(120)	// �ړ������o����t���[����

//=====================================================
// �R���X�g���N�^
//=====================================================
CTutorialManager::CTutorialManager(int nPriority)
{
	m_nCntProgress = 0;
	m_progress = PROGRESS_START;
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
	m_progress = PROGRESS_MOVE;

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CTutorialManager::Uninit(void)
{
	Release();
}

//=====================================================
// �X�V����
//=====================================================
void CTutorialManager::Update(void)
{

}

//=====================================================
// �i�s�󋵂̐i�s
//=====================================================
void CTutorialManager::AddProgress(ACTION action)
{
	switch (m_progress)
	{
	case CTutorialManager::PROGRESS_START:
		break;
	case CTutorialManager::PROGRESS_MOVE:

		if (action == ACTION_MOVE)
		{
			m_nCntProgress++;
		}

		if (m_nCntProgress > MOVE_PROGRESS)
		{// ���̐i�s�󋵂Ɉڍs
			m_nCntProgress = 0;

			m_progress = PROGRESS_SHOT;
		}

		break;
	case CTutorialManager::PROGRESS_SHOT:

		if (action == ACTION_SHOT)
		{// �`���[�W�V���b�g�֗U��
			m_progress = PROGRESS_CHARGE;
		}

		break;
	case CTutorialManager::PROGRESS_MAX:
		break;
	default:
		break;
	}
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

		// ����������
		pTutorialManager->Init();
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

	pDebugProc->Print("\n���݂̐i�s[%d]",m_progress);
}