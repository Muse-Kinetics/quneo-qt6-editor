#-------------------------------------------------
#
# Project created by QtCreator 2012-01-24T15:21:06
#
#-------------------------------------------------

QT       += core gui \
            widgets \
            network

TARGET = "QuNeo Editor"
TEMPLATE = app
VERSION = 2.0.3
DEFINES += APP_VERSION=\\\"$$VERSION\\\"

QMAKE_TARGET_BUNDLE_PREFIX = com.keithmcmillen.QuNeo Editor

#uncomment this and DEPLOY = 1 to build a console version of the app. Do this once before deploying the app.
#BUILD_CONSOLE = 1

# Uncomment this line if you want to deploy the app (codesign, xxxDeployqt, copy content, and create installer/dmg etc
DEPLOY = 1

# Uncomment this line if your project includes QML, this will add the qmldir option to the qt deployment utility command
INCLUDE_QML = 1


# still holding onto support for High Sierra here, separate build

message("Building with Qt $${QT_VERSION}")

# build with Qt 5.11.3 to support El Capitan, Sierra, and High Sierra
lessThan(QT_MAJOR_VERSION, 6){
    message("Building legacy MacOS Intel Binary")
    macx{
        QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.11
    }
}

# build with Qt 6.2.1 to support Mojave and later
versionAtLeast(QT_VERSION, 6.2.1){
    message("Building Apple M1/Intel Universal Binary")
    macx{
        QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.14
        QMAKE_APPLE_DEVICE_ARCHS = x86_64 arm64
    }
}

!isEmpty(BUILD_CONSOLE) {
    CONFIG += console
}

INCLUDEPATH += ./ui/buttons \
                ./ui/editPanes \
                ./data \
                ./midi \
                ./ui/labels \
                ./inc/rtmidi \
                ./inc/KMI_Ports \
                ./inc/KMI_Updates \
                ./inc/KMI_MIDI_Device_Manager \
                ./inc/KMI_MIDI_Device_Manager/fwupdate \
                ./inc/KMI_MIDI_Device_Manager/troubleshoot


SOURCES += main.cpp\
    inc/KMI_MIDI_Device_Manager/KMI_mdm.cpp \
    inc/KMI_MIDI_Device_Manager/fwupdate/fwupdate.cpp \
    inc/KMI_MIDI_Device_Manager/troubleshoot/troubleshoot.cpp \
    inc/KMI_Ports/kmi_ports.cpp \
    inc/KMI_Updates/kmi_updates.cpp \
    inc/RtMidi/RtMidi.cpp \
        mainwindow.cpp \
    ui/buttons/padbutton.cpp \
    data/presethandler.cpp \
    ui/buttons/vsliderbutton.cpp \
    ui/buttons/rotarybutton.cpp \
    ui/buttons/hsliderbutton.cpp \
    ui/buttons/lsliderbutton.cpp \
    ui/buttons/buttonbutton.cpp \
    ui/editPanes/padeditpane.cpp \
    ui/editPanes/rotaryeditpane.cpp \
    ui/editPanes/hslidereditpane.cpp \
    ui/editPanes/vslidereditpane.cpp \
    ui/editPanes/lslidereditpane.cpp \
    ui/editPanes/transporteditpane.cpp \
    ui/editPanes/leftrighteditpane.cpp \
    ui/editPanes/rhombuseditpane.cpp \
    ui/editPanes/updowneditpane.cpp \
    ui/editPanes/modeeditpane.cpp \
    ui/labels/padlabels.cpp \
    ui/labels/hsliderlabels.cpp \
    ui/labels/vsliderlabels.cpp \
    ui/labels/lsliderlabels.cpp \
    ui/labels/rotarylabels.cpp \
    ui/editPanes/globalparameters.cpp \
    ui/labels/updateindicator.cpp \
    ui/labels/updownlabels.cpp \
    ui/labels/transportlabels.cpp \
    ui/labels/rhombuslabels.cpp \
    ui/labels/modelabels.cpp \
    ui/labels/leftrightlabels.cpp \
    midi/sysexformat.cpp \
    data/copypastehandler.cpp \
    data/datavalidator.cpp \
    midi/midideviceaccess.cpp



