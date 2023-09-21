//*****************************************************
//
// プレイヤーの処理[player.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "main.h"
#include "player.h"
#include "renderer.h"
#include "manager.h"
#include "inputkeyboard.h"
#include "inputmouse.h"
#include "inputjoypad.h"
#include "bullet.h"
#include "enemy.h"
#include "debugproc.h"
#include "particle.h"
#include "sound.h"
#include "collision.h"
#include "model.h"
#include "texture.h"
#include "shadow.h"
#include "camera.h"
#include "collision.h"
#include "meshfield.h"
#include "universal.h"
#include "effect3D.h"
#include "boost.h"
#include "orbit.h"
#include "game.h"
#include "cockpit.h"
#include "rader.h"
#include "score.h"
#include <stdio.h>
#include "bonus.h"
#include "assess.h"
#include "lockon.h"
#include "texture.h"
#include "noise.h"
#include "explosionspawner.h"
#include "smokespawner.h"
#include "debrisspawner.h"
#include "objectmanager.h"
#include "result.h"

//*****************************************************
// マクロ定義
//*****************************************************
#define SPEED_MOVE	(2.0f)	// 移動速度
#define RAPID_SPEED	(35.0f)	// 連射速度
#define RAPID_MAX	(RAPID_SPEED * 0.8f)	// 連射最大速度の倍率
#define SHOT_COST	(50.0f)	// 通常射撃のブーストコスト
#define COST_MAX	(SHOT_COST * 0.8f)	// コスト最大効率の倍率
#define CHARGE_COST	(SHOT_COST * 1.5f)	// チャージショットのコスト
#define CHARGE_DAMAGE	(10.0f)	// チャージショットの威力
#define NORMAL_DAMAGE	(5.0f)	// 通常ショットの威力
#define ROLL_SPEED	(0.05f)	// 視点操作速度
#define RAPID_TIME	(10)	// 連射モードにうつる猶予
#define SPEED_SPRINT	(0.4f)	// ダッシュ時の移動倍率
#define GRAVITY	(0.98f)	// 重力
#define JUMP_POW	(1.2f)	// ジャンプ力
#define JUMP_START_POW	(5.0f)	// 初動のジャンプ力
#define COST_JUMP	(1.5f)	// ジャンプのコスト
#define RGN_BOOST	(1.5f)	// ブーストの回復量
#define COST_SPRINT	(1.5f)	// ダッシュのコスト
#define BULLET_SPEED	(90.0f)	// 弾の速度
#define BULLET_LIFE		(120)	// 弾の寿命
#define AIM_LENGTH	(370.0f)	// エイム時のカメラの距離
#define NOT_AIM_LENGTH	(700.0f)	// 非エイム時のカメラの距離
#define TURN_DOWNER	(D3DX_PI * 0.55f)	// 下半身が反転するまでの差分
#define TURN_ROT	(D3DX_PI * 0.45f)	// 下半身が反転するまでの差分
#define LOCKON_ANGLE	(D3DX_PI * 0.13f)	// ロックオンの上下角度
#define LOCKON_RANGE	(3000.0f)	// ロックオン距離
#define AIM_LOCKON_RANGE	(LOCKON_RANGE * 1.3f)	// エイム時のロックオン距離
#define CHARGE_TIMER	(40.0f)	// チャージショットを撃てるまでのカウンタ
#define BULLET_RADIUS	(30.0f)	// 通常弾の大きさ
#define CHARGE_RADIUS	(300.0f)	// チャージショットの基準の大きさ
#define MOVE_FACT	(0.8f)	// 通常の移動慣性
#define SPRINT_FACT	(0.96f)	// ダッシュの移動慣性
#define RADIUS_SPHERE	(100)	// 球の当たり判定の大きさ
#define HIT_ADD	(0.025f)	// 命中率の表示
#define DAMAGE_PENALTY	(0.05f)	// ダメージ時の評価ペナルティ
#define LIMIT_UPPERROT	(0.5f)	// 上半身の角度制限
#define LIMIT_CAMERAROT	(0.8f)	// カメラの角度制限
#define TIME_DEATH	(240)	// 死ぬまでのフレーム数

//=====================================================
// 優先順位を決めるコンストラクタ
//=====================================================
CPlayer::CPlayer(int nPriority)
{
	m_nLife = 0;
	m_nCntDeath = 0;
	m_fBoost = 0.0f;
	m_nTimerRapid = 0;
	m_nTimerShot = 0;
	m_nNumShot = 0;
	m_nNumHit = 0;
	m_posLockon = { 0.0f,0.0f,0.0f };
	m_posBoost = { 0.0f,0.0f,0.0f };
	m_vecBoost = { 0.0f,0.0f,0.0f };
	m_pos = { 0.0f,0.0f,0.0f };
	m_posOld = { 0.0f,0.0f,0.0f };
	m_rot = { 0.0f,0.0f,0.0f };
	m_rotDest = { 0.0f,0.0f,0.0f };
	m_rotUpper= { 0.0f,0.0f,0.0f };
	m_rotDowner = { 0.0f,0.0f,0.0f };
	m_move = { 0.0f,0.0f,0.0f };
	m_fParamCost = 0.0f;
	m_fParamPower = 0.0f;
	m_fParamRapid = 0.0f;
	m_state = STATE_NONE;
	m_bAim = false;
	m_bLand = false;
	m_bSprint = false;
	m_bTurnDowner = false;
	m_pShadow = nullptr;
	m_pCollisionSphere = nullptr;
	m_pCollisionCube = nullptr;
	m_pBodyDowner = nullptr;
	m_pBodyUpper = nullptr;
	m_pEnemy = nullptr;
	m_pCockpit = nullptr;
	m_pRader = nullptr;
	m_pAssess = nullptr;
	m_pLockon = nullptr;
	m_nCounterAir = 0;
	ZeroMemory(&m_mtxMazzle[0], sizeof(m_mtxMazzle));
	ZeroMemory(&m_offsetMazzle[0], sizeof(m_offsetMazzle));
	ZeroMemory(&m_mtxLockon[0], sizeof(m_mtxLockon));
	ZeroMemory(&m_offsetLockon[0], sizeof(m_offsetLockon));
	ZeroMemory(&m_mtxWaist,sizeof(m_mtxWaist));
}

