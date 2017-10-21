// Copyright (c) Microsoft Corporation.
// All rights reserved.
//
// This code is licensed under the MIT License.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#import "ADLogger.h"

#define AD_LOG(_level, _code, _correlationId, _dict, _isPii, _fmt, ...) \
    [ADLogger log:_level context:self errorCode:_code correlationId:_correlationId userInfo:_dict isPii:_isPii format:_fmt, ##__VA_ARGS__]

#define FIRST_ARG(ARG,...) ARG

#define AD_LOG_ERROR(_code, _correlationId, _dict, _isPii, _fmt, ...) \
    AD_LOG(ADAL_LOG_LEVEL_ERROR, _code, _correlationId, _dict, _isPii, _fmt, ##__VA_ARGS__)

#define AD_LOG_WARN(_correlationId, _dict, _isPii, _fmt, ...) \
    AD_LOG(ADAL_LOG_LEVEL_WARN, AD_ERROR_SUCCEEDED, _correlationId, _dict, _isPii, _fmt, ##__VA_ARGS__)

#define AD_LOG_INFO(_correlationId, _dict, _isPii, _fmt, ...) \
    AD_LOG(ADAL_LOG_LEVEL_INFO, AD_ERROR_SUCCEEDED, _correlationId, _dict, _isPii, _fmt, ##__VA_ARGS__)

#define AD_LOG_VERBOSE(_correlationId, _dict, _isPii, _fmt, ...) \
    AD_LOG(ADAL_LOG_LEVEL_VERBOSE, AD_ERROR_SUCCEEDED, _correlationId, _dict, _isPii, _fmt, ##__VA_ARGS__)

#ifndef DebugLog
#ifdef DEBUG
#   define DebugLog(fmt, ...) NSLog((@"%s[%d][%@] " fmt), __PRETTY_FUNCTION__, __LINE__, [[NSThread currentThread] isEqual:[NSThread mainThread]] ? @"main" : @"work", ##__VA_ARGS__);
#else
#   define DebugLog(...)
#endif
#endif

@interface ADLogger (Internal)

/*! Returns diagnostic trace data to be sent to the Auzure Active Directory servers. */
+ (NSDictionary*)adalId;

/*! Calculates a hash of the passed string. Useful for logging tokens, where we do not log
 the actual contents, but still want to log something that can be correlated. */
+ (NSString*)getHash:(NSString*)input;

+ (void)setAdalVersion:(NSString*)version;

+ (NSString*)getAdalVersion;

+ (NSString*)getCPUInfo;

/*! Returns previously set callback call or nil, if the user has not set such callback. */
+ (LogCallback)getLogCallBack;

///*! Main logging function. Macros like ADAL_LOG_ERROR are provided on top for convenience
// @param logLevel The applicable priority of the logged message. Use AD_LOG_LEVEL_NO_LOG to disable all logging.
// @param context Reference to the context object (usually self).
// @param message Short text defining the operation/condition.
// @param errorCode if an explicit error has occurred, this code will contain its code.
// @param piiMessage Message possibly containing Personally Identifiable Information (PII).
// @param correlationId CorrelationId string.
// @param userInfo User's info.
// this should be true.
//
// */
//+ (void)log:(ADAL_LOG_LEVEL)logLevel
//    context:(id)context
//    message:(NSString *)message
//  errorCode:(NSInteger)errorCode
// piiMessage:(NSString *)piiMessage
//correlationId:(NSUUID *)correlationId
//   userInfo:(NSDictionary *)userInfo;


+ (void)log:(ADAL_LOG_LEVEL)level
    context:(id)context
  errorCode:(NSInteger)code
correlationId:(NSUUID *)correlationId
   userInfo:(NSDictionary *)userInfo
      isPii:(BOOL)isPii
     format:(NSString *)format, ... __attribute__((format(__NSString__, 7, 8)));

/*! Logs obtaining of a token. The method does not log the actual token, only its hash.
 @param token The token to log.
 @param tokenType "access token", "refresh token", "multi-resource refresh token"
 @param expiresOn The time when an access token will stop to be valid. Nil for refresh token types.
 @param correlationId In case the token was just obtained from the server, the correlation id of the call.
 This parameter can be nil.
 */
+ (void)logToken:(NSString *)token
       tokenType:(NSString *)tokenType
       expiresOn:(NSDate *)expiresOn
         context:(NSString *)context
   correlationId:(NSUUID *)correlationId;

+ (void)setIdValue:(NSString*)value
            forKey:(NSString*)key;

@end
