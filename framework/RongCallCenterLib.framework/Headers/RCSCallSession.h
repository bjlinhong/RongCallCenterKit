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

NS_ASSUME_NONNULL_BEGIN


/*!
 通话事件的代理回调
 */
@protocol RCSCallSessionDelegate <NSObject>

@optional

/*!
 通话已发起
 */
- (void)callDidStarted;

/*!
 通话已接通
 
 @discussion
 通话已接通
 
 @remarks 代理
 */
- (void)callDidConnect;

/*!
 通话已结束
 
 @discussion
 通话已结束
 
 @remarks 代理
 */
- (void)callDidDisconnect;

/*!
 对端用户正在振铃
 
 @param userId 正在振铃的用户ID
 @discussion
 对端用户正在振铃
 
 @remarks 代理
 */
- (void)remoteUserDidRing:(NSString *)userId;

/*!
 有用户被邀请加入通话
 
 @param userId 被邀请的用户ID
 @param mediaType 希望被邀请者使用的媒体类型
 @discussion
 有用户被邀请加入通话
 
 @remarks 代理
 */
- (void)remoteUserDidInvite:(NSString *)userId mediaType:(RCSCallMediaType)mediaType;

/*!
 对端用户加入了通话
 
 @param userId 用户ID
 @param mediaType 用户的媒体类型
 @discussion
 对端用户加入了通话
 
 @remarks 代理
 */
- (void)remoteUserDidJoin:(NSString *)userId mediaType:(RCSCallMediaType)mediaType;

/*!
 对端用户切换了媒体类型
 
 @param userId 用户ID
 @param mediaType 切换至的媒体类型
 @discussion
 对端用户切换了媒体类型
 
 @remarks 代理
 */
- (void)remoteUserDidChangeMediaType:(NSString *)userId mediaType:(RCSCallMediaType)mediaType;

/*!
 对端用户开启或关闭了麦克风的状态
 
 @param disabled 是否关闭麦克风
 @param userId 用户ID
 @discussion
 对端用户开启或关闭了麦克风的状态
 
 @remarks 代理
 */
- (void)remoteUserDidDisableMicrophone:(BOOL)disabled byUser:(NSString *)userId;

/*!
 对端用户开启或关闭了摄像头的状态
 
 @param disabled 是否关闭摄像头
 @param userId 用户ID
 @discussion
 对端用户开启或关闭了摄像头的状态
 
 @remarks 代理
 */
- (void)remoteUserDidDisableCamera:(BOOL)disabled byUser:(NSString *)userId;

/*!
 对端用户挂断
 
 @param userId 用户ID
 @param reason 挂断的原因
 @discussion
 对端用户挂断
 
 @remarks 代理
 */
- (void)remoteUserDidLeft:(NSString *)userId reason:(RCSCallDisconnectReason)reason;

/*!
 对端用户发布自定义媒体流
 
 @param streamId 自定义流ID
 @param tag 自定义流标签
 @param mediaType 媒体类型
 @discussion
 对端用户发布自定义媒体流
 
 @remarks 代理
 */
- (void)remoteUserDidPublishCustomMediaStream:(NSString *)streamId
                                    streamTag:(NSString *)tag
                                    mediaType:(RCSCallMediaType)mediaType;

/*!
 对端用户取消发布自定义媒体流
 
 @param streamId 自定义流ID
 @discussion
 对端用户取消发布自定义媒体流
 
 @remarks 代理
 */
- (void)remoteUserDidUnpublishCustomMediaStream:(NSString *)streamId;

/*!
 本地发布资源上报
 
 @param success 发布资源是否成功
 @discussion
 本地发布资源上报
 
 @remarks 代理
 */
- (void)localUserDidPublishStream:(BOOL)success;

/*!
 主叫端发起呼叫后, 播放等待音
 
 @discussion
 主叫端发起呼叫后, 播放等待音
 
 @remarks 代理
 */
- (void)shouldAlertForWaitingRemoteResponse;

/*!
 被叫端收到来电后, 播放来电铃声
 
 @discussion
 被叫端收到来电后, 播放来电铃声
 
 @remarks 代理
 */
