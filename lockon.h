//*****************************************************
//
// ロックオンの処理[lockon.h]
// Author:髙山桃也
//
//*****************************************************

#ifndef _LOCKON_H_
#define _LOCKON_H_

//*****************************************************
// インクルード
//*****************************************************
#include "object.h"

//*****************************************************
// 前方宣言
//*****************************************************
class CBillboard;
class CEnemy;

//*****************************************************
// クラスの定義
//*****************************************************
class CLockon : public CObject
{
public:
	CLockon(int nPriority = 3);	// コンストラクタ
	~CLockon();	// デストラクタ

	static CLockon *Create(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void SetEnemy(CEnemy *pEnemy) { m_pEnemy = pEnemy; }
	D3DXVECTOR3 GetPosDest(void) { return m_posDest; }
	void SetPosDest(D3DXVECTOR3 pos) { m_posDest = pos; }

private:
	CBillboard *m_pLockOn;	// ロックオンマーカーのビルボード
	CEnemy *m_pEnemy;	// 見てる敵のポインタ
	D3DXVECTOR3 m_posDest;	// 目的の座標
	float m_fSize;	// サイズ
};

#endif
