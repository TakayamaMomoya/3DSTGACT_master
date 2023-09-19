//*****************************************************
//
// �`���[�g���A������[tutorial.h]
// Author:���R����
//
//*****************************************************
#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "main.h"
#include "scene.h"

//*****************************************************
// �O���錾
//*****************************************************
class CPlayer;
class CMeshField;

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CTutorial : public CScene
{
public:
	enum PROGRESS
	{// �i�s��
		PROGRESS_START,	// �J�n
		PROGRESS_SHOT,	// �ˌ�
		PROGRESS_MAX
	};
	enum ACTION
	{// �A�N�V����
		ACTION_SHOT,	// �ʏ�ˌ�
		ACTION_RAPID,	// �A�ˎˌ�
		ACTION_CHARGE,	// �`���[�W�ˌ�
		ACTION_MAX
	};

	CTutorial();	// �R���X�g���N�^
	~CTutorial();	// �f�X�g���N�^

	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update();
	virtual void Draw();

	void AddProgress(ACTION action);

private:
	void Debug(void);	// �f�o�b�O����
	void ManageState(void);	// ��ԊǗ�
	void LimitPlayerPos(void);	// �v���C���[�ړ�����

	PROGRESS m_progress;	// �i�s��
};

#endif