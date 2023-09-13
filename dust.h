//*****************************************************
//
// 塵の処理[dust.h]
// Author:髙山桃也
//
//*****************************************************

#ifndef _DUST_H_
#define _DUST_H_

//*****************************************************
// インクルード
//*****************************************************
#include "billboard.h"

//*****************************************************
// クラスの定義
//*****************************************************
class CDust : public CBillboard
{
public:
	CDust(int nPriority = 6);	// コンストラクタ
	~CDust();	// デストラクタ

	static CDust *Create(D3DXVECTOR3 pos);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	enum STATE
	{
		STATE_NONE = 0,	// 何でもない状態
		STATE_IN,	// 出現
		STATE_OUT,	// 消える状態
		STATE_MAX
	};

	void SetMove(void);

	float m_fAlpha;	// α値の変化量
	STATE m_state;	// 状態
	D3DXVECTOR3 m_move;	// 移動量
};

#endif