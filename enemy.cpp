//*****************************************************
//
// GÌ[enemy.cpp]
// Author:ûüRç
//
//*****************************************************

//*****************************************************
// CN[h
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
#include "sound.h"

//*****************************************************
// }Nè`
//*****************************************************
#define SPEED_MOVE	(1.0f)	// Ú®¬x
#define DEFAULT_SIZE	(130.0f)		// îÌå«³
#define DEFAULT_DIST	(2000.0f)		// îÌ£
#define PATH_LOCKON	("data\\TEXTURE\\UI\\lockon002.png")	// bNI}[J[ÌpX
#define ENEMY_DISTANCE	(6000)	// Gªo»·é£
#define RATE_RADIUS	(1.5f)	// ½è»èÌå«³Ì{¦
#define INITIAL_LIFE	(1.0f)	// úÌÍ
#define DAMAGE_FRAME	(10)	// _[WóÔÌp±t[
#define TIME_DEATH	(100)	// SÜÅÌt[
#define AVOID_LENGTH	(3.0f)	// ÇÕÉ¤ÂéÜÅÌ£
#define NUM_AIR_BONUS	(3)	// óÅ{[iXªüéÜÅÄ
#define AIR_SCORE	(300)	// óÄÌ{[iX
#define INITIAL_SCORE	(1000)	// úXRA
#define CLOSE_DISTANCE	(1000.0f)	// ß¢Æ»f·é£
#define BONUS_TIME	(420)	// {[iXªá¦éîÌt[

//*****************************************************
// ÃIoÏé¾
//*****************************************************
int CEnemy::m_nNumAll = 0;	// 
int CEnemy::m_nNumWave = 0;	// EF[u
CEnemy::WAVEINFO *CEnemy::m_pWaveInfo = nullptr;	// GîñÖÌ|C^
CEnemy *CEnemy::m_pHead = nullptr;	// æªÌAhX
CEnemy *CEnemy::m_pTail = nullptr;	// ÅãöÌAhX

//=====================================================
// RXgN^
//=====================================================
CEnemy::CEnemy()
{
	// JEgAbv
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

	// lÌNA
	m_pPrev = nullptr;
	m_pNext = nullptr;

	if (m_pHead == nullptr)
	{// æªÆÅãöAhXÌãü
		m_pHead = this;
		m_pTail = this;

		return;
	}

	// OÌAhXÉÅãöÌAhXðãü·é
	m_pPrev = m_pTail;

	// ÅãöÌAhXð©ªÉ·é
	m_pTail = this;

	// OÌIuWFNgÌÌAhXð©ªÉ·é
	m_pPrev->m_pNext = this;
}

//=====================================================
// fXgN^
//=====================================================
CEnemy::~CEnemy()
{
	if (m_state != STATE_DEATH)
	{
		m_nNumAll--;
	}

	if (m_pHead == this)
	{// æªAhXÌjü
	 //if (m_pNext != nullptr)
		{// æªAhXðÌAhXÉø«p®
			m_pHead = m_pNext;

			if (m_pNext != nullptr)
			{
				m_pNext->m_pPrev = nullptr;
			}
		}
	}
	else if (m_pTail == this)
	{// ÅãöAhXÌjü
		if (m_pPrev != nullptr)
		{// ÅãöAhXðOÌAhXÉø«p®
			m_pTail = m_pPrev;

			m_pPrev->m_pNext = nullptr;
		}
	}
	else
	{// ^ñÌAhXÌjü
	 // OÌAhX©çÌAhXðÂÈ®
		m_pPrev->m_pNext = m_pNext;

		// ÌAhX©çOÌAhXðÂÈ®
		m_pNext->m_pPrev = m_pPrev;
	}
}

//=====================================================
// ¶¬
//=====================================================
CEnemy *CEnemy::Create(D3DXVECTOR3 pos, TYPE type)
{
	CEnemy *pEnemy = nullptr;

	if (pEnemy == nullptr)
	{// CX^X¶¬
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

		// eÌ¶¬
		if (pEnemy->m_pShadow == nullptr)
		{
			pEnemy->m_pShadow = CShadow::Create(D3DXVECTOR3(pEnemy->GetPosition().x, 1.0f, pEnemy->GetPosition().z), 150.0f, 150.0f);

			if (pEnemy->m_pShadow != nullptr)
			{
				pEnemy->m_pShadow->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
			}
		}

		pEnemy->SetPosition(D3DXVECTOR3(pos));

		// ú»
		pEnemy->Init();
	}

	return pEnemy;
}

