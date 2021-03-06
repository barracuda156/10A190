/*
    NSBrowser.h
    Application Kit
    Copyright (c) 1994-2008, Apple Inc.
    All rights reserved.
*/

#import <AppKit/NSControl.h>
#import <AppKit/NSDragging.h>
#import <AppKit/NSViewController.h>

#define NSAppKitVersionNumberWithContinuousScrollingBrowser 680.0
#define NSAppKitVersionNumberWithColumnResizingBrowser      685.0

@class NSMatrix, NSScroller, NSMutableArray, NSIndexSet;
@protocol NSBrowserDelegate;

typedef struct __Brflags {
#ifdef __BIG_ENDIAN__
    unsigned int        allowsMultipleSelection:1;
    unsigned int        allowsBranchSelection:1;
    unsigned int        reuseColumns:1;
    unsigned int        isTitled:1;
    unsigned int        titleFromPrevious:1;
    unsigned int        separateColumns:1;
    unsigned int        delegateImplementsWillDisplayCell:1;
    unsigned int        delegateSetsTitles:1;
    unsigned int        delegateSelectsCellsByString:1;
    unsigned int        delegateDoesNotCreateRowsInMatrix:1;
    unsigned int        delegateValidatesColumns:1;
    unsigned int        acceptArrowKeys:1;
    unsigned int        dontDrawTitles:1;
    unsigned int        sendActionOnArrowKeys:1;
    unsigned int        prohibitEmptySel:1;
    unsigned int        hasHorizontalScroller:1;
    unsigned int        time:1;
    unsigned int        allowsIncrementalSearching:1;
    unsigned int        delegateSelectsCellsByRow:1;
    unsigned int        disableCompositing:6;
    unsigned int        refusesFirstResponder:1;
    unsigned int	acceptsFirstMouse:1;
    unsigned int	actionNeedsToBeSent:1;
    unsigned int        usesSmallSizeTitleFont:1;
    unsigned int	usesSmallScrollers:1;
    unsigned int	prefersAllColumnUserResizing:1;
    unsigned int        firstVisibleCalculationDisabled:1;
#else
    unsigned int        firstVisibleCalculationDisabled:1;
    unsigned int 	prefersAllColumnUserResizing:1;
    unsigned int	usesSmallScrollers:1;
    unsigned int        usesSmallSizeTitleFont:1;
    unsigned int	actionNeedsToBeSent:1;
    unsigned int	acceptsFirstMouse:1;
    unsigned int        refusesFirstResponder:1;
    unsigned int        disableCompositing:6;
    unsigned int        delegateSelectsCellsByRow:1;
    unsigned int        allowsIncrementalSearching:1;
    unsigned int        time:1;
    unsigned int        hasHorizontalScroller:1;
    unsigned int        prohibitEmptySel:1;
    unsigned int        sendActionOnArrowKeys:1;
    unsigned int        dontDrawTitles:1;
    unsigned int        acceptArrowKeys:1;
    unsigned int        delegateValidatesColumns:1;
    unsigned int        delegateDoesNotCreateRowsInMatrix:1;
    unsigned int        delegateSelectsCellsByString:1;
    unsigned int        delegateSetsTitles:1;
    unsigned int        delegateImplementsWillDisplayCell:1;
    unsigned int        separateColumns:1;
    unsigned int        titleFromPrevious:1;
    unsigned int        isTitled:1;
    unsigned int        reuseColumns:1;
    unsigned int        allowsBranchSelection:1;
    unsigned int        allowsMultipleSelection:1;
#endif
} _Brflags;

#if MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_3

enum {
/* Column sizes are fixed and set by developer.     
 */
    NSBrowserNoColumnResizing = 0,
    
/* No user resizing. Columns grow as window grows.  
 */
    NSBrowserAutoColumnResizing = 1,
    
/* Columns fixed as window grows.  User can resize. 
 */
    NSBrowserUserColumnResizing = 2,
};

#endif

typedef NSUInteger NSBrowserColumnResizingType;

#if MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_5

/* In drag and drop, used to specify the drop operation from inside the delegate method browser:validateDrop:proposedRow:column:dropOperation. See the delegate method description for more information.
 */
enum { 
    NSBrowserDropOn, 
    NSBrowserDropAbove,
};

#endif

typedef NSUInteger NSBrowserDropOperation;

@interface NSBrowser : NSControl
{
    /* All instance variables are private */
    id                  _target;
    SEL                 _action;
    id                  _delegate;
    SEL                 _doubleAction;
    Class               _matrixClass;
    id                  _cellPrototype;
    NSSize		_columnSize;
    short		_numberOfVisibleColumns;
    short		_minColumnWidth;
    short		_firstVisibleColumn;
    short		_maxVisibleColumns;
    NSMutableArray	*_titles;
    NSString		*_pathSeparator;
    NSMutableArray	*_columns;
    id                  _brAuxiliaryStorage;
    NSString		*_firstColumnTitle;
    NSScroller		*_scroller;
    _Brflags            _brflags;
}