- (void)shouldRingForIncomingCall;

/*!
 通话接通或挂断时停止播放铃声
 
 @discussion
 通话接通或挂断时停止播放铃声
 
 @remarks 代理
 */
- (void)shouldStopAlertAndRing;

/*!
 通话过程中的错误回调
 
 @param error 错误码
 @discussion
 通话过程中的错误回调
 
 @warning
 这个接口回调的错误码主要是为了提供必要的log以及提示用户,
 如果是不可恢复的错误，SDK会挂断电话并回调callDidDisconnect,
 App可以在callDidDisconnect中统一处理通话结束的逻辑
 
 @remarks 代理
 */
- (void)errorDidOccur:(RCSCallStatusCode)error;

/*!
 当前通话网络状态的回调，该回调方法每秒触发一次
 
 @param txQuality 上行网络质量
 @param rxQuality 下行网络质量, 接收到的所有远端用户网络质量的平均值
 @discussion
 当前通话网络状态的回调，该回调方法每秒触发一次
 
 @remarks 代理
 */
- (void)networkTxQuality:(RCSCallQuality)txQuality rxQuality:(RCSCallQuality)rxQuality;

/*!
 当前通话网络状态的回调，该回调方法每秒触发一次
 
 @param txQuality 上行网络质量
 @param rxQuality 下行网络质量, 接收到的某个远端用户的网络质量
 @param userId 远端用户
 @discussion
 当前通话网络状态的回调，该回调方法每秒触发一次
 
 @remarks 代理
 */
- (void)networkTxQuality:(RCSCallQuality)txQuality rxQuality:(RCSCallQuality)rxQuality remoteUserID:(NSString *)userId;

/*!
 当前通话某用户声音音量回调，该回调方法每两秒触发一次
 
 @param leavel 声音级别: 0 ~ 9, 0为无声, 依次变大
 @param userId 用户ID, 本端用户ID(发送音量) 或 远端用户ID(接收音量)
 @discussion
 当前通话某用户声音音量回调，该回调方法每两秒触发一次
 
 @remarks 代理
 */
- (void)audioLevel:(NSInteger)leavel userID:(NSString *)userId;

/*!
 本地发送视频数据上报
 
 @param sampleBuffer 本地发送视频数据
 @discussion
 当前视频通话本地视频数据, 同步返回处理后的同一 sampleBuffer 对象
 
 @remarks 代理
 @return 处理后的本地视频数据
 */
- (CMSampleBufferRef)processVideoFrame:(CMSampleBufferRef)sampleBuffer;

/*!
 当前通话为视频通话时, 收到远端用户的第一个视频帧的回调
 @param userId 远端用户ID
 
 @remarks 代理
 */
- (void)receiveRemoteUserVideoFirstKeyFrame:(NSString *)userId;

/*!
 当前通话为音频或视频通话时, 收到远端用户的第一个音频帧的回调
 @param userId 远端用户ID
 
 @remarks 代理
 */
- (void)receiveRemoteUserVideoFirstAudioFrame:(NSString *)userId;

/*!
 对端用户视频分辨率变化的回调
 
 @param size 变化后的视频分辨率
 @param userId 用户ID
 @discussion
 对端用户视频分辨率变化的回调
 
 @remarks 代理
 */
- (void)remoteUserDidChangeResolution:(CGSize)size byUser:(NSString *)userId;

@end


@class RCMessagePushConfig;

@interface RCSCallSession : NSObject

#pragma mark - Property
/*!
 通话的会话类型
 */
@property (nonatomic, assign, readonly) RCConversationType conversationType;

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
 待机时收到来电, 接听来电
 
 @param type 接听使用的媒体类型
 @discussion
 待机时收到来电, 接听来电
 
 @remarks 通话管理
 */
- (void)accept:(RCSCallMediaType)type;

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
- (void)hangup:(void (^)(BOOL isSuccess, NSInteger code))completion;

#pragma mark - 通话中收到另一来电的处理
/*!
 通话中收到另一来电, 挂断另一来电
 
 @param incomingCallId 另一来电ID
 @discussion
 通话中收到另一来电, 挂断另一来电
 
 @remarks 通话管理
 */