//=====================================================
// ú»
//=====================================================
HRESULT CEnemy::Init(void)
{
	// Ïé¾
	float fRadius = GetRadiusMax();

	// p³NXÌú»
	CMotion::Init();

	// ^CvÌÝè
	SetType(TYPE_ENEMY);

	if (m_pCollisionSphere == nullptr)
	{// Ì½è»è¶¬
		m_pCollisionSphere = CCollisionSphere::Create(CCollision::TAG_ENEMY, CCollision::TYPE_SPHERE,this);

		if (m_pCollisionSphere != nullptr)
		{// îñÌÝè
			m_pCollisionSphere->SetPosition(GetPosition());
			m_pCollisionSphere->SetRadius(fRadius * RATE_RADIUS);
		}
	}

	if (m_pCollisionCube == nullptr)
	{// §ûÌÌ½è»è
		m_pCollisionCube = CCollisionCube::Create(CCollision::TAG_ENEMY, this);

		D3DXVECTOR3 vtxMax = { fRadius,fRadius,fRadius };
		D3DXVECTOR3 vtxMin = { -fRadius,-fRadius,-fRadius };

		if (m_pCollisionCube != nullptr)
		{
			m_pCollisionCube->SetVtx(vtxMax, vtxMin);
		}
	}

	// ÌÍÌúÝè
	m_fLife = INITIAL_LIFE;

	// ÊíóÔÉ·é
	m_moveState = MOVESTATE_CHASE;
	m_state = STATE_NORMAL;

	m_nScore = INITIAL_SCORE;

	return S_OK;
}

//=====================================================
// I¹
//=====================================================
void CEnemy::Uninit(void)
{
	if (m_pCollisionSphere != nullptr)
	{// ½è»èÌÁ
		m_pCollisionSphere->Uninit();

		m_pCollisionSphere = nullptr;
	}

	if (m_pShadow != nullptr)
	{// eÌí
		m_pShadow->Uninit();

		m_pShadow = nullptr;
	}

	if (m_pExplSpawner != nullptr)
	{
		m_pExplSpawner->Uninit();
		m_pExplSpawner = nullptr;
	}

	// p³NXÌI¹
	CMotion::Uninit();
}

//=====================================================
// XV
//=====================================================
void CEnemy::Update(void)
{
	if (CGame::GetState() != CGame::STATE_NORMAL)
	{
		return;
	}

	// p³NXÌXV
	CMotion::Update();

	// óÔÇ
	ManageState();

	// UJE^[ÁZ
	m_nCntAttack++;

	if (m_nCntAttack == INT_MAX)
	{// lÌâ³
		m_nCntAttack = 0;
	}

	// bVtB[hÆÌ½è»è
	D3DXVECTOR3 move = GetMove();
	float fHeight = 0.0f;
	CMeshField *pMesh = nullptr;
	CObjectManager *pObjManager = CManager::GetObjectManager();

	if (pObjManager != nullptr)
	{// bVtB[hÌæ¾
		pMesh = pObjManager->GetMeshField();
	}

	if (pMesh != nullptr)
	{
		fHeight = pMesh->GetHeight(GetPosition(), &move);
	}

	// ÊuÌXV
	SetPosition(GetPosition() + GetMove());

	// ÚWÊuÇ
	ManagePosDest();

	if (fHeight >= GetPosition().y)
	{// ÊuÝè
		SetPosition(D3DXVECTOR3(GetPosition().x, fHeight, GetPosition().z));

		SetMove(D3DXVECTOR3(move.x, 0.0f, move.z));
	}

	// ½è»èÇ
	ManageCollision();

	if (m_pShadow != nullptr)
	{
		// eÌÊuÝè
		m_pShadow->SetPosition(D3DXVECTOR3(GetPosition().x, m_pShadow->GetPosition().y, GetPosition().z));
	}
}

