/*
 * Copyright (c) 1998-2000 Apple Computer, Inc. All rights reserved.
 *
 * @APPLE_OSREFERENCE_LICENSE_HEADER_START@
 * 
 * This file contains Original Code and/or Modifications of Original Code
 * as defined in and that are subject to the Apple Public Source License
 * Version 2.0 (the 'License'). You may not use this file except in
 * compliance with the License. The rights granted to you under the License
 * may not be used to create, or enable the creation or redistribution of,
 * unlawful or unlicensed copies of an Apple operating system, or to
 * circumvent, violate, or enable the circumvention or violation of, any
 * terms of an Apple operating system software license agreement.
 * 
 * Please obtain a copy of the License at
 * http://www.opensource.apple.com/apsl/ and read it before using this file.
 * 
 * The Original Code and all software distributed under the License are
 * distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT.
 * Please see the License for the specific language governing rights and
 * limitations under the License.
 * 
 * @APPLE_OSREFERENCE_LICENSE_HEADER_END@
 */

/*
 * Changes to this API are expected.
 */

#ifndef _IOKIT_IOUSERCLIENT_H
#define _IOKIT_IOUSERCLIENT_H

#include <IOKit/IOTypes.h>
#include <IOKit/IOService.h>
#include <IOKit/OSMessageNotification.h>


enum {
    kIOUCTypeMask	= 0x0000000f,
    kIOUCScalarIScalarO = 0,
    kIOUCScalarIStructO = 2,
    kIOUCStructIStructO = 3,
    kIOUCScalarIStructI = 4
};

/*! @enum
    @abstract Constant to denote a variable length structure argument to IOUserClient.
    @constant kIOUCVariableStructureSize Use in the structures IOExternalMethod, IOExternalAsyncMethod, IOExternalMethodDispatch to specify the size of the structure is variable.
*/
enum {
    kIOUCVariableStructureSize = 0xffffffff
};


typedef IOReturn (IOService::*IOMethod)(void * p1, void * p2, void * p3,
                                        void * p4, void * p5, void * p6 );

typedef IOReturn (IOService::*IOAsyncMethod)(OSAsyncReference asyncRef,
                                        void * p1, void * p2, void * p3,
                                        void * p4, void * p5, void * p6 );

typedef IOReturn (IOService::*IOTrap)(void * p1, void * p2, void * p3,
                                      void * p4, void * p5, void * p6 );

struct IOExternalMethod {
    IOService *		object;
    IOMethod		func;
    IOOptionBits	flags;
    IOByteCount		count0;
    IOByteCount		count1;
};

struct IOExternalAsyncMethod {
    IOService *		object;
    IOAsyncMethod	func;
    IOOptionBits	flags;
    IOByteCount		count0;
    IOByteCount		count1;
};

struct IOExternalTrap {
    IOService *		object;
    IOTrap		func;
};

enum {
    kIOUserNotifyMaxMessageSize = 64
};

// keys for clientHasPrivilege
#define kIOClientPrivilegeAdministrator	"root"
#define kIOClientPrivilegeLocalUser	"local"

/*! @enum
    @abstract Constants to specify the maximum number of scalar arguments in the IOExternalMethodArguments structure. These constants are documentary since the scalarInputCount, scalarOutputCount fields reflect the actual number passed.
    @constant kIOExternalMethodScalarInputCountMax The maximum number of scalars able to passed on input.
    @constant kIOExternalMethodScalarOutputCountMax The maximum number of scalars able to passed on output.
*/
enum {
    kIOExternalMethodScalarInputCountMax  = 16,
    kIOExternalMethodScalarOutputCountMax = 16,
};


struct IOExternalMethodArguments
{
    uint32_t		version;

    uint32_t		selector;

    mach_port_t           asyncWakePort;
    io_user_reference_t * asyncReference;
    uint32_t              asyncReferenceCount;

    const uint64_t *    scalarInput;
    uint32_t		scalarInputCount;

    const void *	structureInput;
    uint32_t		structureInputSize;

    IOMemoryDescriptor * structureInputDescriptor;
   
    uint64_t *		scalarOutput;
    uint32_t		scalarOutputCount;

    void *		structureOutput;
    uint32_t		structureOutputSize;

    IOMemoryDescriptor * structureOutputDescriptor;
    uint32_t		 structureOutputDescriptorSize;

    uint32_t		__reserved[32];
};

typedef IOReturn (*IOExternalMethodAction)(OSObject * target, void * reference, 
					    IOExternalMethodArguments * arguments);
struct IOExternalMethodDispatch
{
    IOExternalMethodAction function;
    uint32_t		   checkScalarInputCount;
    uint32_t		   checkStructureInputSize;
    uint32_t		   checkScalarOutputCount;
    uint32_t		   checkStructureOutputSize;
};

enum {
#define IO_EXTERNAL_METHOD_ARGUMENTS_CURRENT_VERSION	1
    kIOExternalMethodArgumentsCurrentVersion = IO_EXTERNAL_METHOD_ARGUMENTS_CURRENT_VERSION
};


/*!
    @class IOUserClient
    @abstract   Provides a basis for communication between client applications and I/O Kit objects.
*/


class IOUserClient : public IOService
{
    OSDeclareAbstractStructors(IOUserClient)

protected:
/*! @struct ExpansionData
    @discussion This structure will be used to expand the capablilties of this class in the future.
*/    
    struct ExpansionData { };

/*! @var reserved
    Reserved for future use.  (Internal use only) 
*/
    ExpansionData * reserved;

private:
    OSSet * mappings;
    UInt8   sharedInstance;
    UInt8   __reservedA[3];
    void  * __reserved[7];

public:
   virtual IOReturn externalMethod( uint32_t selector, IOExternalMethodArguments * arguments,
					IOExternalMethodDispatch * dispatch = 0, OSObject * target = 0, void * reference = 0 );

