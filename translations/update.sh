#! /bin/bash

LANGS="it"
SRC_DIR="src"
TS_DIR="translations"

SCRIPT=$(realpath "$0")
DIR=$(dirname "$SCRIPT")
cd "$DIR/.."

if [ ! -d $SRC_DIR ]; then
    echo "Cannot find '$SRC_DIR' directory"
    exit 1
fi
if [ ! -d $TS_DIR ]; then
    echo "Cannot find '$TS_DIR' directory"
    exit 1
fi

TS=""
for lang in $LANGS; do
    TS="$TS $TS_DIR/$lang.ts"
done

lupdate -recursive $SRC_DIR -ts $TS
echo "Done!"