//=====================================================
// デストラクタ
//=====================================================
CPlayer::~CPlayer()
{

}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CPlayer::Init(void)
{
	// カーソルを非表示にする
	ShowCursor(false);

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// 値の初期化
	m_fBoost = MAX_BOOST;
	SetPosition(D3DXVECTOR3(-500.0f, 1.0f, 1.0f));
	m_move = { 1.0f,2.0f,1.0f };

	// 影の生成
	if (m_pShadow == nullptr)
	{
		m_pShadow = CShadow::Create(D3DXVECTOR3(GetPosition().x, 1.0f, GetPosition().z), 150.0f, 150.0f);

		if (m_pShadow != nullptr)
		{
			m_pShadow->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
		}
	}

	if (m_pBodyDowner == nullptr)
	{// 下半身の生成
		m_pBodyDowner = CMotion::Create("data\\MOTION\\motionArms01Down.txt");
	}

	if (m_pBodyUpper == nullptr)
	{// 上半身の生成
		m_pBodyUpper = CMotion::Create("data\\MOTION\\motionArms01Up.txt");
	}
	
	// タイプの設定
	SetType(TYPE_PLAYER);

	if (m_pCollisionSphere == nullptr)
	{// 球の当たり判定生成
		m_pCollisionSphere = CCollisionSphere::Create(CCollision::TAG_PLAYER, CCollision::TYPE_SPHERE,this);

		float fRadius;

		if (m_pCollisionSphere != nullptr)
		{// 情報の設定
			fRadius = RADIUS_SPHERE;

			m_pCollisionSphere->SetPosition(GetPosition());
			m_pCollisionSphere->SetRadius(fRadius);
		}
	}

	if (m_pCollisionCube == nullptr)
	{// 立方体の当たり判定
		m_pCollisionCube = CCollisionCube::Create(CCollision::TAG_PLAYER,this);

		D3DXVECTOR3 vtxMax = { 100.0f,300.0f,100.0f };
		D3DXVECTOR3 vtxMin = { -100.0f,0.0f,-100.0f };

		if (m_pCollisionCube != nullptr)
		{
			m_pCollisionCube->SetVtx(vtxMax, vtxMin);
		}
	}

	// 銃口のオフセット設定
	m_offsetMazzle[0] = { 0.0f,-150.0f,-30.0f };
	m_offsetMazzle[1] = { 0.0f, 1.0f, 0.0f};

	// ロックオンのオフセット設定
	m_offsetLockon[0] = { 100.0f,0.0f,-200.0f };
	m_offsetLockon[1] = { -100.0f,0.0f,-200.0f };

	m_boostState = BOOSTSTATE_NONE;
	m_state = STATE_NORMAL;

	// ゲージの生成
	CBoost::Create();

	// 評価パラメーターの生成
	m_pAssess = CAssess::Create();

	// コックピットの生成
	m_pCockpit = CCockPit::Create();

	// レーダーの生成
	m_pRader = CRader::Create();

	// ロックオン生成
	m_pLockon = CLockon::Create();

	// パラメーター読込
	Load();

	m_nLife = INITIAL_LIFE_PLAYER;

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CPlayer::Uninit(void)
{
	if (m_pCollisionSphere != nullptr)
	{// 当たり判定の消去
		m_pCollisionSphere->Uninit();

		m_pCollisionSphere = nullptr;
	}

	if (m_pCollisionCube != nullptr)
	{// 当たり判定の消去
		m_pCollisionCube->Uninit();

		m_pCollisionCube = nullptr;
	}

	if (m_pShadow != nullptr)
	{// 影の削除
		m_pShadow->Uninit();

		m_pShadow = nullptr;
	}

	if (m_pBodyUpper != nullptr)
	{// 上半身の終了
		m_pBodyUpper->Uninit();

		m_pBodyUpper = nullptr;
	}

	if (m_pBodyDowner != nullptr)
	{// 下半身の終了
		m_pBodyDowner->Uninit();

		m_pBodyDowner = nullptr;
	}

	if (m_pBodyDowner != nullptr)
	{// 下半身の終了
		m_pBodyDowner->Uninit();

		m_pBodyDowner = nullptr;
	}

	if (m_pCockpit != nullptr)
	{// コックピットの終了
		m_pCockpit->Uninit();

		m_pCockpit = nullptr;
	}

	if (m_pRader != nullptr)
	{// レーダーの終了
		m_pRader->Uninit();

		m_pRader = nullptr;
	}

	if (m_pLockon != nullptr)
	{// ロックオンの終了
		m_pLockon->Uninit();

		m_pLockon = nullptr;
	}

	if (m_pAssess != nullptr)
	{// 評価の終了
		m_pAssess->Uninit();

		m_pAssess = nullptr;
	}
	
	// プレイヤーの終了
	CObjectManager *pObjManager = CManager::GetObjectManager();

	if (pObjManager != nullptr)
	{// プレイヤーの適用
		pObjManager->ReleasePlayer();
	}

	// 自身の破棄
	Release();
}

//=====================================================
// 更新処理
//=====================================================
void CPlayer::Update(void)
{
	// 変数宣言
	float fMoveFact = MOVE_FACT;

	// カメラ取得
	CCamera *pCamera = CManager::GetCamera();

	// マウスカーソルを画面中心に固定する
	SetCursorPos((int)(SCREEN_WIDTH * 0.5f), (int)(SCREEN_HEIGHT * 0.5f));

	// ロックオン処理
	Lockon();

	if (m_bSprint == false)
	{
		// 重力による落下
		SetMove(D3DXVECTOR3(GetMove().x, GetMove().y - GRAVITY, GetMove().z));
	}
	else
	{
		// 落下の無効化
		SetMove(D3DXVECTOR3(GetMove().x, GetMove().y + (0 - GetMove().y) * 0.2f, GetMove().z));

		// ブーストの設定
		Boost();
	}

	// オフセット管理
	ManageOffset();

	// 状態管理
	ManageState();

	// 位置を保存
	SetPositionOld(GetPosition());

	// ブースト量を保存
	float fBoost = m_fBoost;

	if (m_state != STATE_DEATH)
	{// 死亡時は反映しない処理
		// 操作処理
		Input();

		// モーションの管理
		ManageMotion();
	}

	// ブースト管理
	ManageBoost(fBoost);

	if (m_bSprint || m_bLand == false)
	{
		fMoveFact = SPRINT_FACT;
	}

	// 位置加算
	SetPosition(GetPosition() + GetMove());

	// 移動量減衰
	SetMove(D3DXVECTOR3(GetMove().x * fMoveFact, GetMove().y, GetMove().z * fMoveFact));

	// 当たり判定管理
	ManageCollision();
}

//=====================================================
// ブースト管理
//=====================================================
void CPlayer::ManageBoost(float fBoostOld)
{
	if (m_fBoost == fBoostOld)
	{// ブーストが減らなかったら回復
		m_fBoost += RGN_BOOST;

		if (m_fBoost > MAX_BOOST)
		{
			m_fBoost = MAX_BOOST;
		}
	}

	if (m_fBoost < 0)
	{// ブーストの欠乏
		m_fBoost = 0.0f;

		m_boostState = BOOSTSTATE_EMPTY;

		m_bSprint = false;
	}

	if (m_boostState == BOOSTSTATE_EMPTY)
	{// ブースト欠乏からの復帰
		if (m_fBoost > MAX_BOOST * 0.5f)
		{
			m_boostState = BOOSTSTATE_NONE;
		}
	}
}

//=====================================================
// オフセットの管理
//=====================================================
void CPlayer::ManageOffset(void)
{
	// 汎用処理の取得
	CUniversal *pUniversal = CManager::GetUniversal();

	// 銃口のオフセット位置を設定
	CParts *pParts = nullptr;

	if (m_pBodyUpper != nullptr)
	{
		pParts = m_pBodyUpper->GetParts(8)->m_pParts;
	}

	if (pParts != nullptr)
	{
		pUniversal->SetOffSet(&m_mtxMazzle[0], *pParts->GetMatrix(), m_offsetMazzle[0]);
		pUniversal->SetOffSet(&m_mtxMazzle[1], m_mtxMazzle[0], m_offsetMazzle[1]);

		m_posMazzle = { m_mtxMazzle[0]._41, m_mtxMazzle[0]._42, m_mtxMazzle[0]._43 };
	}

	// ロックオンのオフセット設定
	pUniversal->SetOffSet(&m_mtxLockon[0], *m_pBodyUpper->GetMatrix(), m_offsetLockon[0]);
	pUniversal->SetOffSet(&m_mtxLockon[1], *m_pBodyUpper->GetMatrix(), m_offsetLockon[1]);
}

//=====================================================
// 当たり判定管理
//=====================================================
void CPlayer::ManageCollision(void)
{
	bool bLandMesh;
	bool bLandBlock = false;

	// スコア取得
	CScore *pScore = CGame::GetScore();

	// メッシュフィールドとの判定
	bLandMesh = CollisionField();

	if (m_pCollisionCube != nullptr)
	{// 当たり判定の管理

		m_pCollisionCube->SetPosition(GetPosition());

		D3DXVECTOR3 vtxMax = { 100.0f,300.0f,100.0f };
		D3DXVECTOR3 vtxMin = { -100.0f,0.0f,-100.0f };

		m_pCollisionCube->SetVtx(vtxMax, vtxMin);

		D3DXVECTOR3 move = GetMove();

		// 押し出しの当たり判定
		bLandBlock = m_pCollisionCube->CubeCollision(CCollision::TAG_BLOCK, &move);

		SetMove(move);
	}

	if (m_pCollisionSphere != nullptr)
	{// 球の当たり判定の管理
		// 腰の位置に設定
		D3DXVECTOR3 pos = { m_mtxWaist._41,m_mtxWaist._42 ,m_mtxWaist._43 };

		m_pCollisionSphere->SetPositionOld(m_pCollisionSphere->GetPosition());

		m_pCollisionSphere->SetPosition(pos);

		m_pCollisionSphere->SetRadius(RADIUS_SPHERE * 4);

		if (m_pCollisionSphere->IsTriggerExit(CCollision::TAG_ENEMYBULLET))
		{// 至近弾回避の判定
			CBonus::Create(CBonus::TYPE_DOGDE);
		}

		if (m_pCollisionSphere->IsTriggerExit(CCollision::TAG_MISSILE))
		{// 至近弾回避の判定
			CBonus::Create(CBonus::TYPE_DOGDE);
		}

		m_pCollisionSphere->SetRadius(RADIUS_SPHERE);
	}

	if (bLandBlock || bLandMesh)
	{// 着地条件をひとつクリアしていれば着地状態にする
		m_bLand = true;

		m_nCounterAir = 0;
	}
	else
	{
		m_bLand = false;
	}
}

//=====================================================
// メッシュフィールドとの判定
//=====================================================
bool CPlayer::CollisionField(void)
{
	bool bLandMesh = false;

	// メッシュフィールドとの当たり判定
	D3DXVECTOR3 move = GetMove();
	D3DXVECTOR3 nor;
	float fHeight = 0.0f;

	CMeshField *pMeshFiled = nullptr;
	CObjectManager *pObjManager = CManager::GetObjectManager();

	if (pObjManager != nullptr)
	{// メッシュフィールド取得
		pMeshFiled = pObjManager->GetMeshField();
	}

	if (m_pShadow != nullptr)
	{
		// 影の位置設定
		m_pShadow->SetPosition(D3DXVECTOR3(GetPosition().x, m_pShadow->GetPosition().y, GetPosition().z));
	}

	if (pMeshFiled != nullptr)
	{// メッシュとの当たり判定
		fHeight = pMeshFiled->GetHeight(GetPosition(), &move, nullptr);

		if (fHeight >= GetPosition().y)
		{// プレイヤーの位置設定
			SetPosition(D3DXVECTOR3(GetPosition().x, fHeight, GetPosition().z));

			SetMove(move);

			move = GetMove();

			if (move.y <= 0.0f)
			{
				SetMove(D3DXVECTOR3(move.x, 0.0f, move.z));
			}

			// 着地状態にする
			bLandMesh = true;
		}
	}

	return bLandMesh;
}

//=====================================================
// 操作処理
//=====================================================
void CPlayer::Input(void)
{
	if (CGame::GetState() != STATE_NORMAL)
	{
		return;
	}

	// 入力情報入手
	CInputKeyboard *pKeyboard = CManager::GetKeyboard();

	// 移動操作
	InputMove();

	// 射撃操作
	InputShot();

	// カメラ操作
	InputCamera();

#ifdef _DEBUG
	if (pKeyboard->GetPress(DIK_B))
	{// ボーナス表示
		CBonus::Create(CBonus::TYPE_DOGDE);
		CBonus::Create(CBonus::TYPE_DOGDE);
	}

	if (pKeyboard->GetTrigger(DIK_B))
	{// ボーナス表示
		CBonus::Create(CBonus::TYPE_MISSILE);
		CBonus::Create(CBonus::TYPE_MISSILE);
	}

	if (pKeyboard->GetTrigger(DIK_F4))
	{// パラメーターカンスト
		m_fParamCost = 1.0f;
		m_fParamPower = 1.0f;
		m_fParamRapid = 1.0f;
	}

	if (pKeyboard->GetTrigger(DIK_F5))
	{// 死亡
		Hit(100000);
	}

	if (pKeyboard->GetTrigger(DIK_F6))
	{// 敵スポーン
		CEnemy::Create(GetPosition(), CEnemy::TYPE_MOVE);
	}

	if (pKeyboard->GetTrigger(DIK_B))
	{// ボーナス表示
		CBonus::Create(CBonus::TYPE_DOGDE);
	}
#endif
}

//=====================================================
// 移動操作
//=====================================================
void CPlayer::InputMove(void)
{
	// 情報入手
	CInputKeyboard *pKeyboard = CManager::GetKeyboard();
	CInputMouse *pMouse = CManager::GetMouse();
	CInputJoypad *pJoypad = CManager::GetJoypad();
	CCamera *pCamera = CManager::GetCamera();
	CCamera::Camera *pCameraInfo = CManager::GetCamera()->GetCamera();

	// 変数宣言
	D3DXVECTOR3 move = { 0.0f,0.0f,0.0f }, rot = { 0.0f,0.0f,0.0f };
	D3DXVECTOR3 vecStick;
	float fRot;

	if (pKeyboard->GetPress(DIK_A))
	{// 左移動
		move.x += sinf(pCameraInfo->rot.y + D3DX_PI * 0.5f) * SPEED_MOVE;
		move.z += cosf(pCameraInfo->rot.y + D3DX_PI * 0.5f) * SPEED_MOVE;
	}
	if (pKeyboard->GetPress(DIK_D))
	{// 右移動
		move.x += sinf(pCameraInfo->rot.y - D3DX_PI * 0.5f) * SPEED_MOVE;
		move.z += cosf(pCameraInfo->rot.y - D3DX_PI * 0.5f) * SPEED_MOVE;
	}
	if (pKeyboard->GetPress(DIK_W))
	{//　奥移動
		move.x += sinf(pCameraInfo->rot.y + D3DX_PI) * SPEED_MOVE;
		move.z += cosf(pCameraInfo->rot.y + D3DX_PI) * SPEED_MOVE;
	}
	if (pKeyboard->GetPress(DIK_S))
	{// 手前移動
		move.x += sinf(pCameraInfo->rot.y) * SPEED_MOVE;
		move.z += cosf(pCameraInfo->rot.y) * SPEED_MOVE;
	}

	// スティック移動
	vecStick = { pJoypad->GetVecStickL().x ,pJoypad->GetVecStickL().y,0.0f };

	if (D3DXVec3Length(&vecStick) != 0)
	{
		// スティックの角度取得
		fRot = atan2f(pJoypad->GetVecStickL().x, pJoypad->GetVecStickL().y);

		move.x -= sinf(fRot + pCameraInfo->rot.y) * SPEED_MOVE;
		move.z -= cosf(fRot + pCameraInfo->rot.y) * SPEED_MOVE;
	}

	// 移動量の正規化
	D3DXVec3Normalize(&move, &move);

	if (m_bTurnDowner)
	{// 後ろ歩きなら遅い
		move *= SPEED_MOVE * 0.5f;
	}
	else
	{
		move *= SPEED_MOVE;
	}

	// 移動方向を向く処理
	Turn(move, 0.0f);

	if (m_boostState != BOOSTSTATE_EMPTY)
	{
		if (D3DXVec3Length(&move) != 0)
		{
			if (pKeyboard->GetPress(DIK_LSHIFT)/* || pJoypad->GetPress(CInputJoypad::PADBUTTONS_LB, 0)*/)
			{// ブースト移動
				if (m_bTurnDowner)
				{
					move.x += sinf(GetRot().y) * SPEED_MOVE * SPEED_SPRINT;
					move.z += cosf(GetRot().y) * SPEED_MOVE * SPEED_SPRINT;

					if (m_pBodyDowner->GetMotion() != MOTION_TURNBOOST)
					{// ブーストモーションへ移行
						m_pBodyDowner->SetMotion(MOTION_TURNBOOST);
					}
				}
				else
				{
					move.x += sinf(GetRot().y + D3DX_PI) * SPEED_MOVE * SPEED_SPRINT;
					move.z += cosf(GetRot().y + D3DX_PI) * SPEED_MOVE * SPEED_SPRINT;

					if (m_pBodyDowner->GetMotion() != MOTION_BOOST)
					{// ブーストモーションへ移行
						m_pBodyDowner->SetMotion(MOTION_BOOST);
					}
				}

				// カメラ揺れ設定
				pCamera->SetQuake(0.005f, 0.005f, 2);

				// ブースト消費
				m_fBoost -= COST_SPRINT;

				// ダッシュ状態に移行
				m_bSprint = true;
			}
			else
			{
				// ダッシュ状態解除
				m_bSprint = false;
			}
		}
		else
		{
			// ダッシュ状態解除
			m_bSprint = false;

			if (m_pBodyDowner->GetMotion() == MOTION_BOOST || m_pBodyDowner->GetMotion() == MOTION_TURNBOOST)
			{// ニュートラルモーションへ移行
				m_pBodyDowner->SetMotion(MOTION_NEUTRAL);
			}
		}

		if ((pKeyboard->GetTrigger(DIK_SPACE) || pJoypad->GetTrigger(CInputJoypad::PADBUTTONS_LB, 0)) && m_bLand)
		{// ジャンプ初動
			move.y += JUMP_START_POW;
		}

		if (pKeyboard->GetPress(DIK_SPACE) || pJoypad->GetPress(CInputJoypad::PADBUTTONS_LB, 0))
		{// ブーストジャンプ
			move.y += JUMP_POW;
			if (m_pBodyDowner->GetMotion() != MOTION_BOOST)
			{// ブーストモーションへ移行
				m_pBodyDowner->SetMotion(MOTION_BOOST);
			}

			// ブーストの設定
			Boost();
			
			m_fBoost -= COST_JUMP;

			m_bJump = true;
		}
		else
		{
			m_bJump = false;
		}
	}
	else
	{
		if (m_pBodyDowner->GetMotion() == MOTION_BOOST || m_pBodyDowner->GetMotion() == MOTION_TURNBOOST)
		{// ニュートラルモーションへ移行
			m_pBodyDowner->SetMotion(MOTION_NEUTRAL);
		}

		m_bSprint = false;
	}

	if (pKeyboard->GetRelease(DIK_LSHIFT) || pJoypad->GetRelease(CInputJoypad::PADBUTTONS_B, 0))
	{// ブーストボタンを離したら
		if (m_pBodyDowner->GetMotion() == MOTION_BOOST || m_pBodyDowner->GetMotion() == MOTION_TURNBOOST)
		{// ニュートラルモーションへ移行
			m_pBodyDowner->SetMotion(MOTION_NEUTRAL);
		}
	}

	if (pKeyboard->GetRelease(DIK_SPACE) || m_boostState == BOOSTSTATE_EMPTY || pJoypad->GetRelease(CInputJoypad::PADBUTTONS_LB, 0))
	{// ジャンプボタンを離したら
		if (m_pBodyDowner->GetMotion() == MOTION_BOOST || m_pBodyDowner->GetMotion() == MOTION_TURNBOOST)
		{// ニュートラルモーションへ移行
			m_pBodyDowner->SetMotion(MOTION_NEUTRAL);
		}
	}

	// 移動量加算
	SetMove(GetMove() + move);
}

//=====================================================
// 射撃操作
//=====================================================
void CPlayer::InputShot(void)
{
	CInputKeyboard *pKeyboard = CManager::GetKeyboard();
	CInputMouse *pMouse = CManager::GetMouse();
	CInputJoypad *pJoypad = CManager::GetJoypad();
	float fRapid, fCost;
	CCamera *pCamera = CManager::GetCamera();
	CBullet *pBullet = nullptr;

	// 銃口の位置、向きを取得
	D3DXVECTOR3 posMazzle = { m_mtxMazzle[0]._41,m_mtxMazzle[0]._42 ,m_mtxMazzle[0]._43 };
	D3DXVECTOR3 DestMazzle = { m_mtxMazzle[1]._41,m_mtxMazzle[1]._42 ,m_mtxMazzle[1]._43 };
	D3DXVECTOR3 vecMazzle = posMazzle - DestMazzle;

	// パラメーターの倍率設定
	fRapid = RAPID_SPEED - RAPID_MAX * m_fParamRapid;
	fCost = SHOT_COST - COST_MAX * m_fParamCost;

	if (m_pEnemy != nullptr)
	{
		if (m_bAim)
		{// 偏差撃ちの計算
			m_posLockon = LinePridiction(posMazzle, BULLET_SPEED, m_pEnemy->GetPosition(), m_pEnemy->GetMove());
		}

		m_posLockon.y += m_pEnemy->GetRadiusMax() * 0.5f;

		vecMazzle = m_posLockon - posMazzle;
	}

	D3DXVec3Normalize(&vecMazzle, &vecMazzle);

	if (m_boostState != BOOSTSTATE_EMPTY)
	{// ブースト残量がないと射撃ができない
		if (m_nTimerRapid <= 0)
		{
			if (pMouse->GetRelease(CInputMouse::BUTTON_LMB) || pJoypad->GetRelease(CInputJoypad::PADBUTTONS_RB, 0))
			{
				if ((pMouse->GetRepeat(CInputMouse::BUTTON_LMB) >= CHARGE_TIMER && pMouse->GetRelease(CInputMouse::BUTTON_LMB)) ||
					(pJoypad->GetRepeat(CInputJoypad::PADBUTTONS_RB, 0) >= CHARGE_TIMER && pJoypad->GetRelease(CInputJoypad::PADBUTTONS_RB, 0)))
				{// タメ撃ちの弾発射
					// 弾の発射
					pBullet = CBullet::Create
					(
						posMazzle,
						vecMazzle * BULLET_SPEED,
						BULLET_LIFE,
						CBullet::TYPE_PLAYER,
						true,
						CHARGE_RADIUS * m_fParamPower,
						CHARGE_DAMAGE
					);

					// エネルギー消費
					m_fBoost -= CHARGE_COST;

					if (m_pBodyUpper != nullptr)
					{// 射撃モーション
						m_pBodyUpper->SetMotion(2);
					}

					// 攻撃力成長
					m_fParamPower += m_fGrawPower;

					if (m_fParamPower > 1.0f)
					{// パラメーター補正
						m_fParamPower = 1.0f;
					}

					// カメラ揺れ設定
					pCamera->SetQuake(0.025f, 0.025f, 15);

					CParticle::Create(m_posMazzle, CParticle::TYPE_SHOT, D3DXVECTOR3(0.0f, 0.0f, 0.0f), &m_posMazzle);

					// SE再生
					CManager::GetSound()->Play(CSound::LABEL_SHOT);
				}
				else
				{
					if (m_nTimerShot == 0)
					{// 単発撃ち

						if (m_pBodyUpper != nullptr)
						{// 射撃モーション
							m_pBodyUpper->SetMotion(2);
						}

						// 弾の発射
						pBullet = CBullet::Create
						(
							posMazzle,
							vecMazzle * BULLET_SPEED,
							BULLET_LIFE,
							CBullet::TYPE_PLAYER,
							false,
							BULLET_RADIUS,
							NORMAL_DAMAGE
						);

						// 発射カウンター設定
						m_nTimerShot = (int)fRapid;

						// エネルギー消費
						m_fBoost -= fCost;

						// 効率成長
						m_fParamCost += m_fGrawCost;

						if (m_fParamCost > 1.0f)
						{// パラメーター補正
							m_fParamCost = 1.0f;
						}

						// カメラ揺れ設定
						pCamera->SetQuake(0.01f, 0.01f, 15);

						CParticle::Create(m_posMazzle, CParticle::TYPE_SHOT, D3DXVECTOR3(0.0f, 0.0f, 0.0f), &m_posMazzle);

						// SE再生
						CManager::GetSound()->Play(CSound::LABEL_SHOT);
					}
				}

				// 発射できなかった場合カウンタをリセット
				m_nTimerRapid = (int)fRapid;
			}
			
			if ((pMouse->GetPress(CInputMouse::BUTTON_LMB)
				&& pMouse->GetRepeat(CInputMouse::BUTTON_LMB) > RAPID_TIME) ||
				(pJoypad->GetPress(CInputJoypad::PADBUTTONS_RB, 0)
					&& pJoypad->GetRepeat(CInputJoypad::PADBUTTONS_RB, 0) > RAPID_TIME))
			{
				if (pMouse->GetRepeat(CInputMouse::BUTTON_LMB) < CHARGE_TIMER &&
					pJoypad->GetRepeat(CInputJoypad::PADBUTTONS_RB, 0) < CHARGE_TIMER)
				{// チャージ中パーティクル
					CParticle::Create(m_posMazzle, CParticle::TYPE_CHARGE, D3DXVECTOR3(0.0f, 0.0f, 0.0f), &m_posMazzle);
				}
				else
				{// マックスチャージのパーティクル
					CParticle::Create(m_posMazzle, CParticle::TYPE_MAXCHARGE, D3DXVECTOR3(0.0f, 0.0f, 0.0f), &m_posMazzle);
				}
			}
		}
		else
		{
			if (pMouse->GetPress(CInputMouse::BUTTON_LMB) || pJoypad->GetPress(CInputJoypad::PADBUTTONS_RB, 0))
			{// 連射モード

				if (pMouse->GetRepeat(CInputMouse::BUTTON_LMB) > 5 || pJoypad->GetRepeat(CInputJoypad::PADBUTTONS_RB, 0) > 5)
				{
					if (pMouse->GetRepeat(CInputMouse::BUTTON_LMB) % (int)fRapid == 0 && pMouse->GetPress(CInputMouse::BUTTON_LMB))
					{
						// 弾の発射
						pBullet = CBullet::Create
						(
							posMazzle,
							vecMazzle * BULLET_SPEED,
							BULLET_LIFE,
							CBullet::TYPE_PLAYER,
							false,
							BULLET_RADIUS,
							NORMAL_DAMAGE
						);

						if (m_pBodyUpper != nullptr)
						{// 射撃モーション
							m_pBodyUpper->SetMotion(2);
						}

						// エネルギー消費
						m_fBoost -= fCost;

						// 連射力成長
						m_fParamRapid += m_fGrawRapid;

						if (m_fParamRapid > 1.0f)
						{// パラメーター補正
							m_fParamRapid = 1.0f;
						}

						CParticle::Create(m_posMazzle, CParticle::TYPE_SHOT, D3DXVECTOR3(0.0f, 0.0f, 0.0f), &m_posMazzle);

						// SE再生
						CManager::GetSound()->Play(CSound::LABEL_SHOT);
					}

					if (pJoypad->GetRepeat(CInputJoypad::PADBUTTONS_RB, 0) % (int)fRapid == 0 && pJoypad->GetPress(CInputJoypad::PADBUTTONS_RB, 0))
					{
						// 弾の発射
						pBullet = CBullet::Create
						(
							posMazzle,
							vecMazzle * BULLET_SPEED,
							BULLET_LIFE,
							CBullet::TYPE_PLAYER,
							false,
							BULLET_RADIUS,
							NORMAL_DAMAGE
						);

						if (m_pBodyUpper != nullptr)
						{// 射撃モーション
							m_pBodyUpper->SetMotion(2);
						}

						// エネルギー消費
						m_fBoost -= fCost;

						// 連射力成長
						m_fParamRapid += m_fGrawRapid;

						if (m_fParamRapid > 1.0f)
						{// パラメーター補正
							m_fParamRapid = 1.0f;
						}

						CParticle::Create(m_posMazzle, CParticle::TYPE_SHOT, D3DXVECTOR3(0.0f, 0.0f, 0.0f), &m_posMazzle);

						// SE再生
						CManager::GetSound()->Play(CSound::LABEL_SHOT);
					}
				}
			}
			else
			{
				m_nTimerRapid--;
			}

			if (pMouse->GetRelease(CInputMouse::BUTTON_LMB) || pJoypad->GetRelease(CInputJoypad::PADBUTTONS_RB, 0))
			{// 連射状態解除
				m_nTimerRapid = 0;
			}
		}
	}

	if (pBullet != nullptr && m_pEnemy != nullptr)
	{// 発射した弾をロックオン弾に設定
		pBullet->EnableLock(true);
	}
}

//=====================================================
// カメラ操作
//=====================================================
void CPlayer::InputCamera(void)
{
	// 汎用処理の取得
	CUniversal *pUniversal = CManager::GetUniversal();

	// 入力情報入手
	CInputMouse *pMouse = CManager::GetMouse();
	CInputJoypad *pJoypad = CManager::GetJoypad();

	// カメラ取得
	CCamera::Camera *pCameraInfo = CManager::GetCamera()->GetCamera();

	if (pMouse->GetPress(CInputMouse::BUTTON_RMB)/* || pJoypad->GetPress(CInputJoypad::PADBUTTONS_LB, 0)*/)
	{// エイム時
		m_bAim = true;
		pCameraInfo->fLength = AIM_LENGTH;
	}
	else
	{// 非エイム時
		m_bAim = false;
		pCameraInfo->fLength = NOT_AIM_LENGTH;
	}

	// マウス入力の取得
	D3DXVECTOR3 vecTurn = { (float)pMouse->GetMoveIY() ,(float)pMouse->GetMoveIX(),0.0f };

	// コントローラーの入力
	vecTurn = vecTurn + D3DXVECTOR3{ -pJoypad->GetJoyStickRY(0), pJoypad->GetJoyStickRX(0), 0.0f };

	// 回転量の正規化
	D3DXVec3Normalize(&vecTurn, &vecTurn);

	// 回転量設定
	pCameraInfo->rot.x -= vecTurn.x * ROLL_SPEED * 0.3f;
	pCameraInfo->rot.y += vecTurn.y * ROLL_SPEED;

	// カメラの角度制限
	if (pCameraInfo->rot.x < D3DX_PI * 0.5f - LIMIT_CAMERAROT)
	{
		pCameraInfo->rot.x = D3DX_PI * 0.5f - LIMIT_CAMERAROT;
	}
	else if (pCameraInfo->rot.x > D3DX_PI * 0.5f + LIMIT_CAMERAROT)
	{
		pCameraInfo->rot.x = D3DX_PI * 0.5f + LIMIT_CAMERAROT;
	}

	if (m_pBodyUpper != nullptr)
	{// 上半身の回転
		D3DXVECTOR3 rotUpper = D3DXVECTOR3(pCameraInfo->rot.x - D3DX_PI * 0.5f, pCameraInfo->rot.y - m_pBodyDowner->GetRot().y, m_pBodyUpper->GetRot().z);

		// 向きの修正
		pUniversal->LimitRot(&rotUpper.x);
		pUniversal->LimitRot(&rotUpper.y);
		pUniversal->LimitRot(&rotUpper.z);

		// 上半身の角度制限
		if (rotUpper.x < -LIMIT_UPPERROT)
		{
			rotUpper.x = -LIMIT_UPPERROT;
		}
		else if (rotUpper.x > LIMIT_UPPERROT)
		{
			rotUpper.x = LIMIT_UPPERROT;
		}

		// 上半身の回転
		m_pBodyUpper->SetRot(rotUpper);

		// 上半身回転制御
		LimitUpperRot(0.0f);
	}

	// 角度の修正
	pUniversal->LimitRot(&pCameraInfo->rot.y);

	if (m_nTimerShot > 0)
	{
		m_nTimerShot--;
	}
}

//=====================================================
// ロックオン処理
//=====================================================
void CPlayer::Lockon(void)
{
	// 汎用処理の取得
	CUniversal *pUniversal = CManager::GetUniversal();

	// カメラ取得
	CCamera::Camera *pCamera = CManager::GetCamera()->GetCamera();

	// 計算用変数
	D3DXVECTOR3 pos1, pos2,vecDiff,veclockon;
	float fLength = FLT_MAX;
	float fDiff,fDiffAngle, fDiffAngle2,fDiffAngle3;
	float fDiffLength, fDiffLength2;
	float fMaxDiffAngle = D3DX_PI * 2;
	D3DXMATRIX *pMtxUpper = m_pBodyUpper->GetMatrix();
	float fRange = LOCKON_RANGE;

	pos1 = { m_mtxLockon[0]._41,m_mtxLockon[0]._42,m_mtxLockon[0]._43 };
	pos2 = { m_mtxLockon[1]._41,m_mtxLockon[1]._42,m_mtxLockon[1]._43 };

	// 敵の取得
	CEnemy *pEnemy = CEnemy::GetEnemy();
	CEnemy *pLockEnemy = nullptr;

	if (m_bAim)
	{// エイム時のロックオン距離
		fRange = AIM_LOCKON_RANGE;
	}

	while (pEnemy != nullptr)
	{
		// 次のアドレスを保存
		CEnemy *pEnemyNext = pEnemy->GetNext();

		// ロックオン表示を初期で解除する
		pEnemy->SetLockon(false);

		if (pEnemy->GetState() == CEnemy::STATE_DEATH)
		{
			// 次のアドレスを代入
			pEnemy = pEnemyNext;

			continue;
		}

		if (CCollision::IsCross(pEnemy->GetPosition(), GetPosition(), pos1, nullptr) == false &&
			CCollision::IsCross(pEnemy->GetPosition(), GetPosition(), pos2, nullptr))
		{// 範囲内にいたらロックオン
		 // 距離を計算
			vecDiff = pEnemy->GetPosition() - D3DXVECTOR3(pMtxUpper->_41, pMtxUpper->_42, pMtxUpper->_43);
			fDiff = D3DXVec3Length(&vecDiff);

			// Y平面のベクトルの長さを取得
			fDiffLength = D3DXVec2Length(&D3DXVECTOR2(vecDiff.x, vecDiff.z));

			// 敵との角度を取得
			fDiffAngle = atan2f(fDiffLength, vecDiff.y);

			// ロックオンの向きを取得
			veclockon = D3DXVECTOR3(m_mtxLockon[0]._41, m_mtxLockon[0]._42, m_mtxLockon[0]._43);
			veclockon = veclockon - D3DXVECTOR3(pMtxUpper->_41, pMtxUpper->_42, pMtxUpper->_43);

			// Y平面のベクトルの長さを取得
			fDiffLength2 = D3DXVec2Length(&D3DXVECTOR2(veclockon.x, veclockon.z));

			// ロックオンの角度を取得
			fDiffAngle2 = atan2f(fDiffLength2, veclockon.y);

			// 上下の角度取得
			fDiffAngle = fDiffAngle - fDiffAngle2;

			// 中心に捉えてるかの角度
			fDiffAngle3 = atan2f(vecDiff.x, vecDiff.z) - pCamera->rot.y - D3DX_PI;

			// 向きの修正
			pUniversal->LimitRot(&fDiffAngle3);
			pUniversal->LimitRot(&fDiffAngle);

			if (fDiff < fRange && fDiffAngle * fDiffAngle < LOCKON_ANGLE * LOCKON_ANGLE)
			{
				if (fDiffAngle3 * fDiffAngle3 < fMaxDiffAngle * fMaxDiffAngle)
				{// 最小値より差分が小さい場合
					fMaxDiffAngle = fDiffAngle3;

					// 一番近い敵を取得
					pLockEnemy = pEnemy;
				}
			}
		}

		// 次のアドレスを代入
		pEnemy = pEnemyNext;
	}

	if (pLockEnemy != nullptr)
	{// 優先された敵をロックオンする
		m_posLockon = pLockEnemy->GetPosition();

		m_pEnemy = pLockEnemy;

		pLockEnemy->SetLockon(true);
	}
	else
	{
		m_posLockon = { 0.0f,0.0f,0.0f };

		m_pEnemy = nullptr;
	}

	if (m_pLockon != nullptr)
	{
		m_pLockon->SetEnemy(m_pEnemy);

		if (m_pEnemy == nullptr)
		{
			// マズル方向にマーカー設置
			D3DXVECTOR3 vecMazzle;

			vecMazzle = 
			{
				m_mtxMazzle[1]._41 - m_mtxMazzle[0]._41,
				m_mtxMazzle[1]._42 - m_mtxMazzle[0]._42,
				m_mtxMazzle[1]._43 - m_mtxMazzle[0]._43
			};

			D3DXVec3Normalize(&vecMazzle, &vecMazzle);

			vecMazzle *= BULLET_SPEED * BULLET_LIFE;

			m_pLockon->SetPosDest(m_posMazzle - vecMazzle);
		}
	}
}

//=====================================================
// マトリックスの設定
//=====================================================
void CPlayer::SetMatrix(void)
{
	// カメラ取得
	CCamera::Camera *pCamera = CManager::GetCamera()->GetCamera();

	// 汎用処理の取得
	CUniversal *pUniversal = CManager::GetUniversal();

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// 変数宣言
	D3DXMATRIX mtxRot, mtxTrans;

	// 本体のワールドマトリックス=================================
	// ワールドマトリックス初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans,
		m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックス設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 下半身のワールドマトリックス=================================
	if (m_pBodyDowner != nullptr)
	{
		D3DXMATRIX *pMtx = m_pBodyDowner->GetMatrix();

		pUniversal->SetOffSet(pMtx, m_mtxWorld, m_pBodyDowner->GetPosition(), m_pBodyDowner->GetRot());

		D3DXMATRIX *pMtxWaist = m_pBodyDowner->GetParts(0)->m_pParts->GetMatrix();

		m_pBodyDowner->MultiplyMtx();

		// 腰のワールドマトリックス=================================
		// ワールドマトリックス初期化
		D3DXMatrixIdentity(&m_mtxWaist);

		// 向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot,
			0.0f, 0.0f, 0.0f);
		D3DXMatrixMultiply(&m_mtxWaist, &m_mtxWaist, &mtxRot);

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans,
			0.0f, 0.0f, 0.0f);
		D3DXMatrixMultiply(&m_mtxWaist, &m_mtxWaist, &mtxTrans);

		D3DXMatrixMultiply(&m_mtxWaist, &m_mtxWaist, pMtxWaist);

		// ワールドマトリックス設定
		pDevice->SetTransform(D3DTS_WORLD, &m_mtxWaist);
	}

	// 上半身のワールドマトリックス=================================
	if (m_pBodyUpper != nullptr)
	{
		D3DXMATRIX *pMtx = m_pBodyUpper->GetMatrix();

		pUniversal->SetOffSet(pMtx, m_mtxWaist, m_pBodyUpper->GetPosition(), m_pBodyUpper->GetRot());
	}

