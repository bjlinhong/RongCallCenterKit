//
//  RCCallAudioMultiCallViewController.m
//  RongCallKit
//
//  Created by RongCloud on 16/3/18.
//  Copyright © 2016年 RongCloud. All rights reserved.
//

#import "RCCallAudioMultiCallViewController.h"
#import "RCAudioMultiCallUserCollectionLayout.h"
#import "RCCXCall.h"
#import "RCCall.h"
#import "RCCallFloatingBoard.h"
#import "RCCallKitUtility.h"
#import "RCCallMultiCallUserCell.h"
#import "RCCallSelectMemberViewController.h"
#import "RCCallUserCallInfoModel.h"
#import "RCUserInfoCacheManager.h"
#import "RCloudImageView.h"

#define currentUserId ([RCIMClient sharedRCIMClient].currentUserInfo.userId)
#define RongVoIPMultiAudioCellReuseID @"RongVoIPMultiAudioCellReuseID"

@interface RCCallAudioMultiCallViewController () <UICollectionViewDataSource>

@property (nonatomic, strong) RCCallUserCallInfoModel *mainModel;
@property (nonatomic, strong) NSMutableArray *subUserModelList;

@end

@implementation RCCallAudioMultiCallViewController

- (instancetype)initWithIncomingCall:(RCSCallSession *)callSession {
    return [super initWithIncomingCall:callSession];
}

- (instancetype)initWithOutgoingCall:(RCSCallType)callType
                            targetId:(NSString *)targetId
                          userIdList:(NSArray *)userIdList {
    return [super initWithOutgoingCall:RCSCallTypeMulty
                        secretChatType:RCSCallSecretChatTypeNO
                              targetId:targetId
                             mediaType:RCSCallMediaTypeAudio
                            userIdList:userIdList];
}

- (instancetype)initWithActiveCall:(RCSCallSession *)callSession {
    return [super initWithActiveCall:callSession];
}

- (void)viewDidLoad {
    [super viewDidLoad];

    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(onUserInfoUpdate:)
                                                 name:RCKitDispatchUserInfoUpdateNotification
                                               object:nil];

    [self initAllUserModel];
    if (self.callSession.callStatus == RCSCallStatusIdle) {
        [[RCCall sharedRCCall] dismissCallViewController:self];
        return;
    }
}

- (void)viewDidAppear:(BOOL)animated {
    [super viewDidAppear:animated];
    [RCCallKitUtility checkSystemPermission:self.callSession.mediaType
        success:^{

        }
        error:^{
            [self hangupButtonClicked];
        }];
}

