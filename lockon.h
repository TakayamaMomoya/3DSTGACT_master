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

private:
	CBillboard *m_pLockOn;	// ロックオンマーカーのビルボード
	CEnemy *m_pEnemy;	// 見てる敵のポインタ
	float m_fSize;	// サイズ
};

#endif
