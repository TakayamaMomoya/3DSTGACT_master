//*****************************************************
//
// ���[�_�[�̏���[rader.h]
// Author:���R����
//
//*****************************************************

#ifndef _RADER_H_
#define _RADER_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "meshcylinder.h"

//*****************************************************
// �N���X��`
//*****************************************************
class CRader : public CMeshCylinder
{
public:
	CRader(void);	// �R���X�g���N�^
	~CRader();	// �f�X�g���N�^

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CRader *Create(void);

private:
	void ChaseTarget(void);
	void Wave(void);

	D3DXVECTOR3 *m_pPosInitial;	// ���_�̏����ʒu
};
#endif