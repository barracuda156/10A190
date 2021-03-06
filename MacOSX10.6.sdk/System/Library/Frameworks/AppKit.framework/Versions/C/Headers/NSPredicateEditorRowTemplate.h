/*
        NSPredicateEditorRowTemplate.h
	Application Kit
	Copyright (c) 2006-2008, Apple Inc.
	All rights reserved.
*/

#import <Foundation/NSObject.h>

#if MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_5

#import <Foundation/NSComparisonPredicate.h>
#import <CoreData/NSAttributeDescription.h>

@class NSPredicate, NSArray, NSEntityDescription;

@interface NSPredicateEditorRowTemplate : NSObject <NSCoding, NSCopying> {
    @private
    NSInteger _templateType;
    NSUInteger _predicateOptions;
    NSUInteger _predicateModifier;
    NSUInteger _leftAttributeType;
    NSUInteger _rightAttributeType;
    NSArray *_views;
    
    struct {
	unsigned leftIsWildcard:1;
	unsigned rightIsWildcard:1;
	unsigned reserved:30;
    } _ptFlags;
    
    id _patternReserved;
}

/* returns a positive number if the template can represent the predicate, and zero if it cannot.  The highest match determines which template is responsible for displaying the predicate.  Developers can override this to determine which predicates their custom template handles.  By default, this returns values in the range [0., 1.]
*/
- (double)matchForPredicate:(NSPredicate *)predicate;

/* returns the list of views that are placed in the row.  NSPopUpButtons are treated specially in that the items of multiple templates are merged together; other views are added as-is.  Developers can override this to return views in addition to or instead of the default views.
*/
- (NSArray *)templateViews;

/* sets the value of the views according to the given predicate.  This is only called if matchForPredicate: returns a positive value for the given predicate.  Developers can override this to set the values of their custom views.
*/
- (void)setPredicate:(NSPredicate *)predicate;

/* returns the predicate represented by the template's views' values and the given subpredicates.  Developers can override this to return the predicate represented by their custom views. */
- (NSPredicate *)predicateWithSubpredicates:(NSArray *)subpredicates;

/* for a given predicate, returns the subpredicates that should be made subrows.  For NSCompoundPredicate, this returns the array of subpredicates; it returns nil for other types of predicates.  This is only called if matchForPredicate: returns a positive value for the given predicate.  Developers can override this to create custom templates that handle complicated compound predicates.
*/ 
- (NSArray *)displayableSubpredicatesOfPredicate:(NSPredicate *)predicate;

/* creates a template of the popup-popup-popup form, with the left and right popups representing the left and right expression arrays, and the center popup representing the operators.
*/
- (id)initWithLeftExpressions:(NSArray *)leftExpressions rightExpressions:(NSArray *)rightExpressions modifier:(NSComparisonPredicateModifier)modifier operators:(NSArray *)operators options:(NSUInteger)options;

/* creates a template of the popup-popup-view form, with the left popups representing the left expressions, the right view representing an arbitrary value, and the center popup representing the operators.
*/
- (id)initWithLeftExpressions:(NSArray *)leftExpressions rightExpressionAttributeType:(NSAttributeType)attributeType modifier:(NSComparisonPredicateModifier)modifier operators:(NSArray *)operators options:(NSUInteger)options;

/* creates a template suitable for displaying compound predicates.  NSPredicateEditor contains such a template by default.
*/
- (id)initWithCompoundTypes:(NSArray *)compoundTypes;

/* returns the various values set in the initializers, or zero/nil if they do not apply
*/
- (NSArray *)leftExpressions;
- (NSArray *)rightExpressions;
- (NSAttributeType)rightExpressionAttributeType;
- (NSComparisonPredicateModifier)modifier;
- (NSArray *)operators;
- (NSUInteger)options;
- (NSArray *)compoundTypes;

/* CoreData convenience method: creates an array of templates of the popup-popup-view variety from the given key paths.  The key paths may cross relationships but must terminate in attributes.
*/
+ (NSArray *)templatesWithAttributeKeyPaths:(NSArray *)keyPaths inEntityDescription:(NSEntityDescription *)entityDescription;

@end

#endif
