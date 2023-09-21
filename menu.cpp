//*****************************************************
//
// ���j���[�̏���[menu.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "menu.h"
#include "manager.h"
#include "object2D.h"
#include "inputkeyboard.h"
#include "inputmouse.h"
#include "inputjoypad.h"
#include "texture.h"
#include "fade.h"
#include "game.h"

//*****************************************************
// �}�N����`
//*****************************************************
#define MENU_WIDTH	(140.0f)	// ���ڂ̕�
#define MENU_HEIGHT	(60.0f)	// ���ڂ̍���
#define CAPTION_WIDTH	(500.0f)	// ���o���̕�
#define CAPTION_HEIGHT	(250.0f)	// ���o���̍���
#define CAPTION_PATH "data\\TEXTURE\\UI\\caption01.png"	// ���o���̃p�X

//====================================================
// �R���X�g���N�^
//====================================================
CMenu::CMenu()
{
	m_menu = MENU_GAME;
	ZeroMemory(&m_apMenu[0], sizeof(m_apMenu));
	m_pCaption = nullptr;
}

//====================================================
// �f�X�g���N�^
//====================================================
CMenu::~CMenu()
{

}

//====================================================
// ��������
//====================================================
CMenu *CMenu::Create(void)
{
	CMenu *pMenu = nullptr;

	pMenu = new CMenu;

	if (pMenu != nullptr)
	{
		pMenu->Init();
	}

	return pMenu;
}

//====================================================
// ����������
//====================================================
HRESULT CMenu::Init(void)
{
	int nIdxTexture;
	char *pPath[MENU_MAX] = 
	{
		"data\\TEXTURE\\UI\\menu_yes.png",
		"data\\TEXTURE\\UI\\menu_no.png",
	};

	// ���o���̐���
	m_pCaption = CObject2D::Create(7);

	m_pCaption->SetPosition(D3DXVECTOR3(SCREEN_WIDTH * 0.5, SCREEN_HEIGHT * 0.5f, 0.0f));

	m_pCaption->SetSize(CAPTION_WIDTH, CAPTION_HEIGHT);

	m_pCaption->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	nIdxTexture = CManager::GetTexture()->Regist(CAPTION_PATH);

	m_pCaption->SetIdxTexture(nIdxTexture);

	m_pCaption->SetVtx();

	for (int nCntMenu = 0; nCntMenu < MENU_MAX; nCntMenu++)
	{// �I�����̐���
		if (m_apMenu[nCntMenu] == nullptr)
		{
			m_apMenu[nCntMenu] = CObject2D::Create(7);

			m_apMenu[nCntMenu]->SetPosition(D3DXVECTOR3(SCREEN_WIDTH * 0.4f + MENU_WIDTH * nCntMenu * 2, SCREEN_HEIGHT * 0.6f, 0.0f));

			m_apMenu[nCntMenu]->SetSize(MENU_WIDTH, MENU_HEIGHT);

			m_apMenu[nCntMenu]->SetCol(D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f));

			nIdxTexture = CManager::GetTexture()->Regist(pPath[nCntMenu]);

			m_apMenu[nCntMenu]->SetIdxTexture(nIdxTexture);

			m_apMenu[nCntMenu]->SetVtx();
		}
	}

	return S_OK;
}

//====================================================
// �I������
//====================================================
void CMenu::Uninit(void)
{
	for (int nCnt = 0; nCnt < MENU_MAX; nCnt++)
	{// ���j���[���ڂ̔j��
		if (m_apMenu[nCnt] != nullptr)
		{
			m_apMenu[nCnt]->Uninit();

			m_apMenu[nCnt] = nullptr;
		}
	}

	Release();
}

//====================================================
// �X�V����
//====================================================
void CMenu::Update(void)
{
	// ���쏈��
	Input();
}

//====================================================
// ���쏈��
//====================================================
void CMenu::Input(void)
{
	CInputKeyboard *pKeyboard = CManager::GetKeyboard();
	CInputJoypad *pJoypad = CManager::GetJoypad();
	CInputMouse *pMouse = CManager::GetMouse();

	CFade *pFade = CManager::GetFade();

	if (pFade == nullptr)
	{
		return;
	}
	else
	{
		if (pFade->GetState() != CFade::FADE_NONE)
		{// �t�F�[�h���͑���ł��Ȃ�
			return;
		}
	}

	if (pKeyboard == nullptr || pMouse == nullptr || pJoypad == nullptr)
	{
		return;
	}

	if (m_apMenu[m_menu] != nullptr)
	{// �ȑO�ɑI�����Ă����ڂ��I��F�ɂ���
		m_apMenu[m_menu]->SetCol(D3DXCOLOR(0.5f,0.5f,0.5f,1.0f));
	}

	// ���ڐ؂�ւ�
	if (pKeyboard->GetTrigger(DIK_A) || pJoypad->GetTrigger(CInputJoypad::PADBUTTONS_DOWN, 0))
	{
		m_menu = (MENU)((m_menu + 1) % MENU_MAX);
	}

	if (pKeyboard->GetTrigger(DIK_D) || pJoypad->GetTrigger(CInputJoypad::PADBUTTONS_UP, 0))
	{
		m_menu = (MENU)((m_menu + MENU_MAX - 1) % MENU_MAX);
	}

	if (m_apMenu[m_menu] != nullptr)
	{// �I�����Ă��鍀�ڂ̐F�ύX
		m_apMenu[m_menu]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}

	if (pKeyboard->GetTrigger(DIK_RETURN) || pJoypad->GetTrigger(CInputJoypad::PADBUTTONS_A, 0) || pMouse->GetTrigger(CInputMouse::BUTTON_LMB))
	{// �I�����ڂɃt�F�[�h����
		Fade(m_menu);
	}
}

//====================================================
// �t�F�[�h���鏈��
//====================================================
void CMenu::Fade(MENU menu)
{
	CFade *pFade = CManager::GetFade();

	if (pFade == nullptr)
	{
		return;
	}

	switch (menu)
	{
	case CMenu::MENU_GAME:

		pFade->SetFade(CScene::MODE_TUTORIAL);

		break;
	case CMenu::MENU_TUTORIAL:

		pFade->SetFade(CScene::MODE_GAME);

		break;
	default:
		break;
	}
}

//====================================================
// �`�揈��
//====================================================
void CMenu::Draw(void)
{

}