//*****************************************************
//
// �v���C���[�̏���[player.cpp]
// Author:���R����
//
//*****************************************************

//*****************************************************
// �C���N���[�h
//*****************************************************
#include "main.h"
#include "player.h"
#include "renderer.h"
#include "manager.h"
#include "inputkeyboard.h"
#include "inputmouse.h"
#include "inputjoypad.h"
#include "bullet.h"
#include "enemy.h"
#include "debugproc.h"
#include "particle.h"
#include "sound.h"
#include "collision.h"
#include "model.h"
#include "texture.h"
#include "shadow.h"
#include "camera.h"
#include "collision.h"
#include "meshfield.h"
#include "universal.h"
#include "effect3D.h"
#include "boost.h"
#include "orbit.h"
#include "game.h"
#include "cockpit.h"
#include "rader.h"
#include "score.h"
#include <stdio.h>
#include "bonus.h"
#include "assess.h"
#include "lockon.h"
#include "texture.h"
#include "noise.h"
#include "explosionspawner.h"
#include "smokespawner.h"
#include "debrisspawner.h"
#include "objectmanager.h"
#include "result.h"

//*****************************************************
// �}�N����`
//*****************************************************
#define SPEED_MOVE	(2.0f)	// �ړ����x
#define RAPID_SPEED	(35.0f)	// �A�ˑ��x
#define RAPID_MAX	(RAPID_SPEED * 0.8f)	// �A�ˍő呬�x�̔{��
#define SHOT_COST	(50.0f)	// �ʏ�ˌ��̃u�[�X�g�R�X�g
#define COST_MAX	(SHOT_COST * 0.8f)	// �R�X�g�ő�����̔{��
#define CHARGE_COST	(SHOT_COST * 1.5f)	// �`���[�W�V���b�g�̃R�X�g
#define CHARGE_DAMAGE	(10.0f)	// �`���[�W�V���b�g�̈З�
#define NORMAL_DAMAGE	(5.0f)	// �ʏ�V���b�g�̈З�
#define ROLL_SPEED	(0.05f)	// ���_���쑬�x
#define RAPID_TIME	(10)	// �A�˃��[�h�ɂ���P�\
#define SPEED_SPRINT	(0.4f)	// �_�b�V�����̈ړ��{��
#define GRAVITY	(0.98f)	// �d��
#define JUMP_POW	(1.2f)	// �W�����v��
#define JUMP_START_POW	(5.0f)	// �����̃W�����v��
#define COST_JUMP	(1.5f)	// �W�����v�̃R�X�g
#define RGN_BOOST	(1.5f)	// �u�[�X�g�̉񕜗�
#define COST_SPRINT	(1.5f)	// �_�b�V���̃R�X�g
#define BULLET_SPEED	(90.0f)	// �e�̑��x
#define BULLET_LIFE		(120)	// �e�̎���
#define AIM_LENGTH	(370.0f)	// �G�C�����̃J�����̋���
#define NOT_AIM_LENGTH	(700.0f)	// ��G�C�����̃J�����̋���
#define TURN_DOWNER	(D3DX_PI * 0.55f)	// �����g�����]����܂ł̍���
#define TURN_ROT	(D3DX_PI * 0.45f)	// �����g�����]����܂ł̍���
#define LOCKON_ANGLE	(D3DX_PI * 0.13f)	// ���b�N�I���̏㉺�p�x
#define LOCKON_RANGE	(3000.0f)	// ���b�N�I������
#define AIM_LOCKON_RANGE	(LOCKON_RANGE * 1.3f)	// �G�C�����̃��b�N�I������
#define CHARGE_TIMER	(40.0f)	// �`���[�W�V���b�g�����Ă�܂ł̃J�E���^
#define BULLET_RADIUS	(30.0f)	// �ʏ�e�̑傫��
#define CHARGE_RADIUS	(300.0f)	// �`���[�W�V���b�g�̊�̑傫��
#define MOVE_FACT	(0.8f)	// �ʏ�̈ړ�����
#define SPRINT_FACT	(0.96f)	// �_�b�V���̈ړ�����
#define RADIUS_SPHERE	(100)	// ���̓����蔻��̑傫��
#define HIT_ADD	(0.025f)	// �������̕\��
#define DAMAGE_PENALTY	(0.05f)	// �_���[�W���̕]���y�i���e�B
#define LIMIT_UPPERROT	(0.5f)	// �㔼�g�̊p�x����
#define LIMIT_CAMERAROT	(0.8f)	// �J�����̊p�x����
#define TIME_DEATH	(240)	// ���ʂ܂ł̃t���[����

//=====================================================
// �D�揇�ʂ����߂�R���X�g���N�^
//=====================================================
CPlayer::CPlayer(int nPriority)
{
	m_nLife = 0;
	m_nCntDeath = 0;
	m_fBoost = 0.0f;
	m_nTimerRapid = 0;
	m_nTimerShot = 0;
	m_nNumShot = 0;
	m_nNumHit = 0;
	m_posLockon = { 0.0f,0.0f,0.0f };
	m_posBoost = { 0.0f,0.0f,0.0f };
	m_vecBoost = { 0.0f,0.0f,0.0f };
	m_pos = { 0.0f,0.0f,0.0f };
	m_posOld = { 0.0f,0.0f,0.0f };
	m_rot = { 0.0f,0.0f,0.0f };
	m_rotDest = { 0.0f,0.0f,0.0f };
	m_rotUpper= { 0.0f,0.0f,0.0f };
	m_rotDowner = { 0.0f,0.0f,0.0f };
	m_move = { 0.0f,0.0f,0.0f };
	m_fParamCost = 0.0f;
	m_fParamPower = 0.0f;
	m_fParamRapid = 0.0f;
	m_state = STATE_NONE;
	m_bAim = false;
	m_bLand = false;
	m_bSprint = false;
	m_bTurnDowner = false;
	m_pShadow = nullptr;
	m_pCollisionSphere = nullptr;
	m_pCollisionCube = nullptr;
	m_pBodyDowner = nullptr;
	m_pBodyUpper = nullptr;
	m_pEnemy = nullptr;
	m_pCockpit = nullptr;
	m_pRader = nullptr;
	m_pAssess = nullptr;
	m_pLockon = nullptr;
	m_nCounterAir = 0;
	ZeroMemory(&m_mtxMazzle[0], sizeof(m_mtxMazzle));
	ZeroMemory(&m_offsetMazzle[0], sizeof(m_offsetMazzle));
	ZeroMemory(&m_mtxLockon[0], sizeof(m_mtxLockon));
	ZeroMemory(&m_offsetLockon[0], sizeof(m_offsetLockon));
	ZeroMemory(&m_mtxWaist,sizeof(m_mtxWaist));
}

//=====================================================
// �f�X�g���N�^
//=====================================================
CPlayer::~CPlayer()
{

}

//=====================================================
// ����������
//=====================================================
HRESULT CPlayer::Init(void)
{
	// �J�[�\�����\���ɂ���
	ShowCursor(false);

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �l�̏�����
	m_fBoost = MAX_BOOST;
	SetPosition(D3DXVECTOR3(-500.0f, 1.0f, 1.0f));
	m_move = { 1.0f,2.0f,1.0f };

	// �e�̐���
	if (m_pShadow == nullptr)
	{
		m_pShadow = CShadow::Create(D3DXVECTOR3(GetPosition().x, 1.0f, GetPosition().z), 150.0f, 150.0f);

		if (m_pShadow != nullptr)
		{
			m_pShadow->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
		}
	}

	if (m_pBodyDowner == nullptr)
	{// �����g�̐���
		m_pBodyDowner = CMotion::Create("data\\MOTION\\motionArms01Down.txt");
	}

	if (m_pBodyUpper == nullptr)
	{// �㔼�g�̐���
		m_pBodyUpper = CMotion::Create("data\\MOTION\\motionArms01Up.txt");
	}
	
	// �^�C�v�̐ݒ�
	SetType(TYPE_PLAYER);

	if (m_pCollisionSphere == nullptr)
	{// ���̓����蔻�萶��
		m_pCollisionSphere = CCollisionSphere::Create(CCollision::TAG_PLAYER, CCollision::TYPE_SPHERE,this);

		float fRadius;

		if (m_pCollisionSphere != nullptr)
		{// ���̐ݒ�
			fRadius = RADIUS_SPHERE;

			m_pCollisionSphere->SetPosition(GetPosition());
			m_pCollisionSphere->SetRadius(fRadius);
		}
	}

	if (m_pCollisionCube == nullptr)
	{// �����̂̓����蔻��
		m_pCollisionCube = CCollisionCube::Create(CCollision::TAG_PLAYER,this);

		D3DXVECTOR3 vtxMax = { 100.0f,300.0f,100.0f };
		D3DXVECTOR3 vtxMin = { -100.0f,0.0f,-100.0f };

		if (m_pCollisionCube != nullptr)
		{
			m_pCollisionCube->SetVtx(vtxMax, vtxMin);
		}
	}

	// �e���̃I�t�Z�b�g�ݒ�
	m_offsetMazzle[0] = { 0.0f,-150.0f,-30.0f };
	m_offsetMazzle[1] = { 0.0f, 1.0f, 0.0f};

	// ���b�N�I���̃I�t�Z�b�g�ݒ�
	m_offsetLockon[0] = { 100.0f,0.0f,-200.0f };
	m_offsetLockon[1] = { -100.0f,0.0f,-200.0f };

	m_boostState = BOOSTSTATE_NONE;
	m_state = STATE_NORMAL;

	// �Q�[�W�̐���
	CBoost::Create();

	// �]���p�����[�^�[�̐���
	m_pAssess = CAssess::Create();

	// �R�b�N�s�b�g�̐���
	m_pCockpit = CCockPit::Create();

	// ���[�_�[�̐���
	m_pRader = CRader::Create();

	// ���b�N�I������
	m_pLockon = CLockon::Create();

	// �p�����[�^�[�Ǎ�
	Load();

	m_nLife = INITIAL_LIFE_PLAYER;

	return S_OK;
}

