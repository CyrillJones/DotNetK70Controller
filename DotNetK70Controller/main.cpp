#include "stdafx.h"

#include<ctime>
#include<list>
#include<conio.h>

#include<lua.hpp>

#include "Device.h"
#include "LuaSetup.h"

std::string GetTime()
{
	time_t CurrentTime;
	struct tm localTime;

	time(&CurrentTime);
	localtime_s(&localTime, &CurrentTime);

	std::ostringstream oss;
	oss << "[" << localTime.tm_hour << ":" << localTime.tm_min << ":" << localTime.tm_sec << "] ";

	return oss.str();
}

int _tmain(int argc, _TCHAR* argv[])
{
	//Setup lua state for loading scripts
	lua_State *L = luaL_newstate();;
	LuaSetup(L);

	Keyboard = new Device(); // convert to smart pointer?

	std::cout << GetTime() << "Looking for Corsair RGB K70/K95..." << std::endl;
	if (Keyboard->InitKeyboard()) // Check that it is found
	{
		std::cout << GetTime() << "Keyboard Initialised." << std::endl;

		bool Done = false;
		std::cout << "Type exit to exit." << std::endl << "Type the name of a valid script file excluding '.lua'." << std::endl;
		while (!Done) // main menu
		{
			std::cout << "> ";
			std::string In = "";
			std::cin >> In;

			if (In != "exit")
			{
				std::string filename = "lua/" + In + ".lua";
				luaL_dofile(L, filename.c_str());
			}
			else
			{
				Done = true;
			}

			if (!Done)
			{
				std::cout << GetTime() << "Hit any key to close script. " <<std::endl;
				bool ScriptDone = false;
				std::string In;
				while (!ScriptDone) //Script loop
				{
					RunMain(L); // runs the lua file main function;

					if (_kbhit())
					{
						ScriptDone = true;
					}
				}
			}
		}

		std::cout << GetTime() << "Closing Thread." << std::endl;

	}
	else // if its not found just end the program
	{
		std::cout << GetTime() << "Corsair K70 RGB keyboard not detected :(" << std::endl;
	}

	delete Keyboard;
	Keyboard = NULL;

	lua_close(L);

	std::cout << GetTime() << "Keyboard pointer deleted.";

	Sleep(1000);
	return 0;
}


