//*****************************************************
//
// ���U���g�̏���[result.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "result.h"
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
CResult::CResult()
{
	m_menu = MENU_REPLAY;
	ZeroMemory(&m_apMenu[0], sizeof(m_apMenu));
	m_pBg = nullptr;
	m_pCaption = nullptr;
}

//====================================================
// �f�X�g���N�^
//====================================================
CResult::~CResult()
{

}

//====================================================
// ��������
//====================================================
CResult *CResult::Create(void)
{
	CResult *pResult = nullptr;

	pResult = new CResult;

	if (pResult != nullptr)
	{
		pResult->Init();
	}

	return pResult;
}

//====================================================
// ����������
//====================================================
HRESULT CResult::Init(void)
{
	int nIdxTexture;
	char *pPath[MENU_MAX] = 
	{
		"data\\TEXTURE\\UI\\menu_yes.png",
		"data\\TEXTURE\\UI\\menu_no.png",
	};

	// �w�i�̐���
	m_pBg = CObject2D::Create(7);

	if (m_pCaption != nullptr)
	{
		m_pBg->SetPosition(D3DXVECTOR3(SCREEN_WIDTH * 0.5, SCREEN_HEIGHT * 0.5f, 0.0f));

		m_pBg->SetSize(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f);

		m_pBg->SetCol(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.6f));

		m_pBg->SetVtx();
	}

	// ���o���̐���
	m_pCaption = CObject2D::Create(7);

	if (m_pCaption != nullptr)
	{
		m_pCaption->SetPosition(D3DXVECTOR3(SCREEN_WIDTH * 0.5, SCREEN_HEIGHT * 0.4f, 0.0f));

		m_pCaption->SetSize(CAPTION_WIDTH, CAPTION_HEIGHT);

		m_pCaption->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

		nIdxTexture = CManager::GetTexture()->Regist(CAPTION_PATH);

		m_pCaption->SetIdxTexture(nIdxTexture);

		m_pCaption->SetVtx();
	}

	for (int nCntResult = 0; nCntResult < MENU_MAX; nCntResult++)
	{// �I�����̐���
		if (m_apMenu[nCntResult] == nullptr)
		{
			m_apMenu[nCntResult] = CObject2D::Create(7);

			m_apMenu[nCntResult]->SetPosition(D3DXVECTOR3(SCREEN_WIDTH * 0.4f + MENU_WIDTH * nCntResult * 2, SCREEN_HEIGHT * 0.6f, 0.0f));

			m_apMenu[nCntResult]->SetSize(MENU_WIDTH, MENU_HEIGHT);

			m_apMenu[nCntResult]->SetCol(D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f));

			nIdxTexture = CManager::GetTexture()->Regist(pPath[nCntResult]);

			m_apMenu[nCntResult]->SetIdxTexture(nIdxTexture);

			m_apMenu[nCntResult]->SetVtx();
		}
	}

	return S_OK;
}

//====================================================
// �I������
//====================================================
void CResult::Uninit(void)
{
	for (int nCnt = 0; nCnt < MENU_MAX; nCnt++)
	{// ���j���[���ڂ̔j��
		if (m_apMenu[nCnt] != nullptr)
		{
			m_apMenu[nCnt]->Uninit();

			m_apMenu[nCnt] = nullptr;
		}
	}

	if (m_pBg != nullptr)
	{
		m_pBg->Uninit();
		m_pBg = nullptr;
	}

	Release();
}

//====================================================
// �X�V����
//====================================================
void CResult::Update(void)
{
	// ���쏈��
	Input();
}

//====================================================
// ���쏈��
//====================================================
void CResult::Input(void)
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
void CResult::Fade(MENU menu)
{
	CFade *pFade = CManager::GetFade();

	if (pFade == nullptr)
	{
		return;
	}

	switch (menu)
	{
	case CResult::MENU_REPLAY:

		pFade->SetFade(CScene::MODE_GAME);

		break;
	case CResult::MENU_RANKING:

		pFade->SetFade(CScene::MODE_RANKING);

		break;
	default:
		break;
	}
}

//====================================================
// �`�揈��
//====================================================
void CResult::Draw(void)
{

}