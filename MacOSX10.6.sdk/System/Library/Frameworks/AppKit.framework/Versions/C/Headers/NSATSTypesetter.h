/*
        NSATSTypesetter.h
        Application Kit
        Copyright (c) 2002-2008, Apple Inc.
        All rights reserved.
*/

#import <AppKit/NSTypesetter.h>
#import <AppKit/NSParagraphStyle.h>

#if MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_3
@interface NSATSTypesetter : NSTypesetter {
/* Ivars used for primitive typesetting */
    NSAttributedString *attributedString; // The text backing-store (usually NSTextStorage)

    NSRange paragraphGlyphRange; // The current glyph range being processed

    NSRange paragraphSeparatorGlyphRange; // The current paragraph separator range (the length == 0 at the end of document)

    CGFloat lineFragmentPadding; // The margin on both sides of lines

/* Ivars for the Cocoa Text System interface */
    NSLayoutManager *layoutManager;
    NSArray *textContainers;

/* Cached NSTextContainer settings */
    NSTextContainer *currentTextContainer;
    NSUInteger currentTextContainerIndex;
    NSSize currentTextContainerSize;

/* Cached NSParagraphStyle */
    NSParagraphStyle *currentParagraphStyle;

@private
    void *_atsReserved[8];
    id _private;
}

// Factory methods
+ (id)sharedTypesetter;
@end

@interface NSATSTypesetter (NSPantherCompatibility)
// This interface is deprecated.
// Use -getLineFragmentRect:usedRect:forStartingGlyphAtIndex:proposedRect:lineSpacing:paragraphSpacingBefore:paragraphSpacingAfter:remainingRect: instead
- (NSRect)lineFragmentRectForProposedRect:(NSRect)proposedRect remainingRect:(NSRectPointer)remainingRect DEPRECATED_IN_MAC_OS_X_VERSION_10_4_AND_LATER;
@end

#if MAC_OS_X_VERSION_MIN_REQUIRED < MAC_OS_X_VERSION_10_4
// The following interfaces are moved to the abstract NSTypesetter class

@interface NSATSTypesetter (NSPrimitiveInterface)
/* Privmitive typesetting methods */
// NSLayoutManager attributes
- (BOOL)usesFontLeading;
- (void)setUsesFontLeading:(BOOL)flag;
- (NSTypesetterBehavior)typesetterBehavior;
- (void)setTypesetterBehavior:(NSTypesetterBehavior)behavior;
- (float)hyphenationFactor;
- (void)setHyphenationFactor:(float)factor;

// NSTextContainer attributes
- (CGFloat)lineFragmentPadding;
- (void)setLineFragmentPadding:(CGFloat)padding;

// Screen/printer font mapping
- (NSFont *)substituteFontForFont:(NSFont *)originalFont;

// Tab stops
- (NSTextTab *)textTabForGlyphLocation:(CGFloat)glyphLocation writingDirection:(NSWritingDirection)direction maxLocation:(CGFloat)maxLocation;

// Bidi control
- (BOOL)bidiProcessingEnabled;
- (void)setBidiProcessingEnabled:(BOOL)flag; // Controls whether to perform bi-directional processing.  You can disable the layout stage if you know the parapgraph does not need this stage (i.e. the backing-store is in the display order) in -fillAttributesForGlyphsInRange:andParagraphSeparatorRange:.

// Accessors for required paragraph typesetting ivars (attributedString, paragraphGlyphRange, paragraphSeparatorGlyphRange
- (void)setAttributedString:(NSAttributedString *)attrString; // Note this method does not retain attrString
- (NSAttributedString *)attributedString;
- (void)setParagraphGlyphRange:(NSRange)paragraphRange separatorGlyphRange:(NSRange)paragraphSeparatorRange;
- (NSRange)paragraphGlyphRange;
- (NSRange)paragraphSeparatorGlyphRange;

// layout primitive
- (NSUInteger)layoutParagraphAtPoint:(NSPoint *)lineFragmentOrigin; // lineFragmentOrigin specifies the upper-left corner of line fragment rect.  On return, set to the next origin. The method returns the next glyph index. Usually the index right after paragraph separator but can be inside the paragraph range (i.e. text container exhaustion)

// Line/paragraph spacing
- (CGFloat)lineSpacingAfterGlyphAtIndex:(NSUInteger)glyphIndex withProposedLineFragmentRect:(NSRect)rect;
- (CGFloat)paragraphSpacingBeforeGlyphAtIndex:(NSUInteger)glyphIndex withProposedLineFragmentRect:(NSRect)rect;
- (CGFloat)paragraphSpacingAfterGlyphAtIndex:(NSUInteger)glyphIndex withProposedLineFragmentRect:(NSRect)rect;

