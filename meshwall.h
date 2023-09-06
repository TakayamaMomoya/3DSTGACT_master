//*****************************************************
//
// ���b�V���E�H�[���̏���[meshwall.h
// Author:���R����
//
//*****************************************************

#ifndef _MESHWALL_H_
#define _MESHWALL_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "object.h"

//*****************************************************
// �}�N����`
//*****************************************************
#define CHENGE_LENGTH	(1000)	// �F��ς��鋗��

//*****************************************************
// �N���X��`
//*****************************************************
class CMeshWall : public CObject
{
public:
	CMeshWall(int nPriority = 5);	// �R���X�g���N�^
	~CMeshWall();	// �f�X�g���N�^

	typedef struct
	{
		D3DXVECTOR3 pos;						//�ʒu
		D3DXVECTOR3 rot;						//����
		D3DXMATRIX mtxWorld;					//���[���h�}�g���b�N�X
		int nNumIdx;							//�C���f�b�N�X��
		int nNumVtx;							//���_��
	}MeshWall;

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CMeshWall *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);
	void CheckPlayer(void);

private:
	LPDIRECT3DTEXTURE9 m_pTexture;	//�e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	//���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff;	//�C���f�b�N�X�o�b�t�@�ւ̃|�C���^
	MeshWall m_MeshWall;	//�\���̂̏��
	D3DXCOLOR m_col;	// �F
	float m_fLengthMesh;	// ���b�V����ӂ̒���
	int m_nDivNumU;
	int m_nDivNumV;
	int m_nDivTex;
};
#endif