   virtual IOReturn registerNotificationPort(
					mach_port_t port, UInt32 type, io_user_reference_t refCon);

private:
#if __LP64__
    OSMetaClassDeclareReservedUnused(IOUserClient, 0);
    OSMetaClassDeclareReservedUnused(IOUserClient, 1);
#else
    OSMetaClassDeclareReservedUsed(IOUserClient, 0);
    OSMetaClassDeclareReservedUsed(IOUserClient, 1);
#endif
    OSMetaClassDeclareReservedUnused(IOUserClient, 2);
    OSMetaClassDeclareReservedUnused(IOUserClient, 3);
    OSMetaClassDeclareReservedUnused(IOUserClient, 4);
    OSMetaClassDeclareReservedUnused(IOUserClient, 5);
    OSMetaClassDeclareReservedUnused(IOUserClient, 6);
    OSMetaClassDeclareReservedUnused(IOUserClient, 7);
    OSMetaClassDeclareReservedUnused(IOUserClient, 8);
    OSMetaClassDeclareReservedUnused(IOUserClient, 9);
    OSMetaClassDeclareReservedUnused(IOUserClient, 10);
    OSMetaClassDeclareReservedUnused(IOUserClient, 11);
    OSMetaClassDeclareReservedUnused(IOUserClient, 12);
    OSMetaClassDeclareReservedUnused(IOUserClient, 13);
    OSMetaClassDeclareReservedUnused(IOUserClient, 14);
    OSMetaClassDeclareReservedUnused(IOUserClient, 15);


protected:
    static IOReturn sendAsyncResult(OSAsyncReference reference,
                                    IOReturn result, void *args[], UInt32 numArgs);
    static void setAsyncReference(OSAsyncReference asyncRef,
                                  mach_port_t wakePort,
                                  void *callback, void *refcon);

    static IOReturn sendAsyncResult64(OSAsyncReference64 reference,
                                        IOReturn result, io_user_reference_t args[], UInt32 numArgs);
    static void setAsyncReference64(OSAsyncReference64 asyncRef,
					mach_port_t wakePort,
					mach_vm_address_t callback, io_user_reference_t refcon);
public:

    static IOReturn clientHasPrivilege( void * securityToken,
                                        const char * privilegeName );

    virtual bool init();
    virtual bool init( OSDictionary * dictionary );
    // Currently ignores the all args, just passes up to IOService::init()
    virtual bool initWithTask(
                    task_t owningTask, void * securityToken, UInt32 type,
                    OSDictionary * properties);

    virtual bool initWithTask(
                    task_t owningTask, void * securityToken, UInt32 type);

    virtual void free();

    virtual IOReturn clientClose( void );
    virtual IOReturn clientDied( void );

    virtual IOService * getService( void );

    virtual IOReturn registerNotificationPort(
		mach_port_t port, UInt32 type, UInt32 refCon );

    virtual IOReturn getNotificationSemaphore( UInt32 notification_type,
                                    semaphore_t * semaphore );

    virtual IOReturn connectClient( IOUserClient * client );

    // memory will be released by user client when last map is destroyed
    virtual IOReturn clientMemoryForType( UInt32 type,
			        IOOptionBits * options,
				IOMemoryDescriptor ** memory );

#if !__LP64__
private:
	APPLE_KEXT_COMPATIBILITY_VIRTUAL
	IOMemoryMap * mapClientMemory( IOOptionBits type,
                                    task_t task,
                                    IOOptionBits mapFlags = kIOMapAnywhere,
									IOVirtualAddress atAddress = 0 );
#endif

public:

    /*!
        @function removeMappingForDescriptor
        Remove the first mapping created from the memory descriptor returned by clientMemoryForType() from IOUserClient's list of mappings. If such a mapping exists, it is retained and the reference currently held by IOUserClient is returned to the caller.
        @param memory The memory descriptor instance previously returned by the implementation of clientMemoryForType().
        @result A reference to the first IOMemoryMap instance found in the list of mappings created by IOUserClient from that passed memory descriptor is returned, or zero if none exist. The caller should release this reference.
    */
    IOMemoryMap * removeMappingForDescriptor(IOMemoryDescriptor * memory);

    /*!
        @function exportObjectToClient
        Make an arbitrary OSObject available to the client task.
        @param task The task.
        @param obj The object we want to export to the client.
        @param clientObj Returned value is the client's port name.
    */
    virtual IOReturn exportObjectToClient(task_t task,
				OSObject *obj, io_object_t *clientObj);

    // Old methods for accessing method vector backward compatiblility only
    virtual IOExternalMethod *
        getExternalMethodForIndex( UInt32 index )
	APPLE_KEXT_DEPRECATED;
    virtual IOExternalAsyncMethod *
        getExternalAsyncMethodForIndex( UInt32 index )
	APPLE_KEXT_DEPRECATED;

    // Methods for accessing method vector.
    virtual IOExternalMethod *
        getTargetAndMethodForIndex( IOService ** targetP, UInt32 index );
    virtual IOExternalAsyncMethod *
        getAsyncTargetAndMethodForIndex( IOService ** targetP, UInt32 index );

    // Methods for accessing trap vector - old and new style
    virtual IOExternalTrap *
		getExternalTrapForIndex( UInt32 index )
	APPLE_KEXT_DEPRECATED;

    virtual IOExternalTrap *
      getTargetAndTrapForIndex( IOService **targetP, UInt32 index );
};

#endif /* ! _IOKIT_IOUSERCLIENT_H */

