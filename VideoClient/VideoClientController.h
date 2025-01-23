#pragma once
#include "Evlc.h"
#include "VideoClientDlg.h"

enum EVlcCommand
{
	EVLC_PLAY,
	EVLC_PAUSE,
	EVLC_STOP,
	EVLC_GET_VOLUME,
	EVLC_GET_POSITION,
	EVLC_GET_LENGTH
};

class VideoClientController
{
public:
	VideoClientController();
	~VideoClientController();
	int Init(CWnd*& pWnd);
	int Invoke();
	int SetMedia(const std::string& strUrl); // 如果strUrl中包含中文符号或字符，则使用utf-8编码
	float VideCtrl(EVlcCommand cmd); // 返回-1.0表示错误
	void SetPosition(float pos);
	int SetWnd(HWND hWnd);
	int SetVolume(int volume);
	VlcSize GetMediaInfo();
	std::string Unicode2Utf8(const std::wstring& strIn);

protected:
	Evlc m_vlc;
	CVideoClientDlg m_dlg;
};