- (void)initAllUserModel {
    if (self.callSession.callStatus == RCSCallStatusIncoming) {
        if ([self inviterHasHangup]) {
            RCSCallUserProfile *firstUserProfile = self.callSession.userProfileList[0];
            self.mainModel = [self generateUserModel:firstUserProfile.userId];

            self.subUserModelList = [[NSMutableArray alloc] init];
            for (RCSCallUserProfile *userProfile in self.callSession.userProfileList) {
                if (![userProfile.userId isEqualToString:firstUserProfile.userId]) {
                    RCCallUserCallInfoModel *userModel = [self generateUserModel:userProfile.userId];
                    [self.subUserModelList addObject:userModel];
                }
            }
            RCCallUserCallInfoModel *userModel = [self generateUserModel:currentUserId];
            [self.subUserModelList addObject:userModel];
        } else {
            self.mainModel = [self generateUserModel:self.callSession.inviter];

            self.subUserModelList = [[NSMutableArray alloc] init];
            BOOL isContaitSelf = NO;
            for (RCSCallUserProfile *userProfile in self.callSession.userProfileList) {
                if (![userProfile.userId isEqualToString:self.callSession.inviter]) {
                    RCCallUserCallInfoModel *userModel = [self generateUserModel:userProfile.userId];
                    [self.subUserModelList addObject:userModel];
                }
                if ([userProfile.userId isEqualToString:currentUserId]) {
                    isContaitSelf = YES;
                }
            }
            if (!isContaitSelf) {
                RCCallUserCallInfoModel *userModel = [self generateUserModel:currentUserId];
                [self.subUserModelList addObject:userModel];
            }
        }
    } else if (self.callSession.callStatus == RCSCallStatusInviting) {
        self.mainModel = [self generateUserModel:currentUserId];

        self.subUserModelList = [[NSMutableArray alloc] init];
        for (RCSCallUserProfile *userProfile in self.callSession.userProfileList) {
            RCCallUserCallInfoModel *userModel = [self generateUserModel:userProfile.userId];
            [self.subUserModelList addObject:userModel];
        }
    } else if (self.callSession.callStatus == RCSCallStatusConnected) {
        if ([self.callSession.inviter isEqualToString:currentUserId] || [self inviterHasHangup]) {
            self.mainModel = [self generateUserModel:currentUserId];

            self.subUserModelList = [[NSMutableArray alloc] init];
            for (RCSCallUserProfile *userProfile in self.callSession.userProfileList) {
                RCCallUserCallInfoModel *userModel = [self generateUserModel:userProfile.userId];
                [self.subUserModelList addObject:userModel];
            }
        } else {
            self.mainModel = [self generateUserModel:self.callSession.inviter];

            self.subUserModelList = [[NSMutableArray alloc] init];
            for (RCSCallUserProfile *userProfile in self.callSession.userProfileList) {
                if (![userProfile.userId isEqualToString:self.callSession.inviter]) {
                    RCCallUserCallInfoModel *userModel = [self generateUserModel:userProfile.userId];
                    [self.subUserModelList addObject:userModel];
                }
            }
            
//            if (self.callSession.minimized) {
//                [self.subUserModelList addObject:self.mainModel];
//            } else {
                RCCallUserCallInfoModel *userModel = [self generateUserModel:currentUserId];
                [self.subUserModelList addObject:userModel];
//            }
        }
    }
}

- (RCCallUserCallInfoModel *)generateUserModel:(NSString *)userId {
    RCCallUserCallInfoModel *userModel = [[RCCallUserCallInfoModel alloc] init];
    userModel.userId = userId;
    userModel.userInfo = [[RCUserInfoCacheManager sharedManager] getUserInfo:userId];

    if ([userId isEqualToString:currentUserId]) {
        userModel.profile = self.callSession.myProfile;
    } else {
        for (RCSCallUserProfile *userProfile in self.callSession.userProfileList) {
            if ([userProfile.userId isEqualToString:userId]) {
                userModel.profile = userProfile;
                break;
            }
        }
    }

    return userModel;
}

- (NSArray *)getAllUserIdInSubUserModel {
    NSMutableArray *userIdList = [[NSMutableArray alloc] init];
    for (RCCallUserCallInfoModel *userModel in self.subUserModelList) {
        [userIdList addObject:userModel.userId];
    }
    if (userIdList.count > 0) {
        return userIdList;
    } else {
        return nil;
    }
}

- (RCCallUserCallInfoModel *)getModelInSubUserModelList:(NSString *)userId {
    for (RCCallUserCallInfoModel *model in self.subUserModelList) {
        if ([model.userId isEqualToString:userId]) {
            return model;
        }
    }
    return nil;
}

- (BOOL)inviterHasHangup {
    if ([self.callSession.inviter isEqualToString:currentUserId]) {
        if (self.callSession.callStatus == RCSCallStatusIdle) {
            return YES;
        } else {
            return NO;
        }
    } else {
        for (RCSCallUserProfile *userProfile in self.callSession.userProfileList) {
            if ([userProfile.userId isEqualToString:self.callSession.inviter] &&
                userProfile.callStatus != RCSCallStatusIdle) {
                return NO;
            }
        }
        return YES;
    }
}

