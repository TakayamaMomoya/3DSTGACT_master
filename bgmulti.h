//*****************************************************
//
// 背景の処理[bgmulti.cpp]
// Author:髙山桃也
//
//*****************************************************

#ifndef _BGMULTI_H_
#define _BGMULTI_H_

#include "object.h"
#include "bg.h"

//*****************************************************
// マクロ定義
//*****************************************************
#define MAX_BG	(3)	// 背景の最大数

//*****************************************************
// クラスの定義
//*****************************************************
class CBgMulti : public CObject
{
public:
	CBgMulti(int nPriority = 0);	// コンストラクタ
	~CBgMulti();	// デストラクタ

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	D3DXVECTOR3 GetPosition(void) { return D3DXVECTOR3(); }	// 取得処理
	D3DXVECTOR3 GetPositionOld(void) { return D3DXVECTOR3(); }	// 取得処理
	static HRESULT Load(void);	// 読込
	static void Unload(void);
	static CBgMulti *Create(void);	// 生成処理
	float GetWidth(void) { return 0.0f; }	// サイズ取得
	float GetHeight(void) { return 0.0f; }	// サイズ取得

private:
	static LPDIRECT3DTEXTURE9 m_apTexture[MAX_BG];	// テクスチャへのポインタ
};

#endif
