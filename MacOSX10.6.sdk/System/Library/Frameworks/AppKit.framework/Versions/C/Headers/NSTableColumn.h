/*
    NSTableColumn.h
    Application Kit
    Copyright (c) 1995-2008, Apple Inc.
    All rights reserved.
*/

#import <Foundation/NSObject.h>
#import <Foundation/NSGeometry.h>

@class NSTableView;
@class NSCell;
@class NSImage;
@class NSSortDescriptor;

@interface NSTableColumn : NSObject <NSCoding>
{
    /*All instance variables are private */
    id		_identifier;
    CGFloat	_width;
    CGFloat	_minWidth;
    CGFloat	_maxWidth;
    NSTableView *_tableView;
    NSCell	*_headerCell;
    NSCell	*_dataCell;
    struct __colFlags {
        unsigned int	oldIsResizable:1;
        unsigned int	isEditable:1;
        unsigned int	resizedPostingDisableCount:8;
        unsigned int    canUseReorderResizeImageCache:1;
        unsigned int    userResizingAllowed:1;
        unsigned int    autoResizingAllowed:1;
        unsigned int    hidden:1; // Defaults to NO
        unsigned int	RESERVED:18;
    } _cFlags;
    id _tcAuxiliaryStorage;
}

/* Designated initializer for NSTableColumns. Also see -setIdentifier: and -identifier.
 */
- (id)initWithIdentifier:(id)identifier;

/* Gets and sets the identifier associated with the NSTableColumn. 'identifier' is a strong reference, and will be retained. If the NSTableColumn is encoded (ie: archived in a nib file), 'identifier' should support the NSCoding protocol.
 */
- (void)setIdentifier:(id)identifier;
- (id)identifier;

- (void)setTableView:(NSTableView *)tableView;
- (NSTableView *)tableView;

/* Gets and sets the current width of the NSTableColumn. The default for is 100.0. Calling setWidth: with a width that is less than the minWidth or larger than the maxWidth will automatically be constrained to minWidth or maxWidth as appropriate.
 */
- (void)setWidth:(CGFloat)width;
- (CGFloat)width;

/* Gets and sets the minimum width of the NSTableColumn. The default for is 10.0. If -[self width] is less than the 'minWidth' value when -setMinWidth: is called, the width will automatically be increased to the minWidth and [tableView tile] will be called.
 */
- (void)setMinWidth:(CGFloat)minWidth;
- (CGFloat)minWidth;

/* Gets and sets the maximum width of the NSTableColumn. The default for is MAXFLOAT on Leopard and higher. If -[self width] is greater than the 'maxWidth' value when -setMaxWidth: is called, the width will automatically be decreased to the maxWidth and [tableView tile] will be called.
 */
- (void)setMaxWidth:(CGFloat)maxWidth;
- (CGFloat)maxWidth;

/* Gets and sets the headerCell associated with this NSTableColumn. 'cell' must be non nil, and should be a member of the NSTableHeaderCell class. The 'headerCell' is a strong reference, and will be retained.
 */
- (void)setHeaderCell:(NSCell *)cell;
- (id)headerCell;

/* Gets and sets the default cell used to draw the actual values in this NSTableColumn. The default value is an NSTextFieldCell that has the following properties set, which are subject to change: setEditable:YES, setScrollable:YES, setDrawsBackground:NO, setFont:[NSFont systemFontOfSize:[NSFont systemFontSize]], setLineBreakMode:NSLineBreakByTruncatingTail, setBackgroundColor:[NSColor controlBackgroundColor]. The 'dataCell' is a strong reference, and will be retained.
 */
- (void)setDataCell:(NSCell *)cell;
- (id)dataCell;

/* Allows per-row customization of the cell used for this NSTableColumn. NSTableView will call -[tableColumn dataCellForRow:] when drawing 'row'. By default this just calls -dataCell. Subclassers can override -dataCellForRow: if they need to potentially use different cells for different rows. On Leopard and higher, the NSTableView delegate method -tableView:dataCellForTableColumn:row: can be used as a convience to avoid subclassing NSTableColumn. In both cases, the returned cell should properly implement -copyWithZone:, since NSTableView may make copies of the cells.
 */
- (id)dataCellForRow:(NSInteger)row;

/* Gets and sets the editability state of this NSTableColumn. The default value is YES. When an NSTableView wants to start editing a dataCell, the following happens: 1. Check [tableColumn isEditable]. If that returns YES, check the (optional) delegate method with tableView:shouldEditTableColumn:row:. If that returns YES, check the [dataCell isEnabled] and [isEnabled isSelectable] states before attempting to edit.
 */
- (void)setEditable:(BOOL)flag;
- (BOOL)isEditable;

- (void)sizeToFit;

/* A column is considered sortable if it has a sortDescriptorPrototype. This prototype defines several things about the columns sorting.  he prototype's ascending value defines the default sorting direction. Its key defines an arbitrary attribute which helps clients identify what to sort, while the selector defines how to sort. Note that, it is not required that the key be the same as the identifier. However, the key must be unique from the key used by other columns. The sortDescriptor is archived.
 */
- (void)setSortDescriptorPrototype:(NSSortDescriptor *)sortDescriptor AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;
- (NSSortDescriptor *)sortDescriptorPrototype AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;

/* The resizing mask controls the resizability of a table column. Values can be combined together with a bitwise or. The default value is 'NSTableColumnUserResizingMask | NSTableColumnAutoresizingMask'. Compatability Note: This method replaces setResizable.
*/ 
- (void)setResizingMask:(NSUInteger)resizingMask AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER;
- (NSUInteger)resizingMask AVAILABLE_MAC_OS_X_VERSION_10_4_AND_LATER;

enum {
    NSTableColumnNoResizing = 0, // Disallow any kind of resizing.
    NSTableColumnAutoresizingMask = ( 1 << 0 ), // This column can be resized as the table is resized.
    NSTableColumnUserResizingMask = ( 1 << 1 ), // The user can resize this column manually.
};

/* Get and set the Tool Tip for the NSTableColumn header that appears when hovering the mouse over the header for the NSTableColumn. The default value is 'nil', meaning there is no headerToolTip.
*/
- (void)setHeaderToolTip:(NSString *)string AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;
- (NSString *)headerToolTip AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;

/* Determines if the column is hidden or not. The isHidden value is stored out when the NSTableView automatically saves out NSTableColumn state. Note that columns which are hidden still exist in the the -[NSTableView tableColumns] array and -[NSTableView numberOfColumns] includes columns which are hidden. The default value is NO, meaning the NSTableColumn will not be hidden.
*/
- (BOOL)isHidden AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;
- (void)setHidden:(BOOL)hidden AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;

@end

/*
 * Deprecated Methods
 */

@interface NSTableColumn(NSDeprecated)

/* Deprecated in Mac OS 10.4.  If flag is YES, calls setResizingMask:(NSTableColumnUserResizingMask | NSTableColumnAutoresizingMask).  If flag is NO, calls setResizingMask:(NSTableColumnNoResizing).
 */
- (void)setResizable:(BOOL)flag DEPRECATED_IN_MAC_OS_X_VERSION_10_4_AND_LATER;
- (BOOL)isResizable DEPRECATED_IN_MAC_OS_X_VERSION_10_4_AND_LATER;

@end
