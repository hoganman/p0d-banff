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

#for RUN in "run2a" "run2w" "run3b" "run3c" "run4a" "run4w" "run5" "run6b" "run6c" "run6d" "run6e" "run7" "run8a" "run8w"; do
for RUN in "run2a" "run2w" "run3b" "run3c" "run4a" "run4w" "run5" "run6b" "run6c" "run6d" "run6e"; do
  DIRECTORY=${LOCATION}/${RUN}
  for FILE in `ls ${DIRECTORY}`; do
    echo ${DIRECTORY}/${FILE} >> ${OUTFILE}
  done
done

echo "Done writing files"
