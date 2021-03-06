#include "Logger.h"
#include "Globals.h"
#include <map>

// Instantiate static variables
std::map <std::string, LogFile> Logger::LogFileMap;
std::vector <LogEntry> Logger::LogData;
int Logger::ConsoleLogFlags = LogAll;
std::string Logger::DefaultLogPath;
unsigned int Logger::MaximumLogCount;

void Logger::Init(int _ConsoleLogFlags, std::string _DefaultLogPath, unsigned int _MaximumLogCount)
{
	ConsoleLogFlags = _ConsoleLogFlags;
	DefaultLogPath = _DefaultLogPath;
	MaximumLogCount = _MaximumLogCount;

	Globals::CreateDirectoryIfNull(DefaultLogPath);
}

void Logger::OpenLogFile(std::string FileName, int LogFlags, std::ios_base::openmode Mode, std::string CustomFilePath)
{
	try
	{
		std::string Path;
		if (CustomFilePath == "DEFAULT")
		{
			Path = DefaultLogPath;
		}
		else
		{
			Path = CustomFilePath;
		}
		for (auto& i : LogFileMap)
		{
			if (i.first == FileName)
			{
                LogError("{} already exists. New log file not created.", FileName);
				return;
			}
		}
		LogFileMap.insert_or_assign(FileName, LogFile(Path + FileName, LogFlags, Mode));
		LogFileMap[FileName].File.exceptions(std::ios::failbit | std::ios::badbit);
		LogFileMap[FileName].File.open(Path + FileName, Mode);
		LogFileMap[FileName].File << GetTimeString(false) << "[Info] " << "Start of " << FileName << "\n";
		LogFileMap[FileName].File << std::flush;
	}
	catch(std::ios_base::failure& Ex)
	{
		std::string ExceptionInfo = Ex.what();
		ExceptionInfo += " \nstd::ios_base::failure when opening ";
		ExceptionInfo += FileName;
		ExceptionInfo += ", Additional info: ";
		ExceptionInfo += "Error code: ";
		ExceptionInfo += Ex.code().message();
		ExceptionInfo += ", File: ";
		ExceptionInfo += __FILE__;
		ExceptionInfo += ", Function: ";
		ExceptionInfo += __func__;
		throw(std::exception(ExceptionInfo.c_str()));
	}
	catch (std::exception& Ex)
	{
		std::string ExceptionInfo = Ex.what();
		ExceptionInfo += " \nstd::exception when opening ";
		ExceptionInfo += FileName;
		ExceptionInfo += ", Additional info: ";
		ExceptionInfo += "File: ";
		ExceptionInfo += __FILE__;
		ExceptionInfo += ", Function: ";
		ExceptionInfo += __func__;
		throw(std::exception(ExceptionInfo.c_str()));
	}
	catch (...)
	{
		std::string ExceptionInfo;// = Ex.what();
		ExceptionInfo += " \nDefault exception when opening ";
		ExceptionInfo += FileName;
		ExceptionInfo += ", Additional info: ";
		ExceptionInfo += "File: ";
		ExceptionInfo += __FILE__;
		ExceptionInfo += ", Function: ";
		ExceptionInfo += __func__;
		throw std::exception(ExceptionInfo.c_str());
	}
}

void Logger::CloseLogFile(const const std::string& FileName)
{
	LogFileMap[FileName].File.close();
	LogFileMap.erase(FileName);
}

void Logger::CloseAllLogFiles()
{
	for (auto& i : LogFileMap)
	{
		i.second.File.close();
	}
	LogFileMap.erase(LogFileMap.begin(), LogFileMap.end());
}

