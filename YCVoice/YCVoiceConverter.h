//
//  YCVoiceConverter.h
//  YCVoiceSample
//
//  Created by 廖金花 on 13-9-12.
//  Copyright (c) 2013年 Yongche. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface YCVoiceConverter : NSObject

+ (BOOL)amr:(NSString *)amrPath toWav:(NSString *)wavPath;
+ (BOOL)wav:(NSString *)wavPath toAmr:(NSString *)amrPath;


@end
