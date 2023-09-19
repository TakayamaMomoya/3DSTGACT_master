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
#include "tutorial.h"
#include "player.h"
#include "enemy.h"
#include "score.h"
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
#include "objectmanager.h"

//*****************************************************
// マクロ定義
//*****************************************************
#define TRANS_TIME	(60)	// 終了までの余韻のフレーム数
#define LIMIT_LENGTH	(10000)	// 移動制限の距離
#define LIMIT_WIDTH	(100)	// 推定プレイヤーの幅
#define LIMIT_HEIGHT	(2000)	// 制限高度
#define BONUS_TIME	(40)	// ボーナスが付与される最低限のタイム
#define RATE_BONUS	(0.015f)	// 1秒当たりのタイムボーナス

//=====================================================
// コンストラクタ
//=====================================================
CTutorial::CTutorial()
{
	m_progress = PROGRESS_START;
}

//=====================================================
// デストラクタ
//=====================================================
CTutorial::~CTutorial()
{

}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CTutorial::Init(void)
{
	CPlayer *pPlayer = CPlayer::Create();
	CMeshField *pMeshField = CMeshField::Create();
	CObjectManager *pObjManager = CManager::GetObjectManager();

	if (pPlayer != nullptr && pObjManager != nullptr)
	{// プレイヤーの適用
		pObjManager->BindPlayer(pPlayer);
	}

	// 敵情報読込
	CEnemy::Load();

	// パーティクルの読込
	CParticle::Load();

	// スカイボックス生成
	//CSkybox::Create();

	// メッシュフィールドの生成
	if (pMeshField != nullptr && pObjManager != nullptr)
	{
		pObjManager->BindMeshField(pMeshField);
	}

	// フォグを消す
	CRenderer::EnableFog(false);

	// 塵管理の生成
	CDustManager::Create();

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CTutorial::Uninit(void)
{
	// ブロック情報削除
	CBlock::DeleteAll();

	// ブロック番号削除
	CBlock::DeleteIdx();

	// 敵破棄
	CEnemy::Unload();

	// オブジェクト全棄
	CObject::ReleaseAll();
}

//=====================================================
// 更新処理
//=====================================================
void CTutorial::Update(void)
{
	// 入力取得
	CInputKeyboard *pKeyboard = CManager::GetKeyboard();
	CInputJoypad *pJoypad = CManager::GetJoypad();

	CCamera *pCamera = CManager::GetCamera();

	// シーンの更新
	CScene::Update();

	// カメラ更新
	if (pCamera != nullptr)
	{
		// 追従処理
		pCamera->FollowPlayer();


		// カメラ揺れの処理
		pCamera->Quake();
	}

	// 状態管理
	ManageState();

	// プレイヤー移動制限処理
	LimitPlayerPos();

#ifdef _DEBUG	// デバッグ処理
	Debug();
#endif
}

//=====================================================
// プレイヤー位置制限処理
//=====================================================
void CTutorial::LimitPlayerPos(void)
{
	D3DXVECTOR3 pos = { 0.0f,0.0f,0.0f };
	D3DXVECTOR3 move = { 0.0f,0.0f,0.0f };

	CPlayer *pPlayer = nullptr;
	CObjectManager *pObjManager = CManager::GetObjectManager();

	if (pObjManager != nullptr)
	{// プレイヤーの適用
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

	pPlayer->SetPosition(pos);
	pPlayer->SetMove(move);
}

//=====================================================
// 状態管理
//=====================================================
void CTutorial::ManageState(void)
{

}

//=====================================================
// デバッグ処理
//=====================================================
void CTutorial::Debug(void)
{

}

//=====================================================
// 進行状況の進行
//=====================================================
void CTutorial::AddProgress(ACTION action)
{
	switch (m_progress)
	{
	case CTutorial::PROGRESS_START:
		break;
	case CTutorial::PROGRESS_SHOT:
		break;
	case CTutorial::PROGRESS_MAX:
		break;
	default:
		break;
	}
}

//=====================================================
// 描画処理
//=====================================================
void CTutorial::Draw(void)
{

}