- (void)hangupIncoming:(NSString *)incomingCallId;

/*!
 通话中收到另一来电, 挂断正在进行的通话, 接听另一来电
 
 @param currentCallId 正在进行的通话ID
 @param incomingCallId 另一来电ID
 @discussion
 通话中收到另一来电, 挂断正在进行的通话, 接听另一来电
 
 @remarks 通话管理
 */
- (void)hangupCurrent:(NSString *)currentCallId acceptIncoming:(NSString *)incomingCallId;

/*!
 通话中收到另一来电, 保持正在进行的通话, 接听另一来电
 
 @param currentCallId 正在进行的通话ID
 @param incomingCallId 另一来电ID
 @discussion
 通话中收到另一来电, 保持正在进行的通话, 接听另一来电
 
 @remarks 通话管理
 */
- (void)holdCurrent:(NSString *)currentCallId acceptIncoming:(NSString *)incomingCallId;

#pragma mark - 另一个通话结束后处理
/*!
 接听的另一来电结束后, 重新接听之前保持的通话
 
 @param holdingCallId 之前保持的通话ID
 @discussion
 接听的另一来电结束后, 重新接听之前保持的通话
 
 @remarks 通话管理
 */
- (void)acceptHolding:(NSString *)holdingCallId;

#pragma mark - 群用户加入通话
/*!
 群用户直接加入已经存在的通话
 
 @param callId 已经存在的通话ID
 @discussion
 群用户直接加入已经存在的通话
 
 @remarks 通话管理
 */
- (void)joinCall:(NSString *)callId;

#pragma mark - 通话中邀请新用户
/*!
 通话建立之后, 再邀请新用户加入通话
 @param userIdList 用户ID列表
 @param type 建议被邀请者使用的媒体类型
 @discussion
 邀请用户加入通话
 
 @remarks 通话管理
 */
- (void)inviteRemoteUsers:(NSArray *)userIdList mediaType:(RCSCallMediaType)type;

/*!
 邀请用户加入通话
 
 @param userIdList 用户ID列表
 @param observerIdList 需要以观察者身份加入房间的用户ID列表
 @param type 建议被邀请者使用的媒体类型
 @discussion
 邀请用户加入通话
 
 @remarks 通话管理
 */
- (void)inviteRemoteUsers:(NSArray *)userIdList
           observerIdList:(NSArray *)observerIdList
                mediaType:(RCSCallMediaType)type;

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
- (void)setVideoView:(UIView *)view userId:(NSString *)userId renderMode:(RCSCallRenderMode)renderMode;

/*!
 更换自己使用的媒体类型
 @param type 媒体类型
 @discussion
 更换自己使用的媒体类型
 
 @remarks 通话管理
 */
- (BOOL)changeMediaType:(RCSCallMediaType)type;

#pragma mark - Device
/*!
 静音状态
 */
@property (nonatomic, readonly) BOOL isMuted;

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
 扬声器状态，是否开启扬声器
 
 @discussion 默认值为NO。
 */
@property (nonatomic, readonly) BOOL speakerEnabled;

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
 摄像头状态，是否开启摄像头
 */
@property (nonatomic, readonly) BOOL cameraEnabled;

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
 观察者身份用户, 发布自己的音视频资源
 
 @param mediaType 发布的媒体类型
 @param block 发布完成的回调
 @discussion
 观察者身份用户, 发布自己的音视频资源
 
 @warning
 只有观察者用户可以发布音视频资源
 
 @remarks 资源管理
 */
- (void)publishMediaResource:(RCSCallMediaType)mediaType
                    complete:(void (^)(BOOL isSuccess, NSInteger code))block;

#pragma mark - 正常用户取消发布资源
/*!
 正常身份用户, 取消发布自己的音视频资源
 
 @param block  取消发布完成的回调
 @discussion
 正常身份用户, 取消发布自己的音视频资源
 
 @warning
 只有正常用户可以取消发布音视频资源
 
 @remarks 资源管理
 */
- (void)unPublishMediaResource:(void (^)(BOOL isSuccess, NSInteger code))block;

@end

NS_ASSUME_NONNULL_END
