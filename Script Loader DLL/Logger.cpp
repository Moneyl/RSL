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

	CreateDirectoryIfNull(DefaultLogPath);
}

void Logger::OpenLogFile(std::string FileName, int LogFlags, std::ios_base::openmode Mode, std::string CustomFilePath)
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
	for (auto i = LogFileMap.begin(); i != LogFileMap.end(); i++)
	{
		if (i->first == FileName)
		{
			Log(std::string(FileName + " already exists. New log file not created."), LogError);
			return;
		}
	}
	LogFileMap.insert_or_assign(FileName, LogFile(Path + FileName, LogFlags, Mode));
	LogFileMap[FileName].File.open(Path + FileName, Mode);
	LogFileMap[FileName].File << GetTimeString(false) << "[Info] " << "Start of " << FileName << "\n";
}

void Logger::CloseLogFile(std::string FileName)
{
	LogFileMap[FileName].File.close();
	LogFileMap.erase(FileName);
}

void Logger::CloseAllLogFiles()
{
	for (auto i = LogFileMap.begin(); i != LogFileMap.end(); i++)
	{
		i->second.File.close();
	}
	LogFileMap.erase(LogFileMap.begin(), LogFileMap.end());
}

void Logger::Log(std::string Message, int LogFlags, bool LogTime, bool NewLine)
{
	std::string TimeString = GetTimeString(false);
	std::string FlagString = GetFlagString(LogFlags);
	LogData.insert(LogData.begin(), LogEntry(FlagString, Message, LogFlags));
	if (ConsoleLogFlags & LogFlags)
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
	for (auto i = LogFileMap.begin(); i != LogFileMap.end(); i++)
	{
		if (i->second.LogFlags & LogFlags)
		{
			i->second.File << FlagString;
			if (LogTime)
			{
				i->second.File << TimeString;
			}
			i->second.File << " ";
			i->second.File << Message;
			if (NewLine)
			{
				i->second.File << "\n";
			}
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

void Logger::LogFlagWithColor(int LogFlags)
{
	if (LogFlags & LogInfo)
	{
		std::cout << "[";
		SetConsoleAttributes(ConsoleMessageLabelTextAttributes);
		std::cout << "Info";
		ResetConsoleAttributes();
		std::cout << "]";
	}
	else if (LogFlags & LogWarning)
	{
		std::cout << "[";
		SetConsoleAttributes(ConsoleWarningTextAttributes);
		std::cout << "Warning";
		ResetConsoleAttributes();
		std::cout << "]";
	}
	else if (LogFlags & LogError)
	{
		std::cout << "[";
		SetConsoleAttributes(ConsoleErrorTextAttributes);
		std::cout << "Error";
		ResetConsoleAttributes();
		std::cout << "]";
	}
	else if (LogFlags & LogFatalError)
	{
		std::cout << "[";
		SetConsoleAttributes(ConsoleFatalErrorTextAttributes);
		std::cout << "Fatal Error";
		ResetConsoleAttributes();
		std::cout << "]";
	}
	else
	{
		//Does nothing if it's LogNone or an undefined value.
	}
	SetConsoleTextAttribute(ConsoleHandle, ConsoleDefaultTextAttributes);
}

std::string Logger::GetFlagString(int LogFlags)
{
	if (LogFlags & LogInfo)
	{
		return "[Info]";
	}
	else if (LogFlags & LogWarning)
	{
		return "[Warning]";
	}
	else if (LogFlags & LogLua)
	{
		return "[Lua]";
	}
	else if (LogFlags & LogJson)
	{
		return "[Json]";
	}
	else if (LogFlags & LogError)
	{
		return "[Error]";
	}
	else if (LogFlags & LogFatalError)
	{
		return "[Fatal Error]";
	}
	else
	{
		return "[+]";
	}
}

void Logger::LogToFile(std::string FileName, std::string Message, int LogFlags, bool LogTime, bool BypassFlagCheck)
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

std::string Logger::GetTimeString(bool MilitaryTime)
{
	std::time_t t = std::time(0);
	std::tm now;
	localtime_s(&now, &t);

	std::string DateTime;
	std::string Year = std::to_string(now.tm_year + 1900);
	std::string Month = std::to_string(now.tm_mon + 1);
	std::string Day = std::to_string(now.tm_mday);
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

	//std::cout << DateTime << "\n";

	//std::cout << "Date & Time:" << now.tm_year + 1900 << "/" << now.tm_mon + 1 << "/" << now.tm_mday << " - " << now.tm_hour << ":" << now.tm_min << "\n";

	return "[" + DateTime + "]";
}