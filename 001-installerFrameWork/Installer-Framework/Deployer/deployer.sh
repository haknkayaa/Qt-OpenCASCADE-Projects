
_title="
  _____             _
 |  __ \           | |
 | |  | | ___ _ __ | | ___  _   _  ___ _ __
 | |  | |/ _ \ '_ \| |/ _ \| | | |/ _ \ '__|
 | |__| |  __/ |_) | | (_) | |_| |  __/ |
 |_____/ \___| .__/|_|\___/ \__, |\___|_|
             | |             __/ |    by Hakan KAYA
             |_|    2021    |___/     mail@hakankaya.kim"


echo $_title

# SETTINGS PROFILE
BINARY_FILE="../Application/cmake-build-debug/opencascadeProject"
TARGET_BIN_FILE="./packages/com.vendor.product/data/bin"
TARGET_LIB_FILE="./packages/com.vendor.product/data/lib"
OUTPUT_FILE="./InstallerPack"
PACKAGE_FILE="./Package"
OVERWRITE=false
DELETE_AFTER_PACKING=true

# START
echo "\n\nStarting...\n"

# Create Folders
echo "Creating InstallerPack folder..."
if [ -e "$OUTPUT_FILE" ]; then
    echo "$OUTPUT_FILE exists."
    echo "Dosya var."

    if [["$OVERWRITE" == "true"]]; then
      echo "Dosya silinip yeniden yazılıyor."
      rm -rf $OUTPUT_FILE
      mkdir $OUTPUT_FILE

    else
      echo "Dosya olduğu gibi bırakılacak."
    fi
else
    echo "$OUTPUT_FILE does not exist."
    mkdir $OUTPUT_FILE
    echo "Dosya yok yeniden yaratılacak."
fi

echo "Created!\n"

echo "Creating Package folder..."
mkdir $PACKAGE_FILE
echo "Created! It will delete after packing."

echo "Searching application depency..."
echo $BINARY_FILE

# Copy binary file
echo "Copying binary file > $TARGET_BIN_FILE"
cp $BINARY_FILE $TARGET_BIN_FILE

# Copy library file
echo "Copying libraries > $TARGET_LIB_FILE"
sh "$PWD"/scripts/qt-library.sh -b $BINARY_FILE -t $TARGET_LIB_FILE

