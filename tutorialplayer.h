//*****************************************************
//
// �`���[�g���A���v���C���[�̏���[tutorialplayer.cpp]
// Author:���R����
//
//*****************************************************

#ifndef _TUTORIALPLAYER_H_
#define _TUTORIALPLAYER_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "player.h"

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CTutorialPlayer : public CObject
{
public:
	typedef enum
	{// ���[�V�����̃^�C�v
		MOTION_NEUTRAL = 0,
		MOTION_WALK,
		MOTION_SHOT,
		MOTION_BOOST,
		MOTION_TURNWALK,
		MOTION_TURNBOOST,
		MOTION_MAX,
	}MOTION;

	typedef enum
	{// �u�[�X�g�̏��
		BOOSTSTATE_NONE = 0,	// ���ł��Ȃ�
		BOOSTSTATE_EMPTY,	// ���R���
		BOOSTSTATE_MAX
	}BOOSTSTATE;

	CTutorialPlayer(int nPriority = 4);	// �R���X�g���N�^
	~CTutorialPlayer();	// �f�X�g���N�^

	static CTutorialPlayer *Create(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	HRESULT Load(void);	// �Ǎ�
	void Hit(float fDamage);
	bool IsAim(void) { return m_bAim; }
	void SetPosition(D3DXVECTOR3 pos) { m_pos = pos; }
	D3DXVECTOR3 GetPosition(void) { return m_pos; }
	void SetMove(D3DXVECTOR3 move) { m_move = move; }
	D3DXVECTOR3 GetMove(void) { return m_move; }
	void SetPositionOld(D3DXVECTOR3 pos) { m_posOld = pos; }
	D3DXVECTOR3 GetPositionOld(void) { return m_posOld; }
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }
	D3DXVECTOR3 GetRot(void) { return m_rot; }
	void LimitUpperRot(float fRotOld);
	float GetBoost(void) { return m_fBoost; }
	BOOSTSTATE GetBoostState(void) { return m_boostState; }
	int GetAirCounter(void) { return m_nCounterAir; }
	void SetAirCounter(int nCounter) { m_nCounterAir = nCounter; }
	bool IsLand(void) { return m_bLand; }
	int GetLife(void) { return m_nLife; }
	float GetRapid(void) { return m_fParamRapid; }
	float GetPower(void) { return m_fParamPower; }
	float GetCost(void) { return m_fParamCost; }
	int GetNumShot(void) { return m_nNumShot; }
	int GetNumHit(void) { return m_nNumHit; }
	void SetNumHit(int nNumHit);
	void SetNumShot(int nNumShot) { m_nNumShot = nNumShot; }
	CAssess *GetAssess(void) { return m_pAssess; }
	CEnemy *GetLockEnemy(void) { return m_pEnemy; }

private:
	enum STATE
	{
		STATE_NONE = 0,	// ���ł��Ȃ����
		STATE_NORMAL,	// �ʏ���
		STATE_DEATH,	// ���S���
	};

	void Input(void);
	void Turn(D3DXVECTOR3 move, float fRotOld);
	void Lockon(void);
	void ManageState(void);
	void ManageOffset(void);
	void ManageMotion(void);
	void ManageBoost(float fBoostOld);
	void ManageCollision(void);
	bool CollisionField(void);
	void InputMove(void);
	void InputShot(void);
	void InputCamera(void);
	void SetMatrix(void);
	D3DXVECTOR3 LinePridiction(D3DXVECTOR3 pos, float fSpeedBullet, D3DXVECTOR3 posTarget, D3DXVECTOR3 moveTarget);
	float PlusMin(float fData1, float fData2);
	void Boost(void);
	void Death(void);

	int m_nLife;	// �̗�
	int m_nCntDeath;	// ���S�܂ł̃J�E���^�[
	float m_fBoost;	// �u�[�X�g�c��
	int m_nTimerRapid;	// �A�˂���܂ł̗P�\�̃J�E���^�[
	int m_nTimerShot;	// �P���Ō��Ă�Ԋu
	int m_nCounterAir;	// �󒆃J�E���^�[
	int m_nNumShot;	// �ˌ���
	int m_nNumHit;	// ������
	int m_nCntSound;	// �T�E���h�𗬂��J�E���^�[
	CShadow *m_pShadow;	// �e
	bool m_bAim;	// �G�C�����Ă��邩�ǂ���
	bool m_bLand;
	bool m_bSprint;	// �_�b�V����Ԃ��ǂ���
	bool m_bJump;	// �W�����v���Ă��邩�ǂ���
	bool m_bTurnDowner;	// �����g����]���Ă��邩�ǂ���
	CCollisionSphere *m_pCollisionSphere;
	CCollisionCube *m_pCollisionCube;
	STATE m_state;	// ���
	D3DXMATRIX m_mtxMazzle[2];	// �e���̃}�g���b�N�X
	D3DXVECTOR3 m_offsetMazzle[2];	// �e���̃I�t�Z�b�g�ʒu
	D3DXVECTOR3 m_posMazzle;	// �e�����W
	D3DXVECTOR3 m_pos;	// �ʒu
	D3DXVECTOR3 m_posOld;	// �O��̈ʒu
	D3DXVECTOR3 m_move;	// �ړ���
	D3DXVECTOR3 m_rot;	// ����
	D3DXVECTOR3 m_rotDest;	// ��������
	D3DXVECTOR3 m_rotUpper;	// �㔼�g�̌���
	D3DXVECTOR3 m_rotDowner;	// �����g�̌���
	D3DXMATRIX m_mtxLockon[2];	// ���b�N�I���̃}�g���b�N�X
	D3DXMATRIX m_mtxWorld;	// ���[���h�}�g���b�N�X
	D3DXMATRIX m_mtxWaist;	// ���̃}�g���b�N�X
	D3DXVECTOR3 m_offsetLockon[2];	// ���b�N�I���̃I�t�Z�b�g�ʒu
	D3DXVECTOR3 m_posLockon;	// ���b�N�I�����W�̃|�C���^
	D3DXVECTOR3 m_moveLockon;	// ���b�N�I�����Ă�G�̈ړ���
	D3DXVECTOR3 m_posBoost;	// �u�[�X�g���W
	D3DXVECTOR3 m_vecBoost;	// �u�[�X�g�x�N�g��
	CEnemy *m_pEnemy;	// ���b�N�I�����Ă�G�̃|�C���^
	CMotion *m_pBodyUpper;	// �㔼�g�̃|�C���^
	CMotion *m_pBodyDowner;	// �����g�̃|�C���^
	BOOSTSTATE m_boostState;	// �u�[�X�g�̏��
	CCockPit *m_pCockpit;	// �R�b�N�s�b�g�̃|�C���^
	CRader *m_pRader;	// ���[�_�[�̃|�C���^
	CAssess *m_pAssess;	// �]���Q�[�W�̃|�C���^
	CLockon *m_pLockon;	// ���b�N�I���̃|�C���^

	float m_fParamRapid;	// �A�˗͂̃p�����[�^�[
	float m_fParamPower;	// �U���n�̃p�����[�^�[
	float m_fParamCost;	// �����n�̃p�����[�^�[
	float m_fGrawRapid;	// �A�˗͐�����
	float m_fGrawPower;	// �U���͐�����
	float m_fGrawCost;	// ����������
};

#endif
