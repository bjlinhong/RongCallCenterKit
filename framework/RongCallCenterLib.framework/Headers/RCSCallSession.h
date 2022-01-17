//
//  RCSCallSession.h
//  RongCallCenterLib
//
//  Created by RongCloud on 2021/12/16.
//  Copyright © 2021 RongCloud. All rights reserved.
//

#import <CoreMedia/CoreMedia.h>
#import <Foundation/Foundation.h>
#import <RongIMLibCore/RongIMLibCore.h>
#import "RCSCallDefine.h"

@class RCSCallProfile;
@class RCSCallUserProfile;
@class RCSCallSession;

NS_ASSUME_NONNULL_BEGIN


/*!
 通话事件的代理回调
 */
@protocol RCSCallSessionDelegate <NSObject>

@optional

/*!
 通话已发起
 
 @param session 当前通话会话
 @param callId 当前通话 ID
 @discussion
 通话已发起
 
 @remarks 代理
 */
- (void)callSession:(RCSCallSession *)session callDidStarted:(NSString *)callId;

/*!
 通话已接通
 
 @param session 当前通话会话
 @param callId 当前通话 ID
 @discussion
 通话已接通
 
 @remarks 代理
 */
- (void)callSession:(RCSCallSession *)session callDidConnect:(NSString *)callId;

/*!
 通话已结束
 
 @param session 当前通话会话
 @param callId 当前通话 ID
 @discussion
 通话已结束
 
 @remarks 代理
 */
- (void)callSession:(RCSCallSession *)session callDidDisconnect:(NSString *)callId;

/*!
 主叫端发起呼叫后, 播放等待音
 
 @param session 当前通话会话
 @param callId 当前通话 ID
 @discussion
 主叫端发起呼叫后, 播放等待音
 
 @remarks 代理
 */
- (void)callSession:(RCSCallSession *)session
shouldAlertForWaitingRemoteResponse:(NSString *)callId;

/*!
 对端用户正在振铃
 
 @param session 当前通话会话
 @param userId 正在振铃的用户ID
 @discussion
 对端用户正在振铃
 
 @remarks 代理
 */
- (void)callSession:(RCSCallSession *)session remoteUserDidRing:(NSString *)userId;

/*!
 对端用户正在另一个已接通的通话中
 
 @param session 当前通话会话
 @param userId 正在通话中的用户ID
 @discussion
 对端用户正在另一个已接通的通话中
 
 @remarks 代理
 */
- (void)callSession:(RCSCallSession *)session remoteUserDidBusy:(NSString *)userId;

/*!
 对端用户正在另一个尚未接听的呼叫中
 
 @param session 当前通话会话
 @param userId 正在通话中的用户ID
 @discussion
 对端用户正在另一个尚未接听的呼叫中
 
 @remarks 代理
 */
- (void)callSession:(RCSCallSession *)session remoteUserDidCallRing:(NSString *)userId;

/*!
 被叫端收到来电后, 播放来电铃声
 
 @param session 当前通话会话
 @param callId 当前通话 ID
 @discussion
 被叫端收到来电后, 播放来电铃声
 
 @remarks 代理
 */
- (void)callSession:(RCSCallSession *)session
shouldRingForIncomingCall:(NSString *)callId;

/*!
 通话接通或挂断时停止播放铃声
 
 @param session 当前通话会话
 @param callId 当前通话 ID
 @discussion
 通话接通或挂断时停止播放铃声
 
 @remarks 代理
 */
- (void)callSession:(RCSCallSession *)session
shouldStopAlertAndRing:(NSString *)callId;

/*!
 有用户被邀请加入通话
 
 @param session 当前通话会话
 @param userId 被邀请的用户ID
 @param mediaType 希望被邀请者使用的媒体类型
 @discussion
 有用户被邀请加入通话
 
 @remarks 代理
 */
- (void)callSession:(RCSCallSession *)session
remoteUserDidInvite:(NSString *)userId
          mediaType:(RCSCallMediaType)mediaType;

/*!
 对端用户加入了通话
 
 @param session 当前通话会话
 @param userId 用户ID
 @param mediaType 用户的媒体类型
 @discussion
 对端用户加入了通话
 
 @remarks 代理
 */
