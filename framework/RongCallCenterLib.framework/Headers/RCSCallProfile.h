//
//  RCSCallProfile.h
//  RongCallCenterLib
//
//  Created by RongCloud on 2021/12/16.
//  Copyright © 2021 RongCloud. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "RCSCallSession.h"
#import "RCSCallUserProfile.h"

NS_ASSUME_NONNULL_BEGIN


@interface RCSCallProfile : NSObject

@property (nonatomic, strong) NSString *callId;
@property (nonatomic, strong) NSString *userName;
@property (nonatomic, assign) RCConversationType conversationType;
@property (nonatomic, strong) NSString *targetId;
@property (nonatomic, weak) RCSCallSession *callSession;
@property (nonatomic, strong) NSString *sessionId;
// call info
@property (nonatomic, strong) NSString *caller;
@property (nonatomic, strong) NSString *inviter;
@property (nonatomic, assign) long long sentTime;
@property (nonatomic, strong) NSString *extra;
@property (nonatomic, assign) RCSCallDisconnectReason hangupReason;
@property (nonatomic, assign, getter=isUseSignalServer) BOOL useSignalServer;

// RCSCallUserProfile List (not include self)
@property (nonatomic, strong) NSMutableArray *memberProfileList;
@property (nonatomic, strong) NSMutableArray *unkownMediaIdList;

// self info
@property (nonatomic, strong) RCSCallUserProfile *myProfile;
@property (nonatomic, assign) BOOL isOfflineCall;
@property (nonatomic, assign) BOOL muted;
@property (nonatomic, assign) BOOL speakerEnabled;
@property (nonatomic, assign) BOOL cameraEnabled;
@property (nonatomic, assign) NSUInteger initialMemberCount;


- (instancetype)initWithCallId:(NSString *)callId
              conversationType:(RCConversationType)conversationType
                      targetId:(NSString *_Nullable)targetId
                        caller:(NSString *)caller
                       inviter:(NSString *)inviter
                     myProfile:(RCSCallUserProfile *)myProfile
               useSignalServer:(BOOL)useSignalServer
                observerIdList:(NSMutableArray *_Nullable)observerIdList
                         extra:(NSString *_Nullable)extra;

- (void)addDelegate:(id<RCSCallSessionDelegate>)delegate;
- (void)removeDelegate:(id<RCSCallSessionDelegate>)delegate;
- (NSArray<id<RCSCallSessionDelegate>> *)allDelegates;
- (RCSCallSession *)generateCallSession;
- (NSArray *)getMemberUserIdList;
- (RCSCallUserProfile *)getUserProfileInMemberProfileList:(NSString *)userId;

@end

NS_ASSUME_NONNULL_END
