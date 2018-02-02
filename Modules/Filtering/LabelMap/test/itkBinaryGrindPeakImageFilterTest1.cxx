/*=========================================================================
 *
 *  Copyright Insight Software Consortium
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkSimpleFilterWatcher.h"

#include "itkBinaryGrindPeakImageFilter.h"
#include "itkTestingMacros.h"


int itkBinaryGrindPeakImageFilterTest1( int argc, char * argv[] )
{

  if( argc != 6 )
    {
    std::cerr << "Usage: " << argv[0]
      << " input output fullyConnected foreground background" << std::endl;
    return EXIT_FAILURE;
    }

  constexpr unsigned int Dimension = 2;
  using PixelType = unsigned char;

  using ImageType = itk::Image< PixelType, Dimension >;

  using ReaderType = itk::ImageFileReader< ImageType >;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( argv[1] );

  TRY_EXPECT_NO_EXCEPTION( reader->Update() );

  using BinaryGrindPeakImageFilterType = itk::BinaryGrindPeakImageFilter<ImageType>;
  BinaryGrindPeakImageFilterType::Pointer binaryGrindPeakImageFilter =
    BinaryGrindPeakImageFilterType::New();

  EXERCISE_BASIC_OBJECT_METHODS( binaryGrindPeakImageFilter,
    BinaryGrindPeakImageFilter, ImageToImageFilter );

  binaryGrindPeakImageFilter->SetInput( reader->GetOutput() );

  bool fullyConnected = atoi( argv[3] ) != 0;
  binaryGrindPeakImageFilter->SetFullyConnected( fullyConnected );
  TEST_SET_GET_VALUE( fullyConnected, binaryGrindPeakImageFilter->GetFullyConnected() );

  if( fullyConnected )
    {
    binaryGrindPeakImageFilter->FullyConnectedOn();
    TEST_SET_GET_VALUE( true, binaryGrindPeakImageFilter->GetFullyConnected() );
    }
  else
    {
    binaryGrindPeakImageFilter->FullyConnectedOff();
    TEST_SET_GET_VALUE( false, binaryGrindPeakImageFilter->GetFullyConnected() );
    }

  BinaryGrindPeakImageFilterType::InputImagePixelType foregroundValue =
    static_cast< BinaryGrindPeakImageFilterType::InputImagePixelType >( atof( argv[4] ) );
  binaryGrindPeakImageFilter->SetForegroundValue( foregroundValue );
  TEST_SET_GET_VALUE( foregroundValue, binaryGrindPeakImageFilter->GetForegroundValue() );

  BinaryGrindPeakImageFilterType::InputImagePixelType backgroundValue =
    static_cast< BinaryGrindPeakImageFilterType::InputImagePixelType >( atoi( argv[5] ) );
  binaryGrindPeakImageFilter->SetBackgroundValue( backgroundValue );
  TEST_SET_GET_VALUE( backgroundValue, binaryGrindPeakImageFilter->GetBackgroundValue() );


  itk::SimpleFilterWatcher watcher( binaryGrindPeakImageFilter, "BinaryGrindPeakImageFilter" );

  using WriterType = itk::ImageFileWriter< ImageType >;
  WriterType::Pointer writer = WriterType::New();
  writer->SetInput( binaryGrindPeakImageFilter->GetOutput() );
  writer->SetFileName( argv[2] );

  TRY_EXPECT_NO_EXCEPTION( writer->Update() );

  return EXIT_SUCCESS;
}