/* Cocoa Text System interface methods */

// Friend classes
- (NSLayoutManager *)layoutManager;
- (NSTextContainer *)currentTextContainer;

// Forces NSLayoutManager to invalidate glyph cache in range when invalidating layout
- (void)setHardInvalidation:(BOOL)flag forGlyphRange:(NSRange)glyphRange;

// This method calculates the line fragment rect/line fragment used rect for blank lines
// theParagraphSeparatorGlyphRange with length == 0 indicates extra line fragment
- (void)getLineFragmentRect:(NSRect *)lineFragmentRect usedRect:(NSRect *)lineFragmentUsedRect forParagraphSeparatorGlyphRange:(NSRange)paragraphSeparatorGlyphRange atProposedOrigin:(NSPoint)lineOrigin;
@end

/* NSLayoutPhaseInterface declares various subclass override points that are invoked if implemented */
@interface NSATSTypesetter (NSLayoutPhaseInterface)
// Called right before setLineFragmentRect:forGlyphRange:usedRect:
- (void)willSetLineFragmentRect:(NSRect *)lineRect forGlyphRange:(NSRange)glyphRange usedRect:(NSRect *)usedRect baselineOffset:(CGFloat *)baselineOffset;

- (BOOL)shouldBreakLineByWordBeforeCharacterAtIndex:(NSUInteger)charIndex;
- (BOOL)shouldBreakLineByHyphenatingBeforeCharacterAtIndex:(NSUInteger)charIndex;

- (float)hyphenationFactorForGlyphAtIndex:(NSUInteger)glyphIndex;
- (UTF32Char)hyphenCharacterForGlyphAtIndex:(NSUInteger)glyphIndex;

- (NSRect)boundingBoxForControlGlyphAtIndex:(NSUInteger)glyphIndex forTextContainer:(NSTextContainer *)textContainer proposedLineFragment:(NSRect)proposedRect glyphPosition:(NSPoint)glyphPosition characterIndex:(NSUInteger)charIndex;
@end

/* NSGlyphStorageInterface declares all primitives interfacing to the glyph storage (usually NSLayoutManager). By overriding all the methods, you can implement an NSATSTypesetter subclass that interacts with custom glyph storage. */
@interface NSATSTypesetter (NSGlyphStorageInterface)
// Glyph/character range mappings
- (NSRange)characterRangeForGlyphRange:(NSRange)glyphRange actualGlyphRange:(NSRangePointer)actualGlyphRange;
- (NSRange)glyphRangeForCharacterRange:(NSRange)charRange actualCharacterRange:(NSRangePointer)actualCharRange;

// Glyph data
- (NSUInteger)getGlyphsInRange:(NSRange)glyphsRange glyphs:(NSGlyph *)glyphBuffer characterIndexes:(NSUInteger *)charIndexBuffer glyphInscriptions:(NSGlyphInscription *)inscribeBuffer elasticBits:(BOOL *)elasticBuffer;

// Layout storage
- (void)setLineFragmentRect:(NSRect)fragmentRect forGlyphRange:(NSRange)glyphRange usedRect:(NSRect)usedRect baselineOffset:(CGFloat)baselineOffset;
- (void)substituteGlyphsInRange:(NSRange)glyphRange withGlyphs:(NSGlyph *)glyphs;
- (void)insertGlyph:(NSGlyph)glyph atGlyphIndex:(NSUInteger)glyphIndex characterIndex:(NSUInteger)characterIndex;
- (void)deleteGlyphsInRange:(NSRange)glyphRange;
- (void)setNotShownAttribute:(BOOL)flag forGlyphRange:(NSRange)glyphRange;
- (void)setDrawsOutsideLineFragment:(BOOL)flag forGlyphRange:(NSRange)glyphRange;
- (void)setLocation:(NSPoint)location withAdvancements:(const CGFloat *)advancements forStartOfGlyphRange:(NSRange)glyphRange;
- (void)setAttachmentSize:(NSSize)attachmentSize forGlyphRange:(NSRange)glyphRange;
- (void)setBidiLevels:(const uint8_t *)levels forGlyphRange:(NSRange)glyphRange;
@end
#endif /* MAC_OS_X_VERSION_MIN_REQUIRED < MAC_OS_X_VERSION_10_4 */
#endif /* MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_3 */

