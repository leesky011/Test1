#include "CCheckTsPacketSize.h"

#ifndef MAX_FASTFETCH_FRAMECOUNT
#define MAX_FASTFETCH_FRAMECOUNT 50
#endif  


#define  COMMON_MEDIA_TS     0
#define  COMMON_MEDIA_AAC    1

FILE * g_fpaacin;
FILE * g_fpaacout;
//extern DRMHANDLE g_engine;

http_live_streaming::vo_http_live_streaming(void)
:m_judgementor(5)
,m_is_first_frame(VO_TRUE)
,m_new_video_file(VO_TRUE)
,m_new_audio_file(VO_TRUE)
,m_bMediaStop(VO_FALSE)
,m_is_pause( VO_FALSE )
,m_recoverfrompause( VO_FALSE )
,m_is_flush(VO_FALSE)
,m_is_seek(VO_FALSE)
{
	m_datacallback_func = 0;
	m_keytag = -1;
	m_drm_eng_handle = NULL;
	m_eventcallback_func = 0;
	m_is_video_delayhappen = FALSE;
	m_is_bitrate_adaptation = FALSE;
	m_audiocounter = 0;
	m_videocounter = 0;
	m_last_big_timestamp = 0;
	m_timestamp_offset = 0;
	m_seekpos = 0;
	m_is_afterseek = FALSE;
	m_mediatype = -1;
	m_is_mediatypedetermine = FALSE;
	m_ptr_audioinfo = 0;
	m_ptr_videoinfo = 0;
	m_last_audio_timestamp = -1;
	m_last_video_timestamp = -1;
	m_download_bitrate = -1;
	m_rightbandwidth = -1;
	m_brokencount = 0;

	memset (&m_tsparser_api, 0, sizeof (PARSER_API));

    memset (&m_aacparser_api, 0, sizeof (PARSER_API));
    
	strcpy (m_szWorkPath, "");
	memset( m_last_keyurl , 0 , 1024 );
	memset( &m_drm_eng , 0 , sizeof( DRM_Callback ) );

	m_adaptationbuffer.set_callback( this , bufferframe_callback );
    m_iCurrentMediaType = COMMON_MEDIA_TS;

	m_pDrmCallback = NULL;
	m_iSeekResult = 0;
	m_bNeedUpdateTimeStamp = VO_FALSE;
	m_iVideoDelayCount = 0;


	LoadWorkPathInfo();
	ResetAllFilters();

    m_iDrmType = (VO_S32)VO_DRMTYPE_Irdeto;
	m_iProcessSize = 0;

	m_pThumbnailList = NULL;

	memset(&m_sHLSUserInfo, 0 ,sizeof(S_HLS_USER_INFO));
    m_drm_eng_handle = NULL;

	//g_fp = fopen( "/sdcard/videodump.h264" , "wb+" );
    //g_fp2 = fopen( "/sdcard/dump.ts" , "wb+" );

	//For Test Ad
	//S_FilterForAdvertisementIn    sFilterTest;
	//sFilterTest.iFilterId = 3;
	//strcpy(sFilterTest.strFilterString, "#EXT-VO-ADINF");

	//Add_AdFilterInfo((void *) &sFilterTest);
	//For Test Ad

    g_fpaacin = fopen( "/sdcard/pureaacin.aac" , "wb+" );
    g_fpaacout = fopen( "/sdcard/pureaacout.aac" , "wb+" );
}


S64 http_live_streaming::GetItem( webdownload_stream * ptr_stream , S32 eReloadType , BOOL is_quick_fetch )
{
	PBYTE ptr_buffer = NULL;
	S32 readed_size = 0;

    
    if( m_pDrmCallback != NULL)
    {
        Prepare_HLSDRM_Process();
    }

    if(m_iCurrentMediaType == COMMON_MEDIA_AAC)
    {
        //DumpAACPureData(0, 0, 1);
    }
    
	m_audiocounter = m_videocounter = 0;
    m_iProcessSize = 0;

	if( (eReloadType != M3U_RELOAD_NULL_TYPE)|| m_iProcessSize == 0 )
	{
		LOGI("get size");
		m_iProcessSize = readbuffer_determinesize( &ptr_buffer , ptr_stream );
		LOGI( "size: %d" , m_iProcessSize );

		if( m_iProcessSize == -1 || m_iProcessSize == 0 )
		{
			LOGE( "Data Fatal Error!" );
			m_iProcessSize = 0;
			ptr_stream->close();

            if(m_pDrmCallback != NULL)
            {
                After_HLSDRM_Process();
            }
			return -1;
		}

		readed_size = m_iProcessSize;
	}
	else
	{
		LOGI("use size: %d" , m_iProcessSize );
		ptr_buffer = new BYTE[m_iProcessSize];
	}

	if(eReloadType != M3U_RELOAD_NULL_TYPE)
	{
		//VOLOGI("reload_ts_parser");
		//reload_ts_parser();
	}

    return (S32)readed;
}