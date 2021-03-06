/* CoreAnimation - CALayer.h

   Copyright (c) 2006-2007 Apple Inc.
   All rights reserved. */

#import <QuartzCore/CAMediaTiming.h>
#import <QuartzCore/CATransform3D.h>
#import <Foundation/NSObject.h>

@class NSArray, NSDictionary, NSEnumerator, CAAnimation, CALayerArray;
@protocol CAAction;

/* Bit definitions for `autoresizingMask' property. */

enum CAAutoresizingMask
{
  kCALayerNotSizable	= 0,
  kCALayerMinXMargin	= 1U << 0,
  kCALayerWidthSizable	= 1U << 1,
  kCALayerMaxXMargin	= 1U << 2,
  kCALayerMinYMargin	= 1U << 3,
  kCALayerHeightSizable	= 1U << 4,
  kCALayerMaxYMargin	= 1U << 5
};

/* Bit definitions for `edgeAntialiasingMask' property. */

enum CAEdgeAntialiasingMask
{
  kCALayerLeftEdge	= 1U << 0,
  kCALayerRightEdge	= 1U << 1,
  kCALayerBottomEdge	= 1U << 2,
  kCALayerTopEdge	= 1U << 3,
};

/** The base layer class. **/

@interface CALayer : NSObject <NSCoding, CAMediaTiming>
{
@private
  struct _CALayerIvars {
    int32_t refcount;
    uint32_t flags;
    uintptr_t parent;
    CALayerArray *sublayers;
    CALayer *mask;
    struct _CALayerState *state;
    struct _CALayerState *previous_state;
    struct _CALayerAnimation *animations;
    uintptr_t slots[3];
#if defined (__LP64__) && __LP64__
    uint32_t reserved;
#endif
  } _attr;
}

/** Layer creation and initialization. **/

+ (id)layer;

/* The designated initializer. */

- (id)init;

/* This initializer is used by CoreAnimation to create shadow copies of
 * layers, e.g. for use as presentation layers. Subclasses can override
 * this method to copy their instance variables into the presentation
 * layer (subclasses should call the superclass afterwards.). Calling this
 * method in any other situation will result in undefined behavior. */

- (id)initWithLayer:(id)layer;

/* Returns a copy of the layer containing all properties as they were
 * at the start of the current transaction, with any active animations
 * applied. This gives a close approximation to the version of the layer
 * that is currently displayed. Returns nil if the layer has not yet
 * been committed.
 *
 * The effect of attempting to modify the returned layer in any way is
 * undefined.
 *
 * The `sublayers', `mask' and `superlayer' properties of the returned
 * layer return the presentation versions of these properties. This
 * carries through to read-only layer methods. E.g., calling -hitTest:
 * on the result of the -presentationLayer will query the presentation
 * values of the layer tree. */

- (id)presentationLayer;

/* When called on the result of the -presentationLayer method, returns
 * the underlying layer with the current model values. When called on a
 * non-presentation layer, returns the receiver. The result of calling
 * this method after the transaction that produced the presentation
 * layer has completed is undefined. */

- (id)modelLayer;

/** Property methods. **/

/* CALayer implements the standard NSKeyValueCoding protocol for all
 * Objective C properties defined by the class and its subclasses. It
 * dynamically implements missing accessor methods for properties
 * declared by subclasses.
 *
 * When accessing properties via KVC whose values are not objects, the
 * standard KVC wrapping conventions are used, with extensions to
 * support the following types:
 *
 *	C Type			Class
 *      ------			-----
 *	CGPoint			NSValue
 *	CGSize			NSValue
 *	CGRect			NSValue
 *	CGAffineTransform	NSAffineTransform
 *	CATransform3D		NSValue  */

/* Returns the default value of the named property, or nil if no
 * default value is known. Subclasses that override this method to
 * define default values for their own properties should call `super'
 * for unknown properties. */

+ (id)defaultValueForKey:(NSString *)key;

/* Called by the object's implementation of -encodeWithCoder:, returns
 * false if the named property should not be archived. The base
 * implementation returns true. Subclasses should call super for
 * unknown properties. */

