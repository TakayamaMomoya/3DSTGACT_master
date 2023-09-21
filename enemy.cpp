//*****************************************************
//
// 敵の処理[enemy.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
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
// マクロ定義
//*****************************************************
#define SPEED_MOVE	(1.0f)	// 移動速度
#define DEFAULT_SIZE	(130.0f)		// 基準の大きさ
#define DEFAULT_DIST	(2000.0f)		// 基準の距離
#define PATH_LOCKON	("data\\TEXTURE\\UI\\lockon002.png")	// ロックオンマーカーのパス
#define ENEMY_DISTANCE	(6000)	// 敵が出現する距離
#define RATE_RADIUS	(1.5f)	// 当たり判定の大きさの倍率
#define INITIAL_LIFE	(1.0f)	// 初期体力
#define DAMAGE_FRAME	(10)	// ダメージ状態の継続フレーム数
#define TIME_DEATH	(100)	// 死亡までのフレーム数
#define AVOID_LENGTH	(3.0f)	// 追跡にうつるまでの距離
#define NUM_AIR_BONUS	(3)	// 空中でボーナスが入るまで撃墜数
#define AIR_SCORE	(300)	// 空中撃墜のボーナス
#define INITIAL_SCORE	(1000)	// 初期スコア
#define CLOSE_DISTANCE	(1000.0f)	// 近いと判断する距離
#define BONUS_TIME	(420)	// ボーナスが貰える基準のフレーム数

//*****************************************************
// 静的メンバ変数宣言
//*****************************************************
int CEnemy::m_nNumAll = 0;	// 総数
int CEnemy::m_nNumWave = 0;	// ウェーブ数
CEnemy::WAVEINFO *CEnemy::m_pWaveInfo = nullptr;	// 敵情報へのポインタ
CEnemy *CEnemy::m_pHead = nullptr;	// 先頭のアドレス
CEnemy *CEnemy::m_pTail = nullptr;	// 最後尾のアドレス

//=====================================================
// コンストラクタ
//=====================================================
CEnemy::CEnemy()
{
	// 総数カウントアップ
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

	// 値のクリア
	m_pPrev = nullptr;
	m_pNext = nullptr;

	if (m_pHead == nullptr)
	{// 先頭と最後尾アドレスの代入
		m_pHead = this;
		m_pTail = this;

		return;
	}

	// 前のアドレスに最後尾のアドレスを代入する
	m_pPrev = m_pTail;

	// 最後尾のアドレスを自分にする
	m_pTail = this;

	// 前のオブジェクトの次のアドレスを自分にする
	m_pPrev->m_pNext = this;
}

//=====================================================
// デストラクタ
//=====================================================
CEnemy::~CEnemy()
{
	if (m_state != STATE_DEATH)
	{
		m_nNumAll--;
	}

	if (m_pHead == this)
	{// 先頭アドレスの破棄
	 //if (m_pNext != nullptr)
		{// 先頭アドレスを次のアドレスに引き継ぐ
			m_pHead = m_pNext;

			if (m_pNext != nullptr)
			{
				m_pNext->m_pPrev = nullptr;
			}
		}
	}
	else if (m_pTail == this)
	{// 最後尾アドレスの破棄
		if (m_pPrev != nullptr)
		{// 最後尾アドレスを前のアドレスに引き継ぐ
			m_pTail = m_pPrev;

			m_pPrev->m_pNext = nullptr;
		}
	}
	else
	{// 真ん中のアドレスの破棄
	 // 前のアドレスから次のアドレスをつなぐ
		m_pPrev->m_pNext = m_pNext;

		// 次のアドレスから前のアドレスをつなぐ
		m_pNext->m_pPrev = m_pPrev;
	}
}

