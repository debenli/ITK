/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    DicomSeriesReadImageWrite.cxx
  Language:  C++
  Date:      $Date$
  Version:   $Revision$

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

//  Software Guide : BeginLatex
//
//  This example illustrates how to read a DICOM series into a volume
//  and then save this volume in another file format.
//
//  Software Guide : EndLatex 


#include "itkImageSeriesReader.h"
#include "itkDICOMImageIO2.h"
#include "itkDICOMSeriesFileNames.h"
#include "itkImageFileWriter.h"

int main( int argc, char* argv[] )
{

  if( argc < 3 )
    {
    std::cerr << "Usage: " << argv[0] << " DicomDirectory  outputFileName  [seriesName]" << std::endl;
    return EXIT_FAILURE;
    }

  typedef itk::Image<unsigned short,3>            ImageType;
  typedef itk::ImageSeriesReader< ImageType >     ReaderType;

  itk::DICOMImageIO2::Pointer dicomIO = itk::DICOMImageIO2::New();

  // Get the DICOM filenames from the directory
  itk::DICOMSeriesFileNames::Pointer nameGenerator = itk::DICOMSeriesFileNames::New();
  nameGenerator->SetDirectory( argv[1] );
  

  try
    {
    typedef std::vector<std::string> seriesIdContainer;
    const seriesIdContainer & seriesUID = nameGenerator->GetSeriesUIDs();

    seriesIdContainer::const_iterator seriesItr = seriesUID.begin();
    seriesIdContainer::const_iterator seriesEnd = seriesUID.end();
  
    std::cout << std::endl << "The directory: " << std::endl;
    std::cout << std::endl << argv[1] << std::endl << std::endl;
    std::cout << "Contains the following DICOM Series: ";
    std::cout << std::endl << std::endl;

    while( seriesItr != seriesEnd )
      {
      std::cout << seriesItr->c_str() << std::endl;
      seriesItr++;
      }
  
    std::cout << std::endl << std::endl;
    std::cout << "Now reading series: " << std::endl << std::endl;

    typedef std::vector<std::string> fileNamesContainer;
    fileNamesContainer fileNames;

    if( argc < 4 ) // If no optional third argument
      {
      std::cout << seriesUID.begin()->c_str() << std::endl;
      fileNames = nameGenerator->GetFileNames();
      }
    else
      {
      std::cout << argv[3] << std::endl;
      fileNames = nameGenerator->GetFileNames( argv[3] );
      }
    std::cout << std::endl << std::endl;

    ReaderType::Pointer reader = ReaderType::New();
    reader->SetFileNames( fileNames );
    reader->SetImageIO( dicomIO );

    try
      {
      reader->Update();
      }
    catch (itk::ExceptionObject &ex)
      {
      std::cout << ex << std::endl;
      return EXIT_FAILURE;
      }

    typedef itk::ImageFileWriter< ImageType > WriterType;
    WriterType::Pointer writer = WriterType::New();

    std::cout  << "Writing the image as " << std::endl << std::endl;
    std::cout  << argv[2] << std::endl << std::endl;

    writer->SetFileName( argv[2] );
    
    writer->SetInput( reader->GetOutput() );

    try
      {
      writer->Update();
      }
    catch (itk::ExceptionObject &ex)
      {
      std::cout << ex;
      return EXIT_FAILURE;
      }
    }
  catch (itk::ExceptionObject &ex)
    {
    std::cout << ex;
    return EXIT_FAILURE;
    }

  return EXIT_SUCCESS;

}