void Logger::LogInternal(std::string Message, int LogFlags, bool LogTime, bool NewLine)
{
	std::string TimeString = GetTimeString(false);
	std::string FlagString = GetFlagString(LogFlags);
	LogData.insert(LogData.begin(), LogEntry(FlagString, Message, LogFlags));
	if (ConsoleLogFlags & LogFlags)
	{
		try
		{
			LogFlagWithColor(LogFlags);
			if (LogTime)
			{
				std::cout << TimeString;
			}
			std::cout << " ";
			std::cout << Message;
			if (NewLine)
			{
				std::cout << "\n";
			}
		}
		catch (std::exception& Ex)
		{
			std::string ExceptionInfo = Ex.what();
			ExceptionInfo += " \nstd::exception caught while logging to external console!";
			ExceptionInfo += " Additional info: ";
			ExceptionInfo += "File: ";
			ExceptionInfo += __FILE__;
			ExceptionInfo += ", Function: ";
			ExceptionInfo += __func__;
			LogError("{}\n", ExceptionInfo);
			MessageBoxA(Globals::FindRfgTopWindow(), ExceptionInfo.c_str(), "Failed to log to external console!", MB_OK);
		}
		catch (...)
		{
			std::string ExceptionInfo = "Default exception caught while logging to external console!";
			ExceptionInfo += " Additional info: ";
			ExceptionInfo += "File: ";
			ExceptionInfo += __FILE__;
			ExceptionInfo += ", Function: ";
			ExceptionInfo += __func__;
            LogError("{}\n", ExceptionInfo);
			MessageBoxA(Globals::FindRfgTopWindow(), ExceptionInfo.c_str(), "Failed to log to external console!", MB_OK);
		}
	}
	for (auto& i : LogFileMap)
	{
		try
		{
			if (i.second.LogFlags & LogFlags)
			{
				i.second.File << FlagString;
				if (LogTime)
				{
					i.second.File << TimeString;
				}
				i.second.File << " ";
				i.second.File << Message;
				if (NewLine)
				{
					i.second.File << "\n";
				}
				i.second.File << std::flush;
			}
		}
		catch (std::ios_base::failure& Ex)
		{
			std::string ExceptionInfo = Ex.what();
			ExceptionInfo += " \nstd::ios_base::failure exception caught while logging to ";
			ExceptionInfo += i.second.FilePath;
			ExceptionInfo += ", Additional info: ";
			ExceptionInfo += "Error code: ";
			ExceptionInfo += Ex.code().message();
			ExceptionInfo += "File: ";
			ExceptionInfo += __FILE__;
			ExceptionInfo += ", Function: ";
			ExceptionInfo += __func__;
            LogError("{}\n", ExceptionInfo);
			MessageBoxA(Globals::FindRfgTopWindow(), ExceptionInfo.c_str(), "Failed to log to file!", MB_OK);
		}	
		catch (std::exception& Ex)
		{
			std::string ExceptionInfo = Ex.what();
			ExceptionInfo += " \nstd::exception caught while logging to ";
			ExceptionInfo += i.second.FilePath;
			ExceptionInfo += ", Additional info: ";
			ExceptionInfo += "File: ";
			ExceptionInfo += __FILE__;
			ExceptionInfo += ", Function: ";
			ExceptionInfo += __func__;
            LogError("{}\n", ExceptionInfo);
			MessageBoxA(Globals::FindRfgTopWindow(), ExceptionInfo.c_str(), "Failed to log to file!", MB_OK);
		}
		catch (...)
		{
			std::string ExceptionInfo = "Default exception caught while logging to ";
			ExceptionInfo += i.second.FilePath;
			ExceptionInfo += ", Additional info: ";
			ExceptionInfo += "File: ";
			ExceptionInfo += __FILE__;
			ExceptionInfo += ", Function: ";
			ExceptionInfo += __func__;
            LogError("{}\n", ExceptionInfo);
			MessageBoxA(Globals::FindRfgTopWindow(), ExceptionInfo.c_str(), "Failed to log to file!", MB_OK);
		}
	}
	if (LogData.size() > MaximumLogCount && MaximumLogCount > 0)
	{
		LogData.pop_back();
	}
	
	/*if (LogData.size() >= 500)
	{
		std::cout << "LogData.size(): " << LogData.size() << "\n";
		int TotalMessageSize = 0;
		for (auto i = LogData.begin(); i != LogData.end(); i++)
		{
			TotalMessageSize += i->Message.length();
			TotalMessageSize += i->FlagString.length();
			TotalMessageSize += sizeof(i->Flags);
		}
		int VectorSize = sizeof(std::vector<std::string>);
		std::cout << "Size of vector: " << VectorSize << " Bytes\n";
		std::cout << "Size of all strings" << TotalMessageSize << " Bytes\n";
		TotalMessageSize += VectorSize;
		std::cout << "Total size of both: " << TotalMessageSize << " Bytes | " << TotalMessageSize / 1000 << " KiloBytes\n";
	}*/
	//std::cout << "sizeof std::string vector with 10000 values: " << (sizeof(std::vector<std::string>) + (sizeof(std::string) * 10000)) / 1000 << "kB\n";
}

