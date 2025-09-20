#!/bin/bash

# This script packages the Qt mac application and handles the Apple signing/notarization process.
# - You must use the entitlements included in this directory.

# Check for required environment variables
if [ -z "$DEVELOPER_ID" ] || [ -z "$APPLE_KEYCHAIN_PROFILE" ] || [ -z "$APPLE_TEAM_ID" ] || [ -z "$APPLE_ID" ]; then
    echo "Error: Required environment variables not set."
    echo "Please ensure the following are set:"
    echo "  DEVELOPER_ID"
    echo "  APPLE_KEYCHAIN_PROFILE" 
    echo "  APPLE_TEAM_ID"
    echo "  APPLE_ID"
    exit 1
fi

# Application specific variables
version=2.0.3
this_year=2024
app_name="QuNeo Editor"
bundle_name="QuNeoEditor"
app_name_fp=QuNeo\ Editor
app_source=./build/Qt_6_9_2_for_macOS-Release/$app_name_fp.app
qml_dir=../QT6-QuNeo

bundle_id="com.keithmcmillen.$bundle_name"

dmg_path="./dmg"
subfolder_path="$dmg_path/$app_name"
app_path="$subfolder_path/$app_name.app"

content_source="../Content"
content_dest="$subfolder_path/Content"

changelog_source="../CHANGELOG.md"
changelog_dest="$subfolder_path/CHANGELOG.md"

icon_file="./$app_name.icns"
path_to_dqt=~/Qt/6.9.2/macos/bin/macdeployqt
developer_id="$DEVELOPER_ID"

final_dmg_name="./$app_name Mac v$version.dmg"


# set current directory to where the script was called from
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
cd "$DIR"

clear

echo "$app_name Builder v$version"
echo 
echo "Press any key to continue"
echo
read -n 1 -s -r -p ""

if [ -d "$app_path" ] 
then
	rm -rf "$app_path"
	echo ""
	echo "Cleaning out old app..."
  echo $app_path
	echo ""
fi


if [ -f "$changelog_dest" ] 
then
  rm -rf "$changelog_dest"
  echo ""
  echo "Cleaning out old changelog..."
  echo $changelog_dest
  echo ""
fi

if [ -d "$content_dest" ] 
then
  rm -rf "$content_dest"
  echo ""
  echo "Cleaning out old content..."
  echo ""
fi

if [ -d "$content_dest" ] 
then
  rm -rf "$content_dest"
  echo ""
  echo "Cleaning out old content..."
  echo ""
fi

# echo "Press any key to continue"
# echo
# read -n 1 -s -r -p ""

# copy the .app content and resources
\cp -R "$app_source" "$subfolder_path"
\cp -R "$changelog_source" "$subfolder_path"
\cp -R -p "$content_source" "$content_dest"

# echo "Press any key to continue"
# echo
# read -n 1 -s -r -p ""



echo ""
echo "### - Updating info.plist"
echo ""

# update info.plist
plutil -insert "App Category" -string "Music" "$app_path/Contents/Info.plist"
plutil -replace "Get Info string" -string "$version, Copyright $this_year Keith McMillen Instruments" "$app_path/Contents/Info.plist"
plutil -replace "CFBundleLongVersionString" -string "$app_name $version, Copyright $this_year Keith McMillen Instruments" "$app_path/Contents/Info.plist"
plutil -replace "Bundle versions string, short" -string "$version" "$app_path/Contents/Info.plist"
plutil -replace "Bundle version" -string "$version" "$app_path/Contents/Info.plist"


echo ""
echo "### - Running macdeployqt"
echo ""


#run macdeployqt
$path_to_dqt "$app_path" -verbose=2 -codesign="$developer_id" -qmldir="$qml_dir" -executable="$app_path/Contents/MacOS/$app_name"

echo "Press any key to continue"
echo
read -n 1 -s -r -p ""

echo ""
echo "### - Signing application"
echo ""

# sign the code
# QT frameworks plists copied. See: http://barenka.blogspot.com/2014/04/signing-qt5-520-applications-in-mac-osx.html
codesign -s "$developer_id" --options runtime --timestamp --force --deep --entitlements ./entitlements.mac.plist -f "$app_path"


#set folder icon (only needed if there is a folder in the dmg)
fileicon set "$subfolder_path/" "$icon_file"

# create dmg (brew install create-dmg)

echo ""
echo "### - Creating dmg..."
echo ""

# delete dmg if it already exists
test -f "$final_dmg_name" && rm "$final_dmg_name"

create-dmg \
  --volname "$app_name" \
  --volicon "$icon_file" \
  --background "$dmg_path/background.tiff" \
  --window-pos 200 120 \
  --window-size 530 380 \
  --icon-size 100 \
  --icon "$app_name" 160 220 \
  --app-drop-link 375 220 \
  --hdiutil-verbose \
  "$final_dmg_name" \
  "$dmg_path/"

# # change folder icon and inspect
# open "$final_dmg_name"
# sleep 4 # wait for image to mount
# open "/Volumes/$app_name"

# echo ""
# echo "### - Inspect volume, eject and press any key to continue"
# echo
# read -n 1 -s -r -p ""

# clean dmg attributes
xattr -cr "$final_dmg_name"

echo ""
echo "### - Signing dmg...."
echo

# sign the dmg
codesign -s "$DEVELOPER_ID" --options runtime --timestamp --force --deep --entitlements ./entitlements.mac.plist -f  "$final_dmg_name"

echo ""
echo "### - Notorizing..."
echo

# submit and notarize

# note - to use the new notarytool you must install a profile/keychain using the following steps in terminal:
# (see https://scriptingosx.com/2021/07/notarize-a-command-line-tool-with-notarytool/)

# 1) Find the profile name by entering: security find-identity -p basic -v
# 2) The profile name is the digits in parenthesis at the end of: "Developer ID Application: Kesumo, LLC ($APPLE_TEAM_ID)"
# 3) Store the credentials by entering: xcrun notarytool store-credentials --apple-id "$APPLE_ID" --team-id "$APPLE_TEAM_ID"
# 4) Enter the profile name when prompted
# 5) Enter the app specific password (signing / notarization) for the apple id
# 6) Use the profile id from step #4 in the command below

xcrun notarytool submit "$final_dmg_name" --keychain-profile "$APPLE_KEYCHAIN_PROFILE" --wait



