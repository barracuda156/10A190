/*
     File:       AudioToolbox/AudioFormat.h

     Contains:   API for finding things out about audio formats.

     Version:    Technology: Mac OS X
                 Release:    Mac OS X

     Copyright:  (c) 1985-2003 by Apple Computer, Inc., all rights reserved.

     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:

                     http://developer.apple.com/bugreporter/

*/
#if !defined(__AudioFormat_h__)
#define __AudioFormat_h__

//=============================================================================
//	Includes
//=============================================================================

//	System Includes
#include <Availability.h>
#if !defined(__COREAUDIO_USE_FLAT_INCLUDES__)
	#include <CoreAudio/CoreAudioTypes.h>
#else
	#include <CoreAudioTypes.h>
#endif

#if defined(__cplusplus)
extern "C"
{
#endif

//=============================================================================
//	Types
//=============================================================================

/*!
    @typedef	AudioFormatPropertyID
    @abstract   A type for four char codes for property IDs
*/
typedef UInt32	AudioFormatPropertyID;

#if !TARGET_OS_IPHONE
/*!
    @enum		PanningMode
    @abstract   (description)
    @discussion (description)
    @constant   kPanningMode_SoundField
					Sound field panning algorithm
    @constant   kPanningMode_VectorBasedPanning
					Vector based panning algorithm
*/
enum {
	kPanningMode_SoundField		 					= 3,
	kPanningMode_VectorBasedPanning					= 4
};

/*!
    @struct		AudioPanningInfo
    @abstract   This struct is for use with kAudioFormatProperty_PanningMatrix.
    @field      mPanningMode			the PanningMode to be used for the pan
    @field      mCoordinateFlags		the coordinates are specified as in the AudioChannelDescription struct in CoreAudioTypes.h
    @field      mCoordinates			the coordinates are specified as in the AudioChannelDescription struct in CoreAudioTypes.h
    @field      mGainScale				
					mGainScale is used to multiply the panning values.
					In typical usage you are applying an existing volume.
					value in 0 -> 1 (where 1 is unity gain) to the panned values.
					1 would give you panning at unity.
					0 would give you back a matrix of zeroes.
    @field      mOutputChannelMap				
					This is the channel map that is going to be used to determine channel volumes for this pan.
*/
struct AudioPanningInfo {
	UInt32						mPanningMode;
	UInt32						mCoordinateFlags;
	Float32						mCoordinates[3];	
	Float32						mGainScale;
	const AudioChannelLayout*	mOutputChannelMap;
};
typedef struct AudioPanningInfo AudioPanningInfo;

/*!
    @enum		AudioBalanceFadeType
    @abstract   used for mType field of AudioBalanceFade struct
    @constant   kAudioBalanceFadeType_MaxUnityGain
					the gain value never exceeds 1.0, the opposite channel fades out. 
					This can reduce overall loudness when the balance or fade is not in the center.
    @constant   kAudioBalanceFadeType_EqualPower
					The overall loudness remains constant, but gain can exceed 1.0.
					the gain value is 1.0 when the balance and fade are in the center.
					From there they can increase to +3dB (1.414) and decrease to -inf dB (0.0).
*/
enum {
	kAudioBalanceFadeType_MaxUnityGain = 0,
	kAudioBalanceFadeType_EqualPower = 1
};
	
/*!
    @struct		AudioBalanceFade
    @abstract   this struct is used with kAudioFormatProperty_BalanceFade
    @field      mLeftRightBalance 
					-1 is full left, 0 is center, +1 is full right
    @field      mBackFrontFade 
					-1 is full rear, 0 is center, +1 is full front
    @field      mType 
					an AudioBalanceFadeType constant
    @field      mChannelLayout 
					a pointer to an AudioChannelLayout
*/
struct AudioBalanceFade
{
	Float32						mLeftRightBalance;		// -1 is full left, 0 is center, +1 is full right
	Float32						mBackFrontFade;			// -1 is full rear, 0 is center, +1 is full front
	UInt32						mType;					// max unity gain, or equal power.
	const AudioChannelLayout*	mChannelLayout;
};
typedef struct AudioBalanceFade AudioBalanceFade;
#endif // !TARGET_OS_IPHONE

/*!
    @struct		AudioFormatInfo
    @abstract   this struct is used as a specifier for the kAudioFormatProperty_FormatList property
    @field      mASBD 
					an AudioStreamBasicDescription
    @field      mMagicCookie 
					a pointer to the decompression info for the data described in mASBD
    @field      mMagicCookieSize 
					the size in bytes of mMagicCookie
*/
struct AudioFormatInfo
{
	AudioStreamBasicDescription		mASBD;
	const void*						mMagicCookie;
	UInt32							mMagicCookieSize;
};
typedef struct AudioFormatInfo AudioFormatInfo;

/*!
    @struct		AudioFormatListItem
    @abstract   this struct is used as output from the kAudioFormatProperty_FormatList property
    @field      mASBD 
					an AudioStreamBasicDescription
    @field      mChannelLayoutTag 
					an AudioChannelLayoutTag
*/
struct AudioFormatListItem
{
	AudioStreamBasicDescription		mASBD;
	AudioChannelLayoutTag			mChannelLayoutTag;
};
typedef struct AudioFormatListItem AudioFormatListItem;

//=============================================================================
//	Properties - for various format structures.
//=============================================================================

/*!
    @enum		AudioFormatProperty constants
    @abstract   constants for use with AudioFormatGetPropertyInfo and AudioFormatGetProperty.
    @constant   kAudioFormatProperty_FormatInfo
					Retrieves general information about a format. The specifier is a
					magic cookie, or NULL.
					On input, the property value is an AudioStreamBasicDescription which 
					should have at least the mFormatID filled out. On output it will be filled out
					as much as possible given the information known about the format 
					and the contents of the magic cookie (if any is given).
					If multiple formats can be described by the AudioStreamBasicDescription and the associated magic cookie,
					this property will return the base level format.
    @constant   kAudioFormatProperty_FormatIsVBR
					Returns whether or not a format has a variable number of bytes per
					packet. The specifier is an AudioStreamBasicDescription describing
					the format to ask about. The value is a UInt32 where non-zero means
					the format is variable bytes per packet.
    @constant   kAudioFormatProperty_FormatIsExternallyFramed
					Returns whether or not a format requires external framing information,
					i.e. AudioStreamPacketDescriptions. 
					The specifier is an AudioStreamBasicDescription describing
					the format to ask about. The value is a UInt32 where non-zero means
					the format is externally framed. Any format which has variable byte sized packets
					requires AudioStreamPacketDescriptions.
    @constant   kAudioFormatProperty_EncodeFormatIDs
					No specifier needed. Must be set to NULL.
					Returns an array of UInt32 format IDs for formats that are valid output formats 
					for a converter. 
    @constant   kAudioFormatProperty_DecodeFormatIDs
					No specifier needed. Must be set to NULL.
					Returns an array of UInt32 format IDs for formats that are valid input formats 
					for a converter. 
    @constant   kAudioFormatProperty_Encoders
					The specifier is the format that you are interested in, e.g. 'aac '
					Returns an array of AudioClassDescriptions for all installed encoders for the given format 
    @constant   kAudioFormatProperty_Decoders
					The specifier is the format that you are interested in, e.g. 'aac '
					Returns an array of AudioClassDescriptions for all installed decoders for the given format 
    @constant   kAudioFormatProperty_AvailableEncodeBitRates
					The specifier is a UInt32 format ID.
					The property value is an array of AudioValueRange describing all available bit rates.
    @constant   kAudioFormatProperty_AvailableEncodeSampleRates
					The specifier is a UInt32 format ID.
					The property value is an array of AudioValueRange describing all available sample rates.
    @constant   kAudioFormatProperty_AvailableEncodeChannelLayoutTags
					The specifier is an AudioStreamBasicDescription with at least the mFormatID 
					and mChannelsPerFrame fields set.
					The property value is an array of AudioChannelLayoutTags for the format and number of
					channels specified. If mChannelsPerFrame is zero, then all layouts supported by
					the format are returned.	
    @constant   kAudioFormatProperty_AvailableEncodeNumberChannels
					The specifier is an AudioStreamBasicDescription with at least the mFormatID field set.
					The property value is an array of UInt32 indicating the number of channels that can be encoded.
					A value of 0xFFFFFFFF indicates that any number of channels may be encoded. 
    @constant   kAudioFormatProperty_FormatName
					Returns a name for a given format. The specifier is an
					AudioStreamBasicDescription describing the format to ask about.
					The value is a CFStringRef. The caller is responsible for releasing the
					returned string. For some formats (eg, Linear PCM) you will get back a
					descriptive string (e.g. 16-bit, interleaved, etc...)
    @constant   kAudioFormatProperty_ASBDFromESDS
					Returns an audio stream description for a given ESDS. The specifier is an ESDS. 
					The value is a AudioStreamBasicDescription. If multiple formats can be described 
					by the ESDS this property will return the base level format.
    @constant   kAudioFormatProperty_ChannelLayoutFromESDS
					Returns an audio channel layout for a given ESDS. The specifier is an
					ESDS. The value is a AudioChannelLayout.
    @constant   kAudioFormatProperty_ASBDFromMPEGPacket
					Returns an audio stream description for a given MPEG Packet. The specifier is an MPEG Packet. 
					The value is a AudioStreamBasicDescription.					
    @constant   kAudioFormatProperty_FormatList
					Returns a list of AudioFormatListItem structs describing the audio formats contained within the compressed bit stream
					as described by the magic cookie. The specifier is an AudioFormatInfo struct. The mFormatID member of the 
					ASBD struct must filled in. Formats are returned in order from the most to least 'rich', with 
					channel count taking the highest precedence followed by sample rate. The kAudioFormatProperty_FormatList property 
					is the preferred method for discovering format information of the audio data. If the audio data can only be described
					by a single AudioFormatListItem, this property would be equivalent to using the kAudioFormatProperty_FormatInfo property,
					which should be used by the application as a fallback case, to ensure backward compatibility with existing systems 
					when kAudioFormatProperty_FormatList is not present on the running system.
    @constant   kAudioFormatProperty_OutputFormatList
					Returns a list of AudioFormatListItem structs describing the audio formats which may be obtained by decoding the format
					described by the specifier.
					The specifier is an AudioFormatInfo struct. At a minimum formatID member of the ASBD struct must filled in. Other fields
					may be filled in. If there is no magic cookie, then the number of channels and sample rate should be filled in. 
	@constant   kAudioFormatProperty_ChannelLayoutForTag
					Returns the channel descriptions for a standard channel layout.
					The specifier is a AudioChannelLayoutTag (the mChannelLayoutTag field 
					of the AudioChannelLayout struct) containing the layout constant. 
					The value is an AudioChannelLayout structure. In typical use a AudioChannelLayout
					can be valid with just a defined AudioChannelLayoutTag (ie, those layouts
					have predefined speaker locations and orderings).
					Returns an error if the tag is kAudioChannelLayoutTag_UseChannelBitmap 
    @constant   kAudioFormatProperty_TagForChannelLayout
					Returns an AudioChannelLayoutTag for a layout, if there is one.
					The specifier is an AudioChannelLayout containing the layout description.
					The value is an AudioChannelLayoutTag.
					This can be used to reduce a layout specified by kAudioChannelLayoutTag_UseChannelDescriptions
					or kAudioChannelLayoutTag_UseChannelBitmap to a known AudioChannelLayoutTag.
    @constant   kAudioFormatProperty_ChannelLayoutForBitmap
					Returns the channel descriptions for a standard channel layout.
					The specifier is a UInt32 (the mChannelBitmap field 
					of the AudioChannelLayout struct) containing the layout bitmap. The value
					is an AudioChannelLayout structure. In some uses, an AudioChannelLayout can be 
					valid with the layoutTag set to "use bitmap" and the bitmap set appropriately.
    @constant   kAudioFormatProperty_BitmapForLayoutTag
					Returns a bitmap for an AudioChannelLayoutTag, if there is one.
					The specifier is a AudioChannelLayoutTag  containing the layout tag.
					The value is an UInt32 bitmap. The bits are as defined in CoreAudioTypes.h.
					To go in the other direction, i.e. get a layout tag for a bitmap, 
					use kAudioFormatProperty_TagForChannelLayout where your layout tag 
					is kAudioChannelLayoutTag_UseChannelBitmap and the bitmap is filled in.
    @constant   kAudioFormatProperty_ChannelLayoutName
					Returns the a name for a particular channel layout. The specifier is
					an AudioChannelLayout containing the layout description. The value
					is a CFStringRef. The caller is responsible for releasing the
					returned string.
    @constant   kAudioFormatProperty_ChannelName
					Returns the name for a particular channel. The specifier is an
					AudioChannelDescription which has the mChannelLabel field set. The value
					is a CFStringRef. The caller is responsible for releasing the
					returned string.
    @constant   kAudioFormatProperty_ChannelShortName
					Returns an abbreviated name for a particular channel. The specifier is an
					AudioChannelDescription which has the mChannelLabel field set. The value
					is a CFStringRef. The caller is responsible for releasing the
					returned string.
    @constant   kAudioFormatProperty_MatrixMixMap
					Returns a matrix of scaling coefficients for converting audio from one channel map 
					to another in a standard way, if one is known. Otherwise an error is returned.
					The specifier is an array of two pointers to AudioChannelLayout structures. 
					The first points to the input layout, the second to the output layout.
					The value is a two dimensional array of Float32 where the first dimension (rows) 
					is the input channel and the second dimension (columns) is the output channel. 
    @constant   kAudioFormatProperty_ChannelMap
					Returns an array of SInt32 for reordering input channels.
					The specifier is an array of two pointers to AudioChannelLayout structures. 
					The first points to the input layout, the second to the output layout.
					The length of the output array is equal to the number of output channels.
    @constant   kAudioFormatProperty_NumberOfChannelsForLayout
					This is a general call for parsing a AudioChannelLayout provided as the specifier,
					to determine the number of valid channels that are represented. So, if the
					LayoutTag is specified, it returns the number of channels for that layout. If 
					the bitmap is specified, it returns the number of channels represented by that bitmap.
					If the layout tag is 'kAudioChannelLayoutTag_UseChannelDescriptions' it returns
						the number of channel descriptions.
    @constant   kAudioFormatProperty_TagsForNumberOfChannels
					returns an array of AudioChannelLayoutTags for the number of channels specified.
					The specifier is a UInt32 number of channels. 
    @constant   kAudioFormatProperty_PanningMatrix
					This call will pass in an AudioPanningInfo struct that specifies one of the
					kPanningMode_ constants for the panning algorithm and an AudioChannelLayout
					to describe the destination channel layout. As in kAudioFormatProperty_MatrixMixMap
					the return value is an array of Float32 values of the number of channels
						represented by this specified channel layout. It is presumed that the source 
					being panned is mono (thus for a quad channel layout, 4 Float32 values are returned).
					The intention of this API is to provide support for panning operations that are
					strictly manipulating the respective volumes of the channels. Thus, more
					complex panners (like HRTF, distance filtering etc,) will not be represented
						by this API. The resultant volume scalars can then be applied to a mixer
					or some other processing code to adapt the individual volumes of the mixed
					output.
					The volume values will typically be presented within a 0->1 range (where 1 is unity gain)
					For stereo formats, vector based panning is equivalent to the equal-power pan mode.
    @constant   kAudioFormatProperty_BalanceFade
					get an array of coefficients for applying left/right balance and front/back fade.
					The specifier is an AudioBalanceFade struct. 
					the return value is an array of Float32 values of the number of channels
						represented by this specified channel layout.  
					The volume values will typically be presented within a 0->1 range (where 1 is unity gain)
    @constant   kAudioFormatProperty_ID3TagSize
					Returns a UInt32 indicating the ID3 tag size. 
					The specifier must begin with the ID3 tag header and be at least 10 bytes in length
    @constant   kAudioFormatProperty_ID3TagToDictionary
					Returns a CFDictionary containing key/value pairs for the frames in the ID3 tag
					The specifier is the entire ID3 tag
					Caller must call CFRelease for the returned dictionary
					
					
					
					
					
*/
enum
{
//=============================================================================
//	The following properties are concerned with the AudioStreamBasicDescription
//=============================================================================
	kAudioFormatProperty_FormatInfo						= 'fmti',
	kAudioFormatProperty_FormatName						= 'fnam',
	kAudioFormatProperty_EncodeFormatIDs				= 'acof',
	kAudioFormatProperty_DecodeFormatIDs				= 'acif',
	kAudioFormatProperty_FormatList						= 'flst',
    kAudioFormatProperty_ASBDFromESDS                   = 'essd',
    kAudioFormatProperty_ChannelLayoutFromESDS          = 'escl',
	kAudioFormatProperty_OutputFormatList				= 'ofls',
#if !TARGET_OS_IPHONE
	kAudioFormatProperty_FormatIsVBR					= 'fvbr',	
	kAudioFormatProperty_FormatIsExternallyFramed		= 'fexf',
	kAudioFormatProperty_Encoders						= 'aven',	
	kAudioFormatProperty_Decoders						= 'avde',
	kAudioFormatProperty_AvailableEncodeBitRates		= 'aebr',
	kAudioFormatProperty_AvailableEncodeSampleRates		= 'aesr',
	kAudioFormatProperty_AvailableEncodeChannelLayoutTags = 'aecl',
	kAudioFormatProperty_AvailableEncodeNumberChannels		= 'avnc',
	kAudioFormatProperty_ASBDFromMPEGPacket				= 'admp',
#endif // !TARGET_OS_IPHONE


//=============================================================================
//	The following properties concern the AudioChannelLayout struct (speaker layouts)
//=============================================================================
	kAudioFormatProperty_BitmapForLayoutTag				= 'bmtg',
	kAudioFormatProperty_MatrixMixMap					= 'mmap',
    kAudioFormatProperty_ChannelMap						= 'chmp',
	kAudioFormatProperty_NumberOfChannelsForLayout		= 'nchm',
	kAudioFormatProperty_ChannelLayoutForTag			= 'cmpl',
	kAudioFormatProperty_TagForChannelLayout			= 'cmpt',
	kAudioFormatProperty_ChannelLayoutName				= 'lonm',
#if !TARGET_OS_IPHONE
	kAudioFormatProperty_ChannelLayoutForBitmap			= 'cmpb',
	kAudioFormatProperty_ChannelName					= 'cnam',
	kAudioFormatProperty_ChannelShortName				= 'csnm',

	kAudioFormatProperty_TagsForNumberOfChannels		= 'tagc',				
	kAudioFormatProperty_PanningMatrix					= 'panm',
	kAudioFormatProperty_BalanceFade					= 'balf',
#endif // !TARGET_OS_IPHONE

//=============================================================================
//	The following properties concern the ID3 Tags
//=============================================================================

	kAudioFormatProperty_ID3TagSize						= 'id3s',
	kAudioFormatProperty_ID3TagToDictionary				= 'id3d'
};

//=============================================================================
//	Routines
//=============================================================================

/*!
    @function	AudioFormatGetPropertyInfo
    @abstract   Retrieve information about the given property
    @param      inPropertyID		an AudioFormatPropertyID constant.
    @param      inSpecifierSize		The size of the specifier data.
    @param      inSpecifier			A specifier is a buffer of data used as an input argument to some of the properties.
    @param      outDataSize			the the size in bytes of the current value of the property. In order to get the property value, 
									you will need a buffer of this size.
    @result     returns noErr if successful.
*/
extern OSStatus
AudioFormatGetPropertyInfo(	AudioFormatPropertyID	inPropertyID,
							UInt32					inSpecifierSize,
							const void*				inSpecifier,
							UInt32*					outPropertyDataSize)	__OSX_AVAILABLE_STARTING(__MAC_10_3,__IPHONE_2_0);

/*!
    @function	AudioFormatGetProperty
    @abstract   Retrieve the indicated property data
    @param      inPropertyID		an AudioFormatPropertyID constant.
    @param      inSpecifierSize		The size of the specifier data.
    @param      inSpecifier			A specifier is a buffer of data used as an input argument to some of the properties.
    @param      ioDataSize			on input the size of the outPropertyData buffer. On output the number of bytes written to the buffer.
    @param      outPropertyData		the buffer in which to write the property data. If outPropertyData is NULL and ioPropertyDataSize is
									not, the amount that would have been written will be reported.
    @result     returns noErr if successful.
*/
extern OSStatus
AudioFormatGetProperty(	AudioFormatPropertyID	inPropertyID,
						UInt32					inSpecifierSize,
						const void*				inSpecifier,
						UInt32*					ioPropertyDataSize,
						void*					outPropertyData)			__OSX_AVAILABLE_STARTING(__MAC_10_3,__IPHONE_2_0);


//-----------------------------------------------------------------------------
//  AudioFormat Error Codes
//-----------------------------------------------------------------------------

enum {
        kAudioFormatUnspecifiedError						= 'what',
        kAudioFormatUnsupportedPropertyError 				= 'prop',
        kAudioFormatBadPropertySizeError 					= '!siz',
        kAudioFormatBadSpecifierSizeError 					= '!spc',
        kAudioFormatUnsupportedDataFormatError 				= 'fmt?',
        kAudioFormatUnknownFormatError 						= '!fmt'
};


#if defined(__cplusplus)
}
#endif

#endif
