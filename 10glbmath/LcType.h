// GalicSoft Game Library Collection. Copyright(C) GALICSOFT. All rights reserved.
//
// Type definition.
//
// platform identity directive
//   windows : _MSC_VER, _PC_WINDOW_
//   linux   : _PC_LINUX_
//   iphone  : __IOS__
//   android : __AOS__
//   tizen   : __TIZEN__
//
// data model : LP64(int-32, long-64, long long-64, pointer-64)
//    a long type is not compatible. If possible, use int and long long type.
//
////////////////////////////////////////////////////////////////////////////////

#if defined(_WIN32)
  #ifndef _PC_WINDOW_
    #define _PC_WINDOW_ 0x0201
  #endif
#endif

#if defined(ANDROID) || defined(__ANDROID__)
  #ifndef __AOS__
    #define __AOS__ 0x0A02
  #endif
#endif

#if defined(__APPLE__) || defined(__MACOSX__)
  #ifndef __IOS__
    #define __IOS__ 0x0102
  #endif
#endif

#if defined(_PC_WINDOW_)
  #pragma once
  #pragma warning(disable:4351)
#endif

#ifndef _LcType_H_
#define _LcType_H_

//++5++++1+++++++++2+++++++++3+++++++++4+++++++++5+++++++++6+++++++++7+++++++++8
// type

typedef	int                    INT       ;						// basic type for Compatibility to microsoft....
typedef	signed int             INT32     ;
typedef	unsigned int           UINT32    ;
typedef	signed char            INT8      ;
typedef	unsigned char          UINT8     ;
typedef	signed short int       INT16     ;
typedef	unsigned short int     UINT16    ;
typedef	short int              SHORT     ;
typedef	unsigned short int     USHORT    ;
typedef long long              LLONG     ;						// 64bit long
typedef long long              llong     ;						// 64bit long

#if !defined(__IOS__)
  typedef int                  BOOL      ;
#endif

typedef float                  FLOAT     ;
typedef double                 DOUBLE    ;
typedef	unsigned char          BYTE      ;
typedef	signed char            SCHAR     ;
typedef unsigned short         MCHAR     ;						// wide or Multi byte character
typedef unsigned short*        MSTR      ;						// wide or Multi byte string
typedef const unsigned short*  COMSTR    ;						// wide or Multi byte string
typedef	unsigned int           UINT      ;
typedef long long              DINT      ;
typedef unsigned long long     UDINT     ;
typedef unsigned long long     UDMAXINT  ;

typedef	unsigned short         WORD      ;
#if defined(_PC_WINDOW_)										// double word
  typedef unsigned long        DWORD     ;
#else
  typedef unsigned int         DWORD     ;
#endif
typedef	unsigned long long     QWORD     ;						// q-word

typedef unsigned int           TIME32    ;						// saved time
typedef DINT                   TIME64    ;						// 64bit time
typedef DINT                   LCTIME    ;						//
typedef unsigned int           SIZE32    ;						// saved buffer length
typedef unsigned int           SIZE32_T  ;						// size_t 32bit only
typedef DINT                   INT64     ;						// 64bit int
typedef UDINT                  UINT64    ;						// 64bit unsigned int
typedef void*                  PVOID     ;						// void pointer
typedef void*                  LPVOID    ;						// void pointer
typedef const void*            CPVOID    ;						// const void pointer
typedef unsigned char*         B_BUF     ;						// unsigned buffer pointer
typedef char*                  C_BUF     ;						// buffer pointer
typedef void*                  V_BUF     ;						// void buffer pointer
typedef LLONG                  L_PTR     ;						// 64bit long pointer for windows
typedef LLONG                  L_RST     ;						// lc 64bit result
typedef LLONG*                 PL_PTR    ;

typedef char*                  CSTR      ;						// for string
typedef const char*            COSTR     ;						// for string
typedef const char* const      CCSTR     ;						// for return const string
typedef const char             COCHAR    ;						// for string
typedef const unsigned char    COBYTE    ;						// for buffer

