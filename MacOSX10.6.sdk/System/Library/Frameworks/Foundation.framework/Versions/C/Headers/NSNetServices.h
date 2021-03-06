/*	NSNetServices.h
        Copyright (c) 2002-2008, Apple Inc. All rights reserved.
*/

#import <Foundation/NSObject.h>
#import <Foundation/NSDate.h>

#if MAC_OS_X_VERSION_10_2 <= MAC_OS_X_VERSION_MAX_ALLOWED

@class NSArray, NSData, NSDictionary, NSInputStream, NSOutputStream, NSRunLoop, NSString;
@protocol NSNetServiceDelegate, NSNetServiceBrowserDelegate;

#pragma mark Error constants

FOUNDATION_EXPORT NSString * const NSNetServicesErrorCode;
FOUNDATION_EXPORT NSString * const NSNetServicesErrorDomain;

enum {
    
/* An unknown error occured during resolution or publication.
*/
    NSNetServicesUnknownError = -72000,
    
/* An NSNetService with the same domain, type and name was already present when the publication request was made.
*/
    NSNetServicesCollisionError = -72001,
    
/* The NSNetService was not found when a resolution request was made.
*/
    NSNetServicesNotFoundError	= -72002,
    
/* A publication or resolution request was sent to an NSNetService instance which was already published or a search request was made of an NSNetServiceBrowser instance which was already searching.
*/
    NSNetServicesActivityInProgress = -72003,
    
/* An required argument was not provided when initializing the NSNetService instance.
*/
    NSNetServicesBadArgumentError = -72004,
    
/* The operation being performed by the NSNetService or NSNetServiceBrowser instance was cancelled.
*/
    NSNetServicesCancelledError = -72005,
    
/* An invalid argument was provided when initializing the NSNetService instance or starting a search with an NSNetServiceBrowser instance.
*/
    NSNetServicesInvalidError = -72006,
    
#if MAC_OS_X_VERSION_10_4 <= MAC_OS_X_VERSION_MAX_ALLOWED
    
/* Resolution of an NSNetService instance failed because the timeout was reached.
*/
    NSNetServicesTimeoutError = -72007,
    
#endif

};
typedef NSInteger NSNetServicesError;


enum {
/* When passed to -publishWithOptions, this suppresses the auto-renaming of an NSNetService in the event of a name collision. The collision is reported to the instance's delegate on the netService:didNotPublish: method.
*/
    NSNetServiceNoAutoRename = 1 << 0
};
typedef NSUInteger NSNetServiceOptions;


#pragma mark -

@interface NSNetService : NSObject {
@private
    id _netService;
    id _delegate;
    id _reserved;
}

/* This is the initializer for publishing. You should use this initializer if you are going to announce the availability of a service on the network. To publish a service in all available domains, pass the empty string as the domain.
*/
- (id)initWithDomain:(NSString *)domain type:(NSString *)type name:(NSString *)name port:(int)port;

/* This is the initializer for resolution. If you know the domain, type and name of the service for which you wish to discover addresses, you should initialize an NSNetService instance using this method and call resolve: on the result. If you wish to connect to this service immediately, you should call getInputStream:getOutputStream: on the result and forego the resolution step entirely.

If publish: is called on an NSNetService instance initialized with this method, an NSNetServicesBadArgumentError will be sent in the error dictionary to the delegate's netService:didNotPublish: method.
*/
- (id)initWithDomain:(NSString *)domain type:(NSString *)type name:(NSString *)name;

- (id <NSNetServiceDelegate>)delegate;
- (void)setDelegate:(id <NSNetServiceDelegate>)delegate;

/* NSNetService instances may be scheduled on NSRunLoops to operate in different modes, or in other threads. It is generally not necessary to schedule NSNetServices in other threads. NSNetServices are scheduled in the current thread's NSRunLoop in the NSDefaultRunLoopMode when they are created.
*/
- (void)scheduleInRunLoop:(NSRunLoop *)aRunLoop forMode:(NSString *)mode;
- (void)removeFromRunLoop:(NSRunLoop *)aRunLoop forMode:(NSString *)mode;

/* Returns the domain of the discovered or published service.
*/
- (NSString *)domain;

/* Returns the type of the discovered or published service.
*/
- (NSString *)type;

/* Returns the name of the discovered or published service.
*/
- (NSString *)name;

