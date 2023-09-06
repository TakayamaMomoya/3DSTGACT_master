//*****************************************************
//
// ��������[explosion.h]
// Author:���R����
//
//*****************************************************

#ifndef _EXPLOSION_H_
#define _EXPLOSION_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "billboard.h"

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CExplosion : public CBillboard
{
public:
	CExplosion(int nPriority = 4);	// �R���X�g���N�^
	~CExplosion();	// �f�X�g���N�^

	static CExplosion *Create(D3DXVECTOR3 pos);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static HRESULT Load(void);	// �Ǎ�
	static void Unload(void);
	static int GetNumAll(void) { return m_nNumAll; }

private:
	int m_nLife;	// ����
	static int m_nNumAll;	// ����
	int m_nCounterAnim;	// �A�j���[�V�����J�E���^�[
	int m_nPatternAnim;	// �A�j���p�^�[��NO.
};

#endif