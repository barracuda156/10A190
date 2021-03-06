/*------------------------------------------------------------------------------------------------------------------------------
 *
 *  ICADevices/ICD_CameraCalls.h
 *
 *  Copyright (c) 2004-2006 Apple Computer, Inc. All rights reserved.
 *
 *  For bug reports, consult the following page onthe World Wide Web:
 *  http://developer.apple.com/bugreporter/
 *
 *----------------------------------------------------------------------------------------------------------------------------*/

#pragma once

#ifndef __ICD_CameraCalls__
#define __ICD_CameraCalls__

//------------------------------------------------------------------------------------------------------------------------------

#include <Carbon/Carbon.h>
#include <CoreFoundation/CoreFoundation.h>
#import <IOBluetooth/Bluetooth.h>

//------------------------------------------------------------------------------------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(push, 2)

//------------------------------------------------------------------------------------------------------------------------------
/* DataTypes for _ICD_ReadFileData/_ICD_WriteFileData */

enum {
    kICD_FileData                 = 'file',
    kICD_MetaData                 = 'meta',
    kICD_ThumbnailData            = 'thum',     // Deprecated in 10.5
    kICD_ThumbnailDataFormatJPEG  = 'tjpg',     // Deprecated in 10.5.
                                                // Use kICAThumbnailFormatJPEG or kICAThumbnailFormatJPEGAutoRotated instead.
    kICD_ThumbnailDataFormatTIFF  = 'ttif',     // Deprecated in 10.5.
                                                // Use kICAThumbnailFormatTIFF or kICAThumbnailFormatTIFFAutoRotated instead.
    kICD_ThumbnailDataFormatPNG   = 'tpng'      // Deprecated in 10.5. 
                                                // Use kICAThumbnailFormatPNG instead.
};

//------------------------------------------------------------------------------------------------------------------------------
/* flags */

enum
{
	hasChildrenMask    = 0x00000001,
	hasThumbnailMask   = 0x00000002,
    fileLockedMask     = 0x00000004,
    rawImageFormatMask = 0x00000008
};

//------------------------------------------------------------------------------------------------------------------------------

typedef struct ObjectInfo
{
    ICAObject		icaObject;			// Apple
    unsigned long	reserved;			// Apple	
    ICAObjectInfo 	icaObjectInfo;		// vendor
    UInt32			uniqueID;			// vendor
    UInt32			thumbnailSize;		// vendor
    UInt32			dataSize;			// vendor
    UInt32 			dataWidth;			// vendor
    UInt32 			dataHeight;			// vendor
    UInt8			name[32];			// vendor
    UInt8			creationDate[20];	// vendor
    UInt32			flags;				// vendor
    Ptr				privateData;		// vendor
    UInt64			uniqueIDFireWire;	// vendor
    UInt32          tag;                // Apple
} ObjectInfo;

//------------------------------------------------------------------------------------------------------------------------------

struct ICD_GetPropertyDataPB {
    ICDHeader                       header;

    ICAObject                       parentObject;               /* <-- */
    ICAObjectInfo                   parentObjectInfo;           /* <-- */
    ICAConnectionID            	  	connectionID;               /* <-- */
    ICAProperty                     property;                   /* <-- */
    ICAPropertyInfo               	propertyInfo;               /* <-- */
    UInt32                          startByte;                  /* <-- */
    UInt32                          requestedSize;              /* <-- */

    void *                          dataPtr;                    /* <-> */

    UInt32                          actualSize;                 /* --> */
    OSType                          dataType;                   /* --> */
};
typedef struct ICD_GetPropertyDataPB    ICD_GetPropertyDataPB;

//------------------------------------------------------------------------------------------------------------------------------

struct ICD_SetPropertyDataPB {
    ICDHeader                       header;

    ICAObject                       parentObject;               /* <-- */
    ICAObjectInfo                   parentObjectInfo;           /* <-- */
    ICAConnectionID            	  	connectionID;               /* <-- */
    ICAProperty                     property;                   /* <-- */
    ICAPropertyInfo               	propertyInfo;               /* <-- */
    UInt32                          startByte;                  /* <-- */
    void *                          dataPtr;                    /* <-- */
    UInt32                          dataSize;                   /* <-- */
    UInt32                          totalDataSize;              /* <-- */
    OSType                          dataType;                   /* <-- */
};
typedef struct ICD_SetPropertyDataPB    ICD_SetPropertyDataPB;

//------------------------------------------------------------------------------------------------------------------------------

struct ICD_ObjectSendMessagePB {
    ICDHeader                       header;

