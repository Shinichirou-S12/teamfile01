//=============================================================================
//
// �v���C���[���� [playerTest.cpp]
// Author : 
//
//=============================================================================
#include "main.h"
#include "scene.h"
#include "playerTest.h"
#include "controller.h"
#include "input.h"
#include "map.h"
#include "bullet.h"
#include "checkhit.h"
#include "life.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define PLAYER_HP	(4)
#define PLAYER_TIME_SHOT	(CHANGE_TIME)
#define MAX_DIFFUSE	(255)
#define PLAYER_INVINCIVLE	(10)
#define PLAYER_MOVE_SPEED	(2.0f)

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void ReadTexturePlayer(void);

HRESULT MakeVertexPlayer(void);
void SetTexturePlayer(VERTEX_2D *Vtx, int cntPattern);
void SetVertexPlayer(VERTEX_2D *Vtx);
void animPlayerState(int * animState, int * partsState);

void Invincible(void);
void FallPlayer(void);
void JumpPlayer(void);
void AttackPlayer(void);
void PlayerMoving(void);

void Restriction(void);

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static LPDIRECT3DTEXTURE9		g_pD3DTexture[STATE_MAX][MAXPARTS] = { NULL };	// �e�N�X�`���ւ̃|���S��

static PLAYER					g_player;		// �v���C���[�\����

//=============================================================================
// ����������
//=============================================================================
HRESULT InitPlayer(void)
{
	ReadTexturePlayer();

	g_player.invincible = false;									// ���G��Ԃł͂Ȃ�
	g_player.pos = D3DXVECTOR3(/*i*200.0f + */200.0f, 300.0f, 0.0f);// ���W�f�[�^��������
	g_player.scrollPos = D3DXVECTOR3(200.0f, 300.0f, 0.0f);			// �X�N���[�����W�f�[�^��������
	g_player.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);					// ��]�f�[�^��������

	g_player.moveSpeed = PLAYER_MOVE_SPEED;
	g_player.use = true;
	g_player.scroll = false;
	g_player.countMove = 0;
	g_player.countScroll = 0;


	// �A�j���p�^�[���ԍ��������_���ŏ�����
	g_player.animeCnt.PatternAnim = rand() % PLAYER_IDLE_TEXTURE_PATTERN_DIVIDE_X;	

	g_player.textureSize = D3DXVECTOR2(PLAYER_TEXTURE_SIZE_X, PLAYER_TEXTURE_SIZE_Y);
	g_player.checkTopTexSize = PLAYER_TEXTURE_BB_SIZE_TOP_X;

	g_player.state = IDLE;
	g_player.partsState = PERFECT;
	g_player.Texture = g_pD3DTexture[g_player.state][g_player.partsState];

	g_player.animeCnt.PatDivX = PLAYER_IDLE_TEXTURE_PATTERN_DIVIDE_X;	// �e�N�X�`���̓�������X��������
	g_player.animeCnt.PatDivY = PLAYER_IDLE_TEXTURE_PATTERN_DIVIDE_Y;	// �e�N�X�`���̓�������Y��������
	g_player.animeCnt.AnimPatNum = PLAYER_IDLE_ANIM_PATTERN_NUM;		// �A�j���[�V�����p�^�[������������
	g_player.animeCnt.TimeAnim = PLAYER_IDLE_TIME_ANIMATION;			// �A�j���[�V�����̐؂�ւ��J�E���g��������
	g_player.animeCnt.PatternAnim = 0;									// �A�j���p�^�[���ԍ�������

	g_player.animeCnt.CountAnim = 0;								// �A�j���J�E���g��������
	g_player.countShot = PLAYER_TIME_SHOT;							// �ŏ��̈ꔭ�͂������Ă�悤��
	g_player.countInvincible = 0;									// ���G�J�E���g��������
	g_player.hp = PLAYER_HP;										// HP�̏�����
	D3DXVECTOR2 temp = D3DXVECTOR2(PLAYER_TEXTURE_SIZE_X, PLAYER_TEXTURE_SIZE_Y);
	g_player.radius = D3DXVec2Length(&temp);									// ���a��������
	g_player.baseAngle = atan2f(PLAYER_TEXTURE_SIZE_Y, PLAYER_TEXTURE_SIZE_X);	// �p�x��������

	g_player.jumpForce = 0;										// �W�����v���Ă��Ȃ�
	g_player.dropSpeed = 0;	// �d�͉����x������
	g_player.ofsPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);				// �I�t�Z�b�g���W��������
	g_player.direction = Right;										// �v���C���[�͉E����

	MakeVertexPlayer();

	if (FAILED(MakeVertexPlayer()))
	{
		return E_FAIL;
	}
	return S_OK;
}

