//*****************************************************
//
// �|�[�Y�̏���[pause.h]
// Author:���R����
//
//*****************************************************

#ifndef _PAUSE_H_
#define _PAUSE_H_

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
class CPause : CObject
{
public:
	CPause();	// �R���X�g���N�^
	~CPause();	// �f�X�g���N�^

	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CPause *Create(void);

private:
	enum MENU
	{// ���ڂ̎��
		MENU_RESUME = 0,	// �ĊJ
		MENU_RETRY,	// ���g���C
		MENU_QUIT,	// �I��
		MENU_MAX
	};

	void Input(void);
	void Fade(MENU menu);

	MENU m_menu;	// �I������

	CObject2D *m_apMenu[MENU_MAX];	// ���j���[���ڂ̂QD�I�u�W�F�N�g
	CObject2D *m_pBg;	// �w�i�̃|�C���^
};

#endif