- (void)callSession:(RCSCallSession *)session
  remoteUserDidJoin:(NSString *)userId
          mediaType:(RCSCallMediaType)mediaType;

/*!
 对端用户挂断
 
 @param session 当前通话会话
 @param userId 用户ID
 @param reason 挂断的原因
 @discussion
 对端用户挂断
 
 @remarks 代理
 */
- (void)callSession:(RCSCallSession *)session
  remoteUserDidLeft:(NSString *)userId
             reason:(RCSCallDisconnectReason)reason;

/*!
 变更当前通话媒体类型时的应答对方响应回调
 
 @param session 当前通话会话
 @param userId 用户ID
 @param mediaType 用户的媒体类型
 @param actionType 变更事件类型
 @discussion
 变更当前通话媒体类型时的应答对方响应回调
 
 @remarks 代理
 */
- (void)callSession:(RCSCallSession *)session
remoteUserDidRequest:(NSString *)userId
          mediaType:(RCSCallMediaType)mediaType
         actionType:(RCSCallChangeActionType)actionType;

/*!
 对端用户切换了媒体类型
 
 @param session 当前通话会话
 @param userId 用户ID
 @param mediaType 切换至的媒体类型
 @discussion
 对端用户切换了媒体类型
 仅在视频变更为音频通话时回调, 此变更不需要请求与应答
 
 @remarks 代理
 */
- (void)callSession:(RCSCallSession *)session
remoteUserDidChangeMediaType:(NSString *)userId
          mediaType:(RCSCallMediaType)mediaType;

/*!
 对端用户开启或关闭了麦克风的状态
 
 @param session 当前通话会话
 @param mute YES:关闭麦克风 NO:打开麦克风
 @param userId 用户ID
 @discussion
 对端用户开启或关闭了麦克风的状态
 
 @remarks 代理
 */
- (void)callSession:(RCSCallSession *)session
remoteUserDidMicrophoneMute:(BOOL)mute
             byUser:(NSString *)userId;

/*!
 对端用户开启或关闭了摄像头的状态
 
 @param session 当前通话会话
 @param enable YES:打开摄像头 NO:关闭摄像头
 @param userId 用户ID
 @discussion
 对端用户开启或关闭了摄像头的状态
 
 @remarks 代理
 */
- (void)callSession:(RCSCallSession *)session
remoteUserDidCameraEnable:(BOOL)enable
             byUser:(NSString *)userId;

/*!
 对端用户发布默认媒体流
 
 @param session 当前通话会话
 @param streamIdArray 媒体流ID列表
 @param userId 对端用户ID
 @discussion
 对端用户发布默认媒体流
 
 @remarks 代理
 */
- (void)callSession:(RCSCallSession *)session
remoteUserDidPublish:(NSArray *)streamIdArray
             byUser:(NSString *)userId;

/*!
 对端用户取消发布默认媒体流
 
 @param session 当前通话会话
 @param streamIdArray 媒体流ID列表
 @param userId 对端用户ID
 @discussion
 对端用户取消发布默认媒体流
 
 @remarks 代理
 */
- (void)callSession:(RCSCallSession *)session
remoteUserDidunpublish:(NSArray *)streamIdArray
             byUser:(NSString *)userId;

/*!
 对端用户发布自定义媒体流
 
 @param session 当前通话会话
 @param streamId 自定义流ID
 @param tag 自定义流标签
 @param mediaType 媒体类型
 @discussion
 对端用户发布自定义媒体流
 
 @remarks 代理
 */
- (void)callSession:(RCSCallSession *)session
remoteUserDidPublishCustomMediaStream:(NSString *)streamId
          streamTag:(NSString *)tag
          mediaType:(RCSCallMediaType)mediaType;

/*!
 对端用户取消发布自定义媒体流
 
 @param session 当前通话会话
 @param streamId 自定义流ID
 @discussion
 对端用户取消发布自定义媒体流
 
 @remarks 代理
 */
- (void)callSession:(RCSCallSession *)session
remoteUserDidUnpublishCustomMediaStream:(NSString *)streamId;

