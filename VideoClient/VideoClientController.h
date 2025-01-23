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
	int SetMedia(const std::string& strUrl); // ���strUrl�а������ķ��Ż��ַ�����ʹ��utf-8����
	float VideCtrl(EVlcCommand cmd); // ����-1.0��ʾ����
	void SetPosition(float pos);
	int SetWnd(HWND hWnd);
	int SetVolume(int volume);
	VlcSize GetMediaInfo();
	std::string Unicode2Utf8(const std::wstring& strIn);

protected:
	Evlc m_vlc;
	CVideoClientDlg m_dlg;
};

