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
#include "menu.h"
#include "sound.h"

//*****************************************************
// マクロ定義
//*****************************************************
#define LOGO_PATH	"data\\TEXTURE\\UI\\logo000.png"	// ロゴのパス
#define START_PATH	"data\\TEXTURE\\UI\\start.png"	// スタート表示のパス
#define START_WIDTH	(200.0f)	// スタート表示の幅
#define START_HEIGHT	(50.0f)	// スタート表示の高さ
#define DEST_WIDTH	(500.0f)	// スタート表示の幅
#define CHANGE_FACT	(0.1f)	// 変化する係数
#define ALPHA_CHANGE	(0.05f)	// α値の変化量

//=====================================================
// コンストラクタ
//=====================================================
CTitle::CTitle()
{
	m_pMenu = nullptr;
	m_pStart = nullptr;
	m_state = STATE_NONE;
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

	// スタート表示の生成
	m_pStart = CObject2D::Create(7);

	if (m_pStart != nullptr)
	{
		m_pStart->SetSize(START_WIDTH, START_HEIGHT);
		m_pStart->SetPosition(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.7f, 0.0f));

		int nIdx = CManager::GetTexture()->Regist(START_PATH);
		m_pStart->SetIdxTexture(nIdx);
		m_pStart->SetVtx();
	}

	// 背景オブジェクトの生成
	CMotion *pMotion = CMotion::Create("data\\MOTION\\motionArms01.txt");

	pMotion->SetPosition(D3DXVECTOR3(0.0f, -50.0f, 0.0f));

	pMotion->SetMatrix();

	CSkybox::Create();

	// フォグを切る
	CRenderer::EnableFog(false);

	CManager::GetSound()->Stop();

	// BGM再生
	CManager::GetSound()->Play(CSound::LABEL_BGM000);

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CTitle::Uninit(void)
{
	if (m_pMenu != nullptr)
	{
		m_pMenu->Uninit();
		m_pMenu = nullptr;
	}

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

	if (m_state == STATE_NONE)
	{
		if (pKeyboard != nullptr && pMouse != nullptr)
		{
			if (pKeyboard->GetTrigger(DIK_RETURN) ||
				pMouse->GetTrigger(CInputMouse::BUTTON_LMB) ||
				pJoypad->GetTrigger(CInputJoypad::PADBUTTONS_A, 0))
			{// スタート表示が動き出す
				if (m_pMenu == nullptr)
				{
					m_state = STATE_OUT;

					// SE再生
					CManager::GetSound()->Play(CSound::LABEL_ENTER);
				}
			}
		}
	}
	else if(m_state == STATE_OUT)
	{
		// スタート表示の管理
		ManageStart();
	}

	// カメラ更新
	if (pCamera != nullptr)
	{
		// タイトルの動き
		pCamera->TitleMove();
	}
}

//=====================================================
// スタート表示の管理
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

	// 減少分の計算
	fDiffWidth = (DEST_WIDTH - fWidth) * CHANGE_FACT;
	fDiffHeight = (0.0f - fHeight) * CHANGE_FACT;

	// 透明にする
	D3DXCOLOR col = m_pStart->GetCol();

	col.a -= ALPHA_CHANGE;

	if (col.a <= 0.0f)
	{
		col.a = 0.0f;

		CMenu::Create();

		m_state = STATE_MENU;
	}

	m_pStart->SetCol(col);

	// サイズ設定
	m_pStart->SetSize(fWidth + fDiffWidth, fHeight + fDiffHeight);
	m_pStart->SetVtx();
}

//=====================================================
// 描画処理
//=====================================================
void CTitle::Draw(void)
{

}