+ (Class)cellClass;

- (void)loadColumnZero;
- (BOOL)isLoaded;

- (void)setDoubleAction:(SEL)aSelector;
- (SEL)doubleAction;
- (void)setMatrixClass:(Class)factoryId;
- (Class)matrixClass;
- (void)setCellClass:(Class)factoryId;
- (void)setCellPrototype:(NSCell *)aCell;
- (id)cellPrototype;
- (void)setDelegate:(id <NSBrowserDelegate>)anObject;
- (id <NSBrowserDelegate>)delegate;
- (void)setReusesColumns:(BOOL)flag;
- (BOOL)reusesColumns;

- (void)setHasHorizontalScroller:(BOOL)flag;
- (BOOL)hasHorizontalScroller;
- (void)setAutohidesScroller:(BOOL)flag AVAILABLE_MAC_OS_X_VERSION_10_6_AND_LATER;
- (BOOL)autohidesScroller AVAILABLE_MAC_OS_X_VERSION_10_6_AND_LATER;
- (void)setSeparatesColumns:(BOOL)flag;
- (BOOL)separatesColumns;
- (void)setTitled:(BOOL)flag;
- (BOOL)isTitled;
- (void)setMinColumnWidth:(CGFloat)columnWidth;
- (CGFloat)minColumnWidth;

- (void)setMaxVisibleColumns:(NSInteger)columnCount;
- (NSInteger)maxVisibleColumns;

- (void)setAllowsMultipleSelection:(BOOL)flag;
- (BOOL)allowsMultipleSelection;
- (void)setAllowsBranchSelection:(BOOL)flag;
- (BOOL)allowsBranchSelection;
- (void)setAllowsEmptySelection:(BOOL)flag;
- (BOOL)allowsEmptySelection;
- (void)setTakesTitleFromPreviousColumn:(BOOL)flag;
- (BOOL)takesTitleFromPreviousColumn;

- (void)setSendsActionOnArrowKeys:(BOOL)flag;
- (BOOL)sendsActionOnArrowKeys;

/* Returns the item at the given index path. This method can only be used if the delegate implements the item data source methods. The indexPath must be displayable in the browser.
 */
- (id)itemAtIndexPath:(NSIndexPath *)indexPath AVAILABLE_MAC_OS_X_VERSION_10_6_AND_LATER;

/* Returns the item located at 'row' in 'column'.
 */
- (id)itemAtRow:(NSInteger)row column:(NSInteger)column AVAILABLE_MAC_OS_X_VERSION_10_6_AND_LATER;

/* Returns the index path of the given item. This method may return nil if the item can not be displayed in the browser, or accessed via -itemAtIndexPath. This method can only be used if the delegate implements the item data source methods.
 */
- (NSIndexPath *)indexPathForItem:(id)item AVAILABLE_MAC_OS_X_VERSION_10_6_AND_LATER;

/* Returns the index path of the item whose children are displayed in the given column. This method can only be used if the delegate implements the item data source methods.
 */
- (NSIndexPath *)indexPathForColumn:(NSInteger)column AVAILABLE_MAC_OS_X_VERSION_10_6_AND_LATER;

/* Returns the parent item of the given item. This method may return nil if the item has never been displayed in the browser, or accessed via -itemAtIndexPath. This method can only be used if the delegate implements the item data source methods.
 */
- (id)parentForItem:(id)item AVAILABLE_MAC_OS_X_VERSION_10_6_AND_LATER;

/* Returns whether the given item is a leaf item. This method may return NO if the item has never been displayed in the browser, or accessed via -itemAtIndexPath. Overriding this method will have no effect on whether the browser treats the item as a leaf; you must return the appropriate value from the delegate method to affect this behavior. This method can only be used if the delegate implements the item data source methods.
 */
- (BOOL)isLeafItem:(id)item AVAILABLE_MAC_OS_X_VERSION_10_6_AND_LATER;

/* Refreshes the state of item, redisplaying it if necessary. If flag is YES, any children item are reloaded as well. This method can only be used if the delegate implements the item data source methods.
 */
- (void)reloadItem:(id)item reloadChildren:(BOOL)flag AVAILABLE_MAC_OS_X_VERSION_10_6_AND_LATER;

/* Returns the item that contains all children located in 'column'. In other words, it is the parent item for that column.
 */
- (id)parentForItemsInColumn:(NSInteger)column AVAILABLE_MAC_OS_X_VERSION_10_6_AND_LATER;

/* Scrolls 'row' to be visible within 'column'. 'column' will not be scrolled visible. To scroll 'column' to visible, first call -[browser scrollColumnToVisible:column].
 */
