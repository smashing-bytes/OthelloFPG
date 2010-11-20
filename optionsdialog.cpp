#include "optionsdialog.h"
#include "mainwindow.h"
#include <QStackedLayout>
#include <QListWidget>
#include <QDialogButtonBox>
#include <QGroupBox>
#include <QRadioButton>
#include <QCheckBox>
#include <QLabel>
#include <QComboBox>
#include <QSpinBox>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QPlastiqueStyle>
#include <QMotifStyle>
#include <QCleanlooksStyle>
#include <QApplication>
#include <QIcon>

#include <QDebug>

OptionsDialog::OptionsDialog(MainWindow *mWindow, QWidget *parent) :QDialog(parent) {

    mainWindow = mWindow;

    createAppearancePage();
    createGameplayPage();

    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

    listWidget = new QListWidget();
    listWidget->addItem("Appearance");
    listWidget->addItem("Gameplay");

    stackedLayout = new QStackedLayout();
    stackedLayout->addWidget(appearancePage);
    stackedLayout->addWidget(gameplayPage);
    connect(listWidget, SIGNAL(currentRowChanged(int)), stackedLayout, SLOT(setCurrentIndex(int)));

    QGridLayout *mainLayout = new QGridLayout();
    mainLayout->setColumnStretch(0,1);
    mainLayout->setColumnStretch(1,3);
    mainLayout->addWidget(listWidget,0,0);
    mainLayout->addLayout(stackedLayout,0,1);
    mainLayout->addWidget(buttonBox,1,0,1,2);
    setLayout(mainLayout);

    connect(buttonBox, SIGNAL(rejected()),this,SLOT(reject()) );
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(saveChanges()));

    setWindowTitle("Options");
    listWidget->setCurrentRow(0);
}

void OptionsDialog::saveChanges() {
    int style = -1;

    if (plastiqueRdBtn->isChecked()) {
        style = 0;
        QApplication::setStyle(new QPlastiqueStyle);
    }
    else if (motifRdBtn->isChecked()) {
        style = 1;
        QApplication::setStyle(new QMotifStyle);
    }
    else if (cleanlooksRdBtn->isChecked()) {
        style = 2;
        QApplication::setStyle(new QCleanlooksStyle);
    }

     // mainwindow->saveStyle(style);

    /*
        mainwindow->saveToolTipsChoice(tooltipsChBox->isChecked());
        mainwindow->saveSystemTrayChoice(systrayChBox->isChecked());
        mainwindow->saveGameplayOptDialogChoice(gameplayOptAtStChBox->isChecked());
        mainwindow->saveDepth(depthBox->getChoice());
        mainwindow->savePlayersName(nameEdit->getText());
        mainwindow->savePlayersTurn(playersTurnBox->getSellected());
    */

    if (classicRdBtn->isChecked()) {
        mainWindow->setIcons(classicIcons[p1Icon], classicIcons[p2Icon]);
    }
    else if (progRdBtn->isChecked()) {
        mainWindow->setIcons(progIcons[p1Icon], progIcons[p2Icon]);
    }
    else if (osRdBtn->isChecked()) {
        mainWindow->setIcons(osIcons[p1Icon], osIcons[p2Icon]);
    }
    else if (teamsRdBtn->isChecked()) {
        mainWindow->setIcons(teamIcons[p1Icon], teamIcons[p2Icon]);
    }


    emit close();
/*

            /*#include <QPlastiqueStyle>
            #include <QMotifStyle>
            #include <QCleanlooksStyle>

            #include <QApplication>/
   // QApplication::setStyle(new QMotifStyle);
    ///QApplication::setStyle(new QCleanlooksStyle);
 */
}

