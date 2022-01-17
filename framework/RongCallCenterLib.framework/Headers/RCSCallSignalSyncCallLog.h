//
//  RCSCallSignalCallLogModel.h
//  RongCallCenterLib
//
//  Created by RongCloud on 2021/12/16.
//  Copyright © 2021 RongCloud. All rights reserved.
//

#import "RCSCallDefine.h"
#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface RCSCallSignalCallLogModel : NSObject

@property (nonatomic, copy) NSString *callId;
@property (nonatomic, assign) RCSCallType callType;
@property (nonatomic, assign) NSInteger callDir;
@property (nonatomic, assign) RCSCallMediaType mediaType;
@property (nonatomic, assign) RCSCallSecretChatType secretChat;
@property (nonatomic, copy) NSString *userId;
@property (nonatomic, copy) NSString *callingUserId;
@property (nonatomic, copy) NSArray<NSString *> *callUserIds;
@property (nonatomic, assign) NSInteger callStatus;
@property (nonatomic, assign) NSInteger sessionStatus;
@property (nonatomic, assign) long long startTime;
@property (nonatomic, assign) long long endTime;
@property (nonatomic, assign) long long duration;
@property (nonatomic, assign) long long syncTime;

@end


@interface RCSCallSignalSyncDelLogListModel : NSObject

@property (nonatomic, copy) NSString *callId;
@property (nonatomic, copy) NSString *userId;

@end


@interface RCSCallSignalSyncCallLog : NSObject

@property (nonatomic, copy) NSArray <RCSCallSignalCallLogModel *> *callLogList;
@property (nonatomic, copy) NSArray <RCSCallSignalSyncDelLogListModel *> *delLogList;
@property (nonatomic, assign) long long maxTime;
@property (nonatomic, assign) NSInteger hasMore;

@end

NS_ASSUME_NONNULL_END
