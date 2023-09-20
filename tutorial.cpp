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
#include "object.h"
#include "block.h"
#include "meshfield.h"
#include "universal.h"
#include "particle.h"
#include "inputkeyboard.h"
#include "inputjoypad.h"
#include "fade.h"
#include "camera.h"
#include "dustmanager.h"
#include "objectmanager.h"
#include "tutorialmanager.h"
#include "tutorialplayer.h"

//*****************************************************
// �}�N����`
//*****************************************************
#define TRANS_TIME	(60)	// �I���܂ł̗]�C�̃t���[����
#define LIMIT_LENGTH	(10000)	// �ړ������̋���
#define LIMIT_WIDTH	(100)	// ����v���C���[�̕�
#define LIMIT_HEIGHT	(2000)	// �������x
#define BONUS_TIME	(40)	// �{�[�i�X���t�^�����Œ���̃^�C��
#define RATE_BONUS	(0.015f)	// 1�b������̃^�C���{�[�i�X

//*****************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************
CTutorialManager *CTutorial::m_pTutorialManager = nullptr;	// �`���[�g���A���Ǘ��̃|�C���^

//=====================================================
// �R���X�g���N�^
//=====================================================
CTutorial::CTutorial()
{

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
	CMeshField *pMeshField = CMeshField::Create();
	CObjectManager *pObjManager = CManager::GetObjectManager();

	CTutorialPlayer *pPlayer = CTutorialPlayer::Create();

	if (pPlayer != nullptr && pObjManager != nullptr)
	{// �v���C���[�̓K�p
		pObjManager->BindPlayer((CPlayer*)pPlayer);
	}

	// �u���b�N�ԍ��Ǎ�
	CBlock::LoadModel();

	CBlock::Create(D3DXVECTOR3(2000.0f, 0.0f, 0.0f), CBlock::TYPE_BILL005);

	CBlock::Create(D3DXVECTOR3(-2000.0f, 0.0f, 0.0f), CBlock::TYPE_BILL005);

	CBlock::Create(D3DXVECTOR3(0.0f, 0.0f, 2000.0f), CBlock::TYPE_BILL005);

	// �p�[�e�B�N���̓Ǎ�
	CParticle::Load();

	// ���b�V���t�B�[���h�̐���
	if (pMeshField != nullptr && pObjManager != nullptr)
	{
		pObjManager->BindMeshField(pMeshField);

		pMeshField->SetIdxTexture(-1);

		pMeshField->SetCol(D3DXCOLOR(0.7f, 0.7f, 1.0f, 0.7f));

		pMeshField->Reset();

		pMeshField->SetNormal();

		pMeshField->EnableWire(true);
	}

	// �t�H�O������
	CRenderer::EnableFog(false);

	// �`���[�g���A���Ǘ��̐���
	if (m_pTutorialManager == nullptr)
	{
		m_pTutorialManager = CTutorialManager::Create();
	}

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

	if (pos.y < 0)
	{// ��
		pos.y = 0;

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
// �`�揈��
//=====================================================
void CTutorial::Draw(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
}