//=============================================================================
// �I������
//=============================================================================
void UninitPlayer(void)
{
	for (int i = 0; i < STATE_MAX; i++)
	{
		for (int j = 0; j < MAXPARTS; j++)
		{
			if (g_pD3DTexture[i][j] != NULL)
			{	// �e�N�X�`���̊J��
				g_pD3DTexture[i][j]->Release();
				g_pD3DTexture[i][j] = NULL;
			}
		}
	}
}

//=============================================================================
// �X�V����
//=============================================================================
void UpdatePlayer(void)
{
	if (!g_player.use)
	{
		SetScene(SCENE_RESULT);
		return;
	}

	// �A�j���[�V�����̃p�^�[�����X�V------------------
	g_player.animeCnt.CountAnim++;

	// ���̃t���[�������Ԑ؂ꂽ��
	if ((g_player.animeCnt.CountAnim % g_player.animeCnt.TimeAnim) == 0)
	{
		//cnt���Z�b�g
		g_player.animeCnt.CountAnim = 0;
		// ���̃t���[���ɐi��
		g_player.animeCnt.PatternAnim = (g_player.animeCnt.PatternAnim + 1) % g_player.animeCnt.AnimPatNum;
	}

		if (g_player.hp <= 0 || g_player.pos.y > SCREEN_HEIGHT + PLAYER_TEXTURE_SIZE_Y * 5)
		{
			g_player.use = false;
		}

		g_player.countShot++;			// �A�˂̃J�E���g�p
		g_player.scrollPos.x = 0.0f;	// ��ʃX�N���[���p�̕ϐ�

		Invincible();

		if (g_player.jumpForce == 0)	// �v���C���[���W�����v���Ă��Ȃ��Ɨ�����ԂɂȂ�
		{
			g_player.state = IDLE;
		}

		// �L�[���͂ňړ� 
		if (GetInput(JUMPMOVE))	// �W�����v�֌W
		{
			if (g_player.dropSpeed > 1 && g_player.jumpForce <= 1)	//�u�n�ォ��W�����v���Ă���̈�񂾂��󒆃W�����v�v�Ɓu���ꂩ�玩�R��������̈�񂾂��󒆃W�����v�v�����邽�߂̔���
			{
				g_player.dropSpeed = 0;	//�d�͉����x��0�ɂ��邱�ƂŁA�Ăуv���C���[���㏸���͂��߂�
				g_player.jumpForce++;	// ��������jump++�ƍ��킹��jump��2�ȏ�ɂȂ�̂ŁA�ǂ̂悤�ȃ^�C�~���O�ł��u�󒆃W�����v��̓W�����v�ł��Ȃ��v
										// SE�Đ�
				//PlaySound(SOUND_LABEL_SE_jump2);
				//playerWk[1].use = true;
			}
			else
			{
				//PlaySound(SOUND_LABEL_SE_jump);
			}

			g_player.jumpForce++;		// �W�����v�̐؂�ւ��A1�ȏ�ŃW�����v��ԂɂȂ�
			g_player.state = JUMP;		// �e�N�X�`���́u�W�����v�v
		}

		if (g_player.scroll)
		{
			MAP *mapchip = GetMapData();

			if(g_player.countMove != (SCREEN_WIDTH / 10))
			{
				g_player.pos.x -= 10.0f;
				for (int j = 0; j < (SIZE_X * SIZE_Y * MAP_MAXDATA); j++)
				{
					mapchip->pos.x -= 10.0f;
					mapchip++;
				}
			}
			g_player.countMove++;

			if (g_player.countMove == (SCREEN_WIDTH / 10))
			{
				g_player.scroll = false;
				g_player.countMove = 0;
			}

		}

		PlayerMoving();

		//g_player.mapPos += g_player.scrollPos;	// �ǂꂾ���X�N���[����������ێ��A�C�x���g�Ɏg�p

		Restriction();
		JumpPlayer();
		FallPlayer();
		AttackPlayer();

		animPlayerState(&g_player.state, &g_player.partsState);

		SetVertexPlayer(g_player.vertexWk);
		SetTexturePlayer(g_player.vertexWk, g_player.animeCnt.PatternAnim);
}

