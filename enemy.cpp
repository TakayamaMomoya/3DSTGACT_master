//*****************************************************
//
// �G�̏���[enemy.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "main.h"
#include "enemy.h"
#include "renderer.h"
#include "manager.h"
#include "score.h"
#include "collision.h"
#include "debugproc.h"
#include "item.h"
#include "explosion.h"
#include "particle.h"
#include "meshfield.h"
#include "billboard.h"
#include "texture.h"
#include "camera.h"
#include "shadow.h"
#include <stdio.h>
#include "universal.h"
#include "defend.h"
#include "player.h"
#include "game.h"
#include "explosionspawner.h"
#include "smokespawner.h"
#include "debrisspawner.h"
#include "player.h"
#include "enemymove.h"
#include "enemytank.h"
#include "enemytutorial.h"
#include "arrow.h"
#include "objectmanager.h"

//*****************************************************
// �}�N����`
//*****************************************************
#define SPEED_MOVE	(1.0f)	// �ړ����x
#define DEFAULT_SIZE	(130.0f)		// ��̑傫��
#define DEFAULT_DIST	(2000.0f)		// ��̋���
#define PATH_LOCKON	("data\\TEXTURE\\UI\\lockon002.png")	// ���b�N�I���}�[�J�[�̃p�X
#define ENEMY_DISTANCE	(6000)	// �G���o�����鋗��
#define RATE_RADIUS	(1.5f)	// �����蔻��̑傫���̔{��
#define INITIAL_LIFE	(1.0f)	// �����̗�
#define DAMAGE_FRAME	(10)	// �_���[�W��Ԃ̌p���t���[����
#define TIME_DEATH	(100)	// ���S�܂ł̃t���[����
#define AVOID_LENGTH	(3.0f)	// �ǐՂɂ���܂ł̋���
#define NUM_AIR_BONUS	(3)	// �󒆂Ń{�[�i�X������܂Ō��Đ�
#define AIR_SCORE	(300)	// �󒆌��Ẵ{�[�i�X
#define INITIAL_SCORE	(1000)	// �����X�R�A
#define CLOSE_DISTANCE	(1000.0f)	// �߂��Ɣ��f���鋗��
#define BONUS_TIME	(420)	// �{�[�i�X���Ⴆ���̃t���[����

//*****************************************************
// �ÓI�����o�ϐ��錾
//*****************************************************
int CEnemy::m_nNumAll = 0;	// ����
int CEnemy::m_nNumWave = 0;	// �E�F�[�u��
CEnemy::WAVEINFO *CEnemy::m_pWaveInfo = nullptr;	// �G���ւ̃|�C���^
CEnemy *CEnemy::m_pHead = nullptr;	// �擪�̃A�h���X
CEnemy *CEnemy::m_pTail = nullptr;	// �Ō���̃A�h���X

