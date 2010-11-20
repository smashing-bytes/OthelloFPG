#ifndef OPTIONSDIALOG_H
#define OPTIONSDIALOG_H

#include <QDialog>

class QStackedLayout;
class QListWidget;
class QDialogButtonBox;
class QGroupBox;
class QRadioButton;
class QCheckBox;
class QLabel;
class QComboBox;
class QSpinBox;
class QLineEdit;
class QIcon;
class MainWindow;

class OptionsDialog : public QDialog {
    Q_OBJECT
public:
    OptionsDialog(MainWindow *mWindow,QWidget *parent = 0);

private slots:
    void saveChanges();
    void updateIconComboBox(int);
    void insertIconsAtComboBox();

private:
    void clearIconComboBoxes();
    void createAppearancePage();
    void createGameplayPage();

    MainWindow *mainWindow;

    QStackedLayout *stackedLayout;
    QListWidget *listWidget;
    QDialogButtonBox *buttonBox;

    QWidget *appearancePage;
    QWidget *gameplayPage;

    QGroupBox *styleGroupBox;
    QRadioButton *plastiqueRdBtn;
    QRadioButton *motifRdBtn;
    QRadioButton *cleanlooksRdBtn;

    QGroupBox *showGroupBox;
    QCheckBox *systrayChBox;
    QCheckBox *gameplayOptAtStChBox;

    QGroupBox *iconsGroupBox;
    QLabel *labelp1;
    QLabel *labelp2;
    QGroupBox *iconsTypeGroupBox;
    QRadioButton *classicRdBtn;
    QRadioButton *progRdBtn;
    QRadioButton *osRdBtn;
    QRadioButton *teamsRdBtn;
    QComboBox *combop1;
    QComboBox *combop2;

    QGroupBox *aiGroupBox;
    QLabel *depthL;
    QSpinBox *depthBox;

    QGroupBox *playerBox;
    QLabel *nameL;
    QLineEdit *nameEdit;
    QLabel *playersTurn;
    QComboBox *playersTurnBox;

    int p1Icon,p2Icon;
    int iconType;
    bool isRemIns;
    QIcon *classicIcons[2];
    QIcon *osIcons[4];
    QIcon *progIcons[3];
    QIcon *teamIcons[3];

};

#endif // OPTIONSDIALOG_H
