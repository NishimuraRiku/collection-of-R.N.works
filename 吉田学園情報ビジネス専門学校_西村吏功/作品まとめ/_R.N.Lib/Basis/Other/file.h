//========================================
// 
// ファイル関連の処理
// Author:RIKU NISHIMURA
// 
//========================================
//  *** file.h ***
//========================================
#ifndef _FILE_H_	// このマクロ定義がされていなかった時
#define _FILE_H_	// 二重インクルード防止のマクロを定義する

//****************************************
// プロトタイプ宣言
//****************************************
//========== *** 取得 ***
// 開くファイルを選択し取得
bool OpenFile(char *pFileName, char *pInitDir, char *pFileType);
// 保存するファイルを選択し取得
bool SaveFile(char *pFileName, char *pInitDir, char *pFileType);
// "data"を起点としたパス文字列を生成する
char *ConvertToDataPath(const char* path);
//========== *** テキスト読み込み ***
bool DataSearch(FILE *pFile, char *pDataSearch, char *pEndLabel);
void fscanf_int(FILE *pFile, int *pData);
void fscanf_float(FILE *pFile, float *pData);
void fscanf_string(FILE *pFile, char *pString);
void fscanf_bool(FILE *pFile, bool *pFlag);
void fscanf_D3DXVECTOR3(FILE *pFile, D3DXVECTOR3 *pVec);
void fscanf_Color(FILE *pFile, Color *pCol);

#endif