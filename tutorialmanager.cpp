//*****************************************************
//
// �]���̏���[assess.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "tutorialmanager.h"

//=====================================================
// �R���X�g���N�^
//=====================================================
CTutorialManager::CTutorialManager(int nPriority)
{

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