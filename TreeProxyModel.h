#ifndef TREEPROXYMODEL_H
#define TREEPROXYMODEL_H

#include <QIdentityProxyModel>

class TreeProxyModel : public QIdentityProxyModel
{
	public:
		TreeProxyModel();

		QVariant data(const QModelIndex &index, int role) const;

		bool moveRows(const QModelIndex &sourceParent, int sourceRow, int count,
					  const QModelIndex &destinationParent, int destinationChild) override;
};

#endif // TREEPROXYMODEL_H