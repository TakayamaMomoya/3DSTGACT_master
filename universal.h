//*****************************************
//
// 汎用処理セット
// Author:髙山桃也
//
//*****************************************
#ifndef _UNIVERSAL_H_
#define _UNIVERSAL_H_

#include "main.h"

//*****************************************
// クラスの定義
//*****************************************
class CUniversal
{
public:
	CUniversal();	// コンストラクタ
	~CUniversal();	// デストラクタ
	void LimitRot(float *fRot);	//回転制御処理
	void SetOffSet(D3DXMATRIX *pMtxWorldOffset, D3DXMATRIX mtxWorldOwner, D3DXVECTOR3 posOffset, D3DXVECTOR3 rot = { 0.0f,0.0f,0.0f });	//オフセット設定処理
	void FactingRot(float *pfRot, float fRotDest, float rotateFact);	//向き補正処理
	D3DXVECTOR3 LinePridiction(D3DXVECTOR3 pos, float fSpeedBullet, D3DXVECTOR3 posTarget, D3DXVECTOR3 moveTarget);
	float PlusMin(float fData1, float fData2);

private:
};

#endif