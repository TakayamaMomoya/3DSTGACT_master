//*****************************************************
//
// マネージャー処理[manager.h]
// Author:髙山桃也
//
//*****************************************************
#ifndef _MANAGER_H_
#define _MANAGER_H_

//*****************************************************
// インクルード
//*****************************************************
#include "main.h"
#include "scene.h"

//*****************************************************
// 前方宣言
//*****************************************************
class CRenderer;
class CInputKeyboard;
class CInputMouse;
class CInputJoypad;
class CDebugProc;
class CSound;
class CCamera;
class CLight;
class CTexture;
class CUniversal;
class CFade;
class CObjectManager;

//*****************************************************
// クラスの定義
//*****************************************************
class CManager
{
public:
	CManager();	// コンストラクタ
	~CManager();	// デストラクタ

	HRESULT Init(HINSTANCE hInstance,HWND hWnd,BOOL bWindow);
	void Uninit(void);
	void Update();
	void Draw();
	static CRenderer *GetRenderer(void) { return m_pRenderer; }
	static CInputKeyboard *GetKeyboard(void) { return m_pKeyboard; }
	static CInputMouse *GetMouse(void) { return m_pMouse; }
	static CInputJoypad *GetJoypad(void) { return m_pJoypad; }
	static CDebugProc *GetDebugProc(void) { return m_pDebugProc; }
	static CSound *GetSound(void) { return m_pSound; }
	static CCamera *GetCamera(void) { return m_pCamera; }
	static CLight *GetLight(void) { return m_pLight; }
	static CTexture *GetTexture(void) { return m_pTexture; }
	static CObjectManager *GetObjectManager(void) { return m_pObjectManager; }
	static CUniversal *GetUniversal(void) { return m_pUniversal; }
	static void SetMode(CScene::MODE mode);
	static CScene::MODE GetMode(void) { return m_mode; }
	static CFade *GetFade(void) { return m_pFade; }
	static void SetScore(int nScore) { m_nScore = nScore; }
	static int GetScore(void) { return m_nScore; }

private:
	static CRenderer *m_pRenderer;	// レンダラーのクラス
	static CInputKeyboard *m_pKeyboard;	// キーボードのクラス
	static CInputMouse *m_pMouse;	// マウスのクラス
	static CInputJoypad *m_pJoypad;	// マウスのクラス
	static CDebugProc *m_pDebugProc;	// デバッグプロシージャのクラス
	static CSound *m_pSound;	// サウンドのクラス
	static CCamera *m_pCamera;	// カメラのポインタ
	static CLight *m_pLight;	// ライトのポインタ
	static CTexture *m_pTexture;	// テクスチャ管理へのポインタ
	static CUniversal *m_pUniversal;	// 汎用処理のポインタ
	static CScene *m_pScene;	// 現在のシーン
	static CScene::MODE m_mode;	// 現在のモード
	static CFade *m_pFade;	// フェードのポインタ
	static CObjectManager *m_pObjectManager;	// オブジェクト管理のポインタ
	static int m_nScore;	// スコア
};

#endif