//=====================================================
// ½è»èÌÇ
//=====================================================
void CEnemy::ManageCollision(void)
{
	if (m_pCollisionSphere != nullptr)
	{// Ì½è»èÌÇ
		float fRadius = GetRadiusMax();

		m_pCollisionSphere->SetPositionOld(m_pCollisionSphere->GetPosition());

		m_pCollisionSphere->SetPosition(GetPosition());

		// vC[ªÚßµÄé©Ì»è
		m_pCollisionSphere->SetRadius(CLOSE_DISTANCE);

		if (m_pCollisionSphere->SphereCollision(CCollision::TAG_PLAYER))
		{
			m_nCntClose++;
		}

		m_pCollisionSphere->SetRadius(fRadius * RATE_RADIUS);
	}

	if (m_pCollisionCube != nullptr)
	{// §ûÌÌ½è»èÌÇ
	 // p[cÌÅå¼aæ¾
		float fRadius = GetRadiusMax();

		// ½è»èÌÊuÝè
		m_pCollisionCube->SetPosition(GetPosition());

		D3DXVECTOR3 vtxMax = { fRadius,fRadius,fRadius };
		D3DXVECTOR3 vtxMin = { -fRadius,10.0f,-fRadius };

		D3DXVECTOR3 move = GetMove();

		if (GetState() != STATE_DEATH)
		{
			// µoµÌ½è»è
			m_pCollisionCube->CubeCollision(CCollision::TAG_BLOCK, &move);
		}
		else
		{
			// ubNÉÔÂ©Á½ç­
			if (m_pCollisionCube->TriggerCube(CCollision::TAG_BLOCK))
			{
				Death();
			}
		}

		SetMove(move);
	}
}

