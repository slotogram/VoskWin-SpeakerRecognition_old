// VoskTest.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#include "vosk_api.h"
//#include "kaldi-types.h"
//#include "model.h"

#include <stdio.h>
#include <locale.h>
#include <iostream>
#include <windows.h>
#include <vector>
#include <string>


//using namespace kaldi;

	std::wstring string_to_wstring(std::string str) { //TODO: check if buf and sss not needed
		std::wstring convertedString;
		int requiredSize = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, 0, 0);


		if (requiredSize > 0) {
			std::vector<wchar_t> buffer(requiredSize);
			MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, &buffer[0], requiredSize);
			convertedString.assign(buffer.begin(), buffer.end() - 1);
		}
		//delete buf, sss;
		return convertedString;
	}

	std::string encode_1251(std::wstring &wstr)
	{
		int size_needed = WideCharToMultiByte(1251, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
		std::string strTo(size_needed, 0);
		WideCharToMultiByte(1251, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);
		return strTo;
	}

	int main()
	{
		
		vosk_set_log_level(-1);
		setlocale(LC_ALL, "Russian");
		//SetConsoleOutputCP(CP_UTF8);
		//SetConsoleCP(CP_UTF8);

		std::cout << "Hello World!\n";
		FILE *wavin;
		char buf[3200];
		int nread, final;

		//VoskModel *model = vosk_model_new("vosk-model-ru-0.6");//vosk-model-small-ru-0.15
		VoskSpkModel *spk_model = vosk_spk_model_new("vosk-model-spk-0.4");
		//VoskRecognizer *recognizer2 = vosk_recognizer_new_spk(model, 8000.0, spk_model);
		//VoskRecognizer *recognizer = vosk_recognizer_new_spk_no_model(8000.0, spk_model);
		bool need_mic = true;
		VoskRecognizer *recognizer = vosk_recognizer_new_spk_no_model(8000.0, spk_model, need_mic);
		
		vosk_set_log_level(-1);
		fopen_s(&wavin, "decoder-test.wav", "rb");
		fseek(wavin, 44, SEEK_SET);
		while (!feof(wavin)) {
			nread = fread(buf, 1, sizeof(buf), wavin);
			final = vosk_recognizer_accept_waveform(recognizer, buf, nread);
			//final = vosk_recognizer_accept_waveform(recognizer2, buf, nread);
			/*std::wstring ws;
			if (final) {
				std::string s(vosk_recognizer_result(recognizer));
				
				ws = string_to_wstring(s);
				std::cout << encode_1251(ws) << '\n';

			}
			else {
				std::string s(vosk_recognizer_partial_result(recognizer));
				ws = string_to_wstring(s);
				std::cout << encode_1251(ws) << '\n';
				
				//printf("%s\n", encode_1251(ws));
			}*/
		}
		std::string s(vosk_recognizer_final_result(recognizer));
		std::wstring ws = string_to_wstring(s);
		std::cout << encode_1251(ws) << '\n';

		//s=(vosk_recognizer_final_result(recognizer2));
		//ws = string_to_wstring(s);
		//std::cout << encode_1251(ws) << '\n';

		//void* trainxv = vosk_getXVector(recognizer);
		//const char* test = "ivan1.wav";
		//float score = vosk_plda2Score(recognizer, "ivan1.wav", "ivan2.wav");
		float score = vosk_cos2Score(recognizer, "ivan1.wav", "ivan2.wav");
		std::cout << score << '\n';
		score = vosk_cos2Score(recognizer, "ivan1.wav", "ivan2.wav",true);
		std::cout << score << '\n';
		score = vosk_cos2Score(recognizer, "ivan1.wav", "decoder-test.wav");
		std::cout << score << '\n';
		score = vosk_cos2Score(recognizer, "ivan1.wav", "decoder-test.wav", true);
		std::cout << score << '\n';
		score = vosk_cos2Score(recognizer, "ivan2.wav", "decoder-test.wav");
		std::cout << score << '\n';
		score = vosk_cos2Score(recognizer, "ivan2.wav", "decoder-test.wav", true);
		std::cout << score << '\n';
		score = vosk_cosScoreMic(recognizer, "ivan1.wav", true);
		std::cout << score << '\n';
		score = vosk_cosScoreMic(recognizer, "ivan2.wav", true);
		std::cout << score << '\n';
		score = vosk_cosScoreMic(recognizer, "decoder-test.wav", true);
		std::cout << score << '\n';
		//vosk_plda2Score
		vosk_recognizer_free(recognizer);
		//vosk_recognizer_free(recognizer2);
		vosk_spk_model_free(spk_model);
		//vosk_model_free(model);
		return 0;
	}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
