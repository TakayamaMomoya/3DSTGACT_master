//*****************************************************
//
// ��q�ڕW�̏���[defend.h]
// Author:���R����
//
//*****************************************************

#ifndef _DEFEND_H_
#define _DEFEND_H_

//*****************************************************
// �C���N���[�h
//****************************************************
#include "main.h"
#include "object.h"

//*****************************************************
// �O���錾
//*****************************************************
class CCollisionCube;
class CCollisionSphere;
class CObjectX;

//*****************************************************
// �N���X��`
//****************************************************
class CDefend : public CObject
{
public:
	CDefend();	// �R���X�g���N�^
	~CDefend();	// �f�X�g���N�^

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	D3DXVECTOR3 GetPosition(void);
	static CDefend *Create(void);
	void Hit(float fDamage);
	int GetLife(void) { return m_nLife; }
	void SetLife(int nLife);

private:
	enum STATE
	{// ���
		STATE_NONE = 0,	// ���ł��Ȃ����
		STATE_NORMAL,	// �ʏ���
		STATE_DEATH,	// ���S���
		STATE_MAX
	};

	void ManageState(void);	// ��ԊǗ�

	CObjectX *m_pObjectX;	// ���f���I�u�W�F�N�g�ւ̃|�C���^
	CCollisionCube *m_pCollisionCube;	// �����̂̓����蔻��
	CCollisionSphere *m_pCollisionSphere;	// ���̓����蔻��
	int m_nLife;	// �̗�
	int m_nTimerDeath;	// ���S�^�C�}�[
	STATE m_state;	// ���
};

#endif