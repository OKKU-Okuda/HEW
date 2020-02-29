//=============================================================================
//
// �f�o�b�O�\������ [debugproc.cpp]
// Author : ���c �^�K
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "main.h"
#include "input.h"
#include "debugproc.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
static LPD3DXFONT	g_pD3DXFont = NULL;			// �t�H���g�ւ̃|�C���^
static VERTEX_2D	g_VtxBack[NUM_VERTEX];		// ���̔w�i
static RECT		g_Rect;							// �t�H���g�\���͈�
static char		g_aStrDebug[2048] = {"\0"};		// �f�o�b�O���
static bool		g_isShow;						// �\������H
//=============================================================================
// �f�o�b�O�\�������̏�����
//=============================================================================
HRESULT InitDebugProc(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	HRESULT hr;

	// ���\���p�t�H���g��ݒ�
	hr = D3DXCreateFont(pDevice, 30, 15, 0, 0, FALSE, SHIFTJIS_CHARSET,
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &g_pD3DXFont);

	// ���N���A
	memset(g_aStrDebug, 0, sizeof g_aStrDebug);

	// �w�i���h�荂����
	{
		g_VtxBack[0].rhw =
			g_VtxBack[1].rhw =
			g_VtxBack[2].rhw =
			g_VtxBack[3].rhw = 1.0f;

		g_VtxBack[0].tex = Vec2(0, 0);
		g_VtxBack[1].tex = Vec2(1, 0);
		g_VtxBack[2].tex = Vec2(0, 1);
		g_VtxBack[3].tex = Vec2(1, 1);

		g_VtxBack[0].diffuse =
			g_VtxBack[1].diffuse =
			g_VtxBack[2].diffuse =
			g_VtxBack[3].diffuse = Color(0.0f, 0.0f, 0.0f, 0.3f);

		g_VtxBack[0].vtx = Vec3(0.0f, 0.0f, 0.0f);
		g_VtxBack[1].vtx = Vec3(100.0f, 0.0f, 0.0f);
		g_VtxBack[2].vtx = Vec3(0.0f, 100.0f, 0.0f);
		g_VtxBack[3].vtx = Vec3(100.0f, 100.0f, 0.0f);
	}

	
	g_isShow = false;

	g_Rect.left = 0;
	g_Rect.right = 100;
	g_Rect.top = 0;
	g_Rect.bottom = 100;// �����l

	return hr;
}

//=============================================================================
// �f�o�b�O�\�������̏I������
//=============================================================================
void UninitDebugProc(void)
{
	if(g_pD3DXFont != NULL)
	{// ���\���p�t�H���g�̊J��
		g_pD3DXFont->Release();
		g_pD3DXFont = NULL;
	}
}

//=============================================================================
// �f�o�b�O�\�������̍X�V����
//=============================================================================
void UpdateDebugProc(void)
{
	if (GetKeyboardTrigger(DIK_F1))
	{
		g_isShow = !g_isShow;
	}

	if (g_isShow)
	{// �\�����̍X�V
		// ���\��
		g_pD3DXFont->DrawText(NULL, g_aStrDebug, -1, &g_Rect, DT_LEFT| DT_CALCRECT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));

		g_VtxBack[1].vtx.x =
			g_VtxBack[3].vtx.x = (float)g_Rect.right;

		g_VtxBack[2].vtx.y =
			g_VtxBack[3].vtx.y = (float)g_Rect.bottom;
	}
}

//=============================================================================
// �f�o�b�O�\�������̕`�揈��
//=============================================================================
void DrawDebugProc(void)
{
	if (g_isShow)
	{
		D3DDEVICE;

		pDevice->SetTexture(0, NULL);
		pDevice->SetFVF(FVF_VERTEX_2D);
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, NUM_POLYGON, g_VtxBack, sizeof(VERTEX_2D));

		// ���\��
		g_pD3DXFont->DrawText(NULL, g_aStrDebug, -1, &g_Rect, DT_LEFT, D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff));
	}

	// ���N���A
	memset(g_aStrDebug, 0, sizeof g_aStrDebug);
}

//=============================================================================
// �f�o�b�O�\���̓o�^
//=============================================================================
void PrintDebugProc(char *fmt,...)
{
#if 0
	long *pParam;
	static char aBuf[256];

	pParam = (long*)&fmt;
	sprintf(aBuf, fmt, pParam[1], pParam[2], pParam[3], pParam[4],
									pParam[5], pParam[6], pParam[7], pParam[8],
									pParam[9], pParam[10], pParam[11], pParam[12]);
#else
	va_list list;			// �ψ�������������ׂɎg�p����ϐ�
	char *pCur;
	char aBuf[256]={"\0"};
	char aWk[64];

	if (g_isShow == false)return;	// �\�����Ȃ��ꍇ�͂��̏������΂�

	// �ψ����ɃA�N�Z�X����O�̏�������
	va_start(list, fmt);

	pCur = fmt;
	for( ; *pCur; ++pCur)
	{
		if(*pCur != '%')
		{
			sprintf(aWk, "%c", *pCur);
		}
		else
		{
			pCur++;

			switch(*pCur)
			{
			case 'd':
				// �ψ����ɃA�N�Z�X���Ă��̕ϐ������o������
				sprintf(aWk, "%d", va_arg(list, int));
				break;

			case 'f':
				// �ψ����ɃA�N�Z�X���Ă��̕ϐ������o������
				sprintf(aWk, "%.2f", va_arg(list, double));		// double�^�Ŏw��
				break;

			case 's':
				// �ψ����ɃA�N�Z�X���Ă��̕ϐ������o������
				sprintf(aWk, "%s", va_arg(list, char*));
				break;

			case 'c':
				// �ψ����ɃA�N�Z�X���Ă��̕ϐ������o������
				sprintf(aWk, "%c", va_arg(list, char));
				break;

			case 'v':
				if (strncmp(pCur, "vec2",4) == 0)
				{
					Vec2 vec = va_arg(list, Vec2);
					pCur += 3;
					sprintf(aWk, "X:%.2f Y:%.2f", vec.x, vec.y);
				}
				else if (strncmp(pCur, "vec3", 4) == 0)
				{
					Vec3 vec = va_arg(list, Vec3);
					pCur += 3;
					sprintf(aWk, "X:%.2f Y:%.2f Z:%.2f", vec.x, vec.y, vec.z);
				}
				else if (strncmp(pCur, "vec4", 4) == 0)
				{
					Vec4 vec = va_arg(list, Vec4);
					pCur += 3;
					sprintf(aWk, "X:%.2f Y:%.2f Z:%.2f W:%.2f", vec.x, vec.y, vec.z, vec.w);
				}

				break;
			default:
				sprintf(aWk, "%c", *pCur);
				break;
			}
		}
		strcat(aBuf, aWk);
	}

	// �ψ����ɃA�N�Z�X������̏I������
	va_end(list);

	// �Ō�ɊJ�Ƃ�t�^
	strcat(aBuf, "\n");
	// �A��
	if((strlen(g_aStrDebug) + strlen(aBuf)) < sizeof g_aStrDebug - 1)
	{
		strcat(g_aStrDebug, aBuf);
	}
#endif
}