- (void)scrollRowToVisible:(NSInteger)row inColumn:(NSInteger)column AVAILABLE_MAC_OS_X_VERSION_10_6_AND_LATER;

- (void)setTitle:(NSString *)aString ofColumn:(NSInteger)column;
- (NSString *)titleOfColumn:(NSInteger)column;
- (void)setPathSeparator:(NSString *)newString;
- (NSString *)pathSeparator;
- (BOOL)setPath:(NSString *)path;
- (NSString *)path;
- (NSString *)pathToColumn:(NSInteger)column;

/* Returns the column and row clicked on to display a context menu. These methods will return -1 when no menu is active.
 */
- (NSInteger)clickedColumn AVAILABLE_MAC_OS_X_VERSION_10_6_AND_LATER;
- (NSInteger)clickedRow AVAILABLE_MAC_OS_X_VERSION_10_6_AND_LATER;

- (NSInteger)selectedColumn;

/* For the item based browser, selectedCell returns the prepared cell at the selected row in the selected column.
 */
- (id)selectedCell;
- (id)selectedCellInColumn:(NSInteger)column;

/* For the item based browser, selectedCells returns a copy of all prepared cells in the selected row in the selected column
 */
- (NSArray *)selectedCells;

- (void)selectRow:(NSInteger)row inColumn:(NSInteger)column;

- (NSInteger)selectedRowInColumn:(NSInteger)column;

/* Returns the index path of the item selected in the browser, or nil if there is no selection.
 */
- (NSIndexPath *)selectionIndexPath AVAILABLE_MAC_OS_X_VERSION_10_6_AND_LATER;

/* Sets the browser's selection to the item at path. Throws an exception if the path is invalid. This method can only be used if the delegate implements the item data source methods.
 */
- (void)setSelectionIndexPath:(NSIndexPath *)path AVAILABLE_MAC_OS_X_VERSION_10_6_AND_LATER;

/* Returns the index paths of all items selected in the browser. 
 */
- (NSArray *)selectionIndexPaths AVAILABLE_MAC_OS_X_VERSION_10_6_AND_LATER;

/* Sets the browser's selection to the specified index paths. Throws an exception if any of the paths are invalid. This method can only be used if the delegate implements the item data source methods.
 */
- (void)setSelectionIndexPaths:(NSArray *)paths AVAILABLE_MAC_OS_X_VERSION_10_6_AND_LATER;

/* Sets the selected row 'indexes' in the matrix located at 'column'. 
 */
- (void)selectRowIndexes:(NSIndexSet *)indexes inColumn:(NSInteger)column AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;

/* Returns the selected cell indexes in the matrix located at 'column'. 
 */
- (NSIndexSet *)selectedRowIndexesInColumn:(NSInteger)column AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;

- (void)reloadColumn:(NSInteger)column;
- (void)validateVisibleColumns;
- (void)scrollColumnsRightBy:(NSInteger)shiftAmount;
- (void)scrollColumnsLeftBy:(NSInteger)shiftAmount;
- (void)scrollColumnToVisible:(NSInteger)column;

/* Returns the last loaded column. This is equal to the total number of columns minus 1.
 */
- (NSInteger)lastColumn;

/* Sets the last loaded column. 'column' must be equal to or less than -lastColumn. To add more columns, use -addColumn.
 */
- (void)setLastColumn:(NSInteger)column;

/* Adds a column at the end. To remove columns, use -setLastColumn:
 */
- (void)addColumn;

- (NSInteger)numberOfVisibleColumns;
- (NSInteger)firstVisibleColumn;
- (NSInteger)lastVisibleColumn;

/* Returns the column that matrix represents. This method will return -1 if the delegate does not implement the matrix data source methods.
 */
- (NSInteger)columnOfMatrix:(NSMatrix *)matrix;

/* Return the matrix representing column. This method will return nil if the delegate does not implement the matrix data source methods.
 */
- (NSMatrix *)matrixInColumn:(NSInteger)column;

- (id)loadedCellAtRow:(NSInteger)row column:(NSInteger)col;
- (void)selectAll:(id)sender;
- (void)tile;
- (void)doClick:(id)sender;
- (void)doDoubleClick:(id)sender;
- (BOOL)sendAction;

- (NSRect)titleFrameOfColumn:(NSInteger)column;
- (void)drawTitleOfColumn:(NSInteger)column inRect:(NSRect)aRect;
- (CGFloat)titleHeight;
- (NSRect)frameOfColumn:(NSInteger)column;
- (NSRect)frameOfInsideOfColumn:(NSInteger)column;

/* These methods convert between column width (the column's scrollview), and the content width (the matrix in the scrollview).  For example, to guarantee that 16 pixels of your browser cell are always visible, call [browser setMinColumnWidth:[browser columnWidthForColumnContentWidth:16]] 
*/
- (CGFloat)columnWidthForColumnContentWidth:(CGFloat)columnContentWidth AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;
- (CGFloat)columnContentWidthForColumnWidth:(CGFloat)columnWidth AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;

