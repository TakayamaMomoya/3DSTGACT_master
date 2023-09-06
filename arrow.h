//*****************************************************
//
// ���̏���[arrow.h]
// Author:���R����
//
//*****************************************************

#ifndef _ARROW_H_
#define _ARROW_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "objectX.h"

//*****************************************************
// �O���錾
//*****************************************************
class CObjectX;
class CNumber;

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CArrow : public CObjectX
{
public:
	CArrow(int nPriority = 3);	// �R���X�g���N�^
	~CArrow();	// �f�X�g���N�^

	static CArrow *Create(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void SetDest(D3DXVECTOR3 pos) { m_posDest = pos; }

private:
	void RollPlayer(void);

	D3DXVECTOR3 m_posDest;	// �ړI�l
};

#endif
