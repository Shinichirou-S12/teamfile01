/*******************************************************************************
* scanf のwarning防止
*******************************************************************************/
#define _CRT_SECURE_NO_WARNINGS

/*******************************************************************************
* インクルードファイル
*******************************************************************************/
#include <stdio.h>
#include "main.h"
#include "file.h"
#include "map.h"

/*******************************************************************************
* グローバル変数
*******************************************************************************/

// ファイルリスト
const char *const fileName[MAP_MAXDATA]=
{
	"data/DATA2/maptest01.csv",
	"data/DATA2/maptest02.csv",
	"data/DATA2/maptest03.csv",
};

FILE *fp[MAP_MAXDATA];

int datas[MAP_MAXDATA][SIZE_Y][SIZE_X] = { 0 };

// ファイルを開く処理
HRESULT OpenFile(void)
{
	char buff[SIZE_Y * SIZE_X], *pbuff;

	for (int k = 0; k < MAP_MAXDATA; k++)
	{
		fp[k] = fopen(fileName[k], "r");

		if (fp[k] == NULL)
		{
			return S_FALSE;
		}

		for (int i = 0; i < SIZE_Y; i++)
		{
			fgets(buff, SIZE_Y * SIZE_X, fp[k]);
			pbuff = buff;
			for (int j = 0; j < SIZE_X; j++)
			{
				datas[k][i][j] = (int)strtod(pbuff, &pbuff);
				pbuff++;
			}
		}

		fclose(fp[k]);
	}

	return S_OK;
}

// データ取得関数
int *GetFileData(void)
{
	return &datas[0][0][0];
}
