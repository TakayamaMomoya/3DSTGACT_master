//*****************************************************
//
// �w�i�̏���[bgmulti.cpp]
// Author:���R����
//
//*****************************************************

#ifndef _BGMULTI_H_
#define _BGMULTI_H_

#include "object.h"
#include "bg.h"

//*****************************************************
// �}�N����`
//*****************************************************
#define MAX_BG	(3)	// �w�i�̍ő吔

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CBgMulti : public CObject
{
public:
	CBgMulti(int nPriority = 0);	// �R���X�g���N�^
	~CBgMulti();	// �f�X�g���N�^

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	D3DXVECTOR3 GetPosition(void) { return D3DXVECTOR3(); }	// �擾����
	D3DXVECTOR3 GetPositionOld(void) { return D3DXVECTOR3(); }	// �擾����
	static HRESULT Load(void);	// �Ǎ�
	static void Unload(void);
	static CBgMulti *Create(void);	// ��������
	float GetWidth(void) { return 0.0f; }	// �T�C�Y�擾
	float GetHeight(void) { return 0.0f; }	// �T�C�Y�擾

private:
	static LPDIRECT3DTEXTURE9 m_apTexture[MAX_BG];	// �e�N�X�`���ւ̃|�C���^
};

#endif