- (BOOL)shouldArchiveValueForKey:(NSString *)key;

/** Geometry and layer hierarchy properties. **/

/* The bounds of the layer. Defaults to the null rectangle. Animatable. */

@property CGRect bounds;

/* The position in the superlayer that the anchor point of the layer's
 * bounds rect is aligned to. Defaults to the zero point. Animatable. */

@property CGPoint position;

/* The Z component of the layer's position in its superlayer. Defaults
 * to zero. Animatable. */

@property CGFloat zPosition;

/* Defines the anchor point of the layer's bounds rect, as a point in
 * the unit coordinate space - '(0, 0)' is the bottom left corner of
 * the bounds rect, '(1, 1)' is the top right corner. Defaults to
 * '(0.5, 0.5)', i.e. the center of the bounds rect. Animatable. */

@property CGPoint anchorPoint;

/* The Z component of the layer's anchor point (i.e. reference point for
 * position and transform). Defaults to zero. Animatable. */

@property CGFloat anchorPointZ;

/* A transform applied to the layer relative to the anchor point of its
 * bounds rect. Defaults to the identity transform. Animatable. */

@property CATransform3D transform;

/* Convenience methods for accessing the `transform' property as an
 * affine transform. */

- (CGAffineTransform)affineTransform;
- (void)setAffineTransform:(CGAffineTransform)m;

/* Unlike NSView, each Layer in the hierarchy has an implicit frame
 * rectangle, a function of the `position', `bounds', `anchorPoint',
 * and `transform' properties. When setting the frame the `position'
 * and `bounds.size' are changed to match the given frame. */

@property CGRect frame;

/* When true the layer and its sublayers are not displayed. Defaults to
 * false. Animatable. */

@property(getter=isHidden) BOOL hidden;

/* When false layers facing away from the viewer are hidden from view.
 * Defaults to true. Animatable. */

@property(getter=isDoubleSided) BOOL doubleSided;

/* Whether or not the geometry of the layer (and its sublayers) is
 * flipped vertically. Defaults to false. Note that even when geometry
 * is flipped, image orientation remains the same (i.e. a CGImageRef
 * stored in the `contents' property will display the same with both
 * flipped=false and flipped=true, assuming no transform on the layer.)
 * Added in Mac OS X 10.6. */

@property(getter=isGeometryFlipped) BOOL geometryFlipped;

/* Returns true if the contents of the contents property of the layer
 * will be implicitly flipped when rendered in relation to the local
 * coordinate space (e.g. if there are an odd number of layers with
 * flippedGeometry=YES from the receiver up to and including the
 * implicit container of the root layer.) Subclasses should not attempt
 * to redefine this method. When this method returns true the
 * CGContextRef object passed to -drawInContext: by the default
 * -display method will have been y- flipped (and rectangles passed to
 * -setNeedsDisplayInRect: will be similarly flipped.) */

- (BOOL)contentsAreFlipped;

/* The receiver's superlayer object. Implicitly changed to match the
 * hierarchy described by the `sublayers' properties. */

@property(readonly) CALayer *superlayer; 

/* Removes the layer from its superlayer, works both if the receiver is
 * in its superlayer's `sublayers' array or set as its `mask' value. */

- (void)removeFromSuperlayer;

/* The array of sublayers of this layer. The layers are listed in back
 * to front order. Defaults to nil. When setting the value of the
 * property, any newly added layers must have nil superlayers, otherwise
 * the behavior is undefined. Note that the returned array is not
 * guaranteed to retain its elements. */

@property(copy) NSArray *sublayers;

/* Add 'layer' to the end of the receiver's sublayers array. If 'layer'
 * already has a superlayer, it will be removed before being added. */

- (void)addSublayer:(CALayer *)layer;

/* Insert 'layer' at position 'idx' in the receiver's sublayers array.
 * If 'layer' already has a superlayer, it will be removed before being
 * inserted. */

- (void)insertSublayer:(CALayer *)layer atIndex:(unsigned)idx;

