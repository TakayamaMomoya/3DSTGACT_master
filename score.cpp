//*****************************************************
//
// �X�R�A�̏���[score.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "score.h"
#include "manager.h"
#include "renderer.h"
#include "game.h"
#include "texture.h"

//*****************************************************
// �}�N����`
//*****************************************************
#define NUM_PLACE	(6)	// ����
#define SCORE_SPEED	(13)	// �X�R�A�̕ς��X�s�[�h	

//*****************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************
LPDIRECT3DTEXTURE9 CScore::m_pTexture = nullptr;	// �e�N�X�`���̃|�C���^

//=====================================================
// �R���X�g���N�^
//=====================================================
CScore::CScore()
{
	m_pObjNumber = nullptr;
	m_nScore = 0;
	m_nSocreDest = 0;
	m_nIdxTexture = -1;
}

//=====================================================
// �f�X�g���N�^
//=====================================================
CScore::~CScore()
{

}

//=====================================================
// ����������
//=====================================================
HRESULT CScore::Init(void)
{
	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CScore::Uninit(void)
{
	if (m_pObjNumber != nullptr)
	{
		m_pObjNumber = nullptr;
	}

	CGame::ReleaseScore();
}

//=====================================================
// �X�V����
//=====================================================
void CScore::Update(void)
{
	//�X�R�A�l�㏸���o==============================
	if (m_nScore < m_nSocreDest)
	{//�㏸
		m_nScore += SCORE_SPEED;
		if (m_nScore > m_nSocreDest)
		{//��~
			m_nScore = m_nSocreDest;
		}
	}
	else if (m_nScore > m_nSocreDest)
	{//����
		m_nScore -= SCORE_SPEED;
		if (m_nScore < m_nSocreDest)
		{//��~
			m_nScore = m_nSocreDest;
		}
	}

	if (m_pObjNumber != nullptr)
	{
		m_pObjNumber->SetValue(m_nScore, NUM_PLACE);
	}
}

//=====================================================
// �X�R�A���Z
//=====================================================
void CScore::AddScore(int nValue)
{
	m_nSocreDest += nValue;
}

//=====================================================
// ��������
//=====================================================
CScore *CScore::Create(void)
{
	CScore *pScore = nullptr;
	
	CObject2D *pObject2D = CObject2D::Create(7);

	if (pObject2D != nullptr)
	{
		pObject2D->SetPosition(D3DXVECTOR3(940.0f, 70.0f, 0.0f));
		pObject2D->SetSize(120.0f, 60.0f);

		// �e�N�X�`���ԍ��擾
		int nIdx = CManager::GetTexture()->Regist("data\\TEXTURE\\UI\\scoreBack.png");

		pObject2D->SetIdxTexture(nIdx);
		pObject2D->SetVtx();
	}

	if (pScore == nullptr)
	{
		pScore = new CScore;

		if (pScore->m_pObjNumber == nullptr)
		{
			pScore->m_pObjNumber = CNumber::Create(NUM_PLACE, pScore->m_nScore);
			pScore->m_pObjNumber->SetPosition(D3DXVECTOR3(1060.0f, 80.0f, 0.0f));
			pScore->m_pObjNumber->SetSizeAll(20.0f,50.0f);
		}
	}

	return pScore;
}

//=====================================================
// �Ǎ�����
//=====================================================
HRESULT CScore::Load(void)
{
	if (m_pTexture == nullptr)
	{
		// �f�o�C�X�̎擾
		LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

		D3DXCreateTextureFromFile
		(
			pDevice,
			"data\\TEXTURE\\UI\\scoreBack.png",
			&m_pTexture
		);
	}

	return S_OK;
}

//=====================================================
// �e�N�X�`���j��
//=====================================================
void CScore::Unload(void)
{
	if (m_pTexture != nullptr)
	{
		m_pTexture->Release();
		m_pTexture = nullptr;
	}
}