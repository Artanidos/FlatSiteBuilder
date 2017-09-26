mkdir -p AppImage/lib
mkdir -p AppImage/plugins
mkdir -p AppImage/themes/default
mkdir -p AppImage/themes/himu
mkdir -p AppImage/testsite

# App
cp ../build-FlatSiteBuilder-Desktop-Release/FlatSiteBuilder AppImage
# Lib
cp ../build-FlatSiteBuilder-Desktop-Release/Widgets/libWidgets.so* AppImage/lib
# Plugins
cp ../build-FlatSiteBuilder-Desktop-Release/plugins/DefaultThemeEditor/libDefaultThemeEditor.so AppImage/plugins
cp ../build-FlatSiteBuilder-Desktop-Release/plugins/TextEditor/libTextEditor.so AppImage/plugins
cp ../build-FlatSiteBuilder-Desktop-Release/plugins/DefaultThemeEditor/libDefaultThemeEditor.so AppImage/plugins
cp ../build-FlatSiteBuilder-Desktop-Release/plugins/NoPublisher/libNoPublisher.so AppImage/plugins
cp ../build-FlatSiteBuilder-Desktop-Release/plugins/ImageEditor/libImageEditor.so AppImage/plugins
cp ../build-FlatSiteBuilder-Desktop-Release/plugins/SliderEditor/libSliderEditor.so AppImage/plugins
#Themes
cp --parents -r themes/default/* AppImage
cp --parents -r themes/himu/* AppImage
#Testsite
cp --parents -r testsite/* AppImage

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:~/SourceCode/FlatSiteBuilder/AppImage/lib
/home/olaf/linuxdeployqt/linuxdeployqt-continuous-x86_64.AppImage AppImage/FlatSiteBuilder -appimage
mv Application-x86_64.AppImage FlatSiteBuilder-Linux-x86_64-1.4.2.beta.AppImage

# Clean up
rm -r AppImage/lib/*
rm -r AppImage/plugins/*
rm -r AppImage/themes/*
rm -r AppImage/testsite/*
rmdir AppImage/lib
rmdir AppImage/plugins
rmdir AppImage/testsite
rmdir AppImage/themes
rm AppImage/FlatSiteBuilder