typedef unsigned char          EXRB      ;						// unsigned character buffer
typedef char                   EXRC      ;						// character buffer
typedef int                    EXRI      ;						// int 32
typedef long long              EXRL      ;						// int 64
typedef float                  EXRF      ;						// float
typedef double                 EXRD      ;						// double
typedef char*                  EXRT      ;						// text

typedef INT32                  DECIIMIL  ;						// 32bit 1/10,000
typedef INT32                  CENTIMIL  ;						// 32bit 1/100,000
typedef INT32                  MICRO     ;						// 32bit 1/1,000,000
typedef BYTE                   COLOR8    ;						//  8bit color
typedef WORD                   COLOR16   ;						// 16bit color
typedef UINT                   COLOR32   ;						// 32bit argb 0xAARRGGBB color
typedef UDINT                  COLOR64   ;						// 64bit color
typedef int                    FIXF      ;						// fixed float for mobile solution.
typedef short int              FIXH      ;						// fixed half float for mobile solution.
typedef	PVOID                  LC_HANDLE ;						// for mobile device
typedef INT                    LC_SOCKET ;						// socket descriptor
typedef	UINT                   LC_STATE  ;						// device state.
typedef	INT                    LC_DEVICE ;						// device handle
typedef UINT                   LC_VB     ;						// VBO(Vertex Buffer Object)
typedef UINT                   LC_IB     ;						// IBO(Index Buffer Object == VBO)

typedef UINT                   GL_SHADER ;						// OpenGL shader object
typedef UINT                   GL_PROGRAM;						// OpenGL shader Program

typedef unsigned int           LC_UINT   ;
typedef unsigned int           LC_WPARAM ;
typedef unsigned long          LC_LPARAM ;
typedef unsigned long          LC_LRESULT;

#if !defined(_PC_WINDOW_)										// Handle for Microsoft windows and X-window system
  struct HWND__ { int unused; };
#endif

#if defined(_PC_WINDOW_)
   typedef struct HWND__      *LC_HWND  ;

#elif defined(_PC_LINUX_)

  typedef struct tagHNWD {
	  LC_HANDLE w, d, n, t;										// window, display, info, temp
  } *LC_HWND;

#else
  typedef void*                LC_HWND  ;

#endif

#if defined(_PC_WINDOW_)
  #pragma pack(push, 1)
#endif

  typedef struct _LCUUID {										// 128 uuid number
	UINT32	d1;
    USHORT	d2;
    USHORT	d3;
    BYTE	d4[8];
}
#if defined(_PC_WINDOW_)
  #pragma pack(pop)
#else
  __attribute__((packed))
#endif
LCUUID;


//++5++++1+++++++++2+++++++++3+++++++++4+++++++++5+++++++++6+++++++++7+++++++++8
// macro

#if defined(_PC_WINDOW_)										// define callback
  #ifndef LC_CALLBACK
    #define LC_CALLBACK        __stdcall
  #endif

  #ifndef LC_WINAPI
    #define LC_WINAPI          __stdcall
  #endif

#else
  #ifndef LC_CALLBACK
    #define LC_CALLBACK
  #endif

  #ifndef LC_WINAPI
    #define LC_WINAPI
  #endif

#endif


#ifndef CONST													// const and null
  #define CONST   const
#endif

#ifndef NULL
  #define NULL    0
#endif

#ifndef L_NULL
  #define L_NULL  0
#endif

#ifndef L_NIL
  #define L_NIL	(LC_HANDLE)0
#endif

#ifndef LC_OVERRIDE												// c++11 override
  #define LC_OVERRIDE override
#endif

#ifndef LC_INLINE												// inline
  #if defined(_PC_WINDOW_)
    #define LC_INLINE __inline
  #else
    #define LC_INLINE inline __attribute__ ((always_inline))
  #endif
#endif


#ifndef   LC_SUCCEEDED											// succeeded/failed macro
  #define LC_SUCCEEDED(hr) ((DINT)(hr) >= 0)
  #define LC_FAILED(hr)    ((DINT)(hr) <  0)
#endif


#define lc_max(a,b)  (((a) > (b)) ? (a) : (b))					// Minimum and maximum macros
#define lc_min(a,b)  (((a) < (b)) ? (a) : (b))


