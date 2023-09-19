//*****************************************************
//
// �Q�[������[game.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "manager.h"
#include "renderer.h"
#include "tutorial.h"
#include "player.h"
#include "enemy.h"
#include "score.h"
#include "block.h"
#include "object.h"
#include "defend.h"
#include "field.h"
#include "meshfield.h"
#include "skybox.h"
#include "universal.h"
#include "particle.h"
#include "edit.h"
#include "inputkeyboard.h"
#include "inputjoypad.h"
#include "fade.h"
#include "meshwall.h"
#include "camera.h"
#include "pause.h"
#include "dustmanager.h"
#include "objectmanager.h"

//*****************************************************
// �}�N����`
//*****************************************************
#define TRANS_TIME	(60)	// �I���܂ł̗]�C�̃t���[����
#define LIMIT_LENGTH	(10000)	// �ړ������̋���
#define LIMIT_WIDTH	(100)	// ����v���C���[�̕�
#define LIMIT_HEIGHT	(2000)	// �������x
#define BONUS_TIME	(40)	// �{�[�i�X���t�^�����Œ���̃^�C��
#define RATE_BONUS	(0.015f)	// 1�b������̃^�C���{�[�i�X

//=====================================================
// �R���X�g���N�^
//=====================================================
CTutorial::CTutorial()
{
	m_progress = PROGRESS_START;
}

//=====================================================
// �f�X�g���N�^
//=====================================================
CTutorial::~CTutorial()
{

}

//=====================================================
// ����������
//=====================================================
HRESULT CTutorial::Init(void)
{
	CPlayer *pPlayer = CPlayer::Create();
	CMeshField *pMeshField = CMeshField::Create();
	CObjectManager *pObjManager = CManager::GetObjectManager();

	if (pPlayer != nullptr && pObjManager != nullptr)
	{// �v���C���[�̓K�p
		pObjManager->BindPlayer(pPlayer);
	}

	// �G���Ǎ�
	CEnemy::Load();

	// �p�[�e�B�N���̓Ǎ�
	CParticle::Load();

	// �X�J�C�{�b�N�X����
	//CSkybox::Create();

	// ���b�V���t�B�[���h�̐���
	if (pMeshField != nullptr && pObjManager != nullptr)
	{
		pObjManager->BindMeshField(pMeshField);
	}

	// �t�H�O������
	CRenderer::EnableFog(false);

	// �o�Ǘ��̐���
	CDustManager::Create();

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CTutorial::Uninit(void)
{
	// �u���b�N���폜
	CBlock::DeleteAll();

	// �u���b�N�ԍ��폜
	CBlock::DeleteIdx();

	// �G�j��
	CEnemy::Unload();

	// �I�u�W�F�N�g�S��
	CObject::ReleaseAll();
}

//=====================================================
// �X�V����
//=====================================================
void CTutorial::Update(void)
{
	// ���͎擾
	CInputKeyboard *pKeyboard = CManager::GetKeyboard();
	CInputJoypad *pJoypad = CManager::GetJoypad();

	CCamera *pCamera = CManager::GetCamera();

	// �V�[���̍X�V
	CScene::Update();

	// �J�����X�V
	if (pCamera != nullptr)
	{
		// �Ǐ]����
		pCamera->FollowPlayer();


		// �J�����h��̏���
		pCamera->Quake();
	}

	// ��ԊǗ�
	ManageState();

	// �v���C���[�ړ���������
	LimitPlayerPos();

#ifdef _DEBUG	// �f�o�b�O����
	Debug();
#endif
}

//=====================================================
// �v���C���[�ʒu��������
//=====================================================
void CTutorial::LimitPlayerPos(void)
{
	D3DXVECTOR3 pos = { 0.0f,0.0f,0.0f };
	D3DXVECTOR3 move = { 0.0f,0.0f,0.0f };

	CPlayer *pPlayer = nullptr;
	CObjectManager *pObjManager = CManager::GetObjectManager();

	if (pObjManager != nullptr)
	{// �v���C���[�̓K�p
		pPlayer = pObjManager->GetPlayer();
	}

	if (pPlayer != nullptr)
	{
		pos = pPlayer->GetPosition();
		move = pPlayer->GetMove();
	}
	else
	{
		return;
	}

	if (pos.x > LIMIT_LENGTH - LIMIT_WIDTH)
	{// �E�̕�
		pos.x = LIMIT_LENGTH - LIMIT_WIDTH;
	}

	if (pos.x < -LIMIT_LENGTH + LIMIT_WIDTH)
	{// ���̕�
		pos.x = -LIMIT_LENGTH + LIMIT_WIDTH;
	}

	if (pos.z > LIMIT_LENGTH - LIMIT_WIDTH)
	{// �O�̕�
		pos.z = LIMIT_LENGTH - LIMIT_WIDTH;
	}

	if (pos.z < -LIMIT_LENGTH + LIMIT_WIDTH)
	{// ���̕�
		pos.z = -LIMIT_LENGTH + LIMIT_WIDTH;
	}

	if (pos.y > LIMIT_HEIGHT)
	{// �V��
		pos.y = LIMIT_HEIGHT;

		move.y = 0;
	}

	pPlayer->SetPosition(pos);
	pPlayer->SetMove(move);
}

//=====================================================
// ��ԊǗ�
//=====================================================
void CTutorial::ManageState(void)
{

}

//=====================================================
// �f�o�b�O����
//=====================================================
void CTutorial::Debug(void)
{

}

//=====================================================
// �i�s�󋵂̐i�s
//=====================================================
void CTutorial::AddProgress(ACTION action)
{
	switch (m_progress)
	{
	case CTutorial::PROGRESS_START:
		break;
	case CTutorial::PROGRESS_SHOT:
		break;
	case CTutorial::PROGRESS_MAX:
		break;
	default:
		break;
	}
}

//=====================================================
// �`�揈��
//=====================================================
void CTutorial::Draw(void)
{

}