void OptionsDialog::createAppearancePage() {
    appearancePage = new QWidget();

    styleGroupBox = new QGroupBox("Windows Style");
    plastiqueRdBtn = new QRadioButton("Plastique");
    motifRdBtn = new QRadioButton("Motif");
    cleanlooksRdBtn = new QRadioButton("Cleanlooks");

    showGroupBox = new QGroupBox("Show...");
    systrayChBox = new QCheckBox("System Tray");
    gameplayOptAtStChBox = new QCheckBox("Gameplay Options Dialog At Startup");

    iconsGroupBox = new QGroupBox("Select Pieces");
    labelp1 = new QLabel("Player1 :");
    labelp2 = new QLabel("Player2 :");
    iconsTypeGroupBox = new QGroupBox();
    classicRdBtn = new QRadioButton("Classic");
    classicRdBtn->setObjectName("Crd");
    progRdBtn = new QRadioButton("Programming");
    progRdBtn->setObjectName("Prd");
    osRdBtn = new QRadioButton("OS");
    osRdBtn->setObjectName("Ord");
    teamsRdBtn = new QRadioButton("Teams");
    teamsRdBtn->setObjectName("Trd");
    combop1 = new QComboBox();
    combop1->setObjectName("Combo1");
    combop2 = new QComboBox();
    combop2->setObjectName("Combo2");


    QVBoxLayout *styleLayout = new QVBoxLayout();
    styleLayout->addWidget(plastiqueRdBtn);
    styleLayout->addWidget(motifRdBtn);
    styleLayout->addWidget(cleanlooksRdBtn);
    styleGroupBox->setLayout(styleLayout);


    QVBoxLayout *showLayout = new QVBoxLayout();
    showLayout->addWidget(systrayChBox);
    showLayout->addWidget(gameplayOptAtStChBox);
    showGroupBox->setLayout(showLayout);

    QVBoxLayout *iconTypeLayout = new QVBoxLayout();
    iconTypeLayout->addWidget(classicRdBtn);
    iconTypeLayout->addWidget(osRdBtn);
    iconTypeLayout->addWidget(progRdBtn);
    iconTypeLayout->addWidget(teamsRdBtn);
    iconsTypeGroupBox->setLayout(iconTypeLayout);


    QHBoxLayout *horp1Layout = new QHBoxLayout();
    horp1Layout->addWidget(labelp1);
    horp1Layout->addWidget(combop1);
    QHBoxLayout *horp2Layout = new QHBoxLayout();
    horp2Layout->addWidget(labelp2);
    horp2Layout->addWidget(combop2);


    QVBoxLayout *iconLayout = new QVBoxLayout();
    iconLayout->addWidget(iconsTypeGroupBox);
    iconLayout->addLayout(horp1Layout);
    iconLayout->addLayout(horp2Layout);
    iconsGroupBox->setLayout(iconLayout);

    QVBoxLayout *pageLayout = new QVBoxLayout();
    pageLayout->setMargin(0);
    pageLayout->addWidget(styleGroupBox);
    pageLayout->addWidget(showGroupBox);
    pageLayout->addWidget(iconsGroupBox);
    pageLayout->addStretch();
    appearancePage->setLayout(pageLayout);

    classicRdBtn->setChecked(true);
    plastiqueRdBtn->setChecked(true);

    classicIcons[0] = new QIcon(":/images/black.png");
    classicIcons[1] = new QIcon(":/images/white.png");

    osIcons[0] = new QIcon(":/images/archlinux.png");
    osIcons[1] = new QIcon(":/images/debian.png");
    osIcons[2] = new QIcon(":/images/mac.png");
    osIcons[3] = new QIcon(":/images/windows.png");

    progIcons[0] = new QIcon(":/images/c.png");
    progIcons[1] = new QIcon(":/images/java.png");
    progIcons[2] = new QIcon(":/images/qt.png");

    teamIcons[0] = new QIcon(":/images/aek.png");
    teamIcons[1] = new QIcon(":/images/osfp.png");
    teamIcons[2] = new QIcon(":/images/pao.png");

    combop1->insertItem(0,*classicIcons[0],tr("Black"));
    combop1->insertItem(1,*classicIcons[1],tr("White"));

    combop2->insertItem(0,*classicIcons[1],tr("White"));

    iconType = 0;
    isRemIns = false;
    p1Icon = 0;
    p2Icon = 1;

    connect(combop1, SIGNAL(currentIndexChanged(int)), this, SLOT(updateIconComboBox(int)));
    connect(combop2, SIGNAL(currentIndexChanged(int)), this, SLOT(updateIconComboBox(int)));
    connect(classicRdBtn, SIGNAL(clicked()), this, SLOT(insertIconsAtComboBox()));
    connect(osRdBtn, SIGNAL(clicked()), this, SLOT(insertIconsAtComboBox()));
    connect(progRdBtn, SIGNAL(clicked()), this, SLOT(insertIconsAtComboBox()));
    connect(teamsRdBtn, SIGNAL(clicked()), this, SLOT(insertIconsAtComboBox()));
}

