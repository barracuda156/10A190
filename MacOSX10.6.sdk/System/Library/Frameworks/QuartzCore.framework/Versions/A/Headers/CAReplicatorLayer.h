/* CoreAnimation - CAReplicatorLayer.h

   Copyright (c) 2008 Apple Inc.
   All rights reserved. */

#import <QuartzCore/CALayer.h>

/* The replicator layer creates a specified number of copies of its
 * sublayers, each copy potentially having geometric, temporal and
 * color transformations applied to it. */

@interface CAReplicatorLayer : CALayer

/* The number of copies to create, including the source object.
 * Default value is one (i.e. no extra copies.) Animatable. */

@property NSInteger instanceCount;

/* Defines whether this layer flattens its sublayers into its plane or
 * not (i.e. whether it's treated similarly to a transform layer or
 * not.) Defaults to NO. If YES, the standard restrictions apply (see
 * CATransformLayer.h.) */

@property BOOL preservesDepth;

/* The temporal delay between replicated copies. Defaults to zero.
 * Animatable. */

@property CFTimeInterval instanceDelay;

/* The matrix applied to instance k-1 to produce instance k. The matrix
 * is applied relative to the center of the replicator layer, i.e. the
 * superlayer of each replicated sublayer. Defaults to the identity
 * matrix. Animatable. */

@property CATransform3D instanceTransform;

/* The color to multiply the first object by (the source object.) Defaults
 * to opaque white. Animatable. */

@property CGColorRef instanceColor;

/* The color components added to the color of instance k-1 to produce
 * the modulation color of instance k. Defaults to the clear color (no
 * change.) Animatable. */

@property float instanceRedOffset, instanceGreenOffset;
@property float instanceBlueOffset, instanceAlphaOffset;

@end
