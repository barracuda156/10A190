/* CoreGraphics - CGFont.h
 * Copyright (c) 1999-2008 Apple Inc.
 * All rights reserved. */

#ifndef CGFONT_H_
#define CGFONT_H_

/* The type used to represent a CoreGraphics font. */

typedef struct CGFont *CGFontRef;

/* A type to represent indexes in a CGFontRef. */

typedef unsigned short CGFontIndex;

/* A type to represent glyph identifiers in a CGFontRef. */

typedef CGFontIndex CGGlyph;

/* The format of a PostScript font subset. Type1 is documented in "Adobe
   Type 1 Font Format"; Type3 in "PostScript Language Reference, 3rd ed."
   and Type42 in "Adobe Technical Note 5012, The Type 42 Font Format
   Specification". */

enum CGFontPostScriptFormat {
    kCGFontPostScriptFormatType1 = 1,
    kCGFontPostScriptFormatType3 = 3,
    kCGFontPostScriptFormatType42 = 42
};
typedef enum CGFontPostScriptFormat CGFontPostScriptFormat;

#include <CoreGraphics/CGBase.h>
#include <CoreGraphics/CGDataProvider.h>
#include <CoreGraphics/CGGeometry.h>
#include <CoreFoundation/CFArray.h>
#include <CoreFoundation/CFData.h>
#include <CoreFoundation/CFDictionary.h>

enum {
    /* The maximum allowed value of a CGFontIndex. Always <= USHRT_MAX - 1. */
    kCGFontIndexMax = ((1 << 16) - 2),

    /* A value representing an invalid CGFontIndex. Always <= USHRT_MAX. */
    kCGFontIndexInvalid = ((1 << 16) - 1),

    /* The maximum allowed value of a CGGlyph. */
    kCGGlyphMax = kCGFontIndexMax
};

/* Return the CFTypeID for CGFontRefs. */

CG_EXTERN CFTypeID CGFontGetTypeID(void)
    CG_AVAILABLE_STARTING(__MAC_10_2, __IPHONE_2_0);

/* Create a CGFontRef using `platformFontReference', a pointer to a
   platform-specific font reference. For MacOS X, `platformFontReference'
   should be a pointer to an ATSFontRef. */

CG_EXTERN CGFontRef CGFontCreateWithPlatformFont(void *platformFontReference)
    CG_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_NA);

/* Return the font defined by the data provided by `provider', or NULL if
   the font can't be created. */

CG_EXTERN CGFontRef CGFontCreateWithDataProvider(CGDataProviderRef provider)
    CG_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);

/* Return the font identified by `name', corresponding to the font's
   PostScript name or its full name, or NULL if the font can't be
   created. */

CG_EXTERN CGFontRef CGFontCreateWithFontName(CFStringRef name)
    CG_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);

/* Return a font based on `font' with the variation specification dictionary
   `variations' applied to `font'. A variation specification dictionary
   contains keys corresponding the variation axis names of the font. Each
   key is a variation axis name; the value for each key is the value
   specified for that particular variation axis represented as a
   CFNumberRef. If a variation axis name is not specified in `variations',
   then the current value from `font' is used. */

CG_EXTERN CGFontRef CGFontCreateCopyWithVariations(CGFontRef font,
    CFDictionaryRef variations)
    CG_AVAILABLE_STARTING(__MAC_10_4, __IPHONE_2_0);

/* Equivalent to `CFRetain(font)', except it doesn't crash (as CFRetain
   does) if `font' is NULL. */

CG_EXTERN CGFontRef CGFontRetain(CGFontRef font)
    CG_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_2_0);

/* Equivalent to `CFRelease(font)', except it doesn't crash (as CFRelease
   does) if `font' is NULL. */

CG_EXTERN void CGFontRelease(CGFontRef font)
    CG_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_2_0);

/* Return the number of glyphs in `font'. */

CG_EXTERN size_t CGFontGetNumberOfGlyphs(CGFontRef font)
    CG_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_2_0);

/* Return the glyph space units/em for `font'. */

