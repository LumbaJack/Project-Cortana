#include <iostream>
#include <windows.h>

#define GCAPI_DIRECT
#include "gcapi.h"
#include <string>

GCDAPI_Load gcdapi_Load = NULL;
GCDAPI_Unload gcdapi_Unload = NULL;
GCAPI_IsConnected gcapi_IsConnected = NULL;
GCAPI_GetFWVer gcapi_GetFWVer = NULL;
GCAPI_Read gcapi_Read = NULL;
GCAPI_Write gcapi_Write = NULL;
GCAPI_GetTimeVal gcapi_GetTimeVal = NULL;
GCAPI_CalcPressTime gcapi_CalcPressTime = NULL;

void press_button(int button, int duration, bool positive){
	int8_t output[GCAPI_OUTPUT_TOTAL] = { 0 };

	if (positive == TRUE){
		output[button] = 100;
	}
	else{
		output[button] = -100;
	}

	gcapi_Write(output);
	Sleep(duration);

	int8_t output2[GCAPI_OUTPUT_TOTAL] = { 0 };
	output2[button] = 0;
	gcapi_Write(output2);
}

void random_timeout(){
	int totaltime = rand() % (180 - 150 + 1) + 150;
	float interval = (float)(totaltime / 20 * 1000);

	std::cout << "Waiting...";
	for (int x = 0; x < 20; x++){
		std::cout << ".";
		Sleep((DWORD)interval);
	}
	std::cout << "Done" << std::endl;
}

void walk_around(){
	std::cout << "SENDING: Walking around." << std::endl;
	int choice = rand() % (3 - 0 + 1) + 0;

	if (choice == 0){
		press_button(XB1_LX, 500, FALSE);
		press_button(XB1_LX, 500, TRUE);
	}
	else if (choice == 1){
		press_button(XB1_LX, 500, TRUE);
		press_button(XB1_LX, 500, FALSE);
	}
	else if (choice == 2){
		press_button(XB1_LY, 500, FALSE);
		press_button(XB1_LY, 500, TRUE);
	}
	else{
		press_button(XB1_LY, 500, TRUE);
		press_button(XB1_LY, 500, FALSE);
	}
}

void look_around(){
	std::cout << "SENDING: Looking around." << std::endl;
	int choice = rand() % (1 - 0 + 1) + 0;

	if (choice == 0){
		press_button(XB1_RX, 300, FALSE);
		press_button(XB1_RX, 300, TRUE);
	}
	else if (choice == 1){
		press_button(XB1_RX, 300, TRUE);
		press_button(XB1_RX, 300, FALSE);
	}
}

void get_down(){
	std::cout << "SENDING: Getting down." << std::endl;
	press_button(XB1_RS, 100, TRUE);
}

void check_gameover(){
	std::cout << "SENDING: Checking if gameover." << std::endl;
	press_button(XB1_B, 100, TRUE);
}

void change_weapon(){
	std::cout << "SENDING: Changing gun." << std::endl;
	press_button(XB1_Y, 100, TRUE);
}

void get_in_the_game(){
	std::cout << "SENDING: Getting in the game." << std::endl;
	press_button(XB1_B, 100, TRUE);
	Sleep(500);
	press_button(XB1_B, 100, TRUE);
	Sleep(500);
	press_button(XB1_B, 100, TRUE);
	Sleep(500);
	press_button(XB1_LY, 50, TRUE);
	Sleep(500);
	press_button(XB1_LX, 50, TRUE);
	Sleep(500);
	press_button(XB1_A, 100, TRUE);
	Sleep(500);
	press_button(XB1_LY, 50, TRUE);
	Sleep(500);
	press_button(XB1_A, 100, TRUE);
	Sleep(500);
}

int main(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
	HINSTANCE hInsGPP = NULL;
	GCAPI_REPORT report = { 0 };
	int8_t output[GCAPI_OUTPUT_TOTAL] = { 0 };

	hInsGPP = LoadLibrary(TEXT("gcdapi.dll"));
	if (hInsGPP == NULL) {
		std::cout << "Error on loading gcdapi.dll" << std::endl;
		return(1);
	}

	gcdapi_Load = (GCDAPI_Load)GetProcAddress(hInsGPP, "gcdapi_Load");
	gcdapi_Unload = (GCDAPI_Unload)GetProcAddress(hInsGPP, "gcdapi_Unload");
	gcapi_IsConnected = (GCAPI_IsConnected)GetProcAddress(hInsGPP, "gcapi_IsConnected");
	gcapi_GetFWVer = (GCAPI_GetFWVer)GetProcAddress(hInsGPP, "gcapi_GetFWVer");
	gcapi_Read = (GCAPI_Read)GetProcAddress(hInsGPP, "gcapi_Read");
	gcapi_Write = (GCAPI_Write)GetProcAddress(hInsGPP, "gcapi_Write");
	gcapi_GetTimeVal = (GCAPI_GetTimeVal)GetProcAddress(hInsGPP, "gcapi_GetTimeVal");
	gcapi_CalcPressTime = (GCAPI_CalcPressTime)GetProcAddress(hInsGPP, "gcapi_CalcPressTime");

	if (gcdapi_Load == NULL || gcdapi_Unload == NULL || gcapi_IsConnected == NULL || gcapi_GetFWVer == NULL ||
		gcapi_Read == NULL || gcapi_Write == NULL || gcapi_GetTimeVal == NULL || gcapi_CalcPressTime == NULL) {
		FreeLibrary(hInsGPP);
		std::cout << "Error on gcdapi.dll" << std::endl;
		return(1);
	}

	if (!gcdapi_Load()) {
		FreeLibrary(hInsGPP);
		std::cout << "Unable to initiate the Direct API" << std::endl;
		return(1);
	}

	bool connected = TRUE;

	while (1){
		if (gcapi_IsConnected()) {
			if (!connected){
				connected = TRUE;
				std::cout << "Titan One is now connected." << std::endl;
			}

			/*int choice = rand() % (4 - 0 + 1) + 0;

			if (choice == 1){
			walk_around();
			}
			else if (choice == 2){
			look_around();
			}
			else if (choice == 3){
			get_down();
			}

			random_timeout();
			check_gameover();

			random_timeout();
			change_weapon();
			*/

			get_down();
			random_timeout();

			get_in_the_game();
			random_timeout();
		}
		else{
			if (connected){
				connected = FALSE;
				std::cout << "Titan One is not connected." << std::endl;
			}
			else{
				std::cout << "Waiting for connection..." << std::endl;
			}
		}
	}

	gcdapi_Unload();
	FreeLibrary(hInsGPP);
	return(0);
}
