/*
	NSDictionaryController.h
	Application Kit
	Copyright (c) 2002-2008, Apple Inc.
	All rights reserved.
 */

#import <AppKit/NSArrayController.h>

#if MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_5

@class NSDictionary;

// NSDictionaryController transforms a dictionary into an array of key-value pairs that is displayed like any other array in an NSArrayController. For each key-value pair, the controller distinguishes between the actual "key" and the "localizedKey" (a user readable version of the key - see setLocalizedKeyDictionary:/-localizedKeyDictionary). If no localizedKeyDictionary is specified, the localized keys default to the keys.
// In general, the dictionary controller's validation logic will prevent duplicate keys (but developers need to turn on "Validates Immediately" for the value bindings of the controls editing keys to run that validation). For insertions of new entries, the controller will enumerate the key by default (so if the initialKey is "key", the inserted keys will be "key", "key1", "key2", and so on). To customize that behavior, override the -newObject method.

// The key-value pair objects respond to the following methods (the class itself is private), so controls can be bound for example to arrangedObjects.localizedKey, arrangedObjects.key, arrangedObjects.value, and arrangedObjects.explicitlyIncluded of the controller. Mutating a key-value-pair object immediately results in the corresponding change in the content dictionary of the controller. 
@interface NSObject (NSDictionaryControllerKeyValuePair)

- (void)setLocalizedKey:(NSString *)localizedKey;
- (NSString *)localizedKey;
- (void)setKey:(NSString *)key;
- (NSString *)key;
- (void)setValue:(id)value;
- (id)value;
- (BOOL)isExplicitlyIncluded;

@end

@interface NSDictionaryController : NSArrayController {
@private
    void *_reserved5;
    void *_reserved6;
    void *_reserved7;
	id _contentDictionary;
	NSString *_initialKey;
	id _initialValue;
	NSUInteger _minimumInsertionKeyIndex;
	NSString *_localizedKeyStringsFileName;
	NSDictionary *_localizedKeyForKeyDictionary;
	NSDictionary *_keyForLocalizedKeyDictionary;
	NSArray *_includedKeys;
	NSArray *_excludedKeys;
    struct __dictionaryControllerFlags {
        unsigned int _deepCopiesValues:1;
        unsigned int _suppressBuildingDictionary:1;
        unsigned int _reservedDictionaryController:30;
    } _dictionaryControllerFlags;
}

- (id)newObject;    // overridden from the superclass to create a key-value pair to represent an entry in the content dictionary of the controller; method is invoked for insertions of new key-value pairs as well as transforming existing dictionary entries into key-value pairs for display

// Inital key and value are assigned to newly inserted entries. The initial key will be copied, the initial value will be retained when inserted into the dictionary (not copied), and must implement NSCoding if the dictionary controller gets archived.
- (void)setInitialKey:(NSString *)key;
- (NSString *)initialKey;
- (void)setInitialValue:(id)value;
- (id)initialValue;

// Included keys are always represented by a key-value pair in the display array, whether present in the underlying dictionary or not. Excluded keys are always suppressed in the display array.
- (void)setIncludedKeys:(NSArray *)keys;
- (NSArray *)includedKeys;
- (void)setExcludedKeys:(NSArray *)keys;
- (NSArray *)excludedKeys;

// Localized key dictionary allows to specify a (typically localized) string for each key in the dictionary (the dictionary needs to contain the keys as keys and the localized keys as values).
- (void)setLocalizedKeyDictionary:(NSDictionary *)dictionary;
- (NSDictionary *)localizedKeyDictionary;
- (void)setLocalizedKeyTable:(NSString *)stringsFileName;
- (NSString *)localizedKeyTable;

@end

#endif
