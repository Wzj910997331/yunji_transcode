/**************************************************************************************************
 *
 * Copyright (c) 2019-2024 Axera Semiconductor Co., Ltd. All Rights Reserved.
 *
 * This source file is the property of Axera Semiconductor Co., Ltd. and
 * may not be copied or distributed in any isomorphic form without the prior
 * written consent of Axera Semiconductor Co., Ltd.
 *
 **************************************************************************************************/

#include "sample_ivps_main.h"

SAMPLE_IVPS_SYNC_API_T gSampleIvpsSyncApi = {

};

SAMPLE_IVPS_REGION_T gSampleIvpsRegion = {

};

AX_S32 SAMPLE_IVPS_SyncApi(const IVPS_ARG_T *ptArg, const SAMPLE_IVPS_GRP_T *pGrp,
                           const SAMPLE_IVPS_SYNC_API_T *ptSyncIntf)
{
    AX_S32 ret;
    int i;
    AX_VIDEO_FRAME_T tOverlay = {0};
    AX_VIDEO_FRAME_T tSpAlpha = {0};
    SAMPLE_IVPS_COVER_T IvpsCover = {0};

    if (ptArg->bCpuDraw == 1)
    {
        ret = SAMPLE_DrawCover(&pGrp->tFrameInput, &IvpsCover, pGrp->pFilePath, ptArg->pCaseId);
        if (ret)
        {
            ALOGE("SAMPLE_DrawCover, ret=0x%x.", ret);
            return -1;
        }
    }

    if (ptArg->bCmmCopy)
    {
        ret = SAMPLE_IVPS_CmmCopy(ptArg->nEngineId, &pGrp->tFrameInput, pGrp->pFilePath, ptArg->pCaseId);
        if (ret)
        {
            ALOGE("SAMPLE_IVPS_CmmCopy failed, ret=0x%x.", ret);
            goto error1;
        }
    }
    if (ptArg->bCsc)
    {
        ret = SAMPLE_IVPS_Csc(ptArg->nEngineId, &pGrp->tFrameInput, pGrp->pFilePath, ptArg->pCaseId);
        if (ret)
        {
            ALOGE("SAMPLE_IVPS_Csc failed, ret=0x%x.", ret);
            goto error1;
        }
    }

    if (ptArg->bFlipRotation)
    {
        ret = SAMPLE_IVPS_FlipAndRotation(&pGrp->tFrameInput, 1, AX_IVPS_ROTATION_90, pGrp->pFilePath, ptArg->pCaseId);
        if (ret)
        {
            ALOGE("SAMPLE_IVPS_FlipAndRotation failed, ret=0x%x.", ret);
            goto error1;
        }
    }

    if (ptArg->bAlphaBlend && ptArg->pOverlayInfo)
    {
        char *pOverlayFile = FrameInfoGet(ptArg->pOverlayInfo, &tOverlay);
        FrameBufGet(0, &tOverlay, pOverlayFile);
        ret = SAMPLE_IVPS_AlphaBlending(ptArg->nEngineId, &pGrp->tFrameInput, &tOverlay, 128, pGrp->pFilePath, ptArg->pCaseId);
        if (ret)
        {
            ALOGE("SAMPLE_IVPS_AlphaBlending failed, ret=0x%x.", ret);
            goto error1;
        }
    }
    if (ptArg->bAlphaBlendV3 && ptArg->pOverlayInfo)
    {
        char *pOverlayFile = FrameInfoGet(ptArg->pOverlayInfo, &tOverlay);
        FrameBufGet(0, &tOverlay, pOverlayFile);
        ret = SAMPLE_IVPS_AlphaBlendingV3(ptArg->nEngineId, &pGrp->tFrameInput, &tOverlay, 128, pGrp->pFilePath, ptArg->pCaseId);
        if (ret)
        {
            ALOGE("SAMPLE_IVPS_AlphaBlendingV3 failed, ret=0x%x.", ret);
            goto error1;
        }
    }
    if (ptArg->bAlphaBlendV2 && ptArg->pOverlayInfo && ptArg->pSpAlphaFileInfo)
    {
        char *pOverlayFile = FrameInfoGet(ptArg->pOverlayInfo, &tOverlay);
        FrameBufGet(0, &tOverlay, pOverlayFile);

        tSpAlpha.u32PicStride[0] = tOverlay.u32PicStride[0];
        tSpAlpha.u32Width = tOverlay.u32Width;
        tSpAlpha.u32Height = tOverlay.u32Height;
        tSpAlpha.u32FrameSize = tSpAlpha.u32PicStride[0] * tSpAlpha.u32Height;
        FrameBufGet(0, &tSpAlpha, ptArg->pSpAlphaFileInfo);
        ALOGI("Sp_alpha stride: %d, width: %d, height: %d, framesize: %d.",
              tSpAlpha.u32PicStride[0], tSpAlpha.u32Width, tSpAlpha.u32Height, tSpAlpha.u32FrameSize);

        AX_IVPS_ALPHA_LUT_T tAlphaLut = {0};
        tAlphaLut.bAlphaEnable = 1;
        tAlphaLut.bAlphaReverse = 0;
        tAlphaLut.u64PhyAddr = tSpAlpha.u64PhyAddr[0];
        ALOGI("AlphaLut Enable: %d, PhyAddr: 0x%llx, AlphaReverse: %d.",
              tAlphaLut.bAlphaEnable, tAlphaLut.u64PhyAddr, tAlphaLut.bAlphaReverse);

        ret = SAMPLE_IVPS_AlphaBlendingV2(&pGrp->tFrameInput, &tOverlay, &tAlphaLut, pGrp->pFilePath, ptArg->pCaseId);
        if (ret)
        {
            ALOGE("SAMPLE_IVPS_AlphaBlendingV2 failed, ret=0x%x.", ret);
            goto error1;
        }
    }

    if (ptArg->bCropResize)
    {
        ret = SAMPLE_IVPS_CropResize(ptArg->nEngineId, &pGrp->tFrameInput, pGrp->pFilePath, ptArg->pCaseId);
        if (ret)
        {
            ALOGE("SAMPLE_IVPS_CropResize failed, ret=0x%x.\n", ret);
            goto error1;
        }
    }
    if (ptArg->bCropResizeV2)
    {
        AX_S32 DstStride[32] = {256, 256, 256, 256,
                                256, 256, 256, 256,
                                256, 256, 256, 256,
                                256, 256, 256, 256,
                                256, 256, 256, 256,
                                256, 256, 256, 256,
                                256, 256, 256, 256,
                                256, 256, 256, 256};
        AX_S32 DstWidth[32] = {256, 256, 256, 256,
                               256, 256, 256, 256,
                               256, 256, 256, 256,
                               256, 256, 256, 256,
                               256, 256, 256, 256,
                               256, 256, 256, 256,
                               256, 256, 256, 256,
                               256, 256, 256, 256};
        AX_S32 DstHeight[32] = {256, 256, 256, 256,
                                256, 256, 256, 256,
                                256, 256, 256, 256,
                                256, 256, 256, 256,
                                256, 256, 256, 256,
                                256, 256, 256, 256,
                                256, 256, 256, 256,
                                256, 256, 256, 256};

        AX_IVPS_RECT_T tBox[32] = {{0, 0, 512, 512},
                                   {0, 0, 512, 512},
                                   {0, 0, 512, 512},
                                   {0, 0, 512, 512},
                                   {0, 0, 512, 512},
                                   {0, 0, 512, 512},
                                   {0, 0, 512, 512},
                                   {0, 0, 512, 512},
                                   {0, 0, 512, 512},
                                   {0, 0, 512, 512},
                                   {0, 0, 512, 512},
                                   {0, 0, 512, 512},
                                   {0, 0, 512, 512},
                                   {0, 0, 512, 512},
                                   {0, 0, 512, 512},
                                   {0, 0, 512, 512},
                                   {0, 0, 512, 512},
                                   {0, 0, 512, 512},
                                   {0, 0, 512, 512},
                                   {0, 0, 512, 512},
                                   {0, 0, 512, 512},
                                   {0, 0, 512, 512},
                                   {0, 0, 512, 512},
                                   {0, 0, 512, 512},
                                   {0, 0, 512, 512},
                                   {0, 0, 512, 512},
                                   {0, 0, 512, 512},
                                   {0, 0, 512, 512},
                                   {0, 0, 512, 512},
                                   {0, 0, 512, 512},
                                   {0, 0, 512, 512},
                                   {0, 0, 512, 512}};

        ret = SAMPLE_IVPS_CropResizeV2(ptArg->nEngineId, &pGrp->tFrameInput, tBox, 32, DstStride, DstWidth, DstHeight, pGrp->pFilePath, ptArg->pCaseId);
        if (ret)
        {
            ALOGE("SAMPLE_IVPS_CropResizeV2 failed, ret=0x%x.\n", ret);
            goto error1;
        }
    }
    if (ptArg->bCropResizeV3)
    {
        AX_S32 DstStride[4] = {1024, 512, 256, 1024};
        AX_S32 DstWidth[4] = {1024, 512, 256, 1024};
        AX_S32 DstHeight[4] = {1024, 512, 256, 1024};
        ret = SAMPLE_IVPS_CropResizeV3(&pGrp->tFrameInput, DstStride, DstWidth, DstHeight, 4, pGrp->pFilePath, ptArg->pCaseId);
        if (ret)
        {
            ALOGE("SAMPLE_IVPS_CropResizeV3 failed, ret=0x%x.", ret);
            goto error1;
        }
    }
    if (ptArg->bMosaic)
    {
        AX_IVPS_RGN_MOSAIC_T tMosaic[32] = {
            {
                .tRect = {
                    .nX = 0,
                    .nY = 0,
                    .nW = 30,
                    .nH = 30,
                },
                .eBklSize = AX_IVPS_MOSAIC_BLK_SIZE_64,
            },
            {
                .tRect = {
                    .nX = 30,
                    .nY = 30,
                    .nW = 30,
                    .nH = 30,
                },
                .eBklSize = AX_IVPS_MOSAIC_BLK_SIZE_64,
            },
            {
                .tRect = {
                    .nX = 60,
                    .nY = 60,
                    .nW = 30,
                    .nH = 30,
                },
                .eBklSize = AX_IVPS_MOSAIC_BLK_SIZE_64,
            },
            {
                .tRect = {
                    .nX = 90,
                    .nY = 90,
                    .nW = 30,
                    .nH = 30,
                },
                .eBklSize = AX_IVPS_MOSAIC_BLK_SIZE_64,
            },
            {
                .tRect = {
                    .nX = 120,
                    .nY = 120,
                    .nW = 30,
                    .nH = 30,
                },
                .eBklSize = AX_IVPS_MOSAIC_BLK_SIZE_64,
            },
            {
                .tRect = {
                    .nX = 150,
                    .nY = 150,
                    .nW = 30,
                    .nH = 30,
                },
                .eBklSize = AX_IVPS_MOSAIC_BLK_SIZE_64,
            },
            {
                .tRect = {
                    .nX = 180,
                    .nY = 180,
                    .nW = 30,
                    .nH = 30,
                },
                .eBklSize = AX_IVPS_MOSAIC_BLK_SIZE_64,
            },
            {
                .tRect = {
                    .nX = 210,
                    .nY = 210,
                    .nW = 30,
                    .nH = 30,
                },
                .eBklSize = AX_IVPS_MOSAIC_BLK_SIZE_64,
            },

            {
                .tRect = {
                    .nX = 240,
                    .nY = 240,
                    .nW = 30,
                    .nH = 30,
                },
                .eBklSize = AX_IVPS_MOSAIC_BLK_SIZE_64,
            },
            {
                .tRect = {
                    .nX = 270,
                    .nY = 270,
                    .nW = 30,
                    .nH = 30,
                },
                .eBklSize = AX_IVPS_MOSAIC_BLK_SIZE_64,
            },
            {
                .tRect = {
                    .nX = 300,
                    .nY = 300,
                    .nW = 30,
                    .nH = 30,
                },
                .eBklSize = AX_IVPS_MOSAIC_BLK_SIZE_64,
            },
            {
                .tRect = {
                    .nX = 330,
                    .nY = 330,
                    .nW = 30,
                    .nH = 30,
                },
                .eBklSize = AX_IVPS_MOSAIC_BLK_SIZE_64,
            },
            {
                .tRect = {
                    .nX = 360,
                    .nY = 360,
                    .nW = 30,
                    .nH = 30,
                },
                .eBklSize = AX_IVPS_MOSAIC_BLK_SIZE_64,
            },
            {
                .tRect = {
                    .nX = 390,
                    .nY = 390,
                    .nW = 30,
                    .nH = 30,
                },
                .eBklSize = AX_IVPS_MOSAIC_BLK_SIZE_64,
            },
            {
                .tRect = {
                    .nX = 420,
                    .nY = 420,
                    .nW = 30,
                    .nH = 30,
                },
                .eBklSize = AX_IVPS_MOSAIC_BLK_SIZE_64,
            },
            {
                .tRect = {
                    .nX = 450,
                    .nY = 450,
                    .nW = 30,
                    .nH = 30,
                },
                .eBklSize = AX_IVPS_MOSAIC_BLK_SIZE_64,
            },

            {
                .tRect = {
                    .nX = 480,
                    .nY = 480,
                    .nW = 30,
                    .nH = 30,
                },
                .eBklSize = AX_IVPS_MOSAIC_BLK_SIZE_64,
            },
            {
                .tRect = {
                    .nX = 510,
                    .nY = 510,
                    .nW = 30,
                    .nH = 30,
                },
                .eBklSize = AX_IVPS_MOSAIC_BLK_SIZE_64,
            },
            {
                .tRect = {
                    .nX = 540,
                    .nY = 540,
                    .nW = 30,
                    .nH = 30,
                },
                .eBklSize = AX_IVPS_MOSAIC_BLK_SIZE_64,
            },
            {
                .tRect = {
                    .nX = 570,
                    .nY = 570,
                    .nW = 30,
                    .nH = 30,
                },
                .eBklSize = AX_IVPS_MOSAIC_BLK_SIZE_64,
            },
            {
                .tRect = {
                    .nX = 600,
                    .nY = 600,
                    .nW = 30,
                    .nH = 30,
                },
                .eBklSize = AX_IVPS_MOSAIC_BLK_SIZE_64,
            },
            {
                .tRect = {
                    .nX = 630,
                    .nY = 630,
                    .nW = 30,
                    .nH = 30,
                },
                .eBklSize = AX_IVPS_MOSAIC_BLK_SIZE_64,
            },
            {
                .tRect = {
                    .nX = 660,
                    .nY = 660,
                    .nW = 30,
                    .nH = 30,
                },
                .eBklSize = AX_IVPS_MOSAIC_BLK_SIZE_64,
            },
            {
                .tRect = {
                    .nX = 690,
                    .nY = 690,
                    .nW = 30,
                    .nH = 30,
                },
                .eBklSize = AX_IVPS_MOSAIC_BLK_SIZE_64,
            },

            {
                .tRect = {
                    .nX = 720,
                    .nY = 720,
                    .nW = 30,
                    .nH = 30,
                },
                .eBklSize = AX_IVPS_MOSAIC_BLK_SIZE_64,
            },
            {
                .tRect = {
                    .nX = 750,
                    .nY = 750,
                    .nW = 30,
                    .nH = 30,
                },
                .eBklSize = AX_IVPS_MOSAIC_BLK_SIZE_64,
            },
            {
                .tRect = {
                    .nX = 780,
                    .nY = 780,
                    .nW = 30,
                    .nH = 30,
                },
                .eBklSize = AX_IVPS_MOSAIC_BLK_SIZE_64,
            },
            {
                .tRect = {
                    .nX = 810,
                    .nY = 810,
                    .nW = 30,
                    .nH = 30,
                },
                .eBklSize = AX_IVPS_MOSAIC_BLK_SIZE_64,
            },
            {
                .tRect = {
                    .nX = 840,
                    .nY = 840,
                    .nW = 30,
                    .nH = 30,
                },
                .eBklSize = AX_IVPS_MOSAIC_BLK_SIZE_64,
            },
            {
                .tRect = {
                    .nX = 870,
                    .nY = 870,
                    .nW = 30,
                    .nH = 30,
                },
                .eBklSize = AX_IVPS_MOSAIC_BLK_SIZE_64,
            },
            {
                .tRect = {
                    .nX = 900,
                    .nY = 900,
                    .nW = 30,
                    .nH = 30,
                },
                .eBklSize = AX_IVPS_MOSAIC_BLK_SIZE_64,
            },
            {
                .tRect = {
                    .nX = 930,
                    .nY = 930,
                    .nW = 30,
                    .nH = 30,
                },
                .eBklSize = AX_IVPS_MOSAIC_BLK_SIZE_64,
            },
        };

        ret = SAMPLE_IVPS_DrawMosaic(ptArg->nEngineId, &pGrp->tFrameInput, tMosaic, 32, pGrp->pFilePath, ptArg->pCaseId);
        if (ret)
        {
            ALOGE("SAMPLE_IVPS_Mosaic failed, ret=0x%x.", ret);
            goto error1;
        }
    }

    if (ptArg->bOsd && ptArg->pOverlayInfo)
    {
        AX_OSD_BMP_ATTR_T arrBmp[10] = {0};
        /* get overlay info */
        char *pOverlayFile = FrameInfoGet(ptArg->pOverlayInfo, &tOverlay);
        FrameBufGet(0, &tOverlay, pOverlayFile);
        for (i = 0; i < 10; i++) {
            arrBmp[i].u64PhyAddr = tOverlay.u64PhyAddr[0];
            arrBmp[i].u32BmpWidth = tOverlay.u32Width;
            arrBmp[i].u32BmpHeight = tOverlay.u32Height;
            arrBmp[i].enRgbFormat = tOverlay.enImgFormat;
            arrBmp[i].u32DstXoffset = 280 * i;
            arrBmp[i].u32DstYoffset = 110 * i;
            arrBmp[i].u16Alpha = 255;
            arrBmp[i].bColorInv = AX_TRUE;
            if (i % 2 ==0){
                arrBmp[i].bColorInv = AX_FALSE;
            }
            arrBmp[i].u32Color = 0xff0000;
            arrBmp[i].u32ColorInv = 0x0000ff;
            arrBmp[i].u32ColorInvThr = 0x00FF00;
            if (i % 3 ==0){
                arrBmp[i].u32Color = 0x00FF00;
                arrBmp[i].u32ColorInv = 0x440000;
                arrBmp[i].u32ColorInvThr = 0x0000AA;
            }
        }
        ret = AX_IVPS_DrawOsd(ptArg->nEngineId, &pGrp->tFrameInput, arrBmp, 10, pGrp->pFilePath, ptArg->pCaseId);
        if (ret)
        {
            ALOGE("SAMPLE_IVPS_DrawOsd failed, ret=0x%x", ret);
            goto error1;
        }
    }

    return 0;
error1:
    if (tOverlay.u32BlkId[0])
    {
        ret = AXCL_POOL_ReleaseBlock(tOverlay.u32BlkId[0]);
        if (ret)
        {
            ALOGE("IVPS Release Overlay BlkId fail, ret=0x%x", ret);
        }
    }

    return ret;
}

AX_S32 SAMPLE_IVPS_SyncApiRegion(const IVPS_ARG_T *ptArg, const SAMPLE_IVPS_GRP_T *pGrp,
                                 const SAMPLE_IVPS_REGION_T *ptRegion)
{
    SAMPLE_DrawCover(&pGrp->tFrameInput, &ptRegion->tCover, pGrp->pFilePath, ptArg->pCaseId);
    return 0;
}