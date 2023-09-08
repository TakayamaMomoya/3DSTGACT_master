//*****************************************************
//
// �����N�̏���[rank.h]
// Author:���R����
//
//*****************************************************

#ifndef _RANK_H_
#define _RANK_H_

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
class CRank : public CObject
{
public:
	CRank(int nPriority = 3);	// �R���X�g���N�^
	~CRank();	// �f�X�g���N�^

	static CRank *Create(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	void AddProgress(float fValue);

private:
	void ManageGauge(void);	// �i�s�Q�[�W�̊Ǘ�

	CObject2D *m_pFrame;	// �t���[���̃|�C���^
	CObject2D *m_pGaugeProgress;	// �i�s�x�����̃Q�[�W
	CNumber *m_pRankNum;	// �����N�̃|�C���^
	float m_fProgress;	// �����N�̐i�s�x��
	int m_nRank;	// �����N
};

#endif