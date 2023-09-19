//*****************************************************
//
// チュートリアル処理[tutorial.h]
// Author:髙山桃也
//
//*****************************************************
#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_

//*****************************************************
// インクルード
//*****************************************************
#include "main.h"
#include "scene.h"

//*****************************************************
// 前方宣言
//*****************************************************
class CTutorialManager;

//*****************************************************
// クラスの定義
//*****************************************************
class CTutorial : public CScene
{
public:
	CTutorial();	// コンストラクタ
	~CTutorial();	// デストラクタ

	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update();
	virtual void Draw();

private:
	void Debug(void);	// デバッグ処理
	void ManageState(void);	// 状態管理
	void LimitPlayerPos(void);	// プレイヤー移動制限

	static CTutorialManager *m_pTutorialManager;	// チュートリアル管理

};

#endif