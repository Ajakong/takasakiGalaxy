#include "DxLib.h"
#include "Application.h"



// プログラムは WinMain から始まります
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	Application& application = Application::GetInstance();

	if (!application.Init())
	{
		return -1;// このアプリは不正終了しました。
	}
	application.Run();


	return 0;
}