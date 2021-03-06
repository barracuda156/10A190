/*
	NSSplitView.h
	Application Kit
	Copyright (c) 1994-2008, Apple Inc.
	All rights reserved.
*/

#import <AppKit/NSView.h>

@class NSNotification;
@protocol NSSplitViewDelegate;

#if MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_5

enum {
    NSSplitViewDividerStyleThick = 1,
    NSSplitViewDividerStyleThin
};
typedef NSInteger NSSplitViewDividerStyle;

#endif

@interface NSSplitView : NSView {
    @private
    id _variables;
}

/* Set or get whether the long axes of a split view's dividers are oriented up-and-down (YES) or left-and-right (NO).
*/
- (void)setVertical:(BOOL)flag;
- (BOOL)isVertical;

#if MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_5

/* What kind of divider to use. NSSplitViewThickDividerStyle is the default.
*/
- (void)setDividerStyle:(NSSplitViewDividerStyle)dividerStyle;
- (NSSplitViewDividerStyle)dividerStyle;

/* The name to use when autosaving the positions of dividers, and whether or not subviews are collapsed, to preferences. If this value is nil or the string is empty no autosaving is done.
*/
- (void)setAutosaveName:(NSString *)autosaveName;
- (NSString *)autosaveName;

#endif

/* Set or get the delegate of the split view. The delegate will be sent NSSplitViewDelegate messages to which it responds.
*/
- (void)setDelegate:(id <NSSplitViewDelegate>)delegate;
- (id <NSSplitViewDelegate>)delegate;

/* Draw the divider between two of the split view's subviews. The rectangle describes the entire divider rectangle in the receiver's coordinates. You can override this method to change the appearance of dividers.
*/
- (void)drawDividerInRect:(NSRect)rect;

#if MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_5

/* Return the color of the dividers that the split view is drawing between subviews. The default implementation of this method returns [NSColor clearColor] for the thick divider style. It will also return [NSColor clearColor] for the thin divider style when the split view is in a textured window. All other thin dividers are drawn with a color that looks good between two white panes. You can override this method to change the color of dividers.
*/
- (NSColor *)dividerColor;

#endif

/* Return the thickness of the dividers that the split view is drawing between subviews. The default implementation returns a value that depends on the divider style. You can override this method to change the size of dividers.
*/
- (CGFloat)dividerThickness;

/* Set the frames of the split view's subviews so that they, plus the dividers, fill the split view. The default implementation of this method resizes all of the subviews proportionally so that the ratio of heights (in the horizontal split view case) or widths (in the vertical split view case) doesn't change, even though the absolute sizes of the subviews do change. This message should be sent to split views from which subviews have been added or removed, to reestablish the consistency of subview placement.
*/
- (void)adjustSubviews;

/* Return YES if the subview is in the collapsed state, NO otherwise.
*/
- (BOOL)isSubviewCollapsed:(NSView *)subview;

#if MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_5

/* Divider indices are zero-based, with the topmost (in horizontal split views) or leftmost (vertical) divider having an index of 0.
*/

/* Get the minimum or maximum possible position of a divider. The position is "possible" in that it is dictated by the bounds of this view and the current position of other dividers. ("Allowable" positions are those that result from letting the delegate apply constraints to the possible positions.) You can invoke these methods to determine the range of values that can be usefully passed to -setPosition:ofDividerAtIndex:. You can also invoke them from delegate methods like -splitView:constrainSplitPosition:ofSubviewAt: to implement relatively complex behaviors that depend on the current state of the split view. The results of invoking these methods when -adjustSubviews has not been invoked recently enough for the subview frames to be valid are undefined.
*/
- (CGFloat)minPossiblePositionOfDividerAtIndex:(NSInteger)dividerIndex;
- (CGFloat)maxPossiblePositionOfDividerAtIndex:(NSInteger)dividerIndex;

/* Set the position of a divider. The default implementation of this method behaves as if the user were attempting to drag the divider to the proposed position, so the constraints imposed by the delegate are applied and one of the views adjacent to the divider may be collapsed. This method is not invoked by NSSplitView itself, so there's probably not much point in overriding it.
*/
- (void)setPosition:(CGFloat)position ofDividerAtIndex:(NSInteger)dividerIndex;

#endif

@end

@protocol NSSplitViewDelegate <NSObject>
@optional

/* Divider indices are zero-based, with the topmost (in horizontal split views) or leftmost (vertical) divider having an index of 0.
*/

