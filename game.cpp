//*****************************************************
//
// ゲーム処理[game.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
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

//*****************************************************
// マクロ定義
//*****************************************************
#define TRANS_TIME	(60)	// 終了までの余韻のフレーム数
#define LIMIT_LENGTH	(10000)	// 移動制限の距離
#define LIMIT_WIDTH	(100)	// 推定プレイヤーの幅
#define LIMIT_HEIGHT	(2000)	// 制限高度
#define BONUS_TIME	(40)	// ボーナスが付与される最低限のタイム
#define RATE_BONUS	(0.015f)	// 1秒当たりのタイムボーナス

//*****************************************************
// 静的メンバ変数宣言
//*****************************************************
CPlayer *CGame::m_pPlayer = nullptr;		// プレイヤーのポインタ
CMeshField *CGame::m_pMeshField = nullptr;	// メッシュフィールドのポインタ
CScore *CGame::m_pScore = nullptr;	// スコアのポインタ
CTimer *CGame::m_pTimer = nullptr;	// タイマーのポインタ
CDefend *CGame::m_pDefend = nullptr;	// 護衛対象へのポインタ
CEdit *CGame::m_pEdit = nullptr;	// エディターへのポインタ
CGame::STATE CGame::m_state = STATE_NONE;
bool CGame::m_bPause = false;
CPause *CGame::m_pPause = nullptr;	// ポーズのアドレス

//=====================================================
// コンストラクタ
//=====================================================
CGame::CGame()
{
	ZeroMemory(&m_apMeshWall[0],sizeof(m_apMeshWall));
	m_bPhoto = false;
	m_bPause = false;
	m_nTimerWave = 0;
	m_nCntSecond = 0;
}