#ifndef DCL_INTERFACE											// Declare the Interface
  #define DCL_INTERFACE struct
#endif

#ifndef DCL_CLASS_DESTROYER
  #define DCL_CLASS_DESTROYER( CLASS_NAME ) \
  virtual ~CLASS_NAME(){}
#endif

#ifndef CONF													// const return function
  #define CONF(P) const P const
#endif

#ifndef LC_TYPE_CAST											// casting for clang++
	#define LC_TYPE_CAST(DEST_TYPE, v) (DEST_TYPE)(UDMAXINT)v
#endif


//++5++++1+++++++++2+++++++++3+++++++++4+++++++++5+++++++++6+++++++++7+++++++++8
// constant

#define LC_CMD_DEFAULT     (COSTR)"GLC_DEFAULT"

enum ELC_CONSTANT												// GLC basic constant
{
	LC_SDK_VER               =  0x00030210,						// VERSION: GLC Library Version: 03.13.10
	LC_SDK_DATE              =  0x20161218,						// VERSION: Last Build Date 2016-12-18

	LC_DEADBEEF              =  0xDeadBeef,						// COMMON: crash or deadlock
	LC_BAADFOOD              =  0xBaadF00d,						// COMMON: indicate uninitialised allocated with LocalAlloc(LMEM_FIXED) on microsoft system
	LC_FeeeFeee              =  0xFeeeFeee,						// COMMON: microsoft, afeter heep released
	LC_TRUE                  =  1         ,						// COMMON: TRUE
	LC_FALSE                 =  0         ,						// COMMON: FALSE

	LC_FTYPE_FOLDER          =  0x00000010,						// FILE TYPE: folder
	LC_FTYPE_FILE            =  0x00000020,						// FILE TYPE: file
	LC_FTYPE_SYS             =  0x00000004,						// FILE TYPE: system
	LC_FILE_BEGIN            =  0x00000000,						// FILE POINER OFFSET: SEEK_SET,
	LC_FILE_CURRENT          =  0x00000001,						// FILE POINER OFFSET: SEEK_CUR,
	LC_FILE_END              =  0x00000002,						// FILE POINER OFFSET: SEEK_END,

	LC_FILE_ACCESS_RW        =  0x00000006,						// FILE Access type read and write RW_OK
	LC_FILE_ACCESS_R         =  0x00000004,						// FILE Access type read           R_OK
	LC_FILE_ACCESS_W         =  0x00000002,						// FILE Access type write          W_OK
	LC_FILE_ACCESS_X         =  0x00000001,						// FILE Access type execute        X_OK
	LC_FILE_ACCESS_F         =  0x00000000,						// FILE Access type exist          F_OK

	LC_STRG_UNKNOWN          =  0x00000000,						// resource storage type is not defined
	LC_STRG_FILE             =  0x00000001,						// resource storage type file
	LC_STRG_BUF              =  0x00000002,						// resource storage type memory buffer
	LC_STRG_READONLY         =  0x00000010,						// storage readonly folder
	LC_STRG_INTERNAL         =  0x00000020,						// storage internal folder
	LC_STRG_EXTERNAL         =  0x00000040,						// storage external folder
	LC_STRG_ALL              =  0x00000070,						// storage all folder

	LC_UNKNOWN               =  0x00000000,						// IUPUT/OUTPUT: unknown or not defined
	LC_DEFAULT               =  ((UINT)-1),						// IUPUT/OUTPUT: default value less then zero to failed process
	LC_FINISH                =  0x7FFFFFFF,						// IUPUT/OUTPUT: finish or end state.
	LC_INFINITE              =  -1        ,						// IUPUT/OUTPUT: finite max bit(all bits 1)
	LC_DEFAULT_NAME          =  160       ,						// IUPUT/OUTPUT: Max Name Length

