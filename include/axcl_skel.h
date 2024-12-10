/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#ifndef _AXCL_SKEL_H_
#define _AXCL_SKEL_H_

#include "axcl_skel_type.h"

#ifdef __cplusplus
extern "C" {
#endif

//////////////////////////////////////////////////////////////////////////////////////
/// @brief Initialize skel sdk
///
/// @param pstParam   [I]: initialize parameter
///
/// @return 0 if success, otherwise failure
//////////////////////////////////////////////////////////////////////////////////////
AX_S32 AXCL_SKEL_Init(const AXCL_SKEL_INIT_PARAM_T *pstParam);

//////////////////////////////////////////////////////////////////////////////////////
/// @brief uninitialize skel sdk
///
/// @param NA
///
/// @return 0 if success, otherwise failure
//////////////////////////////////////////////////////////////////////////////////////
AX_S32 AXCL_SKEL_DeInit(AX_VOID);

//////////////////////////////////////////////////////////////////////////////////////
/// @brief create handle
///
/// @param pstParam   [I]: handle parameter
/// @param handle     [O]: handle
///
/// @return 0 if success, otherwise failure
//////////////////////////////////////////////////////////////////////////////////////
AX_S32 AXCL_SKEL_Create(const AXCL_SKEL_HANDLE_PARAM_T *pstParam, AXCL_SKEL_HANDLE *pHandle);

//////////////////////////////////////////////////////////////////////////////////////
/// @brief destroy handle
///
/// @param pHandle    [I]: handle
///
/// @return 0 if success, otherwise failure
//////////////////////////////////////////////////////////////////////////////////////
AX_S32 AXCL_SKEL_Destroy(AXCL_SKEL_HANDLE handle);

//////////////////////////////////////////////////////////////////////////////////////
/// @brief register get algorithm result callback
///
/// @param pHandle    [I]: handle
/// @param callback   [I]: callback function
/// @param pUserData  [I]: private user data
///
/// @return 0 if success, otherwise failure
//////////////////////////////////////////////////////////////////////////////////////
AX_S32 AXCL_SKEL_RegisterResultCallback(AXCL_SKEL_HANDLE handle, AXCL_SKEL_RESULT_CALLBACK_FUNC callback, AX_VOID *pUserData);

//////////////////////////////////////////////////////////////////////////////////////
/// @brief send frame
///
/// @param pHandle    [I]: handle
/// @param pstImage   [I]: image
/// @param ppstResult [O]: algorithm result
///
/// @return 0 if success, otherwise failure
//////////////////////////////////////////////////////////////////////////////////////
AX_S32 AXCL_SKEL_SendFrame(AXCL_SKEL_HANDLE handle, const AXCL_SKEL_FRAME_T *pstFrame, AX_S32 nTimeout);

//////////////////////////////////////////////////////////////////////////////////////
/// @brief process image
///
/// @param pHandle    [I]: handle
/// @param ppstResult [O]: algorithm result
/// @param nTimeout   [I]: timeout
///
/// @return 0 if success, otherwise failure
//////////////////////////////////////////////////////////////////////////////////////
AX_S32 AXCL_SKEL_GetResult(AXCL_SKEL_HANDLE handle, AXCL_SKEL_RESULT_T **ppstResult, AX_S32 nTimeout);

//////////////////////////////////////////////////////////////////////////////////////
/// @brief free memory
///
/// @param p           [I]: memory address
///
/// @return 0 if success, otherwise failure
//////////////////////////////////////////////////////////////////////////////////////
AX_S32 AXCL_SKEL_Release(AX_VOID *p);

//////////////////////////////////////////////////////////////////////////////////////
/// @brief get capability
///
/// @param ppstCapability [O]: Capability
///
/// @return version info
//////////////////////////////////////////////////////////////////////////////////////
AX_S32 AXCL_SKEL_GetCapability(const AXCL_SKEL_CAPABILITY_T **ppstCapability);

//////////////////////////////////////////////////////////////////////////////////////
/// @brief get version
///
/// @param NA
///
/// @return version info
//////////////////////////////////////////////////////////////////////////////////////
AX_S32 AXCL_SKEL_GetVersion(const AXCL_SKEL_VERSION_INFO_T **ppstVersion);

//////////////////////////////////////////////////////////////////////////////////////
/// @brief get config
///
/// @param pstConfig           [I/O]: config
///
/// @return version info
//////////////////////////////////////////////////////////////////////////////////////
AX_S32 AXCL_SKEL_GetConfig(AXCL_SKEL_HANDLE handle, const AXCL_SKEL_CONFIG_T **ppstConfig);

//////////////////////////////////////////////////////////////////////////////////////
/// @brief set config
///
/// @param pstConfig           [I]: config
///
/// @return version info
//////////////////////////////////////////////////////////////////////////////////////
AX_S32 AXCL_SKEL_SetConfig(AXCL_SKEL_HANDLE handle, const AXCL_SKEL_CONFIG_T *pstConfig);
#ifdef __cplusplus
}
#endif

#endif /* _AXCL_SKEL_H_ */