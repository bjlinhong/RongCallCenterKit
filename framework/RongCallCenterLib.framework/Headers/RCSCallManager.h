//
//  RCSCallManager.h
//  RongCallCenterLib
//
//  Created by RongCloud on 2021/12/16.
//  Copyright © 2021 RongCloud. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>
#import <RongIMLibCore/RongIMLibCore.h>
#import "RCSCallProfile.h"
#import "RCSCallSignalSyncCallLog.h"

NS_ASSUME_NONNULL_BEGIN

/*!
 CallLib全局通话呼入的监听器
 */
@protocol RCSCallReceiveDelegate <NSObject>

/*!
 接收到通话呼入的回调
 
 @param callSession 呼入的通话实体
 @discussion
 接收到通话呼入的回调
 
 @remarks 代理
 */
- (void)didReceiveCall:(RCSCallSession *)callSession;

/*!
 接收到通话呼入的远程通知的回调
 
 @param callId 呼入通话的唯一值
 @param inviterUserId 通话邀请者的UserId
 @param mediaType 通话的媒体类型
 @param userIdList 被邀请者的UserId列表
 @param userDict 远程推送包含的其他扩展信息
 @param isVoIPPush 是否 VoIP 推送
 @param pushConfig 推送配置

 @discussion
 接收到通话呼入的远程通知的回调
 
 @remarks 代理
 */
- (void)didReceiveCallRemoteNotification:(NSString *)callId
                           inviterUserId:(NSString *)inviterUserId
                               mediaType:(RCSCallMediaType)mediaType
                              userIdList:(NSArray *)userIdList
                                userDict:(NSDictionary *)userDict
                              isVoIPPush:(BOOL)isVoIPPush
                              pushConfig:(RCMessagePushConfig *)pushConfig;

/*!
 接收到取消通话的远程通知的回调
 
 @param callId 呼入通话的唯一值
 @param inviterUserId 通话邀请者的UserId
 @param mediaType 通话的媒体类型
 @param userIdList 被邀请者的UserId列表
 @param pushConfig 推送配置
 @param isRemoteCancel 是否为远端取消呼叫
 @discussion
 接收到取消通话的远程通知的回调
 
 @remarks 代理
 */
- (void)didCancelCallRemoteNotification:(NSString *)callId
                          inviterUserId:(NSString *)inviterUserId
                              mediaType:(RCSCallMediaType)mediaType
                             userIdList:(NSArray *)userIdList
                             pushConfig:(RCMessagePushConfig *)pushConfig
                         isRemoteCancel:(BOOL)isRemoteCancel;

@end


@interface RCSCallManager : NSObject

/*!
 自定义push消息. 应在startCall之前调用
 */
@property (nonatomic, strong) RCMessagePushConfig *invitePushConfig;

/*!
 自定义push消息. 应在startCall之前调用
 */
@property (nonatomic, strong) RCMessagePushConfig *hangupPushConfig;

/*!
 呼叫或通话过程中, 当收到手机号系统来电时, 是否挂断当前呼叫或通话, 默认: Yes 挂断
*/
@property (nonatomic, assign) BOOL canHangupSystemPhonecall;

/*!
 多端登录时, 同一UserId的两端加入不同通话的类型, 默认: RCRTCJoinRoomTypeCoexist 多端可共存不同通话
 */
@property (nonatomic, assign) RCSCallJoinCallType joinCallType;

/*!
 被叫端接收来电代理
 */
@property (nonatomic, weak) id<RCSCallReceiveDelegate> callReceiveDelegate;

/*!
 是否生成通话记录消息，默认值: YES
 */
@property (nonatomic, assign) BOOL enableCallSummary;

/*!
 是否开启多平台模式，默认值: NO
 */
@property (nonatomic, assign) BOOL enableMultiPlatformMode;

/*!
 收到对端发布资源后后, 是否自动订阅, 默认值: Yes
 */
@property (nonatomic, assign) BOOL autoSubscribe;

#pragma mark - sharedManager
/*!
 RCSCallManager 单例
 */
+ (instancetype)sharedManager;

#pragma mark - 发起呼叫
/*!
 发起一个通话
 
 @param callType 会话类型
 @param secretChatType 密聊类型
 @param targetId 目标会话ID, 单人呼叫时targetId为被叫端UserId, 多人呼叫时targetId为群组Id, 请不要填写主叫端UserId, 否则无法发起呼叫
 @param userIdList 被叫端的用户ID列表, 数组中仅填写被叫端UserId, 请不要填写主叫端UserId, 否则无法发起呼叫
 @param type 发起的通话媒体类型
 @param delegate 通话监听
 @param extra 附件信息
 @discussion
 发起一个通话
 
 @remarks 通话管理
 @return 呼出的通话实体
 */
