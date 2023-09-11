//*****************************************************
//
// �]���̏���[assess.h]
// Author:���R����
//
//*****************************************************

#ifndef _ASSESS_H_
#define _ASSESS_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "object.h"

//*****************************************************
// �O���錾
//*****************************************************
class CObject2D;

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CAssess : public CObject
{
public:
	CAssess(int nPriority = 3);	// �R���X�g���N�^
	~CAssess();	// �f�X�g���N�^

	static CAssess *Create(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void AddHit(float fHit) { m_fAssessHit += fHit; }
	void AddDodge(float fDodge) { m_fAssessDodge += fDodge; }
	void AddAttack(float fAttack) { m_fAssessAttack += fAttack; }
	float GetHit(void) { return m_fAssessHit; }
	float GetDodge(void) { return m_fAssessDodge; }
	float GetAttack(void) { return m_fAssessAttack; }

private:
	enum PARAM
	{
		PARAM_HIT = 0,	// ������
		PARAM_DODGE,	// ���
		PARAM_ATTACK,	// �U��
		PARAM_MAX
	};

	enum GAUGESTATE
	{
		GAUGESTATE_EXTEND = 0,	// �L�т���
		GAUGESTATE_SHRINK,	// �k�ޏ��
		GAUGESTATE_MAX
	};

	struct Param
	{
		CObject2D *pGauge;	// �Q�[�W�̃|�C���^
		CObject2D *pFrame;	// �t���[���̃|�C���^
		GAUGESTATE state;	// ���
	};

	void ManageGauge(void);	// �Q�[�W�̊Ǘ�
	void LimitAssess(void);	// �]���̐���

	float m_fAssessHit;	// �q�b�g���̕]��
	float m_fAssessDodge;	// ��𗦂̕]��
	float m_fAssessAttack;	// �U�������̕]��
	Param *m_apParam[PARAM_MAX];	// �p�����[�^�[�̏��
};

#endif