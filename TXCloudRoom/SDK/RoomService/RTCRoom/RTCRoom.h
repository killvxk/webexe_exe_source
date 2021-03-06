#ifndef __RTCROOM_H__
#define __RTCROOM_H__

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <memory>
#include "RTCRoomUtil.h"

#define MIC_DEVICE_NAME_MAX_SIZE    (512)

class RTCRBussiness;

class RTCRoom
{
public:
    RTCRoom();
    virtual ~RTCRoom();

	/**
    * \brief：获取RTCRoom单例，通过单例调用RTCRoom的接口
	*/
	static RTCRoom* instance();

    /**
    * \brief：设置回调 RTCRoom 的回调代理，监听 RTCRoom 的内部状态和接口的执行结果
    * \param：callback - IRTCRoomCallback 类型的代理指针
    * \return 无
    */
    void setCallback(IRTCRoomCallback * callback);

    /**
    * \brief：设置代理地址
    * \param：ip - 代理服务器的ip
    * \param：port - 代理服务器的端口
    * \return 无
    */
    void setProxy(const std::string& ip, unsigned short port);

    /**
    * \brief：登录业务服务器RoomService，登录后才能够正常使用其他接口和使用IM功能
    * \param：serverDomain - RoomService的URL地址，安全起见，建议访问https加密连接
    * \param：authData - RoomService提供的登录信息，包括IM相关的配置字段，在login成功后，获取到token字段
    * \param：callback - ILoginCallback 类型的代理指针，回调login的结果
    */
    void login(const std::string & serverDomain, const RTCAuthData & authData, ILoginRTCCallback* callback);

	/**
	* \brief：录制本地视频，请注意在startLocalPreview之前调用。录制的同时会开启混流，可将房间成员的视频合成在创建者画面中
	* \param：multi - 是否为多人混流。若为true, 混流画面将合成在视频下方，若为false，混流画面将合成在视频右方
	* \param：picture_id - 背景图水印ID。若不设置，默认为-1，背景图显示为黑色画布
	*/
	void recordVideo(bool multi, int picture_id = -1);

    /**
    * \brief：登出业务服务器RoomService，请注意在leaveRoom调用后，再调用logout，否则leaveRoom会调用失败
    */
    void logout();

    /**
    * \brief：获取房间列表，房间数量比较多时，可以分页获取
    * \param：index - 分页获取，初始默认可设置为0，后续获取为起始房间索引（如第一次设置index=0, cnt=5,获取第二页时可用index=5）
    * \param：cnt - 每次调用，最多返回房间个数；0表示所有满足条件的房间
    * \param：callback - IGetRTCRoomListCallback 类型的代理指针，查询结果的回调
    */
    void getRoomList(int index, int cnt, IGetRTCRoomListCallback* callback);

    /**
    * \brief：创建房间，后台的房间列表中会添加一个新房间，同时会议创建者会进入推流模式
    * \param：roomID - 房间ID，若传入空字符串，则后台会为您分配roomID，否则，传入的roomID作为这个房间的ID
    * \param：roomInfo - 自定义数据，该字段包含在房间信息中，推荐您将 roomInfo 定义为 json 格式，这样可以有很强的扩展性
    */
    void createRoom(const std::string& roomID, const std::string& roomInfo);

    /**
    * \brief：进入房间
    * \param：roomID - 要进入的房间ID，在 getRoomList 接口房间列表中查询得到
    */
    void enterRoom(const std::string& roomID);

    /**
    * \brief：离开房间，如果是会议创建者，这个房间会被后台销毁，如果是会议参与者，不影响其他人继续通话
    */
    void leaveRoom();

    /**
    * \brief：在房间内，发送普通文本消息
    * \param：msg - 文本消息
    */
    void sendRoomTextMsg(const char * msg);

    /**
    * \brief：在房间内，发送普通自定义消息
    * \param：cmd - 自定义cmd，收发双方协商好的cmd
    * \param：msg - 自定义消息
    */
    void sendRoomCustomMsg(const char * cmd, const char * msg);

    /**
    * \brief：发送C2C自定义消息
    * \param：cmd - 自定义cmd，收发双方协商好的cmd
    * \param：msg - 自定义消息
    */
    void sendC2CCustomMsg(const char* userID, const char * cmd, const char * msg);

    /**
    * \brief：启动默认的摄像头预览
    * \param：rendHwnd - 承载预览画面的 HWND，目前 SDK 是采用 OpenGL 向 HWND 上绘制图像的,rendHwnd = null时无需预览视频
    * \param：rect - 指定视频图像在 HWND 上的渲染区域
    */
    void startLocalPreview(HWND rendHwnd, const RECT & rect);

    /**
    * \brief：重设摄像头预览区域，当您指定的本地 HWND 的窗口尺寸发生变化时，可以通过这个函数重新调整视频渲染区域
    * \param：rendHwnd - 承载预览画面的 HWND
    * \param：rect - 指定视频图像在 HWND 上的渲染区域
    * \return 无
    */
    void updateLocalPreview(HWND rendHwnd, const RECT &rect);