/* Insert 'layer' either above or below the specified layer in the
 * receiver's sublayers array. If 'layer' already has a superlayer, it
 * will be removed before being inserted. */

- (void)insertSublayer:(CALayer *)layer below:(CALayer *)sibling;
- (void)insertSublayer:(CALayer *)layer above:(CALayer *)sibling;

/* Remove 'layer' from the sublayers array of the receiver and insert
 * 'layer2' if non-nil in its position. If the superlayer of 'layer'
 * is not the receiver, the behavior is undefined. */

- (void)replaceSublayer:(CALayer *)layer with:(CALayer *)layer2;

/* A transform applied to each member of the `sublayers' array while
 * rendering its contents into the receiver's output. Typically used as
 * the projection matrix to add perspective and other viewing effects
 * into the model. Defaults to identity. Animatable. */

@property CATransform3D sublayerTransform;

/* A layer whose alpha channel is used as a mask to select between the
 * layer's background and the result of compositing the layer's
 * contents with its filtered background. Defaults to nil. When used as
 * a mask the layer's `compositingFilter' and `backgroundFilters'
 * properties are ignored. When setting the mask to a new layer, the
 * new layer must have a nil superlayer, otherwise the behavior is
 * undefined. */

@property(retain) CALayer *mask;

/* When true an implicit mask matching the layer bounds is applied to
 * the layer (including the effects of the `cornerRadius' property). If
 * both `mask' and `masksToBounds' are non-nil the two masks are
 * multiplied to get the actual mask values. Defaults to false.
 * Animatable. */

@property BOOL masksToBounds;

/** Mapping between layer coordinate and time spaces. **/

- (CGPoint)convertPoint:(CGPoint)p fromLayer:(CALayer *)l;
- (CGPoint)convertPoint:(CGPoint)p toLayer:(CALayer *)l;
- (CGRect)convertRect:(CGRect)r fromLayer:(CALayer *)l;
- (CGRect)convertRect:(CGRect)r toLayer:(CALayer *)l;

- (CFTimeInterval)convertTime:(CFTimeInterval)t fromLayer:(CALayer *)l;
- (CFTimeInterval)convertTime:(CFTimeInterval)t toLayer:(CALayer *)l;

/** Hit testing methods. **/

/* Returns the farthest descendant of the layer containing point 'p'.
 * Siblings are searched in top-to-bottom order. 'p' is in the
 * coordinate system of the receiver's superlayer. */

- (CALayer *)hitTest:(CGPoint)p;

/* Returns true if the bounds of the layer contains point 'p'. */

- (BOOL)containsPoint:(CGPoint)p;

/** Layer content properties and methods. **/

/* An object providing the contents of the layer, typically a CGImageRef,
 * but may be something else. Default value is nil. Animatable. */

@property(retain) id contents;

/* A rectangle in the unit coordinate space defining the subrectangle
 * of the `contents' property that will be drawn into the layer. If
 * pixels outside the unit rectangles are requested, the edge pixels of
 * the contents image will be extended outwards. If an empty rectangle
 * is provided, the results are undefined. Defaults to the unit
 * rectangle [0 0 1 1]. Animatable. */

@property CGRect contentsRect;

/* A string defining how the contents of the layer is mapped into its
 * bounds rect. Options are `center', `top', `bottom', `left',
 * `right', `topLeft', `topRight', `bottomLeft', `bottomRight',
 * `resize', `resizeAspect', `resizeAspectFill'. The default value is
 * `resize'. */

@property(copy) NSString *contentsGravity;

/* The filter types to use when rendering the `contents' property of
 * the layer. The minification filter is used when to reduce the size
 * of image data, the magnification filter to increase the size of
 * image data. Currently the allowed values are `nearest' and `linear'.
 * Both properties default to `linear'. */

@property(copy) NSString *minificationFilter, *magnificationFilter;

/* The bias factor added when determining which levels of detail to use
 * when minifying using trilinear filtering. The default value is 0. */

@property float minificationFilterBias;