/* Default is NSBrowserAutoColumnResizing.  This setting is persistent. 
 */
- (void)setColumnResizingType:(NSBrowserColumnResizingType)columnResizingType AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;
- (NSBrowserColumnResizingType)columnResizingType AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;

/* Default is NO.  This setting is persistent.  This setting only applies to NSBrowserUserColumnResizing type browsers.  If YES, the browser defaults to resizing all columns simultaneously, otherwise it defaults to single column resizing. Holding down the option key while resizing switches the type of resizing used. 
 */
- (void)setPrefersAllColumnUserResizing:(BOOL)prefersAllColumnResizing AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;
- (BOOL)prefersAllColumnUserResizing AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;

/* setWidth:ofColumn: does nothing if columnResizingType is NSBrowserAutoColumnResizing.  Otherwise, Sets the width of the specified column.  Due to binary compatibility constraints, you may still set the default width for new columns by passing a columnIndex of -1; you are encouraged to use -setDefaultColumnWidth: instead.  NSBrowserColumnConfigurationDidChangeNotification will be posted (not immediately) if necessary.  The receiver will autosave its column configuration if necessary. 
 */
- (void)setWidth:(CGFloat)columnWidth ofColumn:(NSInteger)columnIndex AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;
- (CGFloat)widthOfColumn:(NSInteger)column AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;

/* Get and set the rowHeight. The value must be greater than 0. Calling -setRowHeight: with a non-pixel aligning (fractional) value will be forced to a pixel aligning (integral) value. For variable row height browsers (ones that have the delegate implement -browser:heightOfRow:column:), -rowHeight will be used to draw alternating rows past the last row in each browser column. The default value is 17.0. Note: The rowHeight methods are only valid when using the "item delegate methods" introduced in MacOS 10.6. (see NSObject(NSBrowserDelegate)). An exception is thrown if using the older "matrix delegate methods" 
 */
- (void)setRowHeight:(CGFloat)height AVAILABLE_MAC_OS_X_VERSION_10_6_AND_LATER;
- (CGFloat)rowHeight AVAILABLE_MAC_OS_X_VERSION_10_6_AND_LATER;

/* If the delegate implements -browser:heightOfRow:column:, this method immediately re-tiles the browser columns using row heights it provides.
*/
- (void)noteHeightOfRowsWithIndexesChanged:(NSIndexSet *)indexSet inColumn:(NSInteger)columnIndex AVAILABLE_MAC_OS_X_VERSION_10_6_AND_LATER;


/* Persistently sets the default width for new columns which don't otherwise have initial width from either defaults or the delegate. This method replaces -setWidth:ofColumn: with a columnIndex of -1. 
 */
- (void)setDefaultColumnWidth:(CGFloat)columnWidth AVAILABLE_MAC_OS_X_VERSION_10_6_AND_LATER;
- (CGFloat)defaultColumnWidth AVAILABLE_MAC_OS_X_VERSION_10_6_AND_LATER;

/* Sets the name used to automatically save the receivers column configuration.  This setting is persistent.  If name is different from the current name, this method also reads in the saved column configuration for the new name and applies the values to the browser.  Column configuration is defined as an array of column content widths.  One width is saved for each level the user has reached.  That is, browser saves column width based on depth, not based on unique paths.  To do more complex column persistence, you should register for NSBrowserColumnConfigurationDidChangeNotifications and handle persistence yourself. 
 */
- (void)setColumnsAutosaveName:(NSString *)name AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;
- (NSString *)columnsAutosaveName AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;

/* Removes the column data stored under name from the applications user defaults. 
 */
+ (void)removeSavedColumnsWithAutosaveName:(NSString *)name AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;


#pragma mark -
#pragma mark **** Drag and Drop Support ****

/* The return value indicates whether the receiver can attempt to initiate a drag for the given event. You can override this method and return NO to disallow initiating drags at the location in the given event. By default, the method will return YES if rowIndexes contains at least one item, and each item is enabled. If the equivalent delegate method is present, it will be preferred over this method, and you can safely call this method from your delegate. 
 */
- (BOOL)canDragRowsWithIndexes:(NSIndexSet *)rowIndexes inColumn:(NSInteger)column withEvent:(NSEvent *)event AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;

/* This method computes and returns an image to use for dragging. You can override this to return a custom drag image, or call it to get the default drag image from the delegate method. 'rowIndexes' are the indexes of the cells being dragged in matrix 'column'. 'event' is a reference to the mouse down event that began the drag. 'dragImageOffset' is an in/out parameter. This method will be called with dragImageOffset set to NSZeroPoint, but it can be modified to re-position the returned image. A dragImageOffset of NSZeroPoint will cause the image to be centered under the mouse. By default, an image will be created that contain the visible cells within 'rowIndexes'. If the equivalent delegate method is present, it will be preferred over this method, and you can safely call this method from your delegate. 
 */
