//*****************************************************
//
// ���[�_�[�̏���[rader.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "rader.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"
#include "player.h"
#include "enemy.h"
#include "debugproc.h"
#include "universal.h"
#include "objectmanager.h"

//*****************************************************
// �}�N����`
//*****************************************************
#define MESHCYLINDER_TEX_FILE		"data\\TEXTURE\\BG\\sky000.png"				// �e�N�X�`���t�@�C����
#define MESH_LENGTH					(230.0f)									// ���b�V���̈�ӂ̒���
#define MESH_U						(256)										// ���̃u���b�N��
#define MESH_V						(1)	// �c�̃u���b�N��
#define SPLIT_TEX_U					(3)	// ���̃e�N�X�`��������
#define SPLIT_TEX_V					(1)	// �c�̃e�N�X�`��������
#define MESH_HEIGHT	(10.0f)	// ���b�V���̍���
#define ADD_HEIGHT	(100.0f)	// �ڕW����̍���
#define WAVE_ANGLE	(D3DX_PI * 0.1f)	// �g�ł͈͂̊p�x
#define WAVE_HEIGHT	(50.0f)	// �g�ł���
#define WAVE_LENGTH	(100.0f)	// �g�ł͈͂̋���
#define CHANGE_COl	(0.15f)	// �F�̕ς�銄��
#define RAND_HEIGHT	(10.0f)	// �U��镝
#define WAVE_SIZE	(0.04f)	// �g�̑傫��
#define SHAKE_SIZE	(0.005f)	// �h��̑傫��
#define WAVE_SPEED	(0.08f)	// �g�̓������x

//=====================================================
// �R���X�g���N�^
//=====================================================
CRader::CRader(void)
{
	m_pPosInitial = nullptr;
}

//=====================================================
// �f�X�g���N�^
//=====================================================
CRader::~CRader()
{

}

//=====================================================
// ��������
//=====================================================
CRader *CRader::Create(void)
{
	CRader *pMeshCylinder = nullptr;

	if (pMeshCylinder == nullptr)
	{
		pMeshCylinder = new CRader;

		if (pMeshCylinder != nullptr)
		{
			pMeshCylinder->Init();
		}
	}

	return pMeshCylinder;
}

