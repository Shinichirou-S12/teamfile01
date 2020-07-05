//=============================================================================
//
// �}�b�v�t�@�C������ [file.h]
// Author : kitade mayumi
//
//=============================================================================

#include "main.h"

#define SIZE_X	(20)
#define SIZE_Y	(15)

#define MAP_TEXTURE_SIZE_X			(16.0f)		    // �e�N�X�`���T�C�Y
#define MAP_TEXTURE_SIZE_Y			(16.0f)		    // ����
#define MAP_TEXTURE_PATTERN_DIVIDE_X	(2)			// �A�j���p�^�[���̃e�N�X�`�����������iX)
#define MAP_TEXTURE_PATTERN_DIVIDE_Y	(2)			// �A�j���p�^�[���̃e�N�X�`�����������iY)

enum BLOCKTYPE
{
	BLOCK0,
	BLOCK1,
	BLOCK2,
	BLOCK3,
	BLOCK4,
	BLOCK5,
	BLOCK6,
	BLOCK7,
	BLOCK8,
	BLOCK9,
	NONE10,
	BLOCK11,
	BLOCK12,
	BLOCK13,
	GLASS14,
	GLASS15,
	MAXTYPE
};

typedef struct
{
	bool			use;						// true:�g�p  false:���g�p
	D3DXVECTOR3		pos;						// �|���S���̈ړ���
	D3DXVECTOR3		rot;						// �|���S���̉�]��
	int				type;

	LPDIRECT3DTEXTURE9	Texture;				// �e�N�X�`�����
	VERTEX_2D		vertexWk[NUM_VERTEX];		// ���_���i�[���[�N
}MAP;


HRESULT InitFile(void);
MAP *GetMapData(void);
HRESULT InitMap(void);
void UninitMap(void);
void UpdateMap(void);
void DrawMap(void);