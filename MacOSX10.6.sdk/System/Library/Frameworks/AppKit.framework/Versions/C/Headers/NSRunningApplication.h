/*
	NSRunningApplication.h
	Application Kit
	Copyright (c) 1994-2008, Apple Inc.
	All rights reserved.
*/



/* NSRunningApplication is a class to manipulate and provide information for a single instance of an application.  Only user applications are tracked; this does not provide information about every process on the system.
 
 Some properties of an application are fixed, such as the bundle identifier.  Other properties may vary over time, such as whether the app is hidden.  Properties that vary can be observed with KVO, in which case the description comment for the method will mention it.
 
 Properties that vary over time are inherently race-prone.  For example, a hidden app may unhide itself at any time.  To ameliorate this, properties persist until the next turn of the main run loop in a common mode.  For example, if you repeatedly poll an unhidden app for its hidden property without allowing the run loop to run, it will continue to return NO, even if the app hides, until the next turn of the run loop.
 
 NSRunningApplication is thread safe, in that its properties are returned atomically.  However, it is still subject to the main run loop policy described above.  If you access an instance of NSRunningApplication from a background thread, be aware that its time-varying properties may change from under you as the main run loop runs (or not).
 
 An NSRunningApplication instance remains valid after the application exits.  However, most properties lose their significance, and some properties may not be available on a terminated application.
 
 To access the list of all running applications, use the -runningApplications method on NSWorkspace.
*/

#import <Foundation/NSObject.h>
#import <AppKit/NSWorkspace.h>

/* The following flags are for activateWithOptions:. */
enum {
    /* By default, activation brings only the main and key windows forward.  If you specify NSApplicationActivateAllWindows, all of the application's windows are brought forward. */
    NSApplicationActivateAllWindows = 1 << 0,
    
    /* By default, activation deactivates the calling app (assuming it was active), and then the new app is activated only if there is no currently active application.  This prevents the new app from stealing focus from the user, if the app is slow to activate and the user has switched to a different app in the interim.  However, if you specify NSApplicationActivateIgnoringOtherApps, the application is activated regardless of the currently active app, potentially stealing focus from the user.
     
     You ALMOST NEVER want to pass this flag, because stealing key focus produces a very bad user experience. */
    NSApplicationActivateIgnoringOtherApps = 1 << 1
};
typedef NSUInteger NSApplicationActivationOptions;


/* The following activation policies control whether and how an application may be activated.  They are determined by the Info.plist. */
enum {
    /* The application is an ordinary app that appears in the Dock and may have a user interface.  This is the default for bundled apps, unless overridden in the Info.plist. */
    NSApplicationActivationPolicyRegular, 
    
    /* The application does not appear in the Dock and does not have a menu bar, but it may be activated programmatically or by clicking on one of its windows.  This corresponds to LSUIElement=1 in the Info.plist. */
    NSApplicationActivationPolicyAccessory,
    
    /* The application does not appear in the Dock and may not create windows or be activated.  This corresponds to LSBackgroundOnly=1 in the Info.plist.  This is also the default for unbundled executables that do not have Info.plists. */
    NSApplicationActivationPolicyProhibited
};
typedef NSInteger NSApplicationActivationPolicy;

#if MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_6

@class NSLock, NSDate, NSImage, NSURL;

@interface NSRunningApplication : NSObject {
    @private
    id _superReserved;
    __strong void *_asn;
    __strong void **_helpers;
    void *_obsInfo;
    NSLock *_lock;
    NSString *_bundleID;
    NSString *_localizedName;
    NSURL *_bundleURL;
    NSURL *_executableURL;
    NSDate *_launchDate;
    NSImage *_icon;
    int _pid;
    unsigned int _lastSeed;
    unsigned int _activeSeed;
    unsigned int _staleSeed;
    unsigned long long _obsMask;
    struct {
	unsigned fetchedDynamic:1;
	unsigned deadToUs:1;
        unsigned terminated:1;
        unsigned finishedLaunching:1;
        unsigned hidden:1;
        unsigned active:1;
	unsigned arch:3;
	unsigned activationPolicy:3;
        unsigned reserved1:20;
    } _aflags;
    id _appReserved;
}

