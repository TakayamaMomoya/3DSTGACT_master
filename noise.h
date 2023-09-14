//*****************************************************
//
// �m�C�Y�̏���[noise.h]
// Author:���R����
//
//*****************************************************

#ifndef _NOISE_H_
#define _NOISE_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "animation2D.h"

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CNoise : public CAnim2D
{
public:
	CNoise(int nPriority = 7);	// �R���X�g���N�^
	~CNoise();	// �f�X�g���N�^

	static CNoise *Create(int nLife = 10);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static int GetNumAll(void) { return m_nNumAll; }

private:
	int m_nLife;	// ����
	static int m_nNumAll;	// ����
};

#endif