/* Return YES if a subview can be collapsed, NO otherwise. If a split view has no delegate, or if its delegate does not respond to this message, none of the split view's subviews can be collapsed. If a split view has a delegate, and the delegate responds to this message, it will be sent at least twice when the user clicks or double-clicks on one of the split view's dividers, once per subview on either side of the divider, and may be resent as the user continues to drag the divider. If a subview is collapsible, the current implementation of NSSplitView will collapse it when the user has dragged the divider more than halfway between the position that would make the subview its minimum size and the position that would make it zero size. The subview will become uncollapsed if the user drags the divider back past that point. The comments for -splitView:constrainMinCoordinate:ofSubviewAt: and -splitView:constrainMaxCoordinate:ofSubviewAt: describe how subviews' minimum sizes are determined. Collapsed subviews are hidden but retained by the split view. Collapsing of a subview will not change its bounds, but may set its frame to zero pixels high (in horizontal split views) or zero pixels wide (vertical).
*/
- (BOOL)splitView:(NSSplitView *)splitView canCollapseSubview:(NSView *)subview;

#if MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_5

/* Return YES if the subview should be collapsed because the user has double-clicked on an adjacent divider. If a split view has a delegate, and the delegate responds to this message, it will be sent once for the subview before a divider when the user double-clicks on that divider, and again for the subview after the divider, but only if the delegate returned YES when sent -splitView:canCollapseSubview: for the subview in question. When the delegate indicates that both subviews should be collapsed NSSplitView's behavior is undefined.
*/
- (BOOL)splitView:(NSSplitView *)splitView shouldCollapseSubview:(NSView *)subview forDoubleClickOnDividerAtIndex:(NSInteger)dividerIndex;

#endif

/* Given a proposed minimum allowable position for one of the dividers of a split view, return the minimum allowable position for the divider. If a split view has no delegate, or if its delegate does not respond to this message, the split view behaves as if it has a delegate that responds to this message by merely returning the proposed minimum. If a split view has a delegate, and the delegate responds to this message, it will be sent at least once when the user begins dragging one of the split view's dividers, and may be resent as the user continues to drag the divider.

Delegates that respond to this message and return a number larger than the proposed minimum position effectively declare a minimum size for the subview above or to the left of the divider in question, the minimum size being the difference between the proposed and returned minimum positions. This minimum size is only effective for the divider-dragging operation during which the -splitView:constrainMinCoordinate:ofSubviewAt: message is sent. NSSplitView's behavior is undefined when a delegate responds to this message by returning a number smaller than the proposed minimum.
*/
- (CGFloat)splitView:(NSSplitView *)splitView constrainMinCoordinate:(CGFloat)proposedMinimumPosition ofSubviewAt:(NSInteger)dividerIndex;

/* Given a proposed maximum allowable position for one of the dividers of a split view, return the maximum allowable position for the divider. If a split view has no delegate, or if its delegate does not respond to this message, the split view behaves as if it has a delegate that responds to this message by merely returning the proposed maximum. If a split view has a delegate, and the delegate responds to this message, it will be sent at least once when the user begins dragging one of the split view's dividers, and may be resent as the user continues to drag the divider.

Delegates that respond to this message and return a number smaller than the proposed maximum position effectively declare a minimum size for the subview below or to the right of the divider in question, the minimum size being the difference between the proposed and returned maximum positions. This minimum size is only effective for the divider-dragging operation during which the -splitView:constrainMaxCoordinate:ofSubviewAt: message is sent. NSSplitView's behavior is undefined when a delegate responds to this message by returning a number larger than the proposed maximum.
*/
- (CGFloat)splitView:(NSSplitView *)splitView constrainMaxCoordinate:(CGFloat)proposedMaximumPosition ofSubviewAt:(NSInteger)dividerIndex;

/* Given a proposed position for one of the dividers of a split view, return a position at which the divider should be placed as the user drags it. If a split view has no delegate, or if its delegate does not respond to this message, the split view behaves as if it has a delegate that responds to this message by merely returning the proposed position. If a split view has a delegate, and the delegate responds to this message, it will be sent repeatedly as the user drags one of the split view's dividers.
*/
- (CGFloat)splitView:(NSSplitView *)splitView constrainSplitPosition:(CGFloat)proposedPosition ofSubviewAt:(NSInteger)dividerIndex;

