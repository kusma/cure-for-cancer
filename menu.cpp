#include <windows.h>
#include <stdio.h>
#include "resource.h"
#include "menu.h"
#include "tool/misc.h"

LRESULT CALLBACK ConfigCallback(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

Menu::Menu(){
	HRESULT result = DialogBox( GetModuleHandle(NULL), (LPCTSTR)IDD_CONFIG, NULL, (DLGPROC)ConfigCallback);
	if(FAILED(result)) PostQuitMessage(1);
}

LRESULT CALLBACK ConfigCallback(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam){
	switch (message){
		case WM_COMMAND:
			if(LOWORD(wParam) == IDC_OK) {
				Menu::instance()->SetFullscreen( !IsDlgButtonChecked(hDlg, IDC_WINDOWED) );
				Menu::instance()->SetSound( !IsDlgButtonChecked(hDlg, IDC_NOSOUND) );
				if(IsDlgButtonChecked(hDlg, IDC_BOYSEX)) SetWindowsBackground();
				EndDialog(hDlg, LOWORD(wParam));
				return TRUE;
			}
			if(LOWORD(wParam) == IDC_CANCEL) {
				EndDialog(hDlg, LOWORD(wParam));
				exit(0);
				return TRUE;
			}
		break;
	}
	return FALSE;
}
