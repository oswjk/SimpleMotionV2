#ifndef SMDEPLOYMENTTOOL_H
#define SMDEPLOYMENTTOOL_H

#ifdef WIN32
//dll specs
#ifdef BUILD_DLL
    #define LIB __declspec(dllexport)
#else
//    #define LIB __declspec(dllimport)
#define LIB
#endif
#else
#define LIB
#endif


#include "simplemotion.h"


#ifdef __cplusplus
extern "C"{
#endif


typedef enum
{
    FWComplete=100,
    FWInvalidFile=-1,
    FWConnectionError=-2,
    FWIncompatibleFW=-3,
    FWConnectionLoss=-4,
    FWUnsupportedTargetDevice=-5,
    FWFileNotReadable=-6
} FirmwareUploadStatus;

/**
 * @brief smFirmwareUpload Sets drive in firmware upgrade mode if necessary and uploads a new firmware. Call this many until it returns value 100 (complete) or a negative value (error).
 * @param smhandle SM bus handle, must be opened before call
 * @param smaddress Target SM device address
 * @param filename .gdf file name
 * @return Enum FirmwareUploadStatus that indicates errors or Complete status. Typecast to integer to get progress value 0-100.
 */
LIB FirmwareUploadStatus smFirmwareUpload(const smbus smhandle, const int smaddress, const char *firmware_filename );

typedef enum
{
    CFGComplete=100,
    CFGInvalidFile=-1,
    CFGCommunicationError=-2,
    CFGConnectingDFUModeFailed=-3,
    CFGIncompatibleFW=-4,
    CFGUnsupportedTargetDevice=-5,
    CFGUnableToOpenFile=-6

} LoadConfigurationStatus;

//TODO implement: #define CONFIGMODE_REQUIRE_SAME_FW 1 //will return IncompatibleFW if firmware checksum does not match the one in .drc files. if this error is returned, perform smFirmwareUpload and perform smLoadConfiguration again. Requires DRC file version 111 or later (if not met, returns InvalidFile).
#define CONFIGMODE_ALWAYS_RESTART_TARGET 2 //will perform device restart after setup even when it's not required
#define CONFIGMODE_DISABLE_DURING_CONFIG 4 //will set device in disabled state during configuration
#define CONFIGMODE_CLEAR_FAULTS_AFTER_CONFIG 8 //will perform clear faults command after configuration

/**
 * @brief smConfigureParameters Configures all target device parameters from file and performs device restart if necessary. This can take few seconds to complete. This may take 2-5 seconds to call.
 * @param smhandle SM bus handle, must be opened before call
 * @param smaddress Target SM device address
 * @param filename .DRC file name
 * @param mode Combined from CONFIGMODE_ define bits (can logic OR mutliple values).
 * @return Enum LoadConfigurationStatus
 *
 * Requires DRC file version 111 or later to use CONFIGMODE_REQUIRE_SAME_FW.
 */
LIB LoadConfigurationStatus smLoadConfiguration( const smbus smhandle, const int smaddress, const char *filename, unsigned int mode, int *skippedCount, int *errorCount );


/**
 * @brief smGetDeviceFirmwareUniqueID Reads installed firmware binary checksum that can be used to verify whether a wanted FW version is installed
 * @param smhandle SM bus handle, must be opened before call
 * @param smaddress Target SM device address. Can be device in DFU mode or main operating mode. For Argon, one device in a bus must be started into DFU mode by DIP switches and smaddress must be set to 255.
 * @param UID result will be written to this pointer
 * @return smtrue if success, smfalse if failed (if communication otherwise works, then probably UID feature not present in this firmware version)
 */
smbool smGetDeviceFirmwareUniqueID( smbus smhandle, int deviceaddress, smuint32 *UID );



#ifdef __cplusplus
}
#endif
#endif // SMDEPLOYMENTTOOL_H