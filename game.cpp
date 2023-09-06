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
#include "game.h"
#include "player.h"
#include "bullet.h"
#include "explosion.h"
#include "enemy.h"
#include "score.h"
#include "timer.h"
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
#include "assess.h"

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
CPlayer *CGame::m_pPlayer = nullptr;		// �v���C���[�̃|�C���^
CMeshField *CGame::m_pMeshField = nullptr;	// ���b�V���t�B�[���h�̃|�C���^
CScore *CGame::m_pScore = nullptr;	// �X�R�A�̃|�C���^
CTimer *CGame::m_pTimer = nullptr;	// �^�C�}�[�̃|�C���^
CDefend *CGame::m_pDefend = nullptr;	// ��q�Ώۂւ̃|�C���^
CEdit *CGame::m_pEdit = nullptr;	// �G�f�B�^�[�ւ̃|�C���^
CGame::STATE CGame::m_state = STATE_NONE;
bool CGame::m_bPause = false;
CPause *CGame::m_pPause = nullptr;	// �|�[�Y�̃A�h���X

//=====================================================
// �R���X�g���N�^
//=====================================================
CGame::CGame()
{
	ZeroMemory(&m_apMeshWall[0],sizeof(m_apMeshWall));
	m_bPhoto = false;
	m_bPause = false;
	m_nTimerWave = 0;
	m_nCntSecond = 0;
}

//=====================================================
// �f�X�g���N�^
//=====================================================
CGame::~CGame()
{

}

