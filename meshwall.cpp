//*****************************************************
//
// ���b�V���E�H�[���̏���[meshwall.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "meshwall.h"
#include "manager.h"
#include "renderer.h"
#include "player.h"
#include "debugproc.h"
#include "game.h"
#include <stdio.h>

//*****************************************************
// �}�N����`
//*****************************************************
#define MESHFIELD_TEX_FILE			"data\\TEXTURE\\MATERIAL\\wall003.png"				// �e�N�X�`���t�@�C����
#define MOVE_SPEED					(1.0f)										// �ړ����x
#define MESH_LENGTH					(200.0f)									// ���b�V���̈�ӂ̒���
#define MESH_U						(254)											// ���̃u���b�N��
#define MESH_V						(254)											// �c�̃u���b�N��
#define SPLIT_TEX					(20)										// �e�N�X�`��������

//=====================================================
// �R���X�g���N�^
//=====================================================
CMeshWall::CMeshWall(int nPriority) : CObject(nPriority)
{
	ZeroMemory(&m_MeshWall,sizeof(m_MeshWall));
	m_pIdxBuff = nullptr;
	m_pVtxBuff = nullptr;
	m_pTexture = nullptr;
	m_fLengthMesh = 0.0f;
	m_nDivNumU = 0;
	m_nDivNumV = 0;
	m_col = { 0.0f,0.0f,0.0f,0.0f };
}

//=====================================================
// �f�X�g���N�^
//=====================================================
CMeshWall::~CMeshWall()
{

}

//=====================================================
// ����������
//=====================================================
HRESULT CMeshWall::Init(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �l�̏����ݒ�
	m_fLengthMesh = MESH_LENGTH;
	m_nDivNumU = MESH_U;
	m_nDivNumV = MESH_V;
	m_nDivTex = SPLIT_TEX;
	m_col = { 1.0f,0.0f,0.0f,0.0f };

	m_MeshWall.nNumVtx = (m_nDivNumU + 1) * (m_nDivNumV + 1);

	// �e�N�X�`���ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		MESHFIELD_TEX_FILE,
		&m_pTexture);

	if (m_pVtxBuff == nullptr)
	{
		// ���_�o�b�t�@�̐���
		pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_MeshWall.nNumVtx,
			D3DUSAGE_WRITEONLY,
			FVF_VERTEX_3D,
			D3DPOOL_MANAGED,
			&m_pVtxBuff,
			nullptr);
	}

	// ���_���̃|�C���^
	VERTEX_3D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	int nCountV, nCountU;
	int nCntVtx = 0;
	float fAngleX, fAngleY;
	float fLength;
	D3DXVECTOR3 pos;

	for (nCountV = 0;nCountV < m_nDivNumV + 1; nCountV++)
	{// ���_���W�̐ݒ�
		for (nCountU = 0; nCountU < m_nDivNumU + 1; nCountU++)
		{
			// ���_���W
			pVtx[nCountV * (m_nDivNumU + 1) + nCountU].pos.x = (nCountU - m_nDivNumU * 0.5f) * m_fLengthMesh;
			pVtx[nCountV * (m_nDivNumU + 1) + nCountU].pos.y = (((m_nDivNumU) * 0.5f) - nCountV) * m_fLengthMesh;
			pVtx[nCountV * (m_nDivNumU + 1) + nCountU].pos.z = 0.0f;

			// X���p�x���v�Z
			fAngleX = atan2f(pVtx[nCountV * (m_nDivNumU + 1) + nCountU].pos.x, pVtx[nCountV * (m_nDivNumU + 1) + nCountU].pos.y);

			// Y���p�x���v�Z
			fAngleY = atan2f(pVtx[nCountV * (m_nDivNumU + 1) + nCountU].pos.x, pVtx[nCountV * (m_nDivNumU + 1) + nCountU].pos.z);

			if (pVtx[nCountV * (m_nDivNumU + 1) + nCountU].pos.x < 0.0f)
			{
				fAngleY += D3DX_PI;
			}

			// �������v�Z
			fLength = D3DXVec3Length(&pVtx[nCountV * (m_nDivNumU + 1) + nCountU].pos);

			// �p�x�Ƌ��������ƂɈʒu���Đݒ�
			pos =
			{
				m_MeshWall.pos.x + sinf(fAngleX + m_MeshWall.rot.x) * sinf(fAngleY + m_MeshWall.rot.y) * fLength,
				m_MeshWall.pos.y + cosf(fAngleX + m_MeshWall.rot.x) * fLength,
				m_MeshWall.pos.z + sinf(fAngleX + m_MeshWall.rot.x) * cosf(fAngleY + m_MeshWall.rot.y) * fLength
			}; 

			pVtx[nCountV * (m_nDivNumU + 1) + nCountU].pos = pos;

			// �e�N�X�`�����W
			pVtx[nCountV * (m_nDivNumU + 1) + nCountU].tex = D3DXVECTOR2
			(
			    ((float)m_nDivTex / m_nDivNumU) * nCountU,
				((float)m_nDivTex / m_nDivNumU) * nCountV
			);
		}
	}

	for (int nCnt = 0;nCnt < m_MeshWall.nNumVtx;nCnt++)
	{
		// ���_�J���[�̐ݒ�
		pVtx[nCnt].col = m_col;

		pVtx[nCnt].nor = D3DXVECTOR3(0.0f,0.0f,1.0f);
	}

	// ���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();

	m_MeshWall.nNumIdx = (m_nDivNumU + 1) * (m_nDivNumV + 1) + ((m_nDivNumV + 1) - 2) * (m_nDivNumU + 1) + (((m_nDivNumV + 1) - 2) * 2);

	// �C���f�b�N�X�o�b�t�@�̐���
	pDevice->CreateIndexBuffer(sizeof(WORD) * m_MeshWall.nNumIdx,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIdxBuff,
		nullptr);

	// �C���f�b�N�X�o�b�t�@�ւ̃|�C���^
	WORD *pIdx;

	// �C���f�b�N�X�o�b�t�@�����b�N���A���_�ԍ��ւ̃|�C���^���擾
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCount = 0; nCount < m_MeshWall.nNumIdx / 2; nCount++)
	{// �C���f�b�N�X����
		if (
			nCount % ((m_nDivNumU + 1) + (m_nDivNumU + 2) * (nCount / (m_nDivNumU + 2))) == 0
			&& nCount != 0
			)
		{
			pIdx[nCount * 2 + 1] = ((m_nDivNumU + 1) * 2) + (m_nDivNumU + 1) * (nCount / (m_nDivNumU + 2));

			pIdx[nCount * 2] = m_nDivNumU + (m_nDivNumU + 1) * (nCount / (m_nDivNumU + 2));
		}
		else
		{
			pIdx[nCount * 2 + 1] = nCount - (nCount / (m_nDivNumU + 2));

			pIdx[nCount * 2] = (nCount - (nCount / (m_nDivNumU + 2))) + (m_nDivNumU + 1);
		}
	}

	// �C���f�b�N�X�o�b�t�@���A�����b�N
	m_pIdxBuff->Unlock();

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CMeshWall::Uninit(void)
{
	if (m_pVtxBuff != nullptr)
	{// ���_�o�b�t�@�|�C���^�̔j��
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

	if (m_pIdxBuff != nullptr)
	{// �C���f�b�N�X�o�b�t�@�|�C���^�̔j��
		m_pIdxBuff->Release();
		m_pIdxBuff = nullptr;
	}

	Release();
}

//=====================================================
// �X�V����
//=====================================================
void CMeshWall::Update(void)
{
	
}

//=====================================================
// �v���C���[���߂Â�����F��ς��鏈��
//=====================================================
void CMeshWall::CheckPlayer(void)
{
	// �v���C���[�̎擾
	CPlayer *pPlayer = CGame::GetPlayer();

	if (pPlayer == nullptr)
	{
		return;
	}

	// �v�Z�p�ϐ�
	D3DXVECTOR3 vecDiff;
	float fLength;

	// ���_���̃|�C���^
	VERTEX_3D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntVtx = 0; nCntVtx < m_MeshWall.nNumVtx; nCntVtx++)
	{
		// �ʒu�̍������擾
		vecDiff = pPlayer->GetPosition() - pVtx[nCntVtx].pos;

		// �����������擾
		fLength = D3DXVec3Length(&vecDiff);

		if (fLength < CHENGE_LENGTH)
		{
			pVtx[nCntVtx].col = D3DXCOLOR(1.0f,0.0f,0.0f,1.0f - (fLength / CHENGE_LENGTH));
		}
	}

	// ���_�o�b�t�@���A�����b�N
	m_pVtxBuff->Unlock();
}

