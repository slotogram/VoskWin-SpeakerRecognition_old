#pragma once
#include "vosk_api.h"


#include <stdio.h>
#include <locale.h>
#include <iostream>
#include <windows.h>
#include <vector>
#include <string>
#ifdef VOICEAUTH_EXPORTS
#define VOICEAUTH_API __declspec(dllexport)
#else
#define VOICEAUTH_API __declspec(dllimport)
#endif

VoskSpkModel *spk_model;
VoskRecognizer *recognizer;
std::string *model_path_;
char* out = nullptr;
wchar_t* outw = nullptr;

extern "C" VOICEAUTH_API bool init_model(const char *model_path);
extern "C" VOICEAUTH_API bool destroy_model();
extern "C" VOICEAUTH_API bool register_new_user(const char *speaker_id, float rec_len);
extern "C" VOICEAUTH_API bool delete_user(const char *speaker_id);
extern "C" VOICEAUTH_API float get_auth_result_mic(const char *speaker_id);
extern "C" VOICEAUTH_API float get_auth_result_mic_len(const char *speaker_id, float rec_len);
extern "C" VOICEAUTH_API float get_auth_result_wav(const char *speaker_id,const char *wav_path);
extern "C" VOICEAUTH_API const char *get_ident_result(float rec_len, float& top_score); //Todo Add conf score in params
extern "C" VOICEAUTH_API const wchar_t *get_ident_result_w(float rec_len, float& top_score); //Todo Add conf score in params
extern "C" VOICEAUTH_API const char *get_speakers_list();
extern "C" VOICEAUTH_API const wchar_t *get_speakers_list_w();

