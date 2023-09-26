//*****************************************************
//
// Q[[game.cpp]
// Author:ûüRç
//
//*****************************************************

//*****************************************************
// CN[h
//*****************************************************
#include "manager.h"
#include "renderer.h"
#include "game.h"
#include "player.h"
#include "bullet.h"
#include "explosion.h"
#include "enemy.h"
#include "score.h"
#include "timer.h"
#include "block.h"
#include "object.h"
#include "defend.h"
#include "field.h"
#include "meshfield.h"
#include "skybox.h"
#include "universal.h"
#include "particle.h"
#include "edit.h"
#include "inputkeyboard.h"
#include "inputjoypad.h"
#include "fade.h"
#include "meshwall.h"
#include "camera.h"
#include "pause.h"
#include "dustmanager.h"
#include "assess.h"
#include "rank.h"
#include "dust.h"
#include "effect3D.h"
#include "noise.h"
#include "objectmanager.h"
#include "result.h"
#include "sound.h"

//*****************************************************
// }Nè`
//*****************************************************
#define TRANS_TIME	(60)	// I¹ÜÅÌ]CÌt[
#define LIMIT_LENGTH	(10000)	// Ú®§ÀÌ£
#define LIMIT_WIDTH	(100)	// èvC[Ì
#define LIMIT_HEIGHT	(2000)	// §Àx
#define BONUS_TIME	(40)	// {[iXªt^³êéÅáÀÌ^C
#define RATE_BONUS	(0.015f)	// 1b½èÌ^C{[iX
#define RESULT_TIME	(5)	// UgæÊ\¦ÜÅÌO

//*****************************************************
// ÃIoÏé¾
//*****************************************************
CScore *CGame::m_pScore = nullptr;	// XRAÌ|C^
CTimer *CGame::m_pTimer = nullptr;	// ^C}[Ì|C^
CDefend *CGame::m_pDefend = nullptr;	// ìqÎÛÖÌ|C^
CRank *CGame::m_pRank = nullptr;	// NÖÌ|C^
CEdit *CGame::m_pEdit = nullptr;	// GfB^[ÖÌ|C^
CGame::STATE CGame::m_state = STATE_NONE;
bool CGame::m_bPause = false;
CPause *CGame::m_pPause = nullptr;	// |[YÌAhX

//=====================================================
// RXgN^
//=====================================================
CGame::CGame()
{
	ZeroMemory(&m_apMeshWall[0],sizeof(m_apMeshWall));
	m_bPhoto = false;
	m_bPause = false;
	m_nTimerWave = 0;
	m_nCntSecond = 0;
	m_nTimerResult = 0;
}

//=====================================================
// fXgN^
//=====================================================
CGame::~CGame()
{

}

