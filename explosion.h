//*****************************************************
//
// 爆発処理[explosion.h]
// Author:髙山桃也
//
//*****************************************************

#ifndef _EXPLOSION_H_
#define _EXPLOSION_H_

//*****************************************************
// インクルード
//*****************************************************
#include "billboard.h"

//*****************************************************
// クラスの定義
//*****************************************************
class CExplosion : public CBillboard
{
public:
	CExplosion(int nPriority = 4);	// コンストラクタ
	~CExplosion();	// デストラクタ

	static CExplosion *Create(D3DXVECTOR3 pos);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static HRESULT Load(void);	// 読込
	static void Unload(void);
	static int GetNumAll(void) { return m_nNumAll; }

private:
	int m_nLife;	// 寿命
	static int m_nNumAll;	// 総数
	int m_nCounterAnim;	// アニメーションカウンター
	int m_nPatternAnim;	// アニメパターンNO.
};

#endif