#include "TreeData.h"

TreeData::TreeData(QString title, Axis axis, Layout layout, QRect bounds, HWND hwnd):
	title(title),
	axis(axis),
	layout(layout),
	bounds(bounds),
	hwnd(hwnd)
{
}

QDataStream &operator<<(QDataStream &out, const TreeData &treeData)
{
	out << treeData.title << quint32(treeData.axis) << quint32(treeData.layout) << treeData.bounds << quint64(treeData.hwnd) << quint8(treeData.children.length());

	for(int i = 0; i < treeData.children.length(); ++i)
	{
		out << treeData.children.at(i);
	}

	return out;
}

QDataStream &operator>>(QDataStream &in, TreeData &treeData)
{
	QString title;
	quint32 axis;
	quint32 layout;
	QRect bounds;
	quint64 hwnd;
	qint8 childCount;
	in >> title >> axis >> layout >> bounds >> hwnd >> childCount;
	treeData = TreeData(title, (Axis)axis, (Layout)layout, bounds, (HWND)hwnd);

	for(int i = 0; i < childCount; ++i)
	{
		TreeData treeChild;
		in >> treeChild;
		treeData.children.append(treeChild);

	}

	return in;
}
