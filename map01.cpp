/*******************************************************************************
* scanf ��warning�h�~
*******************************************************************************/
#define _CRT_SECURE_NO_WARNINGS

/*******************************************************************************
* �C���N���[�h�t�@�C��
*******************************************************************************/
#include <stdio.h>
#include "main.h"
#include "file.h"
#include "map.h"
#include "checkhit.h"
#include "player.h"
#include "input.h"
#include "playerTest.h"

/*******************************************************************************
* �}�N����`
*******************************************************************************/
#define MAP_TEXTURE                 _T("data/TEXTURE/maptip.jpg")    // �}�b�v�`�b�v�摜

/*******************************************************************************
* �\���̒�`
*******************************************************************************/

/*******************************************************************************
* �v���g�^�C�v�錾
*******************************************************************************/
HRESULT MakeVertexMap(int x, int y, int mapData);
void SetTexture(int x, int y, int mapData);
void SetVertexMapChip(int y, int x, int mapData);

/*******************************************************************************
* �O���[�o���ϐ�
*******************************************************************************/
static LPDIRECT3DTEXTURE9		g_pD3DTextureMap = NULL;		// �e�N�X�`���ւ̃|���S��
MAP mapBlock[MAP_MAXDATA][SIZE_Y][SIZE_X];


// ����������
HRESULT InitMap(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	//int *data = GetMapDataType();
	//int *data2 = GetMapDataType2();
	//int *data3 = GetMapDataType3();

	int *data = GetFileData();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,						// �f�o�C�X�̃|�C���^
		MAP_TEXTURE,										// �t�@�C���̖��O
		&g_pD3DTextureMap);	// �ǂݍ��ރ������̃|�C���^

	for (int k = 0; k < MAP_MAXDATA; k++)
	{
		for (int i = 0; i < SIZE_Y; i++)
		{
			for (int j = 0; j < SIZE_X; j++)
			{
				mapBlock[k][i][j].use = true;
				mapBlock[k][i][j].Texture = g_pD3DTextureMap;
				mapBlock[k][i][j].pos = D3DXVECTOR3((k * (SCREEN_WIDTH * 2)) + MAP_TEXTURE_SIZE_X + (j * MAP_TEXTURE_SIZE_X * 2),
					MAP_TEXTURE_SIZE_Y + (i * MAP_TEXTURE_SIZE_Y * 2), 0.0f);

				mapBlock[k][i][j].type = *data;
				//switch (k)
				//{
				//case 0:
				//	mapBlock[k][i][j].type = *data;
				//	break;
				//case 1:
				//	mapBlock[k][i][j].type = *data2;
				//	break;
				//case 2:
				//	mapBlock[k][i][j].type = *data3;
				//default:
				//	break;
				//}
				mapBlock[k][i][j].move = D3DXVECTOR3(0.0f, 2.0f, 0.0f);	// �ړ���
				MakeVertexMap(j, i, k);

				//switch (k)
				//{
				//case 0:
				//	data++;
				//	break;
				//case 1:
				//	data2++;
				//	break;
				//case 2:
				//	data3++;
				//default:
				//	break;
				//}
				*data++;
			}
		}
	}
	return S_OK;
}


// �I������
void UninitMap(void)
{
	if (g_pD3DTextureMap != NULL)
	{
		// �e�N�X�`���̊J��
		g_pD3DTextureMap->Release();
		g_pD3DTextureMap = NULL;
	}
}

// �X�V����
void UpdateMap(void)
{
	PLAYER *player = GetPlayer();

	for (int k = 0; k < MAP_MAXDATA; k++)
	{
		for (int i = 0; i < SIZE_Y; i++)
		{
			for (int j = 0; j < SIZE_X; j++)
			{
				
				//mapBlock[k][i][j].pos.x -= player->scrollPos.x;
				if (mapBlock[k][i][j].type == BLOCK10)
				{
					mapBlock[k][i][j].pos.y -= mapBlock[k][i][j].move.y;
					if (mapBlock[k][i][j].pos.y < -MAP_TEXTURE_SIZE_Y)
					{
						mapBlock[k][i][j].pos.y = SCREEN_HEIGHT + MAP_TEXTURE_SIZE_Y * 2;
					}
				}
				SetVertexMapChip(i, j, k);
			}
		}
	}
}

