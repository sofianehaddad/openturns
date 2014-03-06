//                                               -*- C++ -*-
/**
 *  @file  PlatformInfo.cxx
 *  @brief The class PlatformInfo gives information about the library
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
 *  @author schueller
 *  @date   2012-02-17 19:35:43 +0100 (Fri, 17 Feb 2012)
 */
#include "OTconfig.hxx"
#include "OTconfigureArgs.hxx"
#include "OSS.hxx"
#include "Log.hxx"
#include "PlatformInfo.hxx"

BEGIN_NAMESPACE_OPENTURNS



UnsignedLong PlatformInfo::Precision_ = 6;

PlatformInfo::PlatformInfo()
{
}


String PlatformInfo::GetVersion()
{
  return PACKAGE_VERSION;
}

String PlatformInfo::GetName()
{
  return PACKAGE_NAME;
}

String PlatformInfo::GetRevision()
{
  return Revision;
}

String PlatformInfo::GetDate()
{
  return ConfigureDate;
}

String PlatformInfo::GetInstallationDirectory()
{
  return InstallationDirectory;
}

String PlatformInfo::GetModuleDirectory()
{
  return ModuleDirectory;
}

String PlatformInfo::GetConfigureCommandLine()
{
  return ConfigureArgs;
}

UnsignedLong PlatformInfo::GetNumericalPrecision()
{
  return PlatformInfo::Precision_;
}

void PlatformInfo::SetNumericalPrecision(SignedInteger precision)
{
  PlatformInfo::Precision_ = (precision >= 0) ? precision : PlatformInfo::Precision_;
}

END_NAMESPACE_OPENTURNS