	LC_OK                    =  0x00000000,						// PROCESSING RESULT: Succeeded default value
	LC_ALREADY_OPENED        =  0x00000001,						// PROCESSING RESULT: opened at prvious process
	LC_EFAIL                 = -1         ,						// PROCESSING RESULT: Failed default value
	LC_INVALID_CALL          =  0xFFFF0001,						// PROCESSING RESULT: Invalid function or device called
	LC_CANNT_ALLOC           =  0xFFFF0002,						// PROCESSING RESULT: resource alloc failed
	LC_CANNT_FIND_RESOURCE   =  0xFFFF0003,						// PROCESSING RESULT: resource find failed
	LC_NOT_IMPLEMENTED       =  0xFFFF0004,						// PROCESSING RESULT: function or module is not implemented
	LC_CANNT_ACQUIRE_DEVICE  =  0xFFFF0005,						// PROCESSING RESULT: device/driver acquire failed
	LC_INVALID_RESOURCE      =  0xFFFF0006,						// PROCESSING RESULT: resource spoiled or not matched
	LC_DEPRECATED            =  0xFFFF0010,						// PROCESSING RESULT: deprecated
	LC_INVALID_DEVICE        = -1         ,						// PROCESSING RESULT: default device error

	LC_MAX_PATH              =   520      ,						// DICTORY/FILE: short length for relative path
	LC_MAX_PATH2             =   640      ,						// DICTORY/FILE: long length for absolute path
	LC_MAX_PATHW             =  1024      ,						// DICTORY/FILE: for temp
	LC_MAX_DRIVE             =    16      ,						// DICTORY/FILE: drive name
	LC_MAX_DIR               =   520      ,						// DICTORY/FILE: directory name
	LC_MAX_FNAME             =   520      ,						// DICTORY/FILE: file name
	LC_MAX_EXT               =    32      ,						// DICTORY/FILE: file extension
	LC_MAX_BUF               = 16384      ,						// WORK BUF size

	LC_RSC_GPACK             =  0x4B435047,						// RESOURCE: G-pack Resource ID = "KCPG" <-GPCK: Little endian
	LC_RSC_GPACK_            =  0x4750434B,						// RESOURCE: G-pack Resource ID = "GPCK"
	LC_RSC_VER               =  0x20100000,						// RESOURCE: version
	LC_RSC_FNAME             =  128       ,						// RESOURCE: file name in resource
	LC_RSC_HEADER            =  128       ,						// RESOURCE: file header offset
	LC_RSC_NAME              =   64       ,						// RESOURCE: resource name or id max length
	LC_RSC_SID               =   40       ,						// RESOURCE: short id max length for animation or hierarchy..

	LC_SRC_T_NONE            =  0x00000000,						// SOURCE: from none
	LC_SRC_T_FILE            =  0x00000001,						// SOURCE: from file
	LC_SRC_T_BUF             =  0x00000002,						// SOURCE: from simple buffer
	LC_SRC_T_MEM             =  0x00000004,						// SOURCE: from memory(read from file)
	LC_SRC_T_RSC             =  0x00000008,						// SOURCE: from resource on windows system

