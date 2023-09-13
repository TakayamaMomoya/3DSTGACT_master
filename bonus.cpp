//*****************************************************
//
// �{�[�i�X�̕\��[bonus.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "bonus.h"
#include "object2D.h"
#include "manager.h"
#include "texture.h"
#include "score.h"
#include "game.h"
#include "player.h"
#include "assess.h"

//*****************************************************
// �}�N����`
//*****************************************************
#define MOVE_FACT	(0.05f)	// �ړ��W��
#define IN_SPEED	(0.03f)	// �t�F�[�h�C���̃X�s�[�h
#define OUT_SPEED	(0.07f)	// �t�F�[�h�A�E�g�̃X�s�[�h
#define TIME_FADE	(60)	// �t�F�[�h�A�E�g�܂ł̃t���[����
#define DOGDE_SCORE	(2000)	// ���ߒe����̃X�R�A
#define MISSILE_SCORE	(500)	// �~�T�C�����j�̃X�R�A
#define MANUALHIT_SCORE	(1000)	// �}�j���A���Ə��̃X�R�A
#define ASSESS_DODGE	(0.07f)	// ����̕]���P��
#define ASSESS_MISSILE	(0.07f)	// �~�T�C�����j�̕]���P��
#define ASSESS_MANUALHIT	(0.1f)	// �}�j���A���q�b�g�̕]���P��

//*****************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************
CBonus *CBonus::m_pHead = nullptr;	// �擪�̃A�h���X
CBonus *CBonus::m_pTail = nullptr;	// �Ō���̃A�h���X

//=====================================================
// �R���X�g���N�^
//=====================================================
CBonus::CBonus(int nPriority) : CObject(nPriority)
{
	// �l�̃N���A
	m_pPrev = nullptr;
	m_pNext = nullptr;
	m_pCaption = nullptr;
	m_state = STATE_NONE;
	m_nTimerOut = 0;

	if (m_pHead == nullptr)
	{// �擪�ƍŌ���A�h���X�̑��
		m_pHead = this;
		m_pTail = this;

		return;
	}

	// �O�̃A�h���X�ɍŌ���̃A�h���X��������
	m_pPrev = m_pTail;

	// �Ō���̃A�h���X�������ɂ���
	m_pTail = this;

	// �O�̃I�u�W�F�N�g�̎��̃A�h���X�������ɂ���
	m_pPrev->m_pNext = this;
}

//=====================================================
// �f�X�g���N�^
//=====================================================
CBonus::~CBonus()
{

}

