//
//  YCVoicePlayer.m
//  YCVoiceSample
//
//  Created by 廖金花 on 13-9-11.
//  Copyright (c) 2013年 Yongche. All rights reserved.
//

#import "YCVoicePlayer.h"
#import "hci_tts_player.h"
#import "CommonTool.h"
#import <AVFoundation/AVFoundation.h>

using namespace std;
/************player回调函数定义************/
/*
 * palyer事件回调
 *
 */
void PlayerStateChangeCallback(_MUST_ _IN_ PLAYER_EVENT ePlayerEvent,
                               _OPT_ _IN_ void * pUsrParam ){
    NSLog(@"PlayerStateChangeCallback : ePlayerEvent = %d",ePlayerEvent);
	switch (ePlayerEvent) {
		case PLAYER_EVENT_END:
		{
		}
			break;
	    case PLAYER_EVENT_RESUME:
		{
		}
			break;
	    case PLAYER_EVENT_PAUSE:
		{
		}
			break;
		default:
			break;
	}
}

/*
 * palyer 播放进度回调
 *
 */
void PlayerProgressChangeCallback(
                                  _MUST_ _IN_ PLAYER_EVENT ePlayerEvent,
                                  _MUST_ _IN_ int nStart,
                                  _MUST_ _IN_ int nStop,
                                  _OPT_ _IN_ void * pUsrParam )
{

	
}

/*
 * palyer 出错回调
 * 可以在这里进行出错处理：如通知用户错误信息
 */
void PlayerErr_Callback(
                        _MUST_ _IN_ PLAYER_EVENT ePlayerEvent,
                        _MUST_ _IN_ HCI_ERR_CODE eErrorCode,
                        _OPT_ _IN_ void * pUsrParam )
{
    NSLog(@"ERROR :__errorcode_[%d]__",eErrorCode);
}

@interface YCVoicePlayer ()
{
    AVAudioPlayer *_player;
}

@end

@implementation YCVoicePlayer

+ (YCVoicePlayer *)shareVoicePlayer
{
    static YCVoicePlayer *sharedPlayer = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        sharedPlayer = [[self alloc] init];
    });
    return sharedPlayer;
}

- (id)init
{
    if (self = [super init]) {
        [self startHciPlayer];
    }
    return self;
}

- (void)startHciPlayer
{
    //初始化公共模块和player
    HCI_ERR_CODE errCode = HCI_ERR_NONE;
	
	// 初始化 灵云 系统
	//	platformId,developerId,appNo,developerKey: 捷通分配的应用账号信息
	//	logFileSize,logLevel,logFilePath,logFileCount: 日志相关配置
	//	authPath: 读写授权文件和用户信息文件的路径
	//	autoCloudAuth: 是否自动检测云端授权
	//	cloudUrl: 云端授权地址
	
	std::string strAccountInfo;
	bool bRet = GetAccountInfo(@"AccountInfo", @"txt",strAccountInfo);
	if (!bRet)
	{
		printf("\n请检测AccountInfo.txt是否填写了从接通申请的授权帐号!\n");
		return ;
	}
	
	std::string strConfig =
	strAccountInfo +
	"logFileSize=500,logLevel=0,logFileCount=1,"
	"autoCloudAuth=no";
    //日志路径，必须是存在的可读写的路径
    string strLogPath = string(",logFilePath=") + GetIosAppDocumentPath();
    //授权文件路径，必须是存在的可读写的路径
    string strAuthPath = string(",authPath=") + GetIosAppDocumentPath();
	strConfig += strLogPath + strAuthPath;
	errCode = hci_init( strConfig.c_str() );
	if (errCode != HCI_ERR_NONE)
	{
		printf("hci_init return %d\n",errCode);
		return ;
	}
	printf("hci_init success\n");
	
	// 检查授权是否正确，必要时会下载云端授权
	errCode = hci_check_auth();
	if (errCode != HCI_ERR_NONE)
	{
		hci_release();
		printf("hci_check_auth return %d\n",errCode);
		return;
	}
	printf("hci_check_auth success\n");
    
    //设置回调函数及用户数据
    
    PLAYER_CALLBACK_PARAM paramcallback;
    paramcallback.pfnStateChange = PlayerStateChangeCallback;
    paramcallback.pfnProgressChange = PlayerProgressChangeCallback;
    paramcallback.pfnPlayerError = PlayerErr_Callback;
	paramcallback.pvProgressChangeUsrParam = nil;
	paramcallback.pvStateChangeUsrParam = nil;
	paramcallback.pvPlayerErrorUsrParam = nil;
    
    // 初始化 tts player 能力
	//	dataPath: 指定 TTS 本地音库所在的路径
	//	initCapKeys: 需要使用的本地能力，云端能力不需要在这里设置
    string strPlayerInitConfig = string("initCapKeys=tts.local.zhangnan");
	
	//获取本地音库所在路径
	string strSourcePath = GetIosResDataPath(@"ZhangNan", @"voclib");
	strPlayerInitConfig += string(",dataPath=") + strSourcePath;
    
    PLAYER_ERR_CODE playerRet = PLAYER_ERR_NONE;
	playerRet = hci_tts_player_init(strPlayerInitConfig.c_str(), &paramcallback);
    if (playerRet != PLAYER_ERR_NONE) {
		hci_release();
		printf("hci_tts_player_init return %d\n",playerRet);
        return;
    }
    NSLog(@"hci_tts_player_init success");

}

- (BOOL)playText:(NSString *)text
{
    if ([self checkTextValid:text]) {
        const char * pszConfig = "capKey=tts.local.zhangnan,pitch=5,volume=5,speed=5,puncMode=off,digitMode=auto_number,voiceStyle=normal,audioFormat=pcm16k16bit";
        PLAYER_ERR_CODE playerRet = PLAYER_ERR_NONE;
        playerRet =  hci_tts_player_start([text UTF8String], pszConfig);
        if (playerRet == PLAYER_ERR_NONE) {
            NSLog(@"hci_tts_player_start success");
            return true;
        }
    }
    return false;
}

- (BOOL)checkTextValid:(NSString *)text
{
    if (text && text.length > 0) {
        return true;
    }
    return false;
}

- (BOOL)playFile:(NSString *)filePath
{
    NSData *fileData = [NSData dataWithContentsOfFile:filePath];
    if (fileData) {
        [self checkPlayingAndStopPlay];
        [self playAudioData:fileData];
    }
    else {
        return false;
    }
    return true;
}

- (void)checkPlayingAndStopPlay
{
    if (_player.isPlaying) {
        [_player stop];
    }
}

- (void)playAudioData:(NSData *)audioData
{
    _player = [[AVAudioPlayer alloc] initWithData:audioData error:nil];
    _player.volume = 1.0;
    [_player prepareToPlay];
    
    [[AVAudioSession sharedInstance] setCategory: AVAudioSessionCategoryPlayAndRecord
                                           error: nil];
    
    UInt32 audioRoute = kAudioSessionOverrideAudioRoute_Speaker;
    AudioSessionSetProperty(kAudioSessionProperty_OverrideAudioRoute,
                            sizeof(audioRoute),
                            &audioRoute);
    
    [_player play];
}
@end