#ifdef DEBUG
	CManager::GetDebugProc()->Print("\n上半身の向き：[%f,%f,%f]", m_pBodyUpper->GetRot().x, m_pBodyUpper->GetRot().y, m_pBodyUpper->GetRot().z);
	CManager::GetDebugProc()->Print("\n下半身の向き：[%f,%f,%f]", m_pBodyDowner->GetRot().x, m_pBodyDowner->GetRot().y, m_pBodyDowner->GetRot().z);
#endif
}

//=====================================================
// 状態管理
//=====================================================
void CPlayer::ManageState(void)
{
	switch (m_state)
	{
	case CPlayer::STATE_NORMAL:
		break;
	case CPlayer::STATE_DEATH:

		m_nCntDeath++;

		if (m_nCntDeath >= TIME_DEATH)
		{
			// 腰の位置取得
			D3DXVECTOR3 pos = { m_mtxWaist._41,m_mtxWaist._42, m_mtxWaist._43 };

			// 爆発パーティクル
			CParticle::Create(pos, CParticle::TYPE_EXPLOSION);

			// 煙と破片の召喚
			CSmokeSpawner::Create(pos, 15.0f, 1, 6);
			CDebrisSpawner::Create(pos, 18.0f, 1, 12);

			// 終了処理
			Uninit();

			CGame::SetState(CGame::STATE_RESULT);

			CResult::Create(false);

			// SE再生
			CManager::GetSound()->Play(CSound::LABEL_EXPLOSION);
		}

		break;
	default:
		break;
	}
}

