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
#ifndef _IOKIT_ROOTDOMAIN_H
#define _IOKIT_ROOTDOMAIN_H

#include <IOKit/IOService.h>
#include <IOKit/pwr_mgt/IOPM.h>


class IOPMPowerStateQueue;
class RootDomainUserClient;

/*
 * Flags for get/setSleepSupported()
 */
enum {
    kRootDomainSleepNotSupported	= 0x00000000,
    kRootDomainSleepSupported 		= 0x00000001,
    kFrameBufferDeepSleepSupported	= 0x00000002,
    kPCICantSleep                   = 0x00000004
};

/* 
 *IOPMrootDomain registry property keys
 */
#define kRootDomainSupportedFeatures        "Supported Features"
#define kRootDomainSleepReasonKey           "Last Sleep Reason"
#define kRootDomainSleepOptionsKey          "Last Sleep Options"
#define kIOPMRootDomainWakeReasonKey        "Wake Reason"
#define kIOPMRootDomainWakeTypeKey          "Wake Type"
#define kIOPMRootDomainPowerStatusKey       "Power Status"

/*
 * Possible sleep reasons found under kRootDomainSleepReasonsKey
 */
#define kIOPMClamshellSleepKey              "Clamshell Sleep"
#define kIOPMPowerButtonSleepKey            "Power Button Sleep"
#define kIOPMSoftwareSleepKey               "Software Sleep"
#define kIOPMOSSwitchHibernationKey         "OS Switch Sleep"
#define kIOPMIdleSleepKey                   "Idle Sleep"
#define kIOPMLowPowerSleepKey               "Low Power Sleep"
#define kIOPMThermalEmergencySleepKey       "Thermal Emergency Sleep"
#define kIOPMMaintenanceSleepKey            "Maintenance Sleep"

/*
 * String constants for communication with PM CPU
 */
#define kIOPMRootDomainLidCloseCString      "LidClose"
#define kIOPMRootDomainBatPowerCString      "BatPower"

/*
 * Supported Feature bitfields for IOPMrootDomain::publishFeature()
 */
enum {
    kIOPMSupportedOnAC      = (1<<0),
    kIOPMSupportedOnBatt    = (1<<1),
    kIOPMSupportedOnUPS     = (1<<2)
};

typedef IOReturn (*IOPMSettingControllerCallback)
                    (OSObject *target, const OSSymbol *type,
                     OSObject *val, uintptr_t refcon);

__BEGIN_DECLS
IONotifier *    registerSleepWakeInterest(
                    IOServiceInterestHandler, void *, void * = 0);
               
IONotifier *    registerPrioritySleepWakeInterest(
                    IOServiceInterestHandler handler, 
                    void * self, void * ref = 0);

IOReturn        acknowledgeSleepWakeNotification(void * );

IOReturn        vetoSleepWakeNotification(void * PMrefcon);
__END_DECLS

#define IOPM_ROOTDOMAIN_REV		2

class IOPMrootDomain: public IOService
{
    OSDeclareDefaultStructors(IOPMrootDomain)

public:
    static IOPMrootDomain * construct( void );

    virtual bool        start( IOService * provider );
    virtual IOReturn    setAggressiveness( unsigned long, unsigned long );

    virtual IOReturn    sleepSystem( void );
    IOReturn            sleepSystemOptions( OSDictionary *options );

    virtual IOReturn    setProperties( OSObject * );

/*! @function systemPowerEventOccurred
    @abstract Other drivers may inform IOPMrootDomain of system PM events
    @discussion systemPowerEventOccurred is a richer alternative to receivePowerNotification()
        Only Apple-owned kexts should have reason to call systemPowerEventOccurred.
    @param event An OSSymbol describing the type of power event.
    @param value A 32-bit integer value associated with the event.
    @param shouldUpdate indicates whether the root domain should send a notification
        to interested parties. Pass false if you're calling systemPowerEventOccurred
        several times in succession; and pass true only on the last invocatino.
    @result kIOReturnSuccess on success */

    IOReturn            systemPowerEventOccurred(
                                    const OSSymbol *event, 
                                    uint32_t intValue );

