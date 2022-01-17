//
//  RCSCallDefine.h
//  RongCallCenterLib
//
//  Created by RongCloud on 2021/12/16.
//  Copyright © 2021 RongCloud. All rights reserved.
//

#import <UIKit/UIKit.h>

#ifndef RCSCallDefine_h
#define RCSCallDefine_h

/*!
 媒体类型
 */
typedef NS_ENUM(NSUInteger, RCSCallMediaType) {
    /*!
     纯音频
     */
    RCSCallMediaTypeAudio = 0,
    /*!
     音频+视频
     */
    RCSCallMediaTypeVideo = 1
};

/*!
 音视频引擎支持通话类型
 */
typedef NS_OPTIONS(NSUInteger, RCSCallEngineCapability) {
    /*!
     无
     */
    RCSCallEngineCapabilityNone = 0,
    /*!
     音频单聊
     */
    RCSCallEngineCapabilityAudioSingleCall = 1,
    /*!
     音频群聊
     */
    RCSCallEngineCapabilityAudioMultiCall = 1 << 1,
    /*!
     视频单聊
     */
    RCSCallEngineCapabilityVideoSingleCall = 1 << 2,
    /*!
     视频群聊
     */
    RCSCallEngineCapabilityVideoMultiCall = 1 << 3,
};

/*!
 密聊类型
 */
typedef NS_OPTIONS(NSUInteger, RCSCallSecretChatType) {
    /*!
     普通
     */
    RCSCallSecretChatTypeNO = 0,
    /*!
     密聊
     */
    RCSCallSecretChatTypeYES = 1
};

/*!
 呼叫类型
 */
typedef NS_OPTIONS(NSUInteger, RCSCallType) {
    /*!
     单聊
     */
    RCSCallTypeSingle = 0,
    /*!
     群聊
     */
    RCSCallTypeMulty = 1
};

/*!
 通话状态
 */
typedef NS_ENUM(NSUInteger, RCSCallStatus) {
    /*!
     待机
     */
    RCSCallStatusIdle = 0,
    /*!
     主叫端: 呼出
     */
    RCSCallStatusInviting = 1,
    /*!
     被叫端: 呼入
     */
    RCSCallStatusIncoming = 2,
    /*!
     主/被叫端: 正在连接中
     */
    RCSCallStatusConnecting = 3,
    /*!
     主/被叫端: 已连接
     */
    RCSCallStatusConnected = 4,
    /*!
     主/被叫端: 保持
     */
    RCSCallStatusHolding = 5
    
};

/*!
 用户类型
 */
typedef NS_ENUM(NSUInteger, RCSCallUserType) {
    /*!
     正常用户, 同时收发音视频数据
     */
    RCSCallUserTypeNormal = 0,
    /*!
     观察者, 仅接收音视频数据
     */
    RCSCallUserTypeObserver = 1
};

/*!
 通话视频参数
 */
typedef NS_ENUM(NSUInteger, RCSVideoProfile) {
    /*!
     176x132, 10fps, 150kbps
     */
    RCSVideoProfile132P = 10,
    /*!
     256x144, 15fps, 350kbps
     */
    RCSVideoProfile144P_1 = 11,
    /*!
     240x240, 15fps, 400kbps
     */
    RCSVideoProfile240P = 20,
    /*!
     320x240, 15fps, 500kbps
     */
    RCSVideoProfile240P_1 = 21,
    /*!
     480x360, 15fps, 650kbps
     */
    RCSVideoProfile360P = 30,
    /*!
     640x360, 15fps, 800kbps
     */
    RCSVideoProfile360P_1 = 40,
    /*!
     640x480, 15fps, 1000kbps
     */
    RCSVideoProfile480P = 50,
    /*!
     720x480, 15fps, 1200kbps
     */
    RCSVideoProfile480P_1 = 60,
    /*!
     1280x720, 15fps, 2500kbps
     */
    RCSVideoProfile720P = 70,
    /*!
     1920x1080, 15fps, 4500kbps
     */
    RCSVideoProfile1080P = 80,
    /*!
     默认的视频参数
     */
    RCSVideoProfileDefault = RCSVideoProfile480P
};

/*!
 视频显示模式
 */