// �`�揈��
void DrawMap(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int k = 0; k < MAP_MAXDATA; k++)
	{
		for (int i = 0; i < SIZE_Y; i++)
		{
			for (int j = 0; j < SIZE_X; j++)
			{
				if (mapBlock[k][i][j].use == true)		// �g�p���Ă����ԂȂ�`�悷��
				{
					// �e�N�X�`���̐ݒ�
					pDevice->SetTexture(0, mapBlock[k][i][j].Texture);

					// �|���S���̕`��
					pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, mapBlock[k][i][j].vertexWk, sizeof(VERTEX_2D));

				}
			}
		}
	}
}

HRESULT MakeVertexMap(int x, int y, int mapData)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_���W�̐ݒ�
	mapBlock[mapData][y][x].vertexWk[0].vtx = D3DXVECTOR3(MAP_TEXTURE_SIZE_X + (x * MAP_TEXTURE_SIZE_X * 2) - MAP_TEXTURE_SIZE_X, MAP_TEXTURE_SIZE_Y + (y * MAP_TEXTURE_SIZE_Y * 2) - MAP_TEXTURE_SIZE_Y, 0.0f);
	mapBlock[mapData][y][x].vertexWk[1].vtx = D3DXVECTOR3(MAP_TEXTURE_SIZE_X + (x * MAP_TEXTURE_SIZE_X * 2) + MAP_TEXTURE_SIZE_X, MAP_TEXTURE_SIZE_Y + (y * MAP_TEXTURE_SIZE_Y * 2) - MAP_TEXTURE_SIZE_Y, 0.0f);
	mapBlock[mapData][y][x].vertexWk[2].vtx = D3DXVECTOR3(MAP_TEXTURE_SIZE_X + (x * MAP_TEXTURE_SIZE_X * 2) - MAP_TEXTURE_SIZE_X, MAP_TEXTURE_SIZE_Y + (y * MAP_TEXTURE_SIZE_Y * 2) + MAP_TEXTURE_SIZE_Y, 0.0f);
	mapBlock[mapData][y][x].vertexWk[3].vtx = D3DXVECTOR3(MAP_TEXTURE_SIZE_X + (x * MAP_TEXTURE_SIZE_X * 2) + MAP_TEXTURE_SIZE_X, MAP_TEXTURE_SIZE_Y + (y * MAP_TEXTURE_SIZE_Y * 2) + MAP_TEXTURE_SIZE_Y, 0.0f);

	// rhw�̐ݒ�
	mapBlock[mapData][y][x].vertexWk[0].rhw =
	mapBlock[mapData][y][x].vertexWk[1].rhw =
	mapBlock[mapData][y][x].vertexWk[2].rhw =
	mapBlock[mapData][y][x].vertexWk[3].rhw = 1.0f;

	if (mapBlock[mapData][y][x].type != -1)
	{
		// ���ˌ��̐ݒ�
		mapBlock[mapData][y][x].vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		mapBlock[mapData][y][x].vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		mapBlock[mapData][y][x].vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
		mapBlock[mapData][y][x].vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 255);
	}

	else
	{
		// ���ˌ��̐ݒ�
		mapBlock[mapData][y][x].vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 0);
		mapBlock[mapData][y][x].vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 0);
		mapBlock[mapData][y][x].vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 0);
		mapBlock[mapData][y][x].vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 0);
	}

	switch (mapBlock[mapData][y][x].type)
	{
	case BLOCK0:
		// �e�N�X�`�����W�̐ݒ�
		mapBlock[mapData][y][x].vertexWk[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		mapBlock[mapData][y][x].vertexWk[1].tex = D3DXVECTOR2(1.0f / MAP_TEXTURE_PATTERN_DIVIDE_X, 0.0f);
		mapBlock[mapData][y][x].vertexWk[2].tex = D3DXVECTOR2(0.0f, 1.0f / MAP_TEXTURE_PATTERN_DIVIDE_Y);
		mapBlock[mapData][y][x].vertexWk[3].tex = D3DXVECTOR2(1.0f / MAP_TEXTURE_PATTERN_DIVIDE_X, 1.0f / MAP_TEXTURE_PATTERN_DIVIDE_Y);
		break;

	case BLOCK1:
		// �e�N�X�`�����W�̐ݒ�
		mapBlock[mapData][y][x].vertexWk[0].tex = D3DXVECTOR2(1.0f / MAP_TEXTURE_PATTERN_DIVIDE_X, 0.0f / MAP_TEXTURE_PATTERN_DIVIDE_Y);
		mapBlock[mapData][y][x].vertexWk[1].tex = D3DXVECTOR2(2.0f / MAP_TEXTURE_PATTERN_DIVIDE_X, 0.0f / MAP_TEXTURE_PATTERN_DIVIDE_Y);
		mapBlock[mapData][y][x].vertexWk[2].tex = D3DXVECTOR2(1.0f / MAP_TEXTURE_PATTERN_DIVIDE_X, 1.0f / MAP_TEXTURE_PATTERN_DIVIDE_Y);
		mapBlock[mapData][y][x].vertexWk[3].tex = D3DXVECTOR2(2.0f / MAP_TEXTURE_PATTERN_DIVIDE_X, 1.0f / MAP_TEXTURE_PATTERN_DIVIDE_Y);
		break;

	case BLOCK10:
		// �e�N�X�`�����W�̐ݒ�
		mapBlock[mapData][y][x].vertexWk[0].tex = D3DXVECTOR2(0.0f, 1.0f / MAP_TEXTURE_PATTERN_DIVIDE_Y);
		mapBlock[mapData][y][x].vertexWk[1].tex = D3DXVECTOR2(1.0f / MAP_TEXTURE_PATTERN_DIVIDE_X, 1.0f / MAP_TEXTURE_PATTERN_DIVIDE_Y);
		mapBlock[mapData][y][x].vertexWk[2].tex = D3DXVECTOR2(0.0f, 2.0f / MAP_TEXTURE_PATTERN_DIVIDE_Y);
		mapBlock[mapData][y][x].vertexWk[3].tex = D3DXVECTOR2(1.0f / MAP_TEXTURE_PATTERN_DIVIDE_X, 2.0f / MAP_TEXTURE_PATTERN_DIVIDE_Y);
		break;

	default:
		break;
	}

	return S_OK;
}

