//*****************************************************
//
// �^�C�g������[title.h]
// Author:���R����
//
//*****************************************************
#ifndef _TITLE_H_
#define _TITLE_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "main.h"
#include "scene.h"

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CTitle : public CScene
{
public:
	CTitle();	// �R���X�g���N�^
	~CTitle();	// �f�X�g���N�^

	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update();
	virtual void Draw();

private:
};

#endif