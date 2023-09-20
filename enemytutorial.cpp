//*****************************************************
//
// �`���[�g���A���G�̏���[enemytutorial.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "main.h"
#include "enemytutorial.h"
#include "renderer.h"
#include "manager.h"
#include "player.h"
#include "bullet.h"
#include "defend.h"
#include "universal.h"
#include "debugproc.h"
#include "game.h"
#include "meshfield.h"
#include "particle.h"
#include "smokespawner.h"
#include "missile.h"
#include "objectmanager.h"

//*****************************************************
// �}�N����`
//*****************************************************
#define SPEED_MOVE	(6.0f)	// �ړ����x
#define SPEED_FIGHTER	(30.0f)	// �퓬�@�̈ړ����x
#define ATTACK_FREQ	(180)	// �U������p�x
#define BULLET_SPEED	(30.0f)	// �e�̃X�s�[�h
#define INITIAL_HEIGHT	(300.0f)	// �����̍���
#define HEIGHT_FIGHTER	(900.0f)	// �퓬�@�̏����̍���
#define ATTACK_RANGE	(3500.0f)	// �U�����鋗��
#define DEFAULT_SPEED	(4.0f)	// ��̃X�s�[�h
#define DEFAULT_TILT	(-D3DX_PI * 0.1f)	// ��̌X��
#define ROT_DIFF	(D3DX_PI * 0.2f)	// ��̌���
#define TURN_ROT	(D3DX_PI * 0.01f)	// �^�[������܂ł̊p�x
#define LIMIT_TILT	(D3DX_PI * 0.5f)	// �p�x�̐���
#define LIFE_HELI	(5)	// �w���̏����̗�
#define LIFE_FIGHTER	(10)	// �퓬�@�̏����̗�
#define FALL_SPEED	(4.0f)	// �ė����x
#define ROLL_FACT	(0.05f)	// ���񑬓x
#define INITIAL_SCORE	(500)	// �����X�R�A
#define FALL_POSSIBILITY	(20)	// �ė�����p�[�Z���e�[�W

//=====================================================
// �R���X�g���N�^
//=====================================================
CEnemyTutorial::CEnemyTutorial()
{
	m_fAngleFall = 0.0f;
	m_fRotateFall = 0.0f;
	m_bFall = false;
}

//=====================================================
// �f�X�g���N�^
//=====================================================
CEnemyTutorial::~CEnemyTutorial()
{

}

//=====================================================
// ����������
//=====================================================
HRESULT CEnemyTutorial::Init(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �p���N���X�̏�����
	CEnemy::Init();

	// ������ɏオ��
	SetPosition(D3DXVECTOR3(GetPosition().x, GetPosition().y + INITIAL_HEIGHT, GetPosition().z));

	// �����̗̑͐ݒ�
	SetLife(LIFE_HELI);

	// �ė����̌����덷��ݒ�
	m_fAngleFall = (float)(rand() % (int)(3.14 * 100.0f) - 3.14 * 50.0f) / 100.0f;

	// �ė����̉�]�ʂ�ݒ�
	m_fRotateFall = (float)(rand() % (int)(0.15f * 50.0f) + 0.15f * 50.0f) / 100.0f;

	// �X�R�A�ݒ�
	SetScore(INITIAL_SCORE);

	// �ė����邩�ǂ����̐ݒ�
	int nRand = rand() % 100;

	if (nRand < FALL_POSSIBILITY)
	{
		m_bFall = true;
	}

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CEnemyTutorial::Uninit(void)
{
	// �p���N���X�̏I��
	CEnemy::Uninit();
}

//=====================================================
// �X�V����
//=====================================================
void CEnemyTutorial::Update(void)
{
	// �ʒu��ۑ�
	SetPositionOld(GetPosition());

	if (GetState() != STATE_DEATH)
	{

	}
	else
	{
		if (m_bFall)
		{
			// �ė�
			Fall();
		}
		else
		{
			Death();
		}
	}

	// �p���N���X�̍X�V
	CEnemy::Update();
}

//=====================================================
// �ė�����
//=====================================================
void CEnemyTutorial::Fall(void)
{
	// �ėp�����擾
	CUniversal *pUniversal = CManager::GetUniversal();
	D3DXVECTOR3 move, rot;

	// �ړ������擾
	m_fAngleFall = atan2f(GetMove().x, GetMove().z) + m_fAngleFall;

	move =
	{
		sinf(m_fAngleFall) * FALL_SPEED,
		-0.2f,
		cosf(m_fAngleFall) * FALL_SPEED
	};

	SetMove(GetMove() + move);

	// ��]������
	rot = GetRot();

	rot.y += m_fRotateFall;

	// �l�̕␳
	pUniversal->LimitRot(&rot.y);

	SetRot(rot);

	// ���b�V���t�B�[���h�Ƃ̓����蔻��
	CMeshField *pMesh = nullptr;
	CObjectManager *pObjManager = CManager::GetObjectManager();

	if (pObjManager != nullptr)
	{// ���b�V���t�B�[���h�̎擾
		pMesh = pObjManager->GetMeshField();
	}

	CParticle::Create(GetPosition(), CParticle::TYPE_SMOKE);

	if (pMesh != nullptr)
	{
		float  fHeight = pMesh->GetHeight(GetPosition(), &move);

		if (fHeight >= GetPosition().y)
		{// ���b�V���t�B�[���h�ɓ��������甚��
			CSmokeSpawner::Create(GetPosition(), 15.0f, 1, 3);

			CParticle::Create(GetPosition(), CParticle::TYPE_SAND);

			Death();
		}
	}
}

//=====================================================
// �`�揈��
//=====================================================
void CEnemyTutorial::Draw(void)
{
	// �p���N���X�̕`��
	CEnemy::Draw();
}