/******************************************************************************
File   :	SelfUpdateModule.cpp
Module :	SelfUpdateModule
Purpose:	Class functions
Author :	Jerry Luck (JerryTube)
Revison:	1.0.0
******************************************************************************/
#include "SelfUpdateModule.h"
using namespace std;
using namespace pugi;

		SelfUpdateModule::SelfUpdateModule(string EXE_NAME, string URL, string SPLASH_SCREEN)
		{
			this->EXE_NAME = EXE_NAME;
			this->URL = URL;
			this->SPLASH_SCREEN = SPLASH_SCREEN;
		}
		void SelfUpdateModule::Init_Update()
		{		
			
			// Splash Screen
			if(SPLASH_SCREEN != "")
			{
				if(!GUI_showSplash())
				{
					char msg[128];
					sprintf(msg, "Could not load splash screen, please check that %s exists.", SPLASH_SCREEN.c_str());
					GUI_error(msg, false);
				}
			}

			// Update popup
			pProgressDialog = (IProgressDialog*)GUI_createUpdatePopup();
				if(pProgressDialog == NULL)
					GUI_error("Could not create update popup.", true);
			// Popup Settings
			pProgressDialog->SetTitle(s2ws(EXE_NAME).c_str());
			pProgressDialog->SetLine(1, L"Fetching file list", NULL, NULL);
			pProgressDialog->SetLine(2, L"Please wait.", NULL, NULL);
			pProgressDialog->SetCancelMsg(L"Cancelling...",NULL);
			pProgressDialog->StartProgressDialog(NULL, NULL, PROGDLG_NOMINIMIZE, NULL);
			
			// Get XML file
			string XML_URL = URL+'/'+"update.xml";
			FILE* uXML = CURL_download("update.xml", XML_URL);
			
			// Parse XML file
			vector<string> uFiles = XML_listFiles("update.xml");

			// Loop over files and download
			for(int i=0; i<uFiles.size();i++)
			{
				filename = uFiles[i];
				if(filename == EXE_NAME){
					//Not tested yet..
					//XML_updateEXE();
				}
				else
				{
				sprintf(info,"%d of %d", i+1, uFiles.size());
				pProgressDialog->SetLine(1, s2ws(filename).c_str(), NULL, NULL);
				pProgressDialog->SetLine(3, s2ws(info).c_str(), NULL, NULL);
				string filename_fixed = replaceChar(filename, '\\', '/');
				file_url = URL+"/"+filename_fixed;
				CURL_download(filename_fixed, file_url);
				}
			}

		}

		#pragma region /* GUI */
		bool SelfUpdateModule::GUI_showSplash()
		{
			HWND hwnd;
			MSG messages;
			WNDCLASSEX wincl;
			char szClassName[] = "BmpSplash";

			// Load Bitmap
			HBITMAP hBitmap = (HBITMAP)LoadImage(NULL, (LPCSTR)SPLASH_SCREEN.c_str(), IMAGE_BITMAP, NULL, NULL, LR_LOADFROMFILE);
			HBRUSH hbr = CreatePatternBrush(hBitmap);

			// Check if image actually loaded
			if(hBitmap == NULL)
				return 0;

			// Create window class
			wincl.hInstance = GetModuleHandle(NULL);
			wincl.lpszClassName = szClassName;
			wincl.lpfnWndProc = GUI_WndProc;
			wincl.style = CS_DBLCLKS;
			wincl.cbSize = sizeof(WNDCLASSEX);
			wincl.hbrBackground = hbr;
			wincl.hIcon = NULL;
			wincl.hIconSm = NULL;
			wincl.hCursor = LoadCursor(NULL, IDC_ARROW);
			wincl.lpszMenuName = NULL;
			wincl.cbClsExtra = 0;
			wincl.cbWndExtra = 0;
			if(!RegisterClassEx(&wincl))
				return 0;

			DEVMODE display;
			EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &display);

			BITMAP bm = {0};
			GetObject( hBitmap, sizeof(bm), &bm );
			LONG cx = bm.bmWidth;
			LONG cy = bm.bmHeight;

			// Center screen
			int ScreenX = (GetSystemMetrics(SM_CXSCREEN) - cx) / 2;
			int ScreenY = (GetSystemMetrics(SM_CYSCREEN) - cy) / 2;

			// Create & Show Window
			hwnd = CreateWindowEx(WS_OVERLAPPED, szClassName, "Bitmap Display", WS_POPUPWINDOW, ScreenX, ScreenY, cx, cy, HWND_DESKTOP, NULL, GetModuleHandle(NULL), NULL);
			ShowWindow(hwnd, 5);
			return 1;

		}
		int SelfUpdateModule::GUI_createUpdatePopup()
		{
			IProgressDialog *pProgressDialog;
			CoInitialize(NULL);
			CoCreateInstance( CLSID_ProgressDialog,
							  NULL,
							  CLSCTX_ALL,
							  IID_IProgressDialog,
							  (LPVOID*)&pProgressDialog);

			if(!pProgressDialog)
				return NULL;

			return (int)pProgressDialog;
		}
		// Standard Window Procedure
		LRESULT CALLBACK SelfUpdateModule::GUI_WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
		{
			switch(msg)
			{
				case WM_CREATE:
    			break;

				default:
					return DefWindowProc(hwnd, msg, wParam, lParam);
			}
			return 0;
		}
		void SelfUpdateModule::GUI_error(char* msg, bool close)
		{
			MessageBoxA(NULL, (LPCSTR)msg, "Error", MB_OK | MB_ICONERROR | MB_TOPMOST);
			if(close)
				exit(0);
		}
		#pragma endregion

		#pragma region /* XML */
		vector<string> SelfUpdateModule::XML_listFiles(char* XML_FILE)
		{		
			string xml_filename, md5;
			vector<string> rFiles;
			xml_document doc;
			xml_parse_result xml_result = doc.load_file(XML_FILE);
			
			if(!xml_result)
				GUI_error("Could not load XML file.", true);
			
			for (xml_node node = doc.child("FCIV").first_child(); node; node = node.next_sibling()) 
			{
				xml_filename = node.first_child().child_value();
				md5 = node.first_child().next_sibling().child_value();
				if(XML_checkFile((char*)xml_filename.c_str(), md5))
					rFiles.push_back(xml_filename);
			}

			return rFiles;
		}
		bool SelfUpdateModule::XML_checkFile(char* filename, string remote_hash)
		{
			string local_hash = HASH_md5(filename);			
			string local_hash_64 = base64_encode((const unsigned char*)local_hash.c_str(),local_hash.size()); 	
			// check if update is needed
			if((!strcmp((const char*)local_hash_64.c_str(), remote_hash.c_str())) || local_hash == "")
				return true;	// Yes, we need to update
			else
				return false;	// No, we have the same file
		}
		void SelfUpdateModule::XML_updateEXE()
		{
			char* msg;
			sprintf(msg, "A newer version of %s is available. \n Do you want to update?", EXE_NAME);
			int choice = MessageBoxA(NULL, msg, "Update Available",  MB_YESNO | MB_ICONINFORMATION | MB_SYSTEMMODAL );
			if(choice == IDYES)
			{
				string old = EXE_NAME+"_old";
				rename(EXE_NAME.c_str(), old.c_str());
				pProgressDialog->SetLine(1, L"Patching Executable...", NULL, NULL);
				string new_url = URL+'/'+EXE_NAME;
				CURL_download(EXE_NAME, new_url);
				ShellExecute(0,                           
                            NULL,							// Operation to perform
							(LPCSTR)EXE_NAME.c_str(),	    // Application name
                            NULL,							// Additional parameters
                            0,								// Default directory
                            SW_SHOW);
				Module_CleanUp();
				exit(0);
			}
			else
				exit(0);
		}
		#pragma endregion

		#pragma region /* CURL */
		FILE* SelfUpdateModule::CURL_download(string file, string url)
		{
			CURL *curl;
			FILE *fp;
			CURLcode res;
			curl = curl_easy_init();
			if (curl) 
			{
				filename = file;
				int dirchk = CURL_chkdir(filename);
				fp = fopen(filename.c_str(), "wb");
				curl_easy_setopt(curl, CURLOPT_URL, url);
				curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);
				curl_easy_setopt(curl, CURLOPT_NOPROGRESS, FALSE);
				curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, CURL_report);
				curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, SelfUpdateModule::pProgressDialog); // Need to pass pProgressDialog pointer as we can't access it in the called function.
				curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
				res = curl_easy_perform(curl);
				curl_easy_cleanup(curl);
				fclose(fp);
				return fp;
			}
			else
			{
				return NULL;
			}
			
		}
		int SelfUpdateModule::CURL_report(void* ptr, double TotalToDownload, double NowDownloaded, double TotalToUpload, double NowUploaded)
		{
			IProgressDialog* pProgressDialog = (IProgressDialog*)ptr;
			char progress[128];
			if(!pProgressDialog->HasUserCancelled())
			{
				if(TotalToDownload > 1024){
					double now = (NowDownloaded/1024);
					double total = (TotalToDownload/1024);
					sprintf(progress, "%.0f/%.0f kB", now, total);
				}
				else
					sprintf(progress, "Just grabbing something, wait a sec!");
				
				pProgressDialog->SetLine(2, s2ws(progress).c_str(), NULL, NULL);
				pProgressDialog->SetProgress(NowDownloaded, TotalToDownload);
			}
			else
			{
				exit(0);
			}
			return 0;
		}
		size_t SelfUpdateModule::CURL_write(void *ptr, size_t size, size_t nmemb, FILE *stream) 
		{
			size_t written = fwrite(ptr, size, nmemb, stream);
			return written;
		}
		int SelfUpdateModule::CURL_chkdir(string filename)
		{
				//Check directory exists or curl gets cranky
				string dir;
				int found = filename.find_last_of("/");
				if(found > 0) // root dir
				dir = filename.substr(0,found);
				else
					return 0;

				if(GetFileAttributesA(dir.c_str()) == INVALID_FILE_ATTRIBUTES)
				{
					CreateDirectory( dir.c_str(), NULL);
					return 1; // yes we had to create it
				}
				else
					return 0; // no we didnt need to create it
		}
		#pragma endregion
		
		#pragma region /* HASH */
		string SelfUpdateModule::HASH_data(LPTSTR szOut, DWORD cchOut, LPCBYTE lpIn, DWORD cbIn, ALG_ID hash_algorithm)
		{
			std::string result;
			HCRYPTPROV hProv      = 0;
			HCRYPTHASH hHash      = 0;
			BYTE       raw[64];
			DWORD      raw_len    = sizeof(raw);
			BOOL       bResult    = FALSE;
			UINT       i;
 
			if(CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT | CRYPT_SILENT) &&
			   CryptCreateHash(hProv, hash_algorithm, 0, 0, &hHash) &&
			   CryptHashData(hHash, lpIn, cbIn, 0) &&
			   CryptGetHashParam(hHash, HP_HASHVAL, raw, &raw_len, 0) &&
			   (raw_len * 2) + 1 <= cchOut)
			{
				for (i = 0;i < raw_len;i++)
				{
					wsprintf(&szOut[i * 2], TEXT("%02.2x"), raw[i]);
				}
 
				result = base64_encode((unsigned char*)raw, raw_len);

				bResult = TRUE;
			}
 
			if (hHash) CryptDestroyHash(hHash);
			if (hProv) CryptReleaseContext(hProv, 0);
 
			return result;
		}
		string SelfUpdateModule::HASH_md5(char* filename)
		{
			ifstream is (filename, ifstream::binary);
			if (is) {
			// get length of file:
			is.seekg (0, is.end);
			int length = is.tellg();
			is.seekg (0, is.beg);
			char * buffer = new char [length];
			is.read (buffer,length);
			is.close();
			TCHAR szHash[64];
			// ...buffer contains the entire file...
			string HASH = HASH_data(szHash, 33, (LPCBYTE)buffer, length, CALG_MD5);
			delete[] buffer;
			return HASH;
		  }
		  else
			  return "";
		}
		#pragma endregion

		#pragma region /* UTILS */
		string SelfUpdateModule::replaceChar(string str, char ch1, char ch2) 
		{
		  for (int i = 0; i < str.length(); ++i) 
		  {
			if (str[i] == ch1)
				str[i] = ch2;
		  }
		  return str;
		}
		wstring SelfUpdateModule::s2ws(const std::string& s)
		{
		 int len;
		 int slength = (int)s.length() + 1;
		 len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0); 
		 wchar_t* buf = new wchar_t[len];
		 MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
		 std::wstring r(buf);
		 delete[] buf;
		 return r;
		}
		#pragma endregion

		#pragma region /* HOUSEKEEPING */
		bool SelfUpdateModule::Module_CleanUp()
		{
			try
			{
				pProgressDialog->StopProgressDialog();
				pProgressDialog->Release();
				CoUninitialize();
				return 1;
			}
			catch(...)
			{
				return 0;
			}
		}
		#pragma endregion