//=====================================================
// �R���X�g���N�^
//=====================================================
CEnemy::CEnemy()
{
	// �����J�E���g�A�b�v
	m_nNumAll++;

	m_nCntAttack = 0;
	m_fLife = 0;
	m_nTimerState = 0;
	m_nScore = 0;
	m_nCntClose = 0;
	m_bLockon = false;
	m_bFind = true;
	m_pCollisionSphere = nullptr;
	m_state = STATE_NORMAL;
	m_moveState = MOVESTATE_NONE;
	m_posDest = { 0.0f,0.0f,0.0f };

	// �l�̃N���A
	m_pPrev = nullptr;
	m_pNext = nullptr;

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
CEnemy::~CEnemy()
{
	if (m_state != STATE_DEATH)
	{
		m_nNumAll--;
	}

	if (m_pHead == this)
	{// �擪�A�h���X�̔j��
	 //if (m_pNext != nullptr)
		{// �擪�A�h���X�����̃A�h���X�Ɉ����p��
			m_pHead = m_pNext;

			if (m_pNext != nullptr)
			{
				m_pNext->m_pPrev = nullptr;
			}
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
}

//=====================================================
// ��������
//=====================================================
CEnemy *CEnemy::Create(D3DXVECTOR3 pos, TYPE type)
{
	CEnemy *pEnemy = nullptr;

	if (pEnemy == nullptr)
	{// �C���X�^���X����
		switch (type)
		{
		case TYPE_MOVE:
			pEnemy = new CEnemyMove;

			pEnemy->CMotion::Load("data\\MOTION\\motionHeli.txt");

			pEnemy->SetMotion(0);

			break;
		case TYPE_TANK:
			pEnemy = new CEnemyTank;

			pEnemy->CMotion::Load("data\\MOTION\\legtank.txt");

			pEnemy->SetMotion(1);

			break;
		case TYPE_ATTACK:
			pEnemy = new CEnemyAttack;

			pEnemy->CMotion::Load("data\\MOTION\\motionFighter.txt");

			break;
		case TYPE_TUTORIAL:
			pEnemy = new CEnemyTutorial;

			pEnemy->CMotion::Load("data\\MOTION\\motionHeli.txt");

			break;
		default:
			break;
		}

		// �e�̐���
		if (pEnemy->m_pShadow == nullptr)
		{
			pEnemy->m_pShadow = CShadow::Create(D3DXVECTOR3(pEnemy->GetPosition().x, 1.0f, pEnemy->GetPosition().z), 150.0f, 150.0f);

			if (pEnemy->m_pShadow != nullptr)
			{
				pEnemy->m_pShadow->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
			}
		}

		pEnemy->SetPosition(D3DXVECTOR3(pos));

		// ����������
		pEnemy->Init();
	}

	return pEnemy;
}

//=====================================================
// ����������
//=====================================================
HRESULT CEnemy::Init(void)
{
	// �ϐ��錾
	float fRadius = GetRadiusMax();

	// �p���N���X�̏�����
	CMotion::Init();

	// �^�C�v�̐ݒ�
	SetType(TYPE_ENEMY);

	if (m_pCollisionSphere == nullptr)
	{// ���̓����蔻�萶��
		m_pCollisionSphere = CCollisionSphere::Create(CCollision::TAG_ENEMY, CCollision::TYPE_SPHERE,this);

		if (m_pCollisionSphere != nullptr)
		{// ���̐ݒ�
			m_pCollisionSphere->SetPosition(GetPosition());
			m_pCollisionSphere->SetRadius(fRadius * RATE_RADIUS);
		}
	}

	if (m_pCollisionCube == nullptr)
	{// �����̂̓����蔻��
		m_pCollisionCube = CCollisionCube::Create(CCollision::TAG_ENEMY, this);

		D3DXVECTOR3 vtxMax = { fRadius,fRadius,fRadius };
		D3DXVECTOR3 vtxMin = { -fRadius,-fRadius,-fRadius };

		if (m_pCollisionCube != nullptr)
		{
			m_pCollisionCube->SetVtx(vtxMax, vtxMin);
		}
	}

	// �̗͂̏����ݒ�
	m_fLife = INITIAL_LIFE;

	// �ʏ��Ԃɂ���
	m_moveState = MOVESTATE_CHASE;
	m_state = STATE_NORMAL;

	m_nScore = INITIAL_SCORE;

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CEnemy::Uninit(void)
{
	if (m_pCollisionSphere != nullptr)
	{// �����蔻��̏���
		m_pCollisionSphere->Uninit();

		m_pCollisionSphere = nullptr;
	}

	if (m_pShadow != nullptr)
	{// �e�̍폜
		m_pShadow->Uninit();

		m_pShadow = nullptr;
	}

	if (m_pExplSpawner != nullptr)
	{
		m_pExplSpawner->Uninit();
		m_pExplSpawner = nullptr;
	}

	// �p���N���X�̏I��
	CMotion::Uninit();
}

//=====================================================
// �X�V����
//=====================================================
void CEnemy::Update(void)
{
	if (CGame::GetState() != CGame::STATE_NORMAL)
	{
		return;
	}

	// �p���N���X�̍X�V
	CMotion::Update();

	// ��ԊǗ�����
	ManageState();

	// �U���J�E���^�[���Z
	m_nCntAttack++;

	if (m_nCntAttack == INT_MAX)
	{// �l�̕␳
		m_nCntAttack = 0;
	}

	// ���b�V���t�B�[���h�Ƃ̓����蔻��
	D3DXVECTOR3 move = GetMove();
	float fHeight = 0.0f;
	CMeshField *pMesh = nullptr;
	CObjectManager *pObjManager = CManager::GetObjectManager();

	if (pObjManager != nullptr)
	{// ���b�V���t�B�[���h�̎擾
		pMesh = pObjManager->GetMeshField();
	}

	if (pMesh != nullptr)
	{
		fHeight = pMesh->GetHeight(GetPosition(), &move);
	}

	// �ʒu�̍X�V
	SetPosition(GetPosition() + GetMove());

	// �ڕW�ʒu�Ǘ�
	ManagePosDest();

	if (fHeight >= GetPosition().y)
	{// �ʒu�ݒ�
		SetPosition(D3DXVECTOR3(GetPosition().x, fHeight, GetPosition().z));

		SetMove(D3DXVECTOR3(move.x, 0.0f, move.z));
	}

	// �����蔻��Ǘ�
	ManageCollision();

	if (m_pShadow != nullptr)
	{
		// �e�̈ʒu�ݒ�
		m_pShadow->SetPosition(D3DXVECTOR3(GetPosition().x, m_pShadow->GetPosition().y, GetPosition().z));
	}
}

//=====================================================
// �����蔻��̊Ǘ�
//=====================================================
void CEnemy::ManageCollision(void)
{
	if (m_pCollisionSphere != nullptr)
	{// ���̓����蔻��̊Ǘ�
		float fRadius = GetRadiusMax();

		m_pCollisionSphere->SetPositionOld(m_pCollisionSphere->GetPosition());

		m_pCollisionSphere->SetPosition(GetPosition());

		// �v���C���[���ڋ߂��Ă邩�̔���
		m_pCollisionSphere->SetRadius(CLOSE_DISTANCE);

		if (m_pCollisionSphere->SphereCollision(CCollision::TAG_PLAYER))
		{
			m_nCntClose++;
		}

		m_pCollisionSphere->SetRadius(fRadius * RATE_RADIUS);
	}

	if (m_pCollisionCube != nullptr)
	{// �����̂̓����蔻��̊Ǘ�
	 // �p�[�c�̍ő唼�a�擾
		float fRadius = GetRadiusMax();

		// �����蔻��̈ʒu�ݒ�
		m_pCollisionCube->SetPosition(GetPosition());

		D3DXVECTOR3 vtxMax = { fRadius,fRadius,fRadius };
		D3DXVECTOR3 vtxMin = { -fRadius,10.0f,-fRadius };

		D3DXVECTOR3 move = GetMove();

		if (GetState() != STATE_DEATH)
		{
			// �����o���̓����蔻��
			m_pCollisionCube->CubeCollision(CCollision::TAG_BLOCK, &move);
		}
		else
		{
			// �u���b�N�ɂԂ������甚��
			if (m_pCollisionCube->TriggerCube(CCollision::TAG_BLOCK))
			{
				Death();
			}
		}

		SetMove(move);
	}
}

//=====================================================
// �^�[�Q�b�g��������
//=====================================================
void CEnemy::SearchTarget(void)
{
	CPlayer *pPlayer = nullptr;
	CObjectManager *pObjManager = CManager::GetObjectManager();

	if (pObjManager != nullptr)
	{// �v���C���[�̓K�p
		pPlayer = pObjManager->GetPlayer();
	}

	CMeshField *pMesh = nullptr;

	if (pObjManager != nullptr)
	{// ���b�V���t�B�[���h�̎擾
		pMesh = pObjManager->GetMeshField();
	}

	bool bHitMesh = false;
	bool bHitBlock = false;
	D3DXVECTOR3 vecDiff;
	D3DXVECTOR3 posTarget;

	if (pPlayer == nullptr)
	{
		return;
	}

	posTarget = pPlayer->GetPosition();
	posTarget.y += 50.0f;

	// �ڕW�ʒu�Ƃ̍����擾
	vecDiff = posTarget - GetPosition();

	if (pMesh != nullptr)
	{
		// ���b�V���Ƃ̔���
		bHitMesh = pMesh->RayCheck(GetPosition(),vecDiff,GetPosition());
	}

	if (m_pCollisionSphere != nullptr)
	{// �u���b�N�Ƃ̔���
		bHitBlock = m_pCollisionSphere->ChckObstclBtwn(this, vecDiff);
	}

	if (bHitBlock || bHitMesh)
	{// ����
		m_bFind = true;
	}
	else
	{
		m_bFind = false;
	}

	//CManager::GetDebugProc()->Print("\n����[%d]",m_bFind);
}

//=====================================================
// ��ԊǗ�
//=====================================================
void CEnemy::ManageState(void)
{
	switch (m_state)
	{
	case STATE_NORMAL:
		break;
	case STATE_DAMAGE:
		if (m_nTimerState > DAMAGE_FRAME)
		{// �ʏ��Ԃɖ߂�
			m_nTimerState = 0;
			m_state = STATE_NORMAL;
		}
		else
		{// �J�E���g�A�b�v
			m_nTimerState++;
		}

		break;
	case STATE_DEATH:
		m_nTimerDeath--;

		if (m_pExplSpawner != nullptr)
		{
			m_pExplSpawner->SetPosition(GetPosition());
		}

		if (m_nTimerDeath <= 0)
		{
			Death();
		}

		break;
	default:
		break;
	}
}

//=====================================================
// �ڕW�ʒu�Ǘ�
//=====================================================
void CEnemy::ManagePosDest(void)
{
	D3DXVECTOR3 vecDiff;

	// ��Q���̉��
	AvoidCollisions();

	switch (m_moveState)
	{
	case CEnemy::MOVESTATE_CHASE:

		break;
	case CEnemy::MOVESTATE_AVOID:

		vecDiff = m_posDest - GetPosition();

		if (D3DXVec3Length(&vecDiff) < GetRadiusMax())
		{
			m_moveState = MOVESTATE_CHASE;
		}

		break;
	default:
		break;
	}
}

//=====================================================
// ���������鏈��
//=====================================================
void CEnemy::AvoidCollisions(void)
{
	if (m_pCollisionCube == nullptr)
	{
		return;
	}

	// �v�Z�p�ϐ�
	D3DXVECTOR3 vecCollision = { 0.0f,0.0f,0.0f };

	// �u���b�N�̃x�N�g�����擾
	vecCollision = m_pCollisionCube->CollisionVector(this);

	if (D3DXVec3Length(&vecCollision) == 0)
	{
		return;
	}

	m_posDest = GetPosition() + vecCollision;

	m_moveState = MOVESTATE_AVOID;
}

//=====================================================
// �̗͐ݒ�
//=====================================================
void CEnemy::SetLife(float fLife)
{
	m_fLife = fLife;

	if (m_fLife < 0)
	{
		m_fLife = 0;
	}
}

//=====================================================
// �q�b�g����
//=====================================================
void CEnemy::Hit(float fDamage)
{
	// �X�R�A�擾
	CScore *pScore = CGame::GetScore();

	if (m_state == STATE_DEATH)
	{// �����ɔ���
		Death();

		return;
	}

	// �̗͌���
	SetLife(GetLife() - fDamage);

	if (GetLife() <= 0)
	{
		// �����J�E���g�_�E��
		m_nNumAll--;

		ManageBonus();

		// ���S��Ԃɂ���
		m_state = STATE_DEATH;

		SetLife(0.0f);

		if (pScore != nullptr)
		{
			pScore->AddScore(m_nScore);
		}

		m_pExplSpawner = CExplSpawner::Create(GetPosition(), GetRadiusMax(), TIME_DEATH, &m_pExplSpawner);

		m_nTimerDeath = TIME_DEATH;
	}
	else
	{
		m_state = STATE_DAMAGE;
	}
}

//=====================================================
// �{�[�i�X�̊Ǘ�����
//=====================================================
void CEnemy::ManageBonus(void)
{
	CScore *pScore = CGame::GetScore();
	CPlayer *pPlayer = nullptr;
	CObjectManager *pObjManager = CManager::GetObjectManager();

	if (pObjManager != nullptr)
	{// �v���C���[�̓K�p
		pPlayer = pObjManager->GetPlayer();
	}

	int nScore = 0;

	if (pPlayer != nullptr)
	{
		if (pPlayer->IsLand() == false)
		{
			pPlayer->SetAirCounter(pPlayer->GetAirCounter() + 1);

			if (pPlayer->GetAirCounter() > NUM_AIR_BONUS)
			{
				nScore += AIR_SCORE;
			}
		}
	}

	if (pScore != nullptr)
	{// �X�R�A���Z
		pScore->AddScore(nScore);
	}
}

//=====================================================
// ���S����
//=====================================================
void CEnemy::Death(void)
{
	// �����̔���
	CParticle::Create(GetPosition(), CParticle::TYPE_EXPLOSION);

	CExplosion::Create(GetPosition())->SetSize(300.0f, 300.0f);

	CDebrisSpawner::Create(GetPosition(), 12.0f, 1, 12);

	// ���g�̏I��
	Uninit();
}

//=====================================================
// �`�揈��
//=====================================================
void CEnemy::Draw(void)
{
	// �}�g���b�N�X�̐ݒ�
	SetMatrix();

	// �p���N���X�̕`��
	CMotion::Draw();
}

//=====================================================
// �U���J�E���^�[�ݒ�
//=====================================================
void CEnemy::SetCntAttack(int nValue)
{
	m_nCntAttack = nValue;
}

//=====================================================
// ���̓����蔻��̈ʒu�ݒ�
//=====================================================
void CEnemy::SetSpherePosition(D3DXVECTOR3 pos)
{
	if (m_pCollisionSphere != nullptr)
	{// �ʒu�ݒ�
		m_pCollisionSphere->SetPosition(pos);
	}
}

//=====================================================
// �Ǎ�����
//=====================================================
HRESULT CEnemy::Load(void)
{
	// �ϐ��錾
	char cTemp[256] = {};
	int nCntWave = 0;
	int nCntEnemy = 0;

	// �t�@�C������ǂݍ���
	FILE *pFile = fopen("data\\enemywave.txt", "r");

	if (pFile != nullptr)
	{// �t�@�C�����J�����ꍇ
		while (true)
		{
			// �����ǂݍ���
			fscanf(pFile, "%s", &cTemp[0]);

			if (strcmp(cTemp, "NUM_WAVE") == 0)
			{// �E�F�[�u���擾
				// "="�ǂݍ���
				fscanf(pFile, "%s", &cTemp[0]);

				// �E�F�[�u���ǂݍ���
				fscanf(pFile, "%d", &m_nNumWave);

				// �E�F�[�u��񐶐�
				if (m_pWaveInfo == nullptr)
				{
					m_pWaveInfo = new WAVEINFO[m_nNumWave];

					ZeroMemory(m_pWaveInfo, sizeof(WAVEINFO) * m_nNumWave);
				}
			}

			if (strcmp(cTemp, "WAVESET") == 0)
			{// �Ǎ��J�n
				nCntEnemy = 0;

				while (true)
				{
					// �����ǂݍ���
					fscanf(pFile, "%s", &cTemp[0]);

					if (strcmp(cTemp, "NUM_ENEMY") == 0)
					{// �G���擾
						// "="�ǂݍ���
						fscanf(pFile, "%s", &cTemp[0]);

						// �G���ǂݍ���
						fscanf(pFile, "%d", &m_pWaveInfo[nCntWave].m_nNumEnemy);

						if (m_pWaveInfo[nCntWave].pEnemyInfo == nullptr)
						{// �G��񐶐�
							m_pWaveInfo[nCntWave].pEnemyInfo = new ENEMYINFO[m_pWaveInfo[nCntWave].m_nNumEnemy];
						}
					}

					if (strcmp(cTemp, "ENEMYSET") == 0)
					{// �G���擾
						while (true)
						{
							fscanf(pFile, "%s", &cTemp[0]);

							if (strcmp(cTemp, "TYPE") == 0)
							{// ��ގ擾
								// "="�ǂݍ���
								fscanf(pFile, "%s", &cTemp[0]);

								// ��ޓǂݍ���
								fscanf(pFile, "%d", &m_pWaveInfo[nCntWave].pEnemyInfo[nCntEnemy].type);
							}

							if (strcmp(cTemp, "POS") == 0)
							{// �ʒu�擾
								// "="�ǂݍ���
								fscanf(pFile, "%s", &cTemp[0]);

								// �ʒu�ǂݍ���
								fscanf(pFile, "%f", &m_pWaveInfo[nCntWave].pEnemyInfo[nCntEnemy].pos.x);
								fscanf(pFile, "%f", &m_pWaveInfo[nCntWave].pEnemyInfo[nCntEnemy].pos.y);
								fscanf(pFile, "%f", &m_pWaveInfo[nCntWave].pEnemyInfo[nCntEnemy].pos.z);
							}

							if (strcmp(cTemp, "END_ENEMYSET") == 0)
							{// while�𔲂���
								break;
							}
						}

						nCntEnemy++;
					}

					if (strcmp(cTemp, "END_WAVESET") == 0)
					{// while�𔲂���
						break;
					}
				}

				nCntWave++;
			}

			if (strcmp(cTemp, "END_SCRIPT") == 0)
			{// �I������
				break;
			}
		}

		// �t�@�C�������
		fclose(pFile);
	}
	else
	{
		return E_FAIL;
	}

	return S_OK;
}

//=====================================================
// �j������
//=====================================================
void CEnemy::Unload(void)
{
	if (m_pWaveInfo != nullptr)
	{// �E�F�[�u���̔j��
		for (int nCntWave = 0;nCntWave < m_nNumWave;nCntWave++)
		{
			if (m_pWaveInfo[nCntWave].pEnemyInfo != nullptr)
			{// �G���̔j��
				delete m_pWaveInfo[nCntWave].pEnemyInfo;
				m_pWaveInfo[nCntWave].pEnemyInfo = nullptr;
			}
		}

		delete m_pWaveInfo;
		m_pWaveInfo = nullptr;
	}
}

//=====================================================
// �X�|�[������
//=====================================================
void CEnemy::SpawnEnemy(int nNumWave)
{
	// ��q�ڕW���擾
	CDefend *pDefend = CGame::GetDefend();

	// �ėp�����̎擾
	CUniversal *pUniversal = CManager::GetUniversal();

	// �ϐ��錾
	int nType = 0;
	D3DXVECTOR3 pos,rot = { 0.0f,0.0f,0.0f },vecDiff = { 0.0f,0.0f,0.0f };
	D3DXMATRIX mtxWorld,mtxTemp,mtxEnemy;
	D3DXMATRIX mtxRot, mtxTrans;
	CEnemy *pEnemy = nullptr;

	// �}�g���b�N�X������
	D3DXMatrixIdentity(&mtxTemp);

	if (m_pWaveInfo != nullptr)
	{// ��ނ̐ݒ�
		nType = rand() % m_nNumWave;
	}

	for (int nCntWave = 0; nCntWave < nNumWave; nCntWave++)
	{
		pos.x = (float)(rand() % 2000 - 1000);
		pos.y = 0.0f;
		pos.z = (float)(rand() % 2000 - 1000);

		// ���K��
		D3DXVec3Normalize(&pos,&pos);

		// ������������
		pos *= ENEMY_DISTANCE;

		if (pDefend != nullptr)
		{
			// �����̃x�N�g���Z�o
			vecDiff = pDefend->GetPosition() - pos;
		}

		// �p�x�Z�o
		rot.y = atan2f(vecDiff.x,vecDiff.z);

		// �X�|�[���n�_�̃}�g���b�N�X�ݒ�
		// �}�g���b�N�X������
		D3DXMatrixIdentity(&mtxWorld);

		// �����𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot,rot.y, rot.x, rot.z);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

		// �ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

		// �}�g���b�N�X���������킹��
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTemp);

		for (int nCntEnemy = 0;nCntEnemy < m_pWaveInfo[nType].m_nNumEnemy;nCntEnemy++)
		{
			pEnemy = nullptr;

			// �X�|�[���n�_�ƓG�̃}�g���b�N�X���������킹��
			pUniversal->SetOffSet(&mtxEnemy, mtxWorld, m_pWaveInfo[nType].pEnemyInfo[nCntEnemy].pos);

			pos = { mtxEnemy._41,mtxEnemy._42 ,mtxEnemy._43 };

			pEnemy = CEnemy::Create(pos, CEnemy::m_pWaveInfo[nType].pEnemyInfo[nCntEnemy].type);

			if (pEnemy != nullptr)
			{
				pEnemy->SetRot(rot);
			}
		}
	}
}