    ICAObject                       object;                     /* <-- */
    ICAObjectInfo                   objectInfo;     		    /* <-- */
    ICAConnectionID            	  	connectionID;               /* <-- */
    ICAMessage                      message;                    /* <-- */
    UInt32                          totalDataSize;              /* <-- */
    UInt32                      	result;                     /* --> */
};
typedef struct ICD_ObjectSendMessagePB  ICD_ObjectSendMessagePB;

//------------------------------------------------------------------------------------------------------------------------------

typedef CALLBACK_API_C(ICAError, __ICD_OpenUSBDevice)
                                    (UInt32 locationID, ObjectInfo* objectInfo);
    
typedef CALLBACK_API_C(ICAError, __ICD_OpenUSBDeviceWithIORegPath)
                                    (UInt32 locationID, io_string_t ioregPath, ObjectInfo* objectInfo);
    
typedef CALLBACK_API_C(ICAError, __ICD_OpenFireWireDevice)
                                    (UInt64 guid, ObjectInfo* objectInfo);

typedef CALLBACK_API_C(ICAError, __ICD_OpenFireWireDeviceWithIORegPath)
                                    (UInt64 guid, io_string_t ioregPath, ObjectInfo* objectInfo);

typedef CALLBACK_API_C(ICAError, __ICD_OpenBluetoothDevice)
                                    (CFDictionaryRef params, ObjectInfo* objectInfo);

typedef CALLBACK_API_C(ICAError, __ICD_OpenTCPIPDevice)
                                    (CFDictionaryRef params, ObjectInfo* objectInfo);

typedef CALLBACK_API_C(ICAError, __ICD_OpenMassStorageDevice)
                                    (CFStringRef diskBSDName, DASessionRef daSession, ObjectInfo* objectInfo);

typedef CALLBACK_API_C(ICAError, __ICD_CloseDevice)
                                    (ObjectInfo* objectInfo);
        
typedef CALLBACK_API_C(ICAError, __ICD_PeriodicTask)
                                    (ObjectInfo* objectInfo);

/* index is zero based */
typedef CALLBACK_API_C(ICAError, __ICD_GetObjectInfo)
                                    (const ObjectInfo* parentInfo, UInt32 index, ObjectInfo* newInfo);
                            
typedef CALLBACK_API_C(ICAError, __ICD_Cleanup)
                                    (ObjectInfo* objectInfo);
                                
typedef CALLBACK_API_C(ICAError, __ICD_GetPropertyData)
                                    (const ObjectInfo* objectInfo, ICD_GetPropertyDataPB* pb);
                            
typedef CALLBACK_API_C(ICAError, __ICD_SetPropertyData)
                                    (const ObjectInfo* objectInfo, const ICD_SetPropertyDataPB* pb);
                            
typedef CALLBACK_API_C(ICAError, __ICD_ReadFileData)
                                    (const ObjectInfo* objectInfo, UInt32 dataType, Ptr buffer, UInt32 offset, UInt32* length);
    
typedef CALLBACK_API_C(ICAError, __ICD_WriteFileData)
                                    (const ObjectInfo* parentInfo, const char* filename, UInt32 dataType, Ptr buffer, UInt32 offset, UInt32* length);
    
typedef CALLBACK_API_C(ICAError, __ICD_SendMessage)
                                    (const ObjectInfo* objectInfo, ICD_ObjectSendMessagePB* pb, ICDCompletion completion);
                            
typedef CALLBACK_API_C(ICAError, __ICD_AddPropertiesToCFDictionary)
                                    (ObjectInfo* objectInfo, CFMutableDictionaryRef dict);

typedef CALLBACK_API_C(ICAError, __ICD_WriteDataToFile)
                                    (const ObjectInfo* objectInfo, FILE* file, UInt32 offset, long* length);

// camera related callBacks into the ICADevices.framework:

int ICD_main (int argc, const char* argv[]);

ICAError ICDGetStandardPropertyData(const ObjectInfo* objectInfo, ICD_GetPropertyDataPB* pb);
ICAError ICDNewObjectInfoCreated(const ObjectInfo* parentInfo, UInt32 index, ICAObject* newICAObject);

typedef CALLBACK_API_C( void , ICDNewObjectCreatedCompletion )(const ObjectInfo* info);

ICAError ICDNewObjectCreated(const ObjectInfo* parentInfo, const ObjectInfo* objectInfo, ICDNewObjectCreatedCompletion completion);