//=============================================================================
// �v���C���[�̃e�N�X�`���ǂݍ���
//=============================================================================
void ReadTexturePlayer(void)
{
	LPDIRECT3DDEVICE9	pDevice = GetDevice();

	// �A�C�h�����
	D3DXCreateTextureFromFile(pDevice,		// �f�o�C�X�ւ̃|�C���^
		TEXTURE_PLAYER_PERFECT_IDLE,		// �t�@�C���̖��O
		&g_pD3DTexture[IDLE][PERFECT]);		// �ǂݍ��ރ������[

	D3DXCreateTextureFromFile(pDevice,		// �f�o�C�X�ւ̃|�C���^
		TEXTURE_PLAYER_THREE_IDLE,			// �t�@�C���̖��O
		&g_pD3DTexture[IDLE][THREE]);		// �ǂݍ��ރ������[

	D3DXCreateTextureFromFile(pDevice,		// �f�o�C�X�ւ̃|�C���^
		TEXTURE_PLAYER_TWO_IDLE,			// �t�@�C���̖��O
		&g_pD3DTexture[IDLE][TWO]);			// �ǂݍ��ރ������[

	D3DXCreateTextureFromFile(pDevice,		// �f�o�C�X�ւ̃|�C���^
		TEXTURE_PLAYER_ONE_IDLE,			// �t�@�C���̖��O
		&g_pD3DTexture[IDLE][ONE]);			// �ǂݍ��ރ������[


	// �ړ�
	D3DXCreateTextureFromFile(pDevice,		// �f�o�C�X�ւ̃|�C���^
		TEXTURE_PLAYER_PERFECT_RUN,			// �t�@�C���̖��O
		&g_pD3DTexture[RUN][PERFECT]);		// �ǂݍ��ރ������[

	D3DXCreateTextureFromFile(pDevice,		// �f�o�C�X�ւ̃|�C���^
		TEXTURE_PLAYER_THREE_RUN,			// �t�@�C���̖��O
		&g_pD3DTexture[RUN][THREE]);		// �ǂݍ��ރ������[

	D3DXCreateTextureFromFile(pDevice,		// �f�o�C�X�ւ̃|�C���^
		TEXTURE_PLAYER_TWO_RUN,				// �t�@�C���̖��O
		&g_pD3DTexture[RUN][TWO]);			// �ǂݍ��ރ������[

	D3DXCreateTextureFromFile(pDevice,		// �f�o�C�X�ւ̃|�C���^
		TEXTURE_PLAYER_ONE_RUN,				// �t�@�C���̖��O
		&g_pD3DTexture[RUN][ONE]);			// �ǂݍ��ރ������[

	// �W�����v
	D3DXCreateTextureFromFile(pDevice,		// �f�o�C�X�ւ̃|�C���^
		TEXTURE_PLAYER_PERFECT_JUMP,		// �t�@�C���̖��O
		&g_pD3DTexture[JUMP][PERFECT]);		// �ǂݍ��ރ������[

	D3DXCreateTextureFromFile(pDevice,		// �f�o�C�X�ւ̃|�C���^
		TEXTURE_PLAYER_THREE_JUMP,			// �t�@�C���̖��O
		&g_pD3DTexture[JUMP][THREE]);		// �ǂݍ��ރ������[

	D3DXCreateTextureFromFile(pDevice,		// �f�o�C�X�ւ̃|�C���^
		TEXTURE_PLAYER_TWO_JUMP,			// �t�@�C���̖��O
		&g_pD3DTexture[JUMP][TWO]);			// �ǂݍ��ރ������[

	D3DXCreateTextureFromFile(pDevice,		// �f�o�C�X�ւ̃|�C���^
		TEXTURE_PLAYER_ONE_JUMP,			// �t�@�C���̖��O
		&g_pD3DTexture[JUMP][ONE]);			// �ǂݍ��ރ������[

}


