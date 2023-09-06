//*****************************************************
//
// ��������[explosion.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "explosion.h"
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
int CExplosion::m_nNumAll = 0;	// ����

//=====================================================
// �R���X�g���N�^
//=====================================================
CExplosion::CExplosion(int nPriority) : CBillboard(nPriority)
{
	m_nLife = 0;
	m_nCounterAnim = 0;
	m_nPatternAnim = 0;

	m_nNumAll++;
}

//=====================================================
// �f�X�g���N�^
//=====================================================
CExplosion::~CExplosion()
{
	m_nNumAll--;
}

//=====================================================
// ����������
//=====================================================
HRESULT CExplosion::Init(void)
{
	// �p���N���X�̏�����
	CBillboard::Init();

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CExplosion::Uninit(void)
{
	// �p���N���X�̏I��
	CBillboard::Uninit();
}

//=====================================================
// �X�V����
//=====================================================
void CExplosion::Update(void)
{
	// ��������
	m_nLife--;

	// �J�E���^�[�����Z
	m_nCounterAnim++;

	if (m_nCounterAnim % ANIM_TIME == 0)
	{// ��莞�Ԃ��ƂɃA�j���[�V�������s��
		m_nCounterAnim = 0;
		m_nPatternAnim++;

		// �A�j���[�V�������𒴂�����0���ɖ߂�
		m_nPatternAnim = m_nPatternAnim % NUM_ANIM;
	}

	D3DXVECTOR2 leftUp = { 0.125f * m_nPatternAnim,0.0f };
	D3DXVECTOR2 rightDown = { 0.125f + 0.125f * m_nPatternAnim,1.0f };

	// �e�N�X�`�����W�ݒ�
	SetTex(leftUp, rightDown);

	// �p���N���X�̍X�V
	CBillboard::Update();

	if (m_nLife < 0)
	{// �����̍폜
		Uninit();
	}
}

//=====================================================
// �`�揈��
//=====================================================
void CExplosion::Draw(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//Z�e�X�g�̖�����
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	// �p���N���X�̕`��
	CBillboard::Draw();

	//Z�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

//=====================================================
// ��������
//=====================================================
CExplosion *CExplosion::Create(D3DXVECTOR3 pos)
{
	CExplosion *pExplosion = nullptr;

	if (pExplosion == nullptr)
	{// �C���X�^���X����
		pExplosion = new CExplosion;
	}
	
	pExplosion->SetPosition(pos);
	pExplosion->SetSize(20.0f, 20.0f);

	// ����������
	pExplosion->Init();

	// �e�N�X�`���̓Ǎ�
	int nIdx = CManager::GetTexture()->Regist("data\\TEXTURE\\EFFECT\\explosion000.png");
	pExplosion->SetIdxTexture(nIdx);

	pExplosion->m_nLife = 8 * 4;

	return pExplosion;
}

//=====================================================
// �Ǎ�����
//=====================================================
HRESULT CExplosion::Load(void)
{
	return S_OK;
}

//=====================================================
// �e�N�X�`���j��
//=====================================================
void CExplosion::Unload(void)
{

}