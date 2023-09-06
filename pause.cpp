//*****************************************************
//
// �|�[�Y�̏���[pause.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "pause.h"
#include "manager.h"
#include "object2D.h"
#include "inputkeyboard.h"
#include "inputjoypad.h"
#include "texture.h"
#include "fade.h"
#include "game.h"

//*****************************************************
// �}�N����`
//*****************************************************
#define MENU_WIDTH	(200.0f)	// ���ڂ̕�
#define MENU_HEIGHT	(60.0f)	// ���ڂ̍���

//====================================================
// �R���X�g���N�^
//====================================================
CPause::CPause()
{
	m_menu = MENU_RESUME;
	ZeroMemory(&m_apMenu[0], sizeof(m_apMenu));
}

//====================================================
// �f�X�g���N�^
//====================================================
CPause::~CPause()
{

}

//====================================================
// ��������
//====================================================
CPause *CPause::Create(void)
{
	CPause *pPause = nullptr;

	pPause = new CPause;

	if (pPause != nullptr)
	{
		pPause->Init();
	}

	return pPause;
}

//====================================================
// ����������
//====================================================
HRESULT CPause::Init(void)
{
	int nIdxTexture;
	char *pPath[MENU_MAX] = 
	{
		"data\\TEXTURE\\UI\\menu_resume.png",
		"data\\TEXTURE\\UI\\menu_retry.png",
		"data\\TEXTURE\\UI\\menu_quit.png",
	};

	for (int nCntMenu = 0; nCntMenu < MENU_MAX; nCntMenu++)
	{
		if (m_apMenu[nCntMenu] == nullptr)
		{
			m_apMenu[nCntMenu] = CObject2D::Create(6);

			m_apMenu[nCntMenu]->SetPosition(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.4f + MENU_HEIGHT * nCntMenu * 2, 0.0f));
			
			m_apMenu[nCntMenu]->SetSize(MENU_WIDTH,MENU_HEIGHT);

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
void CPause::Uninit(void)
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
void CPause::Update(void)
{
	// ���쏈��
	Input();
}

//====================================================
// ���쏈��
//====================================================
void CPause::Input(void)
{
	CInputKeyboard *pKeyboard = CManager::GetKeyboard();
	CInputJoypad *pJoypad = CManager::GetJoypad();

	CFade *pFade = CManager::GetFade();

	if (pFade == nullptr)
	{
		return;
	}
	else
	{
		if (pFade->GetState() != CFade::FADE_NONE)
		{
			return;
		}
	}

	if (pKeyboard == nullptr)
	{
		return;
	}

	if (m_apMenu[m_menu] != nullptr)
	{// �ȑO�ɑI�����Ă����ڂ��I��F�ɂ���
		m_apMenu[m_menu]->SetCol(D3DXCOLOR(0.5f,0.5f,0.5f,1.0f));
	}

	// ���ڐ؂�ւ�
	if (pKeyboard->GetTrigger(DIK_S) || pJoypad->GetTrigger(CInputJoypad::PADBUTTONS_DOWN, 0))
	{
		m_menu = (MENU)((m_menu + 1) % MENU_MAX);
	}

	if (pKeyboard->GetTrigger(DIK_W) || pJoypad->GetTrigger(CInputJoypad::PADBUTTONS_UP, 0))
	{
		m_menu = (MENU)((m_menu + MENU_MAX - 1) % MENU_MAX);
	}

	if (m_apMenu[m_menu] != nullptr)
	{// �I�����Ă��鍀�ڂ̐F�ύX
		m_apMenu[m_menu]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}

	if (pKeyboard->GetTrigger(DIK_RETURN) || pJoypad->GetTrigger(CInputJoypad::PADBUTTONS_A, 0))
	{// �I�����ڂɃt�F�[�h����
		Fade(m_menu);
	}
}

//====================================================
// �t�F�[�h���鏈��
//====================================================
void CPause::Fade(MENU menu)
{
	CFade *pFade = CManager::GetFade();

	if (pFade == nullptr)
	{
		return;
	}

	switch (menu)
	{
	case CPause::MENU_RESUME:

		CGame::TogglePause(this);

		break;
	case CPause::MENU_RETRY:

		pFade->SetFade(CScene::MODE_GAME);

		break;
	case CPause::MENU_QUIT:

		pFade->SetFade(CScene::MODE_TITLE);

		break;
	default:
		break;
	}
}

//====================================================
// �`�揈��
//====================================================
void CPause::Draw(void)
{

}