//=============================================================================
// ���_���W�̐ݒ�
//=============================================================================
void SetVertexMapChip(int y, int x, int mapData)
{
	// ���_���W�̐ݒ�
	mapBlock[mapData][y][x].vertexWk[0].vtx.x = mapBlock[mapData][y][x].pos.x - MAP_TEXTURE_SIZE_X;
	mapBlock[mapData][y][x].vertexWk[0].vtx.y = mapBlock[mapData][y][x].pos.y - MAP_TEXTURE_SIZE_Y;
	mapBlock[mapData][y][x].vertexWk[0].vtx.z = 0.0f;

	mapBlock[mapData][y][x].vertexWk[1].vtx.x = mapBlock[mapData][y][x].pos.x + MAP_TEXTURE_SIZE_X;
	mapBlock[mapData][y][x].vertexWk[1].vtx.y = mapBlock[mapData][y][x].pos.y - MAP_TEXTURE_SIZE_Y;
	mapBlock[mapData][y][x].vertexWk[1].vtx.z = 0.0f;

	mapBlock[mapData][y][x].vertexWk[2].vtx.x = mapBlock[mapData][y][x].pos.x - MAP_TEXTURE_SIZE_X;
	mapBlock[mapData][y][x].vertexWk[2].vtx.y = mapBlock[mapData][y][x].pos.y + MAP_TEXTURE_SIZE_Y;
	mapBlock[mapData][y][x].vertexWk[2].vtx.z = 0.0f;

	mapBlock[mapData][y][x].vertexWk[3].vtx.x = mapBlock[mapData][y][x].pos.x + MAP_TEXTURE_SIZE_X;
	mapBlock[mapData][y][x].vertexWk[3].vtx.y = mapBlock[mapData][y][x].pos.y + MAP_TEXTURE_SIZE_Y;
	mapBlock[mapData][y][x].vertexWk[3].vtx.z = 0.0f;
}

void SetTexture(int x, int y, int mapData)
{
	// ���ˌ��̐ݒ�
	mapBlock[mapData][y][x].vertexWk[0].diffuse = D3DCOLOR_RGBA(255, 255, 255, 0);
	mapBlock[mapData][y][x].vertexWk[1].diffuse = D3DCOLOR_RGBA(255, 255, 255, 0);
	mapBlock[mapData][y][x].vertexWk[2].diffuse = D3DCOLOR_RGBA(255, 255, 255, 0);
	mapBlock[mapData][y][x].vertexWk[3].diffuse = D3DCOLOR_RGBA(255, 255, 255, 0);
}

MAP *GetMapData(void)
{
	return &mapBlock[0][0][0];
}