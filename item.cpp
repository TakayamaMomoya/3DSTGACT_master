//*****************************************************
//
// �A�C�e���̏���[item.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "main.h"
#include "item.h"
#include "renderer.h"
#include "debugproc.h"
#include "manager.h"
#include "object.h"
#include "number.h"

//*****************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************
LPDIRECT3DTEXTURE9 CItem::m_pTexture = nullptr;	// �e�N�X�`���̃|�C���^
int CItem::m_nNumAll = 0;	// ����

//=====================================================
// �R���X�g���N�^
//=====================================================
CItem::CItem()
{
	// �����J�E���g�A�b�v
	m_nNumAll++;
}

//=====================================================
// �f�X�g���N�^
//=====================================================
CItem::~CItem()
{
	m_nNumAll--;
}

//=====================================================
// ����������
//=====================================================
HRESULT CItem::Init(void)
{
	// �p���N���X�̏�����
	CObject2D::Init();

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CItem::Uninit(void)
{
	// �p���N���X�̏I��
	CObject2D::Uninit();
}

//=====================================================
// �X�V����
//=====================================================
void CItem::Update(void)
{
	// �p���N���X�̍X�V
	CObject2D::Update();

	// ���_�ʒu�ݒ�
	CObject2D::SetVtx();
}

//=====================================================
// �`�揈��
//=====================================================
void CItem::Draw(void)
{
	// �p���N���X�̕`��
	CObject2D::Draw();

}

//=====================================================
// ��������
//=====================================================
CItem *CItem::Create(D3DXVECTOR3 pos, TYPE type, float height, float width)
{
	CItem *pItem = nullptr;

	if (pItem == nullptr)
	{// �C���X�^���X����
		pItem = new CItem;

		// ����������
		pItem->Init();

		pItem->SetSize(width, height);
		pItem->SetPosition(D3DXVECTOR3(pos));
	}

	return pItem;
}

//=====================================================
// �Ǎ�����
//=====================================================
HRESULT CItem::Load(void)
{
	if (m_pTexture == nullptr)
	{
		// �f�o�C�X�̎擾
		LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

		D3DXCreateTextureFromFile
		(
			pDevice,
			"data\\TEXTURE\\ITEM\\item000.png",
			&m_pTexture
		);
	}

	return S_OK;
}

//=====================================================
// �e�N�X�`���j��
//=====================================================
void CItem::Unload(void)
{
	if (m_pTexture != nullptr)
	{
		m_pTexture->Release();
		m_pTexture = nullptr;
	}
}