	LC_OBJ_UNKNOWN           =  LC_UNKNOWN,						// RESOURCE OBJECT: UnKnown or undefined Object
	LC_OBJ_DEVICE            =  0x00002001,						// RESOURCE OBJECT: 3d opengl or direct3d Device
	LC_OBJ_SPRITE            =  0x00002002,						// RESOURCE OBJECT: 2d Sprite
	LC_OBJ_SPRITE_C          =  0x00002011,						// RESOURCE OBJECT: 2d Sprite Collector
	LC_OBJ_SPRITE_L          =  0x00002012,						// RESOURCE OBJECT: 2d Sprite Layer
	LC_OBJ_BUF               =  0x00002021,						// RESOURCE OBJECT: simple buffer
	LC_OBJ_FONT              =  0x00002022,						// RESOURCE OBJECT: FONT face
	LC_OBJ_FONT_BUF          =  0x00002023,						// RESOURCE OBJECT: FONT buffer
	LC_OBJ_IMAGE             =  0x00002101,						// RESOURCE OBJECT: image
	LC_OBJ_TEXTURE           =  LC_OBJ_IMAGE,					// RESOURCE OBJECT: texture
	LC_OBJ_GUI               =  LC_OBJ_SPRITE,					// RESOURCE OBJECT: Gui
	LC_OBJ_MODEL2D           =  0x00002111,						// RESOURCE OBJECT: 2D Model
	LC_OBJ_TDA               =  0x00002112,						// RESOURCE OBJECT: Tda File
	LC_OBJ_TDM               =  0x00002113,						// RESOURCE OBJECT: Tdm File
	LC_OBJ_TDS               =  0x00002114,						// RESOURCE OBJECT: Tds File
	LC_OBJ_MODEL3D           =  0x00002121,						// RESOURCE OBJECT: Simple, billboard, or unknown 3D model.
	LC_OBJ_SLD               =  0x00002122,						// RESOURCE OBJECT: 3D Solid model
	LC_OBJ_RGD               =  0x00002123,						// RESOURCE OBJECT: 3D Rigid body model
	LC_OBJ_SKN               =  0x00002124,						// RESOURCE OBJECT: 3D Skinning animation model
	LC_OBJ_MDS               =  0x00002125,						// RESOURCE OBJECT: 3D animation assemble model
	LC_OBJ_FBX               =  0x00002126,						// RESOURCE OBJECT: 3D Max FBX File
	LC_OBJ_IMG               =  0x00002200,						// RESOURCE OBJECT: image
	LC_OBJ_BMP               =  0x00002201,						// RESOURCE OBJECT: image bmp
	LC_OBJ_TGA               =  0x00002202,						// RESOURCE OBJECT: image tga
	LC_OBJ_PNG               =  0x00002203,						// RESOURCE OBJECT: image png
	LC_OBJ_JPG               =  0x00002204,						// RESOURCE OBJECT: image jpg
	LC_OBJ_KTX               =  0x00002205,						// RESOURCE OBJECT: image ktx
	LC_OBJ_DDS               =  0x00002206,						// RESOURCE OBJECT: image dds dxt1, dxt3, dxt5
	LC_OBJ_PVR               =  0x00002207,						// RESOURCE OBJECT: image pvr
	LC_OBJ_GPT               =  0x00002210,						// RESOURCE OBJECT: image gptf
	LC_OBJ_RNT               =  0x00002220,						// RESOURCE OBJECT: render target
	LC_OBJ_MEDIA             =  0x00002300,						// RESOURCE OBJECT: MEDIA
	LC_OBJ_AUDIO             =  0x00002311,						// RESOURCE OBJECT: sound
	LC_OBJ_VIDEO             =  0x00002312,						// RESOURCE OBJECT: video
	LC_OBJ_WAV               =  0x00002321,						// RESOURCE OBJECT: wave
	LC_OBJ_MIDI              =  0x00002322,						// RESOURCE OBJECT: midi
	LC_OBJ_OGG               =  0x00002323,						// RESOURCE OBJECT: ogg
	LC_OBJ_MP3               =  0x00002324,						// RESOURCE OBJECT: mp3
	LC_OBJ_GPS               =  0x00002325,						// RESOURCE OBJECT: gpsf
	LC_OBJ_NSD               =  0x00002330,						// RESOURCE OBJECT: Non-standard data or native.
	LC_OBJ_CSV               =  0x00002331,						// RESOURCE OBJECT: csv
	LC_OBJ_XML               =  0x00002332,						// RESOURCE OBJECT: XML
	LC_OBJ_INPUT             =  0x00002300,						// RESOURCE OBJECT: Input
	LC_OBJ_SCRIPT            =  0x00002501,						// RESOURCE OBJECT: Script file
	LC_MEDIA_MINE            =  0x00001000,						// Media type: mine
	LC_SND_PCM               =  0x00001001,						// Sound Format: pcm
	LC_DSHOW_EVENT           =  0x0400+0x77BB,					// Direct show event. 0x0400+0x77BB = 0x7BBB
	LC_MAX_RAND              =  0x7FFFFFFE,						// Maximum uniform random number
	LC_MAX_MTL               =   16       ,						// Maximum material count
	LC_MAX_BONE              = 1024       ,						// Maximum skinning bone number for weighting and indexing
	LC_MAX_BLEND_BONE        =   46       ,						// Maximum blending bone number in hardware acceleration.
	LC_MAX_PARSE_LINE        =  512       ,						// Maximum parsing line for text data.
	LC_MAX_READ_BUF          = 1024       ,						// Maximum read buffer size
	LC_MAX_STRING            =  0x00008000,						// Maximum string length
	LC_MAX_STR_MSG           = 1200       ,						// Maximum string message
	LC_MAX_TOKEN_BUF         = 2048       ,						// Maximum tokenizer buffer size
	LC_MAX_EGL_CONFIG        =   16       ,						// SYSTEM: Max EGL Configuration
	LC_MAX_KEY               =  256       ,						// SYSTEM: Keypad or keyboard
	LC_MAX_TSB               =   16       ,						// SYSTEM: touch screen
	LC_MAX_SENSOR            =   32       ,						// SYSTEM: sensor
	LC_ZIP_NO_COMPRESSION    =    0       ,						// SYSTEM: no compression
	LC_ZIP_BEST_SPEED        =    1       ,						// SYSTEM: best speed
	LC_ZIP_BEST_COMPRESSION  =    9       ,						// SYSTEM: best size
	LC_ZIP_LEVEL             =    4       ,						// SYSTEM: lc zip default level
	LC_ZIP_TEMP_BUF_SIZE     = 2048       ,						// SYSTEM: zip temporary buffer size
	LC_LOOP_DONE             =  LC_FINISH ,						// Loop state: data loop done state
	LC_LOOP_INFINITE         =  LC_INFINITE,					// Loop state: data loop infinite state
	LC_LOOP_STOP             =  0x00000000,						// Loop state: data loop stop state
	LC_LOOP_RESET            =  0x00000001,						// Loop state: data loop reset state
	LC_LOOP_SET              =  0x00000002,						// Loop state: data loop set state
	LC_LOOP_PAUSE            =  0x00000003,						// Loop state: data loop pause state
	LC_LOOP_PLAY             =  0x00000004,						// Loop state: data loop playing state
	LC_LOOP_MAX_COUNT        =  0x0000FFFF,						// limit while loop counter some state
	LC_IP_ADDR               =   20       ,						// Network: ip address length
	LC_HOST_NAME             =  320       ,						// Network: network host name