HEADERS  += mainwindow.h \
    inc/KMI_MIDI_Device_Manager/KMI_DevData.h \
    inc/KMI_MIDI_Device_Manager/KMI_FwVersions.h \
    inc/KMI_MIDI_Device_Manager/KMI_SysexMessages.h \
    inc/KMI_MIDI_Device_Manager/KMI_mdm.h \
    inc/KMI_MIDI_Device_Manager/fwupdate/fwupdate.h \
    inc/KMI_MIDI_Device_Manager/midi.h \
    inc/KMI_MIDI_Device_Manager/troubleshoot/troubleshoot.h \
    inc/KMI_Ports/kmi_ports.h \
    inc/KMI_Updates/kmi_updates.h \
    inc/RtMidi/RtMidi.h \
    ui/buttons/padbutton.h \
    data/presethandler.h \
    ui/buttons/vsliderbutton.h \
    ui/buttons/rotarybutton.h \
    ui/buttons/hsliderbutton.h \
    ui/buttons/lsliderbutton.h \
    ui/buttons/buttonbutton.h \
    ui/editPanes/padeditpane.h \
    ui/editPanes/rotaryeditpane.h \
    ui/editPanes/hslidereditpane.h \
    ui/editPanes/vslidereditpane.h \
    ui/editPanes/lslidereditpane.h \
    ui/editPanes/transporteditpane.h \
    ui/editPanes/leftrighteditpane.h \
    ui/editPanes/rhombuseditpane.h \
    ui/editPanes/updowneditpane.h \
    ui/editPanes/modeeditpane.h \
    ui/labels/padlabels.h \
    ui/labels/hsliderlabels.h \
    ui/labels/vsliderlabels.h \
    ui/labels/lsliderlabels.h \
    ui/labels/rotarylabels.h \
    ui/editPanes/globalparameters.h \
    ui/labels/updateindicator.h \
    ui/labels/updownlabels.h \
    ui/labels/transportlabels.h \
    ui/labels/rhombuslabels.h \
    ui/labels/modelabels.h \
    ui/labels/leftrightlabels.h \
    midi/sysexformat.h \
    data/copypastehandler.h \
    data/datavalidator.h \
    midi/midideviceaccess.h


FORMS    += mainwindow.ui \
    inc/KMI_MIDI_Device_Manager/fwupdate/fwupdate.ui \
    inc/KMI_MIDI_Device_Manager/troubleshoot/troubleshoot.ui

RESOURCES += \
    Resources.qrc \
    inc/KMI_MIDI_Device_Manager/fwupdate/fw_stylesheets.qrc \
    resources/fonts/fonts.qrc

#------QJson-------#
static{
DEFINES += STATIC_BUILD
}

#QJSON_SRCBASE = $$PWD/qjson/src

#QJSON_CPP = $$QJSON_SRCBASE
#INCLUDEPATH += $$QJSON_CPP


#SOURCES += \
#  $$QJSON_CPP/json_parser.cc \
#  $$QJSON_CPP/json_scanner.cpp \
#  $$QJSON_CPP/parser.cpp \
#   $$QJSON_CPP/qobjecthelper.cpp \
#  $$QJSON_CPP/serializer.cpp \

HEADERS += \

#------OS Specific MIDI-------#
#win32{
#LIBS += -lwinmm
#}

macx {
#LIBS += -framework CoreMIDI
#LIBS += -framework CoreFoundation
LIBS += -framework Cocoa
LIBS += -framework CoreServices
}

# SSL support for checking for updates on windows
win32{
    LIBS += -LC:\Qt\Tools\OpenSSL\Win_x64\lib -llibcrypto
    INCLUDEPATH+= C:\Qt\Tools\OpenSSL\Win_x64\include\openssl
}

#------OS Specific ICONS-------#
win32{
OTHER_FILES += \
    quneoIcon.rc.txt
RC_FILE = quneoIcon.rc.txt
}

macx{
ICON = darth_quneo_icon.icns
}

OTHER_FILES += \
    darth_quneo_icon.ico \
    darth_quneo_icon.icns \
    quneoIcon.txt


#DEFINES += \
    #MDM_DEBUG_ENABLED # enable deeper deebugging for KMI Midi Device Manager

# These defines set RtMidi to the correct OS API
macx{
    DEFINES += __MACOSX_CORE__=1
    DEFINES += __MACOSX_SYX_XMIT_SIZE__=1023 #32767
    LIBS += -framework CoreMidi
    LIBS += -framework CoreAudio
    LIBS += -framework CoreFoundation
}

#ios{
#    plistupdate.commands = /usr/libexec/PlistBuddy -c \"Set :CFBundleShortVersionString $$VERSION\" $$QMAKE_INFO_PLIST
#    QMAKE_EXTRA_TARGETS += plistupdate
#    PRE_TARGETDEPS += plistupdate
#}


ios{
    DEFINES += TARGET_OS_IPHONE=1
    LIBS += -framework CoreMidi
    LIBS += -framework CoreAudio
    LIBS += -framework CoreFoundation
}

linux{
    DEFINES += define __LINUX_ALSA__=1
    LIBS += -lasound
}

win32{
    DEFINES += __WINDOWS_MM__=1
        LIBS += -lwinmm
}
# end rtmidi defines

#--------------- deploy --------


