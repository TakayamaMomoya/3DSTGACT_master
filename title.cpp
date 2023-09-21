//*****************************************************
//
// �^�C�g������[title.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "title.h"
#include "object.h"
#include "inputkeyboard.h"
#include "inputmouse.h"
#include "inputjoypad.h"
#include "manager.h"
#include "motion.h"
#include "skybox.h"
#include "fade.h"
#include "texture.h"
#include "camera.h"
#include "renderer.h"
#include "menu.h"
#include "sound.h"

//*****************************************************
// �}�N����`
//*****************************************************
#define LOGO_PATH	"data\\TEXTURE\\UI\\logo000.png"	// ���S�̃p�X
#define START_PATH	"data\\TEXTURE\\UI\\start.png"	// �X�^�[�g�\���̃p�X
#define START_WIDTH	(200.0f)	// �X�^�[�g�\���̕�
#define START_HEIGHT	(50.0f)	// �X�^�[�g�\���̍���
#define DEST_WIDTH	(500.0f)	// �X�^�[�g�\���̕�
#define CHANGE_FACT	(0.1f)	// �ω�����W��
#define ALPHA_CHANGE	(0.05f)	// ���l�̕ω���

//=====================================================
// �R���X�g���N�^
//=====================================================
CTitle::CTitle()
{
	m_pMenu = nullptr;
	m_pStart = nullptr;
	m_state = STATE_NONE;
}

//=====================================================
// �f�X�g���N�^
//=====================================================
CTitle::~CTitle()
{

}

//=====================================================
// ����������
//=====================================================
HRESULT CTitle::Init(void)
{
	// ���S�̐���
	CObject2D *pObject2D = CObject2D::Create(7);
	pObject2D->SetSize(875.0f * 0.45f, 320.0f * 0.45f);
	pObject2D->SetPosition(D3DXVECTOR3(SCREEN_WIDTH * 0.7f, 200.0f, 0.0f));

	int nIdx = CManager::GetTexture()->Regist(LOGO_PATH);
	pObject2D->SetIdxTexture(nIdx);
	pObject2D->SetVtx();

	// �X�^�[�g�\���̐���
	m_pStart = CObject2D::Create(7);

	if (m_pStart != nullptr)
	{
		m_pStart->SetSize(START_WIDTH, START_HEIGHT);
		m_pStart->SetPosition(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.7f, 0.0f));

		int nIdx = CManager::GetTexture()->Regist(START_PATH);
		m_pStart->SetIdxTexture(nIdx);
		m_pStart->SetVtx();
	}

	// �w�i�I�u�W�F�N�g�̐���
	CMotion *pMotion = CMotion::Create("data\\MOTION\\motionArms01.txt");

	pMotion->SetPosition(D3DXVECTOR3(0.0f, -50.0f, 0.0f));

	pMotion->SetMatrix();

	CSkybox::Create();

	// �t�H�O��؂�
	CRenderer::EnableFog(false);

	CManager::GetSound()->Stop();

	// BGM�Đ�
	CManager::GetSound()->Play(CSound::LABEL_BGM000);

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CTitle::Uninit(void)
{
	if (m_pMenu != nullptr)
	{
		m_pMenu->Uninit();
		m_pMenu = nullptr;
	}

	// �I�u�W�F�N�g�S�j��
	CObject::ReleaseAll();
}

//=====================================================
// �X�V����
//=====================================================
void CTitle::Update(void)
{
	// ���擾
	CInputKeyboard *pKeyboard = CManager::GetKeyboard();
	CInputMouse *pMouse = CManager::GetMouse();
	CInputJoypad *pJoypad = CManager::GetJoypad();

	CCamera *pCamera = CManager::GetCamera();

	// �V�[���̍X�V
	CScene::Update();
	
	CFade *pFade = CManager::GetFade();

	if (m_state == STATE_NONE)
	{
		if (pKeyboard != nullptr && pMouse != nullptr)
		{
			if (pKeyboard->GetTrigger(DIK_RETURN) ||
				pMouse->GetTrigger(CInputMouse::BUTTON_LMB) ||
				pJoypad->GetTrigger(CInputJoypad::PADBUTTONS_A, 0))
			{// �X�^�[�g�\���������o��
				if (m_pMenu == nullptr)
				{
					m_state = STATE_OUT;

					// SE�Đ�
					CManager::GetSound()->Play(CSound::LABEL_ENTER);
				}
			}
		}
	}
	else if(m_state == STATE_OUT)
	{
		// �X�^�[�g�\���̊Ǘ�
		ManageStart();
	}

	// �J�����X�V
	if (pCamera != nullptr)
	{
		// �^�C�g���̓���
		pCamera->TitleMove();
	}
}

//=====================================================
// �X�^�[�g�\���̊Ǘ�
//=====================================================
void CTitle::ManageStart(void)
{
	if (m_pStart == nullptr)
	{
		return;
	}

	float fWidth = m_pStart->GetWidth();
	float fHeight = m_pStart->GetHeight();
	float fDiffWidth;
	float fDiffHeight;

	// �������̌v�Z
	fDiffWidth = (DEST_WIDTH - fWidth) * CHANGE_FACT;
	fDiffHeight = (0.0f - fHeight) * CHANGE_FACT;

	// �����ɂ���
	D3DXCOLOR col = m_pStart->GetCol();

	col.a -= ALPHA_CHANGE;

	if (col.a <= 0.0f)
	{
		col.a = 0.0f;

		CMenu::Create();

		m_state = STATE_MENU;
	}

	m_pStart->SetCol(col);

	// �T�C�Y�ݒ�
	m_pStart->SetSize(fWidth + fDiffWidth, fHeight + fDiffHeight);
	m_pStart->SetVtx();
}

//=====================================================
// �`�揈��
//=====================================================
void CTitle::Draw(void)
{

}