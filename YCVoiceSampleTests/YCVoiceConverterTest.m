//
//  YCVoiceConverterTest.m
//  YCVoiceSample
//
//  Created by 廖金花 on 13-9-12.
//  Copyright (c) 2013年 Yongche. All rights reserved.
//

#import "YCVoiceConverterTest.h"
#import "YCVoiceConverter.h"

@implementation YCVoiceConverterTest

- (NSString *)documentPath
{
    return [NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES) lastObject];
}

- (void)testWavToAmrSuccess
{
    NSBundle *bundle = [NSBundle bundleForClass:[self class]];
    NSString *wavPath = [bundle pathForResource:@"welcome" ofType:@"wav"];
    
    NSString *amrPath = [NSString stringWithFormat:@"%@/welcome.amr",[self documentPath]];
                         
    STAssertTrue([YCVoiceConverter wav:wavPath toAmr:amrPath], @"Voice converter convert wav file to amr file failed.");
    
    NSFileManager *filemanger = [NSFileManager defaultManager];
    STAssertTrue([filemanger fileExistsAtPath:amrPath], @"Amr file is not exist.convert failed.");
    [filemanger removeItemAtPath:amrPath error:nil];
}

- (void)testWavToAmrFailed
{
    NSString *amrPath = [NSString stringWithFormat:@"%@/welcome.amr",[self documentPath]];
    STAssertFalse([YCVoiceConverter wav:nil toAmr:amrPath], @"Source file is nil,but convert success");
    
    NSBundle *bundle = [NSBundle bundleForClass:[self class]];
    NSString *wavPath = [bundle pathForResource:@"welcome" ofType:@"wav"];
    STAssertFalse([YCVoiceConverter wav:wavPath toAmr:nil], @"Destinate file is nil,but convert success");
}

- (void)testAmrToWavSuccess
{
    NSBundle *bundle = [NSBundle bundleForClass:[self class]];
    NSString *amrPath = [bundle pathForResource:@"bubugao" ofType:@"amr"];
    
    NSString *wavPath = [NSString stringWithFormat:@"%@/bubugao.wav",[self documentPath]];
    
    STAssertTrue([YCVoiceConverter amr:amrPath toWav:wavPath], @"Voice converter convert amr file to wav file failed.");
    
    NSFileManager *filemanger = [NSFileManager defaultManager];
    STAssertTrue([filemanger fileExistsAtPath:amrPath], @"Wav file is not exist.convert failed.");
    [filemanger removeItemAtPath:wavPath error:nil];
}

- (void)testAmrToWavFailed
{
    NSString *wavPath = [NSString stringWithFormat:@"%@/bubugao.wav",[self documentPath]];
    STAssertFalse([YCVoiceConverter amr:nil toWav:wavPath], @"Source file is nil,but convert success");

    NSBundle *bundle = [NSBundle bundleForClass:[self class]];
    NSString *amrPath = [bundle pathForResource:@"bubugao" ofType:@"amr"];
    STAssertFalse([YCVoiceConverter amr:amrPath toWav:nil], @"Destinate file is nil,but convert success");
}

@end