//=====================================================
// 生成処理
//=====================================================
CEnemy *CEnemy::Create(D3DXVECTOR3 pos, TYPE type)
{
	CEnemy *pEnemy = nullptr;

	if (pEnemy == nullptr)
	{// インスタンス生成
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

		// 影の生成
		if (pEnemy->m_pShadow == nullptr)
		{
			pEnemy->m_pShadow = CShadow::Create(D3DXVECTOR3(pEnemy->GetPosition().x, 1.0f, pEnemy->GetPosition().z), 150.0f, 150.0f);

			if (pEnemy->m_pShadow != nullptr)
			{
				pEnemy->m_pShadow->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
			}
		}

		pEnemy->SetPosition(D3DXVECTOR3(pos));

		// 初期化処理
		pEnemy->Init();
	}

	return pEnemy;
}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CEnemy::Init(void)
{
	// 変数宣言
	float fRadius = GetRadiusMax();

	// 継承クラスの初期化
	CMotion::Init();

	// タイプの設定
	SetType(TYPE_ENEMY);

	if (m_pCollisionSphere == nullptr)
	{// 球の当たり判定生成
		m_pCollisionSphere = CCollisionSphere::Create(CCollision::TAG_ENEMY, CCollision::TYPE_SPHERE,this);

		if (m_pCollisionSphere != nullptr)
		{// 情報の設定
			m_pCollisionSphere->SetPosition(GetPosition());
			m_pCollisionSphere->SetRadius(fRadius * RATE_RADIUS);
		}
	}

	if (m_pCollisionCube == nullptr)
	{// 立方体の当たり判定
		m_pCollisionCube = CCollisionCube::Create(CCollision::TAG_ENEMY, this);

		D3DXVECTOR3 vtxMax = { fRadius,fRadius,fRadius };
		D3DXVECTOR3 vtxMin = { -fRadius,-fRadius,-fRadius };

		if (m_pCollisionCube != nullptr)
		{
			m_pCollisionCube->SetVtx(vtxMax, vtxMin);
		}
	}

	// 体力の初期設定
	m_fLife = INITIAL_LIFE;

	// 通常状態にする
	m_moveState = MOVESTATE_CHASE;
	m_state = STATE_NORMAL;

	m_nScore = INITIAL_SCORE;

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CEnemy::Uninit(void)
{
	if (m_pCollisionSphere != nullptr)
	{// 当たり判定の消去
		m_pCollisionSphere->Uninit();

		m_pCollisionSphere = nullptr;
	}

	if (m_pShadow != nullptr)
	{// 影の削除
		m_pShadow->Uninit();

		m_pShadow = nullptr;
	}

	if (m_pExplSpawner != nullptr)
	{
		m_pExplSpawner->Uninit();
		m_pExplSpawner = nullptr;
	}

	// 継承クラスの終了
	CMotion::Uninit();
}

//=====================================================
// 更新処理
//=====================================================
void CEnemy::Update(void)
{
	if (CGame::GetState() != CGame::STATE_NORMAL)
	{
		return;
	}

	// 継承クラスの更新
	CMotion::Update();

	// 状態管理処理
	ManageState();

	// 攻撃カウンター加算
	m_nCntAttack++;

	if (m_nCntAttack == INT_MAX)
	{// 値の補正
		m_nCntAttack = 0;
	}

	// メッシュフィールドとの当たり判定
	D3DXVECTOR3 move = GetMove();
	float fHeight = 0.0f;
	CMeshField *pMesh = nullptr;
	CObjectManager *pObjManager = CManager::GetObjectManager();

	if (pObjManager != nullptr)
	{// メッシュフィールドの取得
		pMesh = pObjManager->GetMeshField();
	}

	if (pMesh != nullptr)
	{
		fHeight = pMesh->GetHeight(GetPosition(), &move);
	}

	// 位置の更新
	SetPosition(GetPosition() + GetMove());

	// 目標位置管理
	ManagePosDest();

	if (fHeight >= GetPosition().y)
	{// 位置設定
		SetPosition(D3DXVECTOR3(GetPosition().x, fHeight, GetPosition().z));

		SetMove(D3DXVECTOR3(move.x, 0.0f, move.z));
	}

	// 当たり判定管理
	ManageCollision();

	if (m_pShadow != nullptr)
	{
		// 影の位置設定
		m_pShadow->SetPosition(D3DXVECTOR3(GetPosition().x, m_pShadow->GetPosition().y, GetPosition().z));
	}
}

//=====================================================
// 当たり判定の管理
//=====================================================
void CEnemy::ManageCollision(void)
{
	if (m_pCollisionSphere != nullptr)
	{// 球の当たり判定の管理
		float fRadius = GetRadiusMax();

		m_pCollisionSphere->SetPositionOld(m_pCollisionSphere->GetPosition());

		m_pCollisionSphere->SetPosition(GetPosition());

		// プレイヤーが接近してるかの判定
		m_pCollisionSphere->SetRadius(CLOSE_DISTANCE);

		if (m_pCollisionSphere->SphereCollision(CCollision::TAG_PLAYER))
		{
			m_nCntClose++;
		}

		m_pCollisionSphere->SetRadius(fRadius * RATE_RADIUS);
	}

	if (m_pCollisionCube != nullptr)
	{// 立方体の当たり判定の管理
	 // パーツの最大半径取得
		float fRadius = GetRadiusMax();

		// 当たり判定の位置設定
		m_pCollisionCube->SetPosition(GetPosition());

		D3DXVECTOR3 vtxMax = { fRadius,fRadius,fRadius };
		D3DXVECTOR3 vtxMin = { -fRadius,10.0f,-fRadius };

		D3DXVECTOR3 move = GetMove();

		if (GetState() != STATE_DEATH)
		{
			// 押し出しの当たり判定
			m_pCollisionCube->CubeCollision(CCollision::TAG_BLOCK, &move);
		}
		else
		{
			// ブロックにぶつかったら爆発
			if (m_pCollisionCube->TriggerCube(CCollision::TAG_BLOCK))
			{
				Death();
			}
		}

		SetMove(move);
	}
}

//=====================================================
// ターゲット発見判定
//=====================================================
void CEnemy::SearchTarget(void)
{
	CPlayer *pPlayer = nullptr;
	CObjectManager *pObjManager = CManager::GetObjectManager();

	if (pObjManager != nullptr)
	{// プレイヤーの適用
		pPlayer = pObjManager->GetPlayer();
	}

	CMeshField *pMesh = nullptr;

	if (pObjManager != nullptr)
	{// メッシュフィールドの取得
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

	// 目標位置との差分取得
	vecDiff = posTarget - GetPosition();

	if (pMesh != nullptr)
	{
		// メッシュとの判定
		bHitMesh = pMesh->RayCheck(GetPosition(),vecDiff,GetPosition());
	}

	if (m_pCollisionSphere != nullptr)
	{// ブロックとの判定
		bHitBlock = m_pCollisionSphere->ChckObstclBtwn(this, vecDiff);
	}

	if (bHitBlock || bHitMesh)
	{// 発見
		m_bFind = true;
	}
	else
	{
		m_bFind = false;
	}

	//CManager::GetDebugProc()->Print("\n発見[%d]",m_bFind);
}

//=====================================================
// 状態管理
//=====================================================
void CEnemy::ManageState(void)
{
	switch (m_state)
	{
	case STATE_NORMAL:
		break;
	case STATE_DAMAGE:
		if (m_nTimerState > DAMAGE_FRAME)
		{// 通常状態に戻る
			m_nTimerState = 0;
			m_state = STATE_NORMAL;
		}
		else
		{// カウントアップ
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
// 目標位置管理
//=====================================================
void CEnemy::ManagePosDest(void)
{
	D3DXVECTOR3 vecDiff;

	// 障害物の回避
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
// 判定を避ける処理
//=====================================================
void CEnemy::AvoidCollisions(void)
{
	if (m_pCollisionCube == nullptr)
	{
		return;
	}

	// 計算用変数
	D3DXVECTOR3 vecCollision = { 0.0f,0.0f,0.0f };

	// ブロックのベクトルを取得
	vecCollision = m_pCollisionCube->CollisionVector(this);

	if (D3DXVec3Length(&vecCollision) == 0)
	{
		return;
	}

	m_posDest = GetPosition() + vecCollision;

	m_moveState = MOVESTATE_AVOID;
}

//=====================================================
// 体力設定
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
// ヒット処理
//=====================================================
void CEnemy::Hit(float fDamage)
{
	// スコア取得
	CScore *pScore = CGame::GetScore();

	if (m_state == STATE_DEATH)
	{// すぐに爆発
		Death();

		return;
	}

	// 体力減少
	SetLife(GetLife() - fDamage);

	if (GetLife() <= 0)
	{
		// 総数カウントダウン
		m_nNumAll--;

		ManageBonus();

		// 死亡状態にする
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
// ボーナスの管理処理
//=====================================================
void CEnemy::ManageBonus(void)
{
	CScore *pScore = CGame::GetScore();
	CPlayer *pPlayer = nullptr;
	CObjectManager *pObjManager = CManager::GetObjectManager();

	if (pObjManager != nullptr)
	{// プレイヤーの適用
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
	{// スコア加算
		pScore->AddScore(nScore);
	}
}

//=====================================================
// 死亡処理
//=====================================================
void CEnemy::Death(void)
{
	// 自分の爆発
	CParticle::Create(GetPosition(), CParticle::TYPE_EXPLOSION);

	CExplosion::Create(GetPosition())->SetSize(300.0f, 300.0f);

	CDebrisSpawner::Create(GetPosition(), 12.0f, 1, 12);

	// 自身の終了
	Uninit();
}

//=====================================================
// 描画処理
//=====================================================
void CEnemy::Draw(void)
{
	// マトリックスの設定
	SetMatrix();

	// 継承クラスの描画
	CMotion::Draw();
}

//=====================================================
// 攻撃カウンター設定
//=====================================================
void CEnemy::SetCntAttack(int nValue)
{
	m_nCntAttack = nValue;
}

//=====================================================
// 球の当たり判定の位置設定
//=====================================================
void CEnemy::SetSpherePosition(D3DXVECTOR3 pos)
{
	if (m_pCollisionSphere != nullptr)
	{// 位置設定
		m_pCollisionSphere->SetPosition(pos);
	}
}

//=====================================================
// 読込処理
//=====================================================
HRESULT CEnemy::Load(void)
{
	// 変数宣言
	char cTemp[256] = {};
	int nCntWave = 0;
	int nCntEnemy = 0;

	// ファイルから読み込む
	FILE *pFile = fopen("data\\enemywave.txt", "r");

	if (pFile != nullptr)
	{// ファイルが開けた場合
		while (true)
		{
			// 文字読み込み
			fscanf(pFile, "%s", &cTemp[0]);

			if (strcmp(cTemp, "NUM_WAVE") == 0)
			{// ウェーブ数取得
				// "="読み込み
				fscanf(pFile, "%s", &cTemp[0]);

				// ウェーブ数読み込み
				fscanf(pFile, "%d", &m_nNumWave);

				// ウェーブ情報生成
				if (m_pWaveInfo == nullptr)
				{
					m_pWaveInfo = new WAVEINFO[m_nNumWave];

					ZeroMemory(m_pWaveInfo, sizeof(WAVEINFO) * m_nNumWave);
				}
			}

			if (strcmp(cTemp, "WAVESET") == 0)
			{// 読込開始
				nCntEnemy = 0;

				while (true)
				{
					// 文字読み込み
					fscanf(pFile, "%s", &cTemp[0]);

					if (strcmp(cTemp, "NUM_ENEMY") == 0)
					{// 敵数取得
						// "="読み込み
						fscanf(pFile, "%s", &cTemp[0]);

						// 敵数読み込み
						fscanf(pFile, "%d", &m_pWaveInfo[nCntWave].m_nNumEnemy);

						if (m_pWaveInfo[nCntWave].pEnemyInfo == nullptr)
						{// 敵情報生成
							m_pWaveInfo[nCntWave].pEnemyInfo = new ENEMYINFO[m_pWaveInfo[nCntWave].m_nNumEnemy];
						}
					}

					if (strcmp(cTemp, "ENEMYSET") == 0)
					{// 敵情報取得
						while (true)
						{
							fscanf(pFile, "%s", &cTemp[0]);

							if (strcmp(cTemp, "TYPE") == 0)
							{// 種類取得
								// "="読み込み
								fscanf(pFile, "%s", &cTemp[0]);

								// 種類読み込み
								fscanf(pFile, "%d", &m_pWaveInfo[nCntWave].pEnemyInfo[nCntEnemy].type);
							}

							if (strcmp(cTemp, "POS") == 0)
							{// 位置取得
								// "="読み込み
								fscanf(pFile, "%s", &cTemp[0]);

								// 位置読み込み
								fscanf(pFile, "%f", &m_pWaveInfo[nCntWave].pEnemyInfo[nCntEnemy].pos.x);
								fscanf(pFile, "%f", &m_pWaveInfo[nCntWave].pEnemyInfo[nCntEnemy].pos.y);
								fscanf(pFile, "%f", &m_pWaveInfo[nCntWave].pEnemyInfo[nCntEnemy].pos.z);
							}

							if (strcmp(cTemp, "END_ENEMYSET") == 0)
							{// whileを抜ける
								break;
							}
						}

						nCntEnemy++;
					}

					if (strcmp(cTemp, "END_WAVESET") == 0)
					{// whileを抜ける
						break;
					}
				}

				nCntWave++;
			}

			if (strcmp(cTemp, "END_SCRIPT") == 0)
			{// 終了条件
				break;
			}
		}

		// ファイルを閉じる
		fclose(pFile);
	}
	else
	{
		return E_FAIL;
	}

	return S_OK;
}

//=====================================================
// 破棄処理
//=====================================================
void CEnemy::Unload(void)
{
	if (m_pWaveInfo != nullptr)
	{// ウェーブ情報の破棄
		for (int nCntWave = 0;nCntWave < m_nNumWave;nCntWave++)
		{
			if (m_pWaveInfo[nCntWave].pEnemyInfo != nullptr)
			{// 敵情報の破棄
				delete m_pWaveInfo[nCntWave].pEnemyInfo;
				m_pWaveInfo[nCntWave].pEnemyInfo = nullptr;
			}
		}

		delete m_pWaveInfo;
		m_pWaveInfo = nullptr;
	}
}

//=====================================================
// スポーン処理
//=====================================================
void CEnemy::SpawnEnemy(int nNumWave)
{
	// 護衛目標情報取得
	CDefend *pDefend = CGame::GetDefend();

	// 汎用処理の取得
	CUniversal *pUniversal = CManager::GetUniversal();

	// 変数宣言
	int nType = 0;
	D3DXVECTOR3 pos,rot = { 0.0f,0.0f,0.0f },vecDiff = { 0.0f,0.0f,0.0f };
	D3DXMATRIX mtxWorld,mtxTemp,mtxEnemy;
	D3DXMATRIX mtxRot, mtxTrans;
	CEnemy *pEnemy = nullptr;

	// マトリックス初期化
	D3DXMatrixIdentity(&mtxTemp);

	if (m_pWaveInfo != nullptr)
	{// 種類の設定
		nType = rand() % m_nNumWave;
	}

	for (int nCntWave = 0; nCntWave < nNumWave; nCntWave++)
	{
		pos.x = (float)(rand() % 2000 - 1000);
		pos.y = 0.0f;
		pos.z = (float)(rand() % 2000 - 1000);

		// 正規化
		D3DXVec3Normalize(&pos,&pos);

		// 距離をかける
		pos *= ENEMY_DISTANCE;

		if (pDefend != nullptr)
		{
			// 差分のベクトル算出
			vecDiff = pDefend->GetPosition() - pos;
		}

		// 角度算出
		rot.y = atan2f(vecDiff.x,vecDiff.z);

		// スポーン地点のマトリックス設定
		// マトリックス初期化
		D3DXMatrixIdentity(&mtxWorld);

		// 向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot,rot.y, rot.x, rot.z);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

		// マトリックスをかけ合わせる
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTemp);

		for (int nCntEnemy = 0;nCntEnemy < m_pWaveInfo[nType].m_nNumEnemy;nCntEnemy++)
		{
			pEnemy = nullptr;

			// スポーン地点と敵のマトリックスをかけ合わせる
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