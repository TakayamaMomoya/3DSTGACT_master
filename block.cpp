//*****************************************************
//
// ブロックの処理[block.cpp]
// Author:髙山桃也
//
//*****************************************************

//*****************************************************
// インクルード
//*****************************************************
#include "main.h"
#include "block.h"
#include "renderer.h"
#include "manager.h"
#include "object.h"
#include "collision.h"
#include <stdio.h>

//*****************************************************
// マクロ定義
//*****************************************************
#define SPEED_MOVE	(1.0f)	// 移動速度
#define MAP_FILE	"data\\MAP\\map00.bin"	// マップのファイルパス

//*****************************************************
// 静的メンバ変数宣言
//*****************************************************
CBlock *CBlock::m_apBlock[NUM_OBJECT] = {};	// ブロックの配列
int CBlock::m_nNumAll = 0;	// 総数
int *CBlock::m_pIdxObject = nullptr;	// 番号のポインタ

//=====================================================
// コンストラクタ
//=====================================================
CBlock::CBlock(int nPriority)
{
	m_type = TYPE_BILL000;

	for (int nCntBlock = 0;nCntBlock < NUM_OBJECT;nCntBlock++)
	{
		if (m_apBlock[nCntBlock] == nullptr)
		{// 保存用配列にコピー
			m_apBlock[nCntBlock] = this;

			break;
		}
	}

	m_nNumAll++;
}

//=====================================================
// デストラクタ
//=====================================================
CBlock::~CBlock()
{
	m_nNumAll--;
}

//=====================================================
// 初期化処理
//=====================================================
HRESULT CBlock::Init(void)
{
	// 継承クラスの初期化
	CObjectX::Init();

	// タイプの設定
	SetType(TYPE_BLOCK);

	return S_OK;
}

//=====================================================
// 終了処理
//=====================================================
void CBlock::Uninit(void)
{
	if (m_pCollisionCube != nullptr)
	{// 当たり判定の消去
		m_pCollisionCube->Uninit();

		m_pCollisionCube = nullptr;
	}

	// 継承クラスの終了
	CObjectX::Uninit();
}

//=====================================================
// 更新処理
//=====================================================
void CBlock::Update(void)
{
	// 継承クラスの更新
	CObjectX::Update();
}

//=====================================================
// 描画処理
//=====================================================
void CBlock::Draw(void)
{
	// 継承クラスの描画
	CObjectX::Draw();
}

//=====================================================
// 生成処理
//=====================================================
CBlock *CBlock::Create(D3DXVECTOR3 pos, TYPE type)
{
	CBlock *pBlock = nullptr;

	if (type <= CBlock::TYPE_NONE && type >= CBlock::TYPE_MAX)
	{// 範囲外の種類が渡されたらnullptrを返す
		return nullptr;
	}

	if (pBlock == nullptr)
	{// インスタンス生成
		pBlock = new CBlock;

		pBlock->SetPosition(pos);

		// 初期化処理
		pBlock->m_type = type;
		pBlock->Init();

		// 種類ごとのモデル読込
		pBlock->SetIdxModel(m_pIdxObject[type]);
		pBlock->BindModel(m_pIdxObject[type]);

		if (pBlock->m_pCollisionCube == nullptr)
		{// 当たり判定生成
			pBlock->m_pCollisionCube = CCollisionCube::Create(CCollision::TAG_BLOCK, pBlock);

			if (pBlock->m_pCollisionCube != nullptr)
			{
				pBlock->m_pCollisionCube->SetPosition(pBlock->GetPosition());

				pBlock->m_pCollisionCube->SetVtx(pBlock->GetVtxMax(), pBlock->GetVtxMin());
			}
		}
	}

	return pBlock;
}

//=====================================================
// 部分削除
//=====================================================
void CBlock::Delete(int nIdx)
{
	if (m_apBlock[nIdx] != nullptr)
	{// 削除処理
		m_apBlock[nIdx]->Uninit();

		m_apBlock[nIdx] = nullptr;
	}

	// 配列を詰める
	for (int nCntBlock = nIdx; nCntBlock < NUM_OBJECT - 1; nCntBlock++)
	{
		if (m_apBlock[nCntBlock + 1] != nullptr)
		{
			m_apBlock[nCntBlock] = m_apBlock[nCntBlock + 1];
			m_apBlock[nCntBlock + 1] = nullptr;
		}
	}
}

