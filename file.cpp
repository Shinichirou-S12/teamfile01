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

// CSVのマップファイルリスト
const char *const fileName[MAP_MAXDATA]=
{
	"data/DATA2/maptest01.csv",
	"data/DATA2/maptest02.csv",
	"data/DATA2/maptest03.csv",
	"data/DATA2/maptest04.csv"
};

// CSVのボーナスマップファイルリスト
const char *const fileBonusName[MAP_BONUS_MAXDATA] =
{
	"data/DATA2/mapbonustest01.csv",
	"data/DATA2/mapbonustest03.csv",
	"data/DATA2/mapbonustest02.csv",
	"data/DATA2/mapbonustest04.csv"
};

FILE *fp[MAP_MAXDATA];

int datas[MAP_MAXDATA][SIZE_Y][SIZE_X] = { NULL };
int bonusData[MAP_BONUS_MAXDATA][SIZE_Y][SIZE_X] = { NULL };

// ファイルを開く処理
HRESULT OpenFile(void)
{
	// CSVファイルを受け取る用のbufferと一文字ずつメモリを動かすためのポインタ
	char buff[SIZE_Y * SIZE_X], *pbuff;

	for (int k = 0; k < MAP_MAXDATA; k++)
	{
		// 開くファイルをリストから指定
		fp[k] = fopen(fileName[k], "r");

		if (fp[k] == NULL)
		{
			return S_FALSE;
		}

		// 一文字ずつデータに値を格納する（csvファイルのカンマは除く）
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

// ボーナスファイルを開く処理
HRESULT OpenBonusFile(void)
{
	// CSVファイルを受け取る用のbufferと一文字ずつメモリを動かすためのポインタ
	char buff[SIZE_Y * SIZE_X], *pbuff;

	for (int k = 0; k < MAP_BONUS_MAXDATA; k++)
	{
		// 開くファイルをリストから指定
		fp[k] = fopen(fileBonusName[k], "r");

		if (fp[k] == NULL)
		{
			return S_FALSE;
		}

		// 一文字ずつデータに値を格納する（csvファイルのカンマは除く）
		for (int i = 0; i < SIZE_Y; i++)
		{
			fgets(buff, SIZE_Y * SIZE_X, fp[k]);
			pbuff = buff;
			for (int j = 0; j < SIZE_X; j++)
			{
				bonusData[k][i][j] = (int)strtod(pbuff, &pbuff);
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

// データ取得関数
int *GetBonusFileData(void)
{
	return &bonusData[0][0][0];
}