/* The addresses of the service. This is an NSArray of NSData instances, each of which contains a single struct sockaddr suitable for use with connect(2). In the event that no addresses are resolved for the service or the service has not yet been resolved, an empty NSArray is returned.
*/
- (NSArray *)addresses;

#if MAC_OS_X_VERSION_10_5 <= MAC_OS_X_VERSION_MAX_ALLOWED

/* The port of a resolved service. This returns -1 if the service has not been resolved. */
- (NSInteger)port;

#endif
 
/* Advertises a given service on the network. This method returns immediately. Success or failure is indicated by callbacks to the NSNetService instance's delegate.
 
    If the name of the service is the default name (@""), then the service will be renamed automatically. If the name of the service has been specified, then the service will not be renamed automatically. If more control over renaming is required, then -[NSNetService publishWithOptions:] is available.
*/
- (void)publish;

#if MAC_OS_X_VERSION_10_5 <= MAC_OS_X_VERSION_MAX_ALLOWED

/* Advertises a given service on the network. This method returns immediately. Success or failure is indicated by callbacks to the NSNetService instance's delegate.
 
    See the notes above for NSNetServiceNoAutoRename for information about controlling the auto-renaming behavior using this method.
*/
- (void)publishWithOptions:(NSNetServiceOptions)options;

#endif

/* Attempts to determine at least one address for the NSNetService instance. For applications linked on or after Mac OS X 10.4 "Tiger", this method calls -resolveWithTimeout: with a value of 5.0. Applications linked prior to Mac OS X 10.4 "Tiger" must call -stop on the instance after an appropriate (short) amount of time to avoid causing unnecessary network traffic.
*/
- (void)resolve DEPRECATED_IN_MAC_OS_X_VERSION_10_4_AND_LATER;

/* Halts a service which is either publishing or resolving.
*/
- (void)stop;

#if MAC_OS_X_VERSION_10_4 <= MAC_OS_X_VERSION_MAX_ALLOWED

/* Returns an NSDictionary created from the provided NSData. The keys will be UTF8-encoded NSStrings. The values are NSDatas. The caller is responsible for interpreting these as types appropriate to the keys. If the NSData cannot be converted into an appropriate NSDictionary, this method will return nil. For applications linked on or after Mac OS X 10.5, this method will throw an NSInvalidException if it is passed nil as the argument.
*/
+ (NSDictionary *)dictionaryFromTXTRecordData:(NSData *)txtData;

/* Returns an NSData created from the provided dictionary. The keys in the provided dictionary must be NSStrings, and the values must be NSDatas. If the dictionary cannot be converted into an NSData suitable for a TXT record, this method will return nil. For applications linked on or after Mac OS X 10.5, this method will throw an NSInvalidArgumentException if it is passed nil as the argument.
*/
+ (NSData *)dataFromTXTRecordDictionary:(NSDictionary *)txtDictionary;

/* Returns the DNS host name of the computer hosting the discovered or published service. If a successful resolve has not yet occurred, this method will return nil.
*/
- (NSString *)hostName;

/* Starts a resolve for the NSNetService instance of the specified duration. If the delegate's -netServiceDidResolveAddress: method is called before the timeout expires, the resolve is successful. If the timeout is reached, the delegate's -netService:didNotResolve: method will be called. The value of the NSNetServicesErrorCode key in the error dictionary will be NSNetServicesTimeoutError.
*/
- (void)resolveWithTimeout:(NSTimeInterval)timeout;

/* Retrieves streams from the NSNetService instance. The instance's delegate methods are not called. Returns YES if the streams requested are created successfully. Returns NO if or any reason the stream could not be created. If only one stream is desired, pass NULL for the address of the other stream. The streams that are created are not open, and are not scheduled in any run loop for any mode.
*/
- (BOOL)getInputStream:(NSInputStream **)inputStream outputStream:(NSOutputStream **)outputStream;

/* Sets the TXT record of the NSNetService instance that has been or will be published. Pass nil to remove the TXT record from the instance.
*/
- (BOOL)setTXTRecordData:(NSData *)recordData;

/* Returns the raw TXT record of the NSNetService instance. If the instance has not been resolved, or the delegate's -netService:didUpdateTXTRecordData: has not been called, this will return nil. It is permitted to have a zero-length TXT record.
*/
- (NSData *)TXTRecordData;

/* Starts monitoring the NSNetService instance for events. In Mac OS X 10.4 Tiger, monitored NSNetService instances inform their delegates of changes to the instance's TXT record by calling the delegate's -netService:didUpdateTXTRecordData: method.
*/
- (void)startMonitoring;