//=====================================================
// �`�揈��
//=====================================================
void CMeshWall::Draw(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;

	// �J�����O�𖳌���
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	//���[���h�}�g���b�N�X������
	D3DXMatrixIdentity(&m_MeshWall.mtxWorld);

	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll (&mtxRot,
		0.0f, 0.0f, 0.0f);
	D3DXMatrixMultiply(&m_MeshWall.mtxWorld, &m_MeshWall.mtxWorld,&mtxRot);

	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans,
		0.0f, 0.0f, 0.0f);
	D3DXMatrixMultiply(&m_MeshWall.mtxWorld, &m_MeshWall.mtxWorld, &mtxTrans);

	//���[���h�}�g���b�N�X�ݒ�
	pDevice->SetTransform(D3DTS_WORLD,&m_MeshWall.mtxWorld);

	//���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	//�C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetIndices(m_pIdxBuff);

	//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	//�e�N�X�`���ݒ�
	pDevice->SetTexture(0, m_pTexture);

	//�|���S���`��
	pDevice->DrawIndexedPrimitive (D3DPT_TRIANGLESTRIP, 0, 0, m_MeshWall.nNumVtx, 0, m_MeshWall.nNumIdx - 2);

	// �J�����O��L����
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

//=====================================================
// ��������
//=====================================================
CMeshWall *CMeshWall::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	CMeshWall *pMeshField = nullptr;

	if (pMeshField == nullptr)
	{
		pMeshField = new CMeshWall;

		if (pMeshField != nullptr)
		{
			pMeshField->m_MeshWall.pos = pos;
			pMeshField->m_MeshWall.rot = rot;
			pMeshField->Init();
		}
	}

	return pMeshField;
}