/* A hint marking that the layer contents provided by -drawInContext:
 * is completely opaque. Defaults to false. Note that this does not affect
 * the interpretation of the `contents' property directly. */

@property(getter=isOpaque) BOOL opaque;

/* Reload the content of this layer. Calls the -drawInContext: method
 * then updates the `contents' property of the layer. Typically this is
 * not called directly. */

- (void)display;

/* Marks that -display needs to be called before the layer is next
 * committed. If a region is specified, only that region of the layer
 * is invalidated. */

- (void)setNeedsDisplay;
- (void)setNeedsDisplayInRect:(CGRect)r;

/* Returns true when the layer is marked as needing redrawing. */

- (BOOL)needsDisplay;

/* Call -display if receiver is marked as needing redrawing. */

- (void)displayIfNeeded;

/* When true -setNeedsDisplay will automatically be called when the
 * bounds of the layer changes. Default value is false. */

@property BOOL needsDisplayOnBoundsChange;

/* Called via the -display method when the `contents' property is being
 * updated. Default implementation does nothing. The context may be
 * clipped to protect valid layer content. Subclasses that wish to find
 * the actual region to draw can call CGContextGetClipBoundingBox(). */

- (void)drawInContext:(CGContextRef)ctx;

/** Rendering properties and methods. **/

/* Renders the receiver and its sublayers into 'ctx'. This method
 * renders directly from the layer tree. Renders in the coordinate space
 * of the layer.
 *
 * WARNING: currently this method does not implement the full
 * CoreAnimation composition model, use with caution. */

- (void)renderInContext:(CGContextRef)ctx;

/* Defines how the edges of the layer are rasterized. For each of the
 * four edges (left, right, bottom, top) if the corresponding bit is
 * set the edge will be antialiased. Typically this property is used to
 * disable antialiasing for edges that abut edges of other layers, to
 * eliminate the seams that would otherwise occur. The default value is
 * for all edges to be antialiased. */

@property unsigned int edgeAntialiasingMask;

/* The background color of the layer. Default value is nil. Colors
 * created from tiled patterns are supported. Animatable. */

@property CGColorRef backgroundColor;

/* When positive, the background of the layer will be drawn with
 * rounded corners. Also effects the mask generated by the
 * `masksToBounds' property. Defaults to zero. Animatable. */

@property CGFloat cornerRadius;

/* The width of the layer's border, inset from the layer bounds. The
 * border is composited above the layer's content and sublayers and
 * includes the effects of the `cornerRadius' property. Defaults to
 * zero. Animatable. */

@property CGFloat borderWidth;

/* The color of the layer's border. Defaults to opaque black. Colors
 * created from tiled patterns are supported. Animatable. */

@property CGColorRef borderColor;

/* The opacity of the layer, as a value between zero and one. Defaults
 * to one. Specifying a value outside the [0,1] range will give undefined
 * results. Animatable. */

@property float opacity;

/* A filter object used to composite the layer with its (possibly
 * filtered) background. Default value is nil, which implies source-
 * over compositing. Animatable.
 *
 * Note that if the inputs of the filter are modified directly after
 * the filter is attached to a layer, the behavior is undefined. The
 * filter must either be reattached to the layer, or filter properties
 * should be modified by calling -setValue:forKeyPath: on each layer
 * that the filter is attached to. (This also applies to the `filters'
 * and `backgroundFilters' properties.) */

@property(retain) id compositingFilter;

/* An array of filters that will be applied to the contents of the
 * layer and its sublayers. Defaults to nil. Animatable. */

@property(copy) NSArray *filters;

/* An array of filters that are applied to the background of the layer.
 * The root layer ignores this property. Animatable. */

@property(copy) NSArray *backgroundFilters;

/** Shadow properties. **/

/* The color of the shadow. Defaults to opaque black. Colors created
 * from patterns are currently NOT supported. Animatable. */

@property CGColorRef shadowColor;

/* The opacity of the shadow. Defaults to 0. Specifying a value outside the
 * [0,1] range will give undefined results. Animatable. */

@property float shadowOpacity;

