//*****************************************************
//
// �e�̏���[shadow.h]
// Author:���R����
//
//*****************************************************

#ifndef _SHADOW_H_
#define _SHADOW_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "object3D.h"

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CShadow : public CObject3D
{
public:
	CShadow(int nPriority = 6);	// �R���X�g���N�^
	~CShadow();	// �f�X�g���N�^

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CShadow *Create(D3DXVECTOR3 pos,float width,float height);
	void SetNormal(D3DXVECTOR3 nor) { m_nor = nor; }
	D3DXVECTOR3 GetNormal(void) { return m_nor; }

private:
	D3DXVECTOR3 m_nor;
};

#endif
