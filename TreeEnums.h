#ifndef TREEENUMS_H
#define TREEENUMS_H

enum Axis {
	A_None = -1,
	A_Horizontal,
	A_Vertical
};

Q_DECLARE_METATYPE(Axis)

enum Layout {
	L_None = -1,
	L_Tabbed,
	L_Split
};

Q_DECLARE_METATYPE(Layout)

#endif // TREEENUMS_H
