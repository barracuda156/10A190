/*
	Copyright:	(c) 1999-2008 Apple Inc. All rights reserved.
*/

#ifndef _CGLRENDERERS_H
#define _CGLRENDERERS_H

/*
** Renderer ID numbers
*/
#define kCGLRendererGenericID        0x00020200
#define kCGLRendererGenericFloatID   0x00020400
#define kCGLRendererAppleSWID        0x00020600
#define kCGLRendererATIRage128ID     0x00021000
#define kCGLRendererATIRadeonID      0x00021200
#define kCGLRendererATIRageProID     0x00021400
#define kCGLRendererATIRadeon8500ID  0x00021600
#define kCGLRendererATIRadeon9700ID  0x00021800
#define kCGLRendererATIRadeonX1000ID 0x00021900
#define kCGLRendererATIRadeonX2000ID 0x00021A00
#define kCGLRendererGeForce2MXID     0x00022000 /* also for GeForce 4MX  */
#define kCGLRendererGeForce3ID       0x00022200 /* also for GeForce 4 Ti */
#define kCGLRendererGeForceFXID      0x00022400 /* also for GeForce 6xxx, 7xxx */
#define kCGLRendererGeForce8xxxID    0x00022600
#define kCGLRendererVTBladeXP2ID     0x00023000
#define kCGLRendererIntel900ID       0x00024000
#define kCGLRendererIntelX3100ID     0x00024200
#define kCGLRendererMesa3DFXID       0x00040000

/*
** kCGLRendererIDMatchingMaskgives the bits that are useful for matching a renderer ID (as
** returned by CGLDescribePixelFormat or CGLDescribeRenderer) with the assigned values above.
** There should be no bits set in the assigned renderer ID's that are not set in this mask.
*/
#define kCGLRendererIDMatchingMask   0x00FE7F00

#endif /* _CGLRENDERERS_H */

