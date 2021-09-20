#ifndef __DALProxy384LUSB_H__
#define __DALProxy384LUSB_H__

#if (defined (LINUX) || defined (__linux__))
    typedef void *HANDLE;
#else
    #include <Windows.h>
#endif

#ifndef DOXYGEN
#if (defined (LINUX) || defined (__linux__))
    #ifdef DALProxy384LUSB_LIBRARY
    #define DALProxy384LUSB_API __attribute__((visibility("default")))
    #else
    #define DALProxy384LUSB_API
    #endif
#else
    #ifdef DALProxy384LUSB_LIBRARY
    #define DALProxy384LUSB_API __declspec(dllexport)
    #else
    #define DALProxy384LUSB_API __declspec(dllimport)
    #endif
#endif
#endif

#ifdef __cplusplus
extern "C" { /* using a C++ compiler */
#endif

#include <stdio.h>

/** \defgroup Module_Management SmartIR384L Management
  * \brief Etablish and manage communication with SmartIR384L.
  */

/** \defgroup Module_Processing SmartIR384L Processing
  * \brief Control SmartIR384L image processing.
  * Query module connected to workstation, Open and close link.
  */

/** \defgroup Module_Control SmartIR384L Control
  * \brief Set or Get module features. Refer to module user guide for details on feature, and SDK header file for paeFeature definition.
  */

/** \defgroup Module_Image SmartIR384L Image
  * \brief Query Image from SmartIR384L.
  */

/** \defgroup Module_Storage SmartIR384L Storage
  * \brief Store and retrieve processing settings into SmartIR384L.
  */

/** \defgroup Module_Calibration SmartIR384L Calibration
  * \brief SmartIR384L NUC, bad pixel and Shutterless correction calibration. Refer to calibration example provided with the SDK for detailed usage of the following function.
  *
  * NUC can be a two points calibration, or a one point calibration.
  * Shutterless can be a T0 calibration only or T0 and T1 calibration.
  */

/** \defgroup Module_ErrFunc Function return code
 * \brief Function execution returned code.
  */

/** \addtogroup Module_ErrFunc
  * \ref eDALProxy384LUSBErr is return by most functions as a result of execution.
  * \see eDALProxy384LUSBErr() to convert code to user friendly string.
  *  @{ */

/** Code return by most functions about execution. */
DALProxy384LUSB_API typedef enum
{
    eProxy384LUSBSuccess=0, /**< Function call success. */
    eProxy384LUSBParameterError, /**< Function call with wrong parameter. */
    eProxy384LUSBHandleError, /**< Function call with wrong or invalid SmartIR384L handle. */
    eProxy384LUSBInitFailed,  /**< Internal error occur. */
    eProxy384LUSBOpenFailed,  /**< Open connection to SmartIR384L failed. Maybe already connected */
    eProxy384LUSBCommFailed,  /**< Exchange with SmartIR384L failed. */
    eProxy384LUSBTimeout, /**< Operation on SmartIR384L timeout before completed. */
    eProxy384LUSBSyncBroken, /**< GetImage(), Sync with SmartIR384L broken. */
    //
    eProxy384LUSBSequencingError, /**< Function call outside correct sequencing */
    eProxy384LUSBFeatureNotAvailable, /**< Feature not available on this module or can't be use due to present configuration. */
    eProxy384LUSBBistInitFailure, /**< Built-In Self Test initialisation failed. */
    eProxy384LUSBBistFailure, /**< SmartIR384L reported a Built-In Self Test error. */
    eProxy384LUSBFormatFailed,  /**< Incompatible file format. */
    //
    eProxy384LUSBErrTotal
} eDALProxy384LUSBErr;


/** Convert \ref eDALProxy384LUSBErr to user message.
 * \param[in] paeError Function returns error code.
 * \return User error message from eDALProxy384LUSBErr.
 * \note String is C-Style, i.e. Ascii with null terminate byte.
*/
DALProxy384LUSB_API const char* Proxy384LUSB_GetErrorString(eDALProxy384LUSBErr paeError);

/** @} */


/** \addtogroup Module_Management
 *
 * This set provides :
 * \li Functions to enumerates and name plugged SmartIR384L.
 * \li Function to connect and disconnect to SmartIR384L.
 *
 * Application call Proxy384LUSB_GetModuleCount() to know how many SmartIR384L are plugged to workstation.
 * First SmartIR384L index is 0, and so on.
 *
 * Calling Proxy384LUSB_GetModuleCount() check SmartIR384L count. So, call it will refresh SmartIR384L list.
 *
 * Before calling any other function's group, Application must connect to a SmartIR384L using Proxy384LUSB_ConnectToModule().<br>
 * Once a SmartIR384L is connected by an application, it's not available to another application.
 * Application must release SmartIR384L by calling Proxy384LUSB_DisconnectFromModule().
 *
 * Connection to SmartIR384L will provide a \em handle. This \em handle is use by all functions addressing this SmartIR384L.
 * It remains valid until Proxy384LUSB_DisconnectFromModule() is called.
 *
 * Application can connect several SmartIR384L, using different \em handles.
* @{ */

/** Retrieve current count of plugged module.
 * \param[out] paiCount Number of plugged module.
*/
DALProxy384LUSB_API eDALProxy384LUSBErr Proxy384LUSB_GetModuleCount(int* paiCount);

/** Query SmartIR384L name by index.
 * \param[in] iIdx Module index.
 * \param[out] paName SmartIR384L name from index.
 * \param[in] iLen paName storage size.
*/
DALProxy384LUSB_API eDALProxy384LUSBErr Proxy384LUSB_GetModuleName(int iIdx, char* paName, int iLen);

/** Connect to SmartIR384L by index.<br>
 * This function will return a handle, which will be uses as SmartIR384L identifier.<br>
 * Connection may failed if SmartIR384L is already connected by another application.
 * \param[in] iIdx Module index. First SmartIR384L index is 0.
 * \param[out] paHandle SmartIR384L handle.
 **/
DALProxy384LUSB_API eDALProxy384LUSBErr Proxy384LUSB_ConnectToModule(int iIdx, HANDLE* paHandle);

/** Check if handle connection.
 * This function will check if handle is still valid, and then check connection with SmartIR384L.
 * \param[in] paHandle SmartIR384L handle.
 *\return \ref eProxy384LUSBSuccess on success, or error code.
 **/
DALProxy384LUSB_API eDALProxy384LUSBErr Proxy384LUSB_IsConnectToModule(HANDLE paHandle);

/** Disconnect to SmartIR384L by index.
 * This function will release SmartIR384L connection.
 * \param[in] paHandle SmartIR384L handle.
 **/
DALProxy384LUSB_API eDALProxy384LUSBErr Proxy384LUSB_DisconnectFromModule(HANDLE paHandle);

/** Run the SmartIR384L Built-In Self-Test.
 * This function will check if handle is still valid, and then run the built-in self tests.
 * \param[in] paHandle SmartIR384L handle.
 * \param[out] diagCode Diagnostic code provided by the SmartIR384L. Value is 0 in case of success.
 * \return \ref eProxy384LUSBSuccess on success, or error code.
 **/
DALProxy384LUSB_API eDALProxy384LUSBErr Proxy384LUSB_RunBIST(HANDLE paHandle, unsigned int *diagCode);


//! @}
#ifndef CONFIG_CUSTOMER
	DALProxy384LUSB_API eDALProxy384LUSBErr Proxy384LUSB_ReadInternalRegister(HANDLE paHandle, int addr, void *paBuffer, int pauSize);
	DALProxy384LUSB_API eDALProxy384LUSBErr Proxy384LUSB_WriteInternalRegister(HANDLE paHandle, int addr, void *paBuffer, int pauSize);
#endif

/** \addtogroup Module_Image
 *
 * This set provides a single function to query current SmartIR384L image.
 * Calling it will block application until an image is available, or timeout occurs.
 *
 * Application may provide image storage for new IR image. Image nature (Raw or Fixed) depend on
 * processing settings (see \ref Module_Processing).
 *
 * IR image is <b>384L width by 288 heigth</b>. Pixel storage is unsigned short, with 16bit effective, LSB aligned.
 *
 * Along IR Image, some meta data are provides.
 *
 * @{ */
/** Query image from SmartIR384L.
 * \param[in] paHandle SmartIR384L handle.
 * \param[out] paImage Image placeholder for new image. Must be at least <em>384L x 288 x 2= 216KB</em>.
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
DALProxy384LUSB_API eDALProxy384LUSBErr Proxy384LUSB_GetImage(HANDLE paHandle, unsigned short* paImage, int* paMeta, int paiTimeout);

//! @}


/** \addtogroup Module_Control
 *
 *
 * @{ */

/** Query string feature.
 * \param[in] paHandle SmartIR384L handle.
 * \param[in] paeFeature Feature requested.
 * \param[out] paStr String from requested feature.
 * \warning String Feature are 32 byte large, including null byte. Ensure paStr is large enougt.
*/
DALProxy384LUSB_API eDALProxy384LUSBErr Proxy384LUSB_GetStringFeature(HANDLE paHandle, int paeFeature, char* paStr);

/** Query integer feature.
 * \param[in] paHandle SmartIR384L handle.
 * \param[in] paeFeature Feature requested.
 * \param[out] paUInt Integer value from requested feature.
*/
DALProxy384LUSB_API eDALProxy384LUSBErr Proxy384LUSB_GetUIntFeature(HANDLE paHandle, int paeFeature, unsigned int* paUInt);

/** Query float feature.
 * \param[in] paHandle SmartIR384L handle.
 * \param[in] paeFeature Feature requested.
 * \param[out] paFloat Float value from requested feature.
*/
DALProxy384LUSB_API eDALProxy384LUSBErr Proxy384LUSB_GetFloatFeature(HANDLE paHandle, int paeFeature, float* paFloat);


/** Set string feature.
 * \param[in] paHandle SmartIR384L handle.
 * \param[in] paeFeature Feature written.
 * \param[in] paStr String for written feature.
 * \warning String Feature are 32 byte large, including null byte. Ensure paStr is large enougt.
*/
DALProxy384LUSB_API eDALProxy384LUSBErr Proxy384LUSB_SetStringFeature(HANDLE paHandle, int paeFeature, const char* paStr);

/** Set integer feature.
 * \param[in] paHandle SmartIR384L handle.
 * \param[in] paeFeature Feature written.
 * \param[in] paUInt Integer value for written feature.
*/
DALProxy384LUSB_API eDALProxy384LUSBErr Proxy384LUSB_SetUIntFeature(HANDLE paHandle, int paeFeature, unsigned int paUInt);

/** Query float feature.
 * \param[in] paHandle SmartIR384L handle.
 * \param[in] paeFeature Feature written.
 * \param[in] paFloat Float value for written feature.
*/
DALProxy384LUSB_API eDALProxy384LUSBErr Proxy384LUSB_SetFloatFeature(HANDLE paHandle, int paeFeature, float paFloat);

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
 * \param[in] paHandle SmartIR384L handle.
 * \param[in] paParam configuration to apply.
 * \li bit[0] Enable (1) or Disable (0) the automatic fast calibration associated with mechanical shutter
 * \li bit[1-31] Reserved.
*/
DALProxy384LUSB_API eDALProxy384LUSBErr Proxy384LUSB_SetCalibrationConfig(HANDLE paHandle, int paParam);

/** Enable/Disable NUC processing steps. These are enabled by default at connection.
 * \param[in] paHandle SmartIR384L handle.
 * \param[in] paBadPixels Enable(1)/Disable(0) bad pixels correction.
 * \param[in] paNUC Enable(1)/Disable(0) Non Uniformity Correction.
 * \return This return error eProxy384LUSBFeatureNotAvailable if Shutterless is activated.
*/
DALProxy384LUSB_API eDALProxy384LUSBErr Proxy384LUSB_SetNUCProcessing(HANDLE paHandle, unsigned char paBadPixels, unsigned char paNUC);

/** Query NUC processing steps status.
 * \param[in] paHandle SmartIR384L handle.
 * \param[out] paBadPixels bad pixels correction enable(1) or disable(0).
 * \param[out] paNUC Non Uniformity Correction enable(1) or disable(0).
 * \return This return error eProxy384LUSBFeatureNotAvailable if Shutterless is activated.
*/
DALProxy384LUSB_API eDALProxy384LUSBErr Proxy384LUSB_GetNUCProcessing(HANDLE paHandle, unsigned char* paBadPixels, unsigned char* paNUC);


/** Enable/Disable ShutterLess processing.
 * \param[in] paHandle SmartIR384L handle.
 * \param[in] bActivate Enable(true)/Disable(false) Shutterless processing.
 * \return This function return eProxy384LUSBFeatureNotAvailable error if Shutterless is unavailable on this module.
*/
DALProxy384LUSB_API eDALProxy384LUSBErr Proxy384LUSB_SetShutterLessProcessing(HANDLE paHandle, bool bActivate);

/** Query Shutterless processing status.
 * \param[in] paHandle SmartIR384L handle.
 * \param[out] pbIsActive shutterless processing enable(true) or disable(false).
*/
DALProxy384LUSB_API eDALProxy384LUSBErr Proxy384LUSB_GetShutterLessProcessing(HANDLE paHandle, bool *pbIsActive);

/** Set Auto Gain Control processing step. By default, No AGC processing set.
 * \param[in] paHandle SmartIR384L handle.
 * \param[in] paeAGCProcessing see eAGCProcessingValue for values.
*/
DALProxy384LUSB_API eDALProxy384LUSBErr Proxy384LUSB_SetAGCProcessing(HANDLE paHandle, unsigned char paeAGCProcessing);

/** Query processing steps status.
 * \param[in] paHandle SmartIR384L handle.
 * \param[out] paeAGCProcessing see eAGCProcessingValue for values.
*/
DALProxy384LUSB_API eDALProxy384LUSBErr Proxy384LUSB_GetAGCProcessing(HANDLE paHandle, unsigned char* paeAGCProcessing);

/** Set Gains values for NUC processing.
 * \param[in] paHandle SmartIR384L handle.
 * \param[in] paTableGains New Gains values for NUC processing.
 * \note Each pixel must have a value. So paTableGains must contains 384L * 288 float values (4 bytes float).
*/
DALProxy384LUSB_API eDALProxy384LUSBErr Proxy384LUSB_SetCurrentTableGain(HANDLE paHandle, float* paTableGains);

/** Set Offset values for NUC processing.
 * \param[in] paHandle SmartIR384L handle.
 * \param[in] paTableOffsets New offsets values for NUC processing.
 * \note Each pixel must have a value. So paTableGains must contains 384L * 288 values (2 bytes signed value).
*/
DALProxy384LUSB_API eDALProxy384LUSBErr Proxy384LUSB_SetCurrentTableOffset(HANDLE paHandle, signed short* paTableOffsets);

/** Set bad pixels position in image for bad pixels correction.
 * \param[in] paHandle SmartIR384L handle.
 * \param[in] paTableX,paTableY Bad pixels position in image.
 * \param[in] paCount bad pixels count.
*/
DALProxy384LUSB_API eDALProxy384LUSBErr Proxy384LUSB_SetCurrentBadPixels(HANDLE paHandle, unsigned short *paTableX, unsigned short *paTableY,
                                             unsigned short paCount );

/** Set Shutterless data for restore purpose. Shutterless data must be considered as binary and must not be modified.
 * \param[in] paHandle SmartIR384L handle.
 * \param[in] paShutterless values.
*/
DALProxy384LUSB_API eDALProxy384LUSBErr Proxy384LUSB_SetCurrentShutterless(HANDLE paHandle, unsigned int* paShutterless);

/** Get Shutterless size of the data for backup and restore purpose.
 * \param[in] paHandle SmartIR384L handle.
 * \param[out] pSize size of shutterless data in bytes.
*/
DALProxy384LUSB_API eDALProxy384LUSBErr Proxy384LUSB_GetCurrentShutterlessSize(HANDLE paHandle, unsigned int* pSize);

/** Get Shutterless data for backup purpose. Shutterless data must be considered as binary and must not be modified.
 * \param[in] paHandle SmartIR384L handle.
 * \param[in] paShutterless New Shutterless values.
*/
DALProxy384LUSB_API eDALProxy384LUSBErr Proxy384LUSB_GetCurrentShutterless(HANDLE paHandle, unsigned int* paShutterless);

/** Get Gains current values from NUC processing.
 * \param[in] paHandle SmartIR384L handle.
 * \param[in] paTableGains New Gains values for NUC processing.
 * \note Each pixel must have a value. So paTableGains must contains 384L * 288 float values (4 bytes float).
*/
DALProxy384LUSB_API eDALProxy384LUSBErr Proxy384LUSB_GetCurrentTableGain(HANDLE paHandle, float* paTableGains);

/** Get Offset current values from NUC processing.
 * \param[in] paHandle SmartIR384L handle.
 * \param[in] paTableOffsets New offsets values for NUC processing.
 * \note Each pixel must have a value. So  paTableGains must contains 384L * 288 values (2 bytes signed value).
*/
DALProxy384LUSB_API eDALProxy384LUSBErr Proxy384LUSB_GetCurrentTableOffset(HANDLE paHandle, signed short* paTableOffsets);

/** Get current bad pixels position in image from bad pixels correction.
 * \param[in] paHandle SmartIR384L handle.
 * \param[in] paTableX,paTableY Bad pixels position in image.
 * \param paCount Initial bad pixels array size, on return, bad pixel count.
 * \note paCount must be init with  paTableX / paTableY placeholder size (to avoid overflow),
 * and will be modified by function with current bad pixel count.
*/
DALProxy384LUSB_API eDALProxy384LUSBErr Proxy384LUSB_GetCurrentBadPixels(HANDLE paHandle, unsigned short *paTableX, unsigned short *paTableY,
                                             unsigned short *paCount );

//! @}

/** \addtogroup Module_Storage
 *  SmartIR384L provides \b 8 slots to store Gain or Offset value. Slot are not dedicated to a kind of data.
 *  \attention Storage space is limited into SmartIR384L. Hence, data (Gain or Offset) are rounded to fit into slot.
 *  This may involve difference if your store data, read it, and compare to your initial values.
 *  For coherence, this data reduction is also apply when update NUC processing data (see \ref Module_Processing).
 *
 *  Save functions provides a \em MakeDefault parameter. When set to 1 (enable), this will mark slot as default.
 * When application connect to SmartIR384L, Proxy384LUSB_ConnectToModule() function will look for default slot,
 * and load into processing data from slot.
 @{ */

/** Default slot index for Gain values and Offset values, last setting's bank used.
 * \param[in] paHandle SmartIR384L handle.
 * \param[out] paiIdxGains Gain slot index, of 255 if no default Gain slot index.
 * \param[out] paiIdxOffsets Offset slot index, of 255 if no default Offset slot index.
 * \param[out] paiIdxBank Settings bank index, of 255 if no default settings index.
 * \note No need to call and use this function (already done at SmartIR384L connection)
 **/
DALProxy384LUSB_API eDALProxy384LUSBErr Proxy384LUSB_StartupDefault(HANDLE paHandle, unsigned char *paiIdxGains, unsigned char* paiIdxOffsets, unsigned char *paiIdxBank);

/** Query slot data type.
 * \param[in] paHandle SmartIR384L handle.
 * \param[in] paiIndex Slot index to query.
 * \param[out] paeType Slot type.
 * \param[out] paData Table associate data. NULL, or 60 bytes placeholder. paData is additional data associated to Gain or Offset array, which can be used freely by application for instance to keep a trace of Gain or Offset table calibration conditions, either sensitivity, either focal plane array temperature.
 * Slot type value are :
 *\li 0 :Empty slot.
 *\li 1 :Gain values.
 *\li 2 :Offset values.
 **/
DALProxy384LUSB_API eDALProxy384LUSBErr Proxy384LUSB_SlotType(HANDLE paHandle, unsigned char paiIndex, unsigned char* paeType,
                                                           void* paData);


/** Retrieve SmartIR384L slot data as Gain values.<br>
 * This function may failed if slot is empty, or slot data are not Gain values.
 * \param[in] paHandle SmartIR384L handle.
 * \param[in] paiIndex Slot index as data.
 * \param[out] paTableGain Gain values from SmartIR384L slot.
 * \param[out] paData Table associate data. NULL, or 60 bytes placeholder. paData is additional data associated to Gain or Offset array, which can be used freely by application for instance to keep a trace of Gain or Offset table calibration conditions, either sensitivity, either focal plane array temperature.
 */
DALProxy384LUSB_API eDALProxy384LUSBErr Proxy384LUSB_LoadTableGain(HANDLE paHandle, unsigned char paiIndex, float* paTableGain,
                                                                void* paData);

/** Retrieve SmartIR384L slot data as Offset values.<br>
 * This function may failed if slot is empty, or slot data are not Offset values.
 * \param[in] paHandle SmartIR384L handle.
 * \param[in] paiIndex Slot index as data.
 * \param[out] paTableOffset Offset values from SmartIR384L slot.
 * \param[out] paData Table associate data. NULL, or 60 bytes placeholder. paData is additional data associated to Gain or Offset array, which can be used freely by application for instance to keep a trace of Gain or Offset table calibration conditions, either sensitivity, either focal plane array temperature.
 */
DALProxy384LUSB_API eDALProxy384LUSBErr Proxy384LUSB_LoadTableOffset(HANDLE paHandle, unsigned char paiIndex, short* paTableOffset,
                                                                  void* paData);


/** Retrieve bad pixel position in image from SmartIR384L.
 * \param[in] paHandle SmartIR384L handle.
 * \param[out] paTableX,paTableY Bad pixels position in image.
 * \param paCount Initial bad pixels array size, on return, bad pixel count.
 * \note paCount must be init with paTableX / paTableY placeholder size (to avoid overflow),
 * and will be modified by function with current bad pixel count.
*/
DALProxy384LUSB_API eDALProxy384LUSBErr Proxy384LUSB_LoadBadPixels(HANDLE paHandle,
                                                                unsigned short *paTableX, unsigned short *paTableY,
                                                                unsigned short* paCount);


/** Save Gain values into SmartIR384L slot data.
 * \param[in] paHandle SmartIR384L handle.
 * \param[in] paiIndex Slot index.
 * \param[in] paTableGain Gain values to store into SmartIR384L slot.
 * \param[in] paData Table associate data. NULL, or 60 bytes placeholder. paData is additional data associated to Gain or Offset array, which can be used freely by application for instance to keep a trace of Gain or Offset table calibration conditions, either sensitivity, either focal plane array temperature.
 */
DALProxy384LUSB_API eDALProxy384LUSBErr Proxy384LUSB_SaveTableGain(HANDLE paHandle, unsigned char paiIndex, const float* paTableGain,
                                              void* paData);

/** Save Offset values into SmartIR384L slot data.
 * \param[in] paHandle SmartIR384L handle.
 * \param[in] paiIndex Slot index.
 * \param[in] paTableOffset Offset values to store into SmartIR384L slot.
 * \param[in] paData Table associate data. NULL, or 60 bytes placeholder. paData is additional data associated to Gain or Offset array, which can be used freely by application for instance to keep a trace of Gain or Offset table calibration conditions, either sensitivity, either focal plane array temperature.
 */
DALProxy384LUSB_API eDALProxy384LUSBErr Proxy384LUSB_SaveTableOffset(HANDLE paHandle, unsigned char paiIndex, const short* paTableOffset,
                                                                  void* paData);

/** Save bad pixel position into SmartIR384L slot data.
 * \param[in] paHandle SmartIR384L handle.
 * \param[in] paTableX,paTableY Bad pixels position in image.
 * \param[in] paCount bad pixels count.
*/
DALProxy384LUSB_API eDALProxy384LUSBErr Proxy384LUSB_SaveBadPixels(HANDLE paHandle, const unsigned short *paTableX, const unsigned short* paTableY, unsigned short paCount);


/** Use SmartIR384L slot data as Gain values for NUC processing,
 * i.e. retrieve it from SmartIR384L, and set it to NUC processing.
 * \param[in] paHandle SmartIR384L handle.
 * \param[in] paiIndex Slot index as data.
 */
DALProxy384LUSB_API eDALProxy384LUSBErr Proxy384LUSB_LoadCurrentTableGain(HANDLE paHandle, unsigned char paiIndex);

/** Use SmartIR384L slot data as Offset values for NUC processing,
 * i.e. retrieve it from SmartIR384L, and set it to NUC processing.
 * \param[in] paHandle SmartIR384L handle.
 * \param[in] paiIndex Slot index as data.
 */
DALProxy384LUSB_API eDALProxy384LUSBErr Proxy384LUSB_LoadCurrentTableOffset(HANDLE paHandle, unsigned char paiIndex);

/** Use SmartIR384L stored bad pixel for bad pixel correction.
 * \param[in] paHandle SmartIR384L handle.
 */
DALProxy384LUSB_API eDALProxy384LUSBErr Proxy384LUSB_LoadCurrentBadPixels(HANDLE paHandle);

/** Save current Gain values into SmartIR384L slot data.
 * \param[in] paHandle SmartIR384L handle.
 * \param[in] paiIndex Slot index.
 * \param[in] paData Table associate data. NULL, or 60 bytes placeholder. paData is additional data associated to Gain or Offset array, which can be used freely by application for instance to keep a trace of Gain or Offset table calibration conditions, either sensitivity, either focal plane array temperature.
 */
DALProxy384LUSB_API eDALProxy384LUSBErr Proxy384LUSB_SaveCurrentTableGain(HANDLE paHandle, unsigned char paiIndex, const void *paData);

/** Save Offset values into SmartIR384L slot data.
 * \param[in] paHandle SmartIR384L handle.
 * \param[in] paiIndex Slot index.
 * \param[in] paData Table associate data. NULL, or 60 bytes placeholder. paData is additional data associated to Gain or Offset array, which can be used freely by application for instance to keep a trace of Gain or Offset table calibration conditions, either sensitivity, either focal plane array temperature.
 */
DALProxy384LUSB_API eDALProxy384LUSBErr Proxy384LUSB_SaveCurrentTableOffset(HANDLE paHandle, unsigned char paiIndex, const void *paData);

/** Save bad pixel position into SmartIR384L slot data.
 * \param[in] paHandle SmartIR384L handle.
*/
DALProxy384LUSB_API eDALProxy384LUSBErr Proxy384LUSB_SaveCurrentBadPixels(HANDLE paHandle);

/** Save Shutterless Tables into SmartIR384L.
 * \param[in] paHandle SmartIR384L handle.
 * \return eProxy384LUSBFeatureNotAvailable error if Shutterless is unavailable on the module.
*/
DALProxy384LUSB_API eDALProxy384LUSBErr Proxy384LUSB_SaveCurrentShutterlessTables(HANDLE paHandle);

/** Load Shutterless Tables from SmartIR384L in order to use it with shutterless processing
 * \param[in] paHandle SmartIR384L handle.
 * \return eProxy384LUSBFeatureNotAvailable error if Shutterless is unavailable on the module.
*/
DALProxy384LUSB_API eDALProxy384LUSBErr Proxy384LUSB_LoadCurrentShutterlessTables(HANDLE paHandle);



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
 * \param[in] paHandle SmartIR384L Handle.
 */
DALProxy384LUSB_API eDALProxy384LUSBErr Proxy384LUSB_AbortCalibration(HANDLE paHandle);

/** Prepare NUC Calibration engine.
 * \param[in] paHandle SmartIR384L Handle.
 * \param[in] iStage Stage of the calibration (1 or 2).
 * \return eProxy384LUSBSequencingError see \ref CalibrationProcessPage for details.
*/
DALProxy384LUSB_API eDALProxy384LUSBErr Proxy384LUSB_InitShutter2PtsCalibration(HANDLE paHandle, unsigned int iStage);

/** Add image for Shutter 2pts calibration.
  *
  * Low temperature image for iStage = 1, High temperature image for iStage = 2.
  *
  * \param[in] paHandle SmartIR384L Handle.
  * \param[in] iStage Stage of calibration (1 (low) or 2 (high)).
  * \return eProxy384LUSBSequencingError see \ref CalibrationProcessPage for details.
  */
DALProxy384LUSB_API eDALProxy384LUSBErr Proxy384LUSB_StepShutter2PtsCalibration(HANDLE paHandle, unsigned int iStage);

/** Perform two points calibration using low and high temperature images.
 *
 * Once calibration is done, new Gain, Offset and bad pixel are set to current NUC and BPC processing.
 *
 * \param[in] paHandle SmartIR384L handle.
 * \param[in] iStage Stage of the calibration. If stage = 2, perform the final step of calibration. Once is done, new Gain, Offset
 * and bad pixel are set to current NUC and BPC processing.
 * \return eProxy384LUSBSequencingError see \ref CalibrationProcessPage for details.
 */
DALProxy384LUSB_API eDALProxy384LUSBErr Proxy384LUSB_FinishShutter2PtsCalibration(HANDLE paHandle,unsigned int iStage);

/** Prepare Shutter Calibration engine, also called one point calibration.
 *
 * This calibration will only produce new Offset values.
 *
 * \param[in] paHandle SmartIR384L handle.
 * \return eProxy384LUSBSequencingError see \ref CalibrationProcessPage for details.
*/
DALProxy384LUSB_API eDALProxy384LUSBErr Proxy384LUSB_InitShutterCalibration(HANDLE paHandle);

/** Add image to prepare Shutter Calibration
 *
 * \param[in] paHandle SmartIR384L handle.
 * \return eProxy384LUSBSequencingError see \ref CalibrationProcessPage for details.
 */
DALProxy384LUSB_API eDALProxy384LUSBErr Proxy384LUSB_StepShutterCalibration(HANDLE paHandle);

/** Perform Shutter calibration.
 *
 * Once calibration is done, Offset values are set to current NUC processing.
 *
 * \param[in] paHandle SmartIR384L handle.
 * \return eProxy384LUSBSequencingError see \ref CalibrationProcessPage for details.
 */
DALProxy384LUSB_API eDALProxy384LUSBErr Proxy384LUSB_FinishShutterCalibration(HANDLE paHandle);



/** Initialise Stage for Shutterless Calibration T0
 *
 * Must be use on correct sequencing with SmartIR384L shutterless module.
 *
 * \param[in] paHandle SmartIR384L handle.
 * \param[in] iStage Stage number of calibration.
 * \return eProxy384LUSBSequencingError see \ref CalibrationProcessPage for details.
 */
DALProxy384LUSB_API eDALProxy384LUSBErr Proxy384LUSB_InitSLCalibrationT0(HANDLE paHandle, unsigned int iStage);

/** Add image for Shutterless Calibration T0
 *
 * Must be used on correct sequencing with SmartIR384L shutterless module.
 *
 * \param[in] paHandle SmartIR384L handle.
 * \param[in] iStage Stage number of calibration (1 = low temperature, 2 = high temperature).
 * \return eProxy384LUSBSequencingError see \ref CalibrationProcessPage for details.
 */
DALProxy384LUSB_API eDALProxy384LUSBErr Proxy384LUSB_StepSLCalibrationT0(HANDLE paHandle, unsigned int iStage);

/** Perform Shutterless T0 calibration
 *
 * Must be used on correct sequencing with SmartIR384L shutterless module.
 * Once calibration is done, new Shutterless tables are set to current Shutterless processing.
 *
 * \param[in] paHandle SmartIR384L handle.
 * \param[in] iStage Stage number of calibration (1 = low temperature, 2 = high temperature).
 * \return eProxy384LUSBSequencingError see \ref CalibrationProcessPage for details.
 */
DALProxy384LUSB_API eDALProxy384LUSBErr Proxy384LUSB_FinishSLCalibrationT0(HANDLE paHandle, unsigned int iStage);



/** Initialise Shutterless Calibration T1
 *
 * Must be use on correct sequencing with SmartIR384L shutterless module.
 *
 * \param[in] paHandle SmartIR384L handle.
 * \return eProxy384LUSBSequencingError see \ref CalibrationProcessPage for details.
 */
DALProxy384LUSB_API eDALProxy384LUSBErr Proxy384LUSB_InitSLCalibrationT1(HANDLE paHandle);

/** Add image for Shutterless Calibration T1
 *
 * Must be use on correct sequencing with SmartIR384L shutterless module.
 *
 * \param[in] paHandle SmartIR384L handle.
 * \return eProxy384LUSBSequencingError see \ref CalibrationProcessPage for details.
 */
DALProxy384LUSB_API eDALProxy384LUSBErr Proxy384LUSB_StepSLCalibrationT1(HANDLE paHandle);

/** Perform Shutterless Calibration T1
 *
 * Must be use on correct sequencing with SmartIR384L shutterless module.
 * Once Calibration is done, new Shutterless T1 tables are set for current Shutterless processing.
 *
 * \param[in] paHandle SmartIR384L handle.
 * \return eProxy384LUSBSequencingError see \ref CalibrationProcessPage for details.
 */
DALProxy384LUSB_API eDALProxy384LUSBErr Proxy384LUSB_FinishSLCalibrationT1(HANDLE paHandle);

//! @}

#ifdef __cplusplus
   }
#endif

#endif // __DALProxy384LUSB_H__