/*!
 对端用户发布默认音视频流, 成为正常用户
 
 @param session 当前通话会话
 @param userId 对端成为正常用户的ID
 @discussion
 对端用户发布默认音视频流, 成为正常用户
 
 @remarks 代理
 */
- (void)callSession:(RCSCallSession *)session
remoteUserDidUngrade:(NSString *)userId;

/*!
 对端用户取消发布默认音视频流, 成为观察者用户
 
 @param session 当前通话会话
 @param userId 对端成为观察者用户的ID
 @discussion
 对端用户取消发布默认音视频流, 成为观察者用户
 
 @remarks 代理
 */
- (void)callSession:(RCSCallSession *)session
remoteUserDidDegrade:(NSString *)userId;

/*!
 发布本地资源
 
 @param session 当前通话会话
 @param success 发布资源是否成功
 @discussion
 发布本地资源
 
 @remarks 代理
 */
- (void)callSession:(RCSCallSession *)session
localUserDidPublishStream:(BOOL)success;

/*!
 取消发布本地资源
 
 @param session 当前通话会话
 @param success 取消发布资源是否成功
 @discussion
 取消发布本地资源,
 仅在正常角色取消发布本地资源变为观察者后回调
 
 @remarks 代理
 */
- (void)callSession:(RCSCallSession *)session
localUserDidUnpublishStream:(BOOL)success;

/*!
 通话过程中的错误回调
 
 @param session 当前通话会话
 @param error 错误码
 @discussion
 通话过程中的错误回调
 
 @warning
 这个接口回调的错误码主要是为了提供必要的log以及提示用户,
 如果是不可恢复的错误，SDK会挂断电话并回调callDidDisconnect,
 App可以在callDidDisconnect中统一处理通话结束的逻辑
 
 @remarks 代理
 */
- (void)callSession:(RCSCallSession *)session
      errorDidOccur:(RCSCallStatusCode)error;

/*!
 当前通话网络状态的回调，该回调方法每秒触发一次
 
 @param session 当前通话会话
 @param txQuality 上行网络质量
 @param rxQuality 下行网络质量, 接收到的所有对端用户网络质量的平均值
 @discussion
 当前通话网络状态的回调，该回调方法每秒触发一次
 
 @remarks 代理
 */
- (void)callSession:(RCSCallSession *)session
   networkTxQuality:(RCSCallQuality)txQuality
          rxQuality:(RCSCallQuality)rxQuality;

/*!
 当前通话网络状态的回调，该回调方法每秒触发一次
 
 @param session 当前通话会话
 @param txQuality 上行网络质量
 @param rxQuality 下行网络质量, 接收到的某个对端用户的网络质量
 @param userId 对端用户
 @discussion
 当前通话网络状态的回调，该回调方法每秒触发一次
 
 @remarks 代理
 */
- (void)callSession:(RCSCallSession *)session
   networkTxQuality:(RCSCallQuality)txQuality
          rxQuality:(RCSCallQuality)rxQuality
       remoteUserID:(NSString *)userId;

/*!
 当前通话某用户声音音量回调，该回调方法每两秒触发一次
 
 @param session 当前通话会话
 @param leavel 声音级别: 0 ~ 9, 0为无声, 依次变大
 @param userId 用户ID, 本端用户ID(发送音量) 或 对端用户ID(接收音量)
 @discussion
 当前通话某用户声音音量回调，该回调方法每两秒触发一次
 
 @remarks 代理
 */
- (void)callSession:(RCSCallSession *)session
         audioLevel:(NSInteger)leavel
             userID:(NSString *)userId;

/*!
 本地发送视频数据上报
 
 @param session 当前通话会话
 @param sampleBuffer 本地发送视频数据
 @discussion
 当前视频通话本地视频数据, 同步返回处理后的同一 sampleBuffer 对象
 
 @remarks 代理
 @return 处理后的本地视频数据
 */
- (CMSampleBufferRef)callSession:(RCSCallSession *)session
               processVideoFrame:(CMSampleBufferRef)sampleBuffer;

/*!
 当前通话为视频通话时, 收到对端用户的第一个视频帧的回调
 
 @param session 当前通话会话
 @param userId 对端用户ID
 @remarks 代理
 */