- (void)setMainModel:(RCCallUserCallInfoModel *)mainModel {
    _mainModel = mainModel;
    if (mainModel.userInfo) {
        self.inviterNameLabel.text = mainModel.userInfo.name;
        [self.inviterPortraitView setImageURL:[NSURL URLWithString:mainModel.userInfo.portraitUri]];
    } else {
        mainModel.userInfo = [[RCUserInfoCacheManager sharedManager] getUserInfo:mainModel.userId];
    }
}

- (void)updateSubUserLayout:(RCCallUserCallInfoModel *)model {
    if (model) {
        NSInteger index = [self.subUserModelList indexOfObject:model];
        if (index != NSNotFound && [self.userCollectionView numberOfItemsInSection:0] > index) {
            NSIndexPath *indexPath = [NSIndexPath indexPathForItem:index inSection:0];
            [self.userCollectionView reloadItemsAtIndexPaths:@[ indexPath ]];
        }
    }
}

- (void)updateAllSubUserLayout {
    [self.userCollectionView reloadData];
}

- (void)addSubUserModel:(RCCallUserCallInfoModel *)model {
    if (model.userId && ![[self getAllUserIdInSubUserModel] containsObject:model.userId]) {
        NSInteger index = self.subUserModelList.count;
        NSIndexPath *indexPath = [NSIndexPath indexPathForItem:index inSection:0];
        [self.subUserModelList addObject:model];
        [self.userCollectionView insertItemsAtIndexPaths:[NSArray arrayWithObject:indexPath]];
    }
}

- (void)removeSubUserModel:(RCCallUserCallInfoModel *)model {
    if (model) {
        NSInteger index = [self.subUserModelList indexOfObject:model];
        if (index != NSNotFound) {
            NSIndexPath *indexPath = [NSIndexPath indexPathForItem:index inSection:0];
            [self.subUserModelList removeObject:model];
            [self.userCollectionView deleteItemsAtIndexPaths:[NSArray arrayWithObject:indexPath]];
        }
    }
}

- (RCloudImageView *)inviterPortraitView {
    if (!_inviterPortraitView) {
        _inviterPortraitView = [[RCloudImageView alloc] init];

        [self.view addSubview:_inviterPortraitView];
        _inviterPortraitView.hidden = YES;
        [_inviterPortraitView setPlaceholderImage:[RCCallKitUtility getDefaultPortraitImage]];
        _inviterPortraitView.layer.masksToBounds = YES;
        if (RCKitConfigCenter.ui.globalConversationAvatarStyle == RC_USER_AVATAR_CYCLE &&
            RCKitConfigCenter.ui.globalMessageAvatarStyle == RC_USER_AVATAR_CYCLE) {
            _inviterPortraitView.layer.cornerRadius = RCCallHeaderLength / 2;
        } else {
            _inviterPortraitView.layer.cornerRadius = 4.f;
        }
    }
    return _inviterPortraitView;
}

- (RCloudImageView *)inviterPortraitBgView {
    if (!_inviterPortraitBgView) {
        _inviterPortraitBgView = [[RCloudImageView alloc] init];

        [self.view addSubview:_inviterPortraitBgView];
        [self.view insertSubview:_inviterPortraitBgView aboveSubview:self.backgroundView];
        _inviterPortraitBgView.hidden = YES;
        [_inviterPortraitBgView setPlaceholderImage:[RCCallKitUtility getDefaultPortraitImage]];
        _inviterPortraitBgView.layer.masksToBounds = YES;
        _inviterPortraitBgView.contentMode = UIViewContentModeScaleAspectFill;
    }
    return _inviterPortraitBgView;
}