/* Stops monitoring the NSNetService instance for events.
*/
- (void)stopMonitoring;

#endif

@end

#pragma mark -

@interface NSNetServiceBrowser : NSObject {
@private
    id _netServiceBrowser;
    id _delegate;
    void * _reserved;
}

- (id)init;

- (id <NSNetServiceBrowserDelegate>)delegate;
- (void)setDelegate:(id <NSNetServiceBrowserDelegate>)delegate;

/* NSNetServiceBrowser instances may be scheduled on NSRunLoops to operate in different modes, or in other threads. It is generally not necessary to schedule NSNetServiceBrowsers in other threads. NSNetServiceBrowsers are scheduled in the current thread's NSRunLoop in the NSDefaultRunLoopMode when they are created.
*/
- (void)scheduleInRunLoop:(NSRunLoop *)aRunLoop forMode:(NSString *)mode;
- (void)removeFromRunLoop:(NSRunLoop *)aRunLoop forMode:(NSString *)mode;

#if MAC_OS_X_VERSION_10_4 <= MAC_OS_X_VERSION_MAX_ALLOWED

/* Starts a search for domains that are browsable via Bonjour and the computer's network configuration. Discovered domains are reported to the delegate's -netServiceBrowser:didFindDomain:moreComing: method. There may be more than one browsable domain.
*/
- (void)searchForBrowsableDomains;

#endif

/* Starts a search for domains in which the network configuration allows registration (i.e. publishing). Most NSNetServiceBrowser clients do not need to use this API, as it is sufficient to publish an NSNetService instance with the empty string (see -[NSNetService initWithDomain:type:name:port:]). Discovered domains are reported to the delegate's -netServiceBrowser:didFindDomain:moreComing: method. There may be more than one registration domain.
*/
- (void)searchForRegistrationDomains;

/* Starts a search for services of the specified type in the domain indicated by domainString. For each service discovered, a -netServiceBrowser:foundService:moreComing: message is sent to the NSNetServiceBrowser instance's delegate.
*/
- (void)searchForServicesOfType:(NSString *)type inDomain:(NSString *)domainString;

/* Stops the currently running search.
*/
- (void)stop;

@end

#pragma mark -

@protocol NSNetServiceDelegate <NSObject>
@optional

/* Sent to the NSNetService instance's delegate prior to advertising the service on the network. If for some reason the service cannot be published, the delegate will not receive this message, and an error will be delivered to the delegate via the delegate's -netService:didNotPublish: method.
*/
- (void)netServiceWillPublish:(NSNetService *)sender;

#if MAC_OS_X_VERSION_10_4 <= MAC_OS_X_VERSION_MAX_ALLOWED

/* Sent to the NSNetService instance's delegate when the publication of the instance is complete and successful.
*/
- (void)netServiceDidPublish:(NSNetService *)sender;

#endif

/* Sent to the NSNetService instance's delegate when an error in publishing the instance occurs. The error dictionary will contain two key/value pairs representing the error domain and code (see the NSNetServicesError enumeration above for error code constants). It is possible for an error to occur after a successful publication.
*/
- (void)netService:(NSNetService *)sender didNotPublish:(NSDictionary *)errorDict;

/* Sent to the NSNetService instance's delegate prior to resolving a service on the network. If for some reason the resolution cannot occur, the delegate will not receive this message, and an error will be delivered to the delegate via the delegate's -netService:didNotResolve: method.
*/
- (void)netServiceWillResolve:(NSNetService *)sender;

/* Sent to the NSNetService instance's delegate when one or more addresses have been resolved for an NSNetService instance. Some NSNetService methods will return different results before and after a successful resolution. An NSNetService instance may get resolved more than once; truly robust clients may wish to resolve again after an error, or to resolve more than once.
*/
- (void)netServiceDidResolveAddress:(NSNetService *)sender;

/* Sent to the NSNetService instance's delegate when an error in resolving the instance occurs. The error dictionary will contain two key/value pairs representing the error domain and code (see the NSNetServicesError enumeration above for error code constants).
*/
- (void)netService:(NSNetService *)sender didNotResolve:(NSDictionary *)errorDict;

/* Sent to the NSNetService instance's delegate when the instance's previously running publication or resolution request has stopped.
*/
- (void)netServiceDidStop:(NSNetService *)sender;

#if MAC_OS_X_VERSION_10_4 <= MAC_OS_X_VERSION_MAX_ALLOWED

