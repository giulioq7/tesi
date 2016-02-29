#! /bin/bash
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:"../Libraries/boost_1_59_0/lib/"
export LD_LIBRARY_PATH
cd "../bin/"
"./SpecificationLanguage" -s "../InputFiles/$1"&&
"./LazyDiagnosis"&&
"./GreedyDiagnosis"

