#include "RectEditorWidget.h"

#include <QBoxLayout>
#include <QLabel>
#include <QSpinBox>

RectEditorWidget::RectEditorWidget(QWidget *parent) : QWidget(parent)
{
	setAutoFillBackground(true);

	// Position Widgets
	QWidget* posWrap = new QWidget;
	QLabel* posLabel = new QLabel("Pos");

	spinBoxX = new QSpinBox;
	spinBoxX->setMinimum(-10000);
	spinBoxX->setMaximum(10000);

	spinBoxY = new QSpinBox;
	spinBoxY->setMinimum(-10000);
	spinBoxY->setMaximum(10000);

	QHBoxLayout* posLayout = new QHBoxLayout(posWrap);
	posLayout->setMargin(0);
	posLayout->setContentsMargins(0, 0, 0, 0);
	posLayout->setSpacing(0);
	posLayout->addWidget(posLabel);
	posLayout->addWidget(spinBoxX);
	posLayout->addWidget(spinBoxY);

	// Size Widgets
	QWidget* sizeWrap = new QWidget;
	QLabel* sizeLabel = new QLabel("Size");
	spinBoxW = new QSpinBox;
	spinBoxW->setMinimum(0);
	spinBoxW->setMaximum(10000);
	spinBoxH = new QSpinBox;
	spinBoxH->setMinimum(0);
	spinBoxH->setMaximum(10000);

	QHBoxLayout* sizeLayout = new QHBoxLayout(sizeWrap);
	sizeLayout->setMargin(0);
	sizeLayout->setContentsMargins(0, 0, 0, 0);
	sizeLayout->setSpacing(0);
	sizeLayout->addWidget(sizeLabel);
	sizeLayout->addWidget(spinBoxW);
	sizeLayout->addWidget(spinBoxH);

	// Main layout
	QVBoxLayout* editorLayout = new QVBoxLayout(this);
	editorLayout->setMargin(0);
	editorLayout->setContentsMargins(0, 0, 0, 0);
	editorLayout->setSpacing(0);
	editorLayout->addWidget(posWrap);
	editorLayout->addWidget(sizeWrap);
}

void RectEditorWidget::setRect(QRect rect)
{
	spinBoxX->setValue(rect.x());
	spinBoxY->setValue(rect.y());
	spinBoxW->setValue(rect.width());
	spinBoxH->setValue(rect.height());
}

QRect RectEditorWidget::getRect()
{
	QRect rect;

	rect.setX(spinBoxX->value());
	rect.setY(spinBoxY->value());
	rect.setWidth(spinBoxW->value());
	rect.setHeight(spinBoxH->value());

	return rect;
}
