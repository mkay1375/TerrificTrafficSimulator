#include "globalization.h"
#include "TTS.h"

int main(int argc, char* args[])
{
	srand(time(0));
	InitializeProgram();
	TTS tts;
	return tts.Run();
}