- (NSImage *)draggingImageForRowsWithIndexes:(NSIndexSet *)rowIndexes inColumn:(NSInteger)column withEvent:(NSEvent *)event offset:(NSPointPointer)dragImageOffset AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;

/* Configures the value returned from -draggingSourceOperationMaskForLocal:. An isLocal value of YES indicates that 'mask' applies when the destination object is in the same application. By default, NSDragOperationEvery will be returned. An isLocal value of NO indicates that 'mask' applies when the destination object in an application outside the receiver's application. By default, NSDragOperationNone is returned. NSBrowser will save the values you set for each isLocal setting. You typically will invoke this method, and not override it. 
 */
- (void)setDraggingSourceOperationMask:(NSDragOperation)mask forLocal:(BOOL)isLocal AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;

#pragma mark -

/* Allow type selection in this NSBrowser. The default for 'allowsTypeSelect' is YES.
 */
- (BOOL)allowsTypeSelect AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;
- (void)setAllowsTypeSelect:(BOOL)value AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;

/* The background color to be drawn. By default, it will be set [NSColor whiteColor]. You can use [NSColor clearColor] to make the background transparent. NSBrowser will return YES from isOpaque if the backgroundColor has an alphaComponent of 1.0 and it doesn't have a title, otherwise, it will return NO. Calling setBackgroundColor: will cause all NSMatrix instances have setDrawsBackground:NO be called in order for the NSBrowser's background color to show through. When drawing with the background color, NSCompositeSourceOver is used for the compositing operation.
 */
- (void)setBackgroundColor:(NSColor *)color AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;
- (NSColor *)backgroundColor AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;

/* Begins editing the item at the specified path. theEvent may be nil if programatically editing. The cell's contents will be selected if select is YES. Overriding this method will not affect the editing behavior of the browser.
 */
- (void)editItemAtIndexPath:(NSIndexPath *)indexPath withEvent:(NSEvent *)theEvent select:(BOOL)select AVAILABLE_MAC_OS_X_VERSION_10_6_AND_LATER;

@end

#pragma mark -
#pragma mark **** Notifications ****

/* The -object in the NSNotification is the browser whose column sizes need to be persisted. There is no userInfo.
 */
APPKIT_EXTERN NSString *NSBrowserColumnConfigurationDidChangeNotification AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;

#pragma mark -
#pragma mark **** Delegate methods ****

@protocol NSBrowserDelegate <NSObject>
@optional

/* As of Mac OS X 10.6, browser has two different mechanisms for populating columns. You may implement either the matrix or item delegate methods listed below. Many newer features of the browser are only available if you implement the item delegate methods. */

/* Implement one of the following two methods to populate the browser's columns with instances of NSMatrix.
 */
/* Called by the browser to determine the number of rows in the given column. The delegate will be called with -browser:willDisplayCell:atRow:column: before displaying each cell, giving it a chance to fill in the NSBrowserCell properties.
 */
- (NSInteger)browser:(NSBrowser *)sender numberOfRowsInColumn:(NSInteger)column;

/* Called by the browser to fill in the matrix with populated NSBrowserCells.
 */
- (void)browser:(NSBrowser *)sender createRowsForColumn:(NSInteger)column inMatrix:(NSMatrix *)matrix;

#pragma mark -

/* Alternatively, implement all of the following methods, patterened after NSOutlineView's data source methods. Note that browsers with delegates implementing these methods do not use NSMatrix to populate columns; the implementation is private. The item parameter passed to each of these methods will be nil if the browser is querying about the root of the tree, unless -rootItemForBrowser: is implemented.
*/

/* Return the number of children of the given item. */
- (NSInteger)browser:(NSBrowser *)browser numberOfChildrenOfItem:(id)item AVAILABLE_MAC_OS_X_VERSION_10_6_AND_LATER;

/* Return the indexth child of item. You may expect that index is never equal to or greater to the number of children of item as reported by -browser:numberOfChildrenOfItem:.
 */
- (id)browser:(NSBrowser *)browser child:(NSInteger)index ofItem:(id)item AVAILABLE_MAC_OS_X_VERSION_10_6_AND_LATER;

/* Return whether item should be shown as a leaf item; that is, an item that can not be expanded into another column. Returning NO does not prevent you from returning 0 from -browser:numberOfChildrenOfItem:.
 */
- (BOOL)browser:(NSBrowser *)browser isLeafItem:(id)item AVAILABLE_MAC_OS_X_VERSION_10_6_AND_LATER;

/* Return the object value passed to the cell displaying item.
 */
- (id)browser:(NSBrowser *)browser objectValueForItem:(id)item AVAILABLE_MAC_OS_X_VERSION_10_6_AND_LATER;

