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
#include "object.h"
#include "block.h"
#include "meshfield.h"
#include "universal.h"
#include "particle.h"
#include "inputkeyboard.h"
#include "inputjoypad.h"
#include "fade.h"
#include "camera.h"
#include "dustmanager.h"
#include "objectmanager.h"
#include "tutorialmanager.h"
#include "tutorialplayer.h"

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
CTutorialManager *CTutorial::m_pTutorialManager = nullptr;	// チュートリアル管理のポインタ

//=====================================================
// コンストラクタ
//=====================================================
CTutorial::CTutorial()
{

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
	CMeshField *pMeshField = CMeshField::Create();
	CObjectManager *pObjManager = CManager::GetObjectManager();

	CTutorialPlayer *pPlayer = CTutorialPlayer::Create();

	if (pPlayer != nullptr && pObjManager != nullptr)
	{// プレイヤーの適用
		pObjManager->BindPlayer((CPlayer*)pPlayer);
	}

	// ブロック番号読込
	CBlock::LoadModel();

	CBlock::Create(D3DXVECTOR3(2000.0f, 0.0f, 0.0f), CBlock::TYPE_BILL005);

	CBlock::Create(D3DXVECTOR3(-2000.0f, 0.0f, 0.0f), CBlock::TYPE_BILL005);

	CBlock::Create(D3DXVECTOR3(0.0f, 0.0f, 2000.0f), CBlock::TYPE_BILL005);

	// パーティクルの読込
	CParticle::Load();

	// メッシュフィールドの生成
	if (pMeshField != nullptr && pObjManager != nullptr)
	{
		pObjManager->BindMeshField(pMeshField);

		pMeshField->SetIdxTexture(-1);

		pMeshField->SetCol(D3DXCOLOR(0.7f, 0.7f, 1.0f, 0.7f));

		pMeshField->Reset();

		pMeshField->SetNormal();

		pMeshField->EnableWire(true);
	}

	// フォグを消す
	CRenderer::EnableFog(false);

	// チュートリアル管理の生成
	if (m_pTutorialManager == nullptr)
	{
		m_pTutorialManager = CTutorialManager::Create();
	}

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

	if (pos.y < 0)
	{// 床
		pos.y = 0;

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
// 描画処理
//=====================================================
void CTutorial::Draw(void)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	//pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
}