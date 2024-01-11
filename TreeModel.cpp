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

#include <QtWidgets>

#include "TreeItem.h"
#include "TreeData.h"
#include "TreeModel.h"

TreeModel::TreeModel(QObject* parent)
{

}

TreeModel::TreeModel(const QStringList& headers, const TreeData& data, QObject* parent)
	: QAbstractItemModel(parent)
{
	QVector<QVariant> rootData;
	foreach (QString header, headers)
		rootData << header;

	rootItem = new TreeItem(rootData);
	for(int i = 0; i < data.children.length(); ++i)
	{
		setupModelData(rootItem, data.children.at(i));
	}
}

TreeModel::~TreeModel()
{
	delete rootItem;
}

QHash<int, QByteArray> TreeModel::roleNames() const
{
	QHash<int, QByteArray> roles;

	roles[TreeRoles::TitleRole] = "title";
	roles[TreeRoles::AxisRole] = "axis";
	roles[TreeRoles::LayoutRole] = "layout";
	roles[TreeRoles::BoundsRole] = "bounds";
	roles[TreeRoles::HwndRole] = "hwnd";

	return roles;
}

int TreeModel::columnCount(const QModelIndex & /* parent */) const
{
	return rootItem->columnCount();
}

QVariant TreeModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();

	TreeItem *item = getItem(index);

	switch(role)
	{
		case TreeRoles::TitleRole:
			return item->data(0);
		case TreeRoles::AxisRole:
			return item->data(1);
		case TreeRoles::LayoutRole:
			return item->data(2);
		case TreeRoles::BoundsRole:
			return item->data(3);
		case TreeRoles::HwndRole:
			return item->data(4);
		case Qt::DisplayRole:
		case Qt::EditRole:
			return item->data(index.column());
		default:
			break;
	}

	return QVariant();
}

Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const
{
	if (!index.isValid())
		return nullptr;

	return Qt::ItemIsEditable | QAbstractItemModel::flags(index);
}

TreeItem *TreeModel::getItem(const QModelIndex &index) const
{
	if (index.isValid()) {
		TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
		if (item)
			return item;
	}
	return rootItem;
}

QVariant TreeModel::headerData(int section, Qt::Orientation orientation,
							   int role) const
{
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
		return rootItem->data(section);

	return QVariant();
}

QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent) const
{
	if (parent.isValid() && parent.column() != 0)
		return QModelIndex();

	TreeItem *parentItem = getItem(parent);

	TreeItem *childItem = parentItem->child(row);
	if (childItem)
		return createIndex(row, column, childItem);
	else
		return QModelIndex();
}

bool TreeModel::insertColumns(int position, int columns, const QModelIndex &parent)
{
	bool success;

	beginInsertColumns(parent, position, position + columns - 1);
	success = rootItem->insertColumns(position, columns);
	endInsertColumns();

	return success;
}

bool TreeModel::insertRows(int position, int rows, const QModelIndex &parent)
{
	TreeItem *parentItem = getItem(parent);
	bool success;

	beginInsertRows(parent, position, position + rows - 1);
	success = parentItem->insertChildren(position, rows, rootItem->columnCount());
	endInsertRows();

	return success;
}

QModelIndex TreeModel::parent(const QModelIndex &index) const
{
	if (!index.isValid())
		return QModelIndex();

	TreeItem *childItem = getItem(index);
	TreeItem *parentItem = childItem->parent();

	if (parentItem == rootItem)
		return QModelIndex();

	return createIndex(parentItem->childNumber(), 0, parentItem);
}

bool TreeModel::removeColumns(int position, int columns, const QModelIndex &parent)
{
	bool success;

	beginRemoveColumns(parent, position, position + columns - 1);
	success = rootItem->removeColumns(position, columns);
	endRemoveColumns();

	if (rootItem->columnCount() == 0)
		removeRows(0, rowCount());

	return success;
}

bool TreeModel::removeRows(int position, int rows, const QModelIndex &parent)
{
	TreeItem *parentItem = getItem(parent);
	bool success = true;

	beginRemoveRows(parent, position, position + rows - 1);
	success = parentItem->removeChildren(position, rows);
	endRemoveRows();

	return success;
}