/* The shadow offset. Defaults to (0, -3). Animatable. */

@property CGSize shadowOffset;

/* The blur radius used to create the shadow. Defaults to 3. Animatable. */

@property CGFloat shadowRadius;

/** Layout methods. **/

/* A bitmask defining how the layer is resized when the bounds of its
 * superlayer changes. See the CAAutoresizingMask enum for the bit
 * definitions. Default value is zero. */

@property unsigned int autoresizingMask;

/* The object responsible for assigning frame rects to sublayers,
 * should implement methods from the CALayoutManager informal protocol.
 * When nil (the default value) only the autoresizing style of layout
 * is done (unless a subclass overrides -layoutSublayers). */

@property(retain) id layoutManager;

/* Returns the preferred frame size of the layer in the coordinate
 * space of the superlayer. The default implementation calls the layout
 * manager if one exists and it implements the -preferredSizeOfLayer:
 * method, otherwise returns the size of the bounds rect mapped into
 * the superlayer. */

- (CGSize)preferredFrameSize;

/* Marks that -layoutSublayers needs to be invoked on the receiver
 * before the next update. If the receiver's layout manager implements
 * the -invalidateLayoutOfLayer: method it will be called.
 *
 * This method is automatically invoked on a layer whenever its
 * `sublayers' or `layoutManager' property is modified, and is invoked
 * on the layer and its superlayer whenever its `bounds' or `transform'
 * properties are modified. Implicit calls to -setNeedsLayout are
 * skipped if the layer is currently executing its -layoutSublayers
 * method. */

- (void)setNeedsLayout;

/* Returns true when the receiver is marked as needing layout. */

- (BOOL)needsLayout;

/* Traverse upwards from the layer while the superlayer requires layout.
 * Then layout the entire tree beneath that ancestor. */

- (void)layoutIfNeeded;

/* Called when the layer requires layout. The default implementation
 * calls the layout manager if one exists and it implements the
 * -layoutSublayersOfLayer: method. Subclasses can override this to
 * provide their own layout algorithm, which should set the frame of
 * each sublayer. */

- (void)layoutSublayers;

/* NSView-style springs and struts layout. -resizeSublayersWithOldSize:
 * is called when the layer's bounds rect is changed. It calls
 * -resizeWithOldSuperlayerSize: to resize the sublayer's frame to
 * match the new superlayer bounds based on the sublayer's autoresizing
 * mask. */

- (void)resizeSublayersWithOldSize:(CGSize)size;
- (void)resizeWithOldSuperlayerSize:(CGSize)size;

/** Action methods. **/

/* An "action" is an object that responds to an "event" via the
 * CAAction protocol (see below.) Events are named using standard
 * dot-separated key paths. Each layer defines a mapping from event key
 * paths to action objects. Events are posted by looking up the action
 * object associated with the key path and sending it the method
 * defined by the CAAction protocol.
 *
 * When an action object is invoked it receives three parameters: the
 * key path naming the event, the object on which the event happened
 * (i.e. the layer), and optionally a dictionary of named arguments
 * specific to each event.
 *
 * To provide implicit animations for layer properties, an event with
 * the same name as each property is posted whenever the value of the
 * property is modified. A suitable CAAnimation object is associated by
 * default with each implicit event (CAAnimation implements the action
 * protocol.)
 *
 * The layer class also defines the following events that are not
 * linked directly to properties:
 *
 * onOrderIn
 *	Invoked when the layer is made visible, i.e. either its
 *	superlayer becomes visible, or it's added as a sublayer of a
 *	visible layer
 *
 * onOrderOut
 *	Invoked when the layer becomes non-visible. */

/* Returns the default action object associated with the event named by
 * the string 'event'. The default implementation returns a suitable
 * animation object for events posted by animatable properties, nil
 * otherwise. */

+ (id<CAAction>)defaultActionForKey:(NSString *)event;

