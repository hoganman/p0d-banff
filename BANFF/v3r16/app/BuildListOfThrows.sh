#!/bin/bash

# Usage
function usage
{
  echo "./BuildListOfThrows.sh -l LOCATION -o OUTFILE"
  echo -e "\t Required:"
  echo -e "\t\t LOCATION - directory where files live, requires an assumed directory structure"
  echo -e "\t\t OUTFILE - name of output file"
}

# Init variables
LOCATION=
OUTFILE=

# Get command line argument
if [ "${1}" = "" ]; then
  usage
  exit 1
fi

while [ "${1}" != "" ]; do
  case ${1} in
    -l | --location )
      shift
      LOCATION=${1}
      ;;
    -o | --output )
      shift
      OUTFILE=${1}
      ;;
    -h | --help )
      usage
      exit
      ;;
    * )
      usage
      exit 1
  esac
  shift
done

if [ "${LOCATION}" = "" ] || [ "${OUTFILE}" = "" ]; then
  usage
  exit 1
fi

for RUN in "run2-air" "run2-water" "run3b-air" "run3c-air" "run4-air" "run4-water" "run5c-water" "run6b-air" "run6c-air" "run6d-air" "run6e-air"; do
  DIRECTORY=${LOCATION}/${RUN}
  for FILE in `ls -v ${DIRECTORY}`; do
    echo ${DIRECTORY}/${FILE} >> ${OUTFILE}
  done
done

echo "Done writing files"
