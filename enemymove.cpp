//*****************************************************
//
// �����G�̏���[enemymove.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "main.h"
#include "enemymove.h"
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
#include "debrisspawner.h"
#include "missile.h"

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
CEnemyMove::CEnemyMove()
{
	m_fAngleFall = 0.0f;
	m_fRotateFall = 0.0f;
	m_bFall = false;
}

//=====================================================
// �f�X�g���N�^
//=====================================================
CEnemyMove::~CEnemyMove()
{

}

//=====================================================
// ����������
//=====================================================
HRESULT CEnemyMove::Init(void)
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
void CEnemyMove::Uninit(void)
{
	// �p���N���X�̏I��
	CEnemy::Uninit();
}

//=====================================================
// �X�V����
//=====================================================
void CEnemyMove::Update(void)
{
	// �ʒu��ۑ�
	SetPositionOld(GetPosition());

	if (GetState() != STATE_DEATH)
	{
		// �ǐՏ���
		CEnemyMove::ChaseTarget();

		// �X���̐���
		Tilt();
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

	if (GetState() == STATE_DEATH)
	{
		// �ړ��ʌ���
		SetMove(GetMove() *= 0.98f);
	}
	else
	{
		// �ړ��ʌ���
		SetMove(D3DXVECTOR3(GetMove().x * 0.98f,0.0f, GetMove().z * 0.98f));
	}

	// �p���N���X�̍X�V
	CEnemy::Update();
}

//=====================================================
// �X���̐��䏈��
//=====================================================
void CEnemyMove::Tilt(void)
{
	// �ėp�����擾
	CUniversal *pUniversal = CManager::GetUniversal();

	// �ϐ��錾
	D3DXVECTOR3 move, rot;
	float fSpeed,fTilt;

	// ���擾
	move = GetMove();
	rot = GetRot();

	// ���x���擾
	fSpeed = D3DXVec3Length(&move);

	// �X������Z�o
	fTilt = -D3DX_PI - fSpeed / DEFAULT_SPEED * DEFAULT_TILT;

	if (pUniversal != nullptr)
	{// �X������
		pUniversal->FactingRot(&rot.x, fTilt, 0.98f);
	}
	
	// �����̔��f
	SetRot(rot);
}

//=====================================================
// �ڕW�ǐ�
//=====================================================
void CEnemyMove::ChaseTarget(void)
{
	// �v���C���[���擾
	CPlayer *pPlayer = CGame::GetPlayer();

	if (pPlayer != nullptr)
	{
		if (GetMoveState() == MOVESTATE_CHASE)
		{
			SetPosDest(pPlayer->GetPosition());
		}
	}

	// �ϐ��錾
	D3DXVECTOR3 vecDest;

	// �������擾
	vecDest = GetPosDest() - GetPosition();

	float length = D3DXVec3Length(&vecDest);

	// �ڕW�p�x���擾
	float fRotDest = atan2f(vecDest.x, vecDest.z);

	float fRotDiff = fRotDest - GetRot().y;

	// �p�x�̏C��
	if (fRotDiff > D3DX_PI)
	{
		fRotDiff -= 6.28f;
	}
	if (fRotDiff < -D3DX_PI)
	{
		fRotDiff += 6.28f;
	}

	// �p�x�␳
	SetRot(D3DXVECTOR3(GetRot().x, GetRot().y + fRotDiff * ROLL_FACT, GetRot().z));

	// �p�x�̏C��
	if (GetRot().y > D3DX_PI)
	{
		// �p�x�␳
		SetRot(D3DXVECTOR3(GetRot().x, GetRot().y - 6.28f, GetRot().z));
	}
	if (GetRot().y < -D3DX_PI)
	{
		// �p�x�␳
		SetRot(D3DXVECTOR3(GetRot().x, GetRot().y + 6.28f, GetRot().z));
	}

	if (length > ATTACK_RANGE)
	{// �ڕW���痣��Ă�����
		// �ڕW�����֐i��
		SetMove(D3DXVECTOR3
		(
			sinf(GetRot().y) * SPEED_MOVE,
			GetMove().y,
			cosf(GetRot().y) * SPEED_MOVE
		));
	}
	else
	{// �ڕW�ɋ߂Â��Ă�����
		if (GetMoveState() == MOVESTATE_CHASE)
		{
			if (GetCntAttack() >= ATTACK_FREQ)
			{// ��莞�Ԃ��ƂɍU��
				CMissile::Create(GetPosition());

				SetCntAttack(0);
			}
		}
		else
		{
			SetMove(D3DXVECTOR3
			(
				sinf(GetRot().y) * SPEED_MOVE,
				GetMove().y,
				cosf(GetRot().y) * SPEED_MOVE
			));
		}

	}

}

//=====================================================
// �ė�����
//=====================================================
void CEnemyMove::Fall(void)
{
	// �ėp�����擾
	CUniversal *pUniversal = CManager::GetUniversal();
	D3DXVECTOR3 move,rot;

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
	CMeshField *pMesh = CGame::GetMeshField();

	CParticle::Create(GetPosition(), CParticle::TYPE_SMOKE);

	if (pMesh != nullptr)
	{
		float  fHeight = CGame::GetMeshField()->GetHeight(GetPosition(), &move);

		if (fHeight >= GetPosition().y)
		{// ���b�V���t�B�[���h�ɓ��������甚��
			CDebrisSpawner::Create(GetPosition(), 15.0f, 1, 3);

			CParticle::Create(GetPosition(), CParticle::TYPE_SAND);

			Death();
		}
	}
}

//=====================================================
// �`�揈��
//=====================================================
void CEnemyMove::Draw(void)
{
	// �p���N���X�̕`��
	CEnemy::Draw();
}

//=====================================================
// �R���X�g���N�^
//=====================================================
CEnemyAttack::CEnemyAttack()
{
	m_state = STATE_NONE;
	m_nTimerLeave = 0;
}

//=====================================================
// �f�X�g���N�^
//=====================================================
CEnemyAttack::~CEnemyAttack()
{

}

//=====================================================
// ����������
//=====================================================
HRESULT CEnemyAttack::Init(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �p���N���X�̏�����
	CEnemy::Init();

	// ������ɏオ��
	SetPosition(D3DXVECTOR3(GetPosition().x, GetPosition().y + HEIGHT_FIGHTER, GetPosition().z));

	// �ǐՏ�Ԃɐݒ�
	m_state = STATE_CHASE;

	// �����̗̑͐ݒ�
	SetLife(LIFE_FIGHTER);

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CEnemyAttack::Uninit(void)
{
	// �p���N���X�̏I��
	CEnemy::Uninit();
}

//=====================================================
// �X�V����
//=====================================================
void CEnemyAttack::Update(void)
{
	// �ʒu��ۑ�
	SetPositionOld(GetPosition());

	// �ǐՏ���
	CEnemyAttack::ChasePlayer();

	// �ړ��ʌ���
	SetMove(GetMove() *= 0.99f);

	// �p���N���X�̍X�V
	CEnemy::Update();
}

//=====================================================
// �X���̐��䏈��
//=====================================================
void CEnemyAttack::Tilt(float fRotDiff)
{
	// �ėp�����擾
	CUniversal *pUniversal = CManager::GetUniversal();

	// �ϐ��錾
	float fTilt;
	float fRot = GetRot().y;

	// �������Z�o
	fTilt = fRotDiff / ROT_DIFF;

	// �����̐ݒ�
	fRot = fTilt * ROT_DIFF;

	// �l�̕␳
	pUniversal->LimitRot(&fRot);

	if (LIMIT_TILT < fRot)
	{
		fRot = LIMIT_TILT;
	}
	else if (-LIMIT_TILT > fRot)
	{
		fRot = -LIMIT_TILT;
	}

	if (m_state == STATE_LEAVE)
	{
		// �{�̂���
		pUniversal->FactingRot(&fRot, 0.0f, 0.10f);
	}
	else
	{
		// �{�̂���
		pUniversal->FactingRot(&fRot, fTilt, 0.10f);
	}

	SetRot(D3DXVECTOR3(GetRot().x, GetRot().y, fRot));
}

//=====================================================
// ��ԊǗ�
//=====================================================
void CEnemyAttack::ManageState(float fLength,float fRotDiff)
{
	switch (m_state)
	{
	case STATE_CHASE:
		// �ǐՏ��
		if (fLength > 7000.0f)
		{// �^�[������
			m_state = STATE_TURN;
		}

		if (fLength < 2000.0f)
		{
			m_state = STATE_LEAVE;
		}

		break;
	case STATE_TURN:
		// �^�[�����
		if (fRotDiff * fRotDiff < TURN_ROT * TURN_ROT)
		{// �ǐՂɂ���
			m_state = STATE_CHASE;
		}
		else
		{
			if (fLength < 2000.0f)
			{
				m_state = STATE_LEAVE;
			}
		}

		break;
	case STATE_LEAVE:
		// ���E���
		m_nTimerLeave++;

		if (m_nTimerLeave > 300)
		{
			m_state = STATE_TURN;
			m_nTimerLeave = 0;
		}

		break;
	default:
		break;
	}
}

//=====================================================
// �ڕW�ǐ�
//=====================================================
void CEnemyAttack::ChasePlayer(void)
{
	// �v���C���[���擾
	CPlayer *pPlayer = CGame::GetPlayer();

	if (pPlayer != nullptr)
	{
		// �ϐ��錾
		D3DXVECTOR3 vecDest;

		// �������擾
		vecDest = pPlayer->GetPosition() - GetPosition();

		float length = D3DXVec3Length(&vecDest);

		// �ڕW�p�x���擾
		float fRotDest = atan2f(vecDest.x, vecDest.z) + D3DX_PI;

		float fRotDiff = fRotDest - GetRot().y;

		// �p�x�̏C��
		if (fRotDiff > D3DX_PI)
		{
			fRotDiff -= 6.28f;
		}
		if (fRotDiff < -D3DX_PI)
		{
			fRotDiff += 6.28f;
		}

		// �X������
		Tilt(fRotDiff);

		// ��ԊǗ�
		ManageState(length, fRotDiff);

		if (m_state != STATE_LEAVE)
		{
			if (m_state == STATE_TURN)
			{
				// �p�x�␳
				SetRot(D3DXVECTOR3(GetRot().x, GetRot().y + fRotDiff * 0.02f, GetRot().z));
			}
			else
			{
				// �p�x�␳
				SetRot(D3DXVECTOR3(GetRot().x, GetRot().y + fRotDiff * 0.001f, GetRot().z));
			}
		}

		// �p�x�̏C��
		if (GetRot().y > D3DX_PI)
		{
			// �p�x�␳
			SetRot(D3DXVECTOR3(GetRot().x, GetRot().y - 6.28f, GetRot().z));
		}
		if (GetRot().y < -D3DX_PI)
		{
			// �p�x�␳
			SetRot(D3DXVECTOR3(GetRot().x, GetRot().y + 6.28f, GetRot().z));
		}

		SetMove(D3DXVECTOR3
		(
			-sinf(GetRot().y) * SPEED_FIGHTER,
			GetMove().y,
			-cosf(GetRot().y) * SPEED_FIGHTER
		));

		if (length > ATTACK_RANGE)
		{// �ڕW���痣��Ă�����
		 // �ڕW�����֐i��
			
		}
		else
		{// �ڕW�ɋ߂Â��Ă�����
			if (GetCntAttack() >= ATTACK_FREQ)
			{// ��莞�Ԃ��ƂɍU��
				D3DXVec3Normalize(&vecDest, &vecDest);

				CBullet::Create
				(// �e�̔���
					D3DXVECTOR3(GetPosition().x, GetPosition().y, GetPosition().z),
					vecDest * BULLET_SPEED,
					300,
					CBullet::TYPE_ENEMY,
					false,
					20.0f
				);

				SetCntAttack(0);
			}
		}
	}
}

//=====================================================
// �`�揈��
//=====================================================
void CEnemyAttack::Draw(void)
{
	// �p���N���X�̕`��
	CEnemy::Draw();
}