/* Returns the action object associated with the event named by the
 * string 'event'. The default implementation searches for an action
 * object in the following places:
 *
 * 1. if defined, call the delegate method -actionForLayer:forKey:
 * 2. look in the layer's `actions' dictionary
 * 3. look in any `actions' dictionaries in the `style' hierarchy
 * 4. call +defaultActionForKey: on the layer's class
 *
 * If any of these steps results in a non-nil action object, the
 * following steps are ignored. If the final result is an instance of
 * NSNull, it is converted to `nil'. */

- (id<CAAction>)actionForKey:(NSString *)event;

/* A dictionary mapping keys to objects implementing the CAAction
 * protocol. Default value is nil. */

@property(copy) NSDictionary *actions;

/** Animation methods. **/

/* Attach an animation object to the layer. Typically this is implicitly
 * invoked through an action that is an CAAnimation object.
 *
 * 'key' may be any string such that only one animation per unique key
 * is added per layer. The special key 'transition' is automatically
 * used for transition animations. The nil pointer is also a valid key.
 *
 * If the `duration' property of the animation is zero or negative it
 * is given the default duration, either the value of the
 * `animationDuration' transaction property or .25 seconds otherwise.
 *
 * The animation is copied before being added to the layer, so any
 * subsequent modifications to `anim' will have no affect unless it is
 * added to another layer. */

- (void)addAnimation:(CAAnimation *)anim forKey:(NSString *)key;

/* Remove all animations attached to the layer. */

- (void)removeAllAnimations;

/* Remove any animation attached to the layer for 'key'. */

- (void)removeAnimationForKey:(NSString *)key;

/* Returns an array containing the keys of all animations currently
 * attached to the receiver. The order of the array matches the order
 * in which animations will be applied. */

- (NSArray *)animationKeys;

/* Returns the animation added to the layer with identifier 'key', or nil
 * if no such animation exists. Attempting to modify any properties of
 * the returned object will result in undefined behavior. */

- (CAAnimation *)animationForKey:(NSString *)key;

/** Miscellaneous properties. **/

/* The name of the layer. Used by some layout managers. Defaults to nil. */

@property(copy) NSString *name;

/* An object that will receive the CALayer delegate methods defined
 * below (for those that it implements). The value of this property is
 * not retained. Default value is nil. */

@property(assign) id delegate;

/* When non-nil, a dictionary dereferenced to find property values that
 * aren't explicitly defined by the layer. (This dictionary may in turn
 * have a `style' property, forming a hierarchy of default values.)
 * If the style dictionary doesn't define a value for an attribute, the
 * +defaultValueForKey: method is called. Defaults to nil.
 *
 * Note that if the dictionary or any of its ancestors are modified,
 * the values of the layer's properties are undefined until the `style'
 * property is reset. */

@property(copy) NSDictionary *style;

@end

/** Layout manager protocol. **/

@interface NSObject (CALayoutManager)

/* Called when the preferred size of 'layer' may have changed. The
 * receiver is responsible for recomputing the preferred size and
 * returning it. */

- (CGSize)preferredSizeOfLayer:(CALayer *)layer;

/* Called when the preferred size of 'layer' may have changed. The
 * receiver should invalidate any cached state. */

- (void)invalidateLayoutOfLayer:(CALayer *)layer;

/* Called when the sublayers of 'layer' may need rearranging (e.g. if
 * something changed size). The receiver is responsible for changing
 * the frame of each sublayer that needs a new layout. */

- (void)layoutSublayersOfLayer:(CALayer *)layer;

@end

/** Action (event handler) protocol. **/

@protocol CAAction

/* Called to trigger the event named 'path' on the receiver. The object
 * (e.g. the layer) on which the event happened is 'anObject'. The
 * arguments dictionary may be nil, if non-nil it carries parameters
 * associated with the event. */

- (void)runActionForKey:(NSString *)event object:(id)anObject
    arguments:(NSDictionary *)dict;

@end

/** Delegate methods. **/

@interface NSObject (CALayerDelegate)

/* If defined, called by the default implementation of the -display
 * method, in which case it should implement the entire display
 * process (typically by setting the `contents' property.) */

- (void)displayLayer:(CALayer *)layer;

