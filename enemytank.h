//*****************************************************
//
// ��ԓG�̏���[enemytank.cpp]
// Author:���R����
//
//*****************************************************

#ifndef _ENEMYTANK_H_
#define _ENEMYTANK_H_

#include "enemy.h"

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CEnemyTank : public CEnemy
{
public:
	CEnemyTank();	// �R���X�g���N�^
	~CEnemyTank();	// �f�X�g���N�^

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	void ChaseDefend(void);	// �ǐՏ���
};

#endif
