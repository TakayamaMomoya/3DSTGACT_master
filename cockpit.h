//*****************************************************
//
// �R�b�N�s�b�g�̏���[cockpit.h]
// Author:���R����
//
//*****************************************************

#ifndef _COCKPIT_H_
#define _COCKPIT_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "object.h"

//*****************************************************
// �O���錾
//*****************************************************
class CObject2D;
class CNumber;

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CCockPit : public CObject
{
public:
	CCockPit(int nPriority = 3);	// �R���X�g���N�^
	~CCockPit();	// �f�X�g���N�^

	static CCockPit *Create(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

private:
	enum PARAM
	{
		PARAM_RAPID = 0,	// �A�˗�
		PARAM_POWER,	// �U����
		PARAM_COST,	// ����
		PARAM_MAX
	};
	void ManageLife(void);
	void ManageParam(void);

	CObject2D *m_pFrame;	// �t���[���̃|�C���^
	CObject2D *m_apGauge[PARAM_MAX];	// �Q�[�W�̃|�C���^
	CObject2D *m_apFrame[PARAM_MAX];	// �t���[���̃|�C���^
	CObject2D *m_apIcon[PARAM_MAX];	// �A�C�R���̃|�C���^
	CNumber *m_pLife;	// �c��̗͕\��
};

#endif
