//
//  YCVoiceConverter.m
//  YCVoiceSample
//
//  Created by 廖金花 on 13-9-12.
//  Copyright (c) 2013年 Yongche. All rights reserved.
//

#import "YCVoiceConverter.h"
#import "amrFileCodec.h"

@implementation YCVoiceConverter

+ (BOOL)amr:(NSString *)amrPath toWav:(NSString *)wavPath
{
    if (DecodeAMRFileToWAVEFile([amrPath cStringUsingEncoding:NSASCIIStringEncoding],
                                [wavPath cStringUsingEncoding:NSASCIIStringEncoding])) {
        return true;
    }
    return false;
}

+ (BOOL)wav:(NSString *)wavPath toAmr:(NSString *)amrPath
{
    if (EncodeWAVEFileToAMRFile([wavPath cStringUsingEncoding:NSASCIIStringEncoding],
                                [amrPath cStringUsingEncoding:NSASCIIStringEncoding], 2, 16)) {
        return true;
    }
    return false;
}

@end
