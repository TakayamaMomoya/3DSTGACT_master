//*****************************************************
//
// ���j���[�̏���[menu.h]
// Author:���R����
//
//*****************************************************

#ifndef _MENU_H_
#define _MENU_H_

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
class CMenu : CObject
{
public:
	CMenu();	// �R���X�g���N�^
	~CMenu();	// �f�X�g���N�^

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CMenu *Create(void);

private:
	enum MENU
	{// ���ڂ̎��
		MENU_GAME = 0,	// �ĊJ
		MENU_TUTORIAL,	// �`���[�g���A��
		MENU_MAX
	};

	void Input(void);
	void Fade(MENU menu);

	MENU m_menu;	// �I������

	CObject2D *m_apMenu[MENU_MAX];	// ���j���[���ڂ̂QD�I�u�W�F�N�g
	CObject2D *m_pCaption;	// ���o���̃|�C���^
};

#endif