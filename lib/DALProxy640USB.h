#ifndef __DALProxy640USB_H__
#define __DALProxy640USB_H__

#if (defined (LINUX) || defined (__linux__))
    typedef void *HANDLE;
#else
    #include <Windows.h>
#endif

#ifndef DOXYGEN
#if (defined (LINUX) || defined (__linux__))
    #ifdef DALProxy640USB_LIBRARY
    #define DALProxy640USB_API __attribute__((visibility("default")))
    #else
    #define DALProxy640USB_API
    #endif
#else
    #ifdef DALProxy640USB_LIBRARY
    #define DALProxy640USB_API __declspec(dllexport)
    #else
    #define DALProxy640USB_API __declspec(dllimport)
    #endif
#endif
#endif

#ifdef __cplusplus
extern "C" { /* using a C++ compiler */
#endif

#include <stdio.h>

/** \defgroup Module_Management SmartIR640 Management
  * \brief Etablish and manage communication with SmartIR640.
  */

/** \defgroup Module_Processing SmartIR640 Processing
  * \brief Control SmartIR640 image processing.
  * Query module connected to workstation, Open and close link.
  */

/** \defgroup Module_Control SmartIR640 Control
  * \brief Set or Get module features. Refer to module user guide for details on feature, and SDK header file for paeFeature definition.
  */

/** \defgroup Module_Image SmartIR640 Image
  * \brief Query Image from SmartIR640.
  */

/** \defgroup Module_Storage SmartIR640 Storage
  * \brief Store and retrieve processing settings into SmartIR640.
  */

/** \defgroup Module_Calibration SmartIR640 Calibration
  * \brief SmartIR640 NUC, bad pixel and Shutterless correction calibration. Refer to calibration example provided with the SDK for detailed usage of the following function.
  *
  * NUC can be a two points calibration, or a one point calibration.
  * Shutterless can be a T0 calibration only or T0 and T1 calibration.
  */

/** \defgroup Module_ErrFunc Function return code
 * \brief Function execution returned code.
  */

/** \addtogroup Module_ErrFunc
  * \ref eDALProxy640USBErr is return by most functions as a result of execution.
  * \see eDALProxy640USBErr() to convert code to user friendly string.
  *  @{ */

/** Code return by most functions about execution. */
DALProxy640USB_API typedef enum
{
    eProxy640USBSuccess=0, /**< Function call success. */
    eProxy640USBParameterError, /**< Function call with wrong parameter. */
    eProxy640USBHandleError, /**< Function call with wrong or invalid SmartIR640 handle. */
    eProxy640USBInitFailed,  /**< Internal error occur. */
    eProxy640USBOpenFailed,  /**< Open connection to SmartIR640 failed. Maybe already connected */
    eProxy640USBCommFailed,  /**< Exchange with SmartIR640 failed. */
    eProxy640USBTimeout, /**< Operation on SmartIR640 timeout before completed. */
    eProxy640USBSyncBroken, /**< GetImage(), Sync with SmartIR640 broken. */
    //
    eProxy640USBSequencingError, /**< Function call outside correct sequencing */
    eProxy640USBFeatureNotAvailable, /**< Feature not available on this module or can't be use due to present configuration. */
    eProxy640USBBistInitFailure, /**< Built-In Self Test initialisation failed. */
    eProxy640USBBistFailure, /**< SmartIR640 reported a Built-In Self Test error. */
    eProxy640USBFormatFailed,  /**< Incompatible file format for SmartIR640. */
    //
    eProxy640USBErrTotal
} eDALProxy640USBErr;


/** Convert \ref eDALProxy640USBErr to user message.
 * \param[in] paeError Function returns error code.
 * \return User error message from eDALProxy640USBErr.
 * \note String is C-Style, i.e. Ascii with null terminate byte.
*/
DALProxy640USB_API const char* Proxy640USB_GetErrorString(eDALProxy640USBErr paeError);

/** @} */


/** \addtogroup Module_Management
 *
 * This set provides :
 * \li Functions to enumerates and name plugged SmartIR640.
 * \li Function to connect and disconnect to SmartIR640.
 *
 * Application call Proxy640USB_GetModuleCount() to know how many SmartIR640 are plugged to workstation.
 * First SmartIR640 index is 0, and so on.
 *
 * Calling Proxy640USB_GetModuleCount() check SmartIR640 count. So, call it will refresh SmartIR640 list.
 *
 * Before calling any other function's group, Application must connect to a SmartIR640 using Proxy640USB_ConnectToModule().<br>
 * Once a SmartIR640 is connected by an application, it's not available to another application.
 * Application must release SmartIR640 by calling Proxy640USB_DisconnectFromModule().
 *
 * Connection to SmartIR640 will provide a \em handle. This \em handle is use by all functions addressing this SmartIR640.
 * It remains valid until Proxy640USB_DisconnectFromModule() is called.
 *
 * Application can connect several SmartIR640, using different \em handles.
* @{ */

/** Retrieve current count of plugged module.
 * \param[out] paiCount Number of plugged module.
*/
DALProxy640USB_API eDALProxy640USBErr Proxy640USB_GetModuleCount(int* paiCount);

/** Query SmartIR640 name by index.
 * \param[in] iIdx Module index.
 * \param[out] paName SmartIR640 name from index.
 * \param[in] iLen paName storage size.
*/
DALProxy640USB_API eDALProxy640USBErr Proxy640USB_GetModuleName(int iIdx, char* paName, int iLen);

/** Connect to SmartIR640 by index.<br>
 * This function will return a handle, which will be uses as SmartIR640 identifier.<br>
 * Connection may failed if SmartIR640 is already connected by another application.
 * \param[in] iIdx Module index. First SmartIR640 index is 0.
 * \param[out] paHandle SmartIR640 handle.
 **/
DALProxy640USB_API eDALProxy640USBErr Proxy640USB_ConnectToModule(int iIdx, HANDLE* paHandle);

/** Check if handle connection.
 * This function will check if handle is still valid, and then check connection with SmartIR640.
 * \param[in] paHandle SmartIR640 handle.
 *\return \ref eProxy640USBSuccess on success, or error code.
 **/
DALProxy640USB_API eDALProxy640USBErr Proxy640USB_IsConnectToModule(HANDLE paHandle);

/** Disconnect to SmartIR640 by index.
 * This function will release SmartIR640 connection.
 * \param[in] paHandle SmartIR640 handle.
 **/
DALProxy640USB_API eDALProxy640USBErr Proxy640USB_DisconnectFromModule(HANDLE paHandle);

/** Run the SmartIR640 Built-In Self-Test.
 * This function will check if handle is still valid, and then run the built-in self tests.
 * \param[in] paHandle SmartIR640 handle.
 * \param[out] diagCode Diagnostic code provided by the SmartIR640. Value is 0 in case of success.
 * \return \ref eProxy640USBSuccess on success, or error code.
 **/
DALProxy640USB_API eDALProxy640USBErr Proxy640USB_RunBIST(HANDLE paHandle, unsigned int *diagCode);

//! @}


/** \addtogroup Module_Image
 *
 * This set provides a single function to query current SmartIR640 image.
 * Calling it will block application until an image is available, or timeout occurs.
 *
 * Application may provide image storage for new IR image. Image nature (Raw or Fixed) depend on
 * processing settings (see \ref Module_Processing).
 *
 * IR image is <b>640 width by 480 heigth</b>. Pixel storage is unsigned short, with 16bit effective, LSB aligned.
 *
 * Along IR Image, some meta data are provides.
 *
 * @{ */
/** Query image from SmartIR640.
 * \param[in] paHandle SmartIR640 handle.
 * \param[out] paImage Image placeholder for new image. Must be at least <em>640 x 480 x 2= 600KB</em>.
 * \param[out] paMeta Meta-Data placeholder. Must be at least 135 32bit values :
 * \li [0] fpa temperature in celsius (cast float to get it).
 * \li [1] period from previous image (in microsecond).
 * \li [2] frame counter (16bit effective).
 * \li [3-4] time from epoch, using 64bit (use 2 values).
 * \li [5-6] Reserved.
 * \li [7-134] Histogram.
 * \param[in] paiTimeout Operation timeout in millisecond.
 *
*/
DALProxy640USB_API eDALProxy640USBErr Proxy640USB_GetImage(HANDLE paHandle, unsigned short* paImage, int* paMeta, int paiTimeout);

//! @}


/** \addtogroup Module_Control
 *
 *
 * @{ */

/** Query string feature.
 * \param[in] paHandle SmartIR640 handle.
 * \param[in] paeFeature Feature requested.
 * \param[out] paStr String from requested feature.
 * \warning String Feature are 32 byte large, including null byte. Ensure paStr is large enougt.
*/
DALProxy640USB_API eDALProxy640USBErr Proxy640USB_GetStringFeature(HANDLE paHandle, int paeFeature, char* paStr);

/** Query integer feature.
 * \param[in] paHandle SmartIR640 handle.
 * \param[in] paeFeature Feature requested.
 * \param[out] paUInt Integer value from requested feature.
*/
DALProxy640USB_API eDALProxy640USBErr Proxy640USB_GetUIntFeature(HANDLE paHandle, int paeFeature, unsigned int* paUInt);

/** Query float feature.
 * \param[in] paHandle SmartIR640 handle.
 * \param[in] paeFeature Feature requested.
 * \param[out] paFloat Float value from requested feature.
*/
DALProxy640USB_API eDALProxy640USBErr Proxy640USB_GetFloatFeature(HANDLE paHandle, int paeFeature, float* paFloat);


/** Set string feature.
 * \param[in] paHandle SmartIR640 handle.
 * \param[in] paeFeature Feature written.
 * \param[in] paStr String for written feature.
 * \warning String Feature are 32 byte large, including null byte. Ensure paStr is large enougt.
*/
DALProxy640USB_API eDALProxy640USBErr Proxy640USB_SetStringFeature(HANDLE paHandle, int paeFeature, const char* paStr);

/** Set integer feature.
 * \param[in] paHandle SmartIR640 handle.
 * \param[in] paeFeature Feature written.
 * \param[in] paUInt Integer value for written feature.
*/
DALProxy640USB_API eDALProxy640USBErr Proxy640USB_SetUIntFeature(HANDLE paHandle, int paeFeature, unsigned int paUInt);

/** Query float feature.
 * \param[in] paHandle SmartIR640 handle.
 * \param[in] paeFeature Feature written.
 * \param[in] paFloat Float value for written feature.
*/
DALProxy640USB_API eDALProxy640USBErr Proxy640USB_SetFloatFeature(HANDLE paHandle, int paeFeature, float paFloat);

//! @}

/** \addtogroup Module_Processing
 *
 * This set of function provides control over image processing.
 * \li Query and change processing step state (enable or disable).
 * \li Query processing parameters.
 * \li Set processing parameters.
 *
 * Processing is compose of :
 * \li Bad pixel correction.
 * \li Non linearity correction.
 *
 * \see <em>User's Guide</em> or \ref ProcessingChainPage for details
 *
 * @{ */

/** Configure Internal Calibration.
 * \param[in] paHandle SmartIR640 handle.
 * \param[in] paParam configuration to apply.
 * \li bit[0] Enable (1) or Disable (0) the automatic fast calibration associated with mechanical shutter
 * \li bit[1-31] Reserved.
*/
DALProxy640USB_API eDALProxy640USBErr Proxy640USB_SetCalibrationConfig(HANDLE paHandle, int paParam);

/** Enable/Disable NUC processing steps. These are enabled by default at connection.
 * \param[in] paHandle SmartIR640 handle.
 * \param[in] paBadPixels Enable(1)/Disable(0) bad pixels correction.
 * \param[in] paNUC Enable(1)/Disable(0) Non Uniformity Correction.
 * \return This return error eProxy640USBFeatureNotAvailable if Shutterless is activated.
*/
DALProxy640USB_API eDALProxy640USBErr Proxy640USB_SetNUCProcessing(HANDLE paHandle, unsigned char paBadPixels, unsigned char paNUC);

/** Query NUC processing steps status.
 * \param[in] paHandle SmartIR640 handle.
 * \param[out] paBadPixels bad pixels correction enable(1) or disable(0).
 * \param[out] paNUC Non Uniformity Correction enable(1) or disable(0).
 * \return This return error eProxy640USBFeatureNotAvailable if Shutterless is activated.
*/
DALProxy640USB_API eDALProxy640USBErr Proxy640USB_GetNUCProcessing(HANDLE paHandle, unsigned char* paBadPixels, unsigned char* paNUC);


/** Enable/Disable ShutterLess processing.
 * \param[in] paHandle SmartIR640 handle.
 * \param[in] bActivate Enable(true)/Disable(false) Shutterless processing.
 * \return This function return eProxy640USBFeatureNotAvailable error if Shutterless is unavailable on this module.
*/
DALProxy640USB_API eDALProxy640USBErr Proxy640USB_SetShutterLessProcessing(HANDLE paHandle, bool bActivate);

/** Query Shutterless processing status.
 * \param[in] paHandle SmartIR640 handle.
 * \param[out] pbIsActive shutterless processing enable(true) or disable(false).
*/
DALProxy640USB_API eDALProxy640USBErr Proxy640USB_GetShutterLessProcessing(HANDLE paHandle, bool *pbIsActive);

/** Set Auto Gain Control processing step. By default, No AGC processing set.
 * \param[in] paHandle SmartIR640 handle.
 * \param[in] paeAGCProcessing see eAGCProcessingValue for values.
*/
DALProxy640USB_API eDALProxy640USBErr Proxy640USB_SetAGCProcessing(HANDLE paHandle, unsigned char paeAGCProcessing);

/** Query processing steps status.
 * \param[in] paHandle SmartIR640 handle.
 * \param[out] paeAGCProcessing see eAGCProcessingValue for values.
*/
DALProxy640USB_API eDALProxy640USBErr Proxy640USB_GetAGCProcessing(HANDLE paHandle, unsigned char* paeAGCProcessing);

/** Set Gains values for NUC processing.
 * \param[in] paHandle SmartIR640 handle.
 * \param[in] paTableGains New Gains values for NUC processing.
 * \note Each pixel must have a value. So paTableGains must contains 640 * 640 float values (4 bytes float).
*/
DALProxy640USB_API eDALProxy640USBErr Proxy640USB_SetCurrentTableGain(HANDLE paHandle, float* paTableGains);

/** Set Offset values for NUC processing.
 * \param[in] paHandle SmartIR640 handle.
 * \param[in] paTableOffsets New offsets values for NUC processing.
 * \note Each pixel must have a value. So paTableGains must contains 640 * 480 values (2 bytes signed value).
*/
DALProxy640USB_API eDALProxy640USBErr Proxy640USB_SetCurrentTableOffset(HANDLE paHandle, signed short* paTableOffsets);

/** Set bad pixels position in image for bad pixels correction.
 * \param[in] paHandle SmartIR640 handle.
 * \param[in] paTableX,paTableY Bad pixels position in image.
 * \param[in] paCount bad pixels count.
*/
DALProxy640USB_API eDALProxy640USBErr Proxy640USB_SetCurrentBadPixels(HANDLE paHandle, unsigned short *paTableX, unsigned short *paTableY,
                                             unsigned short paCount );

/** Set Shutterless data for restore purpose. Shutterless data must be considered as binary and must not be modified.
 * \param[in] paHandle SmartIR640 handle.
 * \param[in] paShutterless values.
*/
DALProxy640USB_API eDALProxy640USBErr Proxy640USB_SetCurrentShutterless(HANDLE paHandle, unsigned int* paShutterless);

/** Get Shutterless size of the data for backup and restore purpose.
 * \param[in] paHandle SmartIR640 handle.
 * \param[out] pSize size of shutterless data in bytes.
*/
DALProxy640USB_API eDALProxy640USBErr Proxy640USB_GetCurrentShutterlessSize(HANDLE paHandle, unsigned int* pSize);

/** Get Shutterless data for backup purpose. Shutterless data must be considered as binary and must not be modified.
 * \param[in] paHandle SmartIR640 handle.
 * \param[in] paShutterless New Shutterless values.
*/
DALProxy640USB_API eDALProxy640USBErr Proxy640USB_GetCurrentShutterless(HANDLE paHandle, unsigned int* paShutterless);

/** Get Gains current values from NUC processing.
 * \param[in] paHandle SmartIR640 handle.
 * \param[in] paTableGains New Gains values for NUC processing.
 * \note Each pixel must have a value. So paTableGains must contains 640 * 480 float values (4 bytes float).
*/
DALProxy640USB_API eDALProxy640USBErr Proxy640USB_GetCurrentTableGain(HANDLE paHandle, float* paTableGains);

/** Get Offset current values from NUC processing.
 * \param[in] paHandle SmartIR640 handle.
 * \param[in] paTableOffsets New offsets values for NUC processing.
 * \note Each pixel must have a value. So  paTableGains must contains 640 * 480 values (2 bytes signed value).
*/
DALProxy640USB_API eDALProxy640USBErr Proxy640USB_GetCurrentTableOffset(HANDLE paHandle, signed short* paTableOffsets);

/** Get current bad pixels position in image from bad pixels correction.
 * \param[in] paHandle SmartIR640 handle.
 * \param[in] paTableX,paTableY Bad pixels position in image.
 * \param paCount Initial bad pixels array size, on return, bad pixel count.
 * \note paCount must be init with  paTableX / paTableY placeholder size (to avoid overflow),
 * and will be modified by function with current bad pixel count.
*/
DALProxy640USB_API eDALProxy640USBErr Proxy640USB_GetCurrentBadPixels(HANDLE paHandle, unsigned short *paTableX, unsigned short *paTableY,
                                             unsigned short *paCount );

//! @}

/** \addtogroup Module_Storage
 *  SmartIR640 provides \b 8 slots to store Gain or Offset value. Slot are not dedicated to a kind of data.
 *  \attention Storage space is limited into SmartIR640. Hence, data (Gain or Offset) are rounded to fit into slot.
 *  This may involve difference if your store data, read it, and compare to your initial values.
 *  For coherence, this data reduction is also apply when update NUC processing data (see \ref Module_Processing).
 *
 *  Save functions provides a \em MakeDefault parameter. When set to 1 (enable), this will mark slot as default.
 * When application connect to SmartIR640, Proxy640USB_ConnectToModule() function will look for default slot,
 * and load into processing data from slot.
 @{ */

/** Default slot index for Gain values and Offset values, last setting's bank used.
 * \param[in] paHandle SmartIR640 handle.
 * \param[out] paiIdxGains Gain slot index, of 255 if no default Gain slot index.
 * \param[out] paiIdxOffsets Offset slot index, of 255 if no default Offset slot index.
 * \param[out] paiIdxBank Settings bank index, of 255 if no default settings index.
 * \note No need to call and use this function (already done at SmartIR640 connection)
 **/
DALProxy640USB_API eDALProxy640USBErr Proxy640USB_StartupDefault(HANDLE paHandle, unsigned char *paiIdxGains, unsigned char* paiIdxOffsets, unsigned char *paiIdxBank);

/** Query slot data type.
 * \param[in] paHandle SmartIR640 handle.
 * \param[in] paiIndex Slot index to query.
 * \param[out] paeType Slot type.
 * \param[out] paData Table associate data. NULL, or 60 bytes placeholder. paData is additional data associated to Gain or Offset array, which can be used freely by application for instance to keep a trace of Gain or Offset table calibration conditions, either sensitivity, either focal plane array temperature.
 * Slot type value are :
 *\li 0 :Empty slot.
 *\li 1 :Gain values.
 *\li 2 :Offset values.
 **/
DALProxy640USB_API eDALProxy640USBErr Proxy640USB_SlotType(HANDLE paHandle, unsigned char paiIndex, unsigned char* paeType,
                                                           void* paData);


/** Retrieve SmartIR640 slot data as Gain values.<br>
 * This function may failed if slot is empty, or slot data are not Gain values.
 * \param[in] paHandle SmartIR640 handle.
 * \param[in] paiIndex Slot index as data.
 * \param[out] paTableGain Gain values from SmartIR640 slot.
 * \param[out] paData Table associate data. NULL, or 60 bytes placeholder. paData is additional data associated to Gain or Offset array, which can be used freely by application for instance to keep a trace of Gain or Offset table calibration conditions, either sensitivity, either focal plane array temperature.
 */
DALProxy640USB_API eDALProxy640USBErr Proxy640USB_LoadTableGain(HANDLE paHandle, unsigned char paiIndex, float* paTableGain,
                                                                void* paData);

/** Retrieve SmartIR640 slot data as Offset values.<br>
 * This function may failed if slot is empty, or slot data are not Offset values.
 * \param[in] paHandle SmartIR640 handle.
 * \param[in] paiIndex Slot index as data.
 * \param[out] paTableOffset Offset values from SmartIR640 slot.
 * \param[out] paData Table associate data. NULL, or 60 bytes placeholder. paData is additional data associated to Gain or Offset array, which can be used freely by application for instance to keep a trace of Gain or Offset table calibration conditions, either sensitivity, either focal plane array temperature.
 */
DALProxy640USB_API eDALProxy640USBErr Proxy640USB_LoadTableOffset(HANDLE paHandle, unsigned char paiIndex, short* paTableOffset,
                                                                  void* paData);


/** Retrieve bad pixel position in image from SmartIR640.
 * \param[in] paHandle SmartIR640 handle.
 * \param[out] paTableX,paTableY Bad pixels position in image.
 * \param paCount Initial bad pixels array size, on return, bad pixel count.
 * \note paCount must be init with paTableX / paTableY placeholder size (to avoid overflow),
 * and will be modified by function with current bad pixel count.
*/
DALProxy640USB_API eDALProxy640USBErr Proxy640USB_LoadBadPixels(HANDLE paHandle,
                                                                unsigned short *paTableX, unsigned short *paTableY,
                                                                unsigned short* paCount);


/** Save Gain values into SmartIR640 slot data.
 * \param[in] paHandle SmartIR640 handle.
 * \param[in] paiIndex Slot index.
 * \param[in] paTableGain Gain values to store into SmartIR640 slot.
 * \param[in] paData Table associate data. NULL, or 60 bytes placeholder. paData is additional data associated to Gain or Offset array, which can be used freely by application for instance to keep a trace of Gain or Offset table calibration conditions, either sensitivity, either focal plane array temperature.
 */
DALProxy640USB_API eDALProxy640USBErr Proxy640USB_SaveTableGain(HANDLE paHandle, unsigned char paiIndex, const float* paTableGain,
                                              void* paData);

/** Save Offset values into SmartIR640 slot data.
 * \param[in] paHandle SmartIR640 handle.
 * \param[in] paiIndex Slot index.
 * \param[in] paTableOffset Offset values to store into SmartIR640 slot.
 * \param[in] paData Table associate data. NULL, or 60 bytes placeholder. paData is additional data associated to Gain or Offset array, which can be used freely by application for instance to keep a trace of Gain or Offset table calibration conditions, either sensitivity, either focal plane array temperature.
 */
DALProxy640USB_API eDALProxy640USBErr Proxy640USB_SaveTableOffset(HANDLE paHandle, unsigned char paiIndex, const short* paTableOffset,
                                                                  void* paData);

/** Save bad pixel position into SmartIR640 slot data.
 * \param[in] paHandle SmartIR640 handle.
 * \param[in] paTableX,paTableY Bad pixels position in image.
 * \param[in] paCount bad pixels count.
*/
DALProxy640USB_API eDALProxy640USBErr Proxy640USB_SaveBadPixels(HANDLE paHandle, const unsigned short *paTableX, const unsigned short* paTableY, unsigned short paCount);


/** Use SmartIR640 slot data as Gain values for NUC processing,
 * i.e. retrieve it from SmartIR640, and set it to NUC processing.
 * \param[in] paHandle SmartIR640 handle.
 * \param[in] paiIndex Slot index as data.
 */
DALProxy640USB_API eDALProxy640USBErr Proxy640USB_LoadCurrentTableGain(HANDLE paHandle, unsigned char paiIndex);

/** Use SmartIR640 slot data as Offset values for NUC processing,
 * i.e. retrieve it from SmartIR640, and set it to NUC processing.
 * \param[in] paHandle SmartIR640 handle.
 * \param[in] paiIndex Slot index as data.
 */
DALProxy640USB_API eDALProxy640USBErr Proxy640USB_LoadCurrentTableOffset(HANDLE paHandle, unsigned char paiIndex);

/** Use SmartIR640 stored bad pixel for bad pixel correction.
 * \param[in] paHandle SmartIR640 handle.
 */
DALProxy640USB_API eDALProxy640USBErr Proxy640USB_LoadCurrentBadPixels(HANDLE paHandle);

/** Save current Gain values into SmartIR640 slot data.
 * \param[in] paHandle SmartIR640 handle.
 * \param[in] paiIndex Slot index.
 * \param[in] paData Table associate data. NULL, or 60 bytes placeholder. paData is additional data associated to Gain or Offset array, which can be used freely by application for instance to keep a trace of Gain or Offset table calibration conditions, either sensitivity, either focal plane array temperature.
 */
DALProxy640USB_API eDALProxy640USBErr Proxy640USB_SaveCurrentTableGain(HANDLE paHandle, unsigned char paiIndex, const void *paData);

/** Save Offset values into SmartIR640 slot data.
 * \param[in] paHandle SmartIR640 handle.
 * \param[in] paiIndex Slot index.
 * \param[in] paData Table associate data. NULL, or 60 bytes placeholder. paData is additional data associated to Gain or Offset array, which can be used freely by application for instance to keep a trace of Gain or Offset table calibration conditions, either sensitivity, either focal plane array temperature.
 */
DALProxy640USB_API eDALProxy640USBErr Proxy640USB_SaveCurrentTableOffset(HANDLE paHandle, unsigned char paiIndex, const void *paData);

/** Save bad pixel position into SmartIR640 slot data.
 * \param[in] paHandle SmartIR640 handle.
*/
DALProxy640USB_API eDALProxy640USBErr Proxy640USB_SaveCurrentBadPixels(HANDLE paHandle);

/** Save Shutterless Tables into SmartIR640.
 * \param[in] paHandle SmartIR640 handle.
 * \return eProxy640USBFeatureNotAvailable error if Shutterless is unavailable on the module.
*/
DALProxy640USB_API eDALProxy640USBErr Proxy640USB_SaveCurrentShutterlessTables(HANDLE paHandle);

/** Load Shutterless Tables from SmartIR640 in order to use it with shutterless processing
 * \param[in] paHandle SmartIR640 handle.
 * \return eProxy640USBFeatureNotAvailable error if Shutterless is unavailable on the module.
*/
DALProxy640USB_API eDALProxy640USBErr Proxy640USB_LoadCurrentShutterlessTables(HANDLE paHandle);



//! @}


/** \addtogroup Module_Calibration
 * This set of function provide 2 kind of NUC calibrations :
 * \li Full Calibration.
 * \li Fast Calibration.
 *
 * And Shutterless Calibration compose of :
 * \li T0 Calibration.
 * \li T1 Calibration.
 *
 * \see <em>User's Guide</em> or \ref CalibrationProcessPage for details.
* @{ */


/** Abort a Calibration process and reset the sequencing.
 * None of the corrections table will be change by the abort calibration.
 *
 * \param[in] paHandle SmartIR640 Handle.
 */
DALProxy640USB_API eDALProxy640USBErr Proxy640USB_AbortCalibration(HANDLE paHandle);

/** Prepare NUC Calibration engine.
 * \param[in] paHandle SmartIR640 Handle.
 * \param[in] iStage Stage of the calibration (1 or 2).
 * \return eProxy640USBSequencingError see \ref CalibrationProcessPage for details.
*/
DALProxy640USB_API eDALProxy640USBErr Proxy640USB_InitShutter2PtsCalibration(HANDLE paHandle, unsigned int iStage);

/** Add image for Shutter 2pts calibration.
  *
  * Low temperature image for iStage = 1, High temperature image for iStage = 2.
  *
  * \param[in] paHandle SmartIR640 Handle.
  * \param[in] iStage Stage of calibration (1 (low) or 2 (high)).
  * \return eProxy640USBSequencingError see \ref CalibrationProcessPage for details.
  */
DALProxy640USB_API eDALProxy640USBErr Proxy640USB_StepShutter2PtsCalibration(HANDLE paHandle, unsigned int iStage);

/** Perform two points calibration using low and high temperature images.
 *
 * Once calibration is done, new Gain, Offset and bad pixel are set to current NUC and BPC processing.
 *
 * \param[in] paHandle SmartIR640 handle.
 * \param[in] iStage Stage of the calibration. If stage = 2, perform the final step of calibration. Once is done, new Gain, Offset
 * and bad pixel are set to current NUC and BPC processing.
 * \return eProxy640USBSequencingError see \ref CalibrationProcessPage for details.
 */
DALProxy640USB_API eDALProxy640USBErr Proxy640USB_FinishShutter2PtsCalibration(HANDLE paHandle,unsigned int iStage);

/** Prepare Shutter Calibration engine, also called one point calibration.
 *
 * This calibration will only produce new Offset values.
 *
 * \param[in] paHandle SmartIR640 handle.
 * \return eProxy640USBSequencingError see \ref CalibrationProcessPage for details.
*/
DALProxy640USB_API eDALProxy640USBErr Proxy640USB_InitShutterCalibration(HANDLE paHandle);

/** Add image to prepare Shutter Calibration
 *
 * \param[in] paHandle SmartIR640 handle.
 * \return eProxy640USBSequencingError see \ref CalibrationProcessPage for details.
 */
DALProxy640USB_API eDALProxy640USBErr Proxy640USB_StepShutterCalibration(HANDLE paHandle);

/** Perform Shutter calibration.
 *
 * Once calibration is done, Offset values are set to current NUC processing.
 *
 * \param[in] paHandle SmartIR640 handle.
 * \return eProxy640USBSequencingError see \ref CalibrationProcessPage for details.
 */
DALProxy640USB_API eDALProxy640USBErr Proxy640USB_FinishShutterCalibration(HANDLE paHandle);



/** Initialise Stage for Shutterless Calibration T0
 *
 * Must be use on correct sequencing with SmartIR640 shutterless module.
 *
 * \param[in] paHandle SmartIR640 handle.
 * \param[in] iStage Stage number of calibration.
 * \return eProxy640USBSequencingError see \ref CalibrationProcessPage for details.
 */
DALProxy640USB_API eDALProxy640USBErr Proxy640USB_InitSLCalibrationT0(HANDLE paHandle, unsigned int iStage);

/** Add image for Shutterless Calibration T0
 *
 * Must be used on correct sequencing with SmartIR640 shutterless module.
 *
 * \param[in] paHandle SmartIR640 handle.
 * \param[in] iStage Stage number of calibration (1 = low temperature, 2 = high temperature).
 * \return eProxy640USBSequencingError see \ref CalibrationProcessPage for details.
 */
DALProxy640USB_API eDALProxy640USBErr Proxy640USB_StepSLCalibrationT0(HANDLE paHandle, unsigned int iStage);

/** Perform Shutterless T0 calibration
 *
 * Must be used on correct sequencing with SmartIR640 shutterless module.
 * Once calibration is done, new Shutterless tables are set to current Shutterless processing.
 *
 * \param[in] paHandle SmartIR640 handle.
 * \param[in] iStage Stage number of calibration (1 = low temperature, 2 = high temperature).
 * \return eProxy640USBSequencingError see \ref CalibrationProcessPage for details.
 */
DALProxy640USB_API eDALProxy640USBErr Proxy640USB_FinishSLCalibrationT0(HANDLE paHandle, unsigned int iStage);



/** Initialise Shutterless Calibration T1
 *
 * Must be use on correct sequencing with SmartIR640 shutterless module.
 *
 * \param[in] paHandle SmartIR640 handle.
 * \return eProxy640USBSequencingError see \ref CalibrationProcessPage for details.
 */
DALProxy640USB_API eDALProxy640USBErr Proxy640USB_InitSLCalibrationT1(HANDLE paHandle);

/** Add image for Shutterless Calibration T1
 *
 * Must be use on correct sequencing with SmartIR640 shutterless module.
 *
 * \param[in] paHandle SmartIR640 handle.
 * \return eProxy640USBSequencingError see \ref CalibrationProcessPage for details.
 */
DALProxy640USB_API eDALProxy640USBErr Proxy640USB_StepSLCalibrationT1(HANDLE paHandle);

/** Perform Shutterless Calibration T1
 *
 * Must be use on correct sequencing with SmartIR640 shutterless module.
 * Once Calibration is done, new Shutterless T1 tables are set for current Shutterless processing.
 *
 * \param[in] paHandle SmartIR640 handle.
 * \return eProxy640USBSequencingError see \ref CalibrationProcessPage for details.
 */
DALProxy640USB_API eDALProxy640USBErr Proxy640USB_FinishSLCalibrationT1(HANDLE paHandle);

//! @}

#ifdef __cplusplus
   }
#endif

#endif // __DALProxy640USB_H__
