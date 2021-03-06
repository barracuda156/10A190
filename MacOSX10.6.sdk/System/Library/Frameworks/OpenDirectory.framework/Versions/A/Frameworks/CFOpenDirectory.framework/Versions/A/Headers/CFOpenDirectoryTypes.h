/*
 * Copyright (c) 2005 Apple Computer, Inc. All rights reserved.
 *
 * @APPLE_LICENSE_HEADER_START@
 * 
 * This file contains Original Code and/or Modifications of Original Code
 * as defined in and that are subject to the Apple Public Source License
 * Version 2.0 (the 'License'). You may not use this file except in
 * compliance with the License. Please obtain a copy of the License at
 * http://www.opensource.apple.com/apsl/ and read it before using this
 * file.
 * 
 * The Original Code and all software distributed under the License are
 * distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT.
 * Please see the License for the specific language governing rights and
 * limitations under the License.
 * 
 * @APPLE_LICENSE_HEADER_END@
 */

/*!
    @header		CFOpenDirectoryTypes
    @abstract   Type definition for OpenDirectory framework types
    @discussion This header includes definitions for types used by OpenDirectory
				framework.
*/

// If NSOpenDirectory.h has already been included, do not redefine the types
#if !defined( __CFOPENDIRECTORYTYPES_H ) && !defined( __NSOPENDIRECTORY_H )
#define __CFOPENDIRECTORYTYPES_H

#include <CoreFoundation/CFString.h>

/*!
	@typedef	_ODRecordType
	@abstract   is used to abstract the type
	@discussion is used to abstract the type.  CFStringRef can be used interchangeably
				with ODRecordType.
*/
typedef CFStringRef _ODRecordType;

/*!
	@typedef	_ODAttributeType
	@abstract   is used to abstract the type
	@discussion is used to abstract the type.  CFStringRef can be used interchangeably
				with ODAttributeType
*/
typedef CFStringRef _ODAttributeType;

/*!
	@typedef	_ODAuthenticationType
	@abstract   is used to abstract the type
	@discussion is used to abstract the type.  CFStringRef can be used interchangeably
				with ODAttributeType
*/
typedef CFStringRef _ODAuthenticationType;

#endif
