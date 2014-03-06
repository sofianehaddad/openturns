//                                               -*- C++ -*-
/**
 *  @file  t_NumericalSample_csv.cxx
 *  @brief The test file of class NumericalSample for load and save methods
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
 *  @date   2012-04-18 17:56:46 +0200 (Wed, 18 Apr 2012)
 */
#include "OT.hxx"
#include "OTtestcode.hxx"

using namespace OT;
using namespace OT::Test;

int main(int argc, char *argv[])
{
  TESTPREAMBLE;
  OStream fullprint(std::cout);

  try
  {
    // Creation temp file with sample
    std::ofstream aTempFile;

    // 1st sample
    aTempFile.open("sample.csv", std::ofstream::out);
    aTempFile << "X1;X2;X3;X4\n";
    aTempFile << "-1.2;2.3;3.4;-4.5\n";
    aTempFile << "5.6;-6.7;7.8;8.9\n";
    aTempFile << "-0.1;3.2;5.1;7.5\n";
    aTempFile.close();

    // Tests of ImportFromCSVFile
    NumericalSample aSample = NumericalSample::ImportFromCSVFile("sample.csv");
    aSample.setName("a good sample");
    fullprint << "aSample=" << aSample << std::endl;
    // Tests of ImportFromTextFile
    aSample = NumericalSample::ImportFromTextFile("sample.csv", ";");
    aSample.setName("a good sample");
    fullprint << "aSample=" << aSample << std::endl;

    // 2nd sample
    aTempFile.open("sample.csv", std::ofstream::out);
    aTempFile << "X1,X2,X3,X4\n";
    aTempFile << "-1.2,2.3,3.4,-4.5\n";
    aTempFile << "5.6,-6.7,7.8,8.9\n";
    aTempFile << "-0.1,3.2,5.1,7.5\n";
    aTempFile.close();


    aSample = NumericalSample::ImportFromCSVFile("sample.csv", ",");
    aSample.setName("a good coma separated sample");
    fullprint << "aSample=" << aSample << std::endl;

    aSample = NumericalSample::ImportFromTextFile("sample.csv", ",");
    aSample.setName("a good coma separated sample");
    fullprint << "aSample=" << aSample << std::endl;

    // 3rd sample
    aTempFile.open("sample.csv", std::ofstream::out);
    aTempFile << "X1 X2 X3 X4\n";
    aTempFile << "-1.2 2.3 3.4 -4.5\n";
    aTempFile << "5.6 -6.7 7.8 8.9\n";
    aTempFile << "-0.1 3.2 5.1 7.5\n";
    aTempFile.close();

    aSample = NumericalSample::ImportFromTextFile("sample.csv");
    aSample.setName("a good sample with spaces");
    fullprint << "aSample=" << aSample << std::endl;

    // 4th sample
    aTempFile.open("sample.csv", std::ofstream::out);
    aTempFile << "-1.2;2.3;3.4;-4.5\n";
    aTempFile << "5.6;-xxx;7.8;8.9\n";
    aTempFile << "-0.1;3.2;5..1;7.5\n";
    aTempFile << "0.9;9.8;8.4;5.4\n";
    aTempFile.close();

    aSample = NumericalSample::ImportFromCSVFile("sample.csv");
    aSample.setName("a sample with bad entries");
    fullprint << "aSample with bad entries (see log)=" << aSample << std::endl;

    aSample = NumericalSample::ImportFromTextFile("sample.csv", ";");
    aSample.setName("a sample with bad entries");
    fullprint << "aSample with bad entries (see log)=" << aSample << std::endl;


    // 5th sample
    aTempFile.open("sample.csv", std::ofstream::out);
    aTempFile << "-1.2;2.3;3.4;-4.5\n";
    aTempFile << "5.6;-6.7;7.8\n";
    aTempFile << "-0.1;3.2;;7.5\n";
    aTempFile << "6.5;9.0;7.3;-3.7\n";
    aTempFile.close();

    aSample = NumericalSample::ImportFromCSVFile("sample.csv");
    aSample.setName("a sample with missing entries");
    fullprint << "aSample with missing entries (see log)=" << aSample << std::endl;

    aSample = NumericalSample::ImportFromTextFile("sample.csv", ";");
    aSample.setName("a sample with missing entries");
    fullprint << "aSample with missing entries (see log)=" << aSample << std::endl;

    // We export the sample to an CSV file and then read it again
    aSample.exportToCSVFile("sample.csv");
    NumericalSample sameSample = NumericalSample::ImportFromCSVFile("sample.csv");
    if (aSample != sameSample) throw TestFailed("Exported sample differs from imported sample");
    // We export a sample with no description to a CSV file and then read it again
    aSample.exportToCSVFile("sample.csv");
    sameSample = NumericalSample::ImportFromCSVFile("sample.csv");
    if (aSample != sameSample) throw TestFailed("Exported sample differs from imported sample");
    Os::Remove("sample.csv");

    try
    {
      NumericalSample aSample = NumericalSample::ImportFromCSVFile("nosample.csv");
      throw TestFailed( "ERROR: test should have failed. Found CSV file 'nosample.csv' though it should not have been there" );
    }
    catch (FileNotFoundException & ex)
    {
      // Nothing to do
    }

  }
  catch (TestFailed & ex)
  {
    std::cerr << ex << std::endl;
    return ExitCode::Error;
  }

  return ExitCode::Success;
}