void Logger::LogFlagWithColor(const int LogFlags)
{
	if (LogFlags & LogType::LogInfo)
	{
		std::cout << "[";
		Globals::SetConsoleAttributes(Globals::ConsoleMessageLabelTextAttributes);
		std::cout << "Info";
		Globals::ResetConsoleAttributes();
		std::cout << "]";
	}
	else if (LogFlags & LogType::LogWarning)
	{
		std::cout << "[";
		Globals::SetConsoleAttributes(Globals::ConsoleWarningTextAttributes);
		std::cout << "Warning";
		Globals::ResetConsoleAttributes();
		std::cout << "]";
	}
	else if (LogFlags & LogType::LogError)
	{
		std::cout << "[";
		Globals::SetConsoleAttributes(Globals::ConsoleErrorTextAttributes);
		std::cout << "Error";
		Globals::ResetConsoleAttributes();
		std::cout << "]";
	}
	else if (LogFlags & LogType::LogFatalError)
	{
		std::cout << "[";
		Globals::SetConsoleAttributes(Globals::ConsoleFatalErrorTextAttributes);
		std::cout << "Fatal Error";
		Globals::ResetConsoleAttributes();
		std::cout << "]";
	}
	else
	{
		//Does nothing if it's LogNone or an undefined value.
	}
	SetConsoleTextAttribute(Globals::ConsoleHandle, Globals::ConsoleDefaultTextAttributes);
}

std::string Logger::GetFlagString(const int LogFlags)
{
	if (LogFlags & LogType::LogInfo)
	{
		return "[Info]";
	}
	else if (LogFlags & LogType::LogWarning)
	{
		return "[Warning]";
	}
	else if (LogFlags & LogType::LogLua)
	{
		return "[Lua]";
	}
	else if (LogFlags & LogType::LogJson)
	{
		return "[Json]";
	}
	else if (LogFlags & LogType::LogError)
	{
		return "[Error]";
	}
	else if (LogFlags & LogType::LogFatalError)
	{
		return "[Fatal Error]";
	}
	else
	{
		return "";
	}
}

void Logger::LogToFile(const std::string& FileName, const std::string& Message, const int LogFlags, const bool LogTime, const bool BypassFlagCheck)
{
	if (!BypassFlagCheck) //Allows things such as using LogInfo for the first message in an error log.
	{
		if (!(LogFileMap[FileName].LogFlags & LogFlags))
		{
			return;
		}
	}
	LogFileMap[FileName].File << GetFlagString(LogFlags);
	if (LogTime)
	{
		LogFileMap[FileName].File << GetTimeString(false);
	}
	LogFileMap[FileName].File << " ";
	LogFileMap[FileName].File << Message << "\n";
}

std::string Logger::GetTimeString(const bool MilitaryTime)
{
	std::time_t t = std::time(0);
	std::tm now{};
	localtime_s(&now, &t);

	std::string DateTime;
	const std::string Year = std::to_string(now.tm_year + 1900);
	const std::string Month = std::to_string(now.tm_mon + 1);
	const std::string Day = std::to_string(now.tm_mday);
	std::string Hour;
	std::string Minutes = std::to_string(now.tm_min);

	if (Minutes.size() == 1) //Changes values such as 1:6 to 1:06
	{
		Minutes.insert(0, 1, '0');
	}
	if (MilitaryTime)
	{
		Hour = std::to_string(now.tm_hour);
	}
	else
	{
		if (now.tm_hour > 11) //PM
		{
			Hour = std::to_string(now.tm_hour - 12);
			DateTime += Hour + ":" + Minutes + " PM, ";
		}
		else //AM
		{
			if (now.tm_hour == 0)
			{
				Hour = "12";
				DateTime += Hour + ":" + Minutes + " AM, ";
			}
			else
			{
				Hour = std::to_string(now.tm_hour);
				DateTime += Hour + ":" + Minutes + " AM, ";
			}
		}
	}
	DateTime += Month + "/" + Day + "/" + Year;
	return "[" + DateTime + "]";
}