/* Indicates that the process is an exited application.  This is observable through KVO. */
@property (readonly, getter=isTerminated) BOOL terminated;

/* Indicates that the process is finished launching, which corresponds to the NSApplicationDidFinishLaunching internal notification.  This is observable through KVO.  Some applications do not post this notification and so are never reported as finished launching. */
@property (readonly, getter=isFinishedLaunching) BOOL finishedLaunching;

/* Indicates whether the application is currently hidden.  This is observable through KVO. */
@property (readonly, getter=isHidden) BOOL hidden;

/* Indicates whether the application is currently frontmost.  This is observable through KVO. */
@property (readonly, getter=isActive) BOOL active;

/* Indicates the activation policy of the application.   This is observable through KVO (the type is usually fixed, but may be changed through a call to -[NSApplication setActivationPolicy:]).  */
@property (readonly) NSApplicationActivationPolicy activationPolicy;

/* Indicates the name of the application.  This is dependent on the current localization of the referenced app, and is suitable for presentation to the user. */
@property (readonly) NSString *localizedName;

/* Indicates the CFBundleIdentifier of the application, or nil if the application does not have an Info.plist. */
@property (readonly) NSString *bundleIdentifier;

/* Indicates the URL to the application's bundle, or nil if the application does not have a bundle. */
@property (readonly) NSURL *bundleURL;

/* Indicates the URL to the application's executable. */
@property (readonly) NSURL *executableURL;

/* Indicates the process identifier (pid) of the application.  Do not rely on this for comparing processes.  Use isEqual: instead.  Not all applications have a pid.  Applications without a pid return -1 from this method. */
@property (readonly) pid_t processIdentifier;

/* Indicates the date when the application was launched.  This property is not available for all applications.  Specifically, it is not avaialble for applications that were launched without going through LaunchServices.   */
@property (readonly) NSDate *launchDate;

/* Returns the icon of the application. */
@property (readonly) NSImage *icon;

/* Indicates the executing processor architecture for the application, as an NSBundleExecutableArchitecture from NSBundle.h. */
@property (readonly) NSInteger executableArchitecture;

/* Attempts to hide or unhide the receiver.  These methods return true if the request to hide or unhide was successfully sent, false if not (for example, if the application has quit, or is of a type that cannot be unhidden). */
- (BOOL)hide;
- (BOOL)unhide;

/* Attempts to activate the receiver.  This method return true if the request to activate was successfully sent, false if not (for example, if the application has quit, or is of a type that cannot be activated). */
- (BOOL)activateWithOptions:(NSApplicationActivationOptions)options;

/* Attempts to quit the receiver normally.  This method returns true if the request was successfully sent, false if not (for example, if the application is no longer running).  This method may return before the receiver exits; you should observe the exited property or listen for the notification to detect when the app has exited. */
- (BOOL)terminate;

/* Attempts to force the receiver to quit.  This method returns true if the request was successfully sent, false if not (for example, if the application is no longer running).  This method may return before the receiver exits; you should observe the exited property or listen for the notification to detect when the app has exited. */
- (BOOL)forceTerminate;

/* Returns an array of currently running applications with the given bundle identifier, or an empty array if no apps match. */
+ (NSArray *)runningApplicationsWithBundleIdentifier:(NSString *)bundleIdentifier;

/* Returns the running application with the given process identifier, or nil if no application has that pid.  Applications that do not have PIDs cannot be returned from this method. */
+ (NSRunningApplication *)runningApplicationWithProcessIdentifier:(pid_t)pid;

/* Returns an NSRunningApplication representing this application. */
+ (NSRunningApplication *)currentApplication;

@end

@interface NSWorkspace (NSWorkspaceRunningApplications)

/* Returns an array of NSRunningApplications representing currently running applications.  The order of the array is unspecified, but it is stable, meaning that the relative order of particular applications will not change across multiple calls to runningApplications.

 Similar to NSRunningApplication's properties, this property will only change when the main run loop is run in a common mode.  Instead of polling, use key-value observing to be notified of changes to this array property.
 
 This property is thread safe, in that it may be called from background threads and the result is returned atomically.  This property is observable through KVO.
 */
- (NSArray *)runningApplications AVAILABLE_MAC_OS_X_VERSION_10_6_AND_LATER;

@end

#endif