- (void)callSession:(RCSCallSession *)session
receiveRemoteUserVideoFirstKeyFrame:(NSString *)userId;

/*!
 当前通话为音频或视频通话时, 收到对端用户的第一个音频帧的回调
 @param userId 对端用户ID
 
 @remarks 代理
 */
- (void)callSession:(RCSCallSession *)session
receiveRemoteUserVideoFirstAudioFrame:(NSString *)userId;

/*!
 对端用户视频分辨率变化的回调
 
 @param size 变化后的视频分辨率
 @param userId 用户ID
 @discussion
 对端用户视频分辨率变化的回调
 
 @remarks 代理
 */
- (void)callSession:(RCSCallSession *)session
remoteUserDidChangeResolution:(CGSize)size
             byUser:(NSString *)userId;

@end


@class RCMessagePushConfig;

@interface RCSCallSession : NSObject

#pragma mark - Property
/*!
 通话的会话类型
 */
@property (nonatomic, assign, readonly) RCSCallType callType;

/*!
 密聊类型
 */
@property (nonatomic, assign, readonly) RCSCallSecretChatType secretChatType;

/*!
 通话ID
 */
@property (nonatomic, strong, readonly) NSString *callId;

/*!
 通话的目标会话ID
 */
@property (nonatomic, strong, readonly) NSString *targetId;

/*!
 RTC会话唯一标识, 用于 Server API
 */
@property (nonatomic, strong, readonly) NSString *sessionId;

/*!
 是否是多方通话
 */
@property (nonatomic, assign, readonly, getter=isMultiCall) BOOL multiCall;

/*!
 通话的扩展信息
 */
@property (nonatomic, strong, readonly) NSString *extra;

/*!
 通话的当前状态
 */
@property (nonatomic, assign, readonly) RCSCallStatus callStatus;

/*!
 通话的最初发起人
 */
@property (nonatomic, strong, readonly) NSString *caller;

/*!
 邀请当前用户加入通话的邀请者
 */
@property (nonatomic, strong, readonly) NSString *inviter;

/*!
 当前的用户列表
 */
@property (nonatomic, strong, readonly) NSArray *userProfileList;

/*!
 自己的状态
 */
@property (nonatomic, strong, readonly) RCSCallUserProfile *myProfile;

/*!
 当前用户使用的媒体类型
 */
@property (nonatomic, assign, readonly) RCSCallMediaType mediaType;

/*!
 通话开始的时间
 
 @discussion 如果是用户呼出的通话，则startTime为通话呼出时间；如果是呼入的通话，则startTime为通话呼入时间。
 */
@property (nonatomic, assign, readonly) long long startTime;

/*!
 通话接通时间
 */
@property (nonatomic, assign, readonly) long long connectedTime;

/*!
 通话挂断原因
 */
@property (nonatomic, assign) RCSCallDisconnectReason disconnectReason;

/*!
 静音状态
 */
@property (nonatomic, readonly) BOOL isMuted;

/*!
 扬声器状态，是否开启扬声器
 
 @discussion 默认值为NO。
 */
@property (nonatomic, readonly) BOOL speakerEnabled;

/*!
 摄像头状态，是否开启摄像头
 */
@property (nonatomic, readonly) BOOL cameraEnabled;

#pragma mark - Init
- (instancetype)initWithProfile:(RCSCallProfile *)profile;

#pragma mark - Delegate
/*!
 添加通话状态的监听器, 支持多代理
 
 @param delegate 通话状态变化的监听器
 @discussion
 添加通话状态的监听器, 支持多代理
 
 @remarks 通话设置
 */
- (void)addDelegate:(id<RCSCallSessionDelegate>)delegate;

/*!
 删除通话状态的监听器
 
 @param delegate 通话状态的监听器
 @discussion
 删除通话状态的监听器
 
 @remarks 通话设置
 */
- (void)removeDelegate:(id<RCSCallSessionDelegate>)delegate;

/*!
 获取所有的监听器
 
 @discussion
 获取所有的监听器
 
 @remarks 通话设置
 @return 所有注册的代理数组
 */
- (NSArray<id<RCSCallSessionDelegate>> *)allDelegates;

