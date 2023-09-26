//*****************************************************
//
// �G�̏���[enemy.cpp]
// Author:���R����
//
//*****************************************************

#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "motion.h"

//*****************************************************
// �O���錾
//*****************************************************
class CCollisionSphere;
class CCollisionCube;
class CBillboard;
class CShadow;
class CExplSpawner;
class CArrow;

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CEnemy : public CMotion
{
public:
	typedef enum
	{// �G�̎��
		TYPE_NONE = 0,	// ���ł��Ȃ�
		TYPE_MOVE,	// �ړ�����G
		TYPE_TANK,	// ��ԓG
		TYPE_ATTACK,	// �U�����Ă���G
		TYPE_TUTORIAL,	// �`���[�g���A���̓G
		TYPE_MAX
	}TYPE;

	typedef enum
	{// ���
		STATE_NONE = 0,	// ���ł��Ȃ����
		STATE_NORMAL,	// �ʏ���
		STATE_DAMAGE,	// �_���[�W���
		STATE_DEATH,	// ���S���
		STATE_MAX
	}STATE;

	enum MOVESTATE
	{// �ړ����
		MOVESTATE_NONE = 0,	// ���ł��Ȃ����
		MOVESTATE_CHASE,	// �ڕW�ǐՏ��
		MOVESTATE_AVOID,	// ��Q�����������
		MOVESTATE_MAX
	};

	CEnemy();	// �R���X�g���N�^
	~CEnemy();	// �f�X�g���N�^

	static CEnemy *Create(D3DXVECTOR3 pos,TYPE type);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static HRESULT Load(void);	// �Ǎ�
	static void Unload(void);
	int GetCntAttack(void) { return m_nCntAttack; }
	void SetCntAttack(int nValue);
	static int GetNumAll(void) { return m_nNumAll; }
	void SetLife(float fLife);
	float GetLife(void) { return m_fLife; }
	void Hit(float fDamage);
	bool IsLockon(void) { return m_bLockon; }
	void SetLockon(bool bLockon) { m_bLockon = bLockon; }
	static CEnemy *GetEnemy(void) { return m_pHead; }
	static void SpawnEnemy(int nNumWave = 1);
	void SetSpherePosition(D3DXVECTOR3 pos);
	void Death(void);
	STATE GetState(void) { return m_state; }
	MOVESTATE GetMoveState(void) { return m_moveState; }
	D3DXVECTOR3 GetPosDest(void) { return m_posDest; }
	void SetPosDest(D3DXVECTOR3 pos) { m_posDest = pos; }
	CCollisionSphere *GetClsnSphere(void) { return m_pCollisionSphere; }
	bool IsFind(void) { return m_bFind; }
	CEnemy *GetNext(void) { return m_pNext; }
	int GetScore(void) { return m_nScore; }
	void SetScore(int nScore) { m_nScore = nScore; }

protected:
	void AvoidCollisions(void);
	void ManagePosDest(void);

private:
	typedef struct
	{// �G�̏��
		TYPE type;	// ���
		D3DXVECTOR3 pos;	// �ʒu
	}ENEMYINFO;

	typedef struct
	{// �E�F�[�u�̏��
		ENEMYINFO *pEnemyInfo;	// �G�̏��
		int m_nNumEnemy;
	}WAVEINFO;

	void ManageState(void);
	void SearchTarget(void);
	void ManageBonus(void);
	void ManageCollision(void);

	int m_nCntAttack;	// �U��������܂ł̎���	
	static int m_nNumAll;	// ����
	int m_nId;	// �ԍ�
	int m_nTimerState;	// ��ԑJ�ڃ^�C�}�[
	float m_fLife;	// �̗�
	CCollisionSphere *m_pCollisionSphere;	// ���̓����蔻��
	CCollisionCube *m_pCollisionCube;	// �����̂̓����蔻��
	bool m_bLockon;	// ���b�N�I������Ă��邩�ǂ���
	bool m_bFind;	// �����Ă��邩�ǂ���
	static int m_nNumWave;	// �E�F�[�u��
	static WAVEINFO *m_pWaveInfo;	// �E�F�[�u���̃|�C���^
	CShadow *m_pShadow;	// �e�̃|�C���^
	STATE m_state;	// ���
	int m_nTimerDeath;	// ���S�܂ł̃J�E���^
	CExplSpawner **m_ppExplSpawner;	// �����̃_�u���|�C���^
	D3DXVECTOR3 m_posDest;	// �ڕW�ʒu
	MOVESTATE m_moveState;	// �ړ����
	int m_nScore;	// �X�R�A�l
	int m_nCntClose;	// �ڋ߃J�E���^�[

	static CEnemy *m_pHead;	// �擪�̃A�h���X
	static CEnemy *m_pTail;	// �Ō���̃A�h���X
	CEnemy *m_pPrev;	// �O�̃A�h���X
	CEnemy *m_pNext;	// ���̃A�h���X
};

#endif
