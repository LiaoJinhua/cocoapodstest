//
//  YCVoicePlayer.h
//  YCVoiceSample
//
//  Created by 廖金花 on 13-9-11.
//  Copyright (c) 2013年 Yongche. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface YCVoicePlayer : NSObject

+ (YCVoicePlayer *)shareVoicePlayer;

- (BOOL)playText:(NSString *)text;

- (BOOL)playFile:(NSString *)filePath;

@end