//=====================================================
// 全削除処理
//=====================================================
void CBlock::DeleteAll(void)
{
	for (int nCntBlock = 0;nCntBlock < NUM_OBJECT;nCntBlock++)
	{
		if (m_apBlock[nCntBlock] != nullptr)
		{
			m_apBlock[nCntBlock] = nullptr;
		}
	}
}

//=====================================================
// 番号削除処理
//=====================================================
void CBlock::DeleteIdx(void)
{
	if (m_pIdxObject != nullptr)
	{// 番号ポインタの破棄
		delete m_pIdxObject;
		m_pIdxObject = nullptr;
	}
}

//=====================================================
// 読込処理
//=====================================================
HRESULT CBlock::Load(void)
{
	int nNumBlock;
	MemBlock memBlock;

	//ポインタ宣言
	FILE *pFile;

	//ファイルを開く
	pFile = fopen(MAP_FILE, "rb");

	if (pFile != nullptr)
	{//ファイルが開けた場合
		
		// 数の読込
		fread(&nNumBlock, sizeof(int), 1, pFile);

		for (int nCntBlock = 0;nCntBlock < nNumBlock;nCntBlock++)
		{
			// ブロックの読込
			fread(&memBlock, sizeof(MemBlock), 1, pFile);

			// ブロックの生成
			Create(memBlock.pos, memBlock.type);
		}

		//ファイルを閉じる
		fclose(pFile);
	}
	else
	{//ファイルが開けなかった場合
	}

	return S_OK;
}

//=====================================================
// モデル番号読込処理
//=====================================================
void CBlock::LoadModel(void)
{
	char *pPath[CBlock::TYPE_MAX] =
	{
		"data\\MODEL\\BLOCK\\bill00.x",	// ビル１
		"data\\MODEL\\BLOCK\\bill01.x",	// ビル２
		"data\\MODEL\\BLOCK\\bill02.x",	// ビル３
		"data\\MODEL\\BLOCK\\bill03.x",	// 何かのタンク
		"data\\MODEL\\BLOCK\\bill04.x",	// コンテナ
		"data\\MODEL\\BLOCK\\base00.x",	// 土台
		"data\\MODEL\\BLOCK\\rader00.x",	// レーダー土台
		"data\\MODEL\\BLOCK\\rader01.x",	// レドーム
	};

	if (m_pIdxObject == nullptr)
	{
		// 番号の生成
		m_pIdxObject = new int[CBlock::TYPE_MAX];

		sizeof(m_pIdxObject);

		// 値の初期化
		ZeroMemory(m_pIdxObject,sizeof(int) * CBlock::TYPE_MAX);

		for (int nCntBlock = 0; nCntBlock < CBlock::TYPE_MAX; nCntBlock++)
		{
			m_pIdxObject[nCntBlock] = CModel::Load(pPath[nCntBlock]);
		}
	}
}

//=====================================================
// 保存処理
//=====================================================
void CBlock::Save(void)
{
	//ポインタ宣言
	FILE *pFile;

	// 情報受け取り用構造体
	MemBlock memBlock;

	//ファイルを開く
	pFile = fopen(MAP_FILE, "wb");

	if (pFile != nullptr)
	{//ファイルが開けた場合

		fwrite(&m_nNumAll, sizeof(int), 1, pFile);

		for (int nCntBlock = 0; nCntBlock < m_nNumAll; nCntBlock++)
		{
			if (m_apBlock[nCntBlock] != nullptr)
			{
				memBlock.pos = m_apBlock[nCntBlock]->GetPosition();
				memBlock.rot = m_apBlock[nCntBlock]->GetRot();
				memBlock.type = m_apBlock[nCntBlock]->m_type;

				//バイナリファイルに書き込む
				fwrite(&memBlock, sizeof(MemBlock), 1, pFile);
			}
		}

		//ファイルを閉じる
		fclose(pFile);
	}
	else
	{//ファイルが開けなかった場合

	}
}