//*****************************************************
//
// マネージャー処理[manager.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "manager.h"
#include "renderer.h"
#include "object2D.h"
#include "player.h"
#include "inputkeyboard.h"
#include "inputmouse.h"
#include "inputjoypad.h"
#include "bullet.h"
#include "explosion.h"
#include "enemy.h"
#include "bg.h"
#include "bgmulti.h"
#include "debugproc.h"
#include "effect2D.h"
#include "effect3D.h"
#include "sound.h"
#include "score.h"
#include "timer.h"
#include "block.h"
#include "item.h"
#include "object.h"
#include "camera.h"
#include "light.h"
#include "object3D.h"
#include "objectX.h"
#include "billboard.h"
#include "model.h"
#include "texture.h"
#include "defend.h"
#include "field.h"
#include "edit.h"
#include "meshfield.h"
#include "skybox.h"
#include "universal.h"
#include "particle.h"
#include "fade.h"
#include "objectmanager.h"

//*****************************************************
// 静的メンバ変数宣言
//*****************************************************
CRenderer *CManager::m_pRenderer = nullptr;	// レンダラーのポインタ
CInputKeyboard *CManager::m_pKeyboard = nullptr;	// キーボードのポインタ
CInputMouse *CManager::m_pMouse = nullptr;	// マウスのポインタ
CInputJoypad *CManager::m_pJoypad = nullptr;	// ジョイパッドのポインタ
CDebugProc *CManager::m_pDebugProc = nullptr;	// デバッグプロシージャのポインタ
CSound *CManager::m_pSound = nullptr;	// サウンドのポインタ
CCamera *CManager::m_pCamera = nullptr;	// カメラのポインタ
CLight *CManager::m_pLight = nullptr;	// ライトのポインタ
CTexture *CManager::m_pTexture = nullptr;	// テクスチャ管理へのポインタ
CUniversal *CManager::m_pUniversal = nullptr;	// 汎用処理へのポインタ
CObjectManager *CManager::m_pObjectManager = nullptr;	// オブジェクト管理へのポインタ
CScene *CManager::m_pScene = nullptr;	// シーンへのポインタ
CFade *CManager::m_pFade = nullptr;	// フェードへのポインタ
CScene::MODE CManager::m_mode = CScene::MODE_GAME;	// 現在のモード
int CManager::m_nScore = 0;	// スコア保存用

//=====================================================
// コンストラクタ
//=====================================================
CManager::CManager()
{
	m_pScene = nullptr;
}

