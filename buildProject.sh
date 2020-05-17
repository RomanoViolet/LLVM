#!/usr/bin/env bash

# Many thanks: https://stackoverflow.com/a/14203146 (parse arguments in bash shell)
#              https://stackoverflow.com/a/42449998 (bold typeface printout)
# defaults, unless overridden
CC="GCC"
STANDARD="11"
BUILDTYPE="Release"

chosenCompiler=''
chosenStandard=''
chosenBuildType=''

POSITIONAL=()
while [[ $# -gt 0 ]]
do
key="$1"

case $key in
    -c|--compiler)
    chosenCompiler=$(echo "$2" | tr '[:upper:]' '[:lower:]')
    if [[ "$chosenCompiler" =~ ^(llvm|clang)$ ]]; then
        CC="LLVM"
    elif [[ "$chosenCompiler" =~ ^(gcc)$ ]]; then
        CC="GCC"
    else
        echo "Acceptable choices for compiler is GCC or LLVM. Defaulting to GCC."
        CC="GCC"
    fi
    shift # past argument
    shift # past value
    ;;
#
#
    -s|--standard)
    chosenStandard="$2"
    if [[ "$chosenStandard" =~ ^(11)$ ]]; then
        STANDARD="11"
    elif [[ "$chosenStandard" =~ ^(14)$ ]]; then
        STANDARD="14"
    elif [[ "$chosenStandard" =~ ^(17)$ ]]; then
    STANDARD="17"
    else
        echo "Acceptable choices for C++ standard are 11, 14, or 17. Defaulting to C++11."
        STANDARD="11"
    fi
    shift # past argument
    shift # past value
    ;;
#
#
    -b|--build)
    chosenBuildType=$(echo "$2" | tr '[:upper:]' '[:lower:]')
    if [[ "$chosenBuildType" =~ ^(debug)$ ]]; then
        BUILDTYPE="DEBUG"
    elif [[ "$chosenBuildType" =~ ^(release)$ ]]; then
        BUILDTYPE="RELEASE"
    else
        echo "Acceptable choices for buildtype are debug or release. Defaulting to release type."
        BUILDTYPE="RELEASE"
    fi
    shift # past argument
    shift # past value
    ;;
    --default)
    CC="GCC"
    STANDARD="11"
    BUILDTYPE="Release"
    shift # past argument
    ;;
    *)    # unknown option
    echo "Oh.. no choice?"
    POSITIONAL+=("$1") # save it in an array for later
    shift # past argument
    ;;
esac
done

if [[ -z ${chosenCompiler} ]]; then
    echo "NOTE: "
    echo    "       You have not chosen a compiler option."
    echo    "       You can do it via -c or --compiler <choice>, where <choice> is either GCC or LLVM."
    echo -e "       Defaulting to: \033[1mGCC\033[0m"
    echo ""
fi

if [[ -z ${chosenStandard} ]]; then
    echo "NOTE: "
    echo    "       You have not chosen a C++ standard."
    echo    "       You can do it via -s or --standard <choice>, where <choice> is either 11, 14, or 17."
    echo -e "       Defaulting to: \033[1mC++11\033[0m"
    echo ""
fi

if [[ -z ${chosenBuildType} ]]; then
    echo "NOTE: "
    echo    "       You have not chosen the build type"
    echo    "       You can do it via -b or --build <choice>, where <choice> is either debug or release."
    echo -e "       Defaulting to: \033[1mRelease\033[0m"
    echo ""
fi

set -- "${POSITIONAL[@]}" # restore positional parameters
echo ""
echo "-----------------------------------------------------"
echo -e "              \033[1m SUMMARY \033[0m"
echo "-----------------------------------------------------"
echo "COMPILER         = ${CC}"
echo "C++ STANDARD     = ${STANDARD}"
echo "BUILD TYPE       = ${BUILDTYPE}"
echo ""
echo ""

# Call the build
./BuildScripts/Docker/GeneralBuild ${BUILDTYPE} ${CC} ${STANDARD}