- (RCSCallSession *)startCall:(RCSCallType)callType
               secretChatType:(RCSCallSecretChatType)secretChatType
                     targetId:(NSString *)targetId
                           to:(NSArray *)userIdList
                    mediaType:(RCSCallMediaType)type
              sessionDelegate:(id<RCSCallSessionDelegate>)delegate
                        extra:(NSString *)extra;

/*!
 发起一个通话
 
 @param callType 会话类型
 @param secretChatType 密聊类型
 @param targetId 目标会话ID, 单人呼叫时targetId为被叫端UserId, 多人呼叫时targetId为群组Id
 @param userIdList 被叫端的用户ID列表, 数组中仅填写被叫端UserId, 请不要填写主叫端UserId, 否则无法发起呼叫
 @param observerIdList 主叫端指定需要以观察者身份加入房间的用户ID列表, 如果主叫端需要以观察者身份加入房间也需要填写主叫端UserId
 @param type 发起的通话媒体类型
 @param delegate 通话监听
 @param extra 附件信息
 @discussion
 发起一个通话
 
 @remarks 通话管理
 @return 呼出的通话实体
 */
- (RCSCallSession *)startCall:(RCSCallType)callType
               secretChatType:(RCSCallSecretChatType)secretChatType
                     targetId:(NSString *)targetId
                           to:(NSArray *)userIdList
               observerIdList:(NSArray *)observerIdList
                    mediaType:(RCSCallMediaType)type
              sessionDelegate:(id<RCSCallSessionDelegate>)delegate
                        extra:(NSString *)extra;

#pragma mark - 查询通话能力
/*!
 查询当前会话类型是否支持音频通话
 @param conversationType 会话类型
 @return 是否支持音频通话
 */
- (BOOL)isAudioCallEnabled:(RCConversationType)conversationType;

/*!
 查询当前会话类型是否支持视频通话
 @param conversationType 会话类型
 @return 是否支持视频通话
 */
- (BOOL)isVideoCallEnabled:(RCConversationType)conversationType;

#pragma mark - 参数设置
/*!
 设置本地视频分辨率
 
 @param profile 通话视频分辨率
 @discussion
 设置本地视频分辨率
 
 @remarks 视频设置
 */
- (void)setVideoProfile:(RCSVideoProfile)profile;

/*!
 设置本地视频属性, 可用此接口设置本地视频方向
 
 @param orientation 视频方向, 默认: AVCaptureVideoOrientationPortrait
 @discussion
 设置本地视频属性, 可用此接口设置本地视频方向
 
 @remarks 视频设置
 */
- (void)setVideoOrientation:(AVCaptureVideoOrientation)orientation;

/*!
 设置码率
 
 @param rate 码率 单位 kbps
 @discussion
 设置码率
 
 @remarks 视频设置
 */
- (void)setBitRate:(NSUInteger)rate;

/*!
 设置是否使用苹果 PushKit 推送
 
 @param enable YES 使用, NO 不使用
 @discussion
 是否打开苹果 PushKit 推送, 该推送可以直接激活 App, 注: iOS 13 以后 PushKit 必须结合苹果 CallKit.framework 进行使用, 否则无法正常处理 VoIP 相关推送逻辑,
 如果设置为 NO 则使用普通 APNS 消息推送来处理音视频信令逻辑, 默认关闭. 打开之后 App 默认需要自行处理 VoIP 推送唤起 CallKit.framework 的逻辑.
 
 @remarks 通话设置
 */
- (void)setApplePushKitEnable:(BOOL)enable;

#pragma mark - 获取当前存在的通话会话
/*!
 通过 CallId 从服务查询目前存在的通话
 
 @param callId 通话ID
 @discussion
 通过 CallId 从服务查询目前存在的通话,
 用于通过 RCSCallSession 中的 joinCall: 方法直接加入多人通话
 */
- (void)getCallSession:(NSString *)callId
            completion:(void (^)(RCSCallSession *callSession))completion;

#pragma mark - 通话记录
/*!
 通过时间戳获取之后通话记录
 
 @param timeInterval 时间戳, 单位: 秒
 @discussion
 通过时间戳获取之后通话记录
 */
- (void)getCallLog:(NSTimeInterval)timeInterval
        completion:(void (^)(RCSCallSignalSyncCallLog *calllog))completion;

/*!
 通过 CallId 列表删除指定通话记录
 
 @param callIdArray 通话ID列表
 @discussion
 通过 CallId 列表删除指定通话记录
 */
- (void)deleteCallLog:(NSArray <NSString *> *)callIdArray
           completion:(void (^)(RCSCallStatusCode code))completion;


#pragma mark - 版本号
/*!
 获取版本号
 */
- (NSString *)getVersion;

@end

NS_ASSUME_NONNULL_END