- (UILabel *)inviterNameLabel {
    if (!_inviterNameLabel) {
        _inviterNameLabel = [[UILabel alloc] init];
        _inviterNameLabel.backgroundColor = [UIColor clearColor];
        _inviterNameLabel.textColor = [UIColor whiteColor];
        _inviterNameLabel.font = [UIFont systemFontOfSize:18];
        _inviterNameLabel.textAlignment = NSTextAlignmentCenter;
        _inviterNameLabel.layer.shadowOpacity = 0.8;
        _inviterNameLabel.layer.shadowRadius = 3.0;
        _inviterNameLabel.layer.shadowColor = [UIColor blackColor].CGColor;
        _inviterNameLabel.layer.shadowOffset = CGSizeMake(0, 1);
        [self.view addSubview:_inviterNameLabel];
        _inviterNameLabel.hidden = YES;
    }
    return _inviterNameLabel;
}

- (UILabel *)userCollectionTitleLabel {
    if (!_userCollectionTitleLabel) {
        _userCollectionTitleLabel = [[UILabel alloc] init];
        _userCollectionTitleLabel.backgroundColor = [UIColor clearColor];
        _userCollectionTitleLabel.textColor = [UIColor whiteColor];
        _userCollectionTitleLabel.font = [UIFont systemFontOfSize:16];
        _userCollectionTitleLabel.textAlignment = NSTextAlignmentCenter;
        _userCollectionTitleLabel.text = RCCallKitLocalizedString(@"VoIPMultiCallUserCollectionTitle");
        _userCollectionTitleLabel.layer.shadowOpacity = 0.8;
        _userCollectionTitleLabel.layer.shadowRadius = 3.0;
        _userCollectionTitleLabel.layer.shadowColor = [UIColor blackColor].CGColor;
        _userCollectionTitleLabel.layer.shadowOffset = CGSizeMake(0, 1);
        [self.view addSubview:_userCollectionTitleLabel];
        _userCollectionTitleLabel.hidden = YES;
    }
    return _userCollectionTitleLabel;
}

- (UICollectionView *)userCollectionView {
    if (!_userCollectionView) {
        if (self.userCollectionViewLayout) {
            _userCollectionView = [[UICollectionView alloc] initWithFrame:CGRectZero
                                                     collectionViewLayout:self.userCollectionViewLayout];
        } else {
            RCAudioMultiCallUserCollectionLayout *userCollectionViewLayout =
                [[RCAudioMultiCallUserCollectionLayout alloc] initWithItemMargin:5 buttomPadding:10];
            _userCollectionView = [[UICollectionView alloc] initWithFrame:CGRectZero
                                                     collectionViewLayout:userCollectionViewLayout];
        }
        _userCollectionView.backgroundColor = [UIColor clearColor];

        [self.view addSubview:_userCollectionView];
        _userCollectionView.hidden = YES;
        _userCollectionView.dataSource = self;
        [_userCollectionView registerClass:[RCCallMultiCallUserCell class]
                forCellWithReuseIdentifier:RongVoIPMultiAudioCellReuseID];
    }
    return _userCollectionView;
}

- (void)inviteUserButtonClicked {
    [self didTapInviteUserButton];

    if (self.callSession.isMultiCall) {
        [self inviteNewUser];
    } else {
        NSLog(@"not support add user for conversation type %zd", self.callSession.callType);
    }
}

