// VoskTest.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#include "vosk_api.h"
//#include "kaldi-types.h"
//#include "model.h"
#include "kaldi_voice_auth.h"
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
		
		vosk_set_log_level(-4);
		setlocale(LC_ALL, "Russian");
		//SetConsoleOutputCP(CP_UTF8);
		//SetConsoleCP(CP_UTF8);

		std::cout << "Hello World!\n";
		FILE *wavin;
		char buf[3200];
		int nread, final;

		//VoskModel *model = vosk_model_new("vosk-model-ru-0.6");//vosk-model-small-ru-0.15
		VoskSpkModel *spk_model = vosk_spk_model_new("vosk-model-spk-0.4");
		//VoskSpkModel *spk_model = vosk_spk_model_new("my-spk"); 
		//VoskRecognizer *recognizer2 = vosk_recognizer_new_spk(model, 8000.0, spk_model);
		//VoskRecognizer *recognizer = vosk_recognizer_new_spk_no_model(8000.0, spk_model);
		bool need_mic = true;
		VoskRecognizer *recognizer = vosk_recognizer_new_spk_no_model(spk_model, need_mic);
		
		//vosk_set_log_level(-1);
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
		float score;
		//vosk_compute_voxceleb_xvectors(recognizer, "vox_vad_my_model.ark", "C:\\Data\\vox1\\test\\wav");
		//vosk_compute_voxceleb_xvectors(recognizer, "vox_vad_my_cmvn_before.ark", "C:\\Data\\vox1\\test\\wav");
		//Equal error rate is 2.31707%, at threshold -1.02434
		//vosk_plda_score_trial(recognizer, "vox_vad_my_cmvn_before.ark", "C:\\Data\\vox1\\veri_test.txt", "my_model_out_cmvn_before.txt");
		//Equal error rate is 2.33828%, at threshold -0.986595
		//vosk_plda_score_trial(recognizer, "vox_vad_my_model.ark", "C:\\Data\\vox1\\veri_test.txt", "my_model_out.txt");
		//Equal error rate is 11.6755%, at threshold 8.1054
		//Equal error rate is 12.6246%, at threshold 8.8546 (2nd)
		vosk_plda_score_trial(recognizer, "vox_vad_8k.ark", "C:\\Data\\vox1\\veri_test.txt", "spk_model_out_8k.txt");
		//Equal error rate is 13.07%, at threshold 9.10212
		//Equal error rate is 11.6172%, at threshold 8.27211 (2nd)
		vosk_plda_score_trial(recognizer, "vox_vad_2nd_8k.ark", "C:\\Data\\vox1\\veri_test.txt", "spk_model_out_2nd_8k.txt");

		//if (!vosk_save_xvector_mic(recognizer, "spk1", 10))
		//	std::cout << "Spk xvector not saved";
		//float score = vosk_plda_score_mic(recognizer, "spk.ark", "spk1");
		//std::cout << score << '\n';
		//s=(vosk_recognizer_final_result(recognizer2));
		//ws = string_to_wstring(s);
		//std::cout << encode_1251(ws) << '\n';

		//void* trainxv = vosk_getXVector(recognizer);
		//const char* test = "ivan1.wav";
		//score = vosk_plda2Score(recognizer, "ivan1_16.wav", "ivan2_16.wav");
		//float score = vosk_cos2Score(recognizer2, "ivan1.wav", "ivan2.wav");
		//std::cout << score << '\n';
		//score = vosk_plda2Score(recognizer, "ivan1_16.wav", "decoder-test.wav");
		//std::cout << score << '\n';
		//vosk_recognizer_free(recognizer);
		//recognizer = vosk_recognizer_new_spk_no_model(8000.0, spk_model, false);
		/*score = vosk_cos2Score(recognizer2, "ivan1.wav", "ivan2.wav");
		std::cout << score << '\n';
		score = vosk_cos2Score(recognizer, "ivan1.wav", "ivan2.wav");
		std::cout << score << '\n';
		score = vosk_cos2Score(recognizer, "ivan1.wav", "ivan2.wav");
		std::cout << score << '\n';

		/*score = vosk_cos2Score(recognizer, "ivan1.wav", "ivan2.wav",true);
		std::cout << score << '\n';
		score = vosk_cos2Score(recognizer, "ivan1.wav", "decoder-test.wav");
		std::cout << score << '\n';
		score = vosk_cos2Score(recognizer, "ivan1.wav", "decoder-test.wav", true);
		std::cout << score << '\n';
		score = vosk_cos2Score(recognizer, "ivan2.wav", "decoder-test.wav");
		std::cout << score << '\n';
		score = vosk_cos2Score(recognizer, "ivan2.wav", "decoder-test.wav", true);
		std::cout << score << '\n';
		/*score = vosk_cosScoreMic(recognizer, "ivan1.wav", true);
		std::cout << score << '\n';
		score = vosk_cosScoreMic(recognizer, "ivan2.wav", true);
		std::cout << score << '\n';
		score = vosk_cosScoreMic(recognizer, "decoder-test.wav", true);
		std::cout << score << '\n';*/


		//vosk_plda2Score
		/*std::cout << "PLDA score:\n";
		score = vosk_plda2Score(recognizer, "ivan1.wav", "ivan2.wav");
		std::cout << score << '\n';
		score = vosk_plda2Score(recognizer, "ivan1.wav", "decoder-test.wav");
		std::cout << score << '\n';
		score = vosk_plda2Score(recognizer, "ivan1.wav", "decoder-test.wav");
		std::cout << score << '\n';
		score = vosk_plda2Score(recognizer, "ivan1.wav", "decoder-test.wav");
		std::cout << score << '\n';
		score = vosk_plda2Score(recognizer, "ivan1.wav", "decoder-test.wav");
		std::cout << score << '\n';
		score = vosk_plda2Score(recognizer, "ivan2.wav", "decoder-test.wav");
		std::cout << score << '\n';
		score = vosk_plda2Score(recognizer, "ivan2.wav", "decoder-test.wav");
		std::cout << score << '\n';
		score = vosk_plda2Score(recognizer, "ivan2.wav", "decoder-test.wav");
		std::cout << score << '\n';
		score = vosk_plda2Score(recognizer, "ivan2.wav", "decoder-test.wav");
		std::cout << score << '\n';
		/*score = vosk_pldaScoreMic(recognizer, "ivan2.wav");
		std::cout << score << '\n';
		score = vosk_pldaScoreMic(recognizer, "ivan1.wav");
		std::cout << score << '\n';
		score = vosk_pldaScoreMic(recognizer, "decoder-test.wav");
		std::cout << score << '\n';*/
		vosk_recognizer_free(recognizer);
		//vosk_recognizer_free(recognizer2);
		vosk_spk_model_free(spk_model);
		//vosk_model_free(model);
		return 0;
	}

	int main2()
	{

		//vosk_set_log_level(-4);
		setlocale(LC_ALL, "Russian");
		std::cout << "Hello World!\n";


		init_model("my-spk");
		int action = 0;
		bool exit_program = false;
		while (!exit_program)
		{
			std::cout << "\n\nSelect one of the actions:\n\n";
			std::cout << "1. Create new user\n";
			std::cout << "2. Authenticate user\n";
			std::cout << "3. Authenticate user from wav file\n";
			std::cout << "4. Identificate user from all users\n";
			std::cout << "5. Get list of all users\n";
			std::cout << "6. Exit program\n";
			std::cin >> action;
			//std::string speaker_id;
			std::string path;
			float score;
			std::string speaker_id;
			switch (action)
			{
				case 1: 
				{
					std::cout << "Print speaker id\n";
					std::getline(std::cin, speaker_id);
					std::getline(std::cin, speaker_id);
					//std::cin >> speaker_id;
					register_new_user(speaker_id.c_str(), 10);
					break;
				}
				case 2:
				{
					std::cout << "Print speaker id\n";
					std::getline(std::cin, speaker_id);
					std::getline(std::cin, speaker_id);
					score = get_auth_result_mic_len(speaker_id.c_str(), 5);
					std::cout << "Speaker score: " << score << "\n";
					break;
				}
				case 3:
				{
					std::cout << "Print speaker id\n";
					std::getline(std::cin, speaker_id);
					std::getline(std::cin, speaker_id);
					std::cout << "Print path to wav\n";
					std::getline(std::cin, path);
					score = get_auth_result_wav(speaker_id.c_str(), path.c_str());
					std::cout << "Speaker score: " << score << "\n";
					break;
				}
				case 4:
				{
					//const char* result;
					std::getline(std::cin, speaker_id);
					speaker_id = get_ident_result(5,score);
					std::cout << "Speaker identification result: " << speaker_id << " score: " << score<<"\n";
					break;
				}
				case 5:
				{
					std::getline(std::cin, speaker_id);
					speaker_id = get_speakers_list();
					std::cout << "Speaker list: \n" << speaker_id << "\n";
					break;
				}
				case 6:
				{
					std::getline(std::cin, speaker_id);
					exit_program = true;					
					break;
				}
				case 7:
				{
					vosk_plda_score_trial(recognizer, "vox_vad_my_cmvn_before.ark", "C:\\Data\\vox1\\veri_test.txt", "my_model_out_cmvn_before.txt");
					vosk_plda_score_trial(recognizer, "vox_vad_my_model.ark", "C:\\Data\\vox1\\veri_test.txt", "my_model_out.txt");
					vosk_plda_score_trial(recognizer, "vox_vad_8k.ark", "C:\\Data\\vox1\\veri_test.txt", "spk_model_out_8k.txt");
					vosk_plda_score_trial(recognizer, "vox_vad_2nd_8k.ark", "C:\\Data\\vox1\\veri_test.txt", "spk_model_out_2nd_8k.txt");
				}
			default:
				std::getline(std::cin, speaker_id);
				std::cout << "Input is not correct\n";
				break;
			}
		}
		destroy_model();
		return 0;
	}