/* Optional - Variable Row Heights
    Implement this method to support varying row heights per column.  The height returned by this method should not include intercell spacing and must be greater than zero.  NSBrowser may cache the values this method returns.  So if you would like to change a row's height make sure to invalidate the row height by calling -noteHeightOfRowsWithIndexesChanged:inColumn:.
*/
- (CGFloat)browser:(NSBrowser *)browser heightOfRow:(NSInteger)row column:(NSInteger)columnIndex AVAILABLE_MAC_OS_X_VERSION_10_6_AND_LATER;

#pragma mark -

/* Optional - Alternate root item. 
 By default, NSBrowser uses 'nil' to identify the root item. It can optionally use a different root item provided by this delegate method. To reload the rootItem that was previously set, call -loadColumnZero, and NSBrowser will call -rootItemForBrowser: again.
 */
- (id)rootItemForBrowser:(NSBrowser *)browser AVAILABLE_MAC_OS_X_VERSION_10_6_AND_LATER;

/* Optional - editing support.
 Implement this method to support editing of browser items. The browser will pass back the object value from the cell displaying item.
 */
- (void)browser:(NSBrowser *)browser setObjectValue:(id)object forItem:(id)item AVAILABLE_MAC_OS_X_VERSION_10_6_AND_LATER;

/* Optional - editing support.
 Implement this method to control whether the browser may start an editing session for item.
 */
- (BOOL)browser:(NSBrowser *)browser shouldEditItem:(id)item AVAILABLE_MAC_OS_X_VERSION_10_6_AND_LATER;

#pragma mark -

/* Optional - display support.
 Called by the browser before displaying a cell at the given location.
 */
- (void)browser:(NSBrowser *)sender willDisplayCell:(id)cell atRow:(NSInteger)row column:(NSInteger)column;

- (NSString *)browser:(NSBrowser *)sender titleOfColumn:(NSInteger)column;

/* Called by the browser when selecting cells by title, either from -setPath:, or when recomputing the selection after -reloadColumn:. Return NO if a matching cell could not be found.
 */
- (BOOL)browser:(NSBrowser *)sender selectCellWithString:(NSString *)title inColumn:(NSInteger)column;

/* Called by the browser when selecting a cell using -selectRow:inColumn:. Return NO if a matching cell could not be found.
 */
- (BOOL)browser:(NSBrowser *)sender selectRow:(NSInteger)row inColumn:(NSInteger)column;

/* Called by the browser from -validateVisibleColumns to determine whether the currently-displayed columns are valid. Returning NO will prompt the browser to call -reloadColumn:.
 */
- (BOOL)browser:(NSBrowser *)sender isColumnValid:(NSInteger)column;

- (void)browserWillScroll:(NSBrowser *)sender;
- (void)browserDidScroll:(NSBrowser *)sender;

/*
 * Delegate methods used by resizable column browsers.
 */

/* Optional for browsers with resize type NSBrowserNoColumnResizing, and NSBrowserUserColumnResizing.
 This method is used for both constraining column resize, and determining a columns initial size.  If 'forUserResize' is NO, this method should return the initial width for a newly added column.  If 'forUserResize' is YES, this method can be used to constrain resizing on a per-column basis.  (Currently per-column constraining is not implemented, so forUserResize will always be NO).
 */
- (CGFloat)browser:(NSBrowser *)browser shouldSizeColumn:(NSInteger)columnIndex forUserResize:(BOOL)forUserResize toWidth:(CGFloat)suggestedWidth AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;

/* Optional for browsers with resize type NSBrowserUserColumnResizing only.
 This method returns the "ideal" width for a column.  This method is used when performing a "right-size-each" or "right-size-one" operation.  If columnIndex is -1, the result is used for a "right-size-all-simultaneous" operation.  In this case, you should return a best uniform right size for all column (every column will be set to this size).  It is assumed that the implementation may be expensive, so it will be called only when necessary.  (See documentation for definitions of right-size one/each/all). 
 */
- (CGFloat)browser:(NSBrowser *)browser sizeToFitWidthOfColumn:(NSInteger)columnIndex AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;

/* Optional and used for browsers with resize type NSBrowserUserColumnResizing only.
 This method is intended to be used by clients wishing to implement their own column width persistence.  It is called when the width of any browser columns has changed.  User column resizing will cause a single notification to be posted when the user is finished resizing.  (See NSBrowserColumnConfigurationDidChangeNotification for more information.)  
 */
- (void)browserColumnConfigurationDidChange:(NSNotification *)notification AVAILABLE_MAC_OS_X_VERSION_10_3_AND_LATER;

/* Optional - Expansion ToolTip support.
    Implement this method and return NO to prevent an expansion tooltip from appearing for a particular cell at 'row' in 'column'. See NSCell.h for more information on expansion tool tips. 
 */
