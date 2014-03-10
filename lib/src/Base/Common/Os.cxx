//                                               -*- C++ -*-
/**
 *  @file  Os.cxx
 *  @brief This class provides operating system specific variables
 *
 *  Copyright (C) 2005-2014 Airbus-EDF-Phimeca
 *
 *  This library is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  along with this library.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  @author dutka
 *  @date   2009-04-30 12:04:13 +0200 (jeu 30 avr 2009)
 */

#include "Os.hxx"
#include "Log.hxx"
#include "OSS.hxx"

#include "OTwindows.h" // For CreateProcess
#include "ResourceMap.hxx" // For ResourceMap
#include <cstdlib>   // for system(3)

BEGIN_NAMESPACE_OPENTURNS

const char * Os::GetDirectorySeparator()
{
#ifndef WIN32
  return "/";
#else
  return "\\";
#endif
}

const char * Os::GetDirectoryListSeparator()
{
#ifndef WIN32
  return ":";
#else
  return ";";
#endif
}

const char * Os::GetEndOfLine()
{
#ifndef WIN32
  return "\n";
#else
  return "\r\n";
#endif
}

String Os::GetDeleteCommandOutput()
{
#ifndef WIN32
  return " > /dev/null 2>&1";
#else
  return " > NUL";
#endif
}

// Returns 0 if no error
int Os::ExecuteCommand(const String & command)
{
  int rc = -1;
  LOGINFO( OSS() << "Execute command=" << command );
#ifdef WIN32
  if ( ResourceMap::GetAsUnsignedInteger("Os-create-process") == 1 )
  {
    // Startup information
    STARTUPINFO si;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    si.dwFlags = STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_HIDE;

    // Process information
    PROCESS_INFORMATION pi;
    ZeroMemory(&pi, sizeof(pi));

    // Create the process
    DWORD dwProcessFlags = 0;
    char cmd[command.size() + 1];
    strcpy(&cmd[0], command.c_str());
    const Bool processOk = CreateProcess(NULL, &cmd[0], NULL, NULL, TRUE, dwProcessFlags, NULL, NULL, &si, &pi);
    if ( processOk )
    {
      // Wait for the external application to finish
      DWORD waitRc = WaitForSingleObject(pi.hProcess, INFINITE);
      if ( waitRc != WAIT_FAILED )
      {
        DWORD exit_code = 0;
        const Bool codeOk = GetExitCodeProcess(pi.hProcess, &exit_code);
        if (codeOk)
        {
          rc = exit_code;
        }
        else
        {
          rc = GetLastError();
        }
      }

      // Close everything
      CloseHandle(pi.hProcess);
      CloseHandle(pi.hThread);
    }
    else
    {
      rc = GetLastError();
    }
  } // use create process
  else
#endif
  {
    rc = system(command.c_str());
  }
  LOGINFO( OSS() << "Return code=" << rc << " for command=" << command );
  return rc;
}


void Os::Remove(const String& fileName)
{
  if (remove(fileName.c_str()) == -1)
  {
    Log::Warn(OSS() << "Warning: cannot remove file " << fileName);
  }
}


END_NAMESPACE_OPENTURNS