//=====================================================
// �I������
//=====================================================
void CPlayer::Uninit(void)
{
	if (m_pCollisionSphere != nullptr)
	{// �����蔻��̏���
		m_pCollisionSphere->Uninit();

		m_pCollisionSphere = nullptr;
	}

	if (m_pCollisionCube != nullptr)
	{// �����蔻��̏���
		m_pCollisionCube->Uninit();

		m_pCollisionCube = nullptr;
	}

	if (m_pShadow != nullptr)
	{// �e�̍폜
		m_pShadow->Uninit();

		m_pShadow = nullptr;
	}

	if (m_pBodyUpper != nullptr)
	{// �㔼�g�̏I��
		m_pBodyUpper->Uninit();

		m_pBodyUpper = nullptr;
	}

	if (m_pBodyDowner != nullptr)
	{// �����g�̏I��
		m_pBodyDowner->Uninit();

		m_pBodyDowner = nullptr;
	}

	if (m_pBodyDowner != nullptr)
	{// �����g�̏I��
		m_pBodyDowner->Uninit();

		m_pBodyDowner = nullptr;
	}

	if (m_pCockpit != nullptr)
	{// �R�b�N�s�b�g�̏I��
		m_pCockpit->Uninit();

		m_pCockpit = nullptr;
	}

	if (m_pRader != nullptr)
	{// ���[�_�[�̏I��
		m_pRader->Uninit();

		m_pRader = nullptr;
	}

	if (m_pLockon != nullptr)
	{// ���b�N�I���̏I��
		m_pLockon->Uninit();

		m_pLockon = nullptr;
	}

	if (m_pAssess != nullptr)
	{// �]���̏I��
		m_pAssess->Uninit();

		m_pAssess = nullptr;
	}
	
	// �v���C���[�̏I��
	CObjectManager *pObjManager = CManager::GetObjectManager();

	if (pObjManager != nullptr)
	{// �v���C���[�̓K�p
		pObjManager->ReleasePlayer();
	}

	// ���g�̔j��
	Release();
}

//=====================================================
// �X�V����
//=====================================================
void CPlayer::Update(void)
{
	// �ϐ��錾
	float fMoveFact = MOVE_FACT;

	// �J�����擾
	CCamera *pCamera = CManager::GetCamera();

	// �}�E�X�J�[�\������ʒ��S�ɌŒ肷��
	SetCursorPos((int)(SCREEN_WIDTH * 0.5f), (int)(SCREEN_HEIGHT * 0.5f));

	// ���b�N�I������
	Lockon();

	if (m_bSprint == false)
	{
		// �d�͂ɂ�闎��
		SetMove(D3DXVECTOR3(GetMove().x, GetMove().y - GRAVITY, GetMove().z));
	}
	else
	{
		// �����̖�����
		SetMove(D3DXVECTOR3(GetMove().x, GetMove().y + (0 - GetMove().y) * 0.2f, GetMove().z));

		// �u�[�X�g�̐ݒ�
		Boost();
	}

	// �I�t�Z�b�g�Ǘ�
	ManageOffset();

	// ��ԊǗ�
	ManageState();

	// �ʒu��ۑ�
	SetPositionOld(GetPosition());

	// �u�[�X�g�ʂ�ۑ�
	float fBoost = m_fBoost;

	if (m_state != STATE_DEATH)
	{// ���S���͔��f���Ȃ�����
		// ���쏈��
		Input();

		// ���[�V�����̊Ǘ�
		ManageMotion();
	}

	// �u�[�X�g�Ǘ�
	ManageBoost(fBoost);

	if (m_bSprint || m_bLand == false)
	{
		fMoveFact = SPRINT_FACT;
	}

	// �ʒu���Z
	SetPosition(GetPosition() + GetMove());

	// �ړ��ʌ���
	SetMove(D3DXVECTOR3(GetMove().x * fMoveFact, GetMove().y, GetMove().z * fMoveFact));

	// �����蔻��Ǘ�
	ManageCollision();
}

//=====================================================
// �u�[�X�g�Ǘ�
//=====================================================
void CPlayer::ManageBoost(float fBoostOld)
{
	if (m_fBoost == fBoostOld)
	{// �u�[�X�g������Ȃ��������
		m_fBoost += RGN_BOOST;

		if (m_fBoost > MAX_BOOST)
		{
			m_fBoost = MAX_BOOST;
		}
	}

	if (m_fBoost < 0)
	{// �u�[�X�g�̌��R
		m_fBoost = 0.0f;

		m_boostState = BOOSTSTATE_EMPTY;

		m_bSprint = false;
	}

	if (m_boostState == BOOSTSTATE_EMPTY)
	{// �u�[�X�g���R����̕��A
		if (m_fBoost > MAX_BOOST * 0.5f)
		{
			m_boostState = BOOSTSTATE_NONE;
		}
	}
}

//=====================================================
// �I�t�Z�b�g�̊Ǘ�
//=====================================================
void CPlayer::ManageOffset(void)
{
	// �ėp�����̎擾
	CUniversal *pUniversal = CManager::GetUniversal();

	// �e���̃I�t�Z�b�g�ʒu��ݒ�
	CParts *pParts = nullptr;

	if (m_pBodyUpper != nullptr)
	{
		pParts = m_pBodyUpper->GetParts(8)->m_pParts;
	}

	if (pParts != nullptr)
	{
		pUniversal->SetOffSet(&m_mtxMazzle[0], *pParts->GetMatrix(), m_offsetMazzle[0]);
		pUniversal->SetOffSet(&m_mtxMazzle[1], m_mtxMazzle[0], m_offsetMazzle[1]);

		m_posMazzle = { m_mtxMazzle[0]._41, m_mtxMazzle[0]._42, m_mtxMazzle[0]._43 };
	}

	// ���b�N�I���̃I�t�Z�b�g�ݒ�
	pUniversal->SetOffSet(&m_mtxLockon[0], *m_pBodyUpper->GetMatrix(), m_offsetLockon[0]);
	pUniversal->SetOffSet(&m_mtxLockon[1], *m_pBodyUpper->GetMatrix(), m_offsetLockon[1]);
}

//=====================================================
// �����蔻��Ǘ�
//=====================================================
void CPlayer::ManageCollision(void)
{
	bool bLandMesh;
	bool bLandBlock = false;

	// �X�R�A�擾
	CScore *pScore = CGame::GetScore();

	// ���b�V���t�B�[���h�Ƃ̔���
	bLandMesh = CollisionField();

	if (m_pCollisionCube != nullptr)
	{// �����蔻��̊Ǘ�

		m_pCollisionCube->SetPosition(GetPosition());

		D3DXVECTOR3 vtxMax = { 100.0f,300.0f,100.0f };
		D3DXVECTOR3 vtxMin = { -100.0f,0.0f,-100.0f };

		m_pCollisionCube->SetVtx(vtxMax, vtxMin);

		D3DXVECTOR3 move = GetMove();

		// �����o���̓����蔻��
		bLandBlock = m_pCollisionCube->CubeCollision(CCollision::TAG_BLOCK, &move);

		SetMove(move);
	}

	if (m_pCollisionSphere != nullptr)
	{// ���̓����蔻��̊Ǘ�
		// ���̈ʒu�ɐݒ�
		D3DXVECTOR3 pos = { m_mtxWaist._41,m_mtxWaist._42 ,m_mtxWaist._43 };

		m_pCollisionSphere->SetPositionOld(m_pCollisionSphere->GetPosition());

		m_pCollisionSphere->SetPosition(pos);

		m_pCollisionSphere->SetRadius(RADIUS_SPHERE * 4);

		if (m_pCollisionSphere->IsTriggerExit(CCollision::TAG_ENEMYBULLET))
		{// ���ߒe����̔���
			CBonus::Create(CBonus::TYPE_DOGDE);
		}

		if (m_pCollisionSphere->IsTriggerExit(CCollision::TAG_MISSILE))
		{// ���ߒe����̔���
			CBonus::Create(CBonus::TYPE_DOGDE);
		}

		m_pCollisionSphere->SetRadius(RADIUS_SPHERE);
	}

	if (bLandBlock || bLandMesh)
	{// ���n�������ЂƂN���A���Ă���Β��n��Ԃɂ���
		m_bLand = true;

		m_nCounterAir = 0;
	}
	else
	{
		m_bLand = false;
	}
}

