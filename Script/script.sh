#! /bin/bash
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:"../Libraries/boost_1_59_0/lib/"
export LD_LIBRARY_PATH
cd "../build-SpecificationLanguage-Desktop_Qt_5_5_1_GCC_64bit-Debug/"
"./SpecificationLanguage" -p -s "../InputFiles/$1"&&
cd "../build-LazyDiagnosis-Desktop_Qt_5_5_1_GCC_64bit-Debug/"&&
"./LazyDiagnosis"&&
cd "../build-MonolithicDiagnosis-Desktop_Qt_5_5_1_GCC_64bit-Debug/"&&
"./MonolithicDiagnosis"