//=====================================================
// ����������
//=====================================================
HRESULT CGame::Init(void)
{
	if (m_pPlayer == nullptr)
	{// �v���C���[����
		m_pPlayer = CPlayer::Create();
	}

	// �X�R�A�Ǎ�����
	CScore::Load();

	if (m_pScore == nullptr)
	{// �X�R�A����
		m_pScore = CScore::Create();
	}

	if (m_pScore != nullptr)
	{// �X�R�A������
		m_pScore->Init();
	}

	if (m_pTimer == nullptr)
	{// �^�C�}�[����
		m_pTimer = CTimer::Create();
	}

	if (m_pTimer != nullptr)
	{// �^�C�}�[������
		m_pTimer->Init();
	}

	// �G���Ǎ�
	CEnemy::Load();

	// �u���b�N�ԍ��Ǎ�
	CBlock::LoadModel();

#if _DEBUG
	if (m_pEdit == nullptr)
	{// �G�f�B�^�[����
		m_pEdit = new CEdit;
	}

	if (m_pEdit != nullptr)
	{// �G�f�B�^�[������
		m_pEdit->Init();
	}
#endif

	if (m_pDefend == nullptr)
	{// ��q�Ώې���
		m_pDefend = CDefend::Create();

		if (m_pDefend != nullptr)
		{
			// ������
			if (FAILED(m_pDefend->Init()))
			{// �������Ɏ��s�����ꍇ
				return E_FAIL;
			}
		}
	}

	// �e�Ǎ�����
	CBullet::Load();

	// �����Ǎ�����
	CExplosion::Load();

	// �u���b�N�Ǎ�����
	CBlock::Load();

	// �p�[�e�B�N���̓Ǎ�
	CParticle::Load();

	// �X�J�C�{�b�N�X����
	CSkybox::Create();

	// ���b�V���t�B�[���h�̐���
	m_pMeshField = CMeshField::Create();

	// �n���{�e���̐���
	CField *pField = nullptr;
	pField = CField::Create();

	if (pField != nullptr)
	{
		pField->SetPosition(D3DXVECTOR3(0.0f, -300.0f, 0.0f));
		pField->SetSize(5000000.0f, 5000000.0f);
	}

	// ���b�V���E�H�[���̐���
	m_apMeshWall[WALLTYPE_FRONT] = CMeshWall::Create(D3DXVECTOR3(0.0f, 0.0f, -10000.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f));
	m_apMeshWall[WALLTYPE_REAR] = CMeshWall::Create(D3DXVECTOR3(0.0f, 0.0f, 10000.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_apMeshWall[WALLTYPE_RIGHT] = CMeshWall::Create(D3DXVECTOR3(10000.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f));
	m_apMeshWall[WALLTYPE_LEFT] = CMeshWall::Create(D3DXVECTOR3(-10000.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, -D3DX_PI * 0.5f, 0.0f));

	m_state = STATE_NORMAL;

	// �t�H�O������
	CRenderer::EnableFog(true);

	// �o�Ǘ��̐���
	CDustManager::Create();

	// �G����
	CEnemy::SpawnEnemy(3);

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CGame::Uninit(void)
{
	if (m_pPause != nullptr)
	{
		m_pPause = nullptr;
	}

	if (m_pPlayer != nullptr)
	{// �v���C���[�̏I���E�j��
		m_pPlayer = nullptr;
	}

	if (m_pDefend != nullptr)
	{// ��q�Ώۂ̏I���E�j��
		m_pDefend->Uninit();

		delete m_pDefend;
		m_pDefend = nullptr;
	}

	if (m_pMeshField != nullptr)
	{
		m_pMeshField->Uninit();

		m_pMeshField = nullptr;
	}

	if (m_pScore != nullptr)
	{// �X�R�A�̏I���E�j��
		m_pScore->Uninit();

		m_pScore = nullptr;
	}

	if (m_pTimer != nullptr)
	{// �^�C�}�[�̏I���E�j��
		m_pTimer->Uninit();

		m_pTimer = nullptr;
	}

#if _DEBUG
	if (m_pEdit != nullptr)
	{// �G�f�B�^�[�̏I���E�j��
		m_pEdit->Uninit();

		delete m_pEdit;
		m_pEdit = nullptr;
	}
#endif

	// �u���b�N���폜
	CBlock::DeleteAll();

	// �u���b�N�ԍ��폜
	CBlock::DeleteIdx();

	// �e�j��
	CBullet::Unload();

	// �v���C���[�j��
	CPlayer::Unload();

	// �����j��
	CExplosion::Unload();

	// �G�j��
	CEnemy::Unload();

	// �X�R�A�j������
	CScore::Unload();

	// �I�u�W�F�N�g�S�j��
	CObject::ReleaseAll();
}

//=====================================================
// �v���C���[�j������
//=====================================================
void CGame::ReleasePlayer(void)
{
	if (m_pPlayer != nullptr)
	{
		m_pPlayer = nullptr;
	}
}

//=====================================================
// �X�R�A�I������
//=====================================================
void CGame::ReleaseScore(void)
{
	if (m_pScore != nullptr)
	{// �X�R�A�̏I���E�j��
		m_pScore = nullptr;
	}
}

//=====================================================
// �^�C�}�[�I������
//=====================================================
void CGame::ReleaseTimer(void)
{
	if (m_pTimer != nullptr)
	{// �X�R�A�̏I���E�j��
		m_pTimer = nullptr;
	}
}

//=====================================================
// ��q�ΏۏI������
//=====================================================
void CGame::ReleaseDefend(void)
{
	// �t�F�[�h�擾
	CFade *pFade = CManager::GetFade();

	if (m_pDefend != nullptr)
	{// ��q�Ώۂ̏I���E�j��
		m_pDefend = nullptr;
	}

	if (m_state == STATE_NORMAL)
	{
		if (pFade != nullptr)
		{
			m_state = STATE_END;
		}
	}
}

//=====================================================
// �X�V����
//=====================================================
void CGame::Update(void)
{
	// ���͎擾
	CInputKeyboard *pKeyboard = CManager::GetKeyboard();
	CInputJoypad *pJoypad = CManager::GetJoypad();

	CCamera *pCamera = CManager::GetCamera();

	if (m_bPause == false)
	{
		if (m_bPhoto == false)
		{
			// �V�[���̍X�V
			CScene::Update();
		}

		// �J�����X�V
		if (pCamera != nullptr)
		{
			if (CGame::IsPhoto())
			{
				pCamera->Control();
			}
			else
			{
				// �Ǐ]����
				pCamera->FollowPlayer();
			}

			// �J�����h��̏���
			pCamera->Quake();
		}
	}

	m_nCntSecond++;

	if (m_nCntSecond >= 60)
	{
		m_nTimerWave++;

		m_nCntSecond = 0;
	}

	if (CEnemy::GetNumAll() == 0)
	{// �G�X�|�[��
		CEnemy::SpawnEnemy(3);

		TimeBonus();

		m_nTimerWave = 0;
	}

#ifdef _DEBUG
	if (m_pEdit != nullptr)
	{
		m_pEdit->Update();
	}

	if (pKeyboard != nullptr)
	{
		if (pKeyboard->GetTrigger(DIK_F))
		{// �t�H�g���[�h�N��
			m_bPhoto = m_bPhoto ? false : true;
		}
	}
#endif

	// ��ԊǗ�
	ManageState();

	// �v���C���[�ړ���������
	LimitPlayerPos();

	// �ǂ̊Ǘ�
	ManageWall();

	if (pKeyboard != nullptr)
	{
		if (pKeyboard->GetTrigger(DIK_P) || pJoypad->GetTrigger(CInputJoypad::PADBUTTONS_START, 0))
		{// �|�[�Y�؂�ւ�
			TogglePause(m_pPause);
		}
	}

	if (m_pPause != nullptr)
	{// �|�[�Y�X�V
		m_pPause->Update();
	}

#ifdef _DEBUG	// �f�o�b�O����
	Debug();
#endif
}

//=====================================================
// �v���C���[�ʒu��������
//=====================================================
void CGame::LimitPlayerPos(void)
{
	D3DXVECTOR3 pos = { 0.0f,0.0f,0.0f };
	D3DXVECTOR3 move = { 0.0f,0.0f,0.0f };

	if (m_pPlayer != nullptr)
	{
		pos = m_pPlayer->GetPosition();
		move = m_pPlayer->GetMove();
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

	m_pPlayer->SetPosition(pos);
	m_pPlayer->SetMove(move);
}

//=====================================================
// �|�[�Y�؂�ւ�����
//=====================================================
void CGame::TogglePause(CPause *pPause)
{
	m_bPause = m_bPause ? false : true;

	if (m_bPause)
	{// �|�[�Y����
		if (m_pPause == nullptr)
		{
			m_pPause = CPause::Create();
		}
	}
	else
	{// �|�[�Y�j��
		if (m_pPause != nullptr)
		{
			m_pPause->Uninit();

			m_pPause = nullptr;
		}
	}
}

//=====================================================
// �ǂ̊Ǘ�
//=====================================================
void CGame::ManageWall(void)
{
	D3DXVECTOR3 pos = { 0.0f,0.0f,0.0f };

	if (m_pPlayer != nullptr)
	{
		pos = m_pPlayer->GetPosition();
	}

	if (pos.x > LIMIT_LENGTH - CHENGE_LENGTH)
	{// �E�̕ǂ̔���
		if (m_apMeshWall[WALLTYPE_RIGHT] != nullptr)
		{
			m_apMeshWall[WALLTYPE_RIGHT]->CheckPlayer();
		}
	}

	if (pos.x < -LIMIT_LENGTH + CHENGE_LENGTH)
	{// ���̕ǂ̔���
		if (m_apMeshWall[WALLTYPE_LEFT] != nullptr)
		{
			m_apMeshWall[WALLTYPE_LEFT]->CheckPlayer();
		}
	}

	if (pos.z < -LIMIT_LENGTH + CHENGE_LENGTH)
	{// �O�̕ǂ̔���
		if (m_apMeshWall[WALLTYPE_FRONT] != nullptr)
		{
			m_apMeshWall[WALLTYPE_FRONT]->CheckPlayer();
		}
	}

	if (pos.z > LIMIT_LENGTH - CHENGE_LENGTH)
	{// ���̕ǂ̔���
		if (m_apMeshWall[WALLTYPE_REAR] != nullptr)
		{
			m_apMeshWall[WALLTYPE_REAR]->CheckPlayer();
		}
	}
}

//=====================================================
// ��ԊǗ�
//=====================================================
void CGame::ManageState(void)
{
	// �t�F�[�h�擾
	CFade *pFade = CManager::GetFade();

	int nTime = 0;

	switch (m_state)
	{
	case CGame::STATE_NORMAL:

		if (m_pTimer != nullptr)
		{
			if (m_pTimer->GetSecond() <= 0)
			{// �^�C���A�b�v�N���A
				// �X�R�A�ۑ�
				CManager::SetScore(m_pScore->GetScore());

				m_state = STATE_END;
			}
		}

		if (m_pPlayer == nullptr)
		{
			// �X�R�A���Z�b�g
			CManager::SetScore(0);

			m_state = STATE_END;
		}

		break;
	case CGame::STATE_END:

		// �J�ڃ^�C�}�[�J�E���g
		nTime = CScene::GetTimer() + 1;

		SetTimer(nTime);

		if (nTime >= TRANS_TIME)
		{
			m_state = STATE_NONE;

			if (pFade != nullptr)
			{
				// ��ʑJ��
				pFade->SetFade(CScene::MODE_RANKING);
			}
		}

		break;
	default:
		break;
	}
}

//=====================================================
// �^�C���{�[�i�X�̊Ǘ�
//=====================================================
void CGame::TimeBonus(void)
{
	CAssess *pAssess;

	int nBonus = BONUS_TIME - m_nTimerWave;

	if (m_pPlayer != nullptr)
	{
		pAssess = m_pPlayer->GetAssess();

		if (pAssess != nullptr)
		{
			pAssess->AddAttack((float)nBonus * RATE_BONUS);
		}
	}
}

//=====================================================
// �f�o�b�O����
//=====================================================
void CGame::Debug(void)
{
	// ���擾
	CInputKeyboard *pKeyboard = CManager::GetKeyboard();
	CFade *pFade = CManager::GetFade();

	if (pKeyboard != nullptr)
	{
		if (pKeyboard->GetTrigger(DIK_M))
		{// �h�q�ڕW�j��
			if (m_pDefend != nullptr)
			{
				m_pDefend->Hit(100000);
			}
		}

		if (pKeyboard->GetTrigger(DIK_RETURN))
		{// �J��
			if (pFade != nullptr)
			{
				//pFade->SetFade(MODE_RANKING);
			}
		}
	}
}

//=====================================================
// �`�揈��
//=====================================================
void CGame::Draw(void)
{

}