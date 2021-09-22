#ifndef DALProxy384LUSBDEF_H
#define DALProxy384LUSBDEF_H

// Largest image properties expected
#define IRIMAGE384_WIDTH 384
#define IRIMAGE384_HEIGHT 288
//#define IRIMAGE384_PIXEL 2
//#define IRIMAGE384_RESOLUTION 16
//#define IRIMAGE384_PIXEL_MAX 0xFFFF
//
#define IRIMAGE384_NBPIXELS (IRIMAGE384_WIDTH * IRIMAGE384_HEIGHT)
//
//#define IRIMAGE384_PAYLOAD (IRIMAGE384_NBPIXELS * IRIMAGE384_PIXEL )
//
//#define IRIMAGE384_META_COUNT 135    // Default type : uint32
//
//#define IRIMAGE384_META_TEMP 0
//#define IRIMAGE384_META_PERIOD 1   // In micro seconds
//#define IRIMAGE384_META_COUNTER 2   // 16bit effective counter
//#define IRIMAGE384_EPOCH 3         // Warning : int64 --> 2 slots
//#define IRIMAGE384_EPOCHLow 4      // Warning : int64 --> 2 slots
//#define IRIMAGE384_FRAMEINFO 5
//#define IRIMAGE384_FRAMEAGE 6
//#define IRIMAGE384_FRAMEHISTO 7 // Warning : uint32*128 -> 128slots
//#define IRIMAGE384_ENDFRAMEHISTO 134
//
///* Module Slot Type (see Proxy384LUSB_SlotType() ) */
//#define SLOT384_EMPTY 0
//#define SLOT384_TYPE_GAIN 1
//#define SLOT384_TYPE_OFFSET 2
//
///* NUC Slot Count */
//#define NUC384_SLOT_COUNT 8
///* NUC Bank Count */
//#define NUC384_BANK_COUNT 8
//
///* NUC Info free data size */
//#define NUC384_INFO_USER_SIZE 60
//
///* Bank Settings count */
//#define BANK384_SETTINGS_COUNT 8
//
//// Maximum BAD Pixel allowed
//#define BADPIXELS384_COUNT 1023 // ( 4K/4 ( -1 to have space for the CRC ) )
//
//// Mask pixel type
//#define PIXEL384_IGNORED 0
//#define PIXEL384_ORIGINAL 1
//#define PIXEL384_FIXED 2
//
//// Feature string regular size
//#define FEATURE384_STRING_SIZE 32
//
//enum eDeviceFeature384
//{
//    // Ident & capabilities
//    eStrPartNumber = 0,
//    eStrSerialNumber,
//    eStrIRSensorSerialNumber,
//    eStrFirmwareBuildDataTime,
//    eiFirmware0Version,
//    eiFirmware1Version,
//    eiIRSensorType,
//    eiShutterAvailable,
//    eiShutterlessAvailable,
//    eiIsExportLimited,
//    eiHardwareDetection,
//    eiSensorOutputNbr,
//    eiUSBSpeed,
//    // IR Sentor Settings
//    eiGain,
//    eiGainMin,
//    eiGainMax,
//    efGSK,
//    efGSKMin,
//    efGSKMax,
//    efGFID,
//    efGFIDMin,
//    efGFIDMax,
//    efFPATemp,
//    // Sequencer
//    eiTriggerMode,
//    efFrameRate,
//    efFrameRateMin,
//    efFrameRateMax,
//    efExptime,
//    efExptimeMin,
//    efExptimeMax,
//    efInterLine,
//    efInterLineMin,
//    efInterLineMax,
//    // Video Stream
//    eiVideoPattern,
//    eiImageWidth,
//    eiImageHeight,
//    eiImagePixelFormat,
//    eiUSBPayloadSize,
//    eiFlipHorizontal,
//    eiFlipVertical,
//    eiVideoUSBOutActivation,
//    eiVideoDF12OutputActivation,
//    eiVideoDF12ChannelNbr,
//    // Shutter management
//    eiShutterMode,
//    eiShutterActivation,
//    efShutterFpaTempThreshold,
//    eiShutterActivationDuration,
//
//    // Power Management
//    eiPowerMode,
//
//    // Configuration
//    eiSaveConfiguration,
//    eiLoadConfiguration,
//    eiLastConfiguration,
//
//    // Table NUC
//    eiLastSavedGainTable,
//    eiLastSavedOffsetTable,
//
//    // Table CRC
//    eiTableCRC0,
//    eiTableCRC1,
//    eiTableCRC2,
//    eiTableCRC3,
//    eiTableCRC4,
//    eiTableCRC5,
//    eiTableCRC6,
//    eiTableCRC7,
//    // Last
//    eDeviceFeatureTotal
//};
//
//enum ePowerModeValue384
//{
//    ePowerNormal=0,
//    ePowerStandby,
//    ePowerDeepStandby
//};
//
//// AGC Processing
//enum eAGCProcessingValue384
//{
//    eNoAGC =0,
//    eAGCEqHisto,
//    eAGCEnhanced,
//    eAGCLinear,
//    eAGCTotal
//};
//
//// Enhancement processing (bitfield)
//enum eEnhancementProcessingValue384
//{
//    eNoEnhancement =0,
//    eColumnCorrection = 1,  // b0
//    eLineCorrection = 2,    // b1
//    eDenoising=4            // b2
//    //
//};
//
//
//
#endif // DALProxy384LUSBDEF_H