bool TreeModel::moveRows(const QModelIndex& sourceParent, int sourceRow, int count, const QModelIndex& destinationParent, int destinationChild)
{
	TreeItem* sourceItem = getItem(sourceParent);
	TreeItem* destinationItem = getItem(destinationParent);
	int destinationRow = destinationChild;

	if(destinationChild < 0 || destinationChild > destinationItem->childCount() - 1)
	{
		return false;
	}

	if(sourceParent == destinationParent)
	{
		if(sourceRow == destinationRow)
		{
			return false;
		}
		else if(destinationRow > sourceRow)
		{
			destinationRow += 1;
		}
	}

	if(!beginMoveRows(sourceParent, sourceRow, sourceRow + count - 1, destinationParent, destinationRow))
	{
		return false;
	}

	QList<TreeItem*> moveChildren = sourceItem->takeChildren(sourceRow, count);
	destinationItem->insertChildren(destinationChild, moveChildren);

	endMoveRows();

	return true;
}

int TreeModel::rowCount(const QModelIndex &parent) const
{
	TreeItem *parentItem = getItem(parent);

	return parentItem->childCount();
}

bool TreeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	if (role != Qt::EditRole)
		return false;

	TreeItem *item = getItem(index);
	bool result = item->setData(index.column(), value);

	if (result)
		emit dataChanged(index, index);

	return result;
}

bool TreeModel::setHeaderData(int section, Qt::Orientation orientation,
							  const QVariant &value, int role)
{
	if (role != Qt::EditRole || orientation != Qt::Horizontal)
		return false;

	bool result = rootItem->setData(section, value);

	if (result)
		emit headerDataChanged(orientation, section, section);

	return result;
}

void TreeModel::setupModelData(TreeItem* parent, const TreeData& treeData)
{
	parent->insertChildren(parent->childCount(), 1, rootItem->columnCount());
	TreeItem* item = parent->child(parent->childCount() - 1);

	QVariant axis = treeData.axis == Axis::A_None ? QVariant::Invalid : QVariant::fromValue<Axis>(treeData.axis);
	QVariant layout = treeData.layout == Layout::L_None ? QVariant::Invalid : QVariant::fromValue<Layout>(treeData.layout);
	QVariant bounds = treeData.bounds == QRect(0, 0, -1, -1) ? QVariant::Invalid : QVariant::fromValue<QRect>(treeData.bounds);
	QVariant hwnd = treeData.hwnd == nullptr ? QVariant::Invalid : QVariant::fromValue<HWND>(treeData.hwnd);

	item->setData(0, treeData.title);
	item->setData(1, axis);
	item->setData(2, layout);
	item->setData(3, bounds);
	item->setData(4, hwnd);

	for(int i = 0; i < treeData.children.length(); ++i)
	{
		this->setupModelData(item, treeData.children.at(i));
	}
}

TreeData TreeModel::toTreeData()
{
	return this->toTreeData_Internal(rootItem);
}

TreeData TreeModel::toTreeData_Internal(TreeItem* item)
{
	TreeData treeData;

	for(int c = 0; c < item->columnCount(); ++c)
	{
		QVariant data = item->data(c);

		switch(c)
		{
			case 0:
				treeData.title = data.toString();
				break;
			case 1:
				treeData.axis = data == QVariant::Invalid ? Axis::A_None : data.value<Axis>();
				break;
			case 2:
				treeData.layout = data == QVariant::Invalid ? Layout::L_None : data.value<Layout>();
				break;
			case 3:
				treeData.bounds = data == QVariant::Invalid ? QRect(0, 0, -1, -1) : data.toRect();
				break;
			case 4:
				treeData.hwnd = data == QVariant::Invalid ? nullptr : data.value<HWND>();
				break;
		}
	}

	for(int i = 0; i < item->childCount(); ++i)
	{
		treeData.children.append(this->toTreeData_Internal(item->child(i)));
	}

	return treeData;
}