//=====================================================
// モーション管理
//=====================================================
void CPlayer::ManageMotion(void)
{
	// 変数宣言
	float fSpeed = D3DXVec3Length(&GetMove());
	MOTION motionDowner = MOTION_NEUTRAL;
	MOTION motionUpper = MOTION_NEUTRAL;
	bool bFinishUpper = false;
	bool bFinishDowner = false;

	if (m_pBodyUpper != nullptr)
	{// 上半身モーション情報取得
		motionUpper = (MOTION)m_pBodyUpper->GetMotion();
		bFinishUpper = m_pBodyUpper->IsFinish();
	}

	if (m_pBodyDowner != nullptr)
	{// 下半身モーション取得
		motionDowner = (MOTION)m_pBodyDowner->GetMotion();
		bFinishDowner = m_pBodyDowner->IsFinish();
	}

#ifdef DEBUG
	CManager::GetDebugProc()->Print("\nプレイヤーの速度：[%f]", fSpeed);
	CManager::GetDebugProc()->Print("\n下半身モーション：[%d]", motionDowner);
	CManager::GetDebugProc()->Print("\n下半身モーション終了：[%d]", bFinishDowner);
#endif

	if (m_bLand)
	{// 地上モーション
		if (motionDowner != MOTION_SHOT || (motionDowner == MOTION_SHOT && bFinishDowner))
		{
			if ((int)fSpeed != 0 && motionDowner != MOTION_BOOST && motionDowner != MOTION_TURNBOOST)
			{// ある程度動いていれば歩くモーション
				if (m_pBodyDowner != nullptr)
				{
					if (m_bTurnDowner)
					{// 後ろ歩きモーション
						if (motionDowner != MOTION_TURNWALK)
						{
							m_pBodyDowner->SetMotion(MOTION_TURNWALK);
						}
					}
					else
					{// 順歩きモーション
						if (motionDowner != MOTION_WALK)
						{
							m_pBodyDowner->SetMotion(MOTION_WALK);
						}
					}
				}
			}
			else
			{
				if ((int)fSpeed == 0 && (motionDowner == MOTION_WALK || motionDowner == MOTION_TURNWALK))
				{// 立ち止まる
					if (m_pBodyDowner != nullptr)
					{// 下半身モーション設定
						m_pBodyDowner->SetMotion(MOTION_NEUTRAL);
					}
				}
			}
		}
	}
}

