QT += widgets quick
requires(qtConfig(treeview))

FORMS       = \
	MainWindow.ui \
	ViewWindow.ui
HEADERS     = \
	MainWindow.h \
	TreeData.h \
	TreeEnums.h \
	TreeItem.h \
	TreeModel.h \
	TreeDelegate.h \
	TreeProxyModel.h \
	Win.h \
	RectEditorWidget.h \
	WindowView.h \
	ViewWindow.h
SOURCES     = \
			  main.cpp \
	MainWindow.cpp \
	TreeData.cpp \
	TreeModel.cpp \
	TreeItem.cpp \
	TreeDelegate.cpp \
	TreeProxyModel.cpp \
	RectEditorWidget.cpp \
	WindowView.cpp \
	ViewWindow.cpp

# install
target.path = $$[QT_INSTALL_EXAMPLES]/widgets/itemviews/editabletreemodel
INSTALLS += target

RESOURCES += \
	qml.qrc
