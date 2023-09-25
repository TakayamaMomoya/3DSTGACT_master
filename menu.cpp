//*****************************************************
//
// メニューの処理[menu.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
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
// マクロ定義
//*****************************************************
#define MENU_WIDTH	(140.0f)	// 項目の幅
#define MENU_HEIGHT	(60.0f)	// 項目の高さ
#define DEST_WIDTH	(300.0f)	// 目標の幅
#define CAPTION_WIDTH	(500.0f)	// 見出しの幅
#define CAPTION_HEIGHT	(250.0f)	// 見出しの高さ
#define CAPTION_PATH "data\\TEXTURE\\UI\\caption01.png"	// 見出しのパス
#define CHANGE_FACT	(0.15f)	// 変わる係数
#define ALPHA_CHANGE	(0.1f)	// α値の変わる速度
#define RANGE_STOP	(3.0f)	// フェードインが終わるしきい値

//====================================================
// コンストラクタ
//====================================================
CMenu::CMenu()
{
	m_menu = MENU_GAME;
	ZeroMemory(&m_apMenu[0], sizeof(m_apMenu));
	m_pCaption = nullptr;
	m_state = STATE_NONE;
}

//====================================================
// デストラクタ
//====================================================
CMenu::~CMenu()
{

}

//====================================================
// 生成処理
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
// 初期化処理
//====================================================
HRESULT CMenu::Init(void)
{
	int nIdxTexture;
	char *pPath[MENU_MAX] = 
	{
		"data\\TEXTURE\\UI\\menu_yes.png",
		"data\\TEXTURE\\UI\\menu_no.png",
	};

	// 見出しの生成
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
	{// 選択肢の生成
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
// 終了処理
//====================================================
void CMenu::Uninit(void)
{
	for (int nCnt = 0; nCnt < MENU_MAX; nCnt++)
	{// メニュー項目の破棄
		if (m_apMenu[nCnt] != nullptr)
		{
			m_apMenu[nCnt]->Uninit();

			m_apMenu[nCnt] = nullptr;
		}
	}

	Release();
}

//====================================================
// 更新処理
//====================================================
void CMenu::Update(void)
{
	// 状態管理
	ManageState();
}

//====================================================
// 状態管理
//====================================================
void CMenu::ManageState(void)
{
	switch (m_state)
	{
	case CMenu::STATE_IN:	// イン状態

		FadeIn();

		break;
	case CMenu::STATE_SELECT:	// 入力状態

		Input();

		break;
	case CMenu::STATE_OUT:	// アウト状態

		FadeOut();

		break;
	default:
		break;
	}
}

//====================================================
// フェードイン処理
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

	// 減少分の計算
	fDiffWidth = (CAPTION_WIDTH - fWidth) * CHANGE_FACT;
	fDiffHeight = (CAPTION_HEIGHT - fHeight) * CHANGE_FACT;

	// 透明にする
	D3DXCOLOR col = m_pCaption->GetCol();

	col.a += ALPHA_CHANGE;

	if (col.a >= 1.0f)
	{// 色の補正
		col.a = 1.0f;
	}

	if (fDiffWidth <= RANGE_STOP)
	{// 選択状態に移行する条件
		m_state = STATE_SELECT;

		for (int nCnt = 0; nCnt < MENU_MAX; nCnt++)
		{// メニュー項目の表示
			if (m_apMenu[nCnt] != nullptr)
			{
				m_apMenu[nCnt]->SetCol(D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f));
			}
		}
	}

	m_pCaption->SetCol(col);

	// サイズ設定
	m_pCaption->SetSize(fWidth + fDiffWidth, fHeight + fDiffHeight);
	m_pCaption->SetVtx();
}

//====================================================
// 操作処理
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
		{// フェード中は操作できない
			return;
		}
	}

	if (pKeyboard == nullptr || pMouse == nullptr || pJoypad == nullptr)
	{
		return;
	}

	if (m_apMenu[m_menu] != nullptr)
	{// 以前に選択してた項目を非選択色にする
		m_apMenu[m_menu]->SetCol(D3DXCOLOR(0.5f,0.5f,0.5f,1.0f));
	}

	// 項目切り替え
	if (pKeyboard->GetTrigger(DIK_A) || pJoypad->GetTrigger(CInputJoypad::PADBUTTONS_LEFT, 0))
	{
		m_menu = (MENU)((m_menu + 1) % MENU_MAX);

		// SE再生
		CManager::GetSound()->Play(CSound::LABEL_SELECT);
	}

	if (pKeyboard->GetTrigger(DIK_D) || pJoypad->GetTrigger(CInputJoypad::PADBUTTONS_RIGHT, 0))
	{
		m_menu = (MENU)((m_menu + MENU_MAX - 1) % MENU_MAX);

		// SE再生
		CManager::GetSound()->Play(CSound::LABEL_SELECT);
	}

	if (m_apMenu[m_menu] != nullptr)
	{// 選択している項目の色変更
		m_apMenu[m_menu]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}

	if (pKeyboard->GetTrigger(DIK_RETURN) || pJoypad->GetTrigger(CInputJoypad::PADBUTTONS_A, 0) || pMouse->GetTrigger(CInputMouse::BUTTON_LMB))
	{// 選択項目にフェードする
		Fade(m_menu);

		// SE再生
		CManager::GetSound()->Play(CSound::LABEL_ENTER);

		m_state = STATE_OUT;
	}
}

//====================================================
// フェードアウト処理
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

	// 減少分の計算
	fDiffWidth = (DEST_WIDTH - fWidth) * CHANGE_FACT;
	fDiffHeight = (0.0f - fHeight) * CHANGE_FACT;

	// 透明にする
	D3DXCOLOR col = m_apMenu[m_menu]->GetCol();

	col.a -= ALPHA_CHANGE;

	if (col.a <= 0.0f)
	{
		col.a = 0.0f;
	}

	m_apMenu[m_menu]->SetCol(col);

	// サイズ設定
	m_apMenu[m_menu]->SetSize(fWidth + fDiffWidth, fHeight + fDiffHeight);
	m_apMenu[m_menu]->SetVtx();
}

//====================================================
// フェードする処理
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
// 描画処理
//====================================================
void CMenu::Draw(void)
{

}