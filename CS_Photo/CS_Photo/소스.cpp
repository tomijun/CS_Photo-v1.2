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

    printf("--------------------------------------\n     。`+˚CS_Photo v1.2 。+.˚\n--------------------------------------\n\t\t\tMade By Ninfia\n\n");

    int Mode;
    Retry:
    cout << "이미지 변환 모드를 선택하세요" << endl << "1. 이미지를 도형 데이터로 변환 (Shape)" << endl << "2. 이미지를 그래픽 데이터로 변환 (CGRP)" << endl;
    cout << "3. 이미지를 마스크 데이터로 변환 (Mask)" << endl;
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
            cout << "\n삽입할 이미지 파일이름을 입력하세요 (0 입력시 종료)\n";
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

                // BMP Size : XxY 단색 팔레트 
                // BMP Filter Slider 0 ~ 255, BMP Preview 

                BITMAP BMP;
                HBITMAP HBMP = (HBITMAP)LoadImageA(0, input, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
                GetObject(HBMP, sizeof(BMP), (LPSTR)&BMP);
                BYTE* IPtr = (BYTE*)BMP.bmBits;
                DWORD X = BMP.bmWidth, Y = BMP.bmHeight;
                int Count = 0;
                if (BMP.bmBitsPixel != 24)
                {
                    cout << "24비트 BMP가 아닙니다. 다른 이미지를 선택해주세요" << endl;
                    Del = 1;
                }
                else
                {
                    cout << input << " 로드완료 (" << X << "x" << Y << ", True Color BMP)" << endl;

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
                    cout << "밝기 필터 허용값 입력 방향을 선택하세요" << endl;
                    cout << "1. 입력값 이상 밝기만 허용함" << endl;
                    cout << "2. 입력값 이하 밝기만 허용함" << endl;
                    cin >> Direction;
                    if (Direction != 1 && Direction != 2)
                        goto RetryX;

                    int Filter;
                    if (Direction == 1)
                        cout << "밝기 필터 허용값을 입력하세요 (0~255, 입력값 이상 밝기만 허용함)" << endl;
                    else
                        cout << "밝기 필터 허용값을 입력하세요 (0~255, 입력값 이하 밝기만 허용함)" << endl;
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
                    cout << input << " : Total " << Count << " Units Converted (" << iname <<"으로 저장됨)";
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
            cout << "\n삽입할 이미지 파일이름을 입력하세요 (0 입력시 종료)\n";
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

                // BMP Size : XxY 단색 팔레트 
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
                    cout << "24비트 BMP가 아닙니다. 다른 이미지를 선택해주세요" << endl;
                    Del = 1;
                }
                else
                {
                    cout << input << " 로드완료 (" << X << "x" << Y << ", True Color BMP)" << endl;

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
                    cout << "밝기 필터 허용값 입력 방향을 선택하세요" << endl;
                    cout << "1. 입력값 이상 밝기만 허용함" << endl;
                    cout << "2. 입력값 이하 밝기만 허용함" << endl;
                    cin >> Direction;
                    if (Direction != 1 && Direction != 2)
                        goto RetryX2;

                    int Filter;
                    if (Direction == 1)
                        cout << "밝기 필터 허용값을 입력하세요 (0~255, 입력값 이상 밝기만 허용함)" << endl;
                    else
                        cout << "밝기 필터 허용값을 입력하세요 (0~255, 입력값 이하 밝기만 허용함)" << endl;
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
                    cout << input << " : Total " << Count << " Units Converted (" << iname << "으로 저장됨)";
            }
        }
    }
    else if (Mode == 2)
    {
        while (true)
        {
            char input[512];
            cout << "\n삽입할 이미지 파일이름을 입력하세요 (0 입력시 종료)\n";
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

                // BMP Size : XxY 단색 팔레트 

                BITMAP BMP;
                HBITMAP HBMP = (HBITMAP)LoadImageA(0, input, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
                GetObject(HBMP, sizeof(BMP), (LPSTR)&BMP);
                BYTE* IPtr = (BYTE*)BMP.bmBits;
                DWORD X = BMP.bmWidth, Y = BMP.bmHeight;
                int Count = 0;
                if (BMP.bmBitsPixel != 8)
                {
                    cout << "8비트 BMP가 아닙니다. 다른 이미지를 선택해주세요" << endl;
                }
                else
                {
                    cout << input << " 로드완료 (" << X << "x" << Y << ", 8Bit Color BMP)" << endl;

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
                    cout << "점 사이의 X축 간격을 입력하세요. (기본값 : 3px)" << endl;
                    cin >> XSize;
                    if (XSize < 0 || XSize > 0xFFFF)
                        goto Retry3;
                    int YSize;
                Retry4:
                    cout << "점 사이의 Y축 간격을 입력하세요. (기본값 : 3px)" << endl;
                    cin >> YSize;
                    if (YSize < 0 || YSize > 0xFFFF)
                        goto Retry4;
                    Buffer[3] = YSize * 65536 + XSize;

                    int Mode2;
                Retry2:
                    cout << "이미지 색상 모드를 선택하세요" << endl 
                        << "1. 단일색상 & 단일높이 (Mask01.ACT)" << endl 
                        << "2. 단일색상 & 음영사용 (Mask02~08.ACT)" << endl
                        << "3. 팔레트사용 & 단일높이 (Palette01~38.ACT)" << endl
                        << "4. 팔레트사용 & 음영사용 (Palette.ACT & 음영을 높이로 처리)" << endl;
                    cin >> Mode2;
                    if (Mode2 != 1 && Mode2 != 2 && Mode2 != 3 && Mode2 != 4)
                        goto Retry2;

                    if (Mode2 == 1)
                    {
                        cout << "인덱스 2번째 이상의 색상는 투명처리 됩니다." << endl;

                        int Color, KLevel, TLevel, None = 0; // RGBW 8/12/15/X K[3] T[3]
                    Retry5:
                        cout << "이미지의 색상 코드를 입력하세요. (뉴크닷을 기준으로 함)" << endl
                            << "입력안함 : 1, 빨강 : 0, 초록 : 13, 파랑 : 16, 하양 : 17, 투명(EMP) : 8, 남색/투명 : 12, 연두(테두리) : 15" << endl;
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
                        cout << "이미지의 흑색 음영 단계를 입력하세요. (0~7중 선택)" << endl;
                        cin >> KLevel;
                        if (KLevel >= 0 && KLevel <= 7)
                            Color += KLevel*256;
                        else
                            goto Retry6;

                        Retry7:
                        cout << "이미지의 적색 음영 단계를 입력하세요. (0~7중 선택 & 뉴크닷을 기준으로 함)" << endl;
                        cin >> TLevel;
                        if (TLevel >= 0 && TLevel <= 7)
                            Color += TLevel*2048;
                        else
                            goto Retry7;

                        int Height;
                    Retry8:
                        cout << "도트의 높이를 입력하세요. (0~255중 선택)" << endl;
                        cin >> Height;
                        if (Height >= 0 && Height <= 255)
                            Color += Height * 16777216;
                        else
                            goto Retry8;

                        int imageID;
                    Retry9:
                        cout << "도트로 찍을 이미지ID를 입력하세요. (기본값 233)" << endl;
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
                        cout << "인덱스 9번째 이상의 색상는 투명처리 됩니다." << endl;

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
                            cout << "이미지의 색상 팔레트가 잘못되었습니다. (Mask01~08.ACT를 사용하세요)" << endl << endl;
                            goto Quit2;
                        }
                    Retry15:
                        cout << "이미지의 색상 코드를 입력하세요. (뉴크닷을 기준으로 함)" << endl
                            << "입력안함 : 1, 빨강 : 0, 초록 : 13, 파랑 : 16, 하양 : 17, 투명(EMP) : 8, 남색/투명 : 12, 연두(테두리) : 15" << endl;
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
                        cout << "이미지의 적색 음영 단계를 입력하세요. (0~7중 선택 & 뉴크닷을 기준으로 함)" << endl;
                        cin >> TLevel;
                        if (TLevel >= 0 && TLevel <= 7)
                            Color += TLevel * 2048;
                        else
                            goto Retry17;

                        int Height[8] = { 0 };
                    Retry18:
                        cout << "도트의 높이를 입력하세요. (0~255중 선택 & " << MNum << "개 입력)" << endl;
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
                        cout << "도트로 찍을 이미지ID를 입력하세요. (기본값 233)" << endl;
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
                        cout << "인덱스 113번째 이상의 색상는 투명처리 됩니다." << endl;

                        int Color=0, None = 0; // RGBW 8/12/15/X K[3] T[3]

                        int Height;
                    Retry28:
                        cout << "도트의 높이를 입력하세요. (0~255중 선택)" << endl;
                        cin >> Height;
                        if (Height >= 0 && Height <= 255)
                            Color += Height * 16777216;
                        else
                            goto Retry28;

                        int imageID;
                    Retry29:
                        cout << "도트로 찍을 이미지ID를 입력하세요. (기본값 233)" << endl;
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
                        cout << "인덱스 113번째 이상의 색상는 투명처리 됩니다." << endl;

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
                        cout << "도트의 높이를 입력하세요. (0~255중 선택 & " << MNum << "개 입력)" << endl;
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
                        cout << "도트로 찍을 이미지ID를 입력하세요. (기본값 233)" << endl;
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

                cout << input << " : Total " << Count << " Dots Converted (" << iname << "으로 저장됨)";
            }
        }
    }

    system("pause");
    return 0;
}