//*****************************************************
//
// �����G�̏���[enemymove.cpp]
// Author:���R����
//
//*****************************************************

#ifndef _ENEMYMOVE_H_
#define _ENEMYMOVE_H_

#include "enemy.h"

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CEnemyMove : public CEnemy
{// ���_�Ɍ������G
public:
	CEnemyMove();	// �R���X�g���N�^
	~CEnemyMove();	// �f�X�g���N�^

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	void Tilt(void);	// �X���̐���
	void ChaseDefend(void);	// �ǐՏ���
	void Fall(void);	// �ė��̓���

	float m_fAngleFall;	// �ė��p�x
	float m_fRotateFall;	// �ė����̉�]��
	bool m_bFall;	// �ė����邩�ǂ���
};

class CEnemyAttack : public CEnemy
{// �v���C���[�Ɍ������G
public:
	CEnemyAttack();	// �R���X�g���N�^
	~CEnemyAttack();	// �f�X�g���N�^

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	typedef enum
	{// ���
		STATE_NONE = 0,	// �������ĂȂ����
		STATE_CHASE,	// �ǐՏ��
		STATE_TURN,	// �^�[�����
		STATE_LEAVE,	// �������
		STATE_MAX
	}STATE;

	void Tilt(float fRotDiff);	// �X���̐���
	void ChasePlayer(void);	// �ǐՏ���
	void ManageState(float fLength, float fRotDiff);	// ��ԊǗ�����

	STATE m_state;	// ���
	int m_nTimerLeave;	// ����^�C�}�[
};

#endif