/* Sent to the NSNetService instance's delegate when the instance is being monitored and the instance's TXT record has been updated. The new record is contained in the data parameter.
*/
- (void)netService:(NSNetService *)sender didUpdateTXTRecordData:(NSData *)data;

#endif

@end

#pragma mark -

@protocol NSNetServiceBrowserDelegate <NSObject>
@optional

/* Sent to the NSNetServiceBrowser instance's delegate before the instance begins a search. The delegate will not receive this message if the instance is unable to begin a search. Instead, the delegate will receive the -netServiceBrowser:didNotSearch: message.
*/
- (void)netServiceBrowserWillSearch:(NSNetServiceBrowser *)aNetServiceBrowser;

/* Sent to the NSNetServiceBrowser instance's delegate when the instance's previous running search request has stopped.
*/
- (void)netServiceBrowserDidStopSearch:(NSNetServiceBrowser *)aNetServiceBrowser;

/* Sent to the NSNetServiceBrowser instance's delegate when an error in searching for domains or services has occurred. The error dictionary will contain two key/value pairs representing the error domain and code (see the NSNetServicesError enumeration above for error code constants). It is possible for an error to occur after a search has been started successfully.
*/
- (void)netServiceBrowser:(NSNetServiceBrowser *)aNetServiceBrowser didNotSearch:(NSDictionary *)errorDict;

/* Sent to the NSNetServiceBrowser instance's delegate for each domain discovered. If there are more domains, moreComing will be YES. If for some reason handling discovered domains requires significant processing, accumulating domains until moreComing is NO and then doing the processing in bulk fashion may be desirable.
*/
- (void)netServiceBrowser:(NSNetServiceBrowser *)aNetServiceBrowser didFindDomain:(NSString *)domainString moreComing:(BOOL)moreComing;

/* Sent to the NSNetServiceBrowser instance's delegate for each service discovered. If there are more services, moreComing will be YES. If for some reason handling discovered services requires significant processing, accumulating services until moreComing is NO and then doing the processing in bulk fashion may be desirable.
*/
- (void)netServiceBrowser:(NSNetServiceBrowser *)aNetServiceBrowser didFindService:(NSNetService *)aNetService moreComing:(BOOL)moreComing;

/* Sent to the NSNetServiceBrowser instance's delegate when a previously discovered domain is no longer available.
*/
- (void)netServiceBrowser:(NSNetServiceBrowser *)aNetServiceBrowser didRemoveDomain:(NSString *)domainString moreComing:(BOOL)moreComing;

/* Sent to the NSNetServiceBrowser instance's delegate when a previously discovered service is no longer published.
*/
- (void)netServiceBrowser:(NSNetServiceBrowser *)aNetServiceBrowser didRemoveService:(NSNetService *)aNetService moreComing:(BOOL)moreComing;

@end

#pragma mark -
#pragma mark Deprecated API

#if (TARGET_OS_MAC && !(TARGET_OS_EMBEDDED || TARGET_OS_ASPEN))

/* Methods in these categories are provided for binary compatibility only.
*/

@interface NSNetService (NSDeprecated)

/* Returns an NSString representing the TXT record or nil if there is none. This string may or may not be the full TXT record.

This method is deprecated on Mac OS X 10.4 "Tiger" and later; use -TXTRecordData instead.
*/
- (NSString *)protocolSpecificInformation DEPRECATED_IN_MAC_OS_X_VERSION_10_4_AND_LATER;

/* Sets the TXT record of the NSNetService instance to be the provided string. It is the caller's responsibility to ensure the string is of the appropriate format with the correct encoding.

This method is deprecated on Mac OS X 10.4 "Tiger" and later; use -setTXTRecordData: instead.
*/
- (void)setProtocolSpecificInformation:(NSString *)specificInformation DEPRECATED_IN_MAC_OS_X_VERSION_10_4_AND_LATER;

@end

@interface NSNetServiceBrowser (NSDeprecated)

/* Starts a search for any domain visible to the host based on network configuration. Discovered domains are reported to the delegate's -netServiceBrowser:didFindDomain:moreComing: method. These may be domains in which it is not possible to register.

This method is deprecated on Mac OS X 10.4 "Tiger" and later; use -searchForBrowsableDomains or -searchForRegistrationDomains instead.
*/
- (void)searchForAllDomains DEPRECATED_IN_MAC_OS_X_VERSION_10_4_AND_LATER;

@end

#endif

#endif
