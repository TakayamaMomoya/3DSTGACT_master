//*****************************************************
//
// 塵の管理[dustmanager.h]
// Author:髙山桃也
//
//*****************************************************

#ifndef _DUSTMANAGER_H_
#define _DUSTMANAGER_H_

//*****************************************************
// インクルード
//****************************************************
#include "object.h"

//*****************************************************
// クラス定義
//****************************************************
class CDustManager : public CObject
{
public:
	CDustManager();	// コンストラクタ
	~CDustManager();	// デストラクタ

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CDustManager *Create(void);

private:
	void SetTimer(void);

	int m_nTimer;	// 次の塵を出すまでのタイマー
};

#endif