- (void)inviteNewUser {
    NSMutableArray *existUserIdList = [[NSMutableArray alloc] init];
    for (RCSCallUserProfile *userProfile in self.callSession.userProfileList) {
        [existUserIdList addObject:userProfile.userId];
    }
    [existUserIdList addObject:currentUserId];

    __weak typeof(self) weakSelf = self;
    if ([RCCall sharedRCCall].callInviteNewUserDelegate &&
        [[RCCall sharedRCCall].callInviteNewUserDelegate
            respondsToSelector:@selector(inviteNewUser:targetId:mediaType:baseOn:selectResult:)] &&
        self.callSession) {
        [[RCCall sharedRCCall].callInviteNewUserDelegate
            inviteNewUser:existUserIdList
                 targetId:self.targetId
                mediaType:self.mediaType
                   baseOn:self
             selectResult:^(NSArray<NSString *> *userIdList) {
//            [weakSelf.callSession inviteRemoteUsers:userIdList completion:nil];
            NSMutableArray *array = [NSMutableArray array];
            for (NSString *userId in userIdList) {
                RCSCallToUser *user = [[RCSCallToUser alloc] init];
                user.userId = userId;
                user.targetType = 1;
                user.targetId = self.targetId;
                user.userType = RCSCallUserTypeNormal;
                [array addObject:user];
            }
            
            [weakSelf.callSession inviteRemoteUsers:array completion:nil];
             }];
    } else {
        RCCallSelectMemberViewController *selectViewController = [[RCCallSelectMemberViewController alloc]
            initWithConversationType:self.conversationType
                            targetId:self.targetId
                           mediaType:self.mediaType
                               exist:[existUserIdList copy]
                             success:^(NSArray *addUserIdList) {
//            [weakSelf.callSession inviteRemoteUsers:addUserIdList completion:nil];
            NSMutableArray *array = [NSMutableArray array];
            for (NSString *userId in addUserIdList) {
                RCSCallToUser *user = [[RCSCallToUser alloc] init];
                user.userId = userId;
                user.targetType = 1;
                user.targetId = self.targetId;
                user.userType = RCSCallUserTypeNormal;
                [array addObject:user];
            }
            
            [weakSelf.callSession inviteRemoteUsers:array completion:nil];
                             }];
        UINavigationController *nav = [[UINavigationController alloc] initWithRootViewController:selectViewController];
        nav.modalPresentationStyle = UIModalPresentationFullScreen;
        dispatch_async(dispatch_get_main_queue(), ^{
            [weakSelf presentViewController:nav animated:YES completion:nil];
        });
    }
}

- (void)inviteNewUserWithClickEvent {
    NSMutableArray *existUserIdList = [[NSMutableArray alloc] init];
    for (RCSCallUserProfile *userProfile in self.callSession.userProfileList) {
        [existUserIdList addObject:userProfile.userId];
    }
    [existUserIdList addObject:currentUserId];

    __weak typeof(self) weakSelf = self;
    RCCallSelectMemberViewController *selectViewController = [[RCCallSelectMemberViewController alloc]
        initWithConversationType:self.conversationType
                        targetId:self.targetId
                       mediaType:self.mediaType
                           exist:existUserIdList
                         success:^(NSArray *addUserIdList) {
//        [weakSelf.callSession inviteRemoteUsers:addUserIdList completion:nil];
        NSMutableArray *array = [NSMutableArray array];
        for (NSString *userId in addUserIdList) {
            RCSCallToUser *user = [[RCSCallToUser alloc] init];
            user.userId = userId;
            user.targetType = 1;
            user.targetId = self.targetId;
            user.userType = RCSCallUserTypeNormal;
            [array addObject:user];
        }
        
        [weakSelf.callSession inviteRemoteUsers:array completion:nil];
                         }];
    UINavigationController *rootVC = [[UINavigationController alloc] initWithRootViewController:selectViewController];
    rootVC.modalPresentationStyle = UIModalPresentationFullScreen;
    dispatch_async(dispatch_get_main_queue(), ^{
        [weakSelf presentViewController:rootVC animated:YES completion:nil];
    });
}

