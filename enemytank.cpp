//*****************************************************
//
// ��ԓG�̏���[enemytank.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "main.h"
#include "enemytank.h"
#include "renderer.h"
#include "manager.h"
#include "player.h"
#include "bullet.h"
#include "defend.h"
#include "universal.h"
#include "debugproc.h"
#include "game.h"
#include "objectmanager.h"
#include "sound.h"

//*****************************************************
// �}�N����`
//*****************************************************
#define SPEED_MOVE	(5.5f)	// �ړ����x
#define ATTACK_FREQ	(180)	// �U������p�x
#define BULLET_SPEED	(90.0f)	// �e�̃X�s�[�h
#define INITIAL_HEIGHT	(300.0f)	// �����̍���
#define ATTACK_RANGE	(4000.0f)	// �U�����鋗��
#define DEFAULT_SPEED	(4.0f)	// ��̃X�s�[�h
#define DEFAULT_TILT	(-D3DX_PI * 0.1f)	// ��̌X��
#define TANK_HEIGHT	(150.0f)	// ����
#define LIFE_TANK	(25)	// �����̗�
#define INITIAL_SCORE	(4000)	// �����X�R�A

//=====================================================
// �R���X�g���N�^
//=====================================================
CEnemyTank::CEnemyTank()
{
	
}

//=====================================================
// �f�X�g���N�^
//=====================================================
CEnemyTank::~CEnemyTank()
{

}

//=====================================================
// ����������
//=====================================================
HRESULT CEnemyTank::Init(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �p���N���X�̏�����
	CEnemy::Init();

	// ������ɏオ��
	SetPosition(D3DXVECTOR3(GetPosition().x, GetPosition().y + INITIAL_HEIGHT, GetPosition().z));

	// �̗͐ݒ�
	SetLife(LIFE_TANK);

	// �X�R�A�ݒ�
	SetScore(INITIAL_SCORE);

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CEnemyTank::Uninit(void)
{
	// �p���N���X�̏I��
	CEnemy::Uninit();
}

//=====================================================
// �X�V����
//=====================================================
void CEnemyTank::Update(void)
{
	if (GetState() == STATE_DEATH)
	{
		if (GetMotion() != 2)
		{
			SetMotion(2);
		}
	}

	// �ʒu��ۑ�
	SetPositionOld(GetPosition());

	// �ǐՏ���
	CEnemyTank::ChaseDefend();

	// �ړ��ʌ���
	SetMove(D3DXVECTOR3(GetMove().x * 0.98f, GetMove().y - 0.98f, GetMove().z * 0.98f));

	// �p���N���X�̍X�V
	CEnemy::Update();
}

//=====================================================
// ��q�ڕW�ǐ�
//=====================================================
void CEnemyTank::ChaseDefend(void)
{
	if (GetState() == STATE_DEATH)
	{
		return;
	}

	// �v���C���[���擾
	CPlayer *pPlayer = nullptr;
	CObjectManager *pObjManager = CManager::GetObjectManager();

	if (pObjManager != nullptr)
	{// �v���C���[�̓K�p
		pPlayer = pObjManager->GetPlayer();
	}

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

	// �p�x�␳
	SetRot(D3DXVECTOR3(GetRot().x, GetRot().y + fRotDiff * 0.1f, GetRot().z));

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
			-sinf(GetRot().y) * SPEED_MOVE,
			GetMove().y,
			-cosf(GetRot().y) * SPEED_MOVE
		));
	}
	else
	{// �ڕW�ɋ߂Â��Ă�����

		if (GetMoveState() == MOVESTATE_CHASE)
		{
			if (GetCntAttack() >= ATTACK_FREQ)
			{// ��莞�Ԃ��ƂɍU��
				D3DXVec3Normalize(&vecDest, &vecDest);

				CBullet::Create
				(// �e�̔���
					D3DXVECTOR3(GetPosition().x, GetPosition().y + TANK_HEIGHT, GetPosition().z),
					vecDest * BULLET_SPEED,
					300,
					CBullet::TYPE_ENEMY,
					false,
					20.0f
				);

				SetCntAttack(0);

				// SE�Đ�
				CManager::GetSound()->Play(CSound::LABEL_BIGSHOT);
			}
		}
		else
		{
			SetMove(D3DXVECTOR3
			(
				-sinf(GetRot().y) * SPEED_MOVE,
				GetMove().y,
				-cosf(GetRot().y) * SPEED_MOVE
			));
		}
	}
}

//=====================================================
// �`�揈��
//=====================================================
void CEnemyTank::Draw(void)
{
	// �p���N���X�̕`��
	CEnemy::Draw();
}