    /**
    * \brief：关闭摄像头预览
    */
    void stopLocalPreview();

    /**
    * \brief：启动屏幕分享
    * \param：rendHwnd - 承载预览画面的 HWND，目前 SDK 是采用 OpenGL 向 HWND 上绘制图像的,rendHwnd = null时无需预览视频
    * \param：captureHwnd - 指定录取窗口，若为NULL，则 captureRect 不起效，并且录取整个屏幕；若不为NULL，则录取这个窗口的画面
    * \param：renderRect - 指定视频图像在 rendHwnd 上的渲染区域
    * \param：captureRect - 指定录取窗口客户区的区域
    * \return true成功，false失败
    */
    bool startScreenPreview(HWND rendHwnd, HWND captureHwnd, const RECT & renderRect, const RECT & captureRect, bool bFollowWndRect = false);

    /**
    * \brief：关闭屏幕分享
    */
    void stopScreenPreview();

    /**
    * \brief：播放房间内其他会议参与者的视频
    * \param：rendHwnd - 承载预览画面的 HWND
    * \param：rect - 指定视频图像在 HWND 上的渲染区域
    * \param：userID - 用户ID
    */
    void addRemoteView(HWND rendHwnd, const RECT & rect, const char * userID);

    /**
    * \brief：重设指定userID的视频预览区域，当您指定的本地 HWND 的窗口尺寸发生变化时，可以通过这个函数重新调整视频渲染区域
    * \param：rendHwnd - 承载预览画面的 HWND
    * \param：rect     - 指定视频图像在 HWND 上的渲染区域，比如 (0，0，width, height) 表示拿整个 HWND 作为渲染画布
    * \param：userID - 用户ID
    */
    void updateRemotePreview(HWND rendHwnd, const RECT &rect, const char * userID);

    /**
    * \brief：停止播放其他会议参与者的视频
    * \param：userID - 用户ID
    */
    void removeRemoteView(const char * userID);

    /**
    * \brief：静音接口
    * \param：mute - 是否静音
    */
    void setMute(bool mute);

    /**
    * \brief：设置美颜和美白效果
    * \param：beautyStyle    - 参考 RTCRoomUtil.h 中定义的 RTCBeautyStyle 枚举值
    * \param：beautyLevel    - 美颜级别取值范围 1 ~ 9； 0 表示关闭，1 ~ 9值越大，效果越明显
    * \param：whitenessLevel - 美白级别取值范围 1 ~ 9； 0 表示关闭，1 ~ 9值越大，效果越明显
    * \return:无
    */
    void setBeautyStyle(RTCBeautyStyle beautyStyle, int beautyLevel, int whitenessLevel);

	/**
	* \brief：枚举当前的摄像头，如果一台Windows同时安装了多个摄像头，那么此函数获取可用的摄像头数量和名称
	* \param：camerasName - 每个摄像头的名字
	* \param: capacity - camerasName 数组的大小
	* \return：当前Windows可用的 摄像头的数量
	* \attention: 该函数可以分两次调用，第一次调用设置 camerasName 为 NULL，可以获得摄像头数量，第二次调用时就可以创建一个大小刚刚合适的 camerasName
	*/
	int enumCameras(wchar_t **camerasName = NULL, size_t capacity = 0);

	/**
	* \brief：切换摄像头，支持在推流中动态切换，
	* \param：cameraIndex : 摄像头需要，取值返回：  0 ~ (摄像头个数 - 1)
	* \return:无
	*/
	void switchCamera(int cameraIndex);

	/**
	* \brief：查询可用的麦克风设备的数量
	* \return：若查询成功，则返回值>=0；若查询失败，则返回值为-1
	*/
	int micDeviceCount();

	/**
	* \brief：查询麦克风设备的名称
	* \param：index - 麦克风设备的索引，要求index值小于 micDeviceCount 接口的返回值
	* \param：name - 用于存放麦克风设备的名称的字符串数组，数组大小要求 <= MIC_DEVICE_NAME_MAX_SIZE，查询得到的字符编码格式是UTF-8
	* \return：若查询成功，则返回值true；若查询失败或者index非法，则返回值为false
	*/
	bool micDeviceName(unsigned int index, char name[MIC_DEVICE_NAME_MAX_SIZE]);

	/**
	* \brief：选择指定的麦克风作为录音设备，不调用该接口时，默认选择索引为0的麦克风
	* \param：index - 麦克风设备的索引，要求index值小于 micDeviceCount 接口的返回值
	*/
	void selectMicDevice(unsigned int index);

	/**
	* \brief：查询已选择麦克风的音量
	* \return：音量值，范围是[0, 65535]
	*/
	unsigned int micVolume();

	/**
	* \brief：设置已选择麦克风的音量
	* \param：volume - 设置的音量大小，范围是[0, 65535]
	*/
	void setMicVolume(unsigned int volume);

protected:
    RTCRBussiness* m_impl;
};

#endif
