#include "window/app_window.h"

int main()
{
	AppWindow app;
	app.Create("Render");
	app.Move(100, 100);
	app.Size(800, 800);
	app.Loop();
	return 0;
}