- (void)resetLayout:(BOOL)isMultiCall mediaType:(RCSCallMediaType)mediaType callStatus:(RCSCallStatus)sessionCallStatus {
    [super resetLayout:isMultiCall mediaType:mediaType callStatus:sessionCallStatus];

    RCSCallStatus callStatus = sessionCallStatus;
    if (callStatus == RCSCallStatusIncoming &&
        [RCCXCall sharedInstance].acceptedFromCallKit) {
        callStatus = RCSCallStatusConnected;
        [RCCXCall sharedInstance].acceptedFromCallKit = NO;
    }

    self.blurView.hidden = YES;

    if (callStatus == RCSCallStatusIncoming) {
        [self.inviterPortraitView setImageURL:[NSURL URLWithString:self.mainModel.userInfo.portraitUri]];
        self.inviterPortraitView.frame =
            CGRectMake((self.view.frame.size.width - RCCallHeaderLength) / 2,
                       RCCallTopGGradientHeight + RCCallStatusBarHeight, RCCallHeaderLength, RCCallHeaderLength);
        self.inviterPortraitView.hidden = NO;

        [self.inviterPortraitBgView setImageURL:[NSURL URLWithString:self.mainModel.userInfo.portraitUri]];
        self.inviterPortraitBgView.frame = CGRectMake(0, 0, self.view.frame.size.width, self.view.frame.size.height);
        self.inviterPortraitBgView.hidden = YES;

        self.inviterNameLabel.text = self.mainModel.userInfo.name;
        self.inviterNameLabel.frame =
            CGRectMake((self.view.frame.size.width - RCCallNameLabelWidth) / 2,
                       RCCallTopGGradientHeight + RCCallHeaderLength + RCCallTopMargin + RCCallStatusBarHeight,
                       RCCallNameLabelWidth, RCCallLabelHeight);
        self.inviterNameLabel.hidden = NO;

    } else if (callStatus == RCSCallStatusInviting) {
        self.inviterPortraitBgView.frame = CGRectMake(0, 0, self.view.frame.size.width, self.view.frame.size.height);
        self.inviterPortraitBgView.hidden = YES;
        self.inviterNameLabel.hidden = YES;
        self.inviterPortraitView.hidden = YES;
    } else {
        self.inviterNameLabel.hidden = YES;
        self.inviterPortraitView.hidden = YES;
        self.inviterPortraitBgView.hidden = YES;
    }

    CGFloat titleY = MAX(RCCallVerticalMargin * 2 + RCCallHeaderLength + RCCallInsideMargin * 3 + RCCallLabelHeight * 2,
                         (self.view.frame.size.height - RCCallLabelHeight) / 2);
    if (callStatus == RCSCallStatusIncoming) {
        self.userCollectionTitleLabel.frame =
            CGRectMake(RCCallHorizontalMargin,
                       self.view.frame.size.height - RCCallButtonBottomMargin * 2 - RCCallButtonLength - 100.0 - 23.0 -
                           16.0 - RCCallExtraSpace,
                       self.view.frame.size.width - RCCallHorizontalMargin * 2, RCCallLabelHeight);
        self.userCollectionTitleLabel.hidden = NO;
        _userCollectionTitleLabel.text = RCCallKitLocalizedString(@"VoIPMultiCallUserCollectionTitle");
        _userCollectionTitleLabel.font = [UIFont fontWithName:@"PingFangSC-Regular" size:16];
        self.userCollectionTitleLabel.alpha = 0.4;
    } else {
        self.userCollectionTitleLabel.alpha = 0.4;
        self.userCollectionTitleLabel.frame =
            CGRectMake(RCCallHorizontalMargin, RCCallMiniButtonTopMargin + RCCallStatusBarHeight,
                       self.view.frame.size.width - RCCallHorizontalMargin * 2, RCCallMiniLabelHeight);
        self.userCollectionTitleLabel.hidden = NO;
        self.userCollectionTitleLabel.text = RCCallKitLocalizedString(@"VoIPAudioCall");
        ;
        _userCollectionTitleLabel.font = [UIFont fontWithName:@"PingFangSC-Regular" size:18];
    }

    if (callStatus == RCSCallStatusIncoming) {
        self.userCollectionView.frame = CGRectMake(
            RCCallHorizontalMargin,
            self.view.frame.size.height - RCCallButtonBottomMargin * 2 - RCCallButtonLength - 100.0 - RCCallExtraSpace,
            self.view.frame.size.width - RCCallHorizontalMargin * 2, 80.0);

        if (self.userCollectionViewLayout) {
            self.userCollectionView.collectionViewLayout = self.userCollectionViewLayout;
        } else {
            UICollectionViewFlowLayout *userCollectionViewLayout = [[UICollectionViewFlowLayout alloc] init];
            userCollectionViewLayout.itemSize = CGSizeMake(55, 55);
            userCollectionViewLayout.scrollDirection = UICollectionViewScrollDirectionHorizontal;
            [self.userCollectionView setCollectionViewLayout:userCollectionViewLayout animated:YES];
        }
        self.userCollectionView.hidden = NO;
    } else if (callStatus == RCSCallStatusInviting || callStatus == RCSCallStatusConnected) {
        self.userCollectionView.frame = CGRectMake(0, RCCallTopGGradientHeight + 2.0f + RCCallStatusBarHeight,
                                                   self.view.frame.size.width, RCCallButtonLength * 3);
        self.tipsLabel.frame =
            CGRectMake(RCCallHorizontalMargin,
                       self.userCollectionView.frame.origin.y + self.userCollectionView.frame.size.height + 40.0f,
                       self.view.frame.size.width - RCCallHorizontalMargin * 2, RCCallLabelHeight);

        if (self.userCollectionViewLayout) {
            self.userCollectionView.collectionViewLayout = self.userCollectionViewLayout;
        } else {
            RCAudioMultiCallUserCollectionLayout *userCollectionViewLayout =
                [[RCAudioMultiCallUserCollectionLayout alloc] initWithItemMargin:7 buttomPadding:15];
            userCollectionViewLayout.scrollDirection = UICollectionViewScrollDirectionVertical;
            [self.userCollectionView setCollectionViewLayout:userCollectionViewLayout animated:YES];
        }
        self.userCollectionView.hidden = NO;
    }
}