CG_EXTERN int CGFontGetUnitsPerEm(CGFontRef font)
    CG_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_2_0);

/* Return the PostScript name of `font'. */

CG_EXTERN CFStringRef CGFontCopyPostScriptName(CGFontRef font)
    CG_AVAILABLE_STARTING(__MAC_10_4, __IPHONE_2_0);

/* Return the "full name" of `font'. */

CG_EXTERN CFStringRef CGFontCopyFullName(CGFontRef font)
    CG_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);

/* Return the ascent of `font'. The ascent is the maximum distance above the
   baseline of glyphs in a font. The value is specified in glyph space
   units. */

CG_EXTERN int CGFontGetAscent(CGFontRef font)
    CG_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);

/* Return the descent of `font'. The descent is the maximum distance below
   the baseline of glyphs in a font. The value is specified in glyph space
   units. */

CG_EXTERN int CGFontGetDescent(CGFontRef font)
    CG_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);

/* Return the leading of `font'. The leading is the spacing between
   consecutive lines of text in a font. The value is specified in glyph
   space units. */

CG_EXTERN int CGFontGetLeading(CGFontRef font)
    CG_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);

/* Return the cap height of `font'. The cap height is the distance above the
   baseline of the top of flat capital letters of glyphs in a font. The
   value is specified in glyph space units. */

CG_EXTERN int CGFontGetCapHeight(CGFontRef font)
    CG_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);

/* Return the x-height of `font'. The x-height is the distance above the
   baseline of the top of flat, non-ascending lowercase letters (such as
   "x") of glyphs in a font. The value is specified in glyph space units. */

CG_EXTERN int CGFontGetXHeight(CGFontRef font)
    CG_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);

/* Return the font bounding box of `font'. The font bounding box is the
   union of all of the bounding boxes for all the glyphs in a font. The
   value is specified in glyph space units. */

CG_EXTERN CGRect CGFontGetFontBBox(CGFontRef font)
    CG_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);

/* Return the italic angle of `font', measured in degrees counter-clockwise
   from the vertical. */

CG_EXTERN CGFloat CGFontGetItalicAngle(CGFontRef font)
    CG_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);

/* Return the thickness of the dominant vertical stems of glyphs in `font'.
   The value is specified in glyph space units. */

CG_EXTERN CGFloat CGFontGetStemV(CGFontRef font)
    CG_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);

/* Return an array of the variation axis dictionaries for `font'. Each
   variation axis dictionary contains values for the variation axis keys
   listed below. This function returns NULL if `font' doesn't support
   variations. */

CG_EXTERN CFArrayRef CGFontCopyVariationAxes(CGFontRef font)
    CG_AVAILABLE_STARTING(__MAC_10_4, __IPHONE_2_0);

/* Return the variation specification dictionary from `font'. This
   dictionary contains keys corresponding the variation axis names of the
   font. Each key is a variation axis name; the value for each key is the
   value specified for that particular variation axis represented as a
   CFNumberRef. This function returns NULL if `font' doesn't support
   variations. */

CG_EXTERN CFDictionaryRef CGFontCopyVariations(CGFontRef font)
    CG_AVAILABLE_STARTING(__MAC_10_4, __IPHONE_2_0);

/* Get the advance of each glyph in `glyphs', an array of `count' glyphs,
   and return it in the corresponding entry of `advances', an array of
   `count' integers. The advances are specified in glyph space. Returns
   false if advances can't be retrieved for any reason; true otherwise. */

CG_EXTERN bool CGFontGetGlyphAdvances(CGFontRef font, const CGGlyph glyphs[],
    size_t count, int advances[])
    CG_AVAILABLE_STARTING(__MAC_10_0, __IPHONE_2_0);

/* Get the bounding box of each glyph in `glyphs', an array of `count'
   glyphs, and return it in the corresponding entry of `bboxes', an array of
   `count' rectangles. The bounding boxes are specified in glyph space.
   Returns false if bounding boxes can't be retrieved for any reason; true
   otherwise. */