	LC_GL_T_UBYTE            =  0x1401    ,
	LC_GL_F_RGB              =  0x1907    ,
	LC_GL_F_RGBA             =  0x1908    ,
};

enum LC_VAR														// GLC Engine stored global variable type. set/get use the LcVar_{} functions
{
	LC_VAR_TYPE_ENGINE       =  0,								// GLOBAL VARIABLE: GLC Engine type of mobile or pc Platform LC_VAR_SYS_{WIN32|LINUX|AOS|IOS|BADA}
	LC_VAR_VERSION           ,									// GLOBAL VARIABLE: GLC Version(_n0) and date(_n1)
	LC_VAR_LOG               ,									// GLOBAL VARIABLE: Log Options
	LC_VAR_TIME              ,									// GLOBAL VARIABLE: Application time, frame rate
	LC_VAR_GRAPHICS          ,									// GLOBAL VARIABLE: Graphic type _n0(LC_VAR_GRAPHICS_NONE|GL|DX) _n1:Major _n2:Minor1 _n3:minor2 _n3:Minor3 _n4:minor4

	LC_VAR_REF_COUNT         ,									// GLOBAL VARIABLE: created instance count
	LC_VAR_DISPLAY           ,									// GLOBAL VARIABLE: device display r, g, b, a, depth, stencil bit, width, height and clear mode. + application screen position(i[0], i[1]), screen width, height(i[2], i[3]) and scaling mode to display
	LC_VAR_WIN_RECT          ,									// GLOBAL VARIABLE: window position, size, ratio on display area. float data
	LC_VAR_WIN_HANDLE        ,									// GLOBAL VARIABLE: window handle(_p0) for default 3d device and application etc...
	LC_VAR_GLDEV			 ,									// GLOBAL VARIABLE: opengl device device(_p0), Egl Display handle(_p1), Egl Surface handle(_p2), Egl Context handle(_p3), and GL_MAX_TEXTURE_IMAGE_UNITS number

