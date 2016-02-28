#!/bin/sh

CURRENT_DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )

git clone --depth 1 https://github.com/cinder/Cinder.git vendor/Cinder

cd "${CURRENT_DIR}/vendor/Cinder/include"
rm -rf boost
git clone --depth 1 https://github.com/cinder/Cinder-Boost.git boost

mv boost/boost boost-temp
rm -rf boost
mv boost-temp boost

cd "${CURRENT_DIR}/vendor/Cinder/xcode"

CURRENT_DIR="${CURRENT_DIR}/vendor/Cinder/xcode"
CINDER_XCODEPROJ="${CURRENT_DIR}/cinder.xcodeproj"

xcrun xcodebuild -project ${CINDER_XCODEPROJ} -target cinder -configuration Release $@
xcrun xcodebuild -project ${CINDER_XCODEPROJ} -target cinder -configuration Debug $@

CURRENT_DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )

cd "${CURRENT_DIR}/vendor/Cinder"
CURRENT_DIR="${CURRENT_DIR}/vendor/Cinder"

rm -rf blocks docs lib/ios lib/ios-sim lib/macosx lib/msw lib/winrt samples src test tools vc2013 vc2015_winrt xcode .git* *.md