//=====================================================
// ú»
//=====================================================
HRESULT CGame::Init(void)
{
	CPlayer *pPlayer = CPlayer::Create();
	CObjectManager *pObjManager = CManager::GetObjectManager();

	if (pPlayer != nullptr && pObjManager != nullptr)
	{// vC[ÌKp
		pObjManager->BindPlayer(pPlayer);
	}

	if (m_pScore == nullptr)
	{// XRA¶¬
		m_pScore = CScore::Create();
	}

	if (m_pScore != nullptr)
	{// XRAú»
		m_pScore->Init();
	}

	if (m_pTimer == nullptr)
	{// ^C}[¶¬
		m_pTimer = CTimer::Create();
	}

#if _DEBUG
	if (m_pEdit == nullptr)
	{// GfB^[¶¬
		m_pEdit = new CEdit;
	}

	if (m_pEdit != nullptr)
	{// GfB^[ú»
		m_pEdit->Init();
	}
#endif

	if (m_pDefend == nullptr)
	{// ìqÎÛ¶¬
		m_pDefend = CDefend::Create();

		if (m_pDefend != nullptr)
		{
			// ú»
			if (FAILED(m_pDefend->Init()))
			{// ú»É¸sµ½ê
				return E_FAIL;
			}
		}
	}

	// ubNÇ
	CBlock::Load();

	// XJC{bNX¶¬
	CSkybox::Create();

	CMeshField *pMeshField = CMeshField::Create();

	// bVtB[hÌ¶¬
	if (pMeshField != nullptr && pObjManager != nullptr)
	{// bVtB[hÌKp
		pObjManager->BindMeshField(pMeshField);
	}

	// bVEH[Ì¶¬
	m_apMeshWall[WALLTYPE_FRONT] = CMeshWall::Create(D3DXVECTOR3(0.0f, 0.0f, -10000.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f));
	m_apMeshWall[WALLTYPE_REAR] = CMeshWall::Create(D3DXVECTOR3(0.0f, 0.0f, 10000.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_apMeshWall[WALLTYPE_RIGHT] = CMeshWall::Create(D3DXVECTOR3(10000.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f));
	m_apMeshWall[WALLTYPE_LEFT] = CMeshWall::Create(D3DXVECTOR3(-10000.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, -D3DX_PI * 0.5f, 0.0f));

	m_state = STATE_NORMAL;

	// tHOðÂ¯é
	CRenderer::EnableFog(true);

	// oÇÌ¶¬
	CDustManager::Create();

	// G¶¬
	CEnemy::SpawnEnemy(3);

	if (m_pRank == nullptr)
	{// N¶¬
		m_pRank = CRank::Create();
	}

	CManager::GetSound()->Stop();

	// BGMÄ¶
	CManager::GetSound()->Play(CSound::LABEL_BGM001);

	return S_OK;
}

//=====================================================
// I¹
//=====================================================
void CGame::Uninit(void)
{
	if (m_pPause != nullptr)
	{
		m_pPause->Uninit();

		m_pPause = nullptr;
	}

	if (m_pDefend != nullptr)
	{// ìqÎÛÌI¹Ejü
		m_pDefend->Uninit();

		m_pDefend = nullptr;
	}

	if (m_pScore != nullptr)
	{// XRAÌI¹Ejü
		m_pScore->Uninit();

		m_pScore = nullptr;
	}

	if (m_pTimer != nullptr)
	{// ^C}[ÌI¹Ejü
		m_pTimer->Uninit();

		m_pTimer = nullptr;
	}

	if (m_pRank != nullptr)
	{// NÌI¹Ejü
		m_pRank->Uninit();

		m_pRank = nullptr;
	}

#if _DEBUG
	if (m_pEdit != nullptr)
	{// GfB^[ÌI¹Ejü
		m_pEdit->Uninit();

		delete m_pEdit;
		m_pEdit = nullptr;
	}
#endif

	// ubNîñí
	CBlock::DeleteAll();

	// IuWFNgSü
	CObject::ReleaseAll();
}

//=====================================================
// XRAI¹
//=====================================================
void CGame::ReleaseScore(void)
{
	if (m_pScore != nullptr)
	{// XRAÌI¹Ejü
		m_pScore = nullptr;
	}
}

//=====================================================
// ^C}[I¹
//=====================================================
void CGame::ReleaseTimer(void)
{
	if (m_pTimer != nullptr)
	{// XRAÌI¹Ejü
		m_pTimer = nullptr;
	}
}

//=====================================================
// ìqÎÛI¹
//=====================================================
void CGame::ReleaseDefend(void)
{
	// tF[hæ¾
	CFade *pFade = CManager::GetFade();

	if (m_pDefend != nullptr)
	{// ìqÎÛÌI¹Ejü
		m_pDefend = nullptr;
	}

	if (m_state == STATE_NORMAL)
	{
		if (pFade != nullptr)
		{
			m_state = STATE_END;
		}
	}
}

//=====================================================
// XV
//=====================================================
void CGame::Update(void)
{
	// üÍæ¾
	CInputKeyboard *pKeyboard = CManager::GetKeyboard();
	CInputJoypad *pJoypad = CManager::GetJoypad();

	CCamera *pCamera = CManager::GetCamera();

	if (m_bPause == false)
	{
		if (m_bPhoto == false)
		{
			// V[ÌXV
			CScene::Update();
		}

		// JXV
		if (pCamera != nullptr)
		{
			if (m_state == STATE_NORMAL)
			{
				if (CGame::IsPhoto())
				{
					pCamera->Control();
				}
				else
				{
					// Ç]
					pCamera->FollowPlayer();
				}

				// JhêÌ
				pCamera->Quake();
			}
			else if (m_state == STATE_RESULT)
			{
				pCamera->UpdateResult();
			}
		}
	}

	m_nCntSecond++;

	if (m_nCntSecond >= 60)
	{
		m_nTimerWave++;

		m_nCntSecond = 0;
	}

	if (CEnemy::GetNumAll() == 0)
	{// GX|[
		CEnemy::SpawnEnemy(3);

		TimeBonus();

		m_nTimerWave = 0;
	}

#ifdef _DEBUG
	if (m_pEdit != nullptr)
	{
		m_pEdit->Update();
	}

	if (pKeyboard != nullptr)
	{
		if (pKeyboard->GetTrigger(DIK_F))
		{// tHg[hN®
			m_bPhoto = m_bPhoto ? false : true;
		}
	}
#endif

	// óÔÇ
	ManageState();

	// vC[Ú®§À
	LimitPlayerPos();

	// ÇÌÇ
	ManageWall();

	if (pKeyboard != nullptr)
	{
		if (pKeyboard->GetTrigger(DIK_P) || pJoypad->GetTrigger(CInputJoypad::PADBUTTONS_START, 0))
		{// |[YØèÖ¦
			TogglePause(m_pPause);
		}
	}

	if (m_pPause != nullptr)
	{// |[YXV
		m_pPause->Update();
	}

#ifdef _DEBUG	// fobO
	Debug();
#endif
}

//=====================================================
// vC[Êu§À
//=====================================================
void CGame::LimitPlayerPos(void)
{
	D3DXVECTOR3 pos = { 0.0f,0.0f,0.0f };
	D3DXVECTOR3 move = { 0.0f,0.0f,0.0f };

	CPlayer *pPlayer = nullptr;
	CObjectManager *pObjManager = CManager::GetObjectManager();

	if (pObjManager != nullptr)
	{// vC[ÌKp
		pPlayer = pObjManager->GetPlayer();
	}

	if (pPlayer != nullptr)
	{
		pos = pPlayer->GetPosition();
		move = pPlayer->GetMove();
	}
	else
	{
		return;
	}

	if (pos.x > LIMIT_LENGTH - LIMIT_WIDTH)
	{// EÌÇ
		pos.x = LIMIT_LENGTH - LIMIT_WIDTH;
	}

	if (pos.x < -LIMIT_LENGTH + LIMIT_WIDTH)
	{// ¶ÌÇ
		pos.x = -LIMIT_LENGTH + LIMIT_WIDTH;
	}

	if (pos.z > LIMIT_LENGTH - LIMIT_WIDTH)
	{// OÌÇ
		pos.z = LIMIT_LENGTH - LIMIT_WIDTH;
	}

	if (pos.z < -LIMIT_LENGTH + LIMIT_WIDTH)
	{// ãëÌÇ
		pos.z = -LIMIT_LENGTH + LIMIT_WIDTH;
	}

	if (pos.y > LIMIT_HEIGHT)
	{// Vä
		pos.y = LIMIT_HEIGHT;

		move.y = 0;
	}

	pPlayer->SetPosition(pos);
	pPlayer->SetMove(move);
}

//=====================================================
// |[YØèÖ¦
//=====================================================
void CGame::TogglePause(CPause *pPause)
{
	m_bPause = m_bPause ? false : true;

	if (m_bPause)
	{// |[Y¶¬
		if (m_pPause == nullptr)
		{
			m_pPause = CPause::Create();
		}
	}
	else
	{// |[Yjü
		if (m_pPause != nullptr)
		{
			m_pPause->Uninit();

			m_pPause = nullptr;
		}
	}
}

//=====================================================
// ÇÌÇ
//=====================================================
void CGame::ManageWall(void)
{
	D3DXVECTOR3 pos = { 0.0f,0.0f,0.0f };

	CPlayer *pPlayer = nullptr;
	CObjectManager *pObjManager = CManager::GetObjectManager();

	if (pObjManager != nullptr)
	{// vC[ÌKp
		pPlayer = pObjManager->GetPlayer();
	}

	if (pPlayer != nullptr)
	{
		pos = pPlayer->GetPosition();
	}

	if (pos.x > LIMIT_LENGTH - CHENGE_LENGTH)
	{// EÌÇÌ»è
		if (m_apMeshWall[WALLTYPE_RIGHT] != nullptr)
		{
			m_apMeshWall[WALLTYPE_RIGHT]->CheckPlayer();
		}
	}

	if (pos.x < -LIMIT_LENGTH + CHENGE_LENGTH)
	{// ¶ÌÇÌ»è
		if (m_apMeshWall[WALLTYPE_LEFT] != nullptr)
		{
			m_apMeshWall[WALLTYPE_LEFT]->CheckPlayer();
		}
	}

	if (pos.z < -LIMIT_LENGTH + CHENGE_LENGTH)
	{// OÌÇÌ»è
		if (m_apMeshWall[WALLTYPE_FRONT] != nullptr)
		{
			m_apMeshWall[WALLTYPE_FRONT]->CheckPlayer();
		}
	}

	if (pos.z > LIMIT_LENGTH - CHENGE_LENGTH)
	{// ãëÌÇÌ»è
		if (m_apMeshWall[WALLTYPE_REAR] != nullptr)
		{
			m_apMeshWall[WALLTYPE_REAR]->CheckPlayer();
		}
	}
}

//=====================================================
// óÔÇ
//=====================================================
void CGame::ManageState(void)
{
	// tF[hæ¾
	CFade *pFade = CManager::GetFade();

	CPlayer *pPlayer = nullptr;
	CMeshField *pMeshField = nullptr;
	CObjectManager *pObjManager = CManager::GetObjectManager();

	if (pObjManager != nullptr)
	{// vC[ÌKp
		pPlayer = pObjManager->GetPlayer();

		pMeshField = pObjManager->GetMeshField();
	}

	if (pFade != nullptr)
	{
		//pFade->SetFade(CScene::MODE_GAME);
	}

	int nTime = 0;

	switch (m_state)
	{
	case CGame::STATE_NORMAL:

		

		break;
	case CGame::STATE_RESULT:

		if (m_pTimer != nullptr)
		{
			nTime = m_pTimer->GetSecond();

			if (m_pTimer->GetSecond() <= 0)
			{// ^CAbvNA
			 // XRAÛ¶
				CManager::SetScore(m_pScore->GetScore());
			}
		}
		if (pPlayer == nullptr)
		{
			// XRAZbg
			CManager::SetScore(0);
		}

		break;
	case CGame::STATE_END:



		break;
	default:
		break;
	}
}

//=====================================================
// ^C{[iXÌÇ
//=====================================================
void CGame::TimeBonus(void)
{
	CAssess *pAssess;

	CPlayer *pPlayer = nullptr;
	CObjectManager *pObjManager = CManager::GetObjectManager();

	if (pObjManager != nullptr)
	{// vC[ÌKp
		pPlayer = pObjManager->GetPlayer();
	}

	int nBonus = BONUS_TIME - m_nTimerWave;

	if (pPlayer != nullptr)
	{
		pAssess = pPlayer->GetAssess();

		if (pAssess != nullptr)
		{
			pAssess->AddParam((float)nBonus * RATE_BONUS, CAssess::PARAM_ATTACK);
		}
	}
}

//=====================================================
// fobO
//=====================================================
void CGame::Debug(void)
{
	// îñæ¾
	CInputKeyboard *pKeyboard = CManager::GetKeyboard();
	CFade *pFade = CManager::GetFade();

	if (pKeyboard != nullptr)
	{
		if (pKeyboard->GetTrigger(DIK_Y))
		{
			CNoise::Create();
		}
	}
}

//=====================================================
// `æ
//=====================================================
void CGame::Draw(void)
{

}