#pragma mark - RCSCallSessionDelegate

/*!
 通话已接通
 */
- (void)callSession:(RCSCallSession *)session callDidConnect:(NSString *)callId {
    [self.userCollectionView removeFromSuperview];
    _userCollectionView = nil;
    if (![self.callSession.caller isEqualToString:currentUserId]) {
        [self.subUserModelList removeObject:[self getModelInSubUserModelList:currentUserId]];
        [self.subUserModelList addObject:self.mainModel];
        self.mainModel = nil;
    }
    [self userCollectionView];
    [self updateAllSubUserLayout];
    [super callSession:session callDidConnect:callId];
}

/*!
 对端正在振铃

 @param userId 对端的用户ID
 */
- (void)callSession:(RCSCallSession *)session remoteUserDidRing:(NSString *)userId {
    RCCallUserCallInfoModel *model = [self getModelInSubUserModelList:userId];
    [self updateSubUserLayout:model];
}

/*!
 有用户被邀请加入通话

 @param userId    被邀请的用户ID
 @param mediaType 希望被邀请者选择的媒体类型
 */
- (void)callSession:(RCSCallSession *)session
remoteUserDidInvite:(NSString *)userId {
    RCCallUserCallInfoModel *model = [self generateUserModel:userId];
    [self addSubUserModel:model];

    NSString *userName = [[RCUserInfoCacheManager sharedManager] getUserInfo:userId].name;
    if (userName) {
        NSString *inviteTips =
            [NSString stringWithFormat:RCCallKitLocalizedString(@"VoIPCallRemoteDidInvite"), userName];
        self.tipsLabel.text = inviteTips;

        __weak typeof(self) weakSelf = self;
        dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(0.5 * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
            if ([weakSelf.tipsLabel.text isEqualToString:inviteTips]) {
                weakSelf.tipsLabel.text = @"";
            }
        });
    }
}

/*!
 有用户加入了通话

 @param userId    用户ID
 @param mediaType 用户的媒体类型
 */
