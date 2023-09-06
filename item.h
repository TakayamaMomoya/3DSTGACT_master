//*****************************************************
//
// アイテムの処理[item.cpp]
// Author:髙山桃也
//
//*****************************************************

#ifndef _ITEM_H_
#define _ITEM_H_

#include "Object2D.h"

//*****************************************************
// 前方宣言
//*****************************************************
class CNumber;

//*****************************************************
// クラスの定義
//*****************************************************
class CItem : public CObject2D
{
public:
	typedef enum
	{
		TYPE_NONE = 0,	// 何でもない
		TYPE_DEBRIS,	// 残骸
		TYPE_MAX
	}TYPE;
	CItem();	// コンストラクタ
	~CItem();	// デストラクタ

	static CItem *Create(D3DXVECTOR3 pos,TYPE type, float height = 40.0f, float width = 40.0f);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	bool CollisionPlayer(D3DXVECTOR3 pos);
	static HRESULT Load(void);	// 読込
	static void Unload(void);
	static int GetNumAll(void) { return m_nNumAll; }

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;	// テクスチャへのポインタ
	static int m_nNumAll;	// 総数
};

#endif