//=====================================================
// ���b�V���t�B�[���h�Ƃ̔���
//=====================================================
bool CPlayer::CollisionField(void)
{
	bool bLandMesh = false;

	// ���b�V���t�B�[���h�Ƃ̓����蔻��
	D3DXVECTOR3 move = GetMove();
	D3DXVECTOR3 nor;
	float fHeight = 0.0f;

	CMeshField *pMeshFiled = nullptr;
	CObjectManager *pObjManager = CManager::GetObjectManager();

	if (pObjManager != nullptr)
	{// ���b�V���t�B�[���h�擾
		pMeshFiled = pObjManager->GetMeshField();
	}

	if (m_pShadow != nullptr)
	{
		// �e�̈ʒu�ݒ�
		m_pShadow->SetPosition(D3DXVECTOR3(GetPosition().x, m_pShadow->GetPosition().y, GetPosition().z));
	}

	if (pMeshFiled != nullptr)
	{// ���b�V���Ƃ̓����蔻��
		fHeight = pMeshFiled->GetHeight(GetPosition(), &move, nullptr);

		if (fHeight >= GetPosition().y)
		{// �v���C���[�̈ʒu�ݒ�
			SetPosition(D3DXVECTOR3(GetPosition().x, fHeight, GetPosition().z));

			SetMove(move);

			move = GetMove();

			if (move.y <= 0.0f)
			{
				SetMove(D3DXVECTOR3(move.x, 0.0f, move.z));
			}

			// ���n��Ԃɂ���
			bLandMesh = true;
		}
	}

	return bLandMesh;
}

//=====================================================
// ���쏈��
//=====================================================
void CPlayer::Input(void)
{
	if (CGame::GetState() != STATE_NORMAL)
	{
		return;
	}

	// ���͏�����
	CInputKeyboard *pKeyboard = CManager::GetKeyboard();

	// �ړ�����
	InputMove();

	// �ˌ�����
	InputShot();

	// �J��������
	InputCamera();

#ifdef _DEBUG
	if (pKeyboard->GetPress(DIK_B))
	{// �{�[�i�X�\��
		CBonus::Create(CBonus::TYPE_DOGDE);
		CBonus::Create(CBonus::TYPE_DOGDE);
	}

	if (pKeyboard->GetTrigger(DIK_B))
	{// �{�[�i�X�\��
		CBonus::Create(CBonus::TYPE_MISSILE);
		CBonus::Create(CBonus::TYPE_MISSILE);
	}

	if (pKeyboard->GetTrigger(DIK_F4))
	{// �p�����[�^�[�J���X�g
		m_fParamCost = 1.0f;
		m_fParamPower = 1.0f;
		m_fParamRapid = 1.0f;
	}

	if (pKeyboard->GetTrigger(DIK_F5))
	{// ���S
		Hit(100000);
	}

	if (pKeyboard->GetTrigger(DIK_F6))
	{// �G�X�|�[��
		CEnemy::Create(GetPosition(), CEnemy::TYPE_MOVE);
	}

	if (pKeyboard->GetTrigger(DIK_B))
	{// �{�[�i�X�\��
		CBonus::Create(CBonus::TYPE_DOGDE);
	}
#endif
}

//=====================================================
// �ړ�����
//=====================================================
void CPlayer::InputMove(void)
{
	// ������
	CInputKeyboard *pKeyboard = CManager::GetKeyboard();
	CInputMouse *pMouse = CManager::GetMouse();
	CInputJoypad *pJoypad = CManager::GetJoypad();
	CCamera *pCamera = CManager::GetCamera();
	CCamera::Camera *pCameraInfo = CManager::GetCamera()->GetCamera();

	// �ϐ��錾
	D3DXVECTOR3 move = { 0.0f,0.0f,0.0f }, rot = { 0.0f,0.0f,0.0f };
	D3DXVECTOR3 vecStick;
	float fRot;

	if (pKeyboard->GetPress(DIK_A))
	{// ���ړ�
		move.x += sinf(pCameraInfo->rot.y + D3DX_PI * 0.5f) * SPEED_MOVE;
		move.z += cosf(pCameraInfo->rot.y + D3DX_PI * 0.5f) * SPEED_MOVE;
	}
	if (pKeyboard->GetPress(DIK_D))
	{// �E�ړ�
		move.x += sinf(pCameraInfo->rot.y - D3DX_PI * 0.5f) * SPEED_MOVE;
		move.z += cosf(pCameraInfo->rot.y - D3DX_PI * 0.5f) * SPEED_MOVE;
	}
	if (pKeyboard->GetPress(DIK_W))
	{//�@���ړ�
		move.x += sinf(pCameraInfo->rot.y + D3DX_PI) * SPEED_MOVE;
		move.z += cosf(pCameraInfo->rot.y + D3DX_PI) * SPEED_MOVE;
	}
	if (pKeyboard->GetPress(DIK_S))
	{// ��O�ړ�
		move.x += sinf(pCameraInfo->rot.y) * SPEED_MOVE;
		move.z += cosf(pCameraInfo->rot.y) * SPEED_MOVE;
	}

	// �X�e�B�b�N�ړ�
	vecStick = { pJoypad->GetVecStickL().x ,pJoypad->GetVecStickL().y,0.0f };

	if (D3DXVec3Length(&vecStick) != 0)
	{
		// �X�e�B�b�N�̊p�x�擾
		fRot = atan2f(pJoypad->GetVecStickL().x, pJoypad->GetVecStickL().y);

		move.x -= sinf(fRot + pCameraInfo->rot.y) * SPEED_MOVE;
		move.z -= cosf(fRot + pCameraInfo->rot.y) * SPEED_MOVE;
	}

	// �ړ��ʂ̐��K��
	D3DXVec3Normalize(&move, &move);

	if (m_bTurnDowner)
	{// �������Ȃ�x��
		move *= SPEED_MOVE * 0.5f;
	}
	else
	{
		move *= SPEED_MOVE;
	}

	// �ړ���������������
	Turn(move, 0.0f);

	if (m_boostState != BOOSTSTATE_EMPTY)
	{
		if (D3DXVec3Length(&move) != 0)
		{
			if (pKeyboard->GetPress(DIK_LSHIFT)/* || pJoypad->GetPress(CInputJoypad::PADBUTTONS_LB, 0)*/)
			{// �u�[�X�g�ړ�
				if (m_bTurnDowner)
				{
					move.x += sinf(GetRot().y) * SPEED_MOVE * SPEED_SPRINT;
					move.z += cosf(GetRot().y) * SPEED_MOVE * SPEED_SPRINT;

					if (m_pBodyDowner->GetMotion() != MOTION_TURNBOOST)
					{// �u�[�X�g���[�V�����ֈڍs
						m_pBodyDowner->SetMotion(MOTION_TURNBOOST);
					}
				}
				else
				{
					move.x += sinf(GetRot().y + D3DX_PI) * SPEED_MOVE * SPEED_SPRINT;
					move.z += cosf(GetRot().y + D3DX_PI) * SPEED_MOVE * SPEED_SPRINT;

					if (m_pBodyDowner->GetMotion() != MOTION_BOOST)
					{// �u�[�X�g���[�V�����ֈڍs
						m_pBodyDowner->SetMotion(MOTION_BOOST);
					}
				}

				// �J�����h��ݒ�
				pCamera->SetQuake(0.005f, 0.005f, 2);

				// �u�[�X�g����
				m_fBoost -= COST_SPRINT;

				// �_�b�V����ԂɈڍs
				m_bSprint = true;
			}
			else
			{
				// �_�b�V����ԉ���
				m_bSprint = false;
			}
		}
		else
		{
			// �_�b�V����ԉ���
			m_bSprint = false;

			if (m_pBodyDowner->GetMotion() == MOTION_BOOST || m_pBodyDowner->GetMotion() == MOTION_TURNBOOST)
			{// �j���[�g�������[�V�����ֈڍs
				m_pBodyDowner->SetMotion(MOTION_NEUTRAL);
			}
		}

		if ((pKeyboard->GetTrigger(DIK_SPACE) || pJoypad->GetTrigger(CInputJoypad::PADBUTTONS_LB, 0)) && m_bLand)
		{// �W�����v����
			move.y += JUMP_START_POW;
		}

		if (pKeyboard->GetPress(DIK_SPACE) || pJoypad->GetPress(CInputJoypad::PADBUTTONS_LB, 0))
		{// �u�[�X�g�W�����v
			move.y += JUMP_POW;
			if (m_pBodyDowner->GetMotion() != MOTION_BOOST)
			{// �u�[�X�g���[�V�����ֈڍs
				m_pBodyDowner->SetMotion(MOTION_BOOST);
			}

			// �u�[�X�g�̐ݒ�
			Boost();
			
			m_fBoost -= COST_JUMP;

			m_bJump = true;
		}
		else
		{
			m_bJump = false;
		}
	}
	else
	{
		if (m_pBodyDowner->GetMotion() == MOTION_BOOST || m_pBodyDowner->GetMotion() == MOTION_TURNBOOST)
		{// �j���[�g�������[�V�����ֈڍs
			m_pBodyDowner->SetMotion(MOTION_NEUTRAL);
		}

		m_bSprint = false;
	}

	if (pKeyboard->GetRelease(DIK_LSHIFT) || pJoypad->GetRelease(CInputJoypad::PADBUTTONS_B, 0))
	{// �u�[�X�g�{�^���𗣂�����
		if (m_pBodyDowner->GetMotion() == MOTION_BOOST || m_pBodyDowner->GetMotion() == MOTION_TURNBOOST)
		{// �j���[�g�������[�V�����ֈڍs
			m_pBodyDowner->SetMotion(MOTION_NEUTRAL);
		}
	}

	if (pKeyboard->GetRelease(DIK_SPACE) || m_boostState == BOOSTSTATE_EMPTY || pJoypad->GetRelease(CInputJoypad::PADBUTTONS_LB, 0))
	{// �W�����v�{�^���𗣂�����
		if (m_pBodyDowner->GetMotion() == MOTION_BOOST || m_pBodyDowner->GetMotion() == MOTION_TURNBOOST)
		{// �j���[�g�������[�V�����ֈڍs
			m_pBodyDowner->SetMotion(MOTION_NEUTRAL);
		}
	}

	// �ړ��ʉ��Z
	SetMove(GetMove() + move);
}

