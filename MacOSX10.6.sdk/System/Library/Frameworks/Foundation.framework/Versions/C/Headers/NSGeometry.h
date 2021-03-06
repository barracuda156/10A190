/*	NSGeometry.h
	Copyright (c) 1994-2008, Apple Inc. All rights reserved.
*/

#import <AvailabilityMacros.h>
#import <Foundation/NSValue.h>
#import <Foundation/NSCoder.h>

#if (TARGET_OS_EMBEDDED || TARGET_OS_IPHONE)
#import <CoreGraphics/CGBase.h>
#import <CoreGraphics/CGGeometry.h>
#elif TARGET_OS_WIN32
#import <CGCompat.h>
#elif TARGET_OS_MAC
#import <ApplicationServices/../Frameworks/CoreGraphics.framework/Headers/CGBase.h>
#import <ApplicationServices/../Frameworks/CoreGraphics.framework/Headers/CGGeometry.h>
#endif

#if __LP64__ || TARGET_OS_EMBEDDED || TARGET_OS_IPHONE || TARGET_OS_WIN32 || NS_BUILD_32_LIKE_64

typedef CGPoint NSPoint;

typedef NSPoint *NSPointPointer;
typedef NSPoint *NSPointArray;

typedef CGSize NSSize;

typedef NSSize *NSSizePointer;
typedef NSSize *NSSizeArray;

typedef CGRect NSRect;

typedef NSRect *NSRectPointer;
typedef NSRect *NSRectArray;

#define NSMinXEdge CGRectMinXEdge
#define NSMinYEdge CGRectMinYEdge
#define NSMaxXEdge CGRectMaxXEdge
#define NSMaxYEdge CGRectMaxYEdge

typedef NSUInteger NSRectEdge;

#define NSGEOMETRY_TYPES_SAME_AS_CGGEOMETRY_TYPES 1

#else

typedef struct _NSPoint {
    CGFloat x;
    CGFloat y;
} NSPoint;

typedef NSPoint *NSPointPointer;
typedef NSPoint *NSPointArray;

typedef struct _NSSize {
    CGFloat width;		/* should never be negative */
    CGFloat height;		/* should never be negative */
} NSSize;

typedef NSSize *NSSizePointer;
typedef NSSize *NSSizeArray;

typedef struct _NSRect {
    NSPoint origin;
    NSSize size;
} NSRect;

typedef NSRect *NSRectPointer;
typedef NSRect *NSRectArray;

typedef enum {
    NSMinXEdge = 0,
    NSMinYEdge = 1,
    NSMaxXEdge = 2,
    NSMaxYEdge = 3	
} NSRectEdge;

#endif

@class NSString;

FOUNDATION_EXPORT const NSPoint NSZeroPoint;
FOUNDATION_EXPORT const NSSize NSZeroSize;
FOUNDATION_EXPORT const NSRect NSZeroRect;

NS_INLINE NSPoint NSMakePoint(CGFloat x, CGFloat y) {
    NSPoint p;
    p.x = x;
    p.y = y;
    return p;
}

NS_INLINE NSSize NSMakeSize(CGFloat w, CGFloat h) {
    NSSize s;
    s.width = w;
    s.height = h;
    return s;
}

NS_INLINE NSRect NSMakeRect(CGFloat x, CGFloat y, CGFloat w, CGFloat h) {
    NSRect r;
    r.origin.x = x;
    r.origin.y = y;
    r.size.width = w;
    r.size.height = h;
    return r;
}

NS_INLINE CGFloat NSMaxX(NSRect aRect) {
    return (aRect.origin.x + aRect.size.width);
}

NS_INLINE CGFloat NSMaxY(NSRect aRect) {
    return (aRect.origin.y + aRect.size.height);
}

NS_INLINE CGFloat NSMidX(NSRect aRect) {
    return (aRect.origin.x + aRect.size.width * (CGFloat)0.5);
}

NS_INLINE CGFloat NSMidY(NSRect aRect) {
    return (aRect.origin.y + aRect.size.height * (CGFloat)0.5);
}

NS_INLINE CGFloat NSMinX(NSRect aRect) {
    return (aRect.origin.x);
}

NS_INLINE CGFloat NSMinY(NSRect aRect) {
    return (aRect.origin.y);
}

NS_INLINE CGFloat NSWidth(NSRect aRect) {
    return (aRect.size.width);
}

NS_INLINE CGFloat NSHeight(NSRect aRect) {
    return (aRect.size.height);
}

#if MAC_OS_X_VERSION_10_5 <= MAC_OS_X_VERSION_MAX_ALLOWED