	LC_VAR_GUI               ,									// GLOBAL VARIABLE: just for gui layer object.
	LC_VAR_SPRITE            ,									// GLOBAL VARIABLE: just for sprite object. default index 0: gui, 1: 2d model sprite saved at device creating
	LC_VAR_APP_MAIN          ,									// GLOBAL VARIABLE: Application main instance pointer
	LC_VAR_APP_INPUT         ,									// GLOBAL VARIABLE: Application input instance pointer
	LC_VAR_APP_NET           ,									// GLOBAL VARIABLE: Application network instance pointer
	LC_VAR_APP_CAMERA        ,									// GLOBAL VARIABLE: Application camera instance pointer

	LC_VAR_SYS_NONE          =  0x00000000,						// GLOBAL VARIABLE: PLATFORM: for LcVar_Platform()
	LC_VAR_SYS_WIN32         =  0x00000001,						// GLOBAL VARIABLE: PLATFORM: Windows32
	LC_VAR_SYS_LINUX         =  0x00000002,						// GLOBAL VARIABLE: PLATFORM: linux pc
	LC_VAR_SYS_AOS           =  0x00000003,						// GLOBAL VARIABLE: PLATFORM: android
	LC_VAR_SYS_IOS           =  0x00000004,						// GLOBAL VARIABLE: PLATFORM: ios
	LC_VAR_SYS_TIZEN         =  0x00000005,						// GLOBAL VARIABLE: PLATFORM: tizen = old bada.

	LC_VAR_GRAPHICS_NONE     =  0x00000000,						// GLOBAL VARIABLE: GRAPHICS:
	LC_VAR_GRAPHICS_GL       =  0x00000010,						// GLOBAL VARIABLE: GRAPHICS: OpenGL
	LC_VAR_GRAPHICS_DX       =  0x00000020,						// GLOBAL VARIABLE: GRAPHICS: Direct3D

	LC_VAR_DISPLAY_NONE      =  0x00000000,						// GLOBAL VARIABLE: to get the application window size mode
	LC_VAR_DISPLAY_FULL      =  0x00000001,						// GLOBAL VARIABLE: application window size will be scaled to display size.
	LC_VAR_DISPLAY_FIT       =  0x00000002,						// GLOBAL VARIABLE: application window size will be scaled fitting to display vertical or horizontal.
	LC_VAR_DISPLAY_FIX       =  0x00000004,						// GLOBAL VARIABLE: application window size will be scaled fix to display vertical or horizontal. some region will be clammped.
	LC_VAR_DISPLAY_SCN       =  0x00000008,						// GLOBAL VARIABLE: application window size will not be changed.

	LC_VAR_USER_DATA         =  32        ,						// GLOBAL VARIABLE: user defined data starting index
	LC_VAR_TOTAL             =  64        ,
};

enum LC_VAR_SYS
{
	LC_LOG_SERIAL            =  0x00000000,						// LOG: default value. printf or LogCat
	LC_LOG_FILE              =  0x00000001,						// LOG: It will make a glc_log.txt with 'wb' option.
	LC_LOG_MIX               =  0x00000002,						// LOG: log to serial and file.
	LC_LOG_DEBUG             =  0x00000044,						// LOG: debugging       'D'
	LC_LOG_INFO              =  0x00000049,						// LOG: Information     'I'
	LC_LOG_WARNING           =  0x00000057,						// LOG: Warning         'W'
	LC_LOG_ERROR             =  0x00000045,						// LOG: Error           'E'
	LC_LOG_FATAL             =  0x00000041,						// LOG: Fatal           'F'
	LC_LOG_GLC               =  0x00000047,						// LOG: General verbose 'G'

	LC_TIME_ELAPSED          =  0x0000B013,						// TIME: DOUBLE, elapsed millisecond time per on frame. same to LcVar_TimeElapsed
	LC_TIME_STORED           =  0x0000B014,						// TIME: DINT, Stored Time. same to LcVar_TimeStored
	LC_TIME_FRAME            =  0x0000B015,						// TIME: double, Frames per one second. same to LcVar_TimeFrameRate
};

