/*
	File:		QTCaptureOutput.h
 
	Copyright:	(c)2007 by Apple Inc., all rights reserved.

 */

#ifndef QTCAPTUREOUTPUT_H  // TODO: FIX BUILD SYSTEM INSTEAD - plus rdar://problem/5947690
#define QTCAPTUREOUTPUT_H

#import <Foundation/Foundation.h>
#import <QTKit/QTKitDefines.h>

#if (QTKIT_VERSION_MAX_ALLOWED >= QTKIT_VERSION_7_2) && (MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_4)

@class QTCaptureSession;

@interface QTCaptureOutput : NSObject {
@private
	QTCaptureSession * __weak	_session;
	long						_reserved1;
	long						_reserved2;
	long						_reserved3;
}

- (NSArray *)connections;

@end

#endif /* (QTKIT_VERSION_MAX_ALLOWED >= QTKIT_VERSION_7_2) && (MAC_OS_X_VERSION_MAX_ALLOWED >= MAC_OS_X_VERSION_10_4) */

#endif // QTCAPTUREOUTPUT_H
