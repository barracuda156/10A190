/* CoreAnimation - CAEmitterCell.h

   Copyright (c) 2007-2008 Apple Inc.
   All rights reserved. */

#import <QuartzCore/CALayer.h>

@interface CAEmitterCell : NSObject <NSCoding, CAMediaTiming>
{
@private
  struct _CAAttrList *_attr[2];
  void *_state;
  uint32_t _flags;
}

+ (id)emitterCell;

/* Emitter cells implement the same property model as defined by CALayer.
 * See CALayer.h for more details. */

+ (id)defaultValueForKey:(NSString *)key;
- (BOOL)shouldArchiveValueForKey:(NSString *)key;

/* The name of the cell. Used to construct key paths. Defaults to nil. */

@property(copy) NSString *name;

/* Controls whether or not cells from this emitter are rendered. */

@property(getter=isEnabled) BOOL enabled;

/* The number of emitted objects created every second. Default value is
 * zero. Animatable. */

@property float birthRate;

/* The lifetime of each emitted object, specified as mean value and
 * a range about the mean. Both values default to zero. Animatable. */

@property float lifetime, lifetimeRange;

/* The orientation of the emission angle, relative to the natural
 * orientation angle of the emission shape. Note that latitude here is
 * what is typically called colatitude, the zenith or phi, the angle
 * from the z-axis. Similarly longitude is the angle in the xy-plane
 * from the x-axis, often referred to as the azimuth or theta. Both
 * values default to zero, which translates to no change relative to
 * the emission shape's direction. Both values are animatable. */

@property CGFloat emissionLatitude, emissionLongitude;

/* An angle defining a cone around the emission angle. Emitted objects
 * are uniformly distributed across this cone. Defaults to zero.
 * Animatable. */

@property CGFloat emissionRange;

/* The initial mean velocity of each emitted object, and its range. Both
 * values default to zero. Animatable. */

@property CGFloat velocity, velocityRange;

/* The acceleration vector applied to emitted objects. Defaults to
 * (0, 0, 0). Animatable. */

@property CGFloat xAcceleration, yAcceleration, zAcceleration;

/* The scale factor applied to each emitted object, defined as mean and
 * range about the mean. Scale defaults to one, range to zero.
 * Animatable. */

@property CGFloat scale, scaleRange, scaleSpeed;

/* The rotation speed applied to each emitted object, defined as mean
 * and range about the mean. Defaults to zero. Animatable. */

@property CGFloat spin, spinRange;

/* The mean color of each emitted object, and the range from that mean
 * color. `color' defaults to opaque white, `colorRange' to (0, 0, 0,
 * 0). Animatable. */

@property CGColorRef color;

@property float redRange, greenRange, blueRange, alphaRange;

/* The speed at which color components of emitted objects change over
 * their lifetime, defined as the rate of change per second. Defaults
 * to (0, 0, 0, 0). Animatable. */

@property float redSpeed, greenSpeed, blueSpeed, alphaSpeed;

/* The cell contents, typically a CGImageRef. Defaults to nil.
 * Animatable. */

@property(retain) id contents;

/* The sub-rectangle of the contents image that will be drawn. See
 * CALayer.h for more details. Defaults to the unit rectangle [0 0 1 1].
 * Animatable. */

@property CGRect contentsRect;

/* The filter parameters used when rendering the `contents' image. See
 * CALayer.h for more details. FIXME: minificationFilterBias is private
 * in CALayer. */

@property(copy) NSString *minificationFilter, *magnificationFilter;
@property float minificationFilterBias;

/* An array containing the sub-cells of this cell, or nil (the default
 * value.) When non-nil each particle emitted by the cell will act as
 * an emitter for each of the cell's sub-cells. The emission point is
 * the current particle position and the emission angle is relative to
 * the current direction of the particle. Animatable. */

@property(copy) NSArray *emitterCells;

/* Inherited attributes similar to in layers. */

@property(copy) NSDictionary *style;

@end
