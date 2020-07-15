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
int data[SIZE_Y][SIZE_X]  =	{ NULL };
int data2[SIZE_Y][SIZE_X] = { NULL };
int data3[SIZE_Y][SIZE_X] = { NULL };

/*******************************************************************************
 関数名:	HRESULT InitFile(void)
 引数:		無し
 戻り値:
 説明:
*******************************************************************************/
HRESULT InitFile(void)
{
	data[SIZE_Y][SIZE_X] = { 0 };
	FILE *fp;
	char buff[SIZE_Y * SIZE_X], *pbuff;

	fp = fopen("data/DATA2/maptest01.csv", "r");

	if (fp == NULL)
	{
		return S_FALSE;
	}

	for (int i = 0; i < SIZE_Y; i++)
	{
		fgets(buff, SIZE_Y * SIZE_X, fp);
		pbuff = buff;
		for (int j = 0; j < SIZE_X; j++)
		{
			data[i][j] = (int)strtod(pbuff, &pbuff);
			pbuff++;
		}
	}

	fclose(fp);

	return S_OK;
}

HRESULT InitFile2(void)
{
	data2[SIZE_Y][SIZE_X] = { 0 };
	FILE *fp;
	char buff[SIZE_Y * SIZE_X], *pbuff;

	fp = fopen("data/DATA2/maptest02.csv", "r");

	if (fp == NULL)
	{
		return S_FALSE;
	}

	for (int i = 0; i < SIZE_Y; i++)
	{
		fgets(buff, SIZE_Y * SIZE_X, fp);
		pbuff = buff;
		for (int j = 0; j < SIZE_X; j++)
		{
			data2[i][j] = (int)strtod(pbuff, &pbuff);
			pbuff++;
		}
	}

	fclose(fp);

	return S_OK;
}

HRESULT InitFile3(void)
{
	data3[SIZE_Y][SIZE_X] = { 0 };
	FILE *fp;
	char buff[SIZE_Y * SIZE_X], *pbuff;

	fp = fopen("data/DATA2/maptest03.csv", "r");

	if (fp == NULL)
	{
		return S_FALSE;
	}

	for (int i = 0; i < SIZE_Y; i++)
	{
		fgets(buff, SIZE_Y * SIZE_X, fp);
		pbuff = buff;
		for (int j = 0; j < SIZE_X; j++)
		{
			data3[i][j] = (int)strtod(pbuff, &pbuff);
			pbuff++;
		}
	}

	fclose(fp);

	return S_OK;
}

int *GetMapDataType(void)
{
	return &data[0][0];
}

int *GetMapDataType2(void)
{
	return &data2[0][0];
}

int *GetMapDataType3(void)
{
	return &data3[0][0];
}