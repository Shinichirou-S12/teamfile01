//=============================================================================
//
// ���U���g��ʏ��� [result.h]
// Author : 
//
//=============================================================================
#pragma once

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define	TEXTURE_RESULT		_T("data/TEXTURE/ResultTexture/bg001.jpg")				// �ǂݍ��ރe�N�X�`���t�@�C����
#define	TEXTURE_RESULT_LOGO	_T("data/TEXTURE/ResultTexture/result_logo.png")		// �ǂݍ��ރe�N�X�`���t�@�C����
#define	RESULT_POS_X			(0)							// �^�C�g���̕\���ʒu
#define	RESULT_POS_Y			(0)							// �^�C�g���̕\���ʒu
#define	RESULT_SIZE_X			(800)						// �^�C�g���̕�
#define	RESULT_SIZE_Y			(600)						// �^�C�g���̍���
#define	RESULTLOGO_POS_X		(160)						// �^�C�g�����S�̕\���ʒu
#define	RESULTLOGO_POS_Y		(200)						// �^�C�g�����S�̕\���ʒu
#define	RESULTLOGO_SIZE_X		(480)						// �^�C�g�����S�̕�
#define	RESULTLOGO_SIZE_Y		(80)						// �^�C�g�����S�̍���

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitResult(int type);
void UninitResult(void);
void UpdateResult(void);
void DrawResult(void);

