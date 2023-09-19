//*****************************************************
//
// �e�̏���[shadow.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "shadow.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "meshfield.h"
#include "game.h"
#include "objectmanager.h"

//=====================================================
// �R���X�g���N�^
//=====================================================
CShadow::CShadow(int nPriority) : CObject3D(nPriority)
{
	m_nor = { 0.0f,0.0f,0.0f };
}

//=====================================================
// �f�X�g���N�^
//=====================================================
CShadow::~CShadow()
{

}

//=====================================================
// ����������
//=====================================================
HRESULT CShadow::Init(void)
{
	// �p���N���X�̏�����
	CObject3D::Init();

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CShadow::Uninit(void)
{
	// �p���N���X�̏I��
	CObject3D::Uninit();
}

//=====================================================
// �X�V����
//=====================================================
void CShadow::Update(void)
{
	// �ϐ��錾
	float fHeight = 0.0f;

	CMeshField *pMesh = nullptr;
	CObjectManager *pObjManager = CManager::GetObjectManager();

	if (pObjManager != nullptr)
	{// ���b�V���t�B�[���h�̎擾
		pMesh = pObjManager->GetMeshField();
	}

	// �p���N���X�̍X�V
	CObject3D::Update();

	if (pMesh != nullptr)
	{
		fHeight = pMesh->GetHeight(D3DXVECTOR3(GetPosition().x, -100.0f, GetPosition().z), nullptr, &m_nor);
	}

	// �e�̈ʒu�ݒ�
	SetPosition(D3DXVECTOR3(GetPosition().x, fHeight + 3, GetPosition().z));
}

//=====================================================
// �`�揈��
//=====================================================
void CShadow::Draw(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �p���N���X�̕`��
	CObject3D::DrawNormal(m_nor);
}

//=====================================================
// ��������
//=====================================================
CShadow *CShadow::Create(D3DXVECTOR3 pos, float width, float height)
{
	CShadow *pShadow = nullptr;

	if (pShadow == nullptr)
	{
		pShadow = new CShadow;

		if (pShadow != nullptr)
		{
			pShadow->SetPosition(D3DXVECTOR3(pos));
			pShadow->SetSize(width, height);

			// ������
			pShadow->Init();

			// �e�N�X�`���̓Ǎ�
			int nIdx = CManager::GetTexture()->Regist("data\\TEXTURE\\EFFECT\\effect000.png");
			pShadow->SetIdxTexture(nIdx);

			pShadow->SetColor(D3DXCOLOR(0.0f,0.0f,0.0f,1.0f));
		}
	}

	return pShadow;
}