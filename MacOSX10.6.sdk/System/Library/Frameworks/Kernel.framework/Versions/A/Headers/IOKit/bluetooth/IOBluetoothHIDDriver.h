/*
	File:		IOBluetoothHIDDriver.h
	Contains:	Driver for generic Bluetooth HID devices.
	Copyright:	(c) 2001-2008 by Apple, all rights reserved.
*/

#ifndef IOBLUETOOTHHIDDRIVER_H
#define IOBLUETOOTHHIDDRIVER_H

#include <IOKit/hid/IOHIDDevice.h>
#include <IOKit/bluetooth/Bluetooth.h>

//===========================================================================================================================
// Forwards
//===========================================================================================================================

class IOTimerEventSource;
class IOWorkLoop;

class IOBluetoothL2CAPChannel;
class IOBluetoothDevice;
class IOWorkQueue;

//===========================================================================================================================
// IOBluetoothHIDDriver
//===========================================================================================================================

class IOBluetoothHIDDriver : public IOHIDDevice
{
	OSDeclareDefaultStructors( IOBluetoothHIDDriver )

    IOWorkLoop*					_workLoop;
	IOCommandGate*				_commandGate;
	
	IOWorkQueue					*_desyncWorkQueue;
	
	IOBluetoothL2CAPChannel*	_controlChannel;
    IOBluetoothL2CAPChannel*	_interruptChannel;

    IOBluetoothDevice*			_device;

	IOMemoryDescriptor*			_memDescriptor;
	IOMemoryDescriptor*			_getReportDescriptor;
	IONotifier*					_interruptOpenNotification;
	IOTimerEventSource*			_timer;
    IONotifier*					_sleepWakeNotifier;

	bool						_deviceReady;

	UInt8						_expectedReportID;
	UInt8						_expectedReportType;
	UInt8						_handshake;
	
	OSDictionary*				_deviceProperties;

	UInt16						_vendorIDSource;
	UInt16						_vendorID;
	UInt16						_productID;
	UInt16						_deviceVersion;
	uint32_t					_classOfDevice;
    UInt16                      _countryCode;
	
	BluetoothDeviceAddress		_deviceAddress;
	char						_deviceAddressString[20];
	
	uint32_t					_outstandingIO;
	bool						_sendOutstanding;
	
	// Debug / Behavior Modifiers
	UInt8						_verboseLevel;
	bool						_logPackets;
	bool						_decodePackets;
	bool						_logOutstandingIO;
	bool						_suppressDisconnectNotifications;
	bool						_suppressSetProtocol;
	bool						_driverIsAwake;
	bool						_reservedFlag4;
	UInt8						_reservedByte;
	
    struct ExpansionData 
    { 
        OSArray*    _sendQueue;
		
		UInt8 *interruptBuffer;
		UInt32 interruptBufferUsed;
		
		UInt8 *controlBuffer;
		UInt32 controlBufferUsed;
    };
    ExpansionData	*_expansionData;
	
public:
	// Standard IOService Methods
    virtual IOService*  probe( IOService * provider, SInt32 * score );
	virtual bool		init( OSDictionary *properties );
	virtual void		free();
	virtual bool 		willTerminate( IOService * provider, IOOptionBits options );
	
	// Starting & Stopping
    virtual bool		handleStart( IOService * provider );
    virtual void		handleStop( IOService *  provider );
	virtual void		deviceReady();
	virtual void		closeDownServices();

	// Power Management
    virtual void		handleSleep();
    virtual void		handleWake();
    virtual void		handleShutdown();			// Does nothing
    virtual void		handleRestart();			// Does nothing
	
	// HID Properties
	virtual OSString*	newTransportString() const;
	virtual OSString*	newManufacturerString() const;
	virtual OSString*	newProductString() const;
	virtual OSNumber*	newVendorIDSourceNumber() const;
	virtual OSNumber*	newVendorIDNumber() const;
	virtual OSNumber*	newProductIDNumber() const;
	virtual OSNumber*	newVersionNumber() const;
    virtual IOReturn	newReportDescriptor( IOMemoryDescriptor ** descriptor ) const;
	virtual OSString*	newSerialNumberString() const;
	virtual OSNumber*	newLocationIDNumber() const;
    virtual OSNumber*	newCountryCodeNumber() const;

	// Main UserLand Entry Points
    virtual IOReturn	getReport( IOMemoryDescriptor* report, IOHIDReportType reportType, IOOptionBits options = 0 );
    virtual IOReturn 	setReport( IOMemoryDescriptor* report, IOHIDReportType reportType, IOOptionBits options = 0 );
    virtual IOReturn	setProperties( OSObject* properties );

	// General IO
	virtual IOReturn	sendData( IOBluetoothL2CAPChannel* theChannel, void* theData, IOByteCount theSize );
    virtual void		processControlData( UInt8 *buffer, UInt16 length );
    virtual void		processInterruptData( UInt8 *buffer, UInt16 length );
	virtual IOReturn	waitForData( IOMemoryDescriptor* report, UInt8 btReportType, UInt8 reportID );
	virtual IOReturn	waitForHandshake();
	
