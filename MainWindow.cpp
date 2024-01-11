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

#include "MainWindow.h"
#include "TreeModel.h"
#include "TreeDelegate.h"
#include "TreeProxyModel.h"

#include <QFile>

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	setupUi(this);

	view->setItemDelegate(new TreeDelegate);

	connect(exitAction, &QAction::triggered, qApp, &QCoreApplication::quit);

	connect(actionsMenu, &QMenu::aboutToShow, this, &MainWindow::updateActions);
	connect(insertRowAction, &QAction::triggered, this, &MainWindow::insertRow);
	connect(removeRowAction, &QAction::triggered, this, &MainWindow::removeRow);
	connect(insertChildAction, &QAction::triggered, this, &MainWindow::insertChild);
	connect(moveRowUpAction, &QAction::triggered, this, &MainWindow::moveRowUp);
	connect(moveRowDownAction, &QAction::triggered, this, &MainWindow::moveRowDown);
}

void MainWindow::setViewModel(QAbstractItemModel *model)
{
	view->setModel(model);

	connect(view->selectionModel(), &QItemSelectionModel::selectionChanged,
			this, &MainWindow::updateActions);

	view->expandAll();
	for (int column = 0; column < model->columnCount(); ++column)
	{
		view->resizeColumnToContents(column);
	}

	updateActions();
}

void MainWindow::insertChild()
{
	QModelIndex index = view->selectionModel()->currentIndex();
	QAbstractItemModel *model = view->model();

	if (model->columnCount(index) == 0) {
		if (!model->insertColumn(0, index))
			return;
	}

	if (!model->insertRow(0, index))
		return;

	for (int column = 0; column < model->columnCount(index); ++column) {
		QModelIndex child = model->index(0, column, index);
		model->setData(child, QVariant("[No data]"), Qt::EditRole);
		if (!model->headerData(column, Qt::Horizontal).isValid())
			model->setHeaderData(column, Qt::Horizontal, QVariant("[No header]"), Qt::EditRole);
	}

	view->selectionModel()->setCurrentIndex(model->index(0, 0, index),
											QItemSelectionModel::ClearAndSelect);
	updateActions();
}

void MainWindow::insertRow()
{
	QModelIndex index = view->selectionModel()->currentIndex();
	QAbstractItemModel *model = view->model();

	if (!model->insertRow(index.row()+1, index.parent()))
		return;

	updateActions();

	for (int column = 0; column < model->columnCount(index.parent()); ++column) {
		QModelIndex child = model->index(index.row()+1, column, index.parent());
		model->setData(child, QVariant("[No data]"), Qt::EditRole);
	}
}

void MainWindow::removeRow()
{
	QModelIndex index = view->selectionModel()->currentIndex();
	QAbstractItemModel *model = view->model();
	if (model->removeRow(index.row(), index.parent()))
		updateActions();
}

void MainWindow::moveRowUp()
{
	QModelIndex index = view->selectionModel()->currentIndex();
	if (view->model()->moveRow(index.parent(), index.row(), index.parent(), index.row() - 1))
		updateActions();
}

void MainWindow::moveRowDown()
{
	QModelIndex index = view->selectionModel()->currentIndex();
	if (view->model()->moveRow(index.parent(), index.row(), index.parent(), index.row() + 1))
		updateActions();
}

void MainWindow::updateActions()
{
	bool hasSelection = !view->selectionModel()->selection().isEmpty();
	removeRowAction->setEnabled(hasSelection);
	moveRowUpAction->setEnabled(hasSelection);
	moveRowDownAction->setEnabled(hasSelection);

	bool hasCurrent = view->selectionModel()->currentIndex().isValid();
	insertRowAction->setEnabled(hasCurrent);

	if (hasCurrent) {
		view->closePersistentEditor(view->selectionModel()->currentIndex());
	}
}