/* If defined, called by the default implementation of -drawInContext: */

- (void)drawLayer:(CALayer *)layer inContext:(CGContextRef)ctx;

/* Called by the default -layoutSublayers implementation before the layout
 * manager is checked. Note that if the delegate method is invoked, the
 * layout manager will be ignored. */

- (void)layoutSublayersOfLayer:(CALayer *)layer;

/* If defined, called by the default implementation of the
 * -actionForKey: method. Should return an object implementating the
 * CAAction protocol. May return 'nil' if the delegate doesn't specify
 * a behavior for the current event. Returning the null object (i.e.
 * '[NSNull null]') explicitly forces no further search. (i.e. the
 * +defaultActionForKey: method will not be called) */

- (id<CAAction>)actionForLayer:(CALayer *)layer forKey:(NSString *)event;

@end

/** Layer `contentsGravity' values. **/

CA_EXTERN NSString * const kCAGravityCenter
    __OSX_AVAILABLE_STARTING (__MAC_10_5, __IPHONE_2_0);
CA_EXTERN NSString * const kCAGravityTop
    __OSX_AVAILABLE_STARTING (__MAC_10_5, __IPHONE_2_0);
CA_EXTERN NSString * const kCAGravityBottom
    __OSX_AVAILABLE_STARTING (__MAC_10_5, __IPHONE_2_0);
CA_EXTERN NSString * const kCAGravityLeft
    __OSX_AVAILABLE_STARTING (__MAC_10_5, __IPHONE_2_0);
CA_EXTERN NSString * const kCAGravityRight
    __OSX_AVAILABLE_STARTING (__MAC_10_5, __IPHONE_2_0);
CA_EXTERN NSString * const kCAGravityTopLeft
    __OSX_AVAILABLE_STARTING (__MAC_10_5, __IPHONE_2_0);
CA_EXTERN NSString * const kCAGravityTopRight
    __OSX_AVAILABLE_STARTING (__MAC_10_5, __IPHONE_2_0);
CA_EXTERN NSString * const kCAGravityBottomLeft
    __OSX_AVAILABLE_STARTING (__MAC_10_5, __IPHONE_2_0);
CA_EXTERN NSString * const kCAGravityBottomRight
    __OSX_AVAILABLE_STARTING (__MAC_10_5, __IPHONE_2_0);
CA_EXTERN NSString * const kCAGravityResize
    __OSX_AVAILABLE_STARTING (__MAC_10_5, __IPHONE_2_0);
CA_EXTERN NSString * const kCAGravityResizeAspect
    __OSX_AVAILABLE_STARTING (__MAC_10_5, __IPHONE_2_0);
CA_EXTERN NSString * const kCAGravityResizeAspectFill
    __OSX_AVAILABLE_STARTING (__MAC_10_5, __IPHONE_2_0);

/** Contents filter names. **/

CA_EXTERN NSString * const kCAFilterNearest
    __OSX_AVAILABLE_STARTING (__MAC_10_5, __IPHONE_2_0);
CA_EXTERN NSString * const kCAFilterLinear
    __OSX_AVAILABLE_STARTING (__MAC_10_5, __IPHONE_2_0);

/* Trilinear minification filter. Enables mipmap generation. Some
 * renderers may ignore this, or impose additional restrictions, such
 * as source images requiring power-of-two dimensions. */

CA_EXTERN NSString * const kCAFilterTrilinear
    __OSX_AVAILABLE_STARTING (__MAC_10_6, __IPHONE_NA);


/** Layer event names. **/

CA_EXTERN NSString * const kCAOnOrderIn
    __OSX_AVAILABLE_STARTING (__MAC_10_5, __IPHONE_2_0);
CA_EXTERN NSString * const kCAOnOrderOut
    __OSX_AVAILABLE_STARTING (__MAC_10_5, __IPHONE_2_0);

/** The animation key used for transitions. **/

CA_EXTERN NSString * const kCATransition
    __OSX_AVAILABLE_STARTING (__MAC_10_5, __IPHONE_2_0);
