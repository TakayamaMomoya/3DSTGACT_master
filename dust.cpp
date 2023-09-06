//*****************************************************
//
// �o�̏���[dust.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "dust.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"

//*****************************************************
// �}�N����`
//*****************************************************
#define SPEED_MOVE	(7.0f)	// �ړ����x
#define ALPHA_SPEED	(0.01f)	// ������X�s�[�h
#define WIND_ANGLE	(D3DX_PI * 0.3f)	// ������
#define WIND_RANGE	(D3DX_PI * 0.1f)	// �������̃����_����

//=====================================================
// �R���X�g���N�^
//=====================================================
CDust::CDust(int nPriority) : CBillboard(nPriority)
{
	m_fAlpha = 0.0f;
	m_state = STATE_NONE;
}

//=====================================================
// �f�X�g���N�^
//=====================================================
CDust::~CDust()
{

}

//=====================================================
// ����������
//=====================================================
HRESULT CDust::Init(void)
{
	// �p���N���X�̏�����
	CBillboard::Init();

	D3DXCOLOR col = { 0.97f, 0.90f, 0.71f, 0.00f };

	SetColor(col);

	// �l�̏�����
	m_state = STATE_IN;
	m_move = { 0.0f,0.0f,0.0f };

	// �ړ��ʂ̐ݒ�
	SetMove();

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CDust::Uninit(void)
{
	// �p���N���X�̏I��
	CBillboard::Uninit();
}

//=====================================================
// �X�V����
//=====================================================
void CDust::Update(void)
{
	// �p���N���X�̍X�V
	CBillboard::Update();

	// �ړ��ʂ��ʒu�ɔ��f
	D3DXVECTOR3 pos = GetPosition();

	pos += m_move;

	SetPosition(pos);

	// �F�̎擾
	D3DXCOLOR col = GetColor();

	switch (m_state)
	{
	case CDust::STATE_IN:

		col.a += ALPHA_SPEED;

		if (col.a >= 1.0f)
		{
			col.a = 1.0f;
			
			m_state = STATE_OUT;
		}

		break;
	case CDust::STATE_OUT:

		col.a -= ALPHA_SPEED;

		if (col.a <= 0)
		{
			Uninit();
		}

		break;
	default:
		break;
	}

	SetColor(col);
}

//=====================================================
// �ړ��ʂ̐ݒ�
//=====================================================
void CDust::SetMove(void)
{
	float fSpeed;
	float fMoveY;
	float fAngle;

	// �ړ��X�s�[�h�̌���
	fSpeed = (float)(rand() % 5 + 1);
	fMoveY = (float)(rand() % 5 + 1) * 0.1f;

	// �ړ������̌���
	fAngle = WIND_ANGLE + (float)(rand() % (int)(WIND_RANGE * 100.0f) - WIND_RANGE * 50.0f) / 100.0f;

	m_move = 
	{
		sinf(fAngle) * fSpeed,
		fMoveY,
		cosf(fAngle) * fSpeed
	};
}

//=====================================================
// �`�揈��
//=====================================================
void CDust::Draw(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �A���t�@�e�X�g�̗L����
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 20);

	// �p���N���X�̕`��
	CBillboard::Draw();

	// �A���t�@�e�X�g�̖�����
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
}

//=====================================================
// ��������
//=====================================================
CDust *CDust::Create(D3DXVECTOR3 pos)
{
	CDust *pDust = nullptr;

	if (pDust == nullptr)
	{// �C���X�^���X����
		pDust = new CDust;

		if (pDust != nullptr)
		{
			pDust->SetPosition(pos);

			pDust->SetSize(500,500);

			// ����������
			pDust->Init();

			// �e�N�X�`���̓Ǎ�
			int nIdx = CManager::GetTexture()->Regist("data\\TEXTURE\\EFFECT\\smoke.png");
			pDust->SetIdxTexture(nIdx);
		}
	}
	
	return pDust;
}