//=====================================================
// 目標方向を向く処理
//=====================================================
void CPlayer::Turn(D3DXVECTOR3 move, float fRotOld)
{
	// 汎用処理の取得
	CUniversal *pUniversal = CManager::GetUniversal();

	// カメラ取得
	CCamera::Camera *pCamera = CManager::GetCamera()->GetCamera();

	// 変数宣言
	float fRotDest;
	float fRotDowner = 0.0f;	// 上半身の角度
	float fRotDiff2;
	D3DXVECTOR3 rot = { 0.0f,0.0f,0.0f };

	if (D3DXVec3Length(&move) != 0)
	{// 動いてる時の判定
		// 移動方向を取得
		fRotDest = atan2f(move.x, move.z) + D3DX_PI;

		// 値の補正
		pUniversal->LimitRot(&fRotDest);

		// 上半身との角度の差分を取得
		if (m_pBodyUpper != nullptr)
		{
			// カメラと目標向きの差分取得
			fRotDiff2 = fRotDest - pCamera->rot.y;

			// 値の補正
			pUniversal->LimitRot(&fRotDiff2);

			if (fRotDiff2 * fRotDiff2 > TURN_DOWNER * TURN_DOWNER)
			{// 逆歩き
				m_bTurnDowner = true;
			}
			else/* if(fRotDiff2 * fRotDiff2 < TURN_ROT * TURN_ROT)*/
			{// 順歩き
				m_bTurnDowner = false;
			}

			if (m_bTurnDowner)
			{
				fRotDest = atan2f(move.x, move.z);
			}
			else
			{
				fRotDest = atan2f(move.x, move.z) + D3DX_PI;
			}

			// 値の補正
			pUniversal->LimitRot(&fRotDest);
		}

		if (m_pBodyDowner != nullptr)
		{
			// 向きの差分を取得
			float fRotDiff = fRotDest - GetRot().y;

			// 値の補正
			pUniversal->LimitRot(&fRotDiff);

			// 向きの設定
			rot = D3DXVECTOR3(m_pBodyDowner->GetRot().x, m_pBodyDowner->GetRot().y + fRotDiff * 0.1f, m_pBodyDowner->GetRot().z);

			// 値の補正
			pUniversal->LimitRot(&rot.y);

			// 下半身の回転
			m_pBodyDowner->SetRot(rot);

			// プレイヤーの回転
			SetRot(rot);
		}
	}
}

