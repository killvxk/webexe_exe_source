#pragma once

#include <QWidget>
#include "ui_MainPanel.h"
#include "WhiteBoard.h"
#include "RTCVideoWidget.h"
#include "RTCRoom.h"
#include "RTCRoomUtil.h"
#include <QScrollArea>
#include "WinWidget.h"
#include "TXCloudRecordCmd.h"
#include "RTCShareVideo.h"
#include "common/TXShareFrameMgr.h"

class MultiVideoPanel : public QWidget, public TXShareFrameCallback
{
	Q_OBJECT

public:
	MultiVideoPanel(QWidget *parent = Q_NULLPTR);
	~MultiVideoPanel();

	void onPusherJoin(const MemberItem& member);
    void onPusherQuit(const MemberItem& member);
    void onRoomClosed();

	void setRoomCreator(const std::string& id);
	void setUserInfo(const std::string& id, const std::string& userName);
	int getVideoCount();

	void updatePreview();
	void setDeviceEnabled(bool camera, bool mic);

	void initConfigSetting(int size, bool whiteboard, bool screenShare, ScreenRecordType screenRecord);
	void initShowVideo();

	void on_menu_actCamera(bool open);
	void on_menu_actMic(bool open);
	MenuInfo & getMenuInfo();

	void on_startRecord(ScreenRecordType recordType);

protected:
	virtual void showEvent(QShowEvent *event) Q_DECL_OVERRIDE;
	virtual void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
	virtual void timerEvent(QTimerEvent *event) Q_DECL_OVERRIDE;
	virtual void resizeEvent(QResizeEvent* event) Q_DECL_OVERRIDE;

	virtual void onSwitch(HWND, QRect, bool bFollowWnd);
	virtual void onClose();
private:
	void initUI();
	void initCameraWidget();
	void showLocalWnd(bool show);
	void resizeUI();
	void shareVideo();
private:
	HWND m_hwnd;
	QWidget * widgetDisplay;
	MenuInfo m_menuInfo;
	WinWidget * winWidget;

	Ui::MainPanel ui;
	WhiteBoard * whiteBoard = nullptr;
	std::vector<RTCCameraWidget*> m_vCameraWidgets;
	QScrollArea *scrollArea_camera;
	QHBoxLayout * hCameraLayout;
	RTCVideoWidget * selfWidget = nullptr;
	QWidget* widget_tab_corner;
	int m_tabIndex = 0;
	bool m_screenFull = false;
	bool m_screenArea = false;
	bool m_cameraPreview = true;
	RECT m_areaRect;
	HWND m_shareHwnd = nullptr;
	bool m_bFollowWnd = false;
	TXShareFrameMgr *m_pTXShareFrameMgr = nullptr;
	bool m_cameraEnabled = true;

	std::string m_roomCreator;
	std::string m_userName;
	std::string m_userID;
	int m_cameraSize = 4;

	QPushButton * m_btn_device_manage;
	QPushButton * m_btn_beauty_manage;
	QPushButton * m_btn_esc;
	QPushButton * m_btn_record_manage;
	ScreenRecordType m_screenRecord = RecordScreenNone;
	int m_timerID = 0;
	int m_recordTime = 0;
	QString m_btnStyle;
	QString m_recordBtnStyle;
	bool m_bResize = false;
	RTCShareVideo * m_RTCShareVideo = nullptr;
	bool m_bShareVideo = false;
	bool m_bInitShowEvent = false;

signals:
	void actCamera(bool open);
	void actMic(bool open);
	void record_manage_clicked();

private slots:
	void on_dis_actCamera(bool open);
	void on_dis_actMic(bool open);
	void on_local_actCamera(bool open);
	void on_selectCaptureArea(QRect rect);
	void on_btn_record_manage_clicked();
	void on_tabWidget_currentChanged();
	void on_btn_area_share_clicked();
	void on_btn_full_share_clicked();
	void on_btn_esc_clicked();
};
