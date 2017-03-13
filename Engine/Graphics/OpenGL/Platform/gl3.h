#include "Engine/Toolkit/Platform/Platform.h"


//--------------------------------------------------------------------------------------------------
#ifdef is_platform_android
//--------------------------------------------------------------------------------------------------

#include <GLES3/gl3.h>

//--------------------------------------------------------------------------------------------------
#elif defined(is_platform_ios)
//--------------------------------------------------------------------------------------------------

#include <OpenGLES/ES3/gl.h>

//--------------------------------------------------------------------------------------------------
#endif
//--------------------------------------------------------------------------------------------------
