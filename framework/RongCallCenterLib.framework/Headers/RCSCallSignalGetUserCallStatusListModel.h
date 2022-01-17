//
//  RCSCallSignalGetUserCallStatusListModel.h
//  RongCallCenterLib
//
//  Created by RongCloud on 2021/12/16.
//  Copyright © 2021 RongCloud. All rights reserved.
//

#import "RCSCallDefine.h"
#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface RCSCallSignalGetUserCallStatusListModel : NSObject

@property (nonatomic, copy) NSString *callId;
@property (nonatomic, assign) NSInteger callStatus;
@property (nonatomic, assign) long long syncTime;

@end


NS_ASSUME_NONNULL_END
