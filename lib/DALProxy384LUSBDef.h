#ifndef DALProxy384LUSBDEF_H
#define DALProxy384LUSBDEF_H

// Largest image properties expected
#define IRIMAGE_WIDTH 384
#define IRIMAGE_HEIGHT 288
#define IRIMAGE_PIXEL 2
#define IRIMAGE_RESOLUTION 16
#define IRIMAGE_PIXEL_MAX 0xFFFF

#define IRIMAGE_NBPIXELS (IRIMAGE_WIDTH * IRIMAGE_HEIGHT)

#define IRIMAGE_PAYLOAD (IRIMAGE_NBPIXELS * IRIMAGE_PIXEL )

#define IRIMAGE_META_COUNT 135    // Default type : uint32

#define IRIMAGE_META_TEMP 0
#define IRIMAGE_META_PERIOD 1   // In micro seconds
#define IRIMAGE_META_COUNTER 2   // 16bit effective counter
#define IRIMAGE_EPOCH 3         // Warning : int64 --> 2 slots
#define IRIMAGE_EPOCHLow 4      // Warning : int64 --> 2 slots
#define IRIMAGE_FRAMEINFO 5
#define IRIMAGE_FRAMEAGE 6
#define IRIMAGE_FRAMEHISTO 7 // Warning : uint32*128 -> 128slots
#define IRIMAGE_ENDFRAMEHISTO 134

/* Module Slot Type (see Proxy384LUSB_SlotType() ) */
#define SLOT_EMPTY 0
#define SLOT_TYPE_GAIN 1
#define SLOT_TYPE_OFFSET 2

/* NUC Slot Count */
#define NUC_SLOT_COUNT 8
/* NUC Bank Count */
#define NUC_BANK_COUNT 8

/* NUC Info free data size */
#define NUC_INFO_USER_SIZE 60

/* Bank Settings count */
#define BANK_SETTINGS_COUNT 8

// Maximum BAD Pixel allowed
#define BADPIXELS_COUNT 1023 // ( 4K/4 ( -1 to have space for the CRC ) )

// Mask pixel type
#define PIXEL_IGNORED 0
#define PIXEL_ORIGINAL 1
#define PIXEL_FIXED 2

// Feature string regular size
#define FEATURE_STRING_SIZE 32

enum eDeviceFeature
{
    // Ident & capabilities
    eStrPartNumber = 0,
    eStrSerialNumber,
    eStrIRSensorSerialNumber,
    eStrFirmwareBuildDataTime,
    eiFirmware0Version,
    eiFirmware1Version,
    eiIRSensorType,
    eiShutterAvailable,
    eiShutterlessAvailable,
    eiIsExportLimited,
    eiHardwareDetection,
    eiSensorOutputNbr,
    eiUSBSpeed,
    // IR Sentor Settings
    eiGain,
    eiGainMin,
    eiGainMax,
    efGSK,
    efGSKMin,
    efGSKMax,
    efGFID,
    efGFIDMin,
    efGFIDMax,
    efFPATemp,
    // Sequencer
    eiTriggerMode,
    efFrameRate,
    efFrameRateMin,
    efFrameRateMax,
    efExptime,
    efExptimeMin,
    efExptimeMax,
    efInterLine,
    efInterLineMin,
    efInterLineMax,
    // Video Stream
    eiVideoPattern,
    eiImageWidth,
    eiImageHeight,
    eiImagePixelFormat,
    eiUSBPayloadSize,
    eiFlipHorizontal,
    eiFlipVertical,
    eiVideoUSBOutActivation,
    eiVideoDF12OutputActivation,
    eiVideoDF12ChannelNbr,
    // Shutter management
    eiShutterMode,
    eiShutterActivation,
    efShutterFpaTempThreshold,
    eiShutterActivationDuration,

    // Power Management
    eiPowerMode,

    // Configuration
    eiSaveConfiguration,
    eiLoadConfiguration,
    eiLastConfiguration,

    // Table NUC
    eiLastSavedGainTable,
    eiLastSavedOffsetTable,

    // Table CRC
    eiTableCRC0,
    eiTableCRC1,
    eiTableCRC2,
    eiTableCRC3,
    eiTableCRC4,
    eiTableCRC5,
    eiTableCRC6,
    eiTableCRC7,
    // Last
    eDeviceFeatureTotal
};

enum ePowerModeValue
{
    ePowerNormal=0,
    ePowerStandby,
    ePowerDeepStandby
};

// AGC Processing
enum eAGCProcessingValue
{
    eNoAGC =0,
    eAGCEqHisto,
    eAGCEnhanced,
    eAGCLinear,
    eAGCTotal
};

// Enhancement processing (bitfield)
enum eEnhancementProcessingValue
{
    eNoEnhancement =0,
    eColumnCorrection = 1,  // b0
    eLineCorrection = 2,    // b1
    eDenoising=4            // b2
    //
};



#endif // DALProxy384LUSBDEF_H
