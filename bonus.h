//*****************************************************
//
// ボーナスの表示[bonus.h]
// Author:髙山桃也
//
//*****************************************************

#ifndef _BONUS_H_
#define _BONUS_H_

//*****************************************************
// インクルード
//*****************************************************
#include "object.h"

//*****************************************************
// 前方宣言
//*****************************************************
class CObject2D;
class CNumber;

//*****************************************************
// クラスの定義
//*****************************************************
class CBonus : public CObject
{
public:
	enum TYPE
	{
		TYPE_DOGDE = 0,	// 回避
		TYPE_MISSILE,	// ミサイル撃破
		TYPE_MANUALHIT,	// マニュアル照準で命中
		TYPE_MAX
	};

	CBonus(int nPriority = 1);	// コンストラクタ
	~CBonus();	// デストラクタ

	static CBonus *Create(TYPE type);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	enum STATE
	{
		STATE_NONE = 0,	// 何でもない状態
		STATE_IN,	// 出現状態
		STATE_WAIT,	// 待機状態
		STATE_OUT,	// 消滅状態
		STATE_MAX
	};

	void MoveToDest(void);
	void ManageState(void);
	void FollowCaption(void);
	int BonusScore(TYPE type);

	CObject2D *m_pCaption;	// 見出しのポインタ
	CNumber *m_pNumber;	// ボーナス数字のポインタ
	D3DXVECTOR3 m_posDest;	// 目標位置
	STATE m_state;	// 状態
	int m_nTimerOut;	// フェードアウトまでのタイマー

	static CBonus *m_pHead;	// 先頭のアドレス
	static CBonus *m_pTail;	// 最後尾のアドレス
	CBonus *m_pPrev;	// 前のアドレス
	CBonus *m_pNext;	// 次のアドレス
};

#endif