NS_INLINE NSRect NSRectFromCGRect(CGRect cgrect) {
    union _ {NSRect ns; CGRect cg;};
    return ((union _ *)&cgrect)->ns;
}

NS_INLINE CGRect NSRectToCGRect(NSRect nsrect) {
    union _ {NSRect ns; CGRect cg;};
    return ((union _ *)&nsrect)->cg;
}

NS_INLINE NSPoint NSPointFromCGPoint(CGPoint cgpoint) {
    union _ {NSPoint ns; CGPoint cg;};
    return ((union _ *)&cgpoint)->ns;
}

NS_INLINE CGPoint NSPointToCGPoint(NSPoint nspoint) {
    union _ {NSPoint ns; CGPoint cg;};
    return ((union _ *)&nspoint)->cg;
}

NS_INLINE NSSize NSSizeFromCGSize(CGSize cgsize) {
    union _ {NSSize ns; CGSize cg;};
    return ((union _ *)&cgsize)->ns;
}

NS_INLINE CGSize NSSizeToCGSize(NSSize nssize) {
    union _ {NSSize ns; CGSize cg;};
    return ((union _ *)&nssize)->cg;
}

#endif

FOUNDATION_EXPORT BOOL NSEqualPoints(NSPoint aPoint, NSPoint bPoint);
FOUNDATION_EXPORT BOOL NSEqualSizes(NSSize aSize, NSSize bSize);
FOUNDATION_EXPORT BOOL NSEqualRects(NSRect aRect, NSRect bRect);
FOUNDATION_EXPORT BOOL NSIsEmptyRect(NSRect aRect);

FOUNDATION_EXPORT NSRect NSInsetRect(NSRect aRect, CGFloat dX, CGFloat dY);
FOUNDATION_EXPORT NSRect NSIntegralRect(NSRect aRect);
FOUNDATION_EXPORT NSRect NSUnionRect(NSRect aRect, NSRect bRect);
FOUNDATION_EXPORT NSRect NSIntersectionRect(NSRect aRect, NSRect bRect);
FOUNDATION_EXPORT NSRect NSOffsetRect(NSRect aRect, CGFloat dX, CGFloat dY);
FOUNDATION_EXPORT void NSDivideRect(NSRect inRect, NSRect *slice, NSRect *rem, CGFloat amount, NSRectEdge edge);
FOUNDATION_EXPORT BOOL NSPointInRect(NSPoint aPoint, NSRect aRect);
FOUNDATION_EXPORT BOOL NSMouseInRect(NSPoint aPoint, NSRect aRect, BOOL flipped);
FOUNDATION_EXPORT BOOL NSContainsRect(NSRect aRect, NSRect bRect);
FOUNDATION_EXPORT BOOL NSIntersectsRect(NSRect aRect, NSRect bRect);

FOUNDATION_EXPORT NSString *NSStringFromPoint(NSPoint aPoint);
FOUNDATION_EXPORT NSString *NSStringFromSize(NSSize aSize);
FOUNDATION_EXPORT NSString *NSStringFromRect(NSRect aRect);
FOUNDATION_EXPORT NSPoint NSPointFromString(NSString *aString);
FOUNDATION_EXPORT NSSize NSSizeFromString(NSString *aString);
FOUNDATION_EXPORT NSRect NSRectFromString(NSString *aString);

@interface NSValue (NSValueGeometryExtensions)

+ (NSValue *)valueWithPoint:(NSPoint)point;
+ (NSValue *)valueWithSize:(NSSize)size;
+ (NSValue *)valueWithRect:(NSRect)rect;

- (NSPoint)pointValue;
- (NSSize)sizeValue;
- (NSRect)rectValue;

@end

@interface NSCoder (NSGeometryCoding)

- (void)encodePoint:(NSPoint)point;
- (NSPoint)decodePoint;

- (void)encodeSize:(NSSize)size;
- (NSSize)decodeSize;

- (void)encodeRect:(NSRect)rect;
- (NSRect)decodeRect;

@end

@interface NSCoder (NSGeometryKeyedCoding)

- (void)encodePoint:(NSPoint)point forKey:(NSString *)key;
- (void)encodeSize:(NSSize)size forKey:(NSString *)key;
- (void)encodeRect:(NSRect)rect forKey:(NSString *)key;

- (NSPoint)decodePointForKey:(NSString *)key;
- (NSSize)decodeSizeForKey:(NSString *)key;
- (NSRect)decodeRectForKey:(NSString *)key;

@end

