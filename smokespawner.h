//*****************************************************
//
// ���X�|�i�[����[smokespawner.h]
// Author:���R����
//
//*****************************************************

#ifndef _SMOKESPAWNER_H_
#define _SMOKESPAWNER_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "object.h"

//*****************************************************
// �N���X��`
//*****************************************************
class CSmokeSpawner : public CObject
{
public:
	CSmokeSpawner(void);	// �R���X�g���N�^
	~CSmokeSpawner();	// �f�X�g���N�^

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void) {};
	static CSmokeSpawner *Create(D3DXVECTOR3 pos, float fSpeed, int nLife,int nNumDebris);
	void SetPosition(D3DXVECTOR3 pos);
	D3DXVECTOR3 GetPosition(void) { return m_pos; }
	D3DXVECTOR3 GetPositionOld(void) { return D3DXVECTOR3(); }	// �擾����

private:
	D3DXVECTOR3 m_pos;	// �ʒu
	int m_nLife;	// ����
	float m_fSpeed;	// �X�s�[�h
	int m_nNumDebris;	// �f�u���̐�
};

#endif