//*****************************************************
//
// ���U���g�̏���[result.h]
// Author:���R����
//
//*****************************************************

#ifndef _RESULT_H_
#define _RESULT_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "object.h"

//*****************************************************
// �O���錾
//*****************************************************
class CObject2D;

//*****************************************************
// �N���X��`
//*****************************************************
class CResult : CObject
{
public:
	CResult();	// �R���X�g���N�^
	~CResult();	// �f�X�g���N�^

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CResult *Create(bool bWin = false);

private:
	enum MENU
	{// ���ڂ̎��
		MENU_REPLAY = 0,	// ���v���C
		MENU_RANKING,	// �����L���O
		MENU_MAX
	};

	void Input(void);
	void Fade(MENU menu);
	void Create2D(bool bWin);

	MENU m_menu;	// �I������

	CObject2D *m_apMenu[MENU_MAX];	// ���j���[���ڂ̂QD�I�u�W�F�N�g
	CObject2D *m_pBg;	// �w�i�̃|�C���^
	CObject2D *m_pCaption;	// ���o���̃|�C���^
};

#endif