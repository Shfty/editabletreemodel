#include "TreeDelegate.h"

#include <QPainter>
#include <QComboBox>
#include <QDebug>

#include "Win.h"
#include "TreeEnums.h"
#include "RectEditorWidget.h"

TreeDelegate::TreeDelegate(QObject* parent)
	:QStyledItemDelegate(parent)
{

}

QWidget*TreeDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	QVariant editorData = index.data(Qt::EditRole);

	if(editorData == QVariant::Invalid)
	{
		return nullptr;
	}
	else if(editorData.userType() == qMetaTypeId<QRect>())
	{
		QWidget* editor = new RectEditorWidget(parent);
		return editor;
	}
	else if(editorData.userType() == qMetaTypeId<Axis>())
	{
		QComboBox* editor = new QComboBox(parent);
		editor->insertItem(0, "None", Axis::A_None);
		editor->insertItem(1, "Horizontal", Axis::A_Horizontal);
		editor->insertItem(2, "Vertical", Axis::A_Vertical);
		return editor;
	}
	else if(editorData.userType() == qMetaTypeId<Layout>())
	{
		QComboBox* editor = new QComboBox(parent);
		editor->insertItem(0, "None");
		editor->insertItem(1, "Tabbed");
		editor->insertItem(2, "Split");
		return editor;
	}
	else if(editorData.userType() == qMetaTypeId<HWND>())
	{
		return nullptr;
	}

	return QStyledItemDelegate::createEditor(parent, option, index);
}

void TreeDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
	QVariant modelData = index.data(Qt::EditRole);

	if(modelData.userType() == qMetaTypeId<QRect>())
	{
		QRect rect = qvariant_cast<QRect>(modelData);
		RectEditorWidget* rectEditor = static_cast<RectEditorWidget*>(editor);
		rectEditor->setRect(rect);
	}
	else if(modelData.userType() == qMetaTypeId<Axis>())
	{
		Axis axis = qvariant_cast<Axis>(modelData);

		QComboBox* comboBox = static_cast<QComboBox*>(editor);
		switch(axis)
		{
			case Axis::A_None:
				comboBox->setCurrentIndex(0);
				break;
			case Axis::A_Horizontal:
				comboBox->setCurrentIndex(1);
				break;
			case Axis::A_Vertical:
				comboBox->setCurrentIndex(2);
				break;
			default:
				break;
		}
	}
	else if(modelData.userType() == qMetaTypeId<Layout>())
	{
		Layout layout = qvariant_cast<Layout>(modelData);

		QComboBox* comboBox = static_cast<QComboBox*>(editor);
		switch(layout)
		{
			case Layout::L_None:
				comboBox->setCurrentIndex(0);
				break;
			case Layout::L_Tabbed:
				comboBox->setCurrentIndex(1);
				break;
			case Layout::L_Split:
				comboBox->setCurrentIndex(2);
				break;
			default:
				break;
		}
	}
	else
	{
		QStyledItemDelegate::setEditorData(editor, index);
	}
}

void TreeDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
	QVariant editorData = index.data(Qt::EditRole);

	if(editorData.userType() == qMetaTypeId<QRect>())
	{
		RectEditorWidget* rectEditor = static_cast<RectEditorWidget*>(editor);
		model->setData(index, QVariant::fromValue<QRect>(rectEditor->getRect()));
	}
	else if(editorData.userType() == qMetaTypeId<Axis>())
	{
		QComboBox* comboBox = static_cast<QComboBox*>(editor);
		switch(comboBox->currentIndex())
		{
			case 0:
				model->setData(index, QVariant::fromValue<Axis>(Axis::A_None), Qt::EditRole);
				break;
			case 1:
				model->setData(index, QVariant::fromValue<Axis>(Axis::A_Horizontal), Qt::EditRole);
				break;
			case 2:
				model->setData(index, QVariant::fromValue<Axis>(Axis::A_Vertical), Qt::EditRole);
				break;
			default:
				break;
		}
	}
	else if(editorData.userType() == qMetaTypeId<Layout>())
	{
		QComboBox* comboBox = static_cast<QComboBox*>(editor);
		switch(comboBox->currentIndex())
		{
			case 0:
				model->setData(index, QVariant::fromValue<Layout>(Layout::L_None), Qt::EditRole);
				break;
			case 1:
				model->setData(index, QVariant::fromValue<Layout>(Layout::L_Tabbed), Qt::EditRole);
				break;
			case 2:
				model->setData(index, QVariant::fromValue<Layout>(Layout::L_Split), Qt::EditRole);
				break;
			default:
				break;
		}
	}
	else
	{
		QStyledItemDelegate::setModelData(editor, model, index);
	}
}

void TreeDelegate::updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	editor->setGeometry(option.rect);
}

QSize TreeDelegate::sizeHint(const QStyleOptionViewItem& option, const QModelIndex& index) const
{
	QVariant editorData = index.data(Qt::EditRole);

	if(editorData.userType() == qMetaTypeId<QRect>())
	{
		return QSize(120, 40);
	}
	else if(editorData.userType() == qMetaTypeId<Axis>())
	{
		return QSize(80, 40);
	}
	else if(editorData.userType() == qMetaTypeId<Layout>())
	{
		return QSize(70, 40);
	}

	return QStyledItemDelegate::sizeHint(option, index);
}