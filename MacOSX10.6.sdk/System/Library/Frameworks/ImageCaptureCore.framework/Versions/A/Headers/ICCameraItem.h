//------------------------------------------------------------------------------------------------------------------------------
//
//  ICCameraItem.h
//  ImageCaptureCore
//
//  Copyright (c) 2008 Apple Inc., all rights reserved.
//
//  Best viewed with the following settings: Tab width 4, Indent width 2, Wrap lines, Indent wrapped lines by 3, Page guide 128.
//
//------------------------------------------------------------------------------------------------------------------------------
/*!
	@header ICCameraItem
    @abstract ICCameraItem is an abstract class that represents an item in an ICCameraDevice object. ICCameraDevice object creates instances of two concrete subclasses of ICCameraItem: ICCameraFolder and ICCameraFile.
*/

//------------------------------------------------------------------------------------------------------------------------------

#import <ImageCaptureCore/ICCommonConstants.h>

//------------------------------------------------------------------------------------------------------------------------------

@class ICCameraDevice;
@class ICCameraFolder;

//----------------------------------------------------------------------------------------------------------------- ICCameraItem
/*! 
  @class ICCameraItem
  @abstract ICCameraItem is an abstract class that represents an item in an ICCameraDevice object. ICCameraDevice object creates instances of two concrete subclasses of ICCameraItem: ICCameraFolder and ICCameraFile.
*/

@interface ICCameraItem : NSObject
{
@private
    id  _itemProperties;
}

/*!
    @property device
    @abstract ￼Parent device of this folder.

*/
@property(readonly)                             ICCameraDevice* device;

/*!
    @property parentFolder
    @abstract ￼Parent folder of this folder. The root folder's parentFolder is nil.

*/
@property(readonly)                             ICCameraFolder* parentFolder;

/*!
    @property name
    @abstract ￼Name of this folder.

*/
@property(readonly)                             NSString*       name;

/*!
    @property UTI
    @abstract ￼Item UTI. This is an Uniform Type Identifier string. It is one of: kUTTypeFolder, kUTTypeImage, kUTTypeMovie, kUTTypeAudio, or kUTTypeData.

*/
@property(readonly)                             NSString*       UTI;

/*!
    @property locked
    @abstract ￼Indicates the protection state of this folder. It is locked if the storage card in the camera is locked.

*/
@property(readonly, getter=isLocked)            BOOL            locked;

/*!
    @property raw
    @abstract ￼Indicates if the file is a raw image file.

*/
@property(readonly, getter=isRaw)            BOOL               raw;

/*!
    @property inTemporaryStore
    @abstract ￼Indicates if this folder is in a temporary store. A temporary store may be used by the device when images are captures on the device when it is tethered to the computer.

*/
@property(readonly, getter=isInTemporaryStore)  BOOL            inTemporaryStore;

/*!
    @property creationDate
    @abstract ￼Creation date of this file. This information is usually the same as the EXIF creation date.

*/
@property(readonly)                             NSDate*         creationDate;

/*!
    @property modificationDate
    @abstract ￼Modification date of this file. This information is usually the same as the EXIF modification date.

*/
@property(readonly)                             NSDate*         modificationDate;

/*!
    @property thumbnailIfAvailable
    @abstract ￼Thumbnail for the item if one is readily available. If one is not readily available, accessing this property will send a message to the device requesting a thumbnail for the file. The delegate of the device will be notified via method "cameraDevice:didReceiveThumbnailForItem:", if this method is implemented on by the delegate.

*/
@property(readonly)                             CGImageRef      thumbnailIfAvailable;

/*!
    @property metadataIfAvailable
    @abstract ￼Metadata for the file if one is readily available. If one is not readily available, accessing this property will send a message to the device requesting a thumbnail for the file. The delegate of the device will be notified via method "cameraDevice:didReceiveMetadataForItem:", if this method is implemented on by the delegate.

*/
@property(readonly)                             NSDictionary*   metadataIfAvailable;

@end

//--------------------------------------------------------------------------------------------------------------- ICCameraFolder
/*! 
  @class ICCameraFolder
  @abstract This class represents a folder on an ICCameraDevice object.
*/

@interface ICCameraFolder : ICCameraItem
{
@private
    id  _folderProperties;
}

/*!
    @property contents
    @abstract ￼A list of items contained by this folder.

*/
@property(readonly)   NSArray*      contents;

@end

//----------------------------------------------------------------------------------------------------------------- ICCameraFile
/*! 
  @class ICCameraFile
  @abstract This class represents a file on an ICCameraDevice object.
*/

@interface ICCameraFile : ICCameraItem
{
@private
    id  _fileProperties;
}

/*!
    @property fileSize
    @abstract ￼Size of file in bytes.

*/
@property(readonly)   off_t                 fileSize;

/*!
    @property orientation
    @abstract ￼Desired orientation of image to use when it is downloaded.
    @discussion This property is set to ICEXIFOrientation1 initially. If the format of this file supports EXIF orientation tag, then this property will be updated to match the value of that tag, when the thumbnail or metadata for this file is received. 

*/
@property(readwrite)  ICEXIFOrientationType orientation;

/*!
    @property userData
    @abstract ￼A mutable dictionary to store arbitrary key-value pairs associated with a file object. This can be used by view objects that bind to a file object to store "house-keeping" information.

*/
@property(readonly)   NSMutableDictionary*  userData;

@end

//------------------------------------------------------------------------------------------------------------------------------

