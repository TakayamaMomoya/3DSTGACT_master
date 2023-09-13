//*****************************************************
//
// �{�[�i�X�̕\��[bonus.h]
// Author:���R����
//
//*****************************************************

#ifndef _BONUS_H_
#define _BONUS_H_

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
class CBonus : public CObject
{
public:
	enum TYPE
	{
		TYPE_DOGDE = 0,	// ���
		TYPE_MISSILE,	// �~�T�C�����j
		TYPE_MANUALHIT,	// �}�j���A���Ə��Ŗ���
		TYPE_MAX
	};

	CBonus(int nPriority = 1);	// �R���X�g���N�^
	~CBonus();	// �f�X�g���N�^

	static CBonus *Create(TYPE type);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	enum STATE
	{
		STATE_NONE = 0,	// ���ł��Ȃ����
		STATE_IN,	// �o�����
		STATE_WAIT,	// �ҋ@���
		STATE_OUT,	// ���ŏ��
		STATE_MAX
	};

	void MoveToDest(void);
	void ManageState(void);
	void FollowCaption(void);
	int BonusScore(TYPE type);

	CObject2D *m_pCaption;	// ���o���̃|�C���^
	CNumber *m_pNumber;	// �{�[�i�X�����̃|�C���^
	D3DXVECTOR3 m_posDest;	// �ڕW�ʒu
	STATE m_state;	// ���
	int m_nTimerOut;	// �t�F�[�h�A�E�g�܂ł̃^�C�}�[

	static CBonus *m_pHead;	// �擪�̃A�h���X
	static CBonus *m_pTail;	// �Ō���̃A�h���X
	CBonus *m_pPrev;	// �O�̃A�h���X
	CBonus *m_pNext;	// ���̃A�h���X
};

#endif
