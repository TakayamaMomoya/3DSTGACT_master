//*****************************************************
//
// エディターの処理[edit.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "edit.h"
#include "objectX.h"
#include "inputkeyboard.h"
#include "manager.h"
#include "debugproc.h"
#include "block.h"

//*****************************************************
// マクロ定義
//*****************************************************
#define SPEED_MOVE	(3.0f)	// 移動速度

//=====================================================
// コンストラクタ
//=====================================================
CEdit::CEdit()
{
	m_pObjectCursor = nullptr;
	m_nIdxObject = 0;
	m_type = CBlock::TYPE_BILL000;
}

//=====================================================
// デストラクタ
//=====================================================
CEdit::~CEdit()
{

}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CEdit::Init(void)
{
	// 番号取得
	int *pIdx = CBlock::GetIndex();

	m_pObjectCursor = nullptr;

	m_pObjectCursor = CObjectX::Create();

	// タイプの初期設定
	m_type = CBlock::TYPE_BILL000;

	// モデル番号の設定
	m_pObjectCursor->SetIdxModel(pIdx[m_type]);
	m_pObjectCursor->BindModel(pIdx[m_type]);

	m_pObjectCursor->SetEmissiveCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CEdit::Uninit(void)
{
	m_pObjectCursor = nullptr;
}

//=====================================================
// 更新処理
//=====================================================
void CEdit::Update(void)
{
	// 番号取得
	int *pIdx = CBlock::GetIndex();

	// 情報取得
	CInputKeyboard *pKeyboard = CManager::GetKeyboard();

	// 変数宣言
	D3DXVECTOR3 pos = { 0.0f,0.0f,0.0f };
	float fSpeed = SPEED_MOVE;
	CBlock **pBlock = CBlock::GetBlock();

	if (m_pObjectCursor != nullptr && pKeyboard != nullptr)
	{
		if (pKeyboard->GetPress(DIK_RSHIFT))
		{
			fSpeed *= 5.0f;
		}

		if (pKeyboard->GetPress(DIK_J))
		{
			pos.x -= fSpeed;
		}
		if (pKeyboard->GetPress(DIK_L))
		{
			pos.x += fSpeed;
		}
		if (pKeyboard->GetPress(DIK_I))
		{
			pos.z += fSpeed;
		}
		if (pKeyboard->GetPress(DIK_K))
		{
			pos.z -= fSpeed;
		}
		if (pKeyboard->GetPress(DIK_U))
		{
			pos.y += fSpeed;
		}
		if (pKeyboard->GetPress(DIK_O))
		{
			pos.y -= fSpeed;
		}

		if (pKeyboard->GetTrigger(DIK_RETURN))
		{// ブロックの生成
			CBlock::Create(m_pObjectCursor->GetPosition(),m_type);
		}

		if (pKeyboard->GetTrigger(DIK_0) && CBlock::GetNumAll() != 0)
		{// オブジェクト選択処理
			if (pBlock[m_nIdxObject] != nullptr)
			{
				pBlock[m_nIdxObject]->SetEmissiveCol(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));
			}

			m_nIdxObject = (m_nIdxObject + 1) % CBlock::GetNumAll();

			if (pBlock[m_nIdxObject] != nullptr)
			{
				pBlock[m_nIdxObject]->SetEmissiveCol(D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.0f));
			}
		}

		if (pKeyboard->GetTrigger(DIK_7))
		{// ブロックタイプ切り替え
			m_type = (CBlock::TYPE)((m_type + 1) % CBlock::TYPE_MAX);

			// モデル番号の設定
			m_pObjectCursor->SetIdxModel(pIdx[m_type]);
			m_pObjectCursor->BindModel(pIdx[m_type]);
		}

		if(pKeyboard->GetTrigger(DIK_9))
		{// 指定のブロックを削除
			CBlock::Delete(m_nIdxObject);
		}

		if (pKeyboard->GetTrigger(DIK_8))
		{// ブロックの保存
			CBlock::Save();
		}

		if (m_pObjectCursor != nullptr)
		{// カーソルの移動
			m_pObjectCursor->SetPosition(m_pObjectCursor->GetPosition() + pos);
		}

		CManager::GetDebugProc()->Print("\n//=======================\n");
		CManager::GetDebugProc()->Print("// エディター\n");
		CManager::GetDebugProc()->Print("//=======================\n");
		CManager::GetDebugProc()->Print("エディターの位置：[%f,%f,%f]\n", m_pObjectCursor->GetPosition().x, m_pObjectCursor->GetPosition().y, m_pObjectCursor->GetPosition().z);
		CManager::GetDebugProc()->Print("移動[IJKL]\n");
		CManager::GetDebugProc()->Print("上下移動[UO]\n");
		CManager::GetDebugProc()->Print("設置[ENTER]\n");
		CManager::GetDebugProc()->Print("保存[8]\n");
		CManager::GetDebugProc()->Print("選択中のブロック：[%d]:[0]\n", m_nIdxObject);
		CManager::GetDebugProc()->Print("選択ブロック削除[9]\n");
		CManager::GetDebugProc()->Print("設置するタイプ：[%d]:[7]\n", m_type);
		CManager::GetDebugProc()->Print("//=======================\n");
	}
}