//=============================================================================
// �v���C���[�̖��G��Ԃ̊m�F
//=============================================================================
void Invincible(void)
{
	if (g_player.invincible == true)
	{
		g_player.countInvincible++;

		if (g_player.countInvincible >= PLAYER_INVINCIVLE)
		{
			g_player.invincible = false;
			g_player.countInvincible = 0;
		}
	}
}

//=============================================================================
// �v���C���[�ړ�
//=============================================================================
void PlayerMoving(void)
{
	// �E�ړ�----------------------------------------
	if (GetInput(RIGHTMOVE))
	{
		g_player.pos.x += g_player.moveSpeed;

		g_player.direction = Right;					// ���ړ�
		g_player.state = RUN;						// �����j���Otrue

	}
	// ���ړ�-----------------------------------------
	else if (GetInput(LEFTMOVE))
	{
		g_player.pos.x -= g_player.moveSpeed;

		g_player.direction = Left;					// ���ړ�
		g_player.state = RUN;						// �����j���Otrue

	}
}


//=============================================================================
// �v���C���[�̗�������
//=============================================================================
void FallPlayer(void)
{
	g_player.pos.y += g_player.dropSpeed * 1.0f;	// �����x�I�ɉ��ֈړ��A�d�͉����x
	g_player.dropSpeed++;

	FallCheckHitPlayer();
}

//=============================================================================
// �v���C���[�̃W�����v����
//=============================================================================
void JumpPlayer(void)
{
	if (g_player.jumpForce >= 1)	//�W�����v�����瓙���ŏ㏸�B�������x�������x�I�ɑ�������̂Ŏ��Ԍo�߂ŗ�������
	{
		g_player.pos.y -= PLAYER_ACCELE;
		g_player.state = JUMP;		// �e�N�X�`���́u�W�����v�v

		g_player.textureSize = D3DXVECTOR2(PLAYER_TEXTURE_SIZE_X, PLAYER_TEXTURE_SIZE_Y);

	}

	/* ���n������W�����v���O�ɂ��ăW�����v��Ԃ���������B���̏�����FallPlayer���ɂ��� */
}

//=============================================================================
// �v���C���[�̍U��
//=============================================================================
void AttackPlayer(void)
{
	CHANGE_LIFE *life = GetLifeState();
	if (GetInput(ATTACK))
	{
		D3DXVECTOR3 pos = g_player.pos;
		if (g_player.countShot >= PLAYER_TIME_SHOT || g_player.jumpForce > 1)	// �A�˗p�̃J�E���g���K��l�𒴂��Ă��邩�A��i�W�����v���Ȃ�e����
		{
			//SetBullet(pos, g_player.direction, g_player.jumpForce);		// 
			SetBullet(g_player.pos);
			g_player.countShot = 0;	// 
			g_player.hp--;
			g_player.partsState--;
			*life = MINUS;
			ChangeLife(-1);
			//PlaySound(SOUND_LABEL_SE_shot2);
		}
	}
}

