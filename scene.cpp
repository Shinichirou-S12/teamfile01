//! @file	scene.cpp
//! @author	�܂�
//! @date	2020-07-13
//! @brief	�V�[���̎���

// �C���N���[�h�t�@�C��
#include "main.h"
#include "scene.h"
#include "titlescene.h"
#include "gamescene.h"
#include "resultscene.h"

// �}�N����`
#define SCENE_START	(SCENE_TITLE)
 
// �O���[�o���ϐ�
int g_scene = SCENE_START;

// ����������
void InitScene(void)
{
	switch (g_scene)
	{
	case SCENE_TITLE:
		InitTitleScene();
		break;
	case SCENE_GAME:
		InitGameScene();
		break;
	case SCENE_RESULT:
		InitResultScene();
		break;
	default:
		break;
	}
}

// �I������
void UninitScene(void)
{
	switch (g_scene)
	{
	case SCENE_TITLE:
		UninitTitleScene();
		break;
	case SCENE_GAME:
		UninitGameScene();
		break;
	case SCENE_RESULT:
		UninitResultScene();
		break;
	default:
		break;
	}
}

// �X�V����
void UpdateScene(void)
{
	switch (g_scene)
	{
	case SCENE_TITLE:
		UpdateTitleScene();
		break;
	case SCENE_GAME:
		UpdateGameScene();
		break;
	case SCENE_RESULT:
		UpdateResultScene();
		break;
	default:
		break;
	}
}

// �`�揈��
void DrawScene(void)
{
	switch (g_scene)
	{
	case SCENE_TITLE:
		DrawTitleScene();
		break;
	case SCENE_GAME:
		DrawGameScene();
		break;
	case SCENE_RESULT:
		DrawResultScene();
		break;
	default:
		break;
	}
}

// �V�[���؂�ւ�
void SetScene(int scene)
{
	UninitScene();

	g_scene = scene;
	
	InitScene();
}

// �V�[���̎擾
int GetScene(void)
{
	return g_scene;
}

