/*
	NSToolbarItemGroup.h
	Application Kit
	Copyright (c) 2000-2008, Apple Inc.
	All rights reserved.
*/

#import <AppKit/NSToolbarItem.h>
#import <AppKit/AppKit.h>
#import <Foundation/Foundation.h>

#if MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_5

/* NSToolbarItemGroup is a subclass of NSToolbarItem which can be used to create sets of NSToolbarItems that are always attached to one another and that are added, removed, or reordered as a single unit.  Properties that get set on the parent toolbar item, such as label or view, apply to the entire item.  Otherwise, the individual properties are displayed adjacent to one another.  */

@interface NSToolbarItemGroup : NSToolbarItem {
    @private
    id _groupItems;
    struct {
      unsigned dirtiedLayout:1;
      unsigned reserved:31;
    } _giFlags;
    id _giReserved;
}

/* Set or get the array of subitems for the toolbar item.  By deafult, a NSToolbarItemGroup has an empty array of subitems.  You should call this to set the subitems before returning the item to the toolbar.  NSToolbarItemGroups may not contain other NSToolbarItemGroups as subitems.
*/
- (void)setSubitems:(NSArray *)subitems;
- (NSArray *)subitems;

@end

#endif