//=====================================================
// �ˌ�����
//=====================================================
void CPlayer::InputShot(void)
{
	CInputKeyboard *pKeyboard = CManager::GetKeyboard();
	CInputMouse *pMouse = CManager::GetMouse();
	CInputJoypad *pJoypad = CManager::GetJoypad();
	float fRapid, fCost;
	CCamera *pCamera = CManager::GetCamera();
	CBullet *pBullet = nullptr;

	// �e���̈ʒu�A�������擾
	D3DXVECTOR3 posMazzle = { m_mtxMazzle[0]._41,m_mtxMazzle[0]._42 ,m_mtxMazzle[0]._43 };
	D3DXVECTOR3 DestMazzle = { m_mtxMazzle[1]._41,m_mtxMazzle[1]._42 ,m_mtxMazzle[1]._43 };
	D3DXVECTOR3 vecMazzle = posMazzle - DestMazzle;

	// �p�����[�^�[�̔{���ݒ�
	fRapid = RAPID_SPEED - RAPID_MAX * m_fParamRapid;
	fCost = SHOT_COST - COST_MAX * m_fParamCost;

	if (m_pEnemy != nullptr)
	{
		if (m_bAim)
		{// �΍������̌v�Z
			m_posLockon = LinePridiction(posMazzle, BULLET_SPEED, m_pEnemy->GetPosition(), m_pEnemy->GetMove());
		}

		m_posLockon.y += m_pEnemy->GetRadiusMax() * 0.5f;

		vecMazzle = m_posLockon - posMazzle;
	}

	D3DXVec3Normalize(&vecMazzle, &vecMazzle);

	if (m_boostState != BOOSTSTATE_EMPTY)
	{// �u�[�X�g�c�ʂ��Ȃ��Ǝˌ����ł��Ȃ�
		if (m_nTimerRapid <= 0)
		{
			if (pMouse->GetRelease(CInputMouse::BUTTON_LMB) || pJoypad->GetRelease(CInputJoypad::PADBUTTONS_RB, 0))
			{
				if ((pMouse->GetRepeat(CInputMouse::BUTTON_LMB) >= CHARGE_TIMER && pMouse->GetRelease(CInputMouse::BUTTON_LMB)) ||
					(pJoypad->GetRepeat(CInputJoypad::PADBUTTONS_RB, 0) >= CHARGE_TIMER && pJoypad->GetRelease(CInputJoypad::PADBUTTONS_RB, 0)))
				{// �^�������̒e����
					// �e�̔���
					pBullet = CBullet::Create
					(
						posMazzle,
						vecMazzle * BULLET_SPEED,
						BULLET_LIFE,
						CBullet::TYPE_PLAYER,
						true,
						CHARGE_RADIUS * m_fParamPower,
						CHARGE_DAMAGE
					);

					// �G�l���M�[����
					m_fBoost -= CHARGE_COST;

					if (m_pBodyUpper != nullptr)
					{// �ˌ����[�V����
						m_pBodyUpper->SetMotion(2);
					}

					// �U���͐���
					m_fParamPower += m_fGrawPower;

					if (m_fParamPower > 1.0f)
					{// �p�����[�^�[�␳
						m_fParamPower = 1.0f;
					}

					// �J�����h��ݒ�
					pCamera->SetQuake(0.025f, 0.025f, 15);

					CParticle::Create(m_posMazzle, CParticle::TYPE_SHOT, D3DXVECTOR3(0.0f, 0.0f, 0.0f), &m_posMazzle);

					// SE�Đ�
					CManager::GetSound()->Play(CSound::LABEL_SHOT);
				}
				else
				{
					if (m_nTimerShot == 0)
					{// �P������

						if (m_pBodyUpper != nullptr)
						{// �ˌ����[�V����
							m_pBodyUpper->SetMotion(2);
						}

						// �e�̔���
						pBullet = CBullet::Create
						(
							posMazzle,
							vecMazzle * BULLET_SPEED,
							BULLET_LIFE,
							CBullet::TYPE_PLAYER,
							false,
							BULLET_RADIUS,
							NORMAL_DAMAGE
						);

						// ���˃J�E���^�[�ݒ�
						m_nTimerShot = (int)fRapid;

						// �G�l���M�[����
						m_fBoost -= fCost;

						// ��������
						m_fParamCost += m_fGrawCost;

						if (m_fParamCost > 1.0f)
						{// �p�����[�^�[�␳
							m_fParamCost = 1.0f;
						}

						// �J�����h��ݒ�
						pCamera->SetQuake(0.01f, 0.01f, 15);

						CParticle::Create(m_posMazzle, CParticle::TYPE_SHOT, D3DXVECTOR3(0.0f, 0.0f, 0.0f), &m_posMazzle);

						// SE�Đ�
						CManager::GetSound()->Play(CSound::LABEL_SHOT);
					}
				}

				// ���˂ł��Ȃ������ꍇ�J�E���^�����Z�b�g
				m_nTimerRapid = (int)fRapid;
			}
			
			if ((pMouse->GetPress(CInputMouse::BUTTON_LMB)
				&& pMouse->GetRepeat(CInputMouse::BUTTON_LMB) > RAPID_TIME) ||
				(pJoypad->GetPress(CInputJoypad::PADBUTTONS_RB, 0)
					&& pJoypad->GetRepeat(CInputJoypad::PADBUTTONS_RB, 0) > RAPID_TIME))
			{
				if (pMouse->GetRepeat(CInputMouse::BUTTON_LMB) < CHARGE_TIMER &&
					pJoypad->GetRepeat(CInputJoypad::PADBUTTONS_RB, 0) < CHARGE_TIMER)
				{// �`���[�W���p�[�e�B�N��
					CParticle::Create(m_posMazzle, CParticle::TYPE_CHARGE, D3DXVECTOR3(0.0f, 0.0f, 0.0f), &m_posMazzle);
				}
				else
				{// �}�b�N�X�`���[�W�̃p�[�e�B�N��
					CParticle::Create(m_posMazzle, CParticle::TYPE_MAXCHARGE, D3DXVECTOR3(0.0f, 0.0f, 0.0f), &m_posMazzle);
				}
			}
		}
		else
		{
			if (pMouse->GetPress(CInputMouse::BUTTON_LMB) || pJoypad->GetPress(CInputJoypad::PADBUTTONS_RB, 0))
			{// �A�˃��[�h

				if (pMouse->GetRepeat(CInputMouse::BUTTON_LMB) > 5 || pJoypad->GetRepeat(CInputJoypad::PADBUTTONS_RB, 0) > 5)
				{
					if (pMouse->GetRepeat(CInputMouse::BUTTON_LMB) % (int)fRapid == 0 && pMouse->GetPress(CInputMouse::BUTTON_LMB))
					{
						// �e�̔���
						pBullet = CBullet::Create
						(
							posMazzle,
							vecMazzle * BULLET_SPEED,
							BULLET_LIFE,
							CBullet::TYPE_PLAYER,
							false,
							BULLET_RADIUS,
							NORMAL_DAMAGE
						);

						if (m_pBodyUpper != nullptr)
						{// �ˌ����[�V����
							m_pBodyUpper->SetMotion(2);
						}

						// �G�l���M�[����
						m_fBoost -= fCost;

						// �A�˗͐���
						m_fParamRapid += m_fGrawRapid;

						if (m_fParamRapid > 1.0f)
						{// �p�����[�^�[�␳
							m_fParamRapid = 1.0f;
						}

						CParticle::Create(m_posMazzle, CParticle::TYPE_SHOT, D3DXVECTOR3(0.0f, 0.0f, 0.0f), &m_posMazzle);

						// SE�Đ�
						CManager::GetSound()->Play(CSound::LABEL_SHOT);
					}

					if (pJoypad->GetRepeat(CInputJoypad::PADBUTTONS_RB, 0) % (int)fRapid == 0 && pJoypad->GetPress(CInputJoypad::PADBUTTONS_RB, 0))
					{
						// �e�̔���
						pBullet = CBullet::Create
						(
							posMazzle,
							vecMazzle * BULLET_SPEED,
							BULLET_LIFE,
							CBullet::TYPE_PLAYER,
							false,
							BULLET_RADIUS,
							NORMAL_DAMAGE
						);

						if (m_pBodyUpper != nullptr)
						{// �ˌ����[�V����
							m_pBodyUpper->SetMotion(2);
						}

						// �G�l���M�[����
						m_fBoost -= fCost;

						// �A�˗͐���
						m_fParamRapid += m_fGrawRapid;

						if (m_fParamRapid > 1.0f)
						{// �p�����[�^�[�␳
							m_fParamRapid = 1.0f;
						}

						CParticle::Create(m_posMazzle, CParticle::TYPE_SHOT, D3DXVECTOR3(0.0f, 0.0f, 0.0f), &m_posMazzle);

						// SE�Đ�
						CManager::GetSound()->Play(CSound::LABEL_SHOT);
					}
				}
			}
			else
			{
				m_nTimerRapid--;
			}

			if (pMouse->GetRelease(CInputMouse::BUTTON_LMB) || pJoypad->GetRelease(CInputJoypad::PADBUTTONS_RB, 0))
			{// �A�ˏ�ԉ���
				m_nTimerRapid = 0;
			}
		}
	}

	if (pBullet != nullptr && m_pEnemy != nullptr)
	{// ���˂����e�����b�N�I���e�ɐݒ�
		pBullet->EnableLock(true);
	}
}

