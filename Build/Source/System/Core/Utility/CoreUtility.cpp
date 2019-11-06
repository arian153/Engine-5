#include "CoreUtility.hpp"
#include <Windows.h>
#include <vector>
#include <sstream>
#include <comdef.h>

namespace Engine5
{
    std::wstring StringToWString(const std::string& str)
    {
        std::wstring convertedString;
        int          requiredSize = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, 0, 0);
        if (requiredSize > 0)
        {
            std::vector<wchar_t> buffer(requiredSize);
            MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, &buffer[0], requiredSize);
            convertedString.assign(buffer.begin(), buffer.end() - 1);
        }
        return convertedString;
    }

    std::string WStringToString(const std::wstring& wstr)
    {
        std::string convertedString;
        int         requiredSize = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, 0, 0, 0, 0);
        if (requiredSize > 0)
        {
            std::vector<char> buffer(requiredSize);
            WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &buffer[0], requiredSize, 0, 0);
            convertedString.assign(buffer.begin(), buffer.end() - 1);
        }
        return convertedString;
    }

    void UnusedParameter()
    {
    }

    bool Assert(int expression, const std::string& output_message, const std::string& function_name, const std::string& file_name, unsigned line_number)
    {
        if (!expression)
        {
            std::stringstream ss;

            /*Set output message*/
            ss << "ASSERTION FAILURE:";
            ss << "\nFile: ";
            ss << file_name;
            ss << "\nLine: ";
            ss << line_number;
            ss << "\nFunction: ";
            ss << function_name;
            ss << "\n\n Description: ";
            ss << output_message;
            ss << "\n\nYES: Break into the Debugger.";
            ss << "\nNO: Exit immediately";

            /*display a message to the user*/
            std::wstring message      = StringToWString(ss.str());
            int          return_value = MessageBox(nullptr, message.c_str(), L"ASSERT!", MB_TASKMODAL | MB_SETFOREGROUND | MB_YESNO | MB_ICONERROR);
            if (return_value == IDYES)
            {
                return true;
            }
            ExitProcess((unsigned)(-1));
        }
        return false;
    }
}
