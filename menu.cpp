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
#include "sound.h"

//*****************************************************
// �}�N����`
//*****************************************************
#define MENU_WIDTH	(140.0f)	// ���ڂ̕�
#define MENU_HEIGHT	(60.0f)	// ���ڂ̍���
#define DEST_WIDTH	(300.0f)	// �ڕW�̕�
#define CAPTION_WIDTH	(500.0f)	// ���o���̕�
#define CAPTION_HEIGHT	(250.0f)	// ���o���̍���
#define CAPTION_PATH "data\\TEXTURE\\UI\\caption01.png"	// ���o���̃p�X
#define CHANGE_FACT	(0.15f)	// �ς��W��
#define ALPHA_CHANGE	(0.1f)	// ���l�̕ς�鑬�x
#define RANGE_STOP	(3.0f)	// �t�F�[�h�C�����I��邵�����l

//====================================================
// �R���X�g���N�^
//====================================================
CMenu::CMenu()
{
	m_menu = MENU_GAME;
	ZeroMemory(&m_apMenu[0], sizeof(m_apMenu));
	m_pCaption = nullptr;
	m_state = STATE_NONE;
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

	if (m_pCaption != nullptr)
	{
		m_pCaption->SetPosition(D3DXVECTOR3(SCREEN_WIDTH * 0.5, SCREEN_HEIGHT * 0.5f, 0.0f));

		m_pCaption->SetSize(0, 0);

		m_pCaption->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

		nIdxTexture = CManager::GetTexture()->Regist(CAPTION_PATH);

		m_pCaption->SetIdxTexture(nIdxTexture);

		m_pCaption->SetVtx();
	}

	for (int nCntMenu = 0; nCntMenu < MENU_MAX; nCntMenu++)
	{// �I�����̐���
		if (m_apMenu[nCntMenu] == nullptr)
		{
			m_apMenu[nCntMenu] = CObject2D::Create(7);

			if (m_apMenu[nCntMenu] != nullptr)
			{
				m_apMenu[nCntMenu]->SetPosition(D3DXVECTOR3(SCREEN_WIDTH * 0.4f + MENU_WIDTH * nCntMenu * 2, SCREEN_HEIGHT * 0.6f, 0.0f));

				m_apMenu[nCntMenu]->SetSize(MENU_WIDTH, MENU_HEIGHT);

				m_apMenu[nCntMenu]->SetCol(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));

				nIdxTexture = CManager::GetTexture()->Regist(pPath[nCntMenu]);

				m_apMenu[nCntMenu]->SetIdxTexture(nIdxTexture);

				m_apMenu[nCntMenu]->SetVtx();
			}
		}
	}

	m_state = STATE_IN;

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
	// ��ԊǗ�
	ManageState();
}

//====================================================
// ��ԊǗ�
//====================================================
void CMenu::ManageState(void)
{
	switch (m_state)
	{
	case CMenu::STATE_IN:	// �C�����

		FadeIn();

		break;
	case CMenu::STATE_SELECT:	// ���͏��

		Input();

		break;
	case CMenu::STATE_OUT:	// �A�E�g���

		FadeOut();

		break;
	default:
		break;
	}
}

//====================================================
// �t�F�[�h�C������
//====================================================
void CMenu::FadeIn(void)
{
	if (m_pCaption == nullptr)
	{
		return;
	}

	float fWidth = m_pCaption->GetWidth();
	float fHeight = m_pCaption->GetHeight();
	float fDiffWidth;
	float fDiffHeight;

	// �������̌v�Z
	fDiffWidth = (CAPTION_WIDTH - fWidth) * CHANGE_FACT;
	fDiffHeight = (CAPTION_HEIGHT - fHeight) * CHANGE_FACT;

	// �����ɂ���
	D3DXCOLOR col = m_pCaption->GetCol();

	col.a += ALPHA_CHANGE;

	if (col.a >= 1.0f)
	{// �F�̕␳
		col.a = 1.0f;
	}

	if (fDiffWidth <= RANGE_STOP)
	{// �I����ԂɈڍs�������
		m_state = STATE_SELECT;

		for (int nCnt = 0; nCnt < MENU_MAX; nCnt++)
		{// ���j���[���ڂ̕\��
			if (m_apMenu[nCnt] != nullptr)
			{
				m_apMenu[nCnt]->SetCol(D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f));
			}
		}
	}

	m_pCaption->SetCol(col);

	// �T�C�Y�ݒ�
	m_pCaption->SetSize(fWidth + fDiffWidth, fHeight + fDiffHeight);
	m_pCaption->SetVtx();
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
	if (pKeyboard->GetTrigger(DIK_A) || pJoypad->GetTrigger(CInputJoypad::PADBUTTONS_LEFT, 0))
	{
		m_menu = (MENU)((m_menu + 1) % MENU_MAX);

		// SE�Đ�
		CManager::GetSound()->Play(CSound::LABEL_SELECT);
	}

	if (pKeyboard->GetTrigger(DIK_D) || pJoypad->GetTrigger(CInputJoypad::PADBUTTONS_RIGHT, 0))
	{
		m_menu = (MENU)((m_menu + MENU_MAX - 1) % MENU_MAX);

		// SE�Đ�
		CManager::GetSound()->Play(CSound::LABEL_SELECT);
	}

	if (m_apMenu[m_menu] != nullptr)
	{// �I�����Ă��鍀�ڂ̐F�ύX
		m_apMenu[m_menu]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}

	if (pKeyboard->GetTrigger(DIK_RETURN) || pJoypad->GetTrigger(CInputJoypad::PADBUTTONS_A, 0) || pMouse->GetTrigger(CInputMouse::BUTTON_LMB))
	{// �I�����ڂɃt�F�[�h����
		Fade(m_menu);

		// SE�Đ�
		CManager::GetSound()->Play(CSound::LABEL_ENTER);

		m_state = STATE_OUT;
	}
}

//====================================================
// �t�F�[�h�A�E�g����
//====================================================
void CMenu::FadeOut(void)
{
	if (m_apMenu[m_menu] == nullptr)
	{
		return;
	}

	float fWidth = m_apMenu[m_menu]->GetWidth();
	float fHeight = m_apMenu[m_menu]->GetHeight();
	float fDiffWidth;
	float fDiffHeight;

	// �������̌v�Z
	fDiffWidth = (DEST_WIDTH - fWidth) * CHANGE_FACT;
	fDiffHeight = (0.0f - fHeight) * CHANGE_FACT;

	// �����ɂ���
	D3DXCOLOR col = m_apMenu[m_menu]->GetCol();

	col.a -= ALPHA_CHANGE;

	if (col.a <= 0.0f)
	{
		col.a = 0.0f;
	}

	m_apMenu[m_menu]->SetCol(col);

	// �T�C�Y�ݒ�
	m_apMenu[m_menu]->SetSize(fWidth + fDiffWidth, fHeight + fDiffHeight);
	m_apMenu[m_menu]->SetVtx();
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