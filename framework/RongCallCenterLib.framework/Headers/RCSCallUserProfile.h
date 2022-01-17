//
//  RCSCallUserProfile.h
//  RongCallCenterLib
//
//  Created by RongCloud on 2021/12/16.
//  Copyright © 2021 RongCloud. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import "RCSCallDefine.h"

NS_ASSUME_NONNULL_BEGIN


@interface RCSCallUserProfile : NSObject

/*!
 用户ID
 */
@property (nonatomic, strong) NSString *userId;

/*!
 用户所使用的媒体类型
 */
@property (nonatomic, assign) RCSCallMediaType mediaType;

/*!
 用户的通话状态
 */
@property (nonatomic, assign) RCSCallStatus callStatus;

/*!
 用户的摄像头是否已经关闭
 */
@property (nonatomic, assign) BOOL cameraDisabled;

/*!
 用户的麦克风是否已经关闭
 */
@property (nonatomic, assign) BOOL micDisabled;

/*!
 收到通话呼入的时间
 
 @discussion 只有当前登录用户的呼入时间是准确的，其他用户的并不准确
 */
@property (nonatomic, assign) long long startTime;

/*!
 通话的接通时间
 */
@property (nonatomic, assign) long long connectedTime;

/*!
 通话的挂断时间
 */
@property (nonatomic, assign) long long endTime;

/*!
 用户的视频View
 */
@property (nonatomic, strong) UIView *videoView;

/*!
 用户类型
 */
@property (nonatomic, assign) RCSCallUserType userType;

/*!
 对端用户发布的音视频资源
 */
@property (nonatomic, strong) NSMutableArray *avResourceArray;

/*!
 用户标签
*/
@property (nonatomic, strong) NSString *tag;

/*!
 初始化用户的通话状态
 
 @param userId     用户ID
 @param mediaType  用户所使用的媒体类型
 @param callStatus 用户的通话状态
 @param startTime  收到通话呼入的时间
 
 @return 用户的通话状态对象
 */
- (instancetype)initWithUserId:(NSString *)userId
                     mediaType:(RCSCallMediaType)mediaType
                    callStatus:(RCSCallStatus)callStatus
                     startTime:(long long)startTime;

@end

NS_ASSUME_NONNULL_END
