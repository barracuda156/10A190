/* CoreGraphics - CGWindowLevel.h
 * Copyright (c) 2000-2008 Apple Inc.
 * All rights reserved. */

#ifndef __CGWINDOWLEVEL_H__
#define __CGWINDOWLEVEL_H__ 1

#include <CoreGraphics/CGBase.h>
#include <stdint.h>

/* Windows may be assigned to a particular level. When assigned to a level,
 * the window is ordered relative to all other windows in that level.
 * Windows with a higher level are sorted in front of windows with a lower
 * level.
 *
 * A common set of window levels is defined here for use within higher level
 * frameworks. The levels are accessed via a key and function, so that
 * levels may be changed or adjusted in future releases without breaking
 * binary compatability.
 */

typedef int32_t CGWindowLevel;
typedef int32_t CGWindowLevelKey;

enum _CGCommonWindowLevelKey {
    kCGBaseWindowLevelKey		= 0,
    kCGMinimumWindowLevelKey,
    kCGDesktopWindowLevelKey,
    kCGBackstopMenuLevelKey,
    kCGNormalWindowLevelKey,
    kCGFloatingWindowLevelKey,
    kCGTornOffMenuWindowLevelKey,
    kCGDockWindowLevelKey,
    kCGMainMenuWindowLevelKey,
    kCGStatusWindowLevelKey,
    kCGModalPanelWindowLevelKey,
    kCGPopUpMenuWindowLevelKey,
    kCGDraggingWindowLevelKey,
    kCGScreenSaverWindowLevelKey,
    kCGMaximumWindowLevelKey,
    kCGOverlayWindowLevelKey,
    kCGHelpWindowLevelKey,
    kCGUtilityWindowLevelKey,
    kCGDesktopIconWindowLevelKey,
    kCGCursorWindowLevelKey,
    kCGAssistiveTechHighWindowLevelKey,
    kCGNumberOfWindowLevelKeys	/* Internal bookkeeping; must be last */
};

CG_EXTERN CGWindowLevel CGWindowLevelForKey(CGWindowLevelKey key)
    CG_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);

/* number of levels above kCGMaximumWindowLevel reserved for internal use */
#define kCGNumReservedWindowLevels	(16)

/* Definitions of older constant values as calls */
#define kCGBaseWindowLevel		CGWindowLevelForKey(kCGBaseWindowLevelKey)	/* INT32_MIN */
#define kCGMinimumWindowLevel 		CGWindowLevelForKey(kCGMinimumWindowLevelKey)	/* (kCGBaseWindowLevel + 1) */
#define kCGDesktopWindowLevel		CGWindowLevelForKey(kCGDesktopWindowLevelKey)	/* kCGMinimumWindowLevel */
#define kCGDesktopIconWindowLevel		CGWindowLevelForKey(kCGDesktopIconWindowLevelKey)	/* kCGMinimumWindowLevel + 20 */
#define kCGBackstopMenuLevel		CGWindowLevelForKey(kCGBackstopMenuLevelKey)	/* -20 */
#define kCGNormalWindowLevel		CGWindowLevelForKey(kCGNormalWindowLevelKey)	/* 0 */
#define kCGFloatingWindowLevel		CGWindowLevelForKey(kCGFloatingWindowLevelKey)	/* 3 */
#define kCGTornOffMenuWindowLevel	CGWindowLevelForKey(kCGTornOffMenuWindowLevelKey)	/* 3 */
#define kCGDockWindowLevel		CGWindowLevelForKey(kCGDockWindowLevelKey)	/* 20 */
#define kCGMainMenuWindowLevel		CGWindowLevelForKey(kCGMainMenuWindowLevelKey)	/* 24 */
#define kCGStatusWindowLevel		CGWindowLevelForKey(kCGStatusWindowLevelKey)	/* 25 */
#define kCGModalPanelWindowLevel	CGWindowLevelForKey(kCGModalPanelWindowLevelKey)	/* 8 */
#define kCGPopUpMenuWindowLevel		CGWindowLevelForKey(kCGPopUpMenuWindowLevelKey)	/* 101 */
#define kCGDraggingWindowLevel		CGWindowLevelForKey(kCGDraggingWindowLevelKey)	/* 500 */
#define kCGScreenSaverWindowLevel	CGWindowLevelForKey(kCGScreenSaverWindowLevelKey)	/* 1000 */
#define kCGCursorWindowLevel		CGWindowLevelForKey(kCGCursorWindowLevelKey)	/* 2000 */
#define kCGOverlayWindowLevel		CGWindowLevelForKey(kCGOverlayWindowLevelKey)	/* 102 */
#define kCGHelpWindowLevel		CGWindowLevelForKey(kCGHelpWindowLevelKey)	/* 200 */
#define kCGUtilityWindowLevel		CGWindowLevelForKey(kCGUtilityWindowLevelKey)	/* 19 */

#define kCGAssistiveTechHighWindowLevel		CGWindowLevelForKey(kCGAssistiveTechHighWindowLevelKey)	/* 1500 */

#define kCGMaximumWindowLevel 		CGWindowLevelForKey(kCGMaximumWindowLevelKey)	/* INT32_MAX - kCGNumReservedWindowLevels */

#endif /* __CGWINDOWLEVEL_H__ */
