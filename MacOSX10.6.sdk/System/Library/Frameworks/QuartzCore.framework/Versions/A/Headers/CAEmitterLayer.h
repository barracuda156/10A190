/* CoreAnimation - CAEmitterLayer.h

   Copyright (c) 2007-2008 Apple Inc.
   All rights reserved. */

/* Particle emitter layer.
 * 
 * Currently this uses the `contents' image as the particle cell and
 * replicates it into each object. If the layer has sublayers the
 * behavior is undefined (we may use sublayers as extra particle cells
 * in a future release.)
 *
 * The particles are drawn above the backgroundColor and border of the
 * layer. */

#import <QuartzCore/CALayer.h>

@interface CAEmitterLayer : CALayer

/* The array of emitter cells attached to the layer. Each object must
 * have the CAEmitterCell class. */

@property(copy) NSArray *emitterCells;

/* The birth rate of each cell is multiplied by this number to give the
 * actual number of particles created every second. Default value is one.
 * Animatable. */

@property float birthRate;

/* The cell lifetime range is multiplied by this value when particles are
 * created. Defaults to one. Animatable. */

@property float lifetime;

/* The center of the emission shape. Defaults to (0, 0, 0). Animatable. */

@property CGPoint emitterPosition;
@property CGFloat emitterZPosition;

/* The size of the emission shape. Defaults to (0, 0, 0). Animatable.
 * Depending on the `emitterShape' property some of the values may be
 * ignored. */

@property CGSize emitterSize;
@property CGFloat emitterDepth;

/* A string defining the type of emission shape used. Current options are:
 * `point' (the default), `line', `rectangle', `circle', `cuboid' and
 * `sphere'. */

@property(copy) NSString *emitterShape;

/* A string defining how particles are created relative to the emission
 * shape. Current options are `points', `outline', `surface' and
 * `volume' (the default). */

@property(copy) NSString *emitterMode;

/* A string defining how particles are composited into the layer's
 * image. Current options are `unordered' (the default), `oldestFirst',
 * `oldestLast', `backToFront' (i.e. sorted into Z order) and
 * `additive'. The first four use source-over compositing, the last
 * uses additive compositing. */

@property(copy) NSString *renderMode;

/* When true the particles are rendered as if they directly inhabit the
 * three dimensional coordinate space of the layer's superlayer, rather
 * than being flattened into the layer's plane first. Defaults to false.
 * If true, the effect of the `filters', `backgroundFilters' and shadow-
 * related properties of the layer is undefined. */

@property BOOL preservesDepth;

/* Multiplies the cell-defined particle velocity. Defaults to one.
 * Animatable. */

@property float velocity;

/* Multiplies the cell-defined particle scale. Defaults to one. Animatable. */

@property float scale;

/* Multiplies the cell-defined particle spin. Defaults to one. Animatable. */

@property float spin;

/* The seed used to initialize the random number generator. Defaults to
 * zero. Each layer has its own RNG state. For properties with a mean M
 * and a range R, random values of the properties are uniformly
 * distributed in the interval [M - R/2, M + R/2]. */

@property unsigned int seed;

@end

/** `emitterShape' values. **/

CA_EXTERN NSString * const kCAEmitterLayerPoint
    __OSX_AVAILABLE_STARTING (__MAC_10_6, __IPHONE_NA);
CA_EXTERN NSString * const kCAEmitterLayerLine
    __OSX_AVAILABLE_STARTING (__MAC_10_6, __IPHONE_NA);
CA_EXTERN NSString * const kCAEmitterLayerRectangle
    __OSX_AVAILABLE_STARTING (__MAC_10_6, __IPHONE_NA);
CA_EXTERN NSString * const kCAEmitterLayerCuboid
    __OSX_AVAILABLE_STARTING (__MAC_10_6, __IPHONE_NA);
CA_EXTERN NSString * const kCAEmitterLayerCircle
    __OSX_AVAILABLE_STARTING (__MAC_10_6, __IPHONE_NA);
CA_EXTERN NSString * const kCAEmitterLayerSphere
    __OSX_AVAILABLE_STARTING (__MAC_10_6, __IPHONE_NA);

/** `emitterMode' values. **/

CA_EXTERN NSString * const kCAEmitterLayerPoints
    __OSX_AVAILABLE_STARTING (__MAC_10_6, __IPHONE_NA);
CA_EXTERN NSString * const kCAEmitterLayerOutline
    __OSX_AVAILABLE_STARTING (__MAC_10_6, __IPHONE_NA);
CA_EXTERN NSString * const kCAEmitterLayerSurface
    __OSX_AVAILABLE_STARTING (__MAC_10_6, __IPHONE_NA);
CA_EXTERN NSString * const kCAEmitterLayerVolume
    __OSX_AVAILABLE_STARTING (__MAC_10_6, __IPHONE_NA);

/** `renderOrder' values. **/

CA_EXTERN NSString * const kCAEmitterLayerUnordered
    __OSX_AVAILABLE_STARTING (__MAC_10_6, __IPHONE_NA);
CA_EXTERN NSString * const kCAEmitterLayerOldestFirst
    __OSX_AVAILABLE_STARTING (__MAC_10_6, __IPHONE_NA);
CA_EXTERN NSString * const kCAEmitterLayerOldestLast
    __OSX_AVAILABLE_STARTING (__MAC_10_6, __IPHONE_NA);
CA_EXTERN NSString * const kCAEmitterLayerBackToFront
    __OSX_AVAILABLE_STARTING (__MAC_10_6, __IPHONE_NA);
CA_EXTERN NSString * const kCAEmitterLayerAdditive
    __OSX_AVAILABLE_STARTING (__MAC_10_6, __IPHONE_NA);