#pragma mark - 待机时收到来电的处理
/*!
 待机时, 收到来电呼叫后, 接听来电
 
 @discussion
 待机时, 收到来电呼叫后, 接听来电
 
 @remarks 通话管理
 */
- (void)accept;

/*!
 待机时, 收到来电呼叫后, 接听来电
 
 @param completion 接听来电回调
 @discussion
 待机时, 收到来电呼叫后, 接听来电
 
 @remarks 通话管理
 */
- (void)accept:(void (^)(RCSCallStatusCode code))completion;

/*!
 待机时收到来电, 挂断通话
 
 @discussion
 待机时收到来电, 挂断通话
 
 @remarks 通话管理
 */
- (void)hangup;

/*!
 待机时收到来电, 挂断通话
 
 @param completion 挂断通话回调
 @discussion
 待机时收到来电, 挂断通话
 
 @remarks 通话管理
 */
- (void)hangup:(void (^)(RCSCallStatusCode code))completion;

#pragma mark - 群用户主动加入通话
/*!
 群用户退出群通话后, 再次直接加入该群通话
 
 @discussion
 群用户退出群通话后, 再次直接加入该群通话,
 如果该通话已结束则无法加入
 
 @remarks 通话管理
 */
- (void)joinCall;

#pragma mark - 通话中邀请新用户
/*!
 通话建立之后, 再邀请新用户加入通话
 @param userIdList 用户ID列表
 @discussion
 邀请用户加入通话
 
 @remarks 通话管理
 */
- (void)inviteRemoteUsers:(NSArray *)userIdList;

/*!
 通话建立之后, 再邀请新用户加入通话
 @param userIdList 用户ID列表
 @param targetId 目标会话ID, 单人呼叫时targetId为被叫端UserId, 多人呼叫时targetId为群组Id, 请不要填写主叫端UserId, 否则无法发起呼叫
 @discussion
 邀请用户加入通话
 
 @remarks 通话管理
 */
- (void)inviteRemoteUsers:(NSArray *)userIdList
                 targetId:(NSString *)targetId;

/*!
 通话建立之后, 再邀请新用户加入通话
 
 @param userIdList 用户ID列表
 @param observerIdList 需要以观察者身份加入房间的用户ID列表
 @param targetId 目标会话ID, 单人呼叫时targetId为被叫端UserId, 多人呼叫时targetId为群组Id, 请不要填写主叫端UserId, 否则无法发起呼叫
 @discussion
 邀请用户加入通话
 
 @remarks 通话管理
 */
- (void)inviteRemoteUsers:(NSArray *)userIdList
           observerIdList:(NSArray *)observerIdList
                 targetId:(NSString *)targetId;

#pragma mark - 设置视频View
/*!
 设置用户所在的视频View
 @param userId 用户ID（自己或他人）
 @param view 视频的View
 @discussion
 设置用户所在的视频View
 
 @remarks 视频配置
 */
- (void)setVideoView:(UIView *)view userId:(NSString *)userId;

/*!
 设置用户所在的视频View
 
 @param view userId 用户ID（自己或他人)
 @param userId 视频的View
 @param renderMode 视频显示模式
 @discussion
 设置用户所在的视频View
 
 @remarks 视频配置
 */
- (void)setVideoView:(UIView *)view
              userId:(NSString *)userId
          renderMode:(RCSCallRenderMode)renderMode;

#pragma mark - 变更通话类型
/*!
 仅供一对一通话使用,
 向对方请求变更当前通话媒体类型
 @param mediaType 媒体类型
 @discussion
 向对方请求变更当前通话媒体类型
 
 音频转视频:
 在收到同意的  回调后, 调用 changeMediaType: 方法切换为视频类型,
 否则不调用 changeMediaType: 方法切换媒体类型.
 
 视频转音频:
 在 requestChangeMediaType:completion: 方法的 completion 中,
 如果 succ 为 YES, 则调用 changeMediaType: 方法切换为音频类型
 如果 succ 为 NO, 则不调用 changeMediaType:
 
 @remarks 通话管理
 */
- (void)requestChangeMediaType:(RCSCallMediaType)mediaType
                    completion:(void(^)(BOOL succ))completion;

