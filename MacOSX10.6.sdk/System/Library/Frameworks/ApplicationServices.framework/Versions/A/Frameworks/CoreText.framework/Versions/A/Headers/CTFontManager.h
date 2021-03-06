/*
 *  CTFontManager.h
 *  CoreText
 *
 *  Copyright (c) 2008 Apple Inc. All rights reserved.
 *
 */

/*!
    @header

    Thread Safety Information

    All functions in this header are thread safe unless otherwise specified.
*/

#ifndef __CTFONTMANAGER__
#define __CTFONTMANAGER__

#include <CoreText/CTFontDescriptor.h>
#include <CoreText/CTFontManagerErrors.h>

#include <CoreFoundation/CoreFoundation.h>

#if defined(__cplusplus)
extern "C" {
#endif

/*!
    @function   CTFontManagerCopyAvailablePostScriptNames
    @abstract   Returns an array of unique PostScript font names.

    @result     This function returns a retained reference to a CFArray of CFString references, or NULL on error. The caller is responsible for releasing the array.
*/
CFArrayRef CTFontManagerCopyAvailablePostScriptNames( void ) AVAILABLE_MAC_OS_X_VERSION_10_6_AND_LATER;

/*!
    @function   CTFontManagerCopyAvailableFontFamilyNames
    @abstract   Returns an array of visible font family names sorted for UI display.

    @result     This function returns a retained reference to a CFArray of CFString references, or NULL on error. The caller is responsible for releasing the array.
*/
CFArrayRef CTFontManagerCopyAvailableFontFamilyNames( void ) AVAILABLE_MAC_OS_X_VERSION_10_6_AND_LATER;

/*!
    @function   CTFontManagerCopyAvailableFontURLs
    @abstract   Returns an array of font URLs.

    @result     This function returns a retained reference to a CFArray of CFURL references, or NULL on error. The caller is responsible for releasing the array.
*/
CFArrayRef CTFontManagerCopyAvailableFontURLs( void ) AVAILABLE_MAC_OS_X_VERSION_10_6_AND_LATER;

/*!
    @function   CTFontManagerCompareFontFamilyNames
    @abstract   A CFComparatorFunction to compare font family names and sort them according to Apple guidelines.
    @discussion This function compares font family names and sorts them in the Apple preferred order, accounting for foundry prefix. Family names with recognized prefixes are sorted after the un-prefixed names in prefix order.
    @param      family1
                The first font family name, as CFStringRef.
    @param      family2
                The first font family name, as CFStringRef.
    @param      context
                Unused. Can be NULL.
    @result     A CFComparisonResult value indicating the sort order for the two family names. kCFComparisonResultGreatherThan if family1 is greater than family2, kCFComparisonResultLessThan if family1 is less than family2, and kCFComparisonResultEqualTo if they are equal.
*/
CFComparisonResult CTFontManagerCompareFontFamilyNames(
    const void *        family1,
    const void *        family2,
    void *              context) AVAILABLE_MAC_OS_X_VERSION_10_6_AND_LATER;

/*!
    @function   CTFontManagerCreateFontDescriptorsFromURL
    @abstract   Returns an array of font descriptors representing each of the fonts in the specified URL.

    @param      fileURL
                A file system URL referencing a valid font file.

    @result     This function returns a retained reference to a CFArray, or NULL on error. The caller is responsible for releasing the array.
*/
CFArrayRef CTFontManagerCreateFontDescriptorsFromURL(
    CFURLRef            fileURL ) AVAILABLE_MAC_OS_X_VERSION_10_6_AND_LATER;

/*!
    @enum       CTFontManagerScope
    @abstract   Scope for font registration.
    @constant   kCTFontManagerScopeProcess
                The font is available to the current process for the duration of the process unless directly unregistered.
    @constant   kCTFontManagerScopeUser
                The font is available to all processes for the current user session and will be available in subsequent sessions unless unregistered.
    @constant   kCTFontManagerScopeSession
                The font is available to the current user session, and will not be available in subsequent sessions.
*/
enum {
    kCTFontManagerScopeNone         = 0,
    kCTFontManagerScopeProcess      = 1,
    kCTFontManagerScopeUser         = 2,
    kCTFontManagerScopeSession      = 3
};
typedef uint32_t CTFontManagerScope;

/*!
    @function   CTFontManagerRegisterFontsForURLs
    @abstract   Registers fonts from the specified font URL with the font manager. Registered fonts are discoverable through font descriptor matching.

    @param      fontURL
                Font URL.

    @param      scope
                Scope constant defining the availability and lifetime of the registration. See scope constants for more details.

    @param      error
                Pointer to receive CFError in the case of failed registration.

    @result     Returns true if registration of the fonts was successful.
*/
bool CTFontManagerRegisterFontsForURL(
    CFURLRef                fontURL,
    CTFontManagerScope      scope,
    CFErrorRef *            error ) AVAILABLE_MAC_OS_X_VERSION_10_6_AND_LATER;

/*!
    @function   CTFontManagerUnregisterFontsForURL
    @abstract   Unregisters fonts from the specified font URL with the font manager. Unregistered fonts are no longer discoverable through font descriptor matching.

    @param      fontURL
                Font URL.

    @param      scope
                Scope constant defining the availability and lifetime of the registration. Should match the scope the fonts are registered in. See scope constants for more details.

    @param      error
                Pointer to receive CFError in the case of failed unregistration.

    @result     Returns true if unregistration of the fonts was successful.

*/
bool CTFontManagerUnregisterFontsForURL(
    CFURLRef                fontURL,
    CTFontManagerScope      scope,
    CFErrorRef *            error ) AVAILABLE_MAC_OS_X_VERSION_10_6_AND_LATER;

/*!
    @function   CTFontManagerRegisterFontsForURLs
    @abstract   Registers fonts from the specified font URLs with the font manager. Registered fonts are discoverable through font descriptor matching.

    @param      fontURLs
                Array of font URLs.

    @param      scope
                Scope constant defining the availability and lifetime of the registration. See scope constants for more details.

    @param      errors
                Pointer to CFArrayRef to receive array of CFError references. Each error will contain a CFArray of font URLs corresponding to kCTFontManagerErrorFontURLsKey. These URLs represent the font files that caused the error, and were not successfully registered. Must be released by caller. Can be NULL.

    @result     Returns true if registration of all font URLs was successful. Otherwise false.
*/
bool CTFontManagerRegisterFontsForURLs(
    CFArrayRef              fontURLs,
    CTFontManagerScope      scope,
    CFArrayRef *            errors ) AVAILABLE_MAC_OS_X_VERSION_10_6_AND_LATER;

/*!
    @function   CTFontManagerUnregisterFontsForURLs
    @abstract   Unregisters fonts from the specified font URLs with the font manager. Unregistered fonts are no longer discoverable through font descriptor matching.

    @param      fontURLs
                Array of font URLs.

    @param      scope
                Scope constant defining the availability and lifetime of the registration. Should match the scope the fonts are registered in. See scope constants for more details.

    @param      errors
                Pointer to CFArrayRef to receive array of CFError references. Each error will contain a CFArray of font URLs corresponding to kCTFontManagerErrorFontURLsKey. These URLs represent the font files that caused the error, and were not successfully unregistered. Must be released by caller. Can be NULL.

    @result     Returns true if unregistration of all font URLs was successful. Otherwise false.
*/
bool CTFontManagerUnregisterFontsForURLs(
    CFArrayRef              fontURLs,
    CTFontManagerScope      scope,
    CFArrayRef *            errors ) AVAILABLE_MAC_OS_X_VERSION_10_6_AND_LATER;

/*!
    @function   CTFontManagerEnableFontDescriptors
    @abstract   Enables or disables the matching font descriptors for font descriptor matching.

    @param      descriptors
                Array of font descriptors.

    @param      enable
                Boolean value indicating whether the fonts matching descriptors should be enabled for font descriptor matching.
*/
void CTFontManagerEnableFontDescriptors(
    CFArrayRef              descriptors,
    bool                    enable ) AVAILABLE_MAC_OS_X_VERSION_10_6_AND_LATER;

/*!
    @function   CTFontManagerGetScopeForURL
    @abstract   Returns the registration scope of the specified URL.

    @param      fontURL
                Font URL.

    @result     Returns the registration scope of the specified URL, will return kCTFontManagerScopeNone if not currently registered.
*/
CTFontManagerScope CTFontManagerGetScopeForURL(
    CFURLRef                fontURL ) AVAILABLE_MAC_OS_X_VERSION_10_6_AND_LATER;

/*!
    @function   CTFontManagerIsSupportedFontFile
    @abstract   Determines whether the referenced font data (usually by file URL) is supported on the current platform.

    @param      fontURL
                A URL to font data.

    @result     This function returns true if the URL represents a valid font that can be used on the current platform.
*/
bool CTFontManagerIsSupportedFont(
    CFURLRef                fontURL ) AVAILABLE_MAC_OS_X_VERSION_10_6_AND_LATER;

/*! --------------------------------------------------------------------------
    @group Manager Auto-Activation
*///--------------------------------------------------------------------------

#if defined(__BLOCKS__)
/*!
    @function   CTFontManagerCreateFontRequestRunLoopSource
    @abstract   Creates a CFRunLoopSourceRef that will be used to convey font requests from CTFontManager.
    @param      sourceOrder
                The order of the created run loop source.
    @param      createMatchesCallback
                A block to handle the font request.
    @result     A CFRunLoopSourceRef that should be added to the run loop. To stop receiving requests, invalidate this run loop source. Will return NULL on error, in the case of a duplicate requestPortName, or invalid context structure.
*/
CFRunLoopSourceRef CTFontManagerCreateFontRequestRunLoopSource(
    CFIndex                         sourceOrder,
    CFArrayRef (^createMatchesCallback)(CTFontDescriptorRef requestDescriptor)) AVAILABLE_MAC_OS_X_VERSION_10_6_AND_LATER;
#endif

/*!
    @const      kCTFontManagerBundleIdentifier
    @abstract   CTFontManage bundle identifier
    @discussion The CTFontManager bundle identifier to be used with get or set global auto-activation settings.
*/
extern const CFStringRef kCTFontManagerBundleIdentifier AVAILABLE_MAC_OS_X_VERSION_10_6_AND_LATER;

/*!
    @enum
    @abstract   Auto-activation settings.
    @constant   kCTFontManagerAutoActivationDefault
                Default auto-activation setting. When specified, the application will use the global setting.
    @constant   kCTFontManagerAutoActivationDisabled
                Disables auto-activation.
    @constant   kCTFontManagerAutoActivationEnabled
                Enables auto-activation.
    @constant   kCTFontManagerAutoActivationPromptUser
                Requires user input for auto-activation. A dialog will be presented to the user to confirm auto
                activation of the font.
*/
enum {
    kCTFontManagerAutoActivationDefault     = 0,
    kCTFontManagerAutoActivationDisabled    = 1,
    kCTFontManagerAutoActivationEnabled     = 2,
    kCTFontManagerAutoActivationPromptUser  = 3
};
typedef uint32_t CTFontManagerAutoActivationSetting;

/*!
    @function   CTFontManagerSetAutoActivationSetting
    @abstract   Sets the auto-activation for the specified bundle identifier.
    @param      bundleIdentifier
                The bundle identifier. Used to specify a particular application bundle. If NULL,
                the current application bundle will be used. If kCTFontManagerBundleIdentifier is specified,
                will set the global auto-activation settings.
    @param      setting
                The new setting.
    @result     Function will apply the setting to the appropriate preferences location.
*/
void CTFontManagerSetAutoActivationSetting(
    CFStringRef                         bundleIdentifier,
    CTFontManagerAutoActivationSetting  setting ) AVAILABLE_MAC_OS_X_VERSION_10_6_AND_LATER;

/*!
    @function   CTFontManagerGetAutoActivationSetting
    @abstract   Accessor for the auto-activation setting.
    @param      bundleIdentifier
                The bundle identifier. Used to specify a particular application bundle. If NULL,
                the current application bundle will be used. If kCTFontManagerBundleIdentifier is specified,
                will set the global auto-activation settings.
    @result     Will return the auto-activation setting for specified bundle identifier.
*/
CTFontManagerAutoActivationSetting CTFontManagerGetAutoActivationSetting(
    CFStringRef                         bundleIdentifier ) AVAILABLE_MAC_OS_X_VERSION_10_6_AND_LATER;

/*! --------------------------------------------------------------------------
    @group Manager Notifications
*///--------------------------------------------------------------------------

/*!
    @constant   kCTFontManagerRegisteredFontsChangedNotification
    @abstract   Notification name for font registry changes.
    @discussion This is the string to use as the notification name when subscribing
                to CTFontManager notifications. This notification will be posted
                when fonts are added to the font registry. The client is responsible for
                registered with the distributed notification center to receive notifications
                for changes to the session or user scopes, and with a local notification
                for changes to the process scope.
*/
extern const CFStringRef kCTFontManagerRegisteredFontsChangedNotification AVAILABLE_MAC_OS_X_VERSION_10_6_AND_LATER;

#if defined(__cplusplus)
}
#endif

#endif