//=============================================================================
// �v���C���[�̃X�e�[�^�X�ɂ��e�N�X�`����ԕω�
//=============================================================================
void animPlayerState(int * animState, int * partsState)
{
	switch (*animState)
	{
	case IDLE:
		// Idle�A�j���[�V����
		g_player.Texture = g_pD3DTexture[*animState][*partsState];
		g_player.animeCnt.PatDivX = PLAYER_IDLE_TEXTURE_PATTERN_DIVIDE_X;
		g_player.animeCnt.PatDivY = PLAYER_IDLE_TEXTURE_PATTERN_DIVIDE_Y;
		g_player.animeCnt.AnimPatNum = PLAYER_IDLE_ANIM_PATTERN_NUM;
		g_player.animeCnt.TimeAnim = PLAYER_IDLE_TIME_ANIMATION;

		g_player.textureSize = D3DXVECTOR2(PLAYER_TEXTURE_SIZE_X, PLAYER_TEXTURE_SIZE_Y);
		break;

	case RUN:
		// Run�A�j���[�V����
		g_player.Texture = g_pD3DTexture[*animState][*partsState];
		g_player.animeCnt.PatDivX = PLAYER_RUN_TEXTURE_PATTERN_DIVIDE_X;
		g_player.animeCnt.PatDivY = PLAYER_RUN_TEXTURE_PATTERN_DIVIDE_Y;
		g_player.animeCnt.AnimPatNum = PLAYER_RUN_ANIM_PATTERN_NUM;
		g_player.animeCnt.TimeAnim = PLAYER_RUN_TIME_ANIMATION;

		g_player.textureSize = D3DXVECTOR2(PLAYER_TEXTURE_SIZE_X, PLAYER_TEXTURE_SIZE_Y);
		break;

	case JUMP:
		// Jump�A�j���[�V����
		g_player.Texture = g_pD3DTexture[*animState][*partsState];
		g_player.animeCnt.PatDivX = PLAYER_JUMP_TEXTURE_PATTERN_DIVIDE_X;
		g_player.animeCnt.PatDivY = PLAYER_JUMP_TEXTURE_PATTERN_DIVIDE_Y;
		g_player.animeCnt.AnimPatNum = PLAYER_JUMP_ANIM_PATTERN_NUM;
		g_player.animeCnt.TimeAnim = PLAYER_JUMP_TIME_ANIMATION;

		break;

	default:
		break;
	}
}

//=============================================================================
// �`�揈��
//=============================================================================
void DrawPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	if (g_player.use == true)			// �g�p���Ă����ԂȂ�`�悷��
	{
		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_player.Texture);

		// �|���S���̕`��
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_player.vertexWk, sizeof(VERTEX_2D));
	}
}

//=============================================================================
// ���_�̍쐬
//=============================================================================
HRESULT MakeVertexPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_���W�̐ݒ�
	g_player.vertexWk[0].vtx = D3DXVECTOR3(100.0f, 100.0f, 0.0f);
	g_player.vertexWk[1].vtx = D3DXVECTOR3(100.0f + PLAYER_TEXTURE_SIZE_X, 100.0f, 0.0f);
	g_player.vertexWk[2].vtx = D3DXVECTOR3(100.0f, 100.0f + PLAYER_TEXTURE_SIZE_Y, 0.0f);
	g_player.vertexWk[3].vtx = D3DXVECTOR3(100.0f + PLAYER_TEXTURE_SIZE_X, 100.0f + PLAYER_TEXTURE_SIZE_Y, 0.0f);

	// rhw�̐ݒ�
	g_player.vertexWk[0].rhw =
		g_player.vertexWk[1].rhw =
		g_player.vertexWk[2].rhw =
		g_player.vertexWk[3].rhw = 1.0f;

	// ���ˌ��̐ݒ�
	g_player.vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_player.vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_player.vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	g_player.vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);

	SetTexturePlayer(g_player.vertexWk, g_player.animeCnt.PatternAnim);

	return S_OK;
}

