//
//  YCVoicePlayerTests.m
//  YCVoiceSample
//
//  Created by 廖金花 on 13-9-11.
//  Copyright (c) 2013年 Yongche. All rights reserved.
//

#import "YCVoicePlayerTests.h"
#import "YCVoicePlayer.h"

@implementation YCVoicePlayerTests

- (void)testPlayTextSuccess
{
    BOOL result = [[YCVoicePlayer shareVoicePlayer] playText:@"请允许我重提我们刚才讨论过的问题，我想我们应该强调保存我们的自然资源"];
    STAssertTrue(result, @"Voice Player play text should success but failed.");
    sleep(10);
}

- (void)testPlayTextFailed
{
    BOOL result = [[YCVoicePlayer shareVoicePlayer] playText:@""];
    STAssertFalse(result, @"Voice Player play text which length is 0should failed but success.");
    
    result = [[YCVoicePlayer shareVoicePlayer] playText:nil];
    STAssertFalse(result, @"Voice Player play nil text should failed but success.");
}

- (void)testPlayAudioFileSuccess
{
    NSBundle *bundle = [NSBundle bundleForClass:[self class]];
    NSString *filePath = [bundle pathForResource:@"welcome" ofType:@"wav"];
    BOOL result = [[YCVoicePlayer shareVoicePlayer] playFile:filePath];
    STAssertTrue(result, @"Voice Player play wav file should success but failed.");
    sleep(5);
}

- (void)testPlayAudioFileFailed
{
    BOOL result = [[YCVoicePlayer shareVoicePlayer] playFile:nil];
    STAssertFalse(result, @"Voice Player play nil file should failed but success");
    
    NSBundle *bundle = [NSBundle bundleForClass:[self class]];
    NSString *filePath = [bundle pathForResource:@"infoPlist" ofType:@"strings"];
    result = [[YCVoicePlayer shareVoicePlayer] playFile:filePath];
    STAssertFalse(result, @"Voice Player play not audio file should failed but success.");
}

@end
