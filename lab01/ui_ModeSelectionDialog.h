/********************************************************************************
** Form generated from reading UI file 'ModeSelectionDialog.ui'
**
** Created by: Qt User Interface Compiler version 6.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MODESELECTIONDIALOG_H
#define UI_MODESELECTIONDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QAbstractButton>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_ModeSelectionDialog
{
public:
    QVBoxLayout *verticalLayout;
    QRadioButton *radioEdit;
    QRadioButton *radioReadOnly;
    QRadioButton *radioSelection;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *ModeSelectionDialog)
    {
        if (ModeSelectionDialog->objectName().isEmpty())
            ModeSelectionDialog->setObjectName("ModeSelectionDialog");
        ModeSelectionDialog->resize(223, 300);
        QIcon icon(QIcon::fromTheme(QIcon::ThemeIcon::DialogQuestion));
        ModeSelectionDialog->setWindowIcon(icon);
        verticalLayout = new QVBoxLayout(ModeSelectionDialog);
        verticalLayout->setObjectName("verticalLayout");
        radioEdit = new QRadioButton(ModeSelectionDialog);
        radioEdit->setObjectName("radioEdit");

        verticalLayout->addWidget(radioEdit);

        radioReadOnly = new QRadioButton(ModeSelectionDialog);
        radioReadOnly->setObjectName("radioReadOnly");

        verticalLayout->addWidget(radioReadOnly);

        radioSelection = new QRadioButton(ModeSelectionDialog);
        radioSelection->setObjectName("radioSelection");

        verticalLayout->addWidget(radioSelection);

        buttonBox = new QDialogButtonBox(ModeSelectionDialog);
        buttonBox->setObjectName("buttonBox");
        buttonBox->setOrientation(Qt::Orientation::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::StandardButton::Cancel|QDialogButtonBox::StandardButton::Ok);
        buttonBox->setCenterButtons(true);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(ModeSelectionDialog);
        QObject::connect(buttonBox, &QDialogButtonBox::accepted, ModeSelectionDialog, qOverload<>(&QDialog::accept));
        QObject::connect(buttonBox, &QDialogButtonBox::rejected, ModeSelectionDialog, qOverload<>(&QDialog::reject));

        QMetaObject::connectSlotsByName(ModeSelectionDialog);
    } // setupUi

    void retranslateUi(QDialog *ModeSelectionDialog)
    {
        ModeSelectionDialog->setWindowTitle(QCoreApplication::translate("ModeSelectionDialog", "\320\222\321\213\320\261\320\276\321\200 \321\200\320\265\320\266\320\270\320\274\320\260", nullptr));
        radioEdit->setText(QCoreApplication::translate("ModeSelectionDialog", "\320\237\321\200\320\276\321\201\320\274\320\276\321\202\321\200 \321\201 \321\200\320\265\320\264\320\260\320\272\321\202\320\270\321\200\320\276\320\262\320\260\320\275\320\270\320\265\320\274", nullptr));
        radioReadOnly->setText(QCoreApplication::translate("ModeSelectionDialog", "\320\237\321\200\320\276\321\201\320\274\320\276\321\202\321\200", nullptr));
        radioSelection->setText(QCoreApplication::translate("ModeSelectionDialog", "\320\222\321\213\320\261\320\276\321\200 \320\267\320\260\320\277\320\270\321\201\320\270", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ModeSelectionDialog: public Ui_ModeSelectionDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MODESELECTIONDIALOG_H
