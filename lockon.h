//*****************************************************
//
// ���b�N�I���̏���[lockon.h]
// Author:���R����
//
//*****************************************************

#ifndef _LOCKON_H_
#define _LOCKON_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "object.h"

//*****************************************************
// �O���錾
//*****************************************************
class CBillboard;
class CEnemy;

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CLockon : public CObject
{
public:
	CLockon(int nPriority = 3);	// �R���X�g���N�^
	~CLockon();	// �f�X�g���N�^

	static CLockon *Create(void);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void SetEnemy(CEnemy *pEnemy) { m_pEnemy = pEnemy; }

private:
	CBillboard *m_pLockOn;	// ���b�N�I���}�[�J�[�̃r���{�[�h
	CEnemy *m_pEnemy;	// ���Ă�G�̃|�C���^
	float m_fSize;	// �T�C�Y
};

#endif