enum LC_STYPE													// Enum simple data type for data base and register
{
	LC_ST_WORD               =  0x00000001,						// SIMPLE TYPE: short int (2byte)
	LC_ST_DWORD              =  0x00000002,						// SIMPLE TYPE: int (4byte). for small number
	LC_ST_QWORD              =  0x00000003,						// SIMPLE TYPE: double int (8byte)
	LC_ST_DOUBLE             =  0x00000004,						// SIMPLE TYPE: double (8byte) for big number
	LC_ST_TEXT               =  0x00000005,						// SIMPLE TYPE: text(null terminated)
	LC_ST_BOOL               =  0x00000006,						// SIMPLE TYPE: boolean (1byte)
	LC_ST_FLOAT              =  0x00000007,						// SIMPLE TYPE: float (4byte)
	LC_ST_DECIIMIL           =  0x00000008,						// SIMPLE TYPE: decimilli.  int(4byte) must be multiple 1/10,000
	LC_ST_CENTIMIL           =  0x00000009,						// SIMPLE TYPE: centimilli. int(4byte) must be multiple 1/100,000
	LC_ST_MICRO              =  0x0000000A,						// SIMPLE TYPE: micro.      int(4byte) must be multiple 1/1,000,000
	LC_ST_VARIANT            =  0x00000010,						// SIMPLE TYPE: not defined data
};

#define LC_STYPE_WORD      ((COSTR)"tiny")						/* schema type 16bit int number */
#define LC_STYPE_DWORD     ((COSTR)"small")						/* schema type 32bit int number */
#define LC_STYPE_QWORD     ((COSTR)"big")						/* schema type 64bit int number */
#define LC_STYPE_DOUBLE    ((COSTR)"number")					/* schema type 64bit floating number */
#define LC_STYPE_TEXT      ((COSTR)"text")						/* schema type string */
#define LC_STYPE_BOOL      ((COSTR)"bool")						/* schema type boolean */
#define LC_STYPE_FLOAT     ((COSTR)"float")						/* schema type 32bit floating number */
#define LC_STYPE_DECIIMIL  ((COSTR)"decimil")					/* schema type 32bit int decimilli number */
#define LC_STYPE_CENTIMIL  ((COSTR)"centimil")					/* schema type 32bit int centimilli number */
#define LC_STYPE_MICRO     ((COSTR)"micro")						/* schema type 32bit int micro number */

//storage tag
#define LC_STRG_OFFSET  4
#define LC_STRG_TAG_EXT ((COSTR)"[+]:")							/* external folder: read and writable */
#define LC_STRG_TAG_INT ((COSTR)"[#]:")							/* internal folder for android */
#define LC_STRE_TAG_RDD ((COSTR)"[-]:")							/* read only folder for ios and aos */
#define LC_STRE_TAG_BUF ((COSTR)"[*]:")							/* just buffer */

// glsl precision
#if !defined(_PC_WINDOW_) && !defined(_PC_LINUX_)
#ifndef GLSL_ENABLE_PRECISION
	#define GLSL_ENABLE_PRECISION
#endif
#endif

#ifdef GLSL_ENABLE_PRECISION
	#define PRECI_HIP	" mediump "
	#define PRECI_MIP	" mediump "
	#define PRECI_LOP	" lowp "
#else
	#define PRECI_HIP	" "
	#define PRECI_MIP	" "
	#define PRECI_LOP	" "
#endif

// alignment
#if !defined(PACKED_ALIGNED)

  #if !defined(_PC_WINDOW_)
    #define PACKED_ALIGNED   __attribute__ ((packed))
    #define PACKED_ALIGNED_2 __attribute__ ((packed,aligned(2)))
    #define PACKED_ALIGNED_4 __attribute__ ((packed,aligned(4)))
    #define PACKED_ALIGNED_8 __attribute__ ((packed,aligned(8)))
  #else
    #define PACKED_ALIGNED
    #define PACKED_ALIGNED_2
    #define PACKED_ALIGNED_4
    #define PACKED_ALIGNED_8
  #endif

#endif

// c++version
#ifndef _CPP_VER_
  #if defined(_MSVC_LANG) && (201402L <= _MSVC_LANG)
    #define _CPP_VER_ 14
  #else
    #define _CPP_VER_ 11
  #endif
#endif


#endif	//_LcType_H_