CG_EXTERN bool CGFontGetGlyphBBoxes(CGFontRef font, const CGGlyph glyphs[],
    size_t count, CGRect bboxes[])
    CG_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);

/* Return the glyph associated with `name' in `font'. If `name' isn't found
   in the font, return 0. */

CG_EXTERN CGGlyph CGFontGetGlyphWithGlyphName(CGFontRef font, CFStringRef name)
    CG_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);

/* Return the glyph name of `glyph' in `font', or NULL if `glyph' does not
   appear in `font'. */

CG_EXTERN CFStringRef CGFontCopyGlyphNameForGlyph(CGFontRef font,
    CGGlyph glyph) CG_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);

/* Return true if a subset in the PostScript format `format' can be created
   for `font'; false otherwise. */

CG_EXTERN bool CGFontCanCreatePostScriptSubset(CGFontRef font,
    CGFontPostScriptFormat format)
    CG_AVAILABLE_STARTING(__MAC_10_4, __IPHONE_2_0);

/* Create a subset of `font' named `subsetName' in the PostScript format
   `format'. The subset will contain the glyphs specified by `glyphs', an
   array of `count' CGGlyphs. If non-NULL, `encoding' specifies the default
   encoding for the subset. */

CG_EXTERN CFDataRef CGFontCreatePostScriptSubset(CGFontRef font,
    CFStringRef subsetName, CGFontPostScriptFormat format,
    const CGGlyph glyphs[], size_t count, const CGGlyph encoding[256])
    CG_AVAILABLE_STARTING(__MAC_10_4, __IPHONE_2_0);

/* Return a PostScript encoding of `font' containing glyphs in
   `encoding'. */

CG_EXTERN CFDataRef CGFontCreatePostScriptEncoding(CGFontRef font,
    const CGGlyph encoding[256])
    CG_AVAILABLE_STARTING(__MAC_10_4, __IPHONE_2_0);

/* Return an array of font table tags in `font'. Each entry in the array is
   a four-byte value representing a single TrueType or OpenType font table
   tag. */

CG_EXTERN CFArrayRef CGFontCopyTableTags(CGFontRef font)
    CG_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);

/* Return the table in `font' corresponding to `tag', or NULL if no such
   table exists. */

CG_EXTERN CFDataRef CGFontCopyTableForTag(CGFontRef font, uint32_t tag)
    CG_AVAILABLE_STARTING(__MAC_10_5, __IPHONE_2_0);

/*** Keys for the font variation axis dictionary. ***/

/* The key used to obtain the variation axis name from a variation axis
   dictionary. The value obtained with this key is a CFStringRef specifying
   the name of the variation axis. */

CG_EXTERN const CFStringRef kCGFontVariationAxisName
    CG_AVAILABLE_STARTING(__MAC_10_4, __IPHONE_2_0);

/* The key used to obtain the minimum variation axis value from a variation
   axis dictionary. The value obtained with this key is a CFNumberRef
   specifying the minimum value of the variation axis. */

CG_EXTERN const CFStringRef kCGFontVariationAxisMinValue
    CG_AVAILABLE_STARTING(__MAC_10_4, __IPHONE_2_0);

/* The key used to obtain the maximum variation axis value from a variation
   axis dictionary. The value obtained with this key is a CFNumberRef
   specifying the maximum value of the variation axis. */

CG_EXTERN const CFStringRef kCGFontVariationAxisMaxValue
    CG_AVAILABLE_STARTING(__MAC_10_4, __IPHONE_2_0);

/* The key used to obtain the default variation axis value from a variation
   axis dictionary. The value obtained with this key is a CFNumberRef
   specifying the default value of the variation axis. */

CG_EXTERN const CFStringRef kCGFontVariationAxisDefaultValue
    CG_AVAILABLE_STARTING(__MAC_10_4, __IPHONE_2_0);

/* Obsolete; don't use these. */

enum {
    CGGlyphMin = 0,
    CGGlyphMax = kCGGlyphMax
};

#endif /* CGFONT_H_ */
