#pragma once
#define WIN32_LEAN_AND_MEAN

#include <string>
#include "CoreDef.hpp"
// ReSharper disable CppUnusedIncludeDirective
#include <windows.h>
// ReSharper restore CppUnusedIncludeDirective

#define E5_UNUSED_PARAM(param) param
#define E5_FALLTHROUGH
#define E5_TYPE_TO_STRING(param) typeid(param).name();
#define E5_UNUSED_ARGS(...) Engine5::UnusedParameter(__VA_ARGS__)
#define E5_ASSERT(exp, str) if(Engine5::Assert((exp),(str), \
  __FUNCTION__,__FILE__, __LINE__))                         \
{                                                           \
    DebugBreak();                                           \
}

//#ifdef _DEBUG
//#ifndef DBG_NEW
//#define DBG_NEW new (_NORMAL_BLOCK , __FILE__ , __LINE__ )
//#define new DBG_NEW
//#endif // !DBG_NEW
//#endif // _DEBUG


#define E5_LEAK_CHECKS(x) \
  _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);\
  _CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);\
  _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);\
  _CrtSetBreakAlloc((x));

namespace Engine5
{
    std::wstring StringToWString(const std::string& str);
    std::string  WStringToString(const std::wstring& wstr);

    template <typename First, typename... Rest>
    void UnusedParameter(const First& first, const Rest& ...rest);

    template <typename T>
    void UnusedParameter(const T& t);

    void UnusedParameter();

    bool Assert(int expression, const std::string& output_message, const std::string& function_name, const std::string& file_name, unsigned line_number);
}

#include "CoreUtility.inl"
// ReSharper disable once CppUnusedIncludeDirective
#include "CoreUtilityTemplate.cpp"
