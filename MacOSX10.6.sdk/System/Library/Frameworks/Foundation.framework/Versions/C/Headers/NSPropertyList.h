/*	NSPropertyList.h
	Copyright (c) 2002-2008, Apple Inc. All rights reserved.
*/

#import <Foundation/NSObject.h>
#include <CoreFoundation/CFPropertyList.h>

#if MAC_OS_X_VERSION_10_2 <= MAC_OS_X_VERSION_MAX_ALLOWED

@class NSData, NSString, NSError, NSInputStream, NSOutputStream;

enum {
    NSPropertyListImmutable = kCFPropertyListImmutable,
    NSPropertyListMutableContainers = kCFPropertyListMutableContainers,
    NSPropertyListMutableContainersAndLeaves = kCFPropertyListMutableContainersAndLeaves
};
typedef NSUInteger NSPropertyListMutabilityOptions;

enum {
    NSPropertyListOpenStepFormat = kCFPropertyListOpenStepFormat,
    NSPropertyListXMLFormat_v1_0 = kCFPropertyListXMLFormat_v1_0,
    NSPropertyListBinaryFormat_v1_0 = kCFPropertyListBinaryFormat_v1_0
};
typedef NSUInteger NSPropertyListFormat;

typedef NSUInteger NSPropertyListReadOptions;
typedef NSUInteger NSPropertyListWriteOptions;

@interface NSPropertyListSerialization : NSObject {
    void *reserved[6];
}

/* Verify that a specified property list is valid for a given format. Returns YES if the property list is valid.
 */
+ (BOOL)propertyList:(id)plist isValidForFormat:(NSPropertyListFormat)format;

/* Create an NSData from a property list. The format can be either NSPropertyListXMLFormat_v1_0 or NSPropertyListBinaryFormat_v1_0. The options parameter is currently unused and should be set to 0. If an error occurs the return value will be nil and the error parameter (if non-NULL) set to an autoreleased NSError describing the problem.
 */
+ (NSData *)dataWithPropertyList:(id)plist format:(NSPropertyListFormat)format options:(NSPropertyListWriteOptions)opt error:(NSError **)error AVAILABLE_MAC_OS_X_VERSION_10_6_AND_LATER;

/* Write a property list to an output stream. The stream should be opened and configured. The format can be either NSPropertyListXMLFormat_v1_0 or NSPropertyListBinaryFormat_v1_0. The options parameter is currently unused and should be set to 0. If an error occurs the return value will be 0 and the error parameter (if non-NULL) set to an autoreleased NSError describing the problem. In a successful case, the return value is the number of bytes written to the stream.
 */
+ (NSInteger)writePropertyList:(id)plist toStream:(NSOutputStream *)stream format:(NSPropertyListFormat)format options:(NSPropertyListWriteOptions)opt error:(NSError **)error AVAILABLE_MAC_OS_X_VERSION_10_6_AND_LATER;

/* Create a property list from an NSData. The options can be any of NSPropertyListMutabilityOptions. If the format parameter is non-NULL, it will be filled out with the format that the property list was stored in. If an error occurs the return value will be nil and the error parameter (if non-NULL) set to an autoreleased NSError describing the problem.
 */
+ (id)propertyListWithData:(NSData *)data options:(NSPropertyListReadOptions)opt format:(NSPropertyListFormat *)format error:(NSError **)error AVAILABLE_MAC_OS_X_VERSION_10_6_AND_LATER;

/* Create a property list by reading from an NSInputStream. The options can be any of NSPropertyListMutabilityOptions. If the format parameter is non-NULL, it will be filled out with the format that the property list was stored in. If an error occurs the return value will be nil and the error parameter (if non-NULL) set to an autoreleased NSError describing the problem.
 */
+ (id)propertyListWithStream:(NSInputStream *)stream options:(NSPropertyListReadOptions)opt format:(NSPropertyListFormat *)format error:(NSError **)error AVAILABLE_MAC_OS_X_VERSION_10_6_AND_LATER;


/* This method is obsolete and will be deprecated soon. Use dataWithPropertyList:format:options:error: instead.
 */
+ (NSData *)dataFromPropertyList:(id)plist format:(NSPropertyListFormat)format errorDescription:(NSString **)errorString;

/* This method is obsolete and will be deprecated soon. Use propertyListWithData:options:format:error: instead.
 */
+ (id)propertyListFromData:(NSData *)data mutabilityOption:(NSPropertyListMutabilityOptions)opt format:(NSPropertyListFormat *)format errorDescription:(NSString **)errorString;

@end

#endif