/*!
 取消之前发起的音频到视频通话的媒体类型变更
 @param mediaType 媒体类型
 @discussion
 取消之前发起的音频到视频通话的媒体类型变更
 
 @remarks 通话管理
 */
- (void)cancelRequestChangeMediaType:(RCSCallMediaType)mediaType
                          completion:(void(^)(BOOL succ))completion;

/*!
 仅供一对一通话使用,
 收到对方请求变更当前通话由音频转为视频时, 回应对方的请求
 @param mediaType 媒体类型
 @discussion
 收到对方请求变更当前通话由音频转为视频时, 回应对方的请求,
 只有在音频变更为视频时需要使用此方法回应对方,
 当回应为 YES 后, SDK 内部会将当前通话切换为视频,
 不需要自行调用 changeMediaType: 方法切换媒体类型
 
 @remarks 通话管理
 */
- (void)answerChangeMediaType:(RCSCallMediaType)mediaType
                       accept:(BOOL)accept
                   completion:(void(^)(BOOL succ))completion;

/*!
 仅供一对一通话使用,
 更换自己使用的媒体类型
 对方同意由音频变更为视频通话后, 调用此方法进行本地变更
 @param mediaType 媒体类型
 @discussion
 更换自己使用的媒体类型
 
 @remarks 通话管理
 */
- (void)changeMediaType:(RCSCallMediaType)mediaType;

#pragma mark - 麦克风/摄像头
/*!
 设置静音状态
 
 @param muted 是否静音, 默认值为NO
 @discussion
 设置静音状态
 
 @remarks 音频配置
 @return 是否设置成功
 */
- (BOOL)setMuted:(BOOL)muted;

/*!
 设置扬声器状态
 
 @param speakerEnabled  是否开启扬声器
 @discussion
 设置扬声器状态
 
 @remarks 音频配置
 @return 是否设置成功
 */
- (BOOL)setSpeakerEnabled:(BOOL)speakerEnabled;

/*!
 设置摄像头状态
 
 @param cameraEnabled  是否开启摄像头
 @discussion
 音频通话的默认值为NO, 视频通话的默认值为YES
 
 @remarks 视频配置
 @return 是否设置成功
 */
- (BOOL)setCameraEnabled:(BOOL)cameraEnabled;

/*!
 切换前后摄像头
 
 @discussion
 切换前后摄像头
 
 @remarks 视频配置
 */
- (BOOL)switchCameraMode;

#pragma mark - 观察者发布资源
/*!
 通话过程中, 观察者身份用户, 发布自己的音视频资源
 
 @discussion
 通话过程中, 观察者身份用户, 发布自己的音视频资源,
 发布结束会收到 callSession:localUserDidPublishStream: 代理回调
 
 @warning
 只有观察者用户可以发布音视频资源
 
 @remarks 资源管理
 */
- (void)publishMediaResource;

#pragma mark - 正常用户取消发布音视频资源
/*!
 通话过程中, 正常身份用户, 取消发布自己的音视频资源
 
 @discussion
 通话过程中, 正常身份用户, 取消发布自己的音视频资源
 
 @warning
 只有正常用户可以取消发布音视频资源
 
 @remarks 资源管理
 */
- (void)unpublishMediaResource;

#pragma mark - 订阅远端用户发布的指定音视频资源
/*!
 通话过程中, 任何身份用户, 订阅远端用户发布的指定音视频资源,
 此功能仅在 autoSubscribe 为 NO 时生效
 
 @discussion
 通话过程中, 任何身份用户, 订阅远端用户发布的指定音视频资源
 
 @remarks 资源管理
 */
- (void)subscribeRemoteUserStreams:(NSArray *)streamIdList;

#pragma mark - 取消订阅远端用户发布的指定音视频资源
/*!
 通话过程中, 任何身份用户, 取消订阅远端用户发布的指定音视频资源,
 此功能仅在 autoSubscribe 为 NO 时生效
 
 @discussion
 通话过程中, 任何身份用户, 取消订阅远端用户发布的指定音视频资源
 
 @remarks 资源管理
 */
- (void)unsubscribeRemoteUserStreams:(NSArray *)streamIdList;

@end

NS_ASSUME_NONNULL_END
