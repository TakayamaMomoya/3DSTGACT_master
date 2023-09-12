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
	enum PARAM
	{
		PARAM_HIT = 0,	// ������
		PARAM_DODGE,	// ���
		PARAM_ATTACK,	// �U��
		PARAM_MAX
	};

	CAssess(int nPriority = 3);	// �R���X�g���N�^
	~CAssess();	// �f�X�g���N�^

	static CAssess *Create(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void AddParam(float fValue, PARAM param);
	float GetParam(PARAM param);

private:
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
		float fParam;	// �p�����[�^�[�l
		float fWidthDest;	// ���̖ړI�l
	};

	void ManageGauge(void);	// �Q�[�W�̊Ǘ�
	void PalsingGauge(Param *pParam);	// �Q�[�W�̖���
	void SetWidthDest(Param *pParam);	// �ړI�̕��ݒ�

	Param *m_apParam[PARAM_MAX];	// �p�����[�^�[�̏��
};

#endif