//=====================================================
// 上半身の回転制御
//=====================================================
void CPlayer::LimitUpperRot(float fRotOld)
{
	// 汎用処理の取得
	CUniversal *pUniversal = CManager::GetUniversal();

	// 変数宣言
	float fRot = 0.0f;
	float fRotDiff = 0.0f;
	D3DXVECTOR3 rot;

	if (m_pBodyUpper != nullptr)
	{
		// 上半身の角度を取得
		fRot = m_pBodyUpper->GetRot().y;

		if (m_pBodyDowner != nullptr)
		{
			if (fRot > TURN_ROT)
			{// 右にはみ出た場合
				fRotDiff = fRot - TURN_ROT;
			}
			else if (fRot < -TURN_ROT)
			{// 左にはみ出た場合
				fRotDiff = fRot + TURN_ROT;
			}

			rot = D3DXVECTOR3
			(
				m_pBodyDowner->GetRot().x,
				m_pBodyDowner->GetRot().y + fRotDiff,
				m_pBodyDowner->GetRot().z
			);

			pUniversal->LimitRot(&rot.y);

			// 下半身を回転させる
			m_pBodyDowner->SetRot(rot);

			// プレイヤーの回転
			SetRot(rot);
		}
	}
}

//=====================================================
// 偏差撃ちの処理
//=====================================================
D3DXVECTOR3 CPlayer::LinePridiction(D3DXVECTOR3 pos, float fSpeedBullet, D3DXVECTOR3 posTarget, D3DXVECTOR3 moveTarget)
{
	// 変数宣言
	D3DXVECTOR3 vecDiff;
	float fSpeedTarget;
	float fDataA, fDataB, fDataC, fDataD;
	int nFrame1, nFrame2;

	// 目標の速度を計算
	fSpeedTarget = D3DXVec3Length(&moveTarget);

	// 目標との位置の差分を取得
	vecDiff = posTarget - pos;

	fDataA = (moveTarget.x * moveTarget.x + moveTarget.y * moveTarget.y + moveTarget.z * moveTarget.z) - fSpeedBullet * fSpeedBullet;

	fDataB = 2 * (posTarget.x * moveTarget.x + posTarget.y * moveTarget.y + posTarget.z * moveTarget.z);

	fDataC = (posTarget.x * posTarget.x + posTarget.y * posTarget.y + posTarget.z * posTarget.z);

	if (fDataA == 0)
	{// 0割り防止
		if (fDataB == 0)
		{
			return posTarget;
		}
		else
		{
			return posTarget + moveTarget * (-fDataC / fDataB);
		}
	}

	//二次方程式の解の公式の判別式で分類
	fDataD = fDataB * fDataB - 4 * fDataA * fDataC;

	if (fDataD > 0)
	{
		float fDataE = (float)sqrt(fDataD);
		nFrame1 = (int)((-fDataB - fDataE) / (2 * fDataA));
		nFrame2 = (int)((-fDataB + fDataE) / (2 * fDataA));
		//解は2つなので正の数の最小値を使う
		nFrame1 = (int)PlusMin((float)nFrame1, (float)nFrame2);
	}
	else
	{
		//虚数解
		//当たらないので今の位置を狙う
		nFrame1 = 0;
	}

	//予想位置を返す
	return posTarget + moveTarget * (float)nFrame1;
}