    IOReturn            systemPowerEventOccurred(
                                    const OSSymbol *event, 
                                    OSObject *value );

    virtual IOReturn    receivePowerNotification( UInt32 msg );

    virtual void        setSleepSupported( IOOptionBits flags );

    virtual IOOptionBits getSleepSupported( void );

    void                wakeFromDoze( void );

    // KEXT driver announces support of power management feature

    void                publishFeature( const char *feature );
    
    // KEXT driver announces support of power management feature
    // And specifies power sources with kIOPMSupportedOn{AC/Batt/UPS} bitfield.
    // Returns a unique uint32_t identifier for later removing support for this
    // feature. 
    // NULL is acceptable for uniqueFeatureID for kexts without plans to unload.

    void                publishFeature( const char *feature, 
                                        uint32_t supportedWhere,
                                        uint32_t *uniqueFeatureID);

    // KEXT driver announces removal of a previously published power management 
    // feature. Pass 'uniqueFeatureID' returned from publishFeature()

    IOReturn            removePublishedFeature( uint32_t removeFeatureID );

/*! @function copyPMSetting
    @abstract Copy the current value for a PM setting. Returns an OSNumber or
        OSData depending on the setting.
    @param whichSetting Name of the desired setting.
    @result OSObject value if valid, NULL otherwise. */

    OSObject *          copyPMSetting( OSSymbol *whichSetting );

/*! @function registerPMSettingController
    @abstract Register for callbacks on changes to certain PM settings.
    @param settings NULL terminated array of C strings, each string for a PM 
        setting that the caller is interested in and wants to get callbacks for. 
    @param callout C function ptr or member function cast as such.
    @param target The target of the callback, usually 'this'
    @param refcon Will be passed to caller in callback; for caller's use.
    @param handle Caller should keep the OSObject * returned here. If non-NULL,
        handle will have a retain count of 1 on return. To deregister, pass to
        unregisterPMSettingController()
    @result kIOReturnSuccess on success. */

    IOReturn            registerPMSettingController(
                                 const OSSymbol *settings[],
                                 IOPMSettingControllerCallback callout,
                                 OSObject   *target,
                                 uintptr_t  refcon,
                                 OSObject   **handle);    // out param

/*! @function registerPMSettingController
    @abstract Register for callbacks on changes to certain PM settings.
    @param settings NULL terminated array of C strings, each string for a PM 
        setting that the caller is interested in and wants to get callbacks for. 
    @param supportedPowerSources bitfield indicating which power sources these
        settings are supported for (kIOPMSupportedOnAC, etc.)
    @param callout C function ptr or member function cast as such.
    @param target The target of the callback, usually 'this'
    @param refcon Will be passed to caller in callback; for caller's use.
    @param handle Caller should keep the OSObject * returned here. If non-NULL,
        handle will have a retain count of 1 on return. To deregister, pass to
        unregisterPMSettingController()
    @result kIOReturnSuccess on success. */

    IOReturn            registerPMSettingController(
                                 const OSSymbol *settings[],
                                 uint32_t   supportedPowerSources,
                                 IOPMSettingControllerCallback callout,
                                 OSObject   *target,
                                 uintptr_t  refcon,
                                 OSObject   **handle);    // out param

    virtual IONotifier * registerInterest(
                                const OSSymbol * typeOfInterest,
                                IOServiceInterestHandler handler,
                                void * target, void * ref = 0 );

private:
    virtual IOReturn    changePowerStateTo( unsigned long ordinal );
    virtual IOReturn    changePowerStateToPriv( unsigned long ordinal );
    virtual IOReturn    requestPowerDomainState( IOPMPowerFlags, IOPowerConnection *, unsigned long );
    virtual void        powerChangeDone( unsigned long );
    virtual bool        tellChangeDown( unsigned long );
    virtual bool        askChangeDown( unsigned long );
    virtual void        tellChangeUp( unsigned long );
    virtual void        tellNoChangeDown( unsigned long );
};


#endif /* _IOKIT_ROOTDOMAIN_H */