//=====================================================
// �J��������
//=====================================================
void CPlayer::InputCamera(void)
{
	// �ėp�����̎擾
	CUniversal *pUniversal = CManager::GetUniversal();

	// ���͏�����
	CInputMouse *pMouse = CManager::GetMouse();
	CInputJoypad *pJoypad = CManager::GetJoypad();

	// �J�����擾
	CCamera::Camera *pCameraInfo = CManager::GetCamera()->GetCamera();

	if (pMouse->GetPress(CInputMouse::BUTTON_RMB)/* || pJoypad->GetPress(CInputJoypad::PADBUTTONS_LB, 0)*/)
	{// �G�C����
		m_bAim = true;
		pCameraInfo->fLength = AIM_LENGTH;
	}
	else
	{// ��G�C����
		m_bAim = false;
		pCameraInfo->fLength = NOT_AIM_LENGTH;
	}

	// �}�E�X���͂̎擾
	D3DXVECTOR3 vecTurn = { (float)pMouse->GetMoveIY() ,(float)pMouse->GetMoveIX(),0.0f };

	// �R���g���[���[�̓���
	vecTurn = vecTurn + D3DXVECTOR3{ -pJoypad->GetJoyStickRY(0), pJoypad->GetJoyStickRX(0), 0.0f };

	// ��]�ʂ̐��K��
	D3DXVec3Normalize(&vecTurn, &vecTurn);

	// ��]�ʐݒ�
	pCameraInfo->rot.x -= vecTurn.x * ROLL_SPEED * 0.3f;
	pCameraInfo->rot.y += vecTurn.y * ROLL_SPEED;

	// �J�����̊p�x����
	if (pCameraInfo->rot.x < D3DX_PI * 0.5f - LIMIT_CAMERAROT)
	{
		pCameraInfo->rot.x = D3DX_PI * 0.5f - LIMIT_CAMERAROT;
	}
	else if (pCameraInfo->rot.x > D3DX_PI * 0.5f + LIMIT_CAMERAROT)
	{
		pCameraInfo->rot.x = D3DX_PI * 0.5f + LIMIT_CAMERAROT;
	}

	if (m_pBodyUpper != nullptr)
	{// �㔼�g�̉�]
		D3DXVECTOR3 rotUpper = D3DXVECTOR3(pCameraInfo->rot.x - D3DX_PI * 0.5f, pCameraInfo->rot.y - m_pBodyDowner->GetRot().y, m_pBodyUpper->GetRot().z);

		// �����̏C��
		pUniversal->LimitRot(&rotUpper.x);
		pUniversal->LimitRot(&rotUpper.y);
		pUniversal->LimitRot(&rotUpper.z);

		// �㔼�g�̊p�x����
		if (rotUpper.x < -LIMIT_UPPERROT)
		{
			rotUpper.x = -LIMIT_UPPERROT;
		}
		else if (rotUpper.x > LIMIT_UPPERROT)
		{
			rotUpper.x = LIMIT_UPPERROT;
		}

		// �㔼�g�̉�]
		m_pBodyUpper->SetRot(rotUpper);

		// �㔼�g��]����
		LimitUpperRot(0.0f);
	}

	// �p�x�̏C��
	pUniversal->LimitRot(&pCameraInfo->rot.y);

	if (m_nTimerShot > 0)
	{
		m_nTimerShot--;
	}
}