void OptionsDialog::createGameplayPage() {
    gameplayPage = new QWidget();

    aiGroupBox = new QGroupBox("AI Options");
    depthL = new QLabel("Max Depth :");
    depthBox = new QSpinBox();

    playerBox = new QGroupBox("Player Options");
    nameL = new QLabel("Nickname :");
    nameEdit = new QLineEdit();
    playersTurn = new QLabel("Player's Turn :");
    playersTurnBox = new QComboBox();

    QHBoxLayout *aiLayout = new QHBoxLayout();
    aiLayout->addWidget(depthL);
    aiLayout->addWidget(depthBox);
    aiGroupBox->setLayout(aiLayout);

    QHBoxLayout *nameLayout = new QHBoxLayout();
    nameLayout->addWidget(nameL);
    nameLayout->addWidget(nameEdit);

    QHBoxLayout *turnLayout = new QHBoxLayout();
    turnLayout->addWidget(playersTurn);
    turnLayout->addWidget(playersTurnBox);

    QVBoxLayout *playerLayout = new QVBoxLayout();
    playerLayout->addLayout(nameLayout);
    playerLayout->addLayout(turnLayout);

    playerBox->setLayout(playerLayout);

    QVBoxLayout *gameplayLayout = new QVBoxLayout();
    gameplayLayout->setMargin(0);
    gameplayLayout->addWidget(aiGroupBox);
    gameplayLayout->addWidget(playerBox);
    gameplayLayout->addStretch();

    gameplayPage->setLayout(gameplayLayout);

    nameEdit->setText("Player1");
    //playersTurnBox->set
}

void OptionsDialog::updateIconComboBox(int index) {
    if (isRemIns == true)
        return;

    QString objName = QObject::sender()->objectName();

    if (objName.endsWith("2")) {
        p2Icon = index;
        return;
    }

    QComboBox *cBoxR = combop2;

    if (iconType == 0) {
        if (index == 0) {
            cBoxR->setItemIcon(0, *classicIcons[1]);
           cBoxR->setItemText(0, tr("White"));
        }
        else {
            cBoxR->setItemIcon(0, *classicIcons[0]);
            cBoxR->setItemText(0, tr("Black"));
        }
    }
    else if (iconType == 1) {
        if (index == 0) {
            cBoxR->setItemIcon(0, *osIcons[1]);
            cBoxR->setItemText(0, tr("Debian"));
            cBoxR->setItemIcon(1, *osIcons[2]);
            cBoxR->setItemText(1, tr("MacOS"));
            cBoxR->setItemIcon(2, *osIcons[3]);
            cBoxR->setItemText(2, tr("Windows"));
        } else if (index == 1) {
            cBoxR->setItemIcon(0, *osIcons[0]);
            cBoxR->setItemText(0, tr("ArchLinux"));
            cBoxR->setItemIcon(1, *osIcons[2]);
            cBoxR->setItemText(1, tr("MacOS"));
            cBoxR->setItemIcon(2, *osIcons[3]);
            cBoxR->setItemText(2, tr("Windows"));
        } else if (index == 2) {
            cBoxR->setItemIcon(0, *osIcons[0]);
            cBoxR->setItemText(0, tr("ArchLinux"));
            cBoxR->setItemIcon(1, *osIcons[1]);
            cBoxR->setItemText(1, tr("Debian"));
            cBoxR->setItemIcon(2, *osIcons[3]);
            cBoxR->setItemText(2, tr("Windows"));
        } else {
            cBoxR->setItemIcon(0, *osIcons[0]);
            cBoxR->setItemText(0, tr("ArchLinux"));
            cBoxR->setItemIcon(1, *osIcons[1]);
            cBoxR->setItemText(1, tr("Debian"));
            cBoxR->setItemIcon(2, *osIcons[2]);
            cBoxR->setItemText(2, tr("MacOS"));
        }
    } else if (iconType == 2) {
        if (index == 0) {
            cBoxR->setItemIcon(0, *progIcons[1]);
            cBoxR->setItemText(0, tr("Java"));
            cBoxR->setItemIcon(1, *progIcons[2]);
            cBoxR->setItemText(1, tr("Qt"));
        } else if (index == 1) {
            cBoxR->setItemIcon(0, *progIcons[0]);
            cBoxR->setItemText(0, tr("C"));
            cBoxR->setItemIcon(1, *progIcons[2]);
            cBoxR->setItemText(1, tr("Qt"));
        } else {
            cBoxR->setItemIcon(0, *progIcons[0]);
            cBoxR->setItemText(0, tr("C"));
            cBoxR->setItemIcon(1, *progIcons[1]);
            cBoxR->setItemText(1, tr("Java"));
        }
    } else {
        if (index == 0) {
            cBoxR->setItemIcon(0, *teamIcons[1]);
            cBoxR->setItemText(0, tr("OSFP"));
            cBoxR->setItemIcon(1, *teamIcons[2]);
            cBoxR->setItemText(1, tr("PAO"));
        } else if (index == 1) {
            cBoxR->setItemIcon(0, *teamIcons[0]);
            cBoxR->setItemText(0, tr("AEK"));
            cBoxR->setItemIcon(1, *teamIcons[2]);
            cBoxR->setItemText(1, tr("PAO"));
        } else {
            cBoxR->setItemIcon(0, *teamIcons[0]);
            cBoxR->setItemText(0, tr("AEK"));
            cBoxR->setItemIcon(1, *teamIcons[1]);
            cBoxR->setItemText(1, tr("OSFP"));
        }
    }
    p1Icon = index;
}

