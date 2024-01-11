/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QApplication>
#include <QScreen>
#include <QQuickView>
#include <QQmlContext>

#include "MainWindow.h"
#include "ViewWindow.h"
#include "TreeModel.h"
#include "TreeProxyModel.h"
#include "TreeData.h"
#include "TreeEnums.h"

void printTreeData(TreeData treeData, int depth)
{
	QString tabs = "";
	for(int i = 0; i < depth; i++)
	{
		tabs += "\t";
	}

	for(int i = 0; i < treeData.children.length(); ++i)
	{
		printTreeData(treeData.children.at(i), depth + 1);
	}
}

TreeData getDefaultTree()
{
	TreeData root;

	QList<QScreen*> screens = QGuiApplication::screens();
	for(int i = 0; i < screens.length(); ++i)
	{
		QScreen* screen = screens.at(i);

		TreeData monData(screen->name(), Axis::A_Horizontal, Layout::L_Tabbed, screen->geometry());
		TreeData desktopData("Desktop 1", Axis::A_Horizontal, Layout::L_Split);
		TreeData windowData("Window", Axis::A_None, Layout::L_None, QRect(0, 0, -1, -1), (HWND)0x5000);

		desktopData.children.append(windowData);
		monData.children.append(desktopData);
		root.children.append(monData);
	}

	return root;
}

TreeData readData(QString filename)
{
	QFile file(filename);
	file.open(QIODevice::ReadOnly);

	QDataStream in(&file);
	TreeData data;
	in >> data;

	file.close();

	printTreeData(data, 0);
	return data;
}

void writeData(QString filename, TreeData treeData)
{
	QFile file(filename);
	file.open(QIODevice::WriteOnly);

	QDataStream out(&file);
	out << treeData;

	file.close();
}

void writeDefaultData(QString filename)
{
	writeData(filename, getDefaultTree());
}

static TreeModel* treeModel = nullptr;
void handleSave()
{
	writeData("file.dat", treeModel->toTreeData());
}

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	QStringList headers;
	headers << "Title" << "Axis" << "Layout" << "Bounds" << "HWND";

	//writeDefaultData("file.dat");
	treeModel = new TreeModel(headers, readData("file.dat"));

	TreeProxyModel proxyModel;
	proxyModel.setSourceModel(treeModel);

	/*
	MainWindow mainWindow;
	mainWindow.setViewModel(&proxyModel);
	mainWindow.show();

	ViewWindow viewWindow;
	viewWindow.setViewModel(&proxyModel);
	viewWindow.show();
	*/

	qmlRegisterType<TreeModel>("com.shiftystudio.tree", 1, 0, "TreeModel");

	QQuickView* view = new QQuickView;
	view->setResizeMode(QQuickView::SizeRootObjectToView);

	QQmlContext* ctxt = view->rootContext();
	ctxt->setContextProperty("treeModel", treeModel);

	view->setSource(QStringLiteral("qrc:/QMLView.qml"));
	view->show();

	//QObject::connect(&app, &QApplication::lastWindowClosed, handleSave);

	return app.exec();
}