//=====================================================
// ���b�N�I������
//=====================================================
void CPlayer::Lockon(void)
{
	// �ėp�����̎擾
	CUniversal *pUniversal = CManager::GetUniversal();

	// �J�����擾
	CCamera::Camera *pCamera = CManager::GetCamera()->GetCamera();

	// �v�Z�p�ϐ�
	D3DXVECTOR3 pos1, pos2,vecDiff,veclockon;
	float fLength = FLT_MAX;
	float fDiff,fDiffAngle, fDiffAngle2,fDiffAngle3;
	float fDiffLength, fDiffLength2;
	float fMaxDiffAngle = D3DX_PI * 2;
	D3DXMATRIX *pMtxUpper = m_pBodyUpper->GetMatrix();
	float fRange = LOCKON_RANGE;

	pos1 = { m_mtxLockon[0]._41,m_mtxLockon[0]._42,m_mtxLockon[0]._43 };
	pos2 = { m_mtxLockon[1]._41,m_mtxLockon[1]._42,m_mtxLockon[1]._43 };

	// �G�̎擾
	CEnemy *pEnemy = CEnemy::GetEnemy();
	CEnemy *pLockEnemy = nullptr;

	if (m_bAim)
	{// �G�C�����̃��b�N�I������
		fRange = AIM_LOCKON_RANGE;
	}

	while (pEnemy != nullptr)
	{
		// ���̃A�h���X��ۑ�
		CEnemy *pEnemyNext = pEnemy->GetNext();

		// ���b�N�I���\���������ŉ�������
		pEnemy->SetLockon(false);

		if (pEnemy->GetState() == CEnemy::STATE_DEATH)
		{
			// ���̃A�h���X����
			pEnemy = pEnemyNext;

			continue;
		}

		if (CCollision::IsCross(pEnemy->GetPosition(), GetPosition(), pos1, nullptr) == false &&
			CCollision::IsCross(pEnemy->GetPosition(), GetPosition(), pos2, nullptr))
		{// �͈͓��ɂ����烍�b�N�I��
		 // �������v�Z
			vecDiff = pEnemy->GetPosition() - D3DXVECTOR3(pMtxUpper->_41, pMtxUpper->_42, pMtxUpper->_43);
			fDiff = D3DXVec3Length(&vecDiff);

			// Y���ʂ̃x�N�g���̒������擾
			fDiffLength = D3DXVec2Length(&D3DXVECTOR2(vecDiff.x, vecDiff.z));

			// �G�Ƃ̊p�x���擾
			fDiffAngle = atan2f(fDiffLength, vecDiff.y);

			// ���b�N�I���̌������擾
			veclockon = D3DXVECTOR3(m_mtxLockon[0]._41, m_mtxLockon[0]._42, m_mtxLockon[0]._43);
			veclockon = veclockon - D3DXVECTOR3(pMtxUpper->_41, pMtxUpper->_42, pMtxUpper->_43);

			// Y���ʂ̃x�N�g���̒������擾
			fDiffLength2 = D3DXVec2Length(&D3DXVECTOR2(veclockon.x, veclockon.z));

			// ���b�N�I���̊p�x���擾
			fDiffAngle2 = atan2f(fDiffLength2, veclockon.y);

			// �㉺�̊p�x�擾
			fDiffAngle = fDiffAngle - fDiffAngle2;

			// ���S�ɑ����Ă邩�̊p�x
			fDiffAngle3 = atan2f(vecDiff.x, vecDiff.z) - pCamera->rot.y - D3DX_PI;

			// �����̏C��
			pUniversal->LimitRot(&fDiffAngle3);
			pUniversal->LimitRot(&fDiffAngle);

			if (fDiff < fRange && fDiffAngle * fDiffAngle < LOCKON_ANGLE * LOCKON_ANGLE)
			{
				if (fDiffAngle3 * fDiffAngle3 < fMaxDiffAngle * fMaxDiffAngle)
				{// �ŏ��l��荷�����������ꍇ
					fMaxDiffAngle = fDiffAngle3;

					// ��ԋ߂��G���擾
					pLockEnemy = pEnemy;
				}
			}
		}

		// ���̃A�h���X����
		pEnemy = pEnemyNext;
	}

	if (pLockEnemy != nullptr)
	{// �D�悳�ꂽ�G�����b�N�I������
		m_posLockon = pLockEnemy->GetPosition();

		m_pEnemy = pLockEnemy;

		pLockEnemy->SetLockon(true);
	}
	else
	{
		m_posLockon = { 0.0f,0.0f,0.0f };

		m_pEnemy = nullptr;
	}

	if (m_pLockon != nullptr)
	{
		m_pLockon->SetEnemy(m_pEnemy);

		if (m_pEnemy == nullptr)
		{
			// �}�Y�������Ƀ}�[�J�[�ݒu
			D3DXVECTOR3 vecMazzle;

			vecMazzle = 
			{
				m_mtxMazzle[1]._41 - m_mtxMazzle[0]._41,
				m_mtxMazzle[1]._42 - m_mtxMazzle[0]._42,
				m_mtxMazzle[1]._43 - m_mtxMazzle[0]._43
			};

			D3DXVec3Normalize(&vecMazzle, &vecMazzle);

			vecMazzle *= BULLET_SPEED * BULLET_LIFE;

			m_pLockon->SetPosDest(m_posMazzle - vecMazzle);
		}
	}
}

//=====================================================
// �}�g���b�N�X�̐ݒ�
//=====================================================
void CPlayer::SetMatrix(void)
{
	// �J�����擾
	CCamera::Camera *pCamera = CManager::GetCamera()->GetCamera();

	// �ėp�����̎擾
	CUniversal *pUniversal = CManager::GetUniversal();

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();

	// �ϐ��錾
	D3DXMATRIX mtxRot, mtxTrans;

	// �{�̂̃��[���h�}�g���b�N�X=================================
	// ���[���h�}�g���b�N�X������
	D3DXMatrixIdentity(&m_mtxWorld);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans,
		m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// �����g�̃��[���h�}�g���b�N�X=================================
	if (m_pBodyDowner != nullptr)
	{
		D3DXMATRIX *pMtx = m_pBodyDowner->GetMatrix();

		pUniversal->SetOffSet(pMtx, m_mtxWorld, m_pBodyDowner->GetPosition(), m_pBodyDowner->GetRot());

		D3DXMATRIX *pMtxWaist = m_pBodyDowner->GetParts(0)->m_pParts->GetMatrix();

		m_pBodyDowner->MultiplyMtx();

		// ���̃��[���h�}�g���b�N�X=================================
		// ���[���h�}�g���b�N�X������
		D3DXMatrixIdentity(&m_mtxWaist);

		// �����𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot,
			0.0f, 0.0f, 0.0f);
		D3DXMatrixMultiply(&m_mtxWaist, &m_mtxWaist, &mtxRot);

		// �ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans,
			0.0f, 0.0f, 0.0f);
		D3DXMatrixMultiply(&m_mtxWaist, &m_mtxWaist, &mtxTrans);

		D3DXMatrixMultiply(&m_mtxWaist, &m_mtxWaist, pMtxWaist);

		// ���[���h�}�g���b�N�X�ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &m_mtxWaist);
	}

	// �㔼�g�̃��[���h�}�g���b�N�X=================================
	if (m_pBodyUpper != nullptr)
	{
		D3DXMATRIX *pMtx = m_pBodyUpper->GetMatrix();

		pUniversal->SetOffSet(pMtx, m_mtxWaist, m_pBodyUpper->GetPosition(), m_pBodyUpper->GetRot());
	}

#ifdef DEBUG
	CManager::GetDebugProc()->Print("\n�㔼�g�̌����F[%f,%f,%f]", m_pBodyUpper->GetRot().x, m_pBodyUpper->GetRot().y, m_pBodyUpper->GetRot().z);
	CManager::GetDebugProc()->Print("\n�����g�̌����F[%f,%f,%f]", m_pBodyDowner->GetRot().x, m_pBodyDowner->GetRot().y, m_pBodyDowner->GetRot().z);
#endif
}

//=====================================================
// ��ԊǗ�
//=====================================================
void CPlayer::ManageState(void)
{
	switch (m_state)
	{
	case CPlayer::STATE_NORMAL:
		break;
	case CPlayer::STATE_DEATH:

		m_nCntDeath++;

		if (m_nCntDeath >= TIME_DEATH)
		{
			// ���̈ʒu�擾
			D3DXVECTOR3 pos = { m_mtxWaist._41,m_mtxWaist._42, m_mtxWaist._43 };

			// �����p�[�e�B�N��
			CParticle::Create(pos, CParticle::TYPE_EXPLOSION);

			// ���Ɣj�Ђ̏���
			CSmokeSpawner::Create(pos, 15.0f, 1, 6);
			CDebrisSpawner::Create(pos, 18.0f, 1, 12);

			// �I������
			Uninit();

			CGame::SetState(CGame::STATE_RESULT);

			CResult::Create(false);

			// SE�Đ�
			CManager::GetSound()->Play(CSound::LABEL_EXPLOSION);
		}

		break;
	default:
		break;
	}
}

//=====================================================
// ���[�V�����Ǘ�
//=====================================================
void CPlayer::ManageMotion(void)
{
	// �ϐ��錾
	float fSpeed = D3DXVec3Length(&GetMove());
	MOTION motionDowner = MOTION_NEUTRAL;
	MOTION motionUpper = MOTION_NEUTRAL;
	bool bFinishUpper = false;
	bool bFinishDowner = false;

	if (m_pBodyUpper != nullptr)
	{// �㔼�g���[�V�������擾
		motionUpper = (MOTION)m_pBodyUpper->GetMotion();
		bFinishUpper = m_pBodyUpper->IsFinish();
	}

	if (m_pBodyDowner != nullptr)
	{// �����g���[�V�����擾
		motionDowner = (MOTION)m_pBodyDowner->GetMotion();
		bFinishDowner = m_pBodyDowner->IsFinish();
	}

#ifdef DEBUG
	CManager::GetDebugProc()->Print("\n�v���C���[�̑��x�F[%f]", fSpeed);
	CManager::GetDebugProc()->Print("\n�����g���[�V�����F[%d]", motionDowner);
	CManager::GetDebugProc()->Print("\n�����g���[�V�����I���F[%d]", bFinishDowner);
#endif

	if (m_bLand)
	{// �n�ヂ�[�V����
		if (motionDowner != MOTION_SHOT || (motionDowner == MOTION_SHOT && bFinishDowner))
		{
			if ((int)fSpeed != 0 && motionDowner != MOTION_BOOST && motionDowner != MOTION_TURNBOOST)
			{// ������x�����Ă���Ε������[�V����
				if (m_pBodyDowner != nullptr)
				{
					if (m_bTurnDowner)
					{// ���������[�V����
						if (motionDowner != MOTION_TURNWALK)
						{
							m_pBodyDowner->SetMotion(MOTION_TURNWALK);
						}
					}
					else
					{// ���������[�V����
						if (motionDowner != MOTION_WALK)
						{
							m_pBodyDowner->SetMotion(MOTION_WALK);
						}
					}
				}
			}
			else
			{
				if ((int)fSpeed == 0 && (motionDowner == MOTION_WALK || motionDowner == MOTION_TURNWALK))
				{// �����~�܂�
					if (m_pBodyDowner != nullptr)
					{// �����g���[�V�����ݒ�
						m_pBodyDowner->SetMotion(MOTION_NEUTRAL);
					}
				}
			}
		}
	}
}