/* Given that a split view has been resized but has not yet adjusted its subviews to accomodate the new size, and given the former size of the split view, adjust the subviews to accomodate the new size of the split view. If a split view has no delegate, or if its delegate does not respond to this message, the split view behaves as if it has a delegate that responds to this message by merely sending the split view an -adjustSubviews message.

Delegates that respond to this message should adjust the frames of the uncollapsed subviews so that they exactly fill the split view with room for dividers in between, taking its new size into consideration. The thickness of dividers can be determined by sending the split view a -dividerThickness message.
*/
- (void)splitView:(NSSplitView *)splitView resizeSubviewsWithOldSize:(NSSize)oldSize;

#if MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_6

/* Given that a split view has been resized and is adjusting its subviews to accomodate the new size, return YES if -adjustSubviews can change the size of the indexed subview, NO otherwise. -adjustSubviews may change the origin of the indexed subview regardless of what this method returns. If a split view has no delegate, or if its delegate does not respond to this message, the split view behaves as if it has a delegate that returns YES when sent this message.
 */
- (BOOL)splitView:(NSSplitView *)splitView shouldAdjustSizeOfSubview:(NSView *)view;

#endif

#if MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_5

/* Given that a split view has been resized and is adjusting its subviews to accomodate the new size, or that the user is dragging a divider, return YES to allow the divider to be dragged or adjusted off the edge of the split view where it will not be visible. If a split view has no delegate, or if its delegate does not respond to this message, the split view behaves as if it has a delegate that returns NO when sent this message.
*/
- (BOOL)splitView:(NSSplitView *)splitView shouldHideDividerAtIndex:(NSInteger)dividerIndex;

/* Given the drawn frame of a divider (in the coordinate system established by the split view's bounds), return the frame in which mouse clicks should initiate divider dragging. If a split view has no delegate, or if its delegate does not respond to this message, the split view behaves as if it has a delegate that returns proposedEffectiveRect when sent this message. A split view with thick dividers proposes the drawn frame as the effective frame. A split view with thin dividers proposes an effective frame that's a litte larger than the drawn frame, to make it easier for the user to actually grab the divider.
*/
- (NSRect)splitView:(NSSplitView *)splitView effectiveRect:(NSRect)proposedEffectiveRect forDrawnRect:(NSRect)drawnRect ofDividerAtIndex:(NSInteger)dividerIndex;

/* Given a divider index, return an additional rectangular area (in the coordinate system established by the split view's bounds) in which mouse clicks should also initiate divider dragging, or NSZeroRect to not add one. If a split view has no delegate, or if its delegate does not respond to this message, only mouse clicks within the effective frame of a divider initiate divider dragging.
*/
- (NSRect)splitView:(NSSplitView *)splitView additionalEffectiveRectOfDividerAtIndex:(NSInteger)dividerIndex;

#endif

/* Respond as if the delegate had registered for the NSSplitViewDidResizeSubviewsNotification or NSSplitViewWillResizeSubviewsNotification notification, described below. A split view's behavior is not explicitly affected by a delegate's ability or inability to respond to these messages, though the delegate may send messages to the split view in response to these messages.
*/
- (void)splitViewWillResizeSubviews:(NSNotification *)notification;
- (void)splitViewDidResizeSubviews:(NSNotification *)notification;

@end

/* A notification that is posted to the default notification center by NSSplitView when a split view is about to resize its subviews either as a result of its own resizing or during the dragging of one of its dividers by the user. Starting in Mac OS 10.5, if the notification is being sent because the user is dragging a divider, the notification's user info dictionary contains an entry whose key is @"NSSplitViewDividerIndex" and whose value is an NSInteger-wrapping NSNumber that is the index of the divider being dragged.
*/
APPKIT_EXTERN NSString *NSSplitViewWillResizeSubviewsNotification;

/* A notification that is posted to the default notification center by NSSplitView when a split view has just resized its subviews either as a result of its own resizing or during the dragging of one of its dividers by the user. Starting in Mac OS 10.5, if the notification is being sent because the user is dragging a divider, the notification's user info dictionary contains an entry whose key is @"NSSplitViewDividerIndex" and whose value is an NSInteger-wrapping NSNumber that is the index of the divider being dragged.
*/
APPKIT_EXTERN NSString *NSSplitViewDidResizeSubviewsNotification;


@interface NSSplitView (NSDeprecated)

/* Set or get whether the split view is a "pane splitter" (YES) or "grabber" (NO) split view. In Mac OS 10.5 the value of this property has no effect. These methods are deprecated in Mac OS 10.6.
 */
- (void)setIsPaneSplitter:(BOOL)flag DEPRECATED_IN_MAC_OS_X_VERSION_10_6_AND_LATER;
- (BOOL)isPaneSplitter DEPRECATED_IN_MAC_OS_X_VERSION_10_6_AND_LATER;

@end