//=====================================================
// ^[Qbg­©»è
//=====================================================
void CEnemy::SearchTarget(void)
{
	CPlayer *pPlayer = nullptr;
	CObjectManager *pObjManager = CManager::GetObjectManager();

	if (pObjManager != nullptr)
	{// vC[ÌKp
		pPlayer = pObjManager->GetPlayer();
	}

	CMeshField *pMesh = nullptr;

	if (pObjManager != nullptr)
	{// bVtB[hÌæ¾
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

	// ÚWÊuÆÌ·ªæ¾
	vecDiff = posTarget - GetPosition();

	if (pMesh != nullptr)
	{
		// bVÆÌ»è
		bHitMesh = pMesh->RayCheck(GetPosition(),vecDiff,GetPosition());
	}

	if (m_pCollisionSphere != nullptr)
	{// ubNÆÌ»è
		bHitBlock = m_pCollisionSphere->ChckObstclBtwn(this, vecDiff);
	}

	if (bHitBlock || bHitMesh)
	{// ­©
		m_bFind = true;
	}
	else
	{
		m_bFind = false;
	}

	//CManager::GetDebugProc()->Print("\n­©[%d]",m_bFind);
}

//=====================================================
// óÔÇ
//=====================================================
void CEnemy::ManageState(void)
{
	switch (m_state)
	{
	case STATE_NORMAL:
		break;
	case STATE_DAMAGE:
		if (m_nTimerState > DAMAGE_FRAME)
		{// ÊíóÔÉßé
			m_nTimerState = 0;
			m_state = STATE_NORMAL;
		}
		else
		{// JEgAbv
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
// ÚWÊuÇ
//=====================================================
void CEnemy::ManagePosDest(void)
{
	D3DXVECTOR3 vecDiff;

	// áQ¨Ìñð
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
// »èðð¯é
//=====================================================
void CEnemy::AvoidCollisions(void)
{
	if (m_pCollisionCube == nullptr)
	{
		return;
	}

	// vZpÏ
	D3DXVECTOR3 vecCollision = { 0.0f,0.0f,0.0f };

	// ubNÌxNgðæ¾
	vecCollision = m_pCollisionCube->CollisionVector(this);

	if (D3DXVec3Length(&vecCollision) == 0)
	{
		return;
	}

	m_posDest = GetPosition() + vecCollision;

	m_moveState = MOVESTATE_AVOID;
}

//=====================================================
// ÌÍÝè
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
// qbg
//=====================================================
void CEnemy::Hit(float fDamage)
{
	// XRAæ¾
	CScore *pScore = CGame::GetScore();

	if (m_state == STATE_DEATH)
	{// ·®É­
		Death();

		return;
	}

	// ÌÍ¸­
	SetLife(GetLife() - fDamage);

	if (GetLife() <= 0)
	{
		// JEg_E
		m_nNumAll--;

		ManageBonus();

		// SóÔÉ·é
		m_state = STATE_DEATH;

		SetLife(0.0f);

		if (pScore != nullptr)
		{
			pScore->AddScore(m_nScore);
		}

		m_pExplSpawner = CExplSpawner::Create(GetPosition(), GetRadiusMax(), TIME_DEATH);

		m_nTimerDeath = TIME_DEATH;

		// SEÄ¶
		CManager::GetSound()->Play(CSound::LABEL_EXPLOSION);
	}
	else
	{
		m_state = STATE_DAMAGE;
	}
}

//=====================================================
// {[iXÌÇ
//=====================================================
void CEnemy::ManageBonus(void)
{
	CScore *pScore = CGame::GetScore();
	CPlayer *pPlayer = nullptr;
	CObjectManager *pObjManager = CManager::GetObjectManager();

	if (pObjManager != nullptr)
	{// vC[ÌKp
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
	{// XRAÁZ
		pScore->AddScore(nScore);
	}
}

//=====================================================
// S
//=====================================================
void CEnemy::Death(void)
{
	// ©ªÌ­
	CParticle::Create(GetPosition(), CParticle::TYPE_EXPLOSION);

	CExplosion::Create(GetPosition())->SetSize(300.0f, 300.0f);

	CDebrisSpawner::Create(GetPosition(), 12.0f, 1, 12);

	// ©gÌI¹
	Uninit();

	// SEÄ¶
	CManager::GetSound()->Play(CSound::LABEL_EXPLOSION);
}

//=====================================================
// `æ
//=====================================================
void CEnemy::Draw(void)
{
	// }gbNXÌÝè
	SetMatrix();

	// p³NXÌ`æ
	CMotion::Draw();
}

//=====================================================
// UJE^[Ýè
//=====================================================
void CEnemy::SetCntAttack(int nValue)
{
	m_nCntAttack = nValue;
}

//=====================================================
// Ì½è»èÌÊuÝè
//=====================================================
void CEnemy::SetSpherePosition(D3DXVECTOR3 pos)
{
	if (m_pCollisionSphere != nullptr)
	{// ÊuÝè
		m_pCollisionSphere->SetPosition(pos);
	}
}

//=====================================================
// Ç
//=====================================================
HRESULT CEnemy::Load(void)
{
	// Ïé¾
	char cTemp[256] = {};
	int nCntWave = 0;
	int nCntEnemy = 0;

	// t@C©çÇÝÞ
	FILE *pFile = fopen("data\\enemywave.txt", "r");

	if (pFile != nullptr)
	{// t@CªJ¯½ê
		while (true)
		{
			// ¶ÇÝÝ
			fscanf(pFile, "%s", &cTemp[0]);

			if (strcmp(cTemp, "NUM_WAVE") == 0)
			{// EF[uæ¾
				// "="ÇÝÝ
				fscanf(pFile, "%s", &cTemp[0]);

				// EF[uÇÝÝ
				fscanf(pFile, "%d", &m_nNumWave);

				// EF[uîñ¶¬
				if (m_pWaveInfo == nullptr)
				{
					m_pWaveInfo = new WAVEINFO[m_nNumWave];

					ZeroMemory(m_pWaveInfo, sizeof(WAVEINFO) * m_nNumWave);
				}
			}

			if (strcmp(cTemp, "WAVESET") == 0)
			{// ÇJn
				nCntEnemy = 0;

				while (true)
				{
					// ¶ÇÝÝ
					fscanf(pFile, "%s", &cTemp[0]);

					if (strcmp(cTemp, "NUM_ENEMY") == 0)
					{// Gæ¾
						// "="ÇÝÝ
						fscanf(pFile, "%s", &cTemp[0]);

						// GÇÝÝ
						fscanf(pFile, "%d", &m_pWaveInfo[nCntWave].m_nNumEnemy);

						if (m_pWaveInfo[nCntWave].pEnemyInfo == nullptr)
						{// Gîñ¶¬
							m_pWaveInfo[nCntWave].pEnemyInfo = new ENEMYINFO[m_pWaveInfo[nCntWave].m_nNumEnemy];
						}
					}

					if (strcmp(cTemp, "ENEMYSET") == 0)
					{// Gîñæ¾
						while (true)
						{
							fscanf(pFile, "%s", &cTemp[0]);

							if (strcmp(cTemp, "TYPE") == 0)
							{// íÞæ¾
								// "="ÇÝÝ
								fscanf(pFile, "%s", &cTemp[0]);

								// íÞÇÝÝ
								fscanf(pFile, "%d", &m_pWaveInfo[nCntWave].pEnemyInfo[nCntEnemy].type);
							}

							if (strcmp(cTemp, "POS") == 0)
							{// Êuæ¾
								// "="ÇÝÝ
								fscanf(pFile, "%s", &cTemp[0]);

								// ÊuÇÝÝ
								fscanf(pFile, "%f", &m_pWaveInfo[nCntWave].pEnemyInfo[nCntEnemy].pos.x);
								fscanf(pFile, "%f", &m_pWaveInfo[nCntWave].pEnemyInfo[nCntEnemy].pos.y);
								fscanf(pFile, "%f", &m_pWaveInfo[nCntWave].pEnemyInfo[nCntEnemy].pos.z);
							}

							if (strcmp(cTemp, "END_ENEMYSET") == 0)
							{// whileð²¯é
								break;
							}
						}

						nCntEnemy++;
					}

					if (strcmp(cTemp, "END_WAVESET") == 0)
					{// whileð²¯é
						break;
					}
				}

				nCntWave++;
			}

			if (strcmp(cTemp, "END_SCRIPT") == 0)
			{// I¹ð
				break;
			}
		}

		// t@CðÂ¶é
		fclose(pFile);
	}
	else
	{
		return E_FAIL;
	}

	return S_OK;
}

//=====================================================
// jü
//=====================================================
void CEnemy::Unload(void)
{
	if (m_pWaveInfo != nullptr)
	{// EF[uîñÌjü
		for (int nCntWave = 0;nCntWave < m_nNumWave;nCntWave++)
		{
			if (m_pWaveInfo[nCntWave].pEnemyInfo != nullptr)
			{// GîñÌjü
				delete m_pWaveInfo[nCntWave].pEnemyInfo;
				m_pWaveInfo[nCntWave].pEnemyInfo = nullptr;
			}
		}

		delete m_pWaveInfo;
		m_pWaveInfo = nullptr;
	}
}

//=====================================================
// X|[
//=====================================================
void CEnemy::SpawnEnemy(int nNumWave)
{
	// ìqÚWîñæ¾
	CDefend *pDefend = CGame::GetDefend();

	// ÄpÌæ¾
	CUniversal *pUniversal = CManager::GetUniversal();

	// Ïé¾
	int nType = 0;
	D3DXVECTOR3 pos,rot = { 0.0f,0.0f,0.0f },vecDiff = { 0.0f,0.0f,0.0f };
	D3DXMATRIX mtxWorld,mtxTemp,mtxEnemy;
	D3DXMATRIX mtxRot, mtxTrans;
	CEnemy *pEnemy = nullptr;

	// }gbNXú»
	D3DXMatrixIdentity(&mtxTemp);

	if (m_pWaveInfo != nullptr)
	{// íÞÌÝè
		nType = rand() % m_nNumWave;
	}

	for (int nCntWave = 0; nCntWave < nNumWave; nCntWave++)
	{
		pos.x = (float)(rand() % 2000 - 1000);
		pos.y = 0.0f;
		pos.z = (float)(rand() % 2000 - 1000);

		// ³K»
		D3DXVec3Normalize(&pos,&pos);

		// £ð©¯é
		pos *= ENEMY_DISTANCE;

		if (pDefend != nullptr)
		{
			// ·ªÌxNgZo
			vecDiff = pDefend->GetPosition() - pos;
		}

		// pxZo
		rot.y = atan2f(vecDiff.x,vecDiff.z);

		// X|[n_Ì}gbNXÝè
		// }gbNXú»
		D3DXMatrixIdentity(&mtxWorld);

		// ü«ð½f
		D3DXMatrixRotationYawPitchRoll(&mtxRot,rot.y, rot.x, rot.z);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

		// Êuð½f
		D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

		// }gbNXð©¯í¹é
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTemp);

		for (int nCntEnemy = 0;nCntEnemy < m_pWaveInfo[nType].m_nNumEnemy;nCntEnemy++)
		{
			pEnemy = nullptr;

			// X|[n_ÆGÌ}gbNXð©¯í¹é
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