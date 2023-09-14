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

	static CNoise *Create(int nLife = 10,float fPeakAlpha = 0.3f);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static int GetNumAll(void) { return m_nNumAll; }

private:
	void ChangeCol(void);

	int m_nLife;	// ����
	int m_nAlpha;	// �A���t�@�e�X�g�̒l
	float m_fAddAlpha;	// �A���t�@�e�X�g�̉��Z�l
	float m_fPeakAlpha;	// ���l���s�[�N�ɂȂ�Ƃ��̊���
	static int m_nNumAll;	// ����
};

#endif