typedef NS_ENUM(NSUInteger, RCSCallRenderMode) {
    /*!
     完整显示, 填充黑边, 等比例填充，直到一个维度到达区域边界
     */
    RCSCallRenderModeAspectFit,
    /*!
     满屏显示, 默认, 等比例填充, 直到填充满整个视图区域, 其中一个维度的部分区域会被裁剪
     */
    RCSCallRenderModeAspectFill,
    /*!
     非均匀模式, 两个维度完全填充至整个视图区域, 视频比例可能会变化
     */
    RCSCallRenderModeResize
};

/*!
 通话质量
 */
typedef NS_ENUM(NSUInteger, RCSCallQuality) {
    /*!
     未知
     */
    RCSCallQualityUnknown = 0,
    /*!
     Excellent
     */
    RCSCallQualityExcellent = 1,
    /*!
     Good
     */
    RCSCallQualityGood = 2,
    /*!
     Poor
     */
    RCSCallQualityPoor = 3,
    /*!
     Bad
     */
    RCSCallQualityBad = 4,
    /*!
     VBad
     */
    RCSCallQualityVBad = 5,
    /*!
     Down
     */
    RCSCallQualityDown = 6
};

/*!
 通话结束原因
 */
typedef NS_ENUM(NSUInteger, RCSCallDisconnectReason) {
    /*!
     己方取消已发出的通话请求
     */
    RCSCallDisconnectReasonCancel = 1,
    /*!
     己方拒绝收到的通话请求
     */
    RCSCallDisconnectReasonReject = 2,
    /*!
     己方挂断
     */
    RCSCallDisconnectReasonHangup = 3,
    /*!
     己方忙碌
     */
    RCSCallDisconnectReasonBusyLine = 4,
    /*!
     己方未接听
     */
    RCSCallDisconnectReasonNoResponse = 5,
    /*!
     己方不支持当前引擎
     */
    RCSCallDisconnectReasonEngineUnsupported = 6,
    /*!
     己方网络出错
     */
    RCSCallDisconnectReasonNetworkError = 7,
    /*!
     己方获取媒体资源失败
     */
    RCSCallDisconnectReasonResourceError = 8,
    /*!
     己方发布资源失败
     */
    RCSCallDisconnectReasonPublishError = 9,
    /*!
     己方订阅资源失败
     */
    RCSCallDisconnectReasonSubscribeError = 10,
    /*!
     对方取消已发出的通话请求
     */
    RCSCallDisconnectReasonRemoteCancel = 11,
    /*!
     对方拒绝收到的通话请求
     */
    RCSCallDisconnectReasonRemoteReject = 12,
    /*!
     对方挂断
     */
    RCSCallDisconnectReasonRemoteHangup = 13,
    /*!
     对方忙碌
     */
    RCSCallDisconnectReasonRemoteBusyLine = 14,
    /*!
     对方未接听
     */
    RCSCallDisconnectReasonRemoteNoResponse = 15,
    /*!
     对方不支持当前引擎
     */
    RCSCallDisconnectReasonRemoteEngineUnsupported = 16,
    /*!
     对方网络错误
     */
    RCSCallDisconnectReasonRemoteNetworkError = 17,
    /*!
     对方获取媒体资源失败
     */
    RCSCallDisconnectReasonRemoteResourceError = 18,
    /*!
     对方发布资源失败
     */
    RCSCallDisconnectReasonRemotePublishError = 19,
    /*!
     对方订阅资源失败
     */
    RCSCallDisconnectReasonRemoteSubscribeError = 20,
    /*!
     己方其他端已加入新通话
     */
    RCSCallDisconnectReasonKickedByOtherCall = 21,
    /*!
     己方其他端已在通话中
     */
    RCSCallDisconnectReasonInOtherCall = 22,
    /*!
     己方已被禁止通话
     */
    RCSCallDisconnectReasonKickedByServer = 23,
    /*!
     己方已接听系统来电
     */
    RCSCallDisconnectReasonAcceptSystemCall = 24,
    /*!
     对方其他端已加入新通话
     */
    RCSCallDisconnectReasonRemoteKickedByOtherCall = 31,
    /*!
     对方其他端已在通话中
     */
    RCSCallDisconnectReasonRemoteInOtherCall = 32,
    /*!
     对方已被禁止通话
     */
    RCSCallDisconnectReasonRemoteKickedByServer = 33,
    /*!
     对方已接听系统来电
     */
    RCSCallDisconnectReasonRemoteAcceptSystemCall = 34,
    /*!
     己方其他端已接听
     */
    RCSCallDisconnectReasonAcceptByOtherClient = 101,
    /*!
     己方其他端已挂断
     */
    RCSCallDisconnectReasonHangupByOtherClient = 102,
    /*!
     己方被对方加入黑名单
    */
    RCSCallDisconnectReasonAddToBlackList = 103,
    /*!
     音视频服务已关闭
     */
    RCSCallDisconnectReasonMediaServerClosed = 104,
    /*!
     己方被降级为观察者
     */
    RCSCallDisconnectReasonDegrade = 301,
    /*!
     对方降级为观察者
     */
    RCSCallDisconnectReasonRemoteDegrade = 311
};