//=============================================================================
// �e�N�X�`�����W�ݒ�
//=============================================================================
void SetTexturePlayer(VERTEX_2D *Vtx, int cntPattern)
{
	// �e�N�X�`�����W�̐ݒ�
	int x = cntPattern % g_player.animeCnt.PatDivX;
	int y = cntPattern / g_player.animeCnt.PatDivX;
	float sizeX = 1.0f / g_player.animeCnt.PatDivX;
	float sizeY = 1.0f / g_player.animeCnt.PatDivY;

	if (g_player.direction == Right)
	{
		Vtx[0].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
		Vtx[1].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
		Vtx[2].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
		Vtx[3].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
	}
	else if (g_player.direction == Left)
	{
		Vtx[1].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY);
		Vtx[0].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY);
		Vtx[3].tex = D3DXVECTOR2((float)(x)* sizeX, (float)(y)* sizeY + sizeY);
		Vtx[2].tex = D3DXVECTOR2((float)(x)* sizeX + sizeX, (float)(y)* sizeY + sizeY);
	}

	// �c��HP�ɉ����ăv���C���[���Ԃ��Ȃ� && ���G���Ԓ��̓_��
	int nokoriHP = MAX_DIFFUSE / PLAYER_HP;
	int amari = MAX_DIFFUSE % PLAYER_HP;

	int flashing = (g_player.countInvincible + 1) % 2;

	Vtx[0].diffuse = D3DCOLOR_RGBA(255, (nokoriHP* g_player.hp) + amari, (nokoriHP* g_player.hp) + amari, flashing * 255);
	Vtx[1].diffuse = D3DCOLOR_RGBA(255, (nokoriHP* g_player.hp) + amari, (nokoriHP* g_player.hp) + amari, flashing * 255);
	Vtx[2].diffuse = D3DCOLOR_RGBA(255, (nokoriHP* g_player.hp) + amari, (nokoriHP* g_player.hp) + amari, flashing * 255);
	Vtx[3].diffuse = D3DCOLOR_RGBA(255, (nokoriHP* g_player.hp) + amari, (nokoriHP* g_player.hp) + amari, flashing * 255);

}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetVertexPlayer(VERTEX_2D *Vtx)
{
	D3DXVECTOR3 pos = g_player.pos + g_player.ofsPos;

	// ���_���W�̐ݒ�

	Vtx[0].vtx.x = g_player.pos.x - cosf(g_player.baseAngle + g_player.rot.z) * g_player.radius;
	Vtx[0].vtx.y = g_player.pos.y - sinf(g_player.baseAngle + g_player.rot.z) * g_player.radius;
	Vtx[0].vtx.z = 0.0f;

	Vtx[1].vtx.x = g_player.pos.x + cosf(g_player.baseAngle - g_player.rot.z) * g_player.radius;
	Vtx[1].vtx.y = g_player.pos.y - sinf(g_player.baseAngle - g_player.rot.z) * g_player.radius;
	Vtx[1].vtx.z = 0.0f;

	Vtx[2].vtx.x = g_player.pos.x - cosf(g_player.baseAngle - g_player.rot.z) * g_player.radius;
	Vtx[2].vtx.y = g_player.pos.y + sinf(g_player.baseAngle - g_player.rot.z) * g_player.radius;
	Vtx[2].vtx.z = 0.0f;

	Vtx[3].vtx.x = g_player.pos.x + cosf(g_player.baseAngle + g_player.rot.z) * g_player.radius;
	Vtx[3].vtx.y = g_player.pos.y + sinf(g_player.baseAngle + g_player.rot.z) * g_player.radius;
	Vtx[3].vtx.z = 0.0f;
}

//=============================================================================
// �v���C���[�擾�֐�
//=============================================================================
PLAYER *GetPlayer(void)
{
	return(&g_player);
}