//=====================================================
// プラスの最小値を返す処理
//=====================================================
float CPlayer::PlusMin(float fData1, float fData2)
{
	if (fData1 < 0 && fData2 < 0)
	{// どちらも負の値なら０を返す
		return 0;
	}
	else if (fData1 < 0)
	{
		return fData2;
	}
	else if (fData2 < 0)
	{
		return fData1;
	}

	return fData1 < fData2 ? fData1 : fData2;
}

//=====================================================
// ブーストパーティクルの設定
//=====================================================
void CPlayer::Boost(void)
{
	// 汎用処理の取得
	CUniversal *pUniversal = CManager::GetUniversal();

	// 変数宣言
	D3DXMATRIX mtxBody, mtxVector;
	D3DXMATRIX *pMtxBody = nullptr;
	D3DXVECTOR3 vecDiff;
	D3DXVECTOR3 rot = { 0.0f,0.0f,0.0f };
	float fLength;

	if (m_pBodyUpper != nullptr)
	{// 胴体のマトリックス取得
		m_pBodyUpper->MultiplyMtx();

		pMtxBody = m_pBodyUpper->GetParts(0)->m_pParts->GetMatrix();

		if (pMtxBody != nullptr)
		{
			// 根元のマトリックスの設定
			pUniversal->SetOffSet(&mtxBody,*pMtxBody,D3DXVECTOR3(0.0f,30.0f,30.0f));

			m_posBoost = D3DXVECTOR3{ mtxBody._41,mtxBody._42 ,mtxBody._43 } + m_move;

			// 噴射方向のマトリックスの設定
			pUniversal->SetOffSet(&mtxVector, *pMtxBody, D3DXVECTOR3(0.0f, -10.0f, 50.0f));

			m_vecBoost = { mtxVector._41,mtxVector._42 ,mtxVector._43 };

			// 噴射ベクトルを計算
			vecDiff = m_posBoost - m_vecBoost;

			// Y軸方向を計算
			rot.y = atan2f(vecDiff.x, vecDiff.z)/* - m_pBodyUpper->GetRot().y*/;

			// Y軸平面の長さを取得
			fLength = sqrtf(vecDiff.x * vecDiff.x + vecDiff.z * vecDiff.z);

			// X軸方向を計算
			rot.x = atan2f(fLength * fLength, vecDiff.y) + D3DX_PI;

			// パーティクル設定
			CParticle::Create(m_posBoost,CParticle::TYPE_BOOST,rot,&m_posBoost);
		}
	}
}

