cd AppImage
mkdir lib
cd ..
cp ../build-FlatSiteBuilder-Desktop-Release/FlatSiteBuilder AppImage
cp ../build-FlatSiteBuilder-Desktop-Release/Widgets/libWidgets.so* AppImage/lib
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:~/SourceCode/FlatSiteBuilder/AppImage/lib
/home/olaf/linuxdeployqt/linuxdeployqt-continuous-x86_64.AppImage AppImage/FlatSiteBuilder -appimage
mv Application-x86_64.AppImage FlatSiteBuilder-Linux-x86_64-1.4.beta.AppImage