//=====================================================
//����������
//=====================================================
HRESULT CRader::Init(void)
{
	// �p���N���X�̏�����
	CMeshCylinder::Init();

	// �����ʒu��񐶐�
	if (m_pPosInitial == nullptr)
	{
		int nNumVtx = GetNumVtx();

		m_pPosInitial = new D3DXVECTOR3[nNumVtx];

		if (m_pPosInitial != nullptr)
		{
			// ��񏉊���
			ZeroMemory(m_pPosInitial, sizeof(D3DXVECTOR3) * nNumVtx);

			// �����ʒu���
			// ���_�o�b�t�@�擾
			LPDIRECT3DVERTEXBUFFER9 pVtxBuff = CMeshCylinder::GetVtxBuff();

			// ���_���̃|�C���^
			VERTEX_3D *pVtx;

			// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
			pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

			for (int nCnt = 0; nCnt < nNumVtx; nCnt++)
			{
				m_pPosInitial[nCnt] = pVtx[nCnt].pos;
			}

			// ���_�o�b�t�@���A�����b�N
			pVtxBuff->Unlock();

		}
	}

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CRader::Uninit(void)
{
	if (m_pPosInitial != nullptr)
	{// �ʒu���̔j��
		delete m_pPosInitial;
		m_pPosInitial = nullptr;
	}

	// �p���N���X�̏I��
	CMeshCylinder::Uninit();
}

//=====================================================
// �X�V����
//=====================================================
void CRader::Update(void)
{
	// �p���N���X�̍X�V
	CMeshCylinder::Update();

	// �ڕW�ɂ��Ă���
	ChaseTarget();

	// �g�`�ɓ�������
	Wave();
}

//=====================================================
// �ڕW�ɂ��Ă���
//=====================================================
void CRader::ChaseTarget(void)
{
	CPlayer *pPlayer = nullptr;
	CObjectManager *pObjManager = CManager::GetObjectManager();

	if (pObjManager != nullptr)
	{// �v���C���[�̓K�p
		pPlayer = pObjManager->GetPlayer();
	}

	if (pPlayer == nullptr)
	{
		return;
	}

	// �ʒu�̎擾
	D3DXVECTOR3 pos = pPlayer->GetPosition();

	pos.y += ADD_HEIGHT;

	SetPosition(pos);
}

//=====================================================
// �g�`�ɓ�������
//=====================================================
void CRader::Wave(void)
{
	// �ėp�����擾
	CUniversal *pUniversal = CManager::GetUniversal();

	// ���_�o�b�t�@�擾
	LPDIRECT3DVERTEXBUFFER9 pVtxBuff = CMeshCylinder::GetVtxBuff();

	// ���S�I�u�W�F�N�g�̎擾
	CEnemy *pEnemy = CEnemy::GetEnemy();
	CPlayer *pPlayer = nullptr;
	CObjectManager *pObjManager = CManager::GetObjectManager();

	if (pObjManager != nullptr)
	{// �v���C���[�̓K�p
		pPlayer = pObjManager->GetPlayer();
	}

	if (pPlayer == nullptr)
	{
		return;
	}

	// �v�Z�p�ϐ�
	int nIdxStart = 0;
	int nCntEnd = 0;
	int nIdx;
	D3DXVECTOR3 vecDiff;
	D3DXVECTOR3 pos = pPlayer->GetPosition();
	D3DXVECTOR3 *pPosDest = nullptr;
	D3DXCOLOR col = { 1.0f,1.0f,1.0f,1.0f };
	float fRot;
	float fRotDiff;
	float fSin;

	// ���_�����̖ړI���W����
	pPosDest = new D3DXVECTOR3[GetNumVtx()];

	if (pPosDest == nullptr)
	{// �����ł��Ȃ������ꍇ�������I��
		return;
	}

	// ���_���̃|�C���^
	VERTEX_3D *pVtx;

	fRot = D3DX_PI * 2 / MESH_U;

	nCntEnd = (int)(WAVE_ANGLE / fRot) * 2;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < MESH_U + 1; nCnt++)
	{
		pPosDest[nCnt] = m_pPosInitial[nCnt];
		pPosDest[nCnt + MESH_U + 1] = m_pPosInitial[nCnt + MESH_U + 1];
		pVtx[nCnt].col = col;
		pVtx[nCnt + MESH_U + 1].col = col;
	}

	fSin = D3DX_PI / nCntEnd;

	while (pEnemy != nullptr)
	{
		// ���̃A�h���X��ۑ�
		CEnemy *pEnemyNext = pEnemy->GetNext();

		// �����p�x���v�Z
		vecDiff = pos - pEnemy->GetPosition();

		fRotDiff = atan2f(vecDiff.x, vecDiff.z) + D3DX_PI;

		// �X�^�[�g�̊������v�Z
		nIdxStart = (int)((fRotDiff - WAVE_ANGLE) / fRot);

		if (nIdxStart < 0)
		{
			nIdxStart = MESH_U + nIdxStart;
		}

		nIdx = nIdxStart;

		// �U�ꕝ�̎Z�o
		float fDiff = (float)(rand() % (int)(RAND_HEIGHT * 10) * 0.1f);

		// �ڕW�����ɂ��钸�_�����o
		for (int nCnt = 0; nCnt < nCntEnd; nCnt++)
		{
			// ���������̊��������߂�
			float fRate = sinf(fSin * nCnt);
			float fHeight = fRate * WAVE_HEIGHT + fDiff;

			pPosDest[nIdx].x += m_pPosInitial[nIdx].x * WAVE_SIZE * fRate + m_pPosInitial[nIdx].x * SHAKE_SIZE * fDiff;
			pPosDest[nIdx].z += m_pPosInitial[nIdx].z * WAVE_SIZE * fRate + m_pPosInitial[nIdx].z * SHAKE_SIZE * fDiff;

			pPosDest[nIdx + MESH_U + 1].x += m_pPosInitial[nIdx].x * WAVE_SIZE * fRate + m_pPosInitial[nIdx].x * SHAKE_SIZE * fDiff;
			pPosDest[nIdx + MESH_U + 1].z += m_pPosInitial[nIdx].z * WAVE_SIZE * fRate + m_pPosInitial[nIdx].z * SHAKE_SIZE * fDiff;

			col = (D3DXCOLOR)pVtx[nIdx].col;

			col.g -= CHANGE_COl;
			col.b -= CHANGE_COl;

			pVtx[nIdx].col = col;
			pVtx[nIdx + MESH_U + 1].col = col;

			// ���[�v�J�E���g�A�b�v
			nIdx = (nIdx + 1) % (MESH_U + 1);
		}

		pEnemy = pEnemyNext;
	}

	for (int nCnt = 0; nCnt < MESH_U + 1; nCnt++)
	{
		pVtx[nCnt].pos += (pPosDest[nCnt] - pVtx[nCnt].pos) * WAVE_SPEED;
		pVtx[nCnt + MESH_U + 1].pos += (pPosDest[nCnt + MESH_U + 1] - pVtx[nCnt + MESH_U + 1].pos) * WAVE_SPEED;
	}

	// �[�̒��_���m���Ȃ���
	pVtx[MESH_U].pos = pVtx[0].pos;
	pVtx[MESH_U * 2 + 1].pos = pVtx[MESH_U + 1].pos;

	// ���_�o�b�t�@���A�����b�N
	pVtxBuff->Unlock();

	if (pPosDest != nullptr)
	{// �ړI���W�̔j��
		delete[] pPosDest;
		pPosDest = nullptr;
	}
}

//=====================================================
// �`�揈��
//=====================================================
void CRader::Draw(void)
{
	CMeshCylinder::Draw();
}