/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    DicomSeriesReadSeriesWrite.cxx
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
//  and then save this volume into another DICOM series using the 
//  exact same header information. It makes use of the GDCM library
//
//  Software Guide : EndLatex 


// Software Guide : BeginCodeSnippet
#include "itkImageSeriesReader.h"
#include "itkImageSeriesWriter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkGDCMImageIO.h"
#include "itkGDCMSeriesFileNames.h"
// Software Guide : EndCodeSnippet

#include <vector>
#include <itksys/SystemTools.hxx>

int main( int argc, char* argv[] )
{
  if( argc < 3 )
    {
    std::cerr << "Usage: " << argv[0] << 
      " DicomDirectory  OutputDicomDirectory" << std::endl;
    return EXIT_FAILURE;
    }

//  Software Guide : BeginLatex
//
//  First we define the image type to be used in this example. From the image
//  type we can define the type of the reader. We also declare types for the
//  \doxygen{GDCMImageIO} object that will actually read and write the DICOM
//  images, and the \doxygen{GDCMSeriesFileNames} object that will generate and
//  order all the filenames for the slices composing the volume dataset.
//
//  Software Guide : EndLatex 


// Software Guide : BeginCodeSnippet
  typedef itk::Image<unsigned short,3>            ImageType;
  typedef itk::ImageSeriesReader< ImageType >     ReaderType;
  typedef itk::GDCMImageIO                        ImageIOType;
  typedef itk::GDCMSeriesFileNames                SeriesFileNames;
// Software Guide : EndCodeSnippet


// Software Guide : BeginCodeSnippet
  ImageIOType::Pointer gdcmIO = ImageIOType::New();
  SeriesFileNames::Pointer it = SeriesFileNames::New();

  // Get the DICOM filenames from the directory
  it->SetInputDirectory( argv[1] );

  ReaderType::Pointer reader = ReaderType::New();

  const ReaderType::FileNamesContainer & filenames = it->GetInputFileNames();
  unsigned int numberOfFilenames =  filenames.size();
  std::cout << numberOfFilenames << std::endl; 
  for(unsigned int fni = 0; fni<numberOfFilenames; fni++)
    {
    std::cout << "filename # " << fni << " = ";
    std::cout << filenames[fni] << std::endl;
    }
  
  reader->SetFileNames( filenames );
  reader->SetImageIO( gdcmIO );

  try
    {
    reader->Update();
    }
  catch (itk::ExceptionObject &excp)
    {
    std::cerr << "Exception thrown while writing the image" << std::endl;
    std::cerr << excp << std::endl;

    return EXIT_FAILURE;
    }
// Software Guide : EndCodeSnippet


//  Software Guide : BeginLatex
//
//  Now we can prepare the process for writing the dataset.
//
//  Software Guide : EndLatex 

// Software Guide : BeginCodeSnippet
  const char * outputDirectory = argv[2];

  itksys::SystemTools::MakeDirectory( outputDirectory );
    
  typedef itk::Image<unsigned short,2>            Image2DType;
  typedef itk::ImageSeriesWriter< ImageType, Image2DType > SeriesWriterType;
    
  SeriesWriterType::Pointer swriter = SeriesWriterType::New();
  swriter->SetInput( reader->GetOutput() );
  swriter->SetImageIO( gdcmIO );

  it->SetOutputDirectory( outputDirectory );
  swriter->SetFileNames( it->GetOutputFileNames() );
// Software Guide : EndCodeSnippet

  
//  Software Guide : BeginLatex
//
//  The next line is extremely important for this process to work correctly.
//  The line is taking the MetaDataDictionary from the input reader and passing
//  it to the output writer. The reason why this step is so important is that
//  the MetaDataDictionary contains all the entries of the input DICOM header.
//
//  Software Guide : EndLatex 

// Software Guide : BeginCodeSnippet
  swriter->SetMetaDataDictionaryArray( reader->GetMetaDataDictionaryArray() );

  // Try to write the serie:
  try
    {
    swriter->Update();
    }
  catch( itk::ExceptionObject & excp )
    {
    std::cerr << "Exception thrown while writing the series " << std::endl;
    std::cerr << excp << std::endl;
    }

// Software Guide : EndCodeSnippet
  return EXIT_SUCCESS;
}

