/******************************************************************************
File   :	SelfUpdateModule.h
Module :	SelfUpdateModule
Purpose:	Includes and class skeleton
Author :	Jerry Luck (JerryTube)
Revison:	1.0.0

 A simple and effective, modular, self updating class.
    Copyright (C) 2013  Jerry Luck

 Permission is hereby granted, free of charge, to any person
 obtaining a copy of this software and associated documentation
 files (the "Software"), to deal in the Software without
 restriction, including without limitation the rights to use,
 copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the
 Software is furnished to do so, subject to the following
 conditions:
 
 The above copyright notice and this permission notice shall be
 included in all copies or substantial portions of the Software.
  
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 OTHER DEALINGS IN THE SOFTWARE.

******************************************************************************/
using namespace std;

#pragma region /* Windows Header Files */
#include <windows.h>
#include <Shellapi.h>
#include <WinCrypt.h>
#include "Shlwapi.h"
#pragma endregion

#pragma region /* Other Header Files */
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <ShlObj.h>
#include <math.h>
#pragma endregion

#pragma region /* CURL */
#include <curl/curl.h>
#include <curl/easy.h>
#pragma endregion

#pragma region /* PUGIXML */
/**
 * Copyright (c) 2006-2012 Arseny Kapoulkine
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */
#include <pugixml/pugixml.hpp>
#pragma endregion

#pragma region /* Class Skeleton */
class SelfUpdateModule
{
public:
		SelfUpdateModule(string EXE_NAME, string URL, string SPLASH_SCREEN);
		void Init_Update();
private:
		#pragma region /* VARIABLES */
		string EXE_NAME;
		string URL;
		string SPLASH_SCREEN;
		string file_url;
		char info[128]; // Not very clean but its a buffer none the less.
		char progress[128];
		string filename;
		IProgressDialog* pProgressDialog;
		#pragma endregion

		#pragma region /* GUI */
		bool GUI_showSplash();
		int GUI_createUpdatePopup();
		static LRESULT CALLBACK GUI_WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam); // Must be static, Class member function expects to receive an additional hidden parameter called 'this'
		void GUI_error(char* msg, bool close);
		#pragma endregion

		#pragma region /* XML */
		vector<string> XML_listFiles(char* XML_FILE);
		bool XML_checkFile(char* filename, string remote_hash);
		void XML_updateEXE();
		#pragma endregion

		#pragma region /* CURL */
		FILE* CURL_download(string filename, string url);
		static int CURL_report(void* ptr, double TotalToDownload, double NowDownloaded, double TotalToUpload, double NowUploaded); //Must be static or we get random crashes..
		size_t CURL_write(void *ptr, size_t size, size_t nmemb, FILE *stream);
		int CURL_chkdir(string filename);
		#pragma endregion

		#pragma region /* HASH */
		string HASH_data(LPTSTR szOut, DWORD cchOut, LPCBYTE lpIn, DWORD cbIn, ALG_ID hash_algorithm);
		string HASH_md5(char* filename);
		#pragma endregion

		#pragma region /* UTILS */
		string base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len); //Thanks to René Nyffenegger
		string replaceChar(string str, char ch1, char ch2) ;
		static wstring s2ws(const std::string& s);
		#pragma endregion

		#pragma region /* HOUSEKEEPING */
		bool Module_CleanUp();
		#pragma endregion
};
#pragma endregion