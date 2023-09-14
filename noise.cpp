//*****************************************************
//
// �m�C�Y����[noise.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "noise.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//*****************************************************
// �}�N����`
//*****************************************************
#define ANIM_TIME				(6)	//�A�j���[�V�����̑��x
#define NUM_ANIM	(8)	// �A�j���[�V�����̖���

//*****************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************
int CNoise::m_nNumAll = 0;	// ����

//=====================================================
// �R���X�g���N�^
//=====================================================
CNoise::CNoise(int nPriority) : CAnim2D(nPriority)
{
	m_nLife = 0;
	m_nAlpha = 0;
	m_nAddAlpha = 0;

	m_nNumAll++;
}

//=====================================================
// �f�X�g���N�^
//=====================================================
CNoise::~CNoise()
{
	m_nNumAll--;
}

//=====================================================
// ����������
//=====================================================
HRESULT CNoise::Init(void)
{
	// �p���N���X�̏�����
	CAnim2D::Init();

	// ���ݒ�
	SetInfo(0,6,3,1);

	// �T�C�Y�ݒ�
	SetSize(SCREEN_WIDTH * 0.5f,SCREEN_HEIGHT * 0.5f);

	// �ʒu�ݒ�
	SetPosition(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f));

	SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CNoise::Uninit(void)
{
	// �p���N���X�̏I��
	CAnim2D::Uninit();
}

//=====================================================
// �X�V����
//=====================================================
void CNoise::Update(void)
{
	// �p���N���X�̍X�V
	CAnim2D::Update();

	m_nLife--;

	m_nAlpha += m_nAddAlpha;

	if (m_nLife <= 0)
	{
		Uninit();
	}
}

//=====================================================
// �`�揈��
//=====================================================
void CNoise::Draw(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �A���t�@�e�X�g�̗L����
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, m_nAlpha);

	// �p���N���X�̕`��
	CAnim2D::Draw();

	// �A���t�@�e�X�g�̖�����
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
}

//=====================================================
// ��������
//=====================================================
CNoise *CNoise::Create(int nLife)
{
	CNoise *pNoise = nullptr;

	pNoise = new CNoise;
	
	if (pNoise != nullptr)
	{
		// ����������
		pNoise->Init();

		pNoise->m_nLife = nLife;

		pNoise->m_nAddAlpha = 255 / nLife;

		// �e�N�X�`���̓Ǎ�
		int nIdx = CManager::GetTexture()->Regist("data\\TEXTURE\\EFFECT\\noise00.png");
		pNoise->SetIdxTexture(nIdx);
	}

	return pNoise;
}