//*****************************************************
//
// �A�C�e���̏���[item.cpp]
// Author:���R����
//
//*****************************************************

#ifndef _ITEM_H_
#define _ITEM_H_

#include "Object2D.h"

//*****************************************************
// �O���錾
//*****************************************************
class CNumber;

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CItem : public CObject2D
{
public:
	typedef enum
	{
		TYPE_NONE = 0,	// ���ł��Ȃ�
		TYPE_DEBRIS,	// �c�[
		TYPE_MAX
	}TYPE;
	CItem();	// �R���X�g���N�^
	~CItem();	// �f�X�g���N�^

	static CItem *Create(D3DXVECTOR3 pos,TYPE type, float height = 40.0f, float width = 40.0f);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	bool CollisionPlayer(D3DXVECTOR3 pos);
	static HRESULT Load(void);	// �Ǎ�
	static void Unload(void);
	static int GetNumAll(void) { return m_nNumAll; }

private:
	static LPDIRECT3DTEXTURE9 m_pTexture;	// �e�N�X�`���ւ̃|�C���^
	static int m_nNumAll;	// ����
};

#endif
