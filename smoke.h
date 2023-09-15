//*****************************************************
//
// ���̏���[smoke.cpp]
// Author:���R����
//
//*****************************************************

#ifndef _SMOKE_H_
#define _SMOKE_H_

#include "ObjectX.h"

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CSmoke : public CObjectX
{
public:
	CSmoke(int nPriority = 3);	// �R���X�g���N�^
	~CSmoke();	// �f�X�g���N�^

	static CSmoke *Create(D3DXVECTOR3 pos,int nLife);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	int m_nLife;	// ����
	static int m_nNumAll;	// ����
};

#endif
