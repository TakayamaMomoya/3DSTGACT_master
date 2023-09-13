//*****************************************************
//
// �o�̏���[dust.h]
// Author:���R����
//
//*****************************************************

#ifndef _DUST_H_
#define _DUST_H_

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "billboard.h"

//*****************************************************
// �N���X�̒�`
//*****************************************************
class CDust : public CBillboard
{
public:
	CDust(int nPriority = 6);	// �R���X�g���N�^
	~CDust();	// �f�X�g���N�^

	static CDust *Create(D3DXVECTOR3 pos);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

private:
	enum STATE
	{
		STATE_NONE = 0,	// ���ł��Ȃ����
		STATE_IN,	// �o��
		STATE_OUT,	// ��������
		STATE_MAX
	};

	void SetMove(void);

	float m_fAlpha;	// ���l�̕ω���
	STATE m_state;	// ���
	D3DXVECTOR3 m_move;	// �ړ���
};

#endif