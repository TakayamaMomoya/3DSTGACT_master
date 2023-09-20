//*****************************************************
//
// �`���[�g���A���p�G�̏���[enemytutorial.cpp]
// Author:���R����
//
//*****************************************************

#ifndef _ENEMYTUTORIAL_H_
#define _ENEMYTUTORIAL_H_

#include "enemy.h"

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CEnemyTutorial : public CEnemy
{// ���_�Ɍ������G
public:
	CEnemyTutorial();	// �R���X�g���N�^
	~CEnemyTutorial();	// �f�X�g���N�^

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	void Fall(void);	// �ė��̓���

	float m_fAngleFall;	// �ė��p�x
	float m_fRotateFall;	// �ė����̉�]��
	bool m_bFall;	// �ė����邩�ǂ���
};

#endif