/*!
 状态码
 */
typedef NS_ENUM(NSUInteger, RCSCallStatusCode) {
    /*!
     成功
     */
    RCSCallStatusCodeSuccess = 0,
    /*!
     网络不可用
     */
    RCSCallStatusCodeNetworkUnavailable = 1,
    /*!
     已经处于通话中了
     */
    RCSCallStatusCodeOneCallExisted = 2,
    /*!
     无效操作
     */
    RCSCallStatusCodeOperationUnavailable = 3,
    /*!
     参数错误
     */
    RCSCallStatusCodeInvalidParam = 4,
    /*!
     网络不稳定
     */
    RCSCallStatusCodeNetworkUnstable = 5,
    /*!
     媒体服务请求失败
     */
    RCSCallStatusCodeMediaRequestFailed = 6,
    /*!
     媒体服务初始化失败
     */
    RCSCallStatusCodeMediaServerNotReady = 7,
    /*!
     媒体服务未初始化
     */
    RCSCallStatusCodeMediaServerNotInitialized = 8,
    /*!
     媒体服务请求超时
     */
    RCSCallStatusCodeMediaRequestTimeout = 9,
    /*!
     未知的媒体服务错误
     */
    RCSCallStatusCodeMediaUnkownError = 10,
    /*!
     已被禁止通话
     */
    RCSCallMediaKickedByServerError = 11,
    /*!
     音视频服务已关闭
     */
    RCSCallMediaServerClosedError = 12,
    /*!
     音视频发布资源失败
     */
    RCSCallMediaServerPublishError = 13,
    /*!
     音视频订阅资源失败
     */
    RCSCallMediaServerSubscribeError = 14,
    /*!
     其他端已在通话中
     */
    RCSCallMediaJoinRoomRefuseError = 15,
    /*!
     己方其他端已加入其他房间
     */
    RCSCallMediaOtherClientJoinedRoom = 16,
    /*!
     默认音视频取消发布资源失败
     */
    RCSCallMediaServerUnpublishDefaultError = 17,
    /*!
     默认视频取消发布资源失败
     */
    RCSCallMediaServerUnpublishDefaultVideoError = 18,
};

/*!
 同UserId两端同时加入通话类型
 */
typedef NS_ENUM(NSUInteger, RCSCallJoinCallType) {
    /*!
     将先加入通话用户踢出通话
     */
    RCSCallJoinCallTypeKick = 0,
    /*!
     拒绝当前用户加入通话
     */
    RCSCallJoinCallTypeRefuse = 1,
    /*!
     两端共存, 可同时都在通话中
     */
    RCSCallJoinCallTypeCoexist = 2
};

/*!
 通话变更媒体事件类型
 */
typedef NS_ENUM(NSUInteger, RCSCallChangeActionType) {
    /*!
     请求
     */
    RCSCallChangeActionTypeRequest = 0,
    /*!
     同意
     */
    RCSCallChangeActionTypeAccept = 1,
    /*!
     拒绝
     */
    RCSCallChangeActionTypeRefuse = 2,
    /*!
     取消
     */
    RCSCallChangeActionTypeCancel = 3,
    /*!
     公告
     */
    RCSCallChangeActionTypeNotice = 4
};

#endif /* RCSCallDefine_h */
