#include "kaldi_voice_auth.h"
//#include "base/kaldi-common.h"
//#include "util/common-utils.h"

const wchar_t *GetWC(const char *c)
{
	size_t cSize = strlen(c) + 1;
	wchar_t* wc = new wchar_t[cSize];
	//mbstowcs(wc, c, cSize);
	std::cerr<<"Got in GetWC func";
	size_t* converted(0);
	mbstowcs_s(converted, wc, cSize, c, cSize);
    std::wcerr << wc << "\n";
	return wc;
}

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

bool init_model(const char *model_path)
{
	vosk_set_log_level(-4);
	spk_model = vosk_spk_model_new(model_path);
	bool need_mic = true; //for now it's only true
	recognizer = vosk_recognizer_new_spk_no_model(spk_model, need_mic);
	model_path_ = new std::string(model_path);
	return true;
}

bool destroy_model()//Todo delete all returned wchar_t arrays
{
	//delete out;
	//delete outw;
	vosk_recognizer_free(recognizer);
	vosk_spk_model_free(spk_model);
	delete model_path_;
}

bool register_new_user(const char *speaker_id, float rec_len)
{
	std::string path = *model_path_ + std::string("\\spk.ark");
	if (vosk_save_xvector_mic(recognizer, path.c_str(), speaker_id, rec_len))
		return true;
	else
		return false;
}
bool delete_user(const char *speaker_id)
{
	std::string path = *model_path_ + std::string("\\spk.ark");
	if (vosk_delete_user(recognizer, path.c_str(), speaker_id))
		return true;
	else
		return false;
}
float get_auth_result_mic(const char *speaker_id)
{
	std::string path = *model_path_ + std::string("\\spk.ark");
	return  vosk_plda_score_mic(recognizer, path.c_str(), speaker_id);

}

float get_auth_result_mic_len(const char *speaker_id, float rec_len)
{
	std::string path = *model_path_ + std::string("\\spk.ark");
	return  vosk_plda_score_mic_len(recognizer, path.c_str(), speaker_id, rec_len);

}

float get_auth_result_wav(const char *speaker_id, const char *wav_path)
{
	std::string path = *model_path_ + std::string("\\spk.ark");
	return  vosk_plda_score_wav(recognizer, path.c_str(), speaker_id, wav_path);
}

const char *get_speakers_list()
{
	std::string path = *model_path_ + std::string("\\spk.ark");
	const char* result = vosk_get_speakers_list(recognizer, path.c_str());
	size_t len = strlen(result)+1;
	//delete out;
	out = new char[len];
	//strncpy(out, result, len);
	strncpy_s(out, len, result, len);
	return out;
	//return vosk_get_speakers_list(recognizer, path.c_str());
}

const wchar_t *get_speakers_list_w()
{
	const char* result = get_speakers_list();
	//std::cerr << "Done Vosk funct call";
	std::string s(result);
	std::cerr << "Done Vosk funct call: "<< s<< "\n";
	std::wstring ws = string_to_wstring(s);
	std::wcerr << "Done str_to_wstr: " << ws << "\n";
	
	rsize_t len = strlen(result) + 1;
	//delete outw;
	outw = new wchar_t[len];
	wcsncpy_s(outw, len, ws.c_str(), len);
	std::wcerr << "Done copy to new arr: " << outw << "\n";
	//return encode_1251(ws).c_str();
	//const wchar_t* ans = ws.c_str();
	//std::wstring???
	return outw;
	//return ws.c_str();
	//return ws;
}

const char *get_ident_result(float rec_len, float& top_score)
{
	std::string path = *model_path_ + std::string("\\spk.ark");
	return vosk_get_ident_result(recognizer, path.c_str(), rec_len, top_score);
}

const wchar_t *get_ident_result_w(float rec_len, float& top_score)
{
	std::string path = *model_path_ + std::string("\\spk.ark");
	const char* result = vosk_get_ident_result(recognizer, path.c_str(), rec_len, top_score);
	std::string s(result);
	std::wstring ws = string_to_wstring(s);

	rsize_t len = strlen(result) + 1;
	//delete outw;
	outw = new wchar_t[len];
	wcsncpy_s(outw, len, ws.c_str(), len);
	return outw;
}