- (void)callSession:(RCSCallSession *)session
  remoteUserDidJoin:(NSString *)userId
          mediaType:(RCSCallMediaType)mediaType {
    if (![userId isEqualToString:self.mainModel.userId]) {
        RCCallUserCallInfoModel *model = [self getModelInSubUserModelList:userId];
        if (!model) {
            [self addSubUserModel:[self generateUserModel:userId]];
        } else {
            [self updateSubUserLayout:model];
        }
    }
}

- (void)callSession:(RCSCallSession *)session
remoteUserDidMicrophoneMute:(BOOL)mute
             byUser:(NSString *)userId {
}

/*!
 对端用户关闭/打开了摄像头

 @param userId    用户ID
 @param muted     是否关闭camera
 */
- (void)callSession:(RCSCallSession *)session remoteUserDidCameraEnable:(BOOL)muted byUser:(NSString *)userId {
    // ignore
}

/*!
 有用户挂断

 @param userId 用户ID
 @param reason 挂断的原因
 */
- (void)callSession:(RCSCallSession *)session
  remoteUserDidLeft:(NSString *)userId
             reason:(RCSCallDisconnectReason)reason {
    if ([userId isEqualToString:self.mainModel.userId]) {
        if (self.callSession.callStatus == RCSCallStatusIncoming ||
            self.callSession.callStatus == RCSCallStatusConnected) {
            RCCallUserCallInfoModel *tempModel = self.subUserModelList[0];
            self.mainModel = tempModel;
            if (self.callSession.callStatus == RCSCallStatusIncoming) {
                self.inviterNameLabel.text = tempModel.userInfo.name;
            }

            [self.subUserModelList removeObject:tempModel];
            [self updateAllSubUserLayout];
        }
    } else {
        for (RCCallUserCallInfoModel *userModel in self.subUserModelList) {
            if ([userId isEqualToString:userModel.userId]) {
                [self removeSubUserModel:userModel];
                break;
            }
        }
    }
}

#pragma mark - UICollectionViewDataSource
- (NSInteger)collectionView:(UICollectionView *)collectionView numberOfItemsInSection:(NSInteger)section {
    return self.subUserModelList.count;
}

- (UICollectionViewCell *)collectionView:(UICollectionView *)collectionView
                  cellForItemAtIndexPath:(NSIndexPath *)indexPath {
    RCCallMultiCallUserCell *cell = [collectionView dequeueReusableCellWithReuseIdentifier:RongVoIPMultiAudioCellReuseID
                                                                              forIndexPath:indexPath];
    RCCallUserCallInfoModel *model = self.subUserModelList[indexPath.row];
    [cell setModel:model status:self.callSession.callStatus];
    return cell;
}

#pragma mark - UserInfo Update
- (void)onUserInfoUpdate:(NSNotification *)notification {
    NSDictionary *userInfoDic = notification.object;
    NSString *updateUserId = userInfoDic[@"userId"];
    RCUserInfo *updateUserInfo = userInfoDic[@"userInfo"];

    __weak typeof(self) weakSelf = self;
    dispatch_async(dispatch_get_main_queue(), ^{
        if ([updateUserId isEqualToString:weakSelf.mainModel.userId]) {
            weakSelf.mainModel.userInfo = updateUserInfo;
            weakSelf.inviterNameLabel.text = updateUserInfo.name;
            [weakSelf.inviterPortraitView setImageURL:[NSURL URLWithString:updateUserInfo.portraitUri]];
        } else {
            RCCallUserCallInfoModel *model = [weakSelf getModelInSubUserModelList:updateUserInfo.userId];
            if (model) {
                model.userInfo = updateUserInfo;
                [weakSelf updateSubUserLayout:model];
            }
        }
    });
}

- (void)dealloc {
    [[NSNotificationCenter defaultCenter] removeObserver:self];
}

@end
