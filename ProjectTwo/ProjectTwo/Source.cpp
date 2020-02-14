#pragma once



#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Audio.hpp>

#include <Box2D/Box2D.h>

#include <iostream>
#include <String>
#include <vector>
#include <Windows.h>
#include <utility>

#include "imgui.h"
#include "imgui-sfml.h"


#include "src/Anim.h"
#include "src/Explover.h"



namespace ImGui // Поддержка строк для GUI
{
	static auto vector_getter = [](void* vec, int idx, const char** out_text)
	{
		auto& vector = *static_cast<std::vector<std::string>*>(vec);
		if (idx < 0 || idx >= static_cast<int>(vector.size())) { return false; }
		*out_text = vector.at(idx).c_str();
		return true;
	};

	bool Combo(const char* label, int* currIndex, std::vector<std::string>& values)
	{
		if (values.empty()) { return false; }
		return Combo(label, currIndex, vector_getter,
			static_cast<void*>(&values), values.size());
	}

	bool ListBox(const char* label, int* currIndex, std::vector<std::string>& values)
	{
		if (values.empty()) { return false; }
		return ListBox(label, currIndex, vector_getter,
			static_cast<void*>(&values), values.size());
	}

}

void Hide()
{
	HWND Hide;
	AllocConsole();
	Hide = FindWindowA("ConsoleWindowClass", NULL);
	ShowWindow(Hide, 0);
}

template <typename T>
std::vector <std::string> MapToString(std::map <std::string, T> a) 
{
	std::vector <std::string> swag;
	for (auto it = a.begin(); it != a.end(); ++it)
	{
		swag.push_back(it->first);
	}
	return swag;
}

bool checNameAnim(std::vector <AnimManager> a, std::string name)
{
	for (int i = 0; i < a.size(); i++)
	{
		if (a[i].Name == name)return true;
	}
	return false;
}

std::vector <std::string> AnimMToString(std::vector <AnimManager> a) {

	std::vector <std::string> V;
	for (int i = 0; i < a.size(); i++) 
	{
		V.push_back(a[i].Name);
	}
	return V;
}


int main()
{

	const int Wy = 900;
	const int Wx = 1440;

	const ImVec2 ConsMenuSize(300, Wy);
	const ImVec2 ConsMenuPos(Wx - 300, 0);
	
	const ImVec2 AnothConsMenuSize(600, 400);

	std::vector<File> FileVec;
	std::vector<std::string> FileNamesVec;
	Ex(FileVec, FileNamesVec);



	sf::RenderWindow window(sf::VideoMode(Wx, Wy), "ProjectTwo");
	window.setVerticalSyncEnabled(true);

	ImGui::SFML::Init(window);

	//ImGui:
	bool Main;
	bool ImgConsole = true;
	bool show_S_Video_window = false;
	bool show_A_Create_window = false;
	
	int ChozeOne = -1;
	int ChozeTwo = -1;
	int Frams = 0;
	float Speed = 0;
	char nam[20] = { };


	View view;
	view.setCenter(0, 0);
	//--------------------------------
	
	std::vector <AnimManager> ALL_Anim_Manager;

	/*AnimManager Hero;
	Hero.create("Stay", "1.Data/Animation/HS", 0.003, 1);*/
	
	/*Что то со шрифтом и поддержкой русиша
	//io.Fonts->AddFontFromFileTTF("1.Data/Front/Roboto-MediumItalic.ttf", 30);
	//io.Fonts->AddFontFromFileTTF("1.Data/Front/Roboto-MediumItalic.ttf", 20.0f, NULL);
	/*ImGuiIO & io = ImGui::GetIO();
	ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\Arial.ttf", 18.0f);
	ImGui::PushFont(font);*/


	sf::Clock deltaClock;
	sf::Clock clock;
	while (window.isOpen()) {

		float time = clock.getElapsedTime().asMicroseconds();
		clock.restart();
		time = time / 300;

		sf::Event event;
		while (window.pollEvent(event)) {
			ImGui::SFML::ProcessEvent(event);

			if (event.type == sf::Event::Closed) {
				window.close();
			}
		}

//ImGui:
		if (ImgConsole) 
		{
			ImGui::SFML::Update(window, deltaClock.restart());
			ImGui::Begin("Main Window", &Main, ImGuiWindowFlags_MenuBar);

			ImGui::SetWindowSize(ConsMenuSize);
			ImGui::SetWindowPos(ConsMenuPos);
			ImGui::StyleColorsLight();

			if (ImGui::BeginMenuBar())
			{
				if (ImGui::BeginMenu("File"))
				{
					if (ImGui::MenuItem("Load")) { /* Do stuff */ }
					ImGui::EndMenu();
				}
				if (ImGui::BeginMenu("Settings"))
				{
					if (ImGui::MenuItem("Video")) { show_S_Video_window   = true; }
					ImGui::EndMenu();
				}
				if (ImGui::BeginMenu("Animation"))
				{
					if (ImGui::MenuItem("Create")) { show_A_Create_window = true; }
					ImGui::EndMenu();
				}
				ImGui::EndMenuBar();
			}
			if (show_S_Video_window)
			{
				ImGui::Begin("Video Settings", &show_S_Video_window);
				ImGui::SetWindowSize(AnothConsMenuSize);
				ImGui::Text("Hello from another window!");
				ImGui::End();
			}
			if (show_A_Create_window)
			{
				ImGui::Begin("Create Animation", &show_A_Create_window);
				ImGui::SetWindowSize(AnothConsMenuSize);

				if (ImGui::CollapsingHeader("Create Anim Manager")) {

					ImGui::InputText("-New Anim Manager", nam, 20);
					if (ImGui::Button("Create")) {  
					    
						if (checNameAnim(ALL_Anim_Manager, nam)) nam = {};
						else {
							AnimManager A(nam);
							ALL_Anim_Manager.push_back(A);
						}
					}
				}
				if (ALL_Anim_Manager.size()!=0)
				{
					std::vector <std::string> a = AnimMToString(ALL_Anim_Manager);
					if (ImGui::ListBox("Chose Anim Manager", &ChozeOne, a)) {}
					if (ChozeOne != -1)
					{
						if (ImGui::ListBox("Chose 0 Frame o Anim", &ChozeTwo, FileNamesVec)) {}
						if (ChozeTwo != -1)
						{
							ImGui::Text("File:");

							ImGui::Text(FileVec[ChozeTwo].name.c_str());
							ImGui::InputText("Name", nam, 20);
							ImGui::SliderInt("Frames", &Frams, 0, 20);
							ImGui::SliderFloat("Speed", &Speed, 0, 1);
							if (ImGui::Button("Create!")) { ALL_Anim_Manager[ChozeOne].create(nam, FileVec[ChozeTwo].GetWayToPic(5), Speed, Frams); }
						}
					}
				}


				ImGui::End();
			}
			ImGui::End();
		}
//Update:

			for (int i = 0; i< ALL_Anim_Manager.size();i++)
			{
				ALL_Anim_Manager[i].tick(time);
			}
	
	
//Start Draw:
			window.clear();

			for (int i = 0; i < ALL_Anim_Manager.size(); i++)
			{
				ALL_Anim_Manager[i].draw(window);
			}
			window.setView(view);
		ImGui::SFML::Render(window);
		window.display();
	}

	ImGui::SFML::Shutdown();
}