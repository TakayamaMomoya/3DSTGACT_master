//*****************************************************
//
// �w�i�̏���[bgmulti.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "main.h"
#include "bgmulti.h"
#include "manager.h"
#include "renderer.h"

//*****************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************
LPDIRECT3DTEXTURE9 CBgMulti::m_apTexture[MAX_BG] = {};	// �e�N�X�`���̃|�C���^

//=====================================================
// �R���X�g���N�^
//=====================================================
CBgMulti::CBgMulti(int nPriority) : CObject(nPriority)
{
	CBg *pBg;
	
	pBg = CBg::Create();
	pBg->SetScroll(-0.01f, 0.0f);
	
	pBg = CBg::Create();
	pBg->SetScroll(-0.035f, 0.0f);
}

//=====================================================
// �f�X�g���N�^
//=====================================================
CBgMulti::~CBgMulti()
{

}

//=====================================================
// ����������
//=====================================================
HRESULT CBgMulti::Init(void)
{


	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CBgMulti::Uninit(void)
{
	Release();
}

//=====================================================
// �X�V����
//=====================================================
void CBgMulti::Update(void)
{

}

//=====================================================
// �`�揈��
//=====================================================
void CBgMulti::Draw(void)
{

}

//=====================================================
// �Ǎ�����
//=====================================================
HRESULT CBgMulti::Load(void)
{
	if (m_apTexture[0] == nullptr)
	{
		// �t�@�C����
		const char *aPathFile[MAX_BG] =
		{
			"data\\TEXTURE\\BG\\bg000.jpg",
			"data\\TEXTURE\\BG\\bg001.png",
		};

		// �f�o�C�X�̎擾
		LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

		for (int nCntTex = 0; nCntTex < MAX_BG; nCntTex++)
		{
			D3DXCreateTextureFromFile
			(
				pDevice,
				aPathFile[nCntTex],
				&m_apTexture[nCntTex]
			);
		}
	}

	return S_OK;
}

//=====================================================
// �j������
//=====================================================
void CBgMulti::Unload(void)
{
	for (int nCntTex = 0; nCntTex < MAX_BG; nCntTex++)
	{
		if (m_apTexture[nCntTex] != nullptr)
		{
			m_apTexture[nCntTex]->Release();
			m_apTexture[nCntTex] = nullptr;
		}
	}
}

//=====================================================
// ��������
//=====================================================
CBgMulti *CBgMulti::Create(void)
{
	CBgMulti *pBgMulti = nullptr;

	if (pBgMulti == nullptr)
	{
		pBgMulti = new CBgMulti;
	}

	return pBgMulti;
}