- (BOOL)browser:(NSBrowser *)browser shouldShowCellExpansionForRow:(NSInteger)row column:(NSInteger)column AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;


#pragma mark -
#pragma mark **** Optional Drag and Drop Support Methods ****

#pragma mark ** Dragging Source Methods **

/* This method is called after it has been determined that a drag should begin, but before the drag has been started.  To refuse the drag, return NO. To start a drag, declared the pasteboard types that you support with [pasteboard declareTypes:owner:], place your data on the pasteboard, and return YES from the method. The drag image and other drag related information will be set up and provided by the view once this call returns with YES. You need to implement this method for your browser to be a drag source. 
 */
- (BOOL)browser:(NSBrowser *)browser writeRowsWithIndexes:(NSIndexSet *)rowIndexes inColumn:(NSInteger)column toPasteboard:(NSPasteboard *)pasteboard AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;

/* The delegate can support file promise drags by adding NSFilesPromisePboardType to the pasteboard in browser:writeRowsWithIndexes:inColumn:toPasteboard:. NSBrowser implements -namesOfPromisedFilesDroppedAtDestination: to return the results of this data source method.  This method should returns an array of filenames for the created files (filenames only, not full paths).  The URL represents the drop location.  For more information on file promise dragging, see documentation on the NSDraggingSource protocol and -namesOfPromisedFilesDroppedAtDestination:. You do not need to implement this method for your browser to be a drag source.
 */
- (NSArray *)browser:(NSBrowser *)browser namesOfPromisedFilesDroppedAtDestination:(NSURL *)dropDestination forDraggedRowsWithIndexes:(NSIndexSet *)rowIndexes inColumn:(NSInteger)column AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;

/* The delegate can control if some particular rows can be dragged or not for a particular event. You do not need to implement this method for your browser to be a drag source. 
 */
- (BOOL)browser:(NSBrowser *)browser canDragRowsWithIndexes:(NSIndexSet *)rowIndexes inColumn:(NSInteger)column withEvent:(NSEvent *)event AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;

/* Allows the delegate to compute a dragging image for the particular cells being dragged. 'rowIndexes' are the indexes of the cells being dragged in the matrix in 'column'. 'event' is a reference to the mouse down event that began the drag. 'dragImageOffset' is an in/out parameter. This method will be called with dragImageOffset set to NSZeroPoint, but it can be modified to re-position the returned image.  A dragImageOffset of NSZeroPoint will cause the image to be centered under the mouse. You can safely call [browser dragImageForRowsWithIndexes:inColumn:withEvent:offset:] from inside this method. You do not need to implement this method for your browser to be a drag source. You can safely call the corresponding NSBrowser method.
 */
- (NSImage *)browser:(NSBrowser *)browser draggingImageForRowsWithIndexes:(NSIndexSet *)rowIndexes inColumn:(NSInteger)column withEvent:(NSEvent *)event offset:(NSPointPointer)dragImageOffset AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;

#pragma mark ** Dragging Destination Methods **

/* This method is used by the browser to determine a valid drop target. Based on the mouse position, the browser will suggest a proposed row, column and dropOperation. These values are in/out parameters and can be changed by the delegate to retarget the drop operation. The dropOperation suggested will be NSBrowserDropOn if the drag location is closer to the middle of a row. If it is closer to being between two rows, it will be NSBrowserDropAbove, indicating a drop above 'row' and below 'row - 1'. This method must return a value that indicates which dragging operation the data source will perform. It must return something other than NSDragOperationNone to accept the drop.

    To retarget a drop and specify a drop on row 2 (with the first visual row being row 0), one would set *row=2, and set *dropOperation=NSBrowserDropOn. To specify a drop below the last row, one would specify *row=[browser numberOfRows], and *dropOperation=NSBrowserDropAbove. To specify a drop on the entire column, you will specify *row=-1, and *dropOperation=NSBrowserDropOn. To specify a drop between rows 2 and 3, one would set *row=3 and *dropOperation=NSBrowserDropAbove.

    Note: to receive drag messages, you must first call [NSBrowser registerForDraggedTypes:] for the drag types you want to support (typically this is done in awakeFromNib). You must implement this method for your browser to be a drag destination. 
*/
- (NSDragOperation)browser:(NSBrowser *)browser validateDrop:(id <NSDraggingInfo>)info proposedRow:(NSInteger *)row column:(NSInteger *)column  dropOperation:(NSBrowserDropOperation *)dropOperation AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;

/* This method is called when the mouse is released over a browser that previously decided to allow a drop via the above validateDrop method.  The delegate should incorporate the data from the dragging pasteboard at this time. You must implement this method for your browser to be a drag destination.
*/
- (BOOL)browser:(NSBrowser *)browser acceptDrop:(id <NSDraggingInfo>)info atRow:(NSInteger)row column:(NSInteger)column dropOperation:(NSBrowserDropOperation)dropOperation AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;