	// HID Transaction Methods
	virtual IOReturn	hidControl( UInt8 controlOperation );
	virtual int			getProtocol();
	virtual IOReturn	setProtocol( UInt8 protocol );
	virtual int			getIdle();
	virtual IOReturn	setIdle( UInt8 idleRate );

	// Device Introspection
	virtual bool		isKeyboard();
	virtual bool		isMouse();
	
	// Misc
	virtual IOReturn	setPowerState( unsigned long powerStateOrdinal, IOService* whatDevice );
	virtual IOReturn	createCommandGate( IOService* provider );
	virtual IOReturn	getDeviceProperties( IOService* provider );
	virtual	bool		readDeviceName();
	
	// Command Gate Actions
	static	IOReturn	staticCloseDownServicesAction( OSObject* owner, void* arg1, void* arg2, void* arg3, void* arg4 );
	static	IOReturn	staticSendToAction( OSObject* owner, void* theChannel, void* theData, void *theSize, void* );
	static	IOReturn	staticPrepControlChannelAction( OSObject* owner, void* arg1, void* arg2, void* arg3, void* arg4 );
	static	IOReturn	staticPrepInterruptChannelAction( OSObject* owner, void* arg1, void* arg2, void* arg3, void* arg4 );
	static	IOReturn	staticGetReportAction( OSObject* owner, void* arg1, void* arg2, void* arg3, void* arg4 );
	static	IOReturn	staticSetReportAction( OSObject* owner, void* arg1, void* arg2, void* arg3, void* arg4 );
	static	IOReturn	staticProcessCommandAction( OSObject* owner, void* arg1, void* arg2, void* arg3, void* arg4 );
	static	IOReturn	staticGetDevicePropertiesAction( OSObject* owner, void* arg1, void* arg2, void* arg3, void* arg4 );
	static	IOReturn	staticInterruptChannelOpeningAction( OSObject* owner, void* newService, void* arg2, void* arg3, void* arg4 );
	
	// Work Loop Methods
	virtual void		closeDownServicesWL();
	virtual IOReturn	prepInterruptChannelWL();
	virtual IOReturn	getReportWL( IOMemoryDescriptor* report, IOHIDReportType reportType, IOOptionBits options );
	virtual IOReturn	setReportWL( IOMemoryDescriptor* report, IOHIDReportType reportType, IOOptionBits options );
	virtual IOReturn	processCommandWL( OSString* command, OSNumber* commandParameter );
	virtual IOReturn	getDevicePropertiesWL( IOService* provider );
	virtual IOReturn	interruptChannelOpeningWL( IOBluetoothL2CAPChannel* theChannel );

	// Timeout Handler
	static	void		timerFired( OSObject* owner, IOTimerEventSource* sender );
	virtual void		handleTimeout();
	
	// IO Counting
	virtual void		incrementOutstandingIO();
	virtual void		decrementOutstandingIO();
	
private:
	// Lazy Interrupt Channel Methods
	static bool			interruptChannelOpeningCallback(	void* me, void* ignoreMe, IOService* newService, IONotifier *notifier );
    static	IOReturn 	powerStateHandler( void *target, void *refCon, UInt32 messageType, IOService *service, void *messageArgument, vm_size_t argSize );
																				

public:
    OSMetaClassDeclareReservedUsed( IOBluetoothHIDDriver,  0 );
    virtual void        sendDeviceDisconnectNotifications( void );

	OSMetaClassDeclareReservedUsed( IOBluetoothHIDDriver,  1 );
	virtual IOReturn	setPowerStateWL( unsigned long powerStateOrdinal, IOService* whatDevice ); 

public:    
    OSMetaClassDeclareReservedUnused( IOBluetoothHIDDriver,  2 );
    OSMetaClassDeclareReservedUnused( IOBluetoothHIDDriver,  3 );
    OSMetaClassDeclareReservedUnused( IOBluetoothHIDDriver,  4 );
    OSMetaClassDeclareReservedUnused( IOBluetoothHIDDriver,  5 );
    OSMetaClassDeclareReservedUnused( IOBluetoothHIDDriver,  6 );
    OSMetaClassDeclareReservedUnused( IOBluetoothHIDDriver,  7 );
    OSMetaClassDeclareReservedUnused( IOBluetoothHIDDriver,  8 );
    OSMetaClassDeclareReservedUnused( IOBluetoothHIDDriver,  9 );
    OSMetaClassDeclareReservedUnused( IOBluetoothHIDDriver, 10 );
    OSMetaClassDeclareReservedUnused( IOBluetoothHIDDriver, 11 );
    OSMetaClassDeclareReservedUnused( IOBluetoothHIDDriver, 12 );
    OSMetaClassDeclareReservedUnused( IOBluetoothHIDDriver, 13 );
    OSMetaClassDeclareReservedUnused( IOBluetoothHIDDriver, 14 );
    OSMetaClassDeclareReservedUnused( IOBluetoothHIDDriver, 15 );
    OSMetaClassDeclareReservedUnused( IOBluetoothHIDDriver, 16 );
    OSMetaClassDeclareReservedUnused( IOBluetoothHIDDriver, 17 );
    OSMetaClassDeclareReservedUnused( IOBluetoothHIDDriver, 18 );
    OSMetaClassDeclareReservedUnused( IOBluetoothHIDDriver, 19 );
};

#endif	// IOBLUETOOTHHIDDRIVER_H
