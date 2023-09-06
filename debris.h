//*****************************************************
//
// �j�Ђ̏���[debris.cpp]
// Author:���R����
//
//*****************************************************

#ifndef _DEBRIS_H_
#define _DEBRIS_H_

#include "ObjectX.h"

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CDebris : public CObjectX
{
public:
	CDebris(int nPriority = 3);	// �R���X�g���N�^
	~CDebris();	// �f�X�g���N�^

	static CDebris *Create(D3DXVECTOR3 pos,int nLife);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	int m_nLife;	// ����
	static int m_nNumAll;	// ����
};

#endif