//=====================================================
// ヒット処理
//=====================================================
void CPlayer::Hit(float fDamage)
{
	if (m_state == STATE_DEATH)
	{
		return;
	}

	// SE再生
	CManager::GetSound()->Play(CSound::LABEL_HIT);

	// 体力減少
	m_nLife -= (int)fDamage;

	if (m_pAssess != nullptr)
	{// ダメージによる評価ペナルティ
		m_pAssess->AddParam(-DAMAGE_PENALTY, CAssess::PARAM_HIT);
	}

	if (m_nLife <= 0)
	{// 死亡判定
		// 腰の位置取得
		D3DXVECTOR3 pos = { m_mtxWaist._41,m_mtxWaist._42, m_mtxWaist._43 };

		m_nLife = 0;

		CNoise::Create(TIME_DEATH);

		CExplSpawner::Create(pos, 500.0f, TIME_DEATH);

		Death();

		// ゲームの状態をリザルトにする
		CGame::SetState(CGame::STATE_RESULT);
	}
	else
	{
		CNoise::Create();
	}
}

//=====================================================
// 死亡処理
//=====================================================
void CPlayer::Death(void)
{
	if (m_pBodyDowner != nullptr && m_pBodyUpper != nullptr)
	{
		m_pBodyUpper->SetRot(D3DXVECTOR3(0.0f,0.0f,0.0f));

		m_pBodyDowner->SetMotion(6);

		m_pBodyUpper->SetMotion(4);
	}

	m_move = { 0.0f,0.0f,0.0f };

	m_bSprint = false;

	m_state = STATE_DEATH;
}

//=====================================================
// ヒット数設定
//=====================================================
void CPlayer::SetNumHit(int nNumHit)
{
	if (m_pAssess != nullptr)
	{// ヒット評価の加減算
		int nDiff = nNumHit - m_nNumHit;

		if (nDiff > 0)
		{// 加算されていたら
			m_pAssess->AddParam(HIT_ADD, CAssess::PARAM_HIT);
		}
		else
		{
			m_pAssess->AddParam(-HIT_ADD, CAssess::PARAM_HIT);
		}
	}

	m_nNumHit = nNumHit;
}

//=====================================================
// 描画処理
//=====================================================
void CPlayer::Draw(void)
{
	// カメラ取得
	CCamera::Camera *pCameraInfo = CManager::GetCamera()->GetCamera();

	// マトリックスの設定
	SetMatrix();

#ifdef _DEBUG
	CManager::GetDebugProc()->Print("\nプレイヤーの位置：[%f,%f,%f]", GetPosition().x, GetPosition().y, GetPosition().z);
	CManager::GetDebugProc()->Print("\nカメラの角度：[%f,%f,%f]", pCameraInfo->rot.x, pCameraInfo->rot.y, pCameraInfo->rot.z);
	CManager::GetDebugProc()->Print("\n連射力：[%f]", m_fParamRapid);
	CManager::GetDebugProc()->Print("\n攻撃力：[%f]", m_fParamPower);
	CManager::GetDebugProc()->Print("\n効率：[%f]", m_fParamCost);
	CManager::GetDebugProc()->Print("\nヒット数：[%d]", m_nNumHit);
	CManager::GetDebugProc()->Print("\nプレイヤー体力[%d]", m_nLife);
	CManager::GetDebugProc()->Print("\nプレイヤージャンプ[%d]", m_bJump);
	CManager::GetDebugProc()->Print("\n当たり判定数[%d]", CCollision::GetNumAll());
	CManager::GetDebugProc()->Print("\nテクスチャ数[%d]", CTexture::GetNumAll());
	CManager::GetDebugProc()->Print("\nモデル数[%d]", CModel::GetNumAll());
	CManager::GetDebugProc()->Print("\nリセット[F3]");
#else
	CManager::GetDebugProc()->Print("\n\n\n\n\n\n\n\n\n\n\n");
	CManager::GetDebugProc()->Print("//----------------------------\n");
	CManager::GetDebugProc()->Print("// プレイヤー基本操作\n");
	CManager::GetDebugProc()->Print("//----------------------------\n");
	CManager::GetDebugProc()->Print("[WASD]移動\n");
	CManager::GetDebugProc()->Print("[LSHIFT + 移動]ダッシュ\n");
	CManager::GetDebugProc()->Print("[SPACE]ジャンプ\n");
	CManager::GetDebugProc()->Print("[LMB]射撃\n");
	CManager::GetDebugProc()->Print("[RMB]ズーム\n");
	CManager::GetDebugProc()->Print("\n");
	CManager::GetDebugProc()->Print("//----------------------------\n");
	CManager::GetDebugProc()->Print("// 射撃の種類\n");
	CManager::GetDebugProc()->Print("//----------------------------\n");
	CManager::GetDebugProc()->Print("[短押し]単発撃ち\n");
	CManager::GetDebugProc()->Print("[長押し]チャージショット\n");
	CManager::GetDebugProc()->Print("[ダブルクリック→長押し]連射\n");
#endif
}

//=====================================================
// 生成処理
//=====================================================
CPlayer *CPlayer::Create(void)
{
	CPlayer *pPlayer = nullptr;

	if (pPlayer == nullptr)
	{// インスタンス生成
		pPlayer = new CPlayer;

		// 初期化処理
		pPlayer->Init();
	}

	return pPlayer;
}

//=====================================================
// パラメーター読込処理
//=====================================================
HRESULT CPlayer::Load(void)
{
	// 変数宣言
	char cTemp[256];

	// ファイルから読み込む
	FILE *pFile = fopen("data\\player.txt", "r");

	if (pFile != nullptr)
	{// ファイルが開けた場合
		while (true)
		{
			// 文字読み込み
			fscanf(pFile, "%s", &cTemp[0]);

			if (strcmp(cTemp, "PARAMSET") == 0)
			{// パラメーター読込開始
				while (true)
				{
					// 文字読み込み
					fscanf(pFile, "%s", &cTemp[0]);

					if (strcmp(cTemp, "INITIAL_RAPID") == 0)
					{// 初期連射力読込
						fscanf(pFile, "%s", &cTemp[0]);

						fscanf(pFile, "%f", &m_fParamRapid);
					}

					if (strcmp(cTemp, "INITIAL_POWER") == 0)
					{// 初期攻撃力読込
						fscanf(pFile, "%s", &cTemp[0]);

						fscanf(pFile, "%f", &m_fParamPower);
					}

					if (strcmp(cTemp, "INITIAL_COST") == 0)
					{// 初期効率読込
						fscanf(pFile, "%s", &cTemp[0]);

						fscanf(pFile, "%f", &m_fParamCost);
					}

					if (strcmp(cTemp, "GRAW_RAPID") == 0)
					{// 連射力成長率読込
						fscanf(pFile, "%s", &cTemp[0]);

						fscanf(pFile, "%f", &m_fGrawRapid);
					}

					if (strcmp(cTemp, "GRAW_POWER") == 0)
					{// 攻撃力成長率読込
						fscanf(pFile, "%s", &cTemp[0]);

						fscanf(pFile, "%f", &m_fGrawPower);
					}

					if (strcmp(cTemp, "GRAW_COST") == 0)
					{// 効率成長率読込
						fscanf(pFile, "%s", &cTemp[0]);

						fscanf(pFile, "%f", &m_fGrawCost);
					}

					if (strcmp(cTemp, "END_PARAMSET") == 0)
					{// パラメーター読込終了
						break;
					}
				}
			}

			if (strcmp(cTemp, "END_SCRIPT") == 0)
			{// 終了条件
				break;
			}
		}

		// ファイルを閉じる
		fclose(pFile);
	}

	return S_OK;
}