//=====================================================
// �ڕW��������������
//=====================================================
void CPlayer::Turn(D3DXVECTOR3 move, float fRotOld)
{
	// �ėp�����̎擾
	CUniversal *pUniversal = CManager::GetUniversal();

	// �J�����擾
	CCamera::Camera *pCamera = CManager::GetCamera()->GetCamera();

	// �ϐ��錾
	float fRotDest;
	float fRotDowner = 0.0f;	// �㔼�g�̊p�x
	float fRotDiff2;
	D3DXVECTOR3 rot = { 0.0f,0.0f,0.0f };

	if (D3DXVec3Length(&move) != 0)
	{// �����Ă鎞�̔���
		// �ړ��������擾
		fRotDest = atan2f(move.x, move.z) + D3DX_PI;

		// �l�̕␳
		pUniversal->LimitRot(&fRotDest);

		// �㔼�g�Ƃ̊p�x�̍������擾
		if (m_pBodyUpper != nullptr)
		{
			// �J�����ƖڕW�����̍����擾
			fRotDiff2 = fRotDest - pCamera->rot.y;

			// �l�̕␳
			pUniversal->LimitRot(&fRotDiff2);

			if (fRotDiff2 * fRotDiff2 > TURN_DOWNER * TURN_DOWNER)
			{// �t����
				m_bTurnDowner = true;
			}
			else/* if(fRotDiff2 * fRotDiff2 < TURN_ROT * TURN_ROT)*/
			{// ������
				m_bTurnDowner = false;
			}

			if (m_bTurnDowner)
			{
				fRotDest = atan2f(move.x, move.z);
			}
			else
			{
				fRotDest = atan2f(move.x, move.z) + D3DX_PI;
			}

			// �l�̕␳
			pUniversal->LimitRot(&fRotDest);
		}

		if (m_pBodyDowner != nullptr)
		{
			// �����̍������擾
			float fRotDiff = fRotDest - GetRot().y;

			// �l�̕␳
			pUniversal->LimitRot(&fRotDiff);

			// �����̐ݒ�
			rot = D3DXVECTOR3(m_pBodyDowner->GetRot().x, m_pBodyDowner->GetRot().y + fRotDiff * 0.1f, m_pBodyDowner->GetRot().z);

			// �l�̕␳
			pUniversal->LimitRot(&rot.y);

			// �����g�̉�]
			m_pBodyDowner->SetRot(rot);

			// �v���C���[�̉�]
			SetRot(rot);
		}
	}
}

//=====================================================
// �㔼�g�̉�]����
//=====================================================
void CPlayer::LimitUpperRot(float fRotOld)
{
	// �ėp�����̎擾
	CUniversal *pUniversal = CManager::GetUniversal();

	// �ϐ��錾
	float fRot = 0.0f;
	float fRotDiff = 0.0f;
	D3DXVECTOR3 rot;

	if (m_pBodyUpper != nullptr)
	{
		// �㔼�g�̊p�x���擾
		fRot = m_pBodyUpper->GetRot().y;

		if (m_pBodyDowner != nullptr)
		{
			if (fRot > TURN_ROT)
			{// �E�ɂ͂ݏo���ꍇ
				fRotDiff = fRot - TURN_ROT;
			}
			else if (fRot < -TURN_ROT)
			{// ���ɂ͂ݏo���ꍇ
				fRotDiff = fRot + TURN_ROT;
			}

			rot = D3DXVECTOR3
			(
				m_pBodyDowner->GetRot().x,
				m_pBodyDowner->GetRot().y + fRotDiff,
				m_pBodyDowner->GetRot().z
			);

			pUniversal->LimitRot(&rot.y);

			// �����g����]������
			m_pBodyDowner->SetRot(rot);

			// �v���C���[�̉�]
			SetRot(rot);
		}
	}
}

//=====================================================
// �΍������̏���
//=====================================================
D3DXVECTOR3 CPlayer::LinePridiction(D3DXVECTOR3 pos, float fSpeedBullet, D3DXVECTOR3 posTarget, D3DXVECTOR3 moveTarget)
{
	// �ϐ��錾
	D3DXVECTOR3 vecDiff;
	float fSpeedTarget;
	float fDataA, fDataB, fDataC, fDataD;
	int nFrame1, nFrame2;

	// �ڕW�̑��x���v�Z
	fSpeedTarget = D3DXVec3Length(&moveTarget);

	// �ڕW�Ƃ̈ʒu�̍������擾
	vecDiff = posTarget - pos;

	fDataA = (moveTarget.x * moveTarget.x + moveTarget.y * moveTarget.y + moveTarget.z * moveTarget.z) - fSpeedBullet * fSpeedBullet;

	fDataB = 2 * (posTarget.x * moveTarget.x + posTarget.y * moveTarget.y + posTarget.z * moveTarget.z);

	fDataC = (posTarget.x * posTarget.x + posTarget.y * posTarget.y + posTarget.z * posTarget.z);

	if (fDataA == 0)
	{// 0����h�~
		if (fDataB == 0)
		{
			return posTarget;
		}
		else
		{
			return posTarget + moveTarget * (-fDataC / fDataB);
		}
	}

	//�񎟕������̉��̌����̔��ʎ��ŕ���
	fDataD = fDataB * fDataB - 4 * fDataA * fDataC;

	if (fDataD > 0)
	{
		float fDataE = (float)sqrt(fDataD);
		nFrame1 = (int)((-fDataB - fDataE) / (2 * fDataA));
		nFrame2 = (int)((-fDataB + fDataE) / (2 * fDataA));
		//����2�Ȃ̂Ő��̐��̍ŏ��l���g��
		nFrame1 = (int)PlusMin((float)nFrame1, (float)nFrame2);
	}
	else
	{
		//������
		//������Ȃ��̂ō��̈ʒu��_��
		nFrame1 = 0;
	}

	//�\�z�ʒu��Ԃ�
	return posTarget + moveTarget * (float)nFrame1;
}

//=====================================================
// �v���X�̍ŏ��l��Ԃ�����
//=====================================================
float CPlayer::PlusMin(float fData1, float fData2)
{
	if (fData1 < 0 && fData2 < 0)
	{// �ǂ�������̒l�Ȃ�O��Ԃ�
		return 0;
	}
	else if (fData1 < 0)
	{
		return fData2;
	}
	else if (fData2 < 0)
	{
		return fData1;
	}

	return fData1 < fData2 ? fData1 : fData2;
}

//=====================================================
// �u�[�X�g�p�[�e�B�N���̐ݒ�
//=====================================================
void CPlayer::Boost(void)
{
	// �ėp�����̎擾
	CUniversal *pUniversal = CManager::GetUniversal();

	// �ϐ��錾
	D3DXMATRIX mtxBody, mtxVector;
	D3DXMATRIX *pMtxBody = nullptr;
	D3DXVECTOR3 vecDiff;
	D3DXVECTOR3 rot = { 0.0f,0.0f,0.0f };
	float fLength;

	if (m_pBodyUpper != nullptr)
	{// ���̂̃}�g���b�N�X�擾
		m_pBodyUpper->MultiplyMtx();

		pMtxBody = m_pBodyUpper->GetParts(0)->m_pParts->GetMatrix();

		if (pMtxBody != nullptr)
		{
			// �����̃}�g���b�N�X�̐ݒ�
			pUniversal->SetOffSet(&mtxBody,*pMtxBody,D3DXVECTOR3(0.0f,30.0f,30.0f));

			m_posBoost = D3DXVECTOR3{ mtxBody._41,mtxBody._42 ,mtxBody._43 } + m_move;

			// ���˕����̃}�g���b�N�X�̐ݒ�
			pUniversal->SetOffSet(&mtxVector, *pMtxBody, D3DXVECTOR3(0.0f, -10.0f, 50.0f));

			m_vecBoost = { mtxVector._41,mtxVector._42 ,mtxVector._43 };

			// ���˃x�N�g�����v�Z
			vecDiff = m_posBoost - m_vecBoost;

			// Y���������v�Z
			rot.y = atan2f(vecDiff.x, vecDiff.z)/* - m_pBodyUpper->GetRot().y*/;

			// Y�����ʂ̒������擾
			fLength = sqrtf(vecDiff.x * vecDiff.x + vecDiff.z * vecDiff.z);

			// X���������v�Z
			rot.x = atan2f(fLength * fLength, vecDiff.y) + D3DX_PI;

			// �p�[�e�B�N���ݒ�
			CParticle::Create(m_posBoost,CParticle::TYPE_BOOST,rot,&m_posBoost);
		}
	}
}