#pragma mark -

#pragma mark **** Type Select Support ****

/* Optional - Type select support
    Implement this method if you want to control the string that is used for type selection. You may want to change what is searched for based on what is displayed, or simply return an empty string for that row and/or column to not be searched. You can also return 'nil' if the cell does not contain any text. By default, all cells with text in them are searched. The default value when this delegate method is not implemented is the stringValue for the cell at that location.
 */
- (NSString *)browser:(NSBrowser *)browser typeSelectStringForRow:(NSInteger)row inColumn:(NSInteger)column AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;

/* Optional - Type select support
    Implement this method if you would like to prevent a type select from happening based on the current event and current search string. Generally, this will be called from keyDown: and the event will be a key event. The search string will be nil if no type select has began. 
 */
- (BOOL)browser:(NSBrowser *)browser shouldTypeSelectForEvent:(NSEvent *)event withCurrentSearchString:(NSString *)searchString AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;

/* Optional - Type select support
    Implement this method if you want to control how type selection works. Return the first row that matches searchString from within the range of startRow to endRow. It is possible for endRow to be less than startRow if the search will wrap. Return -1 when there is no match. Include startRow as a possible match, but do not include endRow. It is not necessary to implement this method in order to support type select.
 */
- (NSInteger)browser:(NSBrowser *)browser nextTypeSelectMatchFromRow:(NSInteger)startRow toRow:(NSInteger)endRow inColumn:(NSInteger)column forString:(NSString *)searchString AVAILABLE_MAC_OS_X_VERSION_10_5_AND_LATER;

#pragma mark -

/* Optional - Preview column support
    Implement this method to provide a preview column for leaf items. Return nil to suppress the preview column. The controller's representedObject will be set to the browser's selected leaf item. This method is only called if the delegate implements the item data source methods.
 */
- (NSViewController *)browser:(NSBrowser *)browser previewViewControllerForLeafItem:(id)item AVAILABLE_MAC_OS_X_VERSION_10_6_AND_LATER;

/* Optional - Column header support
    Implement this method to provide a header view for columns. Return nil to omit the header view. The controller's representedObject will be set to the column's item. This method is only called if the delegate implements the item data source methods.
 */
- (NSViewController *)browser:(NSBrowser *)browser headerViewControllerForItem:(id)item AVAILABLE_MAC_OS_X_VERSION_10_6_AND_LATER;

/* Optional - Notification when the lastColumn changes.
 */
- (void)browser:(NSBrowser *)browser didChangeLastColumn:(NSInteger)oldLastColumn toColumn:(NSInteger)column;

/* Optional - Return a set of new indexes to select when the user changes the selection with the keyboard or mouse. This method may be called multiple times with one new index added to the existing selection to find out if a particular index can be selected when the user is extending the selection with the keyboard or mouse. Note that 'proposedSelectionIndexes' will contain the entire newly suggested selection, and you can return the exsiting selection to avoid changing the selection. This method only works for item-based NSBrowsers.
 */
- (NSIndexSet *)browser:(NSBrowser *)browser selectionIndexesForProposedSelection:(NSIndexSet *)proposedSelectionIndexes inColumn:(NSInteger)column AVAILABLE_MAC_OS_X_VERSION_10_6_AND_LATER;

@end

#pragma mark -
#pragma mark **** Deprecated Methods ****

@interface NSBrowser(NSDeprecated)

/* This property has been non-functional since before 10.0.
 */
- (void)setAcceptsArrowKeys:(BOOL)flag DEPRECATED_IN_MAC_OS_X_VERSION_10_6_AND_LATER;
- (BOOL)acceptsArrowKeys DEPRECATED_IN_MAC_OS_X_VERSION_10_6_AND_LATER;

/* Use of -displayColumn: is deprecated in 10.3.  Use setNeedsDisplayInRect: instead.
 */
- (void)displayColumn:(NSInteger)column DEPRECATED_IN_MAC_OS_X_VERSION_10_3_AND_LATER;

/* Use of -displayAllColumns; is deprecated in 10.3.  Use setNeedsDisplay/InRect: instead.
 */
- (void)displayAllColumns DEPRECATED_IN_MAC_OS_X_VERSION_10_3_AND_LATER;

/* Use of -scrollViaScroller: is deprecated in 10.3.  Continuous scrolling no longer requires this functionality.
 */
- (void)scrollViaScroller:(NSScroller *)sender DEPRECATED_IN_MAC_OS_X_VERSION_10_3_AND_LATER;

/* Use of -updateScroller is deprecated in 10.3.  Continuous scrolling no longer requires this functionality.
 */
- (void)updateScroller DEPRECATED_IN_MAC_OS_X_VERSION_10_3_AND_LATER;

@end
