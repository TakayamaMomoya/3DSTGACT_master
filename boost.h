//*****************************************************
//
// �u�[�X�g�\���̏���[boost.h]
// Author:���R����
//
//*****************************************************

#ifndef _BOOST_H_
#define _BOOST_H_

#include "Object.h"

//*****************************************************
// �O���錾
//*****************************************************
class CObject2D;

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CBoost : public CObject
{
public:
	CBoost(int nPriority = 6);	// �R���X�g���N�^
	~CBoost();	// �f�X�g���N�^

	static CBoost *Create(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void) {};

private:
	CObject2D *m_pObjectGauge;	// �Q�[�W�̃|�C���^
	CObject2D *m_pCaption;	// ���o���̃|�C���^
};

#endif