//=====================================================
// �q�b�g����
//=====================================================
void CPlayer::Hit(float fDamage)
{
	if (m_state == STATE_DEATH)
	{
		return;
	}

	// SE�Đ�
	CManager::GetSound()->Play(CSound::LABEL_HIT);

	// �̗͌���
	m_nLife -= (int)fDamage;

	if (m_pAssess != nullptr)
	{// �_���[�W�ɂ��]���y�i���e�B
		m_pAssess->AddParam(-DAMAGE_PENALTY, CAssess::PARAM_HIT);
	}

	if (m_nLife <= 0)
	{// ���S����
		// ���̈ʒu�擾
		D3DXVECTOR3 pos = { m_mtxWaist._41,m_mtxWaist._42, m_mtxWaist._43 };

		m_nLife = 0;

		CNoise::Create(TIME_DEATH);

		CExplSpawner::Create(pos, 500.0f, TIME_DEATH);

		Death();

		// �Q�[���̏�Ԃ����U���g�ɂ���
		CGame::SetState(CGame::STATE_RESULT);
	}
	else
	{
		CNoise::Create();
	}
}

//=====================================================
// ���S����
//=====================================================
void CPlayer::Death(void)
{
	if (m_pBodyDowner != nullptr && m_pBodyUpper != nullptr)
	{
		m_pBodyUpper->SetRot(D3DXVECTOR3(0.0f,0.0f,0.0f));

		m_pBodyDowner->SetMotion(6);

		m_pBodyUpper->SetMotion(4);
	}

	m_move = { 0.0f,0.0f,0.0f };

	m_bSprint = false;

	m_state = STATE_DEATH;
}

//=====================================================
// �q�b�g���ݒ�
//=====================================================
void CPlayer::SetNumHit(int nNumHit)
{
	if (m_pAssess != nullptr)
	{// �q�b�g�]���̉����Z
		int nDiff = nNumHit - m_nNumHit;

		if (nDiff > 0)
		{// ���Z����Ă�����
			m_pAssess->AddParam(HIT_ADD, CAssess::PARAM_HIT);
		}
		else
		{
			m_pAssess->AddParam(-HIT_ADD, CAssess::PARAM_HIT);
		}
	}

	m_nNumHit = nNumHit;
}

//=====================================================
// �`�揈��
//=====================================================
void CPlayer::Draw(void)
{
	// �J�����擾
	CCamera::Camera *pCameraInfo = CManager::GetCamera()->GetCamera();

	// �}�g���b�N�X�̐ݒ�
	SetMatrix();

#ifdef _DEBUG
	CManager::GetDebugProc()->Print("\n�v���C���[�̈ʒu�F[%f,%f,%f]", GetPosition().x, GetPosition().y, GetPosition().z);
	CManager::GetDebugProc()->Print("\n�J�����̊p�x�F[%f,%f,%f]", pCameraInfo->rot.x, pCameraInfo->rot.y, pCameraInfo->rot.z);
	CManager::GetDebugProc()->Print("\n�A�˗́F[%f]", m_fParamRapid);
	CManager::GetDebugProc()->Print("\n�U���́F[%f]", m_fParamPower);
	CManager::GetDebugProc()->Print("\n�����F[%f]", m_fParamCost);
	CManager::GetDebugProc()->Print("\n�q�b�g���F[%d]", m_nNumHit);
	CManager::GetDebugProc()->Print("\n�v���C���[�̗�[%d]", m_nLife);
	CManager::GetDebugProc()->Print("\n�v���C���[�W�����v[%d]", m_bJump);
	CManager::GetDebugProc()->Print("\n�����蔻�萔[%d]", CCollision::GetNumAll());
	CManager::GetDebugProc()->Print("\n�e�N�X�`����[%d]", CTexture::GetNumAll());
	CManager::GetDebugProc()->Print("\n���f����[%d]", CModel::GetNumAll());
	CManager::GetDebugProc()->Print("\n���Z�b�g[F3]");
#else
	CManager::GetDebugProc()->Print("\n\n\n\n\n\n\n\n\n\n\n");
	CManager::GetDebugProc()->Print("//----------------------------\n");
	CManager::GetDebugProc()->Print("// �v���C���[��{����\n");
	CManager::GetDebugProc()->Print("//----------------------------\n");
	CManager::GetDebugProc()->Print("[WASD]�ړ�\n");
	CManager::GetDebugProc()->Print("[LSHIFT + �ړ�]�_�b�V��\n");
	CManager::GetDebugProc()->Print("[SPACE]�W�����v\n");
	CManager::GetDebugProc()->Print("[LMB]�ˌ�\n");
	CManager::GetDebugProc()->Print("[RMB]�Y�[��\n");
	CManager::GetDebugProc()->Print("\n");
	CManager::GetDebugProc()->Print("//----------------------------\n");
	CManager::GetDebugProc()->Print("// �ˌ��̎��\n");
	CManager::GetDebugProc()->Print("//----------------------------\n");
	CManager::GetDebugProc()->Print("[�Z����]�P������\n");
	CManager::GetDebugProc()->Print("[������]�`���[�W�V���b�g\n");
	CManager::GetDebugProc()->Print("[�_�u���N���b�N��������]�A��\n");
#endif
}

//=====================================================
// ��������
//=====================================================
CPlayer *CPlayer::Create(void)
{
	CPlayer *pPlayer = nullptr;

	if (pPlayer == nullptr)
	{// �C���X�^���X����
		pPlayer = new CPlayer;

		// ����������
		pPlayer->Init();
	}

	return pPlayer;
}

//=====================================================
// �p�����[�^�[�Ǎ�����
//=====================================================
HRESULT CPlayer::Load(void)
{
	// �ϐ��錾
	char cTemp[256];

	// �t�@�C������ǂݍ���
	FILE *pFile = fopen("data\\player.txt", "r");

	if (pFile != nullptr)
	{// �t�@�C�����J�����ꍇ
		while (true)
		{
			// �����ǂݍ���
			fscanf(pFile, "%s", &cTemp[0]);

			if (strcmp(cTemp, "PARAMSET") == 0)
			{// �p�����[�^�[�Ǎ��J�n
				while (true)
				{
					// �����ǂݍ���
					fscanf(pFile, "%s", &cTemp[0]);

					if (strcmp(cTemp, "INITIAL_RAPID") == 0)
					{// �����A�˗͓Ǎ�
						fscanf(pFile, "%s", &cTemp[0]);

						fscanf(pFile, "%f", &m_fParamRapid);
					}

					if (strcmp(cTemp, "INITIAL_POWER") == 0)
					{// �����U���͓Ǎ�
						fscanf(pFile, "%s", &cTemp[0]);

						fscanf(pFile, "%f", &m_fParamPower);
					}

					if (strcmp(cTemp, "INITIAL_COST") == 0)
					{// ���������Ǎ�
						fscanf(pFile, "%s", &cTemp[0]);

						fscanf(pFile, "%f", &m_fParamCost);
					}

					if (strcmp(cTemp, "GRAW_RAPID") == 0)
					{// �A�˗͐������Ǎ�
						fscanf(pFile, "%s", &cTemp[0]);

						fscanf(pFile, "%f", &m_fGrawRapid);
					}

					if (strcmp(cTemp, "GRAW_POWER") == 0)
					{// �U���͐������Ǎ�
						fscanf(pFile, "%s", &cTemp[0]);

						fscanf(pFile, "%f", &m_fGrawPower);
					}

					if (strcmp(cTemp, "GRAW_COST") == 0)
					{// �����������Ǎ�
						fscanf(pFile, "%s", &cTemp[0]);

						fscanf(pFile, "%f", &m_fGrawCost);
					}

					if (strcmp(cTemp, "END_PARAMSET") == 0)
					{// �p�����[�^�[�Ǎ��I��
						break;
					}
				}
			}

			if (strcmp(cTemp, "END_SCRIPT") == 0)
			{// �I������
				break;
			}
		}

		// �t�@�C�������
		fclose(pFile);
	}

	return S_OK;
}