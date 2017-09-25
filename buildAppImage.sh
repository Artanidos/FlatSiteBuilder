cd AppImage
mkdir lib
cd ..
cp ../build-FlatSiteBuilder-Desktop-Release/FlatSiteBuilder AppImage
cp ../build-FlatSiteBuilder-Desktop-Release/Widgets/libWidgets.so* AppImage/lib
cp ../build-FlatSiteBuilder-Desktop-Release/plugins/DefaultThemeEditor/libDefaultThemeEditor.so AppImage/plugins
cp ../build-FlatSiteBuilder-Desktop-Release/plugins/TextEditor/libTextEditor.so AppImage/plugins
cp ../build-FlatSiteBuilder-Desktop-Release/plugins/DefaultThemeEditor/libDefaultThemeEditor.so AppImage/plugins
cp ../build-FlatSiteBuilder-Desktop-Release/plugins/NoPublisher/libNoPublisher.so AppImage/plugins
cp ../build-FlatSiteBuilder-Desktop-Release/plugins/ImageEditor/libImageEditor.so AppImage/plugins
cp ../build-FlatSiteBuilder-Desktop-Release/plugins/SliderEditor/libSliderEditor.so AppImage/plugins
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:~/SourceCode/FlatSiteBuilder/AppImage/lib
/home/olaf/linuxdeployqt/linuxdeployqt-continuous-x86_64.AppImage AppImage/FlatSiteBuilder -appimage
mv Application-x86_64.AppImage FlatSiteBuilder-Linux-x86_64-1.4.beta.AppImage
