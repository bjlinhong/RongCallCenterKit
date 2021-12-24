//
//  RCCallKitExtensionModule.m
//  RongCallKit
//
//  Created by RongCloud on 16/7/2.
//  Copyright © 2016年 Rong Cloud. All rights reserved.
//

#import "RCCallKitExtensionModule.h"
#import "RCCall.h"
#import "RCCallDetailMessageCell.h"
#import "RCCallKitUtility.h"
#import "RCCallTipMessageCell.h"

@implementation RCCallKitExtensionModule

/*!
 CallKit 插件生成方法

 @warning IMKit会通过这个方法生成并加载CallKit，强烈建议不要删除其中的逻辑，否则肯能导致CallKit功能不正常。
 */
+ (instancetype)loadRongExtensionModule {
    [RCCall sharedRCCall];
    return [[RCCallKitExtensionModule alloc] init];
}

- (void)destroyModule {
}

- (NSArray<RCExtensionMessageCellInfo *> *)getMessageCellInfoList:(RCConversationType)conversationType
                                                         targetId:(NSString *)targetId {
    if (conversationType == ConversationType_PRIVATE) {
        RCExtensionMessageCellInfo *cellInfo = [RCExtensionMessageCellInfo new];
        cellInfo.messageContentClass = [RCSCallSummaryMessage class];
        cellInfo.messageCellClass = [RCCallDetailMessageCell class];
        return @[ cellInfo ];
    } else if (conversationType == ConversationType_GROUP || conversationType == ConversationType_DISCUSSION) {
        RCExtensionMessageCellInfo *cellInfo = [RCExtensionMessageCellInfo new];
        cellInfo.messageContentClass = [RCSCallSummaryMessage class];
        cellInfo.messageCellClass = [RCCallTipMessageCell class];
        return @[ cellInfo ];
    }
    return nil;
}

- (void)didTapMessageCell:(RCMessageModel *)messageModel {
    if ([messageModel.content isKindOfClass:[RCSCallSummaryMessage class]]) {
        RCSCallSummaryMessage *callMessage = (RCSCallSummaryMessage *)messageModel.content;
        //  其他端接听显示的消息不可点击
        if (messageModel.conversationType == ConversationType_PRIVATE &&
            callMessage.hangupReason != RCSCallDisconnectReasonAcceptByOtherClient) {
            [[RCCall sharedRCCall] startSingleCall:messageModel.targetId mediaType:callMessage.mediaType];
        }
    }
}

- (NSArray<RCExtensionPluginItemInfo *> *)getPluginBoardItemInfoList:(RCConversationType)conversationType
                                                            targetId:(NSString *)targetId {
    NSMutableArray *itemList = [[NSMutableArray alloc] init];
    if ([targetId isEqualToString:[RCIMClient sharedRCIMClient].currentUserInfo.userId]) {
        return itemList;
    }

    if ([[RCCall sharedRCCall] isAudioCallEnabled:conversationType]) {
        RCExtensionPluginItemInfo *audioItem = [[RCExtensionPluginItemInfo alloc] init];

        audioItem.normalImage = [RCCallKitUtility imageFromVoIPBundle:@"voip/plugin_item_audio.png"];
        audioItem.highlightedImage = [RCCallKitUtility imageFromVoIPBundle:@"voip/plugin_item_audio_highlighted.png"];
        audioItem.title = RCCallKitLocalizedString(@"VoIPAudioCall");

        if (conversationType == ConversationType_PRIVATE) {
            audioItem.tapBlock = ^(RCChatSessionInputBarControl *chatSessionInputBar) {
                [[RCCall sharedRCCall] startSingleCall:targetId mediaType:RCSCallMediaTypeAudio];
            };
        } else if (conversationType == ConversationType_GROUP || conversationType == ConversationType_DISCUSSION) {
            audioItem.tapBlock = ^(RCChatSessionInputBarControl *chatSessionInputBar) {
                [[RCCall sharedRCCall] startMultiCall:conversationType targetId:targetId mediaType:RCSCallMediaTypeAudio];
            };
        }
        audioItem.tag = PLUGIN_BOARD_ITEM_VOIP_TAG;
        [itemList addObject:audioItem];
    }
    if ([[RCCall sharedRCCall] isVideoCallEnabled:conversationType]) {
        RCExtensionPluginItemInfo *videoItem = [[RCExtensionPluginItemInfo alloc] init];

        videoItem.normalImage = [RCCallKitUtility imageFromVoIPBundle:@"voip/plugin_item_video.png"];
        videoItem.highlightedImage = [RCCallKitUtility imageFromVoIPBundle:@"voip/plugin_item_video_highlighted.png"];
        videoItem.title = RCCallKitLocalizedString(@"VoIPVideoCall");

        videoItem.tapBlock = ^(RCChatSessionInputBarControl *chatSessionInputBar) {
            [[RCCall sharedRCCall] startSingleCall:targetId mediaType:RCSCallMediaTypeVideo];
        };
        if (conversationType == ConversationType_PRIVATE) {
            videoItem.tapBlock = ^(RCChatSessionInputBarControl *chatSessionInputBar) {
                [[RCCall sharedRCCall] startSingleCall:targetId mediaType:RCSCallMediaTypeVideo];
            };
        } else if (conversationType == ConversationType_GROUP || conversationType == ConversationType_DISCUSSION) {
            videoItem.tapBlock = ^(RCChatSessionInputBarControl *chatSessionInputBar) {
                [[RCCall sharedRCCall] startMultiCall:conversationType targetId:targetId mediaType:RCSCallMediaTypeVideo];
            };
        }
        videoItem.tag = PLUGIN_BOARD_ITEM_VIDEO_VOIP_TAG;
        [itemList addObject:videoItem];
    }
    return itemList;
}

- (void)onMessageReceived:(RCMessage *)message {
}

- (BOOL)handleAlertForMessageReceived:(RCMessage *)message {
    if ([message.content isKindOfClass:[RCSCallSummaryMessage class]]) {
        return YES;
    } else if ([RCCall sharedRCCall].currentCallSession.callStatus == RCSCallStatusConnected) {
        return YES;
    } else {
        return NO;
    }
}

- (BOOL)handleNotificationForMessageReceived:(RCMessage *)message
                                        from:(NSString *)fromName
                                    userInfo:(NSDictionary *)userInfo {
    if ([message.content isKindOfClass:[RCSCallSummaryMessage class]]) {
        // Todo post call end notification here?
        return YES;
    } else {
        return NO;
    }
}

- (BOOL)isAudioHolding {
    if ([RCCall sharedRCCall].currentCallSession) {
        return YES;
    } else {
        return NO;
    }
}

- (BOOL)isCameraHolding {
    if ([RCCall sharedRCCall].currentCallSession &&
        [RCCall sharedRCCall].currentCallSession.mediaType == RCSCallMediaTypeVideo) {
        return YES;
    } else {
        return NO;
    }
}

@end