//=====================================================
// デストラクタ
//=====================================================
CManager::~CManager()
{

}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	if (m_pRenderer == nullptr)
	{// レンダラー生成
		m_pRenderer = new CRenderer;

		if (m_pRenderer != nullptr)
		{
			// レンダラー初期化
			if (FAILED(m_pRenderer->Init(hWnd, bWindow)))
			{// 初期化に失敗した場合
				return E_FAIL;
			}
		}
	}

	if (m_pKeyboard == nullptr)
	{// キーボード生成
		m_pKeyboard = new CInputKeyboard;

		if (m_pKeyboard != nullptr)
		{
			// キーボード初期化
			if (FAILED(m_pKeyboard->Init(hInstance, hWnd)))
			{// 初期化に失敗した場合
				return E_FAIL;
			}
		}
	}

	if (m_pMouse == nullptr)
	{// マウス生成
		m_pMouse = new CInputMouse;

		if (m_pMouse != nullptr)
		{
			// マウス初期化
			if (FAILED(m_pMouse->Init(hInstance, hWnd)))
			{// 初期化に失敗した場合
				return E_FAIL;
			}
		}
	}

	if (m_pJoypad == nullptr)
	{// ジョイパッド生成
		m_pJoypad = new CInputJoypad;

		if (m_pJoypad != nullptr)
		{
			// ジョイパッド初期化
			if (FAILED(m_pJoypad->Init()))
			{// 初期化に失敗した場合
				return E_FAIL;
			}
		}
	}

	if (m_pDebugProc == nullptr)
	{// デバッグプロシージャ生成
		m_pDebugProc = new CDebugProc;

		if (m_pDebugProc != nullptr)
		{// デバッグプロシージャ初期化
			m_pDebugProc->Init();
		}
	}

	if (m_pSound == nullptr)
	{// サウンド生成
		m_pSound = new CSound;

		if (m_pSound != nullptr)
		{
			// サウンド初期化
			if (FAILED(m_pSound->Init(hWnd)))
			{// 初期化に失敗した場合
				return E_FAIL;
			}
		}
	}

	if (m_pCamera == nullptr)
	{// カメラ生成
		m_pCamera = new CCamera;

		if (m_pCamera != nullptr)
		{
			// 初期化
			if (FAILED(m_pCamera->Init()))
			{// 初期化に失敗した場合
				return E_FAIL;
			}
		}
	}

	if (m_pLight == nullptr)
	{// ライト生成
		m_pLight = new CLight;

		if (m_pLight != nullptr)
		{
			// 初期化
			if (FAILED(m_pLight->Init()))
			{// 初期化に失敗した場合
				return E_FAIL;
			}
		}
	}

	if (m_pTexture == nullptr)
	{// テクスチャ管理生成
		m_pTexture = new CTexture;

		if (m_pTexture != nullptr)
		{
			// 初期化
			if (FAILED(m_pTexture->Load()))
			{// 初期化に失敗した場合
				return E_FAIL;
			}
		}
	}

	if (m_pFade == nullptr)
	{// フェード生成
		m_pFade = CFade::Create();

		if (m_pFade != nullptr)
		{
			m_pFade->Init();
		}
	}

	if (m_pUniversal == nullptr)
	{// 汎用処理生成
		m_pUniversal = new CUniversal;
	}

	if (m_pObjectManager == nullptr)
	{// オブジェクト管理の生成
		m_pObjectManager = new CObjectManager;
	}

	// 敵情報読込
	CEnemy::Load();

	SetMode(m_mode);

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CManager::Uninit(void)
{
	if (m_pScene != nullptr)
	{
		m_pScene->Uninit();
	}

	if (m_pFade != nullptr)
	{
		m_pFade->Uninit();
	}

	if (m_pObjectManager != nullptr)
	{// オブジェクト管理の削除
		delete m_pObjectManager;
		m_pObjectManager = nullptr;
	}

	// ブロック情報削除
	CBlock::DeleteAll();

	// ブロック番号削除
	CBlock::DeleteIdx();

	// パーティクル情報破棄
	CParticle::Unload();

	if (m_pRenderer != nullptr)
	{// レンダラーの終了・破棄
		m_pRenderer->Uninit();

		delete m_pRenderer;
		m_pRenderer = nullptr;
	}

	if (m_pUniversal != nullptr)
	{// 汎用処理の終了・破棄

		delete m_pUniversal;
		m_pUniversal = nullptr;
	}

	if (m_pKeyboard != nullptr)
	{// キーボードの終了・破棄
		m_pKeyboard->Uninit();

		delete m_pKeyboard;
		m_pKeyboard = nullptr;
	}

	if (m_pMouse != nullptr)
	{// マウスの終了・破棄
		m_pMouse->Uninit();

		delete m_pMouse;
		m_pMouse = nullptr;
	}

	if (m_pJoypad != nullptr)
	{// ジョイパッドの終了・破棄
		m_pJoypad->Uninit();

		delete m_pJoypad;
		m_pJoypad = nullptr;
	}

	if (m_pSound != nullptr)
	{// サウンドの終了・破棄
		m_pSound->Uninit();

		delete m_pSound;
		m_pSound = nullptr;
	}

	if (m_pDebugProc != nullptr)
	{// デバッグプロシージャの終了・破棄
		m_pDebugProc->Uninit();

		delete m_pDebugProc;
		m_pDebugProc = nullptr;
	}

	if (m_pLight != nullptr)
	{// ライトの終了・破棄
		m_pLight->Uninit();

		delete m_pLight;
		m_pLight = nullptr;
	}

	if (m_pCamera != nullptr)
	{// カメラの終了・破棄
		m_pCamera->Uninit();

		delete m_pCamera;
		m_pCamera = nullptr;
	}

	if (m_pTexture != nullptr)
	{// テクスチャの終了・破棄
		m_pTexture->Unload();

		delete m_pTexture;
		m_pTexture = nullptr;
	}

	// 敵破棄
	CEnemy::Unload();

	// モデル破棄
	CModel::Unload();
}

//=====================================================
// 更新処理
//=====================================================
void CManager::Update(void)
{
	if (m_pFade != nullptr)
	{
		m_pFade->Update();
	}

	if (m_pScene != nullptr)
	{
		m_pScene->Update();
	}

	if (m_pKeyboard != nullptr)
	{
		// キーボードの更新
		m_pKeyboard->Update();
	}

	if (m_pMouse != nullptr)
	{
		// マウスの更新
		m_pMouse->Update();
	}

	if (m_pJoypad != nullptr)
	{
		// ジョイパッドの更新
		m_pJoypad->Update();
	}

	if (m_pSound != nullptr)
	{
		// サウンドの更新
		m_pSound->Update();
	}

	if (m_pDebugProc != nullptr)
	{
		// デバッグプロシージャの更新
		m_pDebugProc->Update();
	}

	if (m_pLight != nullptr)
	{
		// ライトの更新
		m_pLight->Update();
	}
	
	if (m_pKeyboard != nullptr)
	{
		if (m_pKeyboard->GetTrigger(DIK_F3))
		{// 強制リセット
			CManager::SetMode(CScene::MODE_TITLE);
		}
	}
}

//=====================================================
// 描画処理
//=====================================================
void CManager::Draw(void)
{
	if (m_pScene != nullptr)
	{
		m_pScene->Draw();
	}

	if (m_pRenderer != nullptr)
	{
		// レンダラーの描画
		m_pRenderer->Draw();
	}
}

//=====================================================
// モード設定
//=====================================================
void CManager::SetMode(CScene::MODE mode)
{
	// サウンド停止
	if (m_pSound != nullptr)
	{
		m_pSound->Stop();
	}

	// シーンを破棄
	if (m_pScene != nullptr)
	{
		m_pScene->Uninit();

		delete m_pScene;
		m_pScene = nullptr;
	}

	// シーン生成
	m_pScene = CScene::Create(mode);

	// モード設定
	m_mode = mode;

	if (m_pCamera != nullptr)
	{
		m_pCamera->Init();
	}
}