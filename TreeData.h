#ifndef TREEDATA_H
#define TREEDATA_H

#include <QtCore>

#include "Win.h"
#include "TreeEnums.h"

class TreeData
{
	public:
		TreeData(QString title = "", Axis axis = Axis::A_None, Layout layout = Layout::L_None, QRect bounds = QRect(0, 0, -1, -1), HWND hwnd = nullptr);

		QString title;
		Axis axis;
		Layout layout;
		QRect bounds;
		HWND hwnd;

		QList<TreeData> children;
};

QDataStream &operator<<(QDataStream &out, const TreeData &treeData);
QDataStream &operator>>(QDataStream &in, TreeData &treeData);

#endif // TREEDATA_H
