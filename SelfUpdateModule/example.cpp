#include "SelfUpdateModule.h"

int main(int argc, char* argv[])
{
	// This is a simple example of how you would implement the update module;
	// Please note that test.bmp is CURRENTLY a local file only, I will get around to making it remote.
	// Also argv[0] is the current exe name if you're not sure.
	SelfUpdateModule update = SelfUpdateModule(argv[0], "http://codf2p.net/files/test", "test.bmp");
	update.Init_Update();
	return 0;
}