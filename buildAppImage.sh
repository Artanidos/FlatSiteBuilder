mkdir -p AppImage/usr/lib
mkdir -p AppImage/usr/bin
mkdir -p AppImage/usr/share/applications
mkdir -p AppImage/usr/share/icons/hicolor
mkdir -p AppImage/plugins
mkdir -p AppImage/themes/default
mkdir -p AppImage/themes/himu
mkdir -p AppImage/testsite

# App
cp ../build-FlatSiteBuilder-Desktop_Qt_5_9_1_GCC_64bit-Release/FlatSiteBuilder AppImage/usr/bin
# Lib
cp ../build-FlatSiteBuilder-Desktop_Qt_5_9_1_GCC_64bit-Release/Widgets/libWidgets.so* AppImage/usr/lib
# Desktop, #Icon
cp default.desktop AppImage/usr/share/applications
cp default.svg AppImage/usr/share/icons/hicolor
# Plugins
cp ../build-FlatSiteBuilder-Desktop_Qt_5_9_1_GCC_64bit-Release/plugins/DefaultThemeEditor/libDefaultThemeEditor.so AppImage/plugins
cp ../build-FlatSiteBuilder-Desktop_Qt_5_9_1_GCC_64bit-Release/plugins/TextEditor/libTextEditor.so AppImage/plugins
cp ../build-FlatSiteBuilder-Desktop_Qt_5_9_1_GCC_64bit-Release/plugins/DefaultThemeEditor/libDefaultThemeEditor.so AppImage/plugins
cp ../build-FlatSiteBuilder-Desktop_Qt_5_9_1_GCC_64bit-Release/plugins/NoPublisher/libNoPublisher.so AppImage/plugins
cp ../build-FlatSiteBuilder-Desktop_Qt_5_9_1_GCC_64bit-Release/plugins/ImageEditor/libImageEditor.so AppImage/plugins
cp ../build-FlatSiteBuilder-Desktop_Qt_5_9_1_GCC_64bit-Release/plugins/SliderEditor/libSliderEditor.so AppImage/plugins
#Themes
cp --parents -r themes/default/* AppImage
cp --parents -r themes/himu/* AppImage
#Testsite
cp --parents -r testsite/* AppImage

export LD_LIBRARY_PATH=~/SourceCode/FlatSiteBuilder/AppImage/usr/lib:~/Qt/5.9.1/gcc_64/lib
/home/olaf/linuxdeployqt/linuxdeployqt-continuous-x86_64.AppImage AppImage/usr/share/applications/*.desktop
rm AppImage/AppRun
cp AppRun AppImage
/home/olaf/appimagetool/appimagetool-x86_64.AppImage AppImage
mv FlatSiteBuilder-x86_64.AppImage FlatSiteBuilder-Linux-x86_64-1.4.7.AppImage

# Clean up
rm -r AppImage/*
