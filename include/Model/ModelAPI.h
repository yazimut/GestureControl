#pragma once

#ifdef MODEL_EXPORTS
    #define MODEL_API __declspec(dllexport)
#else
    #define MODEL_API __declspec(dllimport)
#endif // MODEL_EXPORTS