isEmpty(DEPLOY) {
    # Define a dummy deploy target that does nothing
    QMAKE_EXTRA_TARGETS += deploy
    deploy.commands = @echo "Deploy is disabled"
} else {


    win32 {

        message("Deploying Windows Installer")

        package_dir = $$shell_path($$absolute_path("..\\win-deploy\\packages\\com.keithmcmillen.quneoeditor\\data\\$${TARGET}", $$PWD))
        content_dir = $$shell_path($$absolute_path("..\\win-deploy\\packages\\com.keithmcmillen.quneoeditor\\data\\Content", $$PWD))
        repo_root_dir = $$shell_path($$absolute_path("..", $$PWD))

        changelog_src = "$${repo_root_dir}\\CHANGELOG.md"
        content_src = "$${repo_root_dir}\\Content"

        LIBCRYPTO_SRC = $$PWD/inc/KMI_Updates/ssl/libcrypto-1_1-x64.dll
        LIBSSL_SRC = $$PWD/inc/KMI_Updates/ssl/libssl-1_1-x64.dll

        LIBCRYPTO_DST = $$replace(LIBCRYPTO_SRC, '/', '\\')
        LIBSSL_DST = $$replace(LIBSSL_SRC, '/', '\\')

        path_to_signtool = C:\\Program Files (x86)\\Windows Kits\\10\\bin\\10.0.22000.0\\x64\\signtool.exe
        path_to_qtwindeploy = $$[QT_INSTALL_BINS]\\windeployqt.exe

        path_to_bincreate = C:\\Qt6\\QtIFW-4.6.0\\bin\\binarycreator.exe
        path_to_installerbase = C:\\Qt6\\QtIFW-4.6.0\\bin\\installerbase.exe
        path_to_install = $$shell_path($$absolute_path("..\\win-deploy", $$PWD))

        # Corrected variable assignments without escaped quotes
        app_name = $${TARGET}
        installer_name = \"$$app_name v$$VERSION Windows Installer\"
        installer_file = \"$$app_name v$$VERSION Windows Installer.exe\"


        message("path_to_bincreate is: " $$quote($$path_to_bincreate))
        message("installer_file is: " $$quote($$installer_file))

        build_subdir = release  # Default to release
        CONFIG(debug, debug|release): build_subdir = debug

        temp_out_pwd = $$replace(OUT_PWD, "/", "\\")
        binary_src = "$${temp_out_pwd}\\$${build_subdir}\\$${TARGET}.exe"
        binary_dest = "$${package_dir}\\$${TARGET}.exe"
        app_name = $${TARGET}

        debug_src = "$${temp_out_pwd}\\$${build_subdir}\\$${TARGET} (debug console).exe"
        debug_dest = "$${package_dir}\\$${TARGET} (debug console).exe"


        deploy_opts = "--compiler-runtime"
        !isEmpty(INCLUDE_QML) {
            deploy_opts += " --qmldir \"$$PWD\""
        }


        # Define custom deployment commands after all variables have been defined
        DEPLOY_COMMANDS = \
            echo Deploying for Windows && \
            echo Copying executable to package_dir && \
            copy /y \"$$binary_src\" \"$$binary_dest\" && \
            copy /y \"$$debug_src\" \"$$debug_dest\" && \
            \
            echo Signing App Executable && \
            \"$$path_to_signtool\" sign /v /debug /a /tr http://timestamp.digicert.com /td SHA256 /fd certHash \"$$binary_dest\" && \
            \"$$path_to_signtool\" sign /v /debug /a /tr http://timestamp.digicert.com /td SHA256 /fd certHash \"$$debug_dest\" && \
            \
            echo Running qtwindeploy: \"$$package_dir\" && \
            \"$$path_to_qtwindeploy\" $$deploy_opts --dir \"$$package_dir\" \"$$binary_dest\" && \
            \
            echo Copying SSL dlls to package_dir && \
            copy /y \"$$LIBCRYPTO_DST\" \"$$package_dir\" && \
            copy /y \"$$LIBSSL_DST\" \"$$package_dir\" && \
            \
            echo Clearing content && \
            (if exist \"$$content_dir\" rmdir /s /q \"$$content_dir\" || echo Directory not found, skipping deletion) && \
            mkdir \"$$content_dir\" && \
            \
            echo Copying content && \
            Robocopy \"$$content_src\" \"$$content_dir\" /MIR && \
            echo Robocopy Exit Code: %ERRORLEVEL% & \
            \
            echo Copying changelog && \
            copy /y \"$$changelog_src\" \"$$content_dir\" && \
            \
            echo Creating Installer && \
            cd \"$$path_to_install\" && \
            \"$$path_to_bincreate\" --verbose --offline-only -c config/config.xml -p packages $$installer_name && \
            \
            echo Signing Installer && \
            \"$$path_to_signtool\" sign /v /debug /a /tr http://timestamp.globalsign.com/tsa/advanced /td SHA256 /fd certHash $$installer_file


        # Define a phony target for deployment
            QMAKE_EXTRA_TARGETS += deploy
            deploy.commands = $$DEPLOY_COMMANDS
            deploy.depends = first  # ensures this runs after the first build

    }
}
