//========================================
// 
// ファイル関連の処理
// Author:RIKU NISHIMURA
// 
//========================================
// *** file.cpp ***
//========================================
// _R.N.Lib
#include "RNmain.h"

//================================================================================
//----------|---------------------------------------------------------------------
//==========| hファイルの共有関数
//----------|---------------------------------------------------------------------
//================================================================================

//============================================================
//--------------------| *** 取得 *** |------------------------
//============================================================
//========================================
// OpenFile関数 - 開くファイルを選択し取得 -
// (pFileName:読み込まれたファイル名を格納する pInitDir:初期表示するディレクトリ pFileType:選択できるファイルの種類)
// Author:RIKU NISHIMURA
//========================================
bool OpenFile(char *pFileName, char *pInitDir, char *pFileType)
{
	TCHAR fileName[MAX_PATH];	// ファイル名格納

	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(ofn));

	ofn.lStructSize = sizeof(ofn);		// 構造体のサイズ指定
	ofn.lpstrFile = fileName;			// 選択されたファイル名が格納される配列指定
	ofn.lpstrFile[0] = '\0';			// ファイル名初期化
	ofn.nMaxFile = sizeof(fileName);	// lpstrFile 配列の最大サイズをファイル名格納用文字列のサイズで指定
	ofn.lpstrFilter = pFileType;		// 選択できるファイルの種類を指定
	ofn.nFilterIndex = 1;				// 初期選択されているフィルタのインデックスを指定
	ofn.lpstrFileTitle = NULL;			// 選択されたファイルのタイトルを指定(使用されない場合はNULL)
	ofn.nMaxFileTitle = 0;				// lpstrFileTitle の最大サイズ(使用されない場合は0)
	ofn.lpstrInitialDir = pInitDir;		// 初期表示するディレクトリ(使用されない場合はNULL)
	// ダイアログのオプションを指定
	// OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST は、選択されたパスが存在することと、選択されたファイルが存在することを確認するもの
	// OFN_NOCHANGEDIR は、ディレクトリを変更しないようにするもの
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

	if (GetOpenFileName(&ofn))
	{// 開いたファイル名を取得出来た時、
		// ファイル名のポインタに読み込んだファイル名を代入
		sprintf(pFileName, fileName);
		// 入力情報のクリア処理
		ClearInputInfo();
		return true;
	}
	else {
		// 入力情報のクリア処理
		ClearInputInfo();
		return false;
	}
}

//========================================
// SaveFile関数 - 保存するファイルを選択し取得 -
// （pFileName:読み込まれたファイル名を格納する pInitDir:初期表示するディレクトリ pFileType:選択できるファイルの拡張子）
// Author:RIKU NISHIMURA
//========================================
bool SaveFile(char *pFileName, char *pInitDir, char *pExt)
{
	TCHAR fileName[MAX_PATH];	// ファイル名格納

	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(ofn));

	ofn.lStructSize = sizeof(ofn);		// 構造体のサイズ指定
	ofn.lpstrFile = fileName;			// 選択されたファイル名が格納される配列指定
	ofn.lpstrFile[0] = '\0';			// ファイル名初期化
	ofn.nMaxFile = sizeof(fileName);	// lpstrFile 配列の最大サイズをファイル名格納用文字列のサイズで指定
	ofn.lpstrFilter = pExt;				// 選択できるファイルの拡張子を指定
	ofn.nFilterIndex = 1;				// 初期選択されているフィルタのインデックスを指定
	ofn.lpstrFileTitle = NULL;			// 選択されたファイルのタイトルを指定(使用されない場合はNULL)
	ofn.nMaxFileTitle = 0;				// lpstrFileTitle の最大サイズ(使用されない場合は0)
	ofn.lpstrInitialDir = pInitDir;		// 初期表示するディレクトリ(使用されない場合はNULL)
	// ダイアログのオプションを指定
	// OFN_NOCHANGEDIR は、ディレクトリを変更しないようにするもの
	ofn.Flags = OFN_NOCHANGEDIR;

	if (GetSaveFileName(&ofn))
	{// 開いたファイル名を取得出来た時、
		// ファイル名のポインタに読み込んだファイル名を代入するが、
		// 文末が拡張子で終わっていない時、それに加えて拡張子を連結させる
		int len = strlen(fileName);
		int extlen = strlen(pExt);
		if (len < extlen || strcmp(fileName + len - extlen, pExt) != 0)
		{
			sprintf(pFileName, "%s%s", fileName, pExt);
		}
		else {
			strcpy(pFileName, fileName);
		}

		// 入力情報のクリア処理
		ClearInputInfo();
		return true;
	}
	else {
		// 入力情報のクリア処理
		ClearInputInfo();
		return false;
	}
}