//=====================================================
// ����������
//=====================================================
HRESULT CBonus::Init(void)
{
	m_state = STATE_IN;

	// ���̌��o���������グ��
	CBonus *pBonus = m_pHead;

	while (pBonus != nullptr)
	{
		CBonus *pBonusNext = pBonus->m_pNext;

		if (pBonus != this)
		{
			pBonus->m_posDest.y -= 70.0f;
		}

		pBonus = pBonusNext;
	}

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CBonus::Uninit(void)
{
	if (m_pCaption != nullptr)
	{
		m_pCaption->Uninit();

		m_pCaption = nullptr;
	}

	if (m_pHead == this)
	{// �擪�A�h���X�̔j��
	 // �擪�A�h���X�����̃A�h���X�Ɉ����p��
		m_pHead = m_pNext;

		if (m_pNext != nullptr)
		{
			m_pNext->m_pPrev = nullptr;
		}
	}
	else if (m_pTail == this)
	{// �Ō���A�h���X�̔j��
		if (m_pPrev != nullptr)
		{// �Ō���A�h���X��O�̃A�h���X�Ɉ����p��
			m_pTail = m_pPrev;

			m_pPrev->m_pNext = nullptr;
		}
	}
	else
	{// �^�񒆂̃A�h���X�̔j��
	 // �O�̃A�h���X���玟�̃A�h���X���Ȃ�
		m_pPrev->m_pNext = m_pNext;

		// ���̃A�h���X����O�̃A�h���X���Ȃ�
		m_pNext->m_pPrev = m_pPrev;
	}

	Release();
}

//=====================================================
// �X�V����
//=====================================================
void CBonus::Update(void)
{
	// ��ԊǗ�
	ManageState();

	// �ڕW�ʒu�Ɍ���������
	MoveToDest();
}

//=====================================================
// ��ԊǗ�
//=====================================================
void CBonus::ManageState(void)
{
	if (m_pCaption == nullptr)
	{
		return;
	}

	if (m_pCaption->GetPosition().y <= SCREEN_WIDTH * 0.4f)
	{
		m_state = STATE_OUT;
	}

	// �F�̎擾
	D3DXCOLOR col = m_pCaption->GetCol();

	switch (m_state)
	{
	case CBonus::STATE_IN:
		col.a += IN_SPEED;

		if (col.a > 1.0f)
		{
			m_state = STATE_WAIT;
		}

		break;
	case CBonus::STATE_WAIT:

		m_nTimerOut++;

		if (m_nTimerOut >= TIME_FADE)
		{
			m_state = STATE_OUT;
		}

		break;
	case CBonus::STATE_OUT:
		col.a -= OUT_SPEED;

		if (col.a <= 0.0f)
		{
			Uninit();
		}

		break;
	default:
		break;
	}

	if (m_pCaption != nullptr)
	{
		m_pCaption->SetCol(col);
	}
}

//=====================================================
// �ڕW�Ɍ���������
//=====================================================
void CBonus::MoveToDest(void)
{
	// �ʒu�̎擾
	D3DXVECTOR3 pos;
	D3DXVECTOR3 vecDiff;

	if (m_pCaption != nullptr)
	{
		pos = m_pCaption->GetPosition();
	}
	else
	{
		return;
	}

	// �����x�N�g���擾
	vecDiff = m_posDest - pos;

	pos += vecDiff * MOVE_FACT;

	// �ʒu�̐ݒ�
	m_pCaption->SetPosition(pos);

	m_pCaption->SetVtx();
}

//=====================================================
// �`�揈��
//=====================================================
void CBonus::Draw(void)
{

}

//=====================================================
// ��������
//=====================================================
CBonus *CBonus::Create(TYPE type)
{
	char *aPath[TYPE_MAX] =
	{
		"data\\TEXTURE\\UI\\message00.png",
		"data\\TEXTURE\\UI\\message01.png",
		"data\\TEXTURE\\UI\\message02.png"
	};

	CBonus *pBonus = nullptr;

	pBonus = new CBonus;

	if (pBonus != nullptr)
	{
		pBonus->m_pCaption = CObject2D::Create(6);

		if (pBonus->m_pCaption != nullptr)
		{
			// �{�[�i�X�t�^
			pBonus->BonusScore(type);

			pBonus->m_pCaption->SetPosition(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 600.0f, 0.0f));
			pBonus->m_pCaption->SetSize(170.0f, 30.0f);

			// �����̖ڕW�ʒu�Ɠ�������
			pBonus->m_posDest = pBonus->m_pCaption->GetPosition();

			pBonus->m_pCaption->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));

			// �e�N�X�`���ԍ��擾
			int nIdx = CManager::GetTexture()->Regist(aPath[type]);

			pBonus->m_pCaption->SetIdxTexture(nIdx);
			pBonus->m_pCaption->SetVtx();
		}

		// ����������
		pBonus->Init();
	}

	return pBonus;
}

//=====================================================
// �X�R�A�{�[�i�X�t�^
//=====================================================
void CBonus::BonusScore(TYPE type)
{
	// �X�R�A�擾
	CScore *pScore = CGame::GetScore();

	// �v���C���[�̎擾
	CPlayer *pPlayer = CGame::GetPlayer();

	CAssess *pAssess = nullptr;

	if (pPlayer != nullptr)
	{// �]���V�X�e���̎擾
		pAssess = pPlayer->GetAssess();
	}

	int aScore[TYPE_MAX] = 
	{
		DOGDE_SCORE,
		MISSILE_SCORE,
		MANUALHIT_SCORE
	};

	if (pScore != nullptr)
	{
		pScore->AddScore(aScore[type]);
	}

	if (pAssess == nullptr)
	{
		return;
	}

	switch (type)
	{
	case CBonus::TYPE_DOGDE:

		pAssess->AddParam(ASSESS_DODGE,CAssess::PARAM_DODGE);

		break;
	case CBonus::TYPE_MISSILE:

		pAssess->AddParam(ASSESS_MISSILE, CAssess::PARAM_HIT);

		break;
	case CBonus::TYPE_MANUALHIT:

		pAssess->AddParam(ASSESS_MANUALHIT, CAssess::PARAM_HIT);

		break;
	default:
		break;
	}
}