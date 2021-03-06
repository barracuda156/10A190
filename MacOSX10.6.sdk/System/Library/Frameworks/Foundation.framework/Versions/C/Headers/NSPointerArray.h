/*
 *  NSPointerArray.h
 *  Copyright (c) 2005-2008, Apple Inc. All rights reserved.
 *
 */
 

#import <Foundation/NSObject.h>
#import <Foundation/NSArray.h>
#import <Foundation/NSPointerFunctions.h>

#if MAC_OS_X_VERSION_10_5 <= MAC_OS_X_VERSION_MAX_ALLOWED

/*
   NSPointerArray.h

   A PointerArray acts like a traditional array that slides elements on insertion or deletion.
   Unlike traditional arrays, it holds NULLs, which can be inserted or extracted (and contribute to count).
   Also unlike traditional arrays, the 'count' of the array may be set directly.
   Under Garbage Collection and using a zeroingWeakMemory configuration, NULLs will appear when elements are collected.

  Fast enumeration, copying, and archiving protocols are applicable only when NSPointerArray is configured for Object uses.
  Since the array may contain NULLs, fast enumeration (for..in) will yield NULLs.  As a convenience, fast enumeration will also enumerate other types of pointers.  Copying and coding are undefined for non-object uses.
*/

@interface NSPointerArray : NSObject <NSFastEnumeration, NSCopying, NSCoding>
// construction
- initWithOptions:(NSPointerFunctionsOptions)options;
- initWithPointerFunctions:(NSPointerFunctions *)functions;

+ pointerArrayWithOptions:(NSPointerFunctionsOptions)options;
+ pointerArrayWithPointerFunctions:(NSPointerFunctions *)functions;

/* return an NSPointerFunctions object reflecting the functions in use.  This is a new autoreleased object that can be subsequently modified and/or used directly in the creation of other pointer "collections". */
- (NSPointerFunctions *)pointerFunctions;

- (void *)pointerAtIndex:(NSUInteger)index;

// Array like operations that slide or grow contents, including NULLs
- (void)addPointer:(void *)pointer;  // add pointer at index 'count'
- (void)removePointerAtIndex:(NSUInteger)index;    // everything above index, including holes, slide lower
- (void)insertPointer:(void *)item atIndex:(NSUInteger)index;  // everything at & above index, including holes, slide higher

- (void)replacePointerAtIndex:(NSUInteger)index withPointer:(void *)item;  // O(1); NULL item is okay; index must be < count

- (void)compact;   // eliminate NULLs

- (NSUInteger)count;    // the number of elements in the array, including NULLs
- (void)setCount:(NSUInteger)count;    // set desired number of elements, adding NULLs or removing items as necessary.

@end





@interface NSPointerArray (NSArrayConveniences)  

// construction
+ pointerArrayWithStrongObjects; // strong objects
+ pointerArrayWithWeakObjects; // weak objects

- (NSArray *)allObjects;

@end

#endif
