//*****************************************************
//
// タイトル処理[title.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
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
// マクロ定義
//*****************************************************
#define LOGO_PATH	"data\\TEXTURE\\UI\\logo000.png"	// ロゴのパス

//=====================================================
// コンストラクタ
//=====================================================
CTitle::CTitle()
{

}

//=====================================================
// デストラクタ
//=====================================================
CTitle::~CTitle()
{

}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CTitle::Init(void)
{
	// ロゴの生成
	CObject2D *pObject2D = CObject2D::Create(7);
	pObject2D->SetSize(875.0f * 0.45f, 320.0f * 0.45f);
	pObject2D->SetPosition(D3DXVECTOR3(SCREEN_WIDTH * 0.7f, 200.0f, 0.0f));

	int nIdx = CManager::GetTexture()->Regist(LOGO_PATH);
	pObject2D->SetIdxTexture(nIdx);
	pObject2D->SetVtx();

	// 背景オブジェクトの生成
	CMotion *pMotion = CMotion::Create("data\\MOTION\\motionArms01.txt");

	pMotion->SetPosition(D3DXVECTOR3(0.0f, -50.0f, 0.0f));

	pMotion->SetMatrix();

	CSkybox::Create();

	// フォグを切る
	CRenderer::EnableFog(false);

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CTitle::Uninit(void)
{
	// オブジェクト全破棄
	CObject::ReleaseAll();
}

//=====================================================
// 更新処理
//=====================================================
void CTitle::Update(void)
{
	// 情報取得
	CInputKeyboard *pKeyboard = CManager::GetKeyboard();
	CInputMouse *pMouse = CManager::GetMouse();
	CInputJoypad *pJoypad = CManager::GetJoypad();

	CCamera *pCamera = CManager::GetCamera();

	// シーンの更新
	CScene::Update();
		
	CFade *pFade = CManager::GetFade();

	if (pKeyboard != nullptr && pMouse != nullptr)
	{
		if (pKeyboard->GetTrigger(DIK_RETURN) || 
			pMouse->GetTrigger(CInputMouse::BUTTON_LMB) || 
			pJoypad->GetTrigger(CInputJoypad::PADBUTTONS_A, 0))
		{// 画面遷移
			if (pFade != nullptr)
			{
				pFade->SetFade(CScene::MODE_GAME);
			}
		}
	}

	// カメラ更新
	if (pCamera != nullptr)
	{
		// タイトルの動き
		pCamera->TitleMove();
	}
}

//=====================================================
// 描画処理
//=====================================================
void CTitle::Draw(void)
{

}