// name of the device (from device's objectInfo)
// this CFDictionaryRef contains information about the camera, e.g. the iconFile,...

ICAError ICDCopyDeviceInfoDictionary( const char* deviceName, CFDictionaryRef* theDict );


ICAError ICDCreateICAThumbnailFromICNS(const char* fileName,		// filename for .icns icon file
                                    ICAThumbnail* thumbnail);		// pointer to ICAThumbnail
                                                                    // NOTE: you have to allocate and prefill the ICAThumbnail
                                                                    //       malloc(sizeof(ICAThumbnail)+9215);
                                                                    //         width & height -> 48
                                                                    //		   dataSize       -> 9216  (= 48*48*4)

ICAError ICDCreateICAThumbnailFromIconRef( const IconRef iconRef, ICAThumbnail* thumbnail );
ICAError ICDInitiateNotificationCallback(const ICAExtendedRegisterEventNotificationPB* pb);


ICAError ICDCreateEventDataCookie(const ICAObject object, ICAEventDataCookie* cookie);

//------------------------------------------------------------------------------------------------------------------------------
// USB
ICAError ICDConnectUSBDevice(UInt32 locationID);
ICAError ICDConnectUSBDeviceWithIORegPath(UInt32 locationID, io_string_t ioregPath);

ICAError ICDDisconnectUSBDevice(UInt32 locationID);
ICAError ICDDisconnectUSBDeviceWithIORegPath(UInt32 locationID, io_string_t ioregPath);

//------------------------------------------------------------------------------------------------------------------------------
// FireWire
ICAError ICDConnectFWDevice(UInt64 guid);
ICAError ICDConnectFWDeviceWithIORegPath(UInt64 guid, io_string_t ioregPath);
ICAError ICDDisconnectFWDevice(UInt64 guid);
ICAError ICDDisconnectFWDeviceWithIORegPath(UInt64 guid, io_string_t ioregPath);

//------------------------------------------------------------------------------------------------------------------------------
// Bluetooth

ICAError ICDConnectBluetoothDevice(CFDictionaryRef params);
ICAError ICDDisconnectBluetoothDevice(CFDictionaryRef params);

//------------------------------------------------------------------------------------------------------------------------------
// TCP/IP

ICAError ICDConnectTCPIPDevice(CFDictionaryRef params);
ICAError ICDDisconnectTCPIPDevice(CFDictionaryRef params);
                          
//------------------------------------------------------------------------------------------------------------------------------
// deprecated - use ICDInitiateNotificationCallback instead
ICAError ICDStatusChanged (ICAObject object, OSType message);

//------------------------------------------------------------------------------------------------------------------------------

// callback functions
typedef struct ICD_callback_functions
{
    __ICD_OpenUSBDevice  					f_ICD_OpenUSBDevice;
    __ICD_CloseDevice 						f_ICD_CloseDevice;
    __ICD_PeriodicTask						f_ICD_PeriodicTask;
    __ICD_GetObjectInfo						f_ICD_GetObjectInfo;
    __ICD_Cleanup							f_ICD_Cleanup;
    __ICD_GetPropertyData					f_ICD_GetPropertyData;
    __ICD_SetPropertyData					f_ICD_SetPropertyData;
    __ICD_ReadFileData						f_ICD_ReadFileData;
    __ICD_WriteFileData						f_ICD_WriteFileData;
    __ICD_SendMessage						f_ICD_SendMessage;
    __ICD_AddPropertiesToCFDictionary	 	f_ICD_AddPropertiesToCFDictionary;
    __ICD_OpenFireWireDevice  				f_ICD_OpenFireWireDevice;
    __ICD_OpenUSBDeviceWithIORegPath		f_ICD_OpenUSBDeviceWithIORegPath;
    __ICD_OpenFireWireDeviceWithIORegPath	f_ICD_OpenFireWireDeviceWithIORegPath;
    __ICD_OpenBluetoothDevice               f_ICD_OpenBluetoothDevice;
    __ICD_OpenTCPIPDevice                   f_ICD_OpenTCPIPDevice;
    __ICD_WriteDataToFile                   f_ICD_WriteDataToFile;
    __ICD_OpenMassStorageDevice             f_ICD_OpenMassStorageDevice;
} ICD_callback_functions;
extern ICD_callback_functions gICDCallbackFunctions;

//------------------------------------------------------------------------------------------------------------------------------

#pragma pack(pop)

#ifdef __cplusplus
}
#endif

//------------------------------------------------------------------------------------------------------------------------------

#endif

//------------------------------------------------------------------------------------------------------------------------------