void OptionsDialog::insertIconsAtComboBox() {
    isRemIns = true;
    QString objName = QObject::sender()->objectName();
    if (objName.compare("Crd") == 0) {
        clearIconComboBoxes();
        combop1->insertItem(0,*classicIcons[0],tr("Black"));
        combop1->insertItem(1,*classicIcons[1],tr("White"));
        combop2->insertItem(0,*classicIcons[1],tr("White"));
        iconType = 0;
    } else if (objName.compare("Ord") == 0) {
        clearIconComboBoxes();
        combop1->insertItem(0, *osIcons[0], tr("ArchLinux"));
        combop1->insertItem(1, *osIcons[1], tr("Debian"));
        combop1->insertItem(2, *osIcons[2], tr("MacOS"));
        combop1->insertItem(3, *osIcons[3], tr("Windows"));
        combop2->insertItem(0, *osIcons[1], tr("Debian"));
        combop2->insertItem(1, *osIcons[2], tr("MacOS"));
        combop2->insertItem(2, *osIcons[3], tr("Windows"));
        iconType = 1;
    }
    else if (objName.compare("Prd") == 0) {
        clearIconComboBoxes();
        combop1->insertItem(0, *progIcons[0], tr("C"));
        combop1->insertItem(1, *progIcons[1], tr("Java"));
        combop1->insertItem(2, *progIcons[2], tr("Qt"));
        combop2->insertItem(0,*progIcons[1], tr("Java"));
        combop2->insertItem(1,*progIcons[2], tr("Qt"));
        iconType = 2;
    } else if (objName.compare("Trd") == 0) {
        clearIconComboBoxes();
        combop1->insertItem(0, *teamIcons[0], tr("AEK"));
        combop1->insertItem(1, *teamIcons[1], tr("OSFP"));
        combop1->insertItem(2, *teamIcons[2], tr("PAO"));
        combop2->insertItem(0,*teamIcons[1], tr("OSFP"));
        combop2->insertItem(1,*teamIcons[2], tr("PAO"));
        iconType = 3;
    }

    isRemIns = false;

}

void OptionsDialog::clearIconComboBoxes() {

    if (iconType == 0) {
        combop1->removeItem(1);
        combop1->removeItem(0);
        combop2->removeItem(0);
    } else if (iconType == 1) {
        combop1->removeItem(3);
        for (int i = 2; i >= 0; i--) {
            combop1->removeItem(i);
            combop2->removeItem(i);
        }
    } else if (iconType == 2) {
        combop1->removeItem(2);
        for (int i = 1; i >= 0; i--) {
            combop1->removeItem(i);
            combop2->removeItem(i);
        }
    } else {
        combop1->removeItem(2);
        for (int i = 1; i >= 0; i--) {
            combop1->removeItem(i);
            combop2->removeItem(i);
        }
    }
}