//=====================================================
// デストラクタ
//=====================================================
CGame::~CGame()
{

}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CGame::Init(void)
{
	if (m_pPlayer == nullptr)
	{// プレイヤー生成
		m_pPlayer = CPlayer::Create();
	}

	// スコア読込処理
	CScore::Load();

	if (m_pScore == nullptr)
	{// スコア生成
		m_pScore = CScore::Create();
	}

	if (m_pScore != nullptr)
	{// スコア初期化
		m_pScore->Init();
	}

	if (m_pTimer == nullptr)
	{// タイマー生成
		m_pTimer = CTimer::Create();
	}

	if (m_pTimer != nullptr)
	{// タイマー初期化
		m_pTimer->Init();
	}

	// 敵情報読込
	CEnemy::Load();

	// ブロック番号読込
	CBlock::LoadModel();

#if _DEBUG
	if (m_pEdit == nullptr)
	{// エディター生成
		m_pEdit = new CEdit;
	}

	if (m_pEdit != nullptr)
	{// エディター初期化
		m_pEdit->Init();
	}
#endif

	if (m_pDefend == nullptr)
	{// 護衛対象生成
		m_pDefend = CDefend::Create();

		if (m_pDefend != nullptr)
		{
			// 初期化
			if (FAILED(m_pDefend->Init()))
			{// 初期化に失敗した場合
				return E_FAIL;
			}
		}
	}

	// 弾読込処理
	CBullet::Load();

	// 爆発読込処理
	CExplosion::Load();

	// ブロック読込処理
	CBlock::Load();

	// パーティクルの読込
	CParticle::Load();

	// スカイボックス生成
	CSkybox::Create();

	// メッシュフィールドの生成
	m_pMeshField = CMeshField::Create();

	// ハリボテ床の生成
	CField *pField = nullptr;
	pField = CField::Create();

	if (pField != nullptr)
	{
		pField->SetPosition(D3DXVECTOR3(0.0f, -300.0f, 0.0f));
		pField->SetSize(5000000.0f, 5000000.0f);
	}

	// メッシュウォールの生成
	m_apMeshWall[WALLTYPE_FRONT] = CMeshWall::Create(D3DXVECTOR3(0.0f, 0.0f, -10000.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f));
	m_apMeshWall[WALLTYPE_REAR] = CMeshWall::Create(D3DXVECTOR3(0.0f, 0.0f, 10000.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_apMeshWall[WALLTYPE_RIGHT] = CMeshWall::Create(D3DXVECTOR3(10000.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f));
	m_apMeshWall[WALLTYPE_LEFT] = CMeshWall::Create(D3DXVECTOR3(-10000.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, -D3DX_PI * 0.5f, 0.0f));

	m_state = STATE_NORMAL;

	// フォグをつける
	CRenderer::EnableFog(true);

	// 塵管理の生成
	CDustManager::Create();

	// 敵生成
	CEnemy::SpawnEnemy(3);

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CGame::Uninit(void)
{
	if (m_pPause != nullptr)
	{
		m_pPause = nullptr;
	}

	if (m_pPlayer != nullptr)
	{// プレイヤーの終了・破棄
		m_pPlayer = nullptr;
	}

	if (m_pDefend != nullptr)
	{// 護衛対象の終了・破棄
		m_pDefend->Uninit();

		delete m_pDefend;
		m_pDefend = nullptr;
	}

	if (m_pMeshField != nullptr)
	{
		m_pMeshField->Uninit();

		m_pMeshField = nullptr;
	}

	if (m_pScore != nullptr)
	{// スコアの終了・破棄
		m_pScore->Uninit();

		m_pScore = nullptr;
	}

	if (m_pTimer != nullptr)
	{// タイマーの終了・破棄
		m_pTimer->Uninit();

		m_pTimer = nullptr;
	}

#if _DEBUG
	if (m_pEdit != nullptr)
	{// エディターの終了・破棄
		m_pEdit->Uninit();

		delete m_pEdit;
		m_pEdit = nullptr;
	}
#endif

	// ブロック情報削除
	CBlock::DeleteAll();

	// ブロック番号削除
	CBlock::DeleteIdx();

	// 弾破棄
	CBullet::Unload();

	// プレイヤー破棄
	CPlayer::Unload();

	// 爆発破棄
	CExplosion::Unload();

	// 敵破棄
	CEnemy::Unload();

	// スコア破棄処理
	CScore::Unload();

	// オブジェクト全破棄
	CObject::ReleaseAll();
}

//=====================================================
// プレイヤー破棄処理
//=====================================================
void CGame::ReleasePlayer(void)
{
	if (m_pPlayer != nullptr)
	{
		m_pPlayer = nullptr;
	}
}

//=====================================================
// スコア終了処理
//=====================================================
void CGame::ReleaseScore(void)
{
	if (m_pScore != nullptr)
	{// スコアの終了・破棄
		m_pScore = nullptr;
	}
}

//=====================================================
// タイマー終了処理
//=====================================================
void CGame::ReleaseTimer(void)
{
	if (m_pTimer != nullptr)
	{// スコアの終了・破棄
		m_pTimer = nullptr;
	}
}

//=====================================================
// 護衛対象終了処理
//=====================================================
void CGame::ReleaseDefend(void)
{
	// フェード取得
	CFade *pFade = CManager::GetFade();

	if (m_pDefend != nullptr)
	{// 護衛対象の終了・破棄
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
// 更新処理
//=====================================================
void CGame::Update(void)
{
	// 入力取得
	CInputKeyboard *pKeyboard = CManager::GetKeyboard();
	CInputJoypad *pJoypad = CManager::GetJoypad();

	CCamera *pCamera = CManager::GetCamera();

	if (m_bPause == false)
	{
		if (m_bPhoto == false)
		{
			// シーンの更新
			CScene::Update();
		}

		// カメラ更新
		if (pCamera != nullptr)
		{
			if (CGame::IsPhoto())
			{
				pCamera->Control();
			}
			else
			{
				// 追従処理
				pCamera->FollowPlayer();
			}

			// カメラ揺れの処理
			pCamera->Quake();
		}
	}

	m_nCntSecond++;

	if (m_nCntSecond >= 60)
	{
		m_nTimerWave++;

		m_nCntSecond = 0;
	}

	if (CEnemy::GetNumAll() == 0)
	{// 敵スポーン
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
		{// フォトモード起動
			m_bPhoto = m_bPhoto ? false : true;
		}
	}
#endif

	// 状態管理
	ManageState();

	// プレイヤー移動制限処理
	LimitPlayerPos();

	// 壁の管理
	ManageWall();

	if (pKeyboard != nullptr)
	{
		if (pKeyboard->GetTrigger(DIK_P) || pJoypad->GetTrigger(CInputJoypad::PADBUTTONS_START, 0))
		{// ポーズ切り替え
			TogglePause(m_pPause);
		}
	}

	if (m_pPause != nullptr)
	{// ポーズ更新
		m_pPause->Update();
	}

#ifdef _DEBUG	// デバッグ処理
	Debug();
#endif
}

//=====================================================
// プレイヤー位置制限処理
//=====================================================
void CGame::LimitPlayerPos(void)
{
	D3DXVECTOR3 pos = { 0.0f,0.0f,0.0f };
	D3DXVECTOR3 move = { 0.0f,0.0f,0.0f };

	if (m_pPlayer != nullptr)
	{
		pos = m_pPlayer->GetPosition();
		move = m_pPlayer->GetMove();
	}
	else
	{
		return;
	}

	if (pos.x > LIMIT_LENGTH - LIMIT_WIDTH)
	{// 右の壁
		pos.x = LIMIT_LENGTH - LIMIT_WIDTH;
	}

	if (pos.x < -LIMIT_LENGTH + LIMIT_WIDTH)
	{// 左の壁
		pos.x = -LIMIT_LENGTH + LIMIT_WIDTH;
	}

	if (pos.z > LIMIT_LENGTH - LIMIT_WIDTH)
	{// 前の壁
		pos.z = LIMIT_LENGTH - LIMIT_WIDTH;
	}

	if (pos.z < -LIMIT_LENGTH + LIMIT_WIDTH)
	{// 後ろの壁
		pos.z = -LIMIT_LENGTH + LIMIT_WIDTH;
	}

	if (pos.y > LIMIT_HEIGHT)
	{// 天井
		pos.y = LIMIT_HEIGHT;

		move.y = 0;
	}

	m_pPlayer->SetPosition(pos);
	m_pPlayer->SetMove(move);
}

//=====================================================
// ポーズ切り替え処理
//=====================================================
void CGame::TogglePause(CPause *pPause)
{
	m_bPause = m_bPause ? false : true;

	if (m_bPause)
	{// ポーズ生成
		if (m_pPause == nullptr)
		{
			m_pPause = CPause::Create();
		}
	}
	else
	{// ポーズ破棄
		if (m_pPause != nullptr)
		{
			m_pPause->Uninit();

			m_pPause = nullptr;
		}
	}
}

//=====================================================
// 壁の管理
//=====================================================
void CGame::ManageWall(void)
{
	D3DXVECTOR3 pos = { 0.0f,0.0f,0.0f };

	if (m_pPlayer != nullptr)
	{
		pos = m_pPlayer->GetPosition();
	}

	if (pos.x > LIMIT_LENGTH - CHENGE_LENGTH)
	{// 右の壁の判定
		if (m_apMeshWall[WALLTYPE_RIGHT] != nullptr)
		{
			m_apMeshWall[WALLTYPE_RIGHT]->CheckPlayer();
		}
	}

	if (pos.x < -LIMIT_LENGTH + CHENGE_LENGTH)
	{// 左の壁の判定
		if (m_apMeshWall[WALLTYPE_LEFT] != nullptr)
		{
			m_apMeshWall[WALLTYPE_LEFT]->CheckPlayer();
		}
	}

	if (pos.z < -LIMIT_LENGTH + CHENGE_LENGTH)
	{// 前の壁の判定
		if (m_apMeshWall[WALLTYPE_FRONT] != nullptr)
		{
			m_apMeshWall[WALLTYPE_FRONT]->CheckPlayer();
		}
	}

	if (pos.z > LIMIT_LENGTH - CHENGE_LENGTH)
	{// 後ろの壁の判定
		if (m_apMeshWall[WALLTYPE_REAR] != nullptr)
		{
			m_apMeshWall[WALLTYPE_REAR]->CheckPlayer();
		}
	}
}

//=====================================================
// 状態管理
//=====================================================
void CGame::ManageState(void)
{
	// フェード取得
	CFade *pFade = CManager::GetFade();

	int nTime = 0;

	switch (m_state)
	{
	case CGame::STATE_NORMAL:

		if (m_pTimer != nullptr)
		{
			if (m_pTimer->GetSecond() <= 0)
			{// タイムアップクリア
				// スコア保存
				CManager::SetScore(m_pScore->GetScore());

				m_state = STATE_END;
			}
		}

		if (m_pPlayer == nullptr)
		{
			// スコアリセット
			CManager::SetScore(0);

			m_state = STATE_END;
		}

		break;
	case CGame::STATE_END:

		// 遷移タイマーカウント
		nTime = CScene::GetTimer() + 1;

		SetTimer(nTime);

		if (nTime >= TRANS_TIME)
		{
			m_state = STATE_NONE;

			if (pFade != nullptr)
			{
				// 画面遷移
				pFade->SetFade(CScene::MODE_RANKING);
			}
		}

		break;
	default:
		break;
	}
}

//=====================================================
// タイムボーナスの管理
//=====================================================
void CGame::TimeBonus(void)
{
	CAssess *pAssess;

	int nBonus = BONUS_TIME - m_nTimerWave;

	if (m_pPlayer != nullptr)
	{
		pAssess = m_pPlayer->GetAssess();

		if (pAssess != nullptr)
		{
			pAssess->AddAttack((float)nBonus * RATE_BONUS);
		}
	}
}

//=====================================================
// デバッグ処理
//=====================================================
void CGame::Debug(void)
{
	// 情報取得
	CInputKeyboard *pKeyboard = CManager::GetKeyboard();
	CFade *pFade = CManager::GetFade();

	if (pKeyboard != nullptr)
	{
		if (pKeyboard->GetTrigger(DIK_M))
		{// 防衛目標破壊
			if (m_pDefend != nullptr)
			{
				m_pDefend->Hit(100000);
			}
		}

		if (pKeyboard->GetTrigger(DIK_RETURN))
		{// 遷移
			if (pFade != nullptr)
			{
				//pFade->SetFade(MODE_RANKING);
			}
		}
	}
}

//=====================================================
// 描画処理
//=====================================================
void CGame::Draw(void)
{

}