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

//*****************************************************
// �}�N����`
//*****************************************************
#define LOGO_PATH	"data\\TEXTURE\\UI\\logo000.png"	// ���S�̃p�X

//=====================================================
// �R���X�g���N�^
//=====================================================
CTitle::CTitle()
{

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

	// �w�i�I�u�W�F�N�g�̐���
	CMotion *pMotion = CMotion::Create("data\\MOTION\\motionArms01.txt");

	pMotion->SetPosition(D3DXVECTOR3(0.0f, -50.0f, 0.0f));

	pMotion->SetMatrix();

	CSkybox::Create();

	// �t�H�O��؂�
	CRenderer::EnableFog(false);

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CTitle::Uninit(void)
{
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

	if (pKeyboard != nullptr && pMouse != nullptr)
	{
		if (pKeyboard->GetTrigger(DIK_RETURN) || 
			pMouse->GetTrigger(CInputMouse::BUTTON_LMB) || 
			pJoypad->GetTrigger(CInputJoypad::PADBUTTONS_A, 0))
		{// ��ʑJ��
			if (pFade != nullptr)
			{
				pFade->SetFade(CScene::MODE_GAME);
			}
		}
	}

	// �J�����X�V
	if (pCamera != nullptr)
	{
		// �^�C�g���̓���
		pCamera->TitleMove();
	}
}

//=====================================================
// �`�揈��
//=====================================================
void CTitle::Draw(void)
{

}