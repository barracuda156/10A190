/* CoreGraphics - CGDisplayFade.h
 * Copyright (c) 2002-2008 Apple Inc.
 * All rights reserved. */

#ifndef CGDISPLAYFADE_H_
#define CGDISPLAYFADE_H_

/* These APIs are used to fade displays to and from a solid color without
   resorting to playing with the gamma table APIs and losing ColorSync
   calibration.

   These APIs should be used in perference to manipulating the gamma tables
   for purposes of performing fade effects. */

#include <CoreGraphics/CGDirectDisplay.h>
#include <CoreGraphics/CGDisplayConfiguration.h>

typedef uint32_t CGDisplayFadeReservationToken;
#define kCGDisplayFadeReservationInvalidToken (0)

typedef float CGDisplayBlendFraction;

/* Values for the limits of the fade. `kCGDisplayBlendNormal' represents a
     normal display state. `kCGDisplayBlendSolidColor' represents a display
     blended to a solid color */

#define kCGDisplayBlendNormal		(0.0)
#define kCGDisplayBlendSolidColor	(1.0)

/* Time in seconds to perform a fade operation. */
typedef float CGDisplayFadeInterval;

/* Set the display fade time and color for a display reconfigure operation.
   Call this function after `CGBeginDisplayConfiguration' and before
   `CGCompleteDisplayConfiguration'.

   When `CGCompleteDisplayConfiguration' is called, a fade-out effect will
   occur prior to the display reconfiguration. When the reconfiguration is
   complete, control returns to the calling program; the fade-in effect runs
   asynchronously. */

CG_EXTERN CGError CGConfigureDisplayFadeEffect(CGDisplayConfigRef config,
  CGDisplayFadeInterval fadeOutSeconds, CGDisplayFadeInterval fadeInSeconds,
  float fadeRed, float fadeGreen, float fadeBlue)
  CG_AVAILABLE_STARTING(__MAC_10_2, __IPHONE_NA);

/* It may be desirable to perform fade operations at other times, such as when
   transitioning between game play and cinematic sequences. The following
   API provides a mechanism for controlling display fade operations outside
   of display mode reconfigurations. */

/* Return a reservation token for the display hardware. Before performing a
   fade operation, the caller must reserve the hardware for the expected
   period of time that the program will be doing fades.

   Failing to release the hardware by the end of the reservation interval will
   result in the reservation token becomingn invalid, and the hardware being
   unfaded back to a normal state.  The reservation interval is limited
   to a maximum of 15 seconds and should be greater than zero. */

typedef float CGDisplayReservationInterval;
#define kCGMaxDisplayReservationInterval ((CGDisplayReservationInterval)(15.0))

CG_EXTERN CGError CGAcquireDisplayFadeReservation(CGDisplayReservationInterval
  seconds, CGDisplayFadeReservationToken *token)
  CG_AVAILABLE_STARTING(__MAC_10_2, __IPHONE_NA);

/* Release a display fade reservation, and unfades the display if needed.
   The reservation token is no longer valid after this operation.
 
   `CGReleaseDisplayFadeReservation' may be safely called while an
   asynchronous fade operation is running; if the ending blend value is
   `kCGDisplayBlendNormal', this will not disturb the running operation. The
   reservation is dropped when the fade operation completes. */

CG_EXTERN CGError CGReleaseDisplayFadeReservation(CGDisplayFadeReservationToken
  token) CG_AVAILABLE_STARTING(__MAC_10_2, __IPHONE_NA);

/* Perform a fade operation. The reservation token `token' must have been
   previously acquired from `CGAcquireDisplayFadeReservation'. The duration
   of the fade is specified by `duration'. The starting and ending values of
   the alpha component of the desired blend color are specified by
   `startBlend' and `endBlend', respectively. The RGB blend color is
   specified by the three components `(redBlend, greenBlend, blueBlend)'.
   If the operation should be synchronous, specify true for `synchronous',
   otherwise specify false.

   Over the fade operation time interval, the system interpolates the
   blending coefficient between the specified starting and ending values,
   applying a nonlinear (sine-based) bias term, and blends the video output
   with the specified color based on the resulting value.

   If the time interval is specifed as 0, then the ending state blend value
   is applied at once and the function returns.

   The maximum allowable time interval is 15 seconds.

   If the `synchronous' is true, the function does not return until the fade
   operation is complete. If false, the function returns at once, and the
   fade operation runs asynchronously.
   
   To perform a two-second fade-to-black, waiting until complete:

     CGDisplayFade(token,
		   2,				// 2 seconds 
		   kCGDisplayBlendNormal,	// Starting state 
		   kCGDisplayBlendSolidColor,	// Ending state 
		   0, 0, 0,			// black 
		   true);			// Wait for completion 

   To perform a two-second fade-from-black to normal, without waiting for
   completion:

     CGDisplayFade(token,
		   2,				// 2 seconds 
		   kCGDisplayBlendSolidColor,	// Starting state 
		   kCGDisplayBlendNormal,	// Ending state 
		   0, 0, 0,			// black 
		   false);			// Don't wait for completion */

CG_EXTERN CGError CGDisplayFade(CGDisplayFadeReservationToken token,
  CGDisplayFadeInterval duration, CGDisplayBlendFraction startBlend,
  CGDisplayBlendFraction endBlend, float redBlend, float greenBlend,
  float blueBlend, boolean_t synchronous)
  CG_AVAILABLE_STARTING(__MAC_10_2, __IPHONE_NA);

/* Return true if a fade operation is currently in progress, false
   otherwise. */

CG_EXTERN boolean_t CGDisplayFadeOperationInProgress(void)
  CG_AVAILABLE_STARTING(__MAC_10_2, __IPHONE_NA);

#define __CGDISPLAY_FADE_H__ 1

#endif /* CGDISPLAYFADE_H_ */
