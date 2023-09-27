//*****************************************************
//
// íÔGÌ[enemytank.cpp]
// Author:ûüRç
//
//*****************************************************

//*****************************************************
// CN[h
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
// }Nè`
//*****************************************************
#define SPEED_MOVE	(5.5f)	// Ú®¬x
#define ATTACK_FREQ	(180)	// U·épx
#define BULLET_SPEED	(90.0f)	// eÌXs[h
#define INITIAL_HEIGHT	(300.0f)	// úÌ³
#define ATTACK_RANGE	(4000.0f)	// U·é£
#define DEFAULT_SPEED	(4.0f)	// îÌXs[h
#define DEFAULT_TILT	(-D3DX_PI * 0.1f)	// îÌX«
#define TANK_HEIGHT	(150.0f)	// ³
#define LIFE_TANK	(25)	// úÌÍ
#define INITIAL_SCORE	(4000)	// úXRA

//=====================================================
// RXgN^
//=====================================================
CEnemyTank::CEnemyTank()
{
	
}

//=====================================================
// fXgN^
//=====================================================
CEnemyTank::~CEnemyTank()
{

}

//=====================================================
// ú»
//=====================================================
HRESULT CEnemyTank::Init(void)
{
	// foCXÌæ¾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// p³NXÌú»
	CEnemy::Init();

	// ­µãÉãªé
	SetPosition(D3DXVECTOR3(GetPosition().x, GetPosition().y + INITIAL_HEIGHT, GetPosition().z));

	// ÌÍÝè
	SetLife(LIFE_TANK);

	// XRAÝè
	SetScore(INITIAL_SCORE);

	return S_OK;
}

//=====================================================
// I¹
//=====================================================
void CEnemyTank::Uninit(void)
{
	// p³NXÌI¹
	CEnemy::Uninit();
}

//=====================================================
// XV
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

	// ÊuðÛ¶
	SetPositionOld(GetPosition());

	// ÇÕ
	CEnemyTank::ChaseDefend();

	// Ú®Ê¸
	SetMove(D3DXVECTOR3(GetMove().x * 0.98f, GetMove().y - 0.98f, GetMove().z * 0.98f));

	// p³NXÌXV
	CEnemy::Update();
}

//=====================================================
// ìqÚWÇÕ
//=====================================================
void CEnemyTank::ChaseDefend(void)
{
	if (GetState() == STATE_DEATH)
	{
		return;
	}

	// vC[îñæ¾
	CPlayer *pPlayer = nullptr;
	CObjectManager *pObjManager = CManager::GetObjectManager();

	if (pObjManager != nullptr)
	{// vC[ÌKp
		pPlayer = pObjManager->GetPlayer();
	}

	if (pPlayer != nullptr)
	{
		if (GetMoveState() == MOVESTATE_CHASE)
		{
			SetPosDest(pPlayer->GetPosition());
		}
	}

	// Ïé¾
	D3DXVECTOR3 vecDest;

	// ·ªðæ¾
	vecDest = GetPosDest() - GetPosition();

	float length = D3DXVec3Length(&vecDest);

	// ÚWpxðæ¾
	float fRotDest = atan2f(vecDest.x, vecDest.z) + D3DX_PI;

	float fRotDiff = fRotDest - GetRot().y;

	// pxÌC³
	if (fRotDiff > D3DX_PI)
	{
		fRotDiff -= 6.28f;
	}
	if (fRotDiff < -D3DX_PI)
	{
		fRotDiff += 6.28f;
	}

	// pxâ³
	SetRot(D3DXVECTOR3(GetRot().x, GetRot().y + fRotDiff * 0.1f, GetRot().z));

	// pxÌC³
	if (GetRot().y > D3DX_PI)
	{
		// pxâ³
		SetRot(D3DXVECTOR3(GetRot().x, GetRot().y - 6.28f, GetRot().z));
	}
	if (GetRot().y < -D3DX_PI)
	{
		// pxâ³
		SetRot(D3DXVECTOR3(GetRot().x, GetRot().y + 6.28f, GetRot().z));
	}

	if (length > ATTACK_RANGE)
	{// ÚW©ç£êÄ¢½ç
		// ÚWûüÖiÞ
		SetMove(D3DXVECTOR3
		(
			-sinf(GetRot().y) * SPEED_MOVE,
			GetMove().y,
			-cosf(GetRot().y) * SPEED_MOVE
		));
	}
	else
	{// ÚWÉßÃ¢Ä¢½ç

		if (GetMoveState() == MOVESTATE_CHASE)
		{
			if (GetCntAttack() >= ATTACK_FREQ)
			{// êèÔ²ÆÉU
				D3DXVec3Normalize(&vecDest, &vecDest);

				CBullet::Create
				(// eÌ­Ë
					D3DXVECTOR3(GetPosition().x, GetPosition().y + TANK_HEIGHT, GetPosition().z),
					vecDest * BULLET_SPEED,
					300,
					CBullet::TYPE_ENEMY,
					false,
					20.0f
				);

				SetCntAttack(0);

				// SEÄ¶
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
// `æ
//=====================================================
void CEnemyTank::Draw(void)
{
	// p³NXÌ`æ
	CEnemy::Draw();
}