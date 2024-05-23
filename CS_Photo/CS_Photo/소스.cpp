#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include <Windows.h>

using namespace std;
typedef unsigned long long QWORD;
void SaveHBITMAPToFile(BYTE* ptr, LPCSTR lpszFileName, DWORD NX,DWORD X, DWORD Y)
{
    FILE* fout;
    fopen_s(&fout, lpszFileName, "wb");

    BYTE temp1 = 0;
    WORD temp2;
    DWORD temp4;
    DWORD R = NX - 3*X;

    temp2 = 0x4D42;
    fwrite(&temp2, 2, 1, fout); // "BM"
    temp4 = 0x36 + NX * Y;
    fwrite(&temp4, 4, 1, fout);
    temp4 = 0;
    fwrite(&temp4, 4, 1, fout);
    temp4 = 0x36;
    fwrite(&temp4, 4, 1, fout);

    temp4 = 40;
    fwrite(&temp4, 4, 1, fout);
    temp4 = X;
    fwrite(&temp4, 4, 1, fout);
    temp4 = Y;
    fwrite(&temp4, 4, 1, fout);
    temp2 = 1;
    fwrite(&temp2, 2, 1, fout);
    temp2 = 24;
    fwrite(&temp2, 2, 1, fout);
    temp4 = 0;
    fwrite(&temp4, 4, 1, fout);
    temp4 = NX * Y;
    fwrite(&temp4, 4, 1, fout);
    temp4 = 0x2E20;
    fwrite(&temp4, 4, 1, fout);
    fwrite(&temp4, 4, 1, fout);
    temp4 = 0;
    fwrite(&temp4, 4, 1, fout);
    fwrite(&temp4, 4, 1, fout);

    for (int j = 0; j < Y; j++)
    {
        for (int i = 0; i < X; i++)
        {
            BYTE Arr[3] = { 0 };
            int temp = j * NX + i * 3;
            Arr[0] = ptr[temp + 0];
            Arr[1] = ptr[temp + 1];
            Arr[2] = ptr[temp + 2];
            fwrite(Arr, 1, 3, fout);
        }
        if (R == 1)
        {
            temp1 = 0;
            fwrite(&temp1, 1, 1, fout);
        }
        else if (R == 2)
        {
            temp2 = 0;
            fwrite(&temp2, 2, 1, fout);
        }
        else if (R == 3)
        {
            temp1 = 0;
            fwrite(&temp1, 1, 1, fout);
            temp2 = 0;
            fwrite(&temp2, 2, 1, fout);
        }
    }

    fclose(fout);
}

int main(int argc, char* argv[])
{

    printf("--------------------------------------\n     ��`+��CS_Photo v1.2 ��+.��\n--------------------------------------\n\t\t\tMade By Ninfia\n\n");

    int Mode;
    Retry:
    cout << "�̹��� ��ȯ ��带 �����ϼ���" << endl << "1. �̹����� ���� �����ͷ� ��ȯ (Shape)" << endl << "2. �̹����� �׷��� �����ͷ� ��ȯ (CGRP)" << endl;
    cout << "3. �̹����� ����ũ �����ͷ� ��ȯ (Mask)" << endl;
    cin >> Mode;
    if (Mode != 1 && Mode != 2 && Mode != 3)
        goto Retry;

    if (Mode == 1)
    {
        while (true)
        {
            char input[512];
            char Shape[512];
            int Shapelen;
            cout << "\n������ �̹��� �����̸��� �Է��ϼ��� (0 �Է½� ����)\n";
            cin >> input;
            if (!strcmp(input, "0"))
                break;
            else
            {
                char iname[512];
                strcpy_s(iname, 512, input);
                int ilength = strlen(iname);
                iname[ilength - 4] = '_';
                iname[ilength - 3] = 'o';
                iname[ilength - 2] = 'u';
                iname[ilength - 1] = 't';
                iname[ilength - 0] = '.';
                iname[ilength + 1] = 't';
                iname[ilength + 2] = 'x';
                iname[ilength + 3] = 't';
                iname[ilength + 4] = 0;

                char iname2[512];
                strcpy_s(iname2, 512, input);
                int ilength2 = strlen(iname2);
                iname2[ilength2 - 4] = '_';
                iname2[ilength2 - 3] = 'p';
                iname2[ilength2 - 2] = 'r';
                iname2[ilength2 - 1] = 'e';
                iname2[ilength2 - 0] = 'v';
                iname2[ilength2 + 1] = 'i';
                iname2[ilength2 + 2] = 'e';
                iname2[ilength2 + 3] = 'w';
                iname2[ilength2 + 4] = '.';
                iname2[ilength2 + 5] = 'b';
                iname2[ilength2 + 6] = 'm';
                iname2[ilength2 + 7] = 'p';
                iname2[ilength2 + 8] = 0;

                int Del = 0;
                FILE* fRet;
                fopen_s(&fRet, iname, "w+");
                strcpy_s(Shape, 512, input);
                Shapelen = strlen(input);
                Shape[Shapelen - 4] = 0;
                fprintf(fRet, "%s = {        ",Shape);

                // BMP Size : XxY �ܻ� �ȷ�Ʈ 
                // BMP Filter Slider 0 ~ 255, BMP Preview 

                BITMAP BMP;
                HBITMAP HBMP = (HBITMAP)LoadImageA(0, input, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
                GetObject(HBMP, sizeof(BMP), (LPSTR)&BMP);
                BYTE* IPtr = (BYTE*)BMP.bmBits;
                DWORD X = BMP.bmWidth, Y = BMP.bmHeight;
                int Count = 0;
                if (BMP.bmBitsPixel != 24)
                {
                    cout << "24��Ʈ BMP�� �ƴմϴ�. �ٸ� �̹����� �������ּ���" << endl;
                    Del = 1;
                }
                else
                {
                    cout << input << " �ε�Ϸ� (" << X << "x" << Y << ", True Color BMP)" << endl;

                    DWORD NX, X3 = (X*3)%4;
                    if (X3 == 0)
                        NX = X * 3;
                    else if (X3 == 1)
                        NX = X * 3 + 3;
                    else if (X3 == 2)
                        NX = X * 3 + 2;
                    else
                        NX = X * 3 + 1;

                    RetryX:
                    int Direction;
                    cout << "��� ���� ��밪 �Է� ������ �����ϼ���" << endl;
                    cout << "1. �Է°� �̻� ��⸸ �����" << endl;
                    cout << "2. �Է°� ���� ��⸸ �����" << endl;
                    cin >> Direction;
                    if (Direction != 1 && Direction != 2)
                        goto RetryX;

                    int Filter;
                    if (Direction == 1)
                        cout << "��� ���� ��밪�� �Է��ϼ��� (0~255, �Է°� �̻� ��⸸ �����)" << endl;
                    else
                        cout << "��� ���� ��밪�� �Է��ϼ��� (0~255, �Է°� ���� ��⸸ �����)" << endl;
                    cin >> Filter;

                    BYTE* BPtr = (BYTE*)BMP.bmBits;
                    BYTE* NPtr = (BYTE*)malloc(NX * Y);

                    for (int j = Y - 1; j >= 0; j--)
                    {
                        for (int i = 0; i < X; i++)
                        {
                            int temp = j * NX + i * 3;
                            int Bright = (BPtr[temp] + BPtr[temp + 1] + BPtr[temp + 2]) / 3;
                            if (Direction == 1)
                            {
                                if (Bright >= Filter)
                                {
                                    Count++;
                                    // Write Shape
                                    fprintf(fRet, ",{%d, %d}", i, Y - 1 - j);
                                    // Write Preview
                                    NPtr[temp] = 0xFF;
                                    NPtr[temp + 1] = 0xFF;
                                    NPtr[temp + 2] = 0xFF;
                                }
                                else
                                {
                                    NPtr[temp] = 0;
                                    NPtr[temp + 1] = 0;
                                    NPtr[temp + 2] = 0;
                                }
                            }
                            else
                            {
                                if (Bright <= Filter)
                                {
                                    Count++;
                                    // Write Shape
                                    fprintf(fRet, ",{%d, %d}", i, Y - 1 - j);
                                    // Write Preview
                                    NPtr[temp] = 0xFF;
                                    NPtr[temp + 1] = 0xFF;
                                    NPtr[temp + 2] = 0xFF;
                                }
                                else
                                {
                                    NPtr[temp] = 0;
                                    NPtr[temp + 1] = 0;
                                    NPtr[temp + 2] = 0;
                                }
                            }
                        }
                    }
                    SaveHBITMAPToFile(NPtr, iname2, NX, X, Y);
                    free(NPtr);
                }
                DeleteObject(HBMP);
                fprintf(fRet, "}");
                fseek(fRet, Shapelen, 0);
                fprintf(fRet, "%d", Count);
                fclose(fRet);
                if (Del == 1)
                    DeleteFileA(iname);
                else
                    cout << input << " : Total " << Count << " Units Converted (" << iname <<"���� �����)";
            }
        }
    }
    else if (Mode == 3)
    {
        while (true)
        {
            char input[512];
            char Shape[512];
            int Shapelen;
            cout << "\n������ �̹��� �����̸��� �Է��ϼ��� (0 �Է½� ����)\n";
            cin >> input;
            if (!strcmp(input, "0"))
                break;
            else
            {
                char iname[512];
                strcpy_s(iname, 512, input);
                int ilength = strlen(iname);
                strcpy_s(iname + ilength-4, 512 - ilength, ".mask");

                char iname2[512];
                strcpy_s(iname2, 512, input);
                int ilength2 = strlen(iname2);
                iname2[ilength2 - 4] = '_';
                iname2[ilength2 - 3] = 'p';
                iname2[ilength2 - 2] = 'r';
                iname2[ilength2 - 1] = 'e';
                iname2[ilength2 - 0] = 'v';
                iname2[ilength2 + 1] = 'i';
                iname2[ilength2 + 2] = 'e';
                iname2[ilength2 + 3] = 'w';
                iname2[ilength2 + 4] = '.';
                iname2[ilength2 + 5] = 'b';
                iname2[ilength2 + 6] = 'm';
                iname2[ilength2 + 7] = 'p';
                iname2[ilength2 + 8] = 0;

                int Del = 0;
                FILE* fRet;
                fopen_s(&fRet, iname, "w+b");
                strcpy_s(Shape, 512, input);
                Shapelen = strlen(input);
                Shape[Shapelen - 4] = 0;

                // BMP Size : XxY �ܻ� �ȷ�Ʈ 
                // BMP Filter Slider 0 ~ 255, BMP Preview 
                BYTE btmp;
                BITMAP BMP;
                HBITMAP HBMP = (HBITMAP)LoadImageA(0, input, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
                GetObject(HBMP, sizeof(BMP), (LPSTR)&BMP);
                BYTE* IPtr = (BYTE*)BMP.bmBits;
                DWORD X = BMP.bmWidth, Y = BMP.bmHeight;
                DWORD tmp = BMP.bmWidth;
                fwrite(&tmp, 4, 1, fRet);
                tmp = BMP.bmHeight;
                fwrite(&tmp, 4, 1, fRet);
                int Count = 0;
                if (BMP.bmBitsPixel != 24)
                {
                    cout << "24��Ʈ BMP�� �ƴմϴ�. �ٸ� �̹����� �������ּ���" << endl;
                    Del = 1;
                }
                else
                {
                    cout << input << " �ε�Ϸ� (" << X << "x" << Y << ", True Color BMP)" << endl;

                    DWORD NX, X3 = (X * 3) % 4;
                    if (X3 == 0)
                        NX = X * 3;
                    else if (X3 == 1)
                        NX = X * 3 + 3;
                    else if (X3 == 2)
                        NX = X * 3 + 2;
                    else
                        NX = X * 3 + 1;

                RetryX2:
                    int Direction;
                    cout << "��� ���� ��밪 �Է� ������ �����ϼ���" << endl;
                    cout << "1. �Է°� �̻� ��⸸ �����" << endl;
                    cout << "2. �Է°� ���� ��⸸ �����" << endl;
                    cin >> Direction;
                    if (Direction != 1 && Direction != 2)
                        goto RetryX2;

                    int Filter;
                    if (Direction == 1)
                        cout << "��� ���� ��밪�� �Է��ϼ��� (0~255, �Է°� �̻� ��⸸ �����)" << endl;
                    else
                        cout << "��� ���� ��밪�� �Է��ϼ��� (0~255, �Է°� ���� ��⸸ �����)" << endl;
                    cin >> Filter;

                    BYTE* BPtr = (BYTE*)BMP.bmBits;
                    BYTE* NPtr = (BYTE*)malloc(NX * Y);

                    for (int j = Y - 1; j >= 0; j--)
                    {
                        for (int i = 0; i < X; i++)
                        {
                            int temp = j * NX + i * 3;
                            int Bright = (BPtr[temp] + BPtr[temp + 1] + BPtr[temp + 2]) / 3;
                            if (Direction == 1)
                            {
                                if (Bright >= Filter)
                                {
                                    Count++;
                                    // Write Shape
                                    btmp = 1;
                                    fwrite(&btmp, 1, 1, fRet);
                                    // Write Preview
                                    NPtr[temp] = 0xFF;
                                    NPtr[temp + 1] = 0xFF;
                                    NPtr[temp + 2] = 0xFF;
                                }
                                else
                                {
                                    btmp = 0;
                                    fwrite(&btmp, 1, 1, fRet);

                                    NPtr[temp] = 0;
                                    NPtr[temp + 1] = 0;
                                    NPtr[temp + 2] = 0;
                                }
                            }
                            else
                            {
                                if (Bright <= Filter)
                                {
                                    Count++;
                                    // Write Shape
                                    btmp = 1;
                                    fwrite(&btmp, 1, 1, fRet);
                                    // Write Preview
                                    NPtr[temp] = 0xFF;
                                    NPtr[temp + 1] = 0xFF;
                                    NPtr[temp + 2] = 0xFF;
                                }
                                else
                                {
                                    btmp = 0;
                                    fwrite(&btmp, 1, 1, fRet);

                                    NPtr[temp] = 0;
                                    NPtr[temp + 1] = 0;
                                    NPtr[temp + 2] = 0;
                                }
                            }
                        }
                    }
                    SaveHBITMAPToFile(NPtr, iname2, NX, X, Y);
                    free(NPtr);
                }
                DeleteObject(HBMP);
                fclose(fRet);
                if (Del == 1)
                    DeleteFileA(iname);
                else
                    cout << input << " : Total " << Count << " Units Converted (" << iname << "���� �����)";
            }
        }
    }
    else if (Mode == 2)
    {
        while (true)
        {
            char input[512];
            cout << "\n������ �̹��� �����̸��� �Է��ϼ��� (0 �Է½� ����)\n";
            cin >> input;
            if (!strcmp(input, "0"))
                break;
            else
            {
                char iname[512];
                strcpy_s(iname, 512, input);
                int ilength = strlen(iname);
                iname[ilength - 4] = '_';
                iname[ilength - 3] = 'o';
                iname[ilength - 2] = 'u';
                iname[ilength - 1] = 't';
                iname[ilength - 0] = '.';
                iname[ilength + 1] = 'c';
                iname[ilength + 2] = 'g';
                iname[ilength + 3] = 'r';
                iname[ilength + 4] = 'p';
                iname[ilength + 5] = 0;

                FILE* fRet;
                fopen_s(&fRet, iname, "w+b");

                // BMP Size : XxY �ܻ� �ȷ�Ʈ 

                BITMAP BMP;
                HBITMAP HBMP = (HBITMAP)LoadImageA(0, input, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
                GetObject(HBMP, sizeof(BMP), (LPSTR)&BMP);
                BYTE* IPtr = (BYTE*)BMP.bmBits;
                DWORD X = BMP.bmWidth, Y = BMP.bmHeight;
                int Count = 0;
                if (BMP.bmBitsPixel != 8)
                {
                    cout << "8��Ʈ BMP�� �ƴմϴ�. �ٸ� �̹����� �������ּ���" << endl;
                }
                else
                {
                    cout << input << " �ε�Ϸ� (" << X << "x" << Y << ", 8Bit Color BMP)" << endl;

                    DWORD NX, X3 = (X) % 4;
                    if (X3 == 0)
                        NX = X ;
                    else if (X3 == 1)
                        NX = X + 3;
                    else if (X3 == 2)
                        NX = X + 2;
                    else
                        NX = X + 1;

                    int Buffer[4] = { 0 }; // Num / XNum / YNum / YSizeXSize
                    fwrite(Buffer, 4, 4, fRet);
                    Buffer[1] = X;
                    Buffer[2] = Y;

                    BYTE* BPtr = (BYTE*)BMP.bmBits;

                    int XSize;
                Retry3:
                    cout << "�� ������ X�� ������ �Է��ϼ���. (�⺻�� : 3px)" << endl;
                    cin >> XSize;
                    if (XSize < 0 || XSize > 0xFFFF)
                        goto Retry3;
                    int YSize;
                Retry4:
                    cout << "�� ������ Y�� ������ �Է��ϼ���. (�⺻�� : 3px)" << endl;
                    cin >> YSize;
                    if (YSize < 0 || YSize > 0xFFFF)
                        goto Retry4;
                    Buffer[3] = YSize * 65536 + XSize;

                    int Mode2;
                Retry2:
                    cout << "�̹��� ���� ��带 �����ϼ���" << endl 
                        << "1. ���ϻ��� & ���ϳ��� (Mask01.ACT)" << endl 
                        << "2. ���ϻ��� & ������� (Mask02~08.ACT)" << endl
                        << "3. �ȷ�Ʈ��� & ���ϳ��� (Palette01~38.ACT)" << endl
                        << "4. �ȷ�Ʈ��� & ������� (Palette.ACT & ������ ���̷� ó��)" << endl;
                    cin >> Mode2;
                    if (Mode2 != 1 && Mode2 != 2 && Mode2 != 3 && Mode2 != 4)
                        goto Retry2;

                    if (Mode2 == 1)
                    {
                        cout << "�ε��� 2��° �̻��� ����� ����ó�� �˴ϴ�." << endl;

                        int Color, KLevel, TLevel, None = 0; // RGBW 8/12/15/X K[3] T[3]
                    Retry5:
                        cout << "�̹����� ���� �ڵ带 �Է��ϼ���. (��ũ���� �������� ��)" << endl
                            << "�Է¾��� : 1, ���� : 0, �ʷ� : 13, �Ķ� : 16, �Ͼ� : 17, ����(EMP) : 8, ����/���� : 12, ����(�׵θ�) : 15" << endl;
                        cin >> Color;
                       
                        if (Color == 1)
                            Color = 0;
                        else if (Color == 0)
                            Color = 1;
                        else if (Color == 13)
                            Color = 2;
                        else if (Color == 16)
                            Color = 4;
                        else if (Color == 17)
                            Color = 8;
                        else if (Color == 8)
                            Color = 16;
                        else if (Color == 12)
                            Color = 32;
                        else if (Color == 15)
                            Color = 64;
                        else
                            goto Retry5;
                        Color += 128;

                        Retry6:
                        cout << "�̹����� ��� ���� �ܰ踦 �Է��ϼ���. (0~7�� ����)" << endl;
                        cin >> KLevel;
                        if (KLevel >= 0 && KLevel <= 7)
                            Color += KLevel*256;
                        else
                            goto Retry6;

                        Retry7:
                        cout << "�̹����� ���� ���� �ܰ踦 �Է��ϼ���. (0~7�� ���� & ��ũ���� �������� ��)" << endl;
                        cin >> TLevel;
                        if (TLevel >= 0 && TLevel <= 7)
                            Color += TLevel*2048;
                        else
                            goto Retry7;

                        int Height;
                    Retry8:
                        cout << "��Ʈ�� ���̸� �Է��ϼ���. (0~255�� ����)" << endl;
                        cin >> Height;
                        if (Height >= 0 && Height <= 255)
                            Color += Height * 16777216;
                        else
                            goto Retry8;

                        int imageID;
                    Retry9:
                        cout << "��Ʈ�� ���� �̹���ID�� �Է��ϼ���. (�⺻�� 233)" << endl;
                        cin >> imageID;
                        if (imageID >= 0 && imageID <= 998)
                            Color += imageID * 16384;
                        else
                            goto Retry9;

                        for (int j = Y - 1; j >= 0; j--)
                        {
                            for (int i = 0; i < X; i++)
                            {
                                int temp = j * NX + i;
                                if (BPtr[temp] == 0)
                                {
                                    Buffer[0]++;
                                    fwrite(&Color, 4, 1, fRet);
                                }
                                else
                                    fwrite(&None, 4, 1, fRet);
                            }
                        }
                    }
                    else if (Mode2 == 2)
                    {
                        cout << "�ε��� 9��° �̻��� ����� ����ó�� �˴ϴ�." << endl;

                        FILE* IMAGE;
                        fopen_s(&IMAGE, input, "r+b");
                        int MNum, Size;
                        fseek(IMAGE, 0xA, 0);
                        fread(&Size, 4, 1, IMAGE);
                        MNum = ((Size - 54) / 4) - 1;
                        fclose(IMAGE);

                        int Color, KLevel, TLevel, None = 0; // RGBW 8/12/15/X K[3] T[3]
                        if (MNum <= 0 || MNum > 8)
                        {
                            cout << "�̹����� ���� �ȷ�Ʈ�� �߸��Ǿ����ϴ�. (Mask01~08.ACT�� ����ϼ���)" << endl << endl;
                            goto Quit2;
                        }
                    Retry15:
                        cout << "�̹����� ���� �ڵ带 �Է��ϼ���. (��ũ���� �������� ��)" << endl
                            << "�Է¾��� : 1, ���� : 0, �ʷ� : 13, �Ķ� : 16, �Ͼ� : 17, ����(EMP) : 8, ����/���� : 12, ����(�׵θ�) : 15" << endl;
                        cin >> Color;
                        if (Color == 1)
                            Color = 0;
                        else if (Color == 0)
                            Color = 1;
                        else if (Color == 13)
                            Color = 2;
                        else if (Color == 16)
                            Color = 4;
                        else if (Color == 17)
                            Color = 8;
                        else if (Color == 8)
                            Color = 16;
                        else if (Color == 12)
                            Color = 32;
                        else if (Color == 15)
                            Color = 64;
                        else
                            goto Retry15;

                        Color += 128;

                    Retry17:
                        cout << "�̹����� ���� ���� �ܰ踦 �Է��ϼ���. (0~7�� ���� & ��ũ���� �������� ��)" << endl;
                        cin >> TLevel;
                        if (TLevel >= 0 && TLevel <= 7)
                            Color += TLevel * 2048;
                        else
                            goto Retry17;

                        int Height[8] = { 0 };
                    Retry18:
                        cout << "��Ʈ�� ���̸� �Է��ϼ���. (0~255�� ���� & " << MNum << "�� �Է�)" << endl;
                        for (int q = 0; q < MNum; q++)
                        {
                            cin >> Height[q];
                            if (Height[q] >= 0 && Height[q] <= 255)
                                Height[q] = Height[q] * 16777216;
                            else
                                goto Retry18;
                        }

                        int imageID;
                    Retry19:
                        cout << "��Ʈ�� ���� �̹���ID�� �Է��ϼ���. (�⺻�� 233)" << endl;
                        cin >> imageID;
                        if (imageID >= 0 && imageID <= 998)
                            Color += imageID * 16384;
                        else
                            goto Retry19;

                        for (int j = Y - 1; j >= 0; j--)
                        {
                            for (int i = 0; i < X; i++)
                            {
                                int temp = j * NX + i;
                                if (BPtr[temp] < 8)
                                {
                                    Buffer[0]++;
                                    int CTemp = Color + BPtr[temp] * 256 + Height[BPtr[temp]];
                                    fwrite(&CTemp, 4, 1, fRet);
                                }
                                else
                                    fwrite(&None, 4, 1, fRet);
                            }
                        }
                    }
                    else if (Mode2 == 3)
                    {
                        cout << "�ε��� 113��° �̻��� ����� ����ó�� �˴ϴ�." << endl;

                        int Color=0, None = 0; // RGBW 8/12/15/X K[3] T[3]

                        int Height;
                    Retry28:
                        cout << "��Ʈ�� ���̸� �Է��ϼ���. (0~255�� ����)" << endl;
                        cin >> Height;
                        if (Height >= 0 && Height <= 255)
                            Color += Height * 16777216;
                        else
                            goto Retry28;

                        int imageID;
                    Retry29:
                        cout << "��Ʈ�� ���� �̹���ID�� �Է��ϼ���. (�⺻�� 233)" << endl;
                        cin >> imageID;
                        if (imageID >= 0 && imageID <= 998)
                            Color += imageID * 16384;
                        else
                            goto Retry29;

                        for (int j = Y - 1; j >= 0; j--)
                        {
                            for (int i = 0; i < X; i++)
                            {
                                int temp = j * NX + i;
                                if (BPtr[temp] < 112)
                                {
                                    Buffer[0]++;
                                    int CTemp = Color;
                                    CTemp += (BPtr[temp] % 8) * 256 + 128;
                                    int C = BPtr[temp] / 8;

                                    if (C == 0) // Red
                                        CTemp += 1;
                                    else if (C == 1) // T1
                                        CTemp += 2048 * 1;
                                    else if (C == 2) // Green
                                        CTemp += 2;
                                    else if (C == 3) // G+T1
                                        CTemp += 2 + 2048 * 1;
                                    else if (C == 4) // G+T2
                                        CTemp += 2 + 2048 * 2;
                                    else if (C == 5) // G+T3
                                        CTemp += 2 + 2048 * 3;
                                    else if (C == 6) // Blue
                                        CTemp += 4;
                                    else if (C == 7) // B+T1
                                        CTemp += 4 + 2048 * 1;
                                    else if (C == 8) // B+T2
                                        CTemp += 4 + 2048 * 2;
                                    else if (C == 9) // B+T3
                                        CTemp += 4 + 2048 * 3;
                                    else if (C == 10) // White
                                        CTemp += 8;
                                    else if (C == 11) // W+T1
                                        CTemp += 8 + 2048 * 1;
                                    else if (C == 12) // W+T2
                                        CTemp += 8 + 2048 * 2;
                                    else if (C == 13) // W+T3
                                        CTemp += 8 + 2048 * 3;

                                    fwrite(&CTemp, 4, 1, fRet);
                                }
                                else
                                    fwrite(&None, 4, 1, fRet);
                            }
                        }
                    }
                    else if (Mode2 == 4)
                    {
                        cout << "�ε��� 113��° �̻��� ����� ����ó�� �˴ϴ�." << endl;

                        FILE* IMAGE;
                        fopen_s(&IMAGE, input, "r+b");
                        int MNum, Size[8];
                        fseek(IMAGE, 0x36, 0);
                        fread(Size, 4, 8, IMAGE);
                        if (Size[1] == 0xFFFF00)
                            MNum = 1;
                        else if (Size[2] == 0xFFFF00)
                            MNum = 2;
                        else if (Size[3] == 0xFFFF00)
                            MNum = 3;
                        else if (Size[4] == 0xFFFF00)
                            MNum = 4;
                        else if (Size[5] == 0xFFFF00)
                            MNum = 5;
                        else if (Size[6] == 0xFFFF00)
                            MNum = 6;
                        else if (Size[7] == 0xFFFF00)
                            MNum = 7;
                        else
                            MNum = 8;
                        fclose(IMAGE);

                        int Color = 0, None = 0; // RGBW 8/12/15/X K[3] T[3]

                        int Height[8] = { 0 };
                    Retry38:
                        cout << "��Ʈ�� ���̸� �Է��ϼ���. (0~255�� ���� & " << MNum << "�� �Է�)" << endl;
                        for (int q = 0; q < MNum; q++)
                        {
                            cin >> Height[q];
                            if (Height[q] >= 0 && Height[q] <= 255)
                                Height[q] = Height[q] * 16777216;
                            else
                                goto Retry38;
                        }

                        int imageID;
                    Retry39:
                        cout << "��Ʈ�� ���� �̹���ID�� �Է��ϼ���. (�⺻�� 233)" << endl;
                        cin >> imageID;
                        if (imageID >= 0 && imageID <= 998)
                            Color += imageID * 16384;
                        else
                            goto Retry39;

                        for (int j = Y - 1; j >= 0; j--)
                        {
                            for (int i = 0; i < X; i++)
                            {
                                int temp = j * NX + i;
                                if (BPtr[temp] < 112)
                                {
                                    Buffer[0]++;
                                    int CTemp = Color;
                                    CTemp += (BPtr[temp] % 8) * 256 + Height[BPtr[temp] % 8] + 128;
                                    int C = BPtr[temp] / 8;

                                    if (C == 0) // Red
                                        CTemp += 1;
                                    else if (C == 1) // T1
                                        CTemp += 2048 * 1;
                                    else if (C == 2) // Green
                                        CTemp += 2;
                                    else if (C == 3) // G+T1
                                        CTemp += 2 + 2048 * 1;
                                    else if (C == 4) // G+T2
                                        CTemp += 2 + 2048 * 2;
                                    else if (C == 5) // G+T3
                                        CTemp += 2 + 2048 * 3;
                                    else if (C == 6) // Blue
                                        CTemp += 4;
                                    else if (C == 7) // B+T1
                                        CTemp += 4 + 2048 * 1;
                                    else if (C == 8) // B+T2
                                        CTemp += 4 + 2048 * 2;
                                    else if (C == 9) // B+T3
                                        CTemp += 4 + 2048 * 3;
                                    else if (C == 10) // White
                                        CTemp += 8;
                                    else if (C == 11) // W+T1
                                        CTemp += 8 + 2048 * 1;
                                    else if (C == 12) // W+T2
                                        CTemp += 8 + 2048 * 2;
                                    else if (C == 13) // W+T3
                                        CTemp += 8 + 2048 * 3;

                                    fwrite(&CTemp, 4, 1, fRet);
                                }
                                else
                                    fwrite(&None, 4, 1, fRet);
                            }
                        }
                    }
                Quit2:
                    fseek(fRet, 0, 0);
                    fwrite(Buffer, 4, 4, fRet);
                    Count = Buffer[0];
                }
                DeleteObject(HBMP);
                fclose(fRet);

                cout << input << " : Total " << Count << " Dots Converted (" << iname << "���� �����)";
            }
        }
    }

    system("pause");
    return 0;
}