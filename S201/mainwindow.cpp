#include "mainwindow.h"
#include "section.h"
#include <QDialog>
#include <QDialogButtonBox>
#include <QCheckBox>

// constructeur de la fenêtre principale, parent c'est le widget père (null par défaut)
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // on crée le menu "Fichier" dans la barre de menu
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));

    // les différentes actions du menu
    QAction *openJsonAction = new QAction(tr("Open JSON"), this);
    QAction *openXmlAction  = new QAction(tr("Open XML"), this);
    QAction *saveAction     = new QAction(tr("Save"), this);
    QAction *saveAsAction   = new QAction(tr("Save As"), this);
    QAction *quitAction     = new QAction(tr("Quit"), this);

    // on ajoute toutes les actions au menu, avec des séparateurs pour faire propre
    fileMenu->addAction(openJsonAction);
    fileMenu->addAction(openXmlAction);
    fileMenu->addSeparator();
    fileMenu->addAction(saveAction);
    fileMenu->addAction(saveAsAction);
    fileMenu->addSeparator();
    fileMenu->addAction(quitAction);

    // connexion des signaux aux slots correspondants
    connect(quitAction,     &QAction::triggered, this, &QMainWindow::close);
    connect(openJsonAction, &QAction::triggered, this, &MainWindow::onOpenJson);
    connect(openXmlAction,  &QAction::triggered, this, &MainWindow::onOpenXml);
    connect(saveAction,     &QAction::triggered, this, &MainWindow::onSave);
    connect(saveAsAction,   &QAction::triggered, this, &MainWindow::onSaveAs);

    // splitter pour diviser la fenêtre en deux parties (liste à gauche, détails à droite)
    QSplitter *splitter = new QSplitter(this);
    setCentralWidget(splitter);

    // les deux panneaux
    QWidget *master = new QWidget(); // panneau gauche avec la liste des étudiants
    QWidget *detail = new QWidget(); // panneau droit avec le formulaire de détail

    splitter->addWidget(master);
    splitter->addWidget(detail);
    // taille initiale : 300px pour la liste, 600px pour le détail
    splitter->setSizes(QList<int>() << 300 << 600);

    // layout vertical pour le panneau gauche
    QVBoxLayout *layoutMaster = new QVBoxLayout();
    master->setLayout(layoutMaster);

    // champ de filtre et liste des étudiants
    filtre       = new QLineEdit();
    liste        = new QListWidget();

    // boutons pour ajouter/supprimer un étudiant
    btnAddStudent    = new QPushButton(tr("Add Student"));
    btnDeleteStudent = new QPushButton(tr("Delete Student"));

    layoutMaster->addWidget(filtre);
    layoutMaster->addWidget(liste);
    layoutMaster->addWidget(btnAddStudent);
    layoutMaster->addWidget(btnDeleteStudent);

    // layout formulaire pour le panneau de droite
    QFormLayout *layoutDetail = new QFormLayout();
    // pour que les champs s'étendent bien horizontalement
    layoutDetail->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
    detail->setLayout(layoutDetail);

    // tous les champs du formulaire étudiant
    lastName        = new QLineEdit();
    firstName       = new QLineEdit();
    birthDate       = new QDateEdit();
    studentId       = new QLineEdit();
    ineNumber       = new QLineEdit();
    phoneNumber     = new QLineEdit();
    amuEmail        = new QLineEdit();
    personalEmail   = new QLineEdit();
    personalAddress = new QLineEdit();
    parentAddress   = new QLineEdit();
    photoPath       = new QLabel();       // label pour afficher la photo
    btnLoadPhoto    = new QPushButton(tr("Browse..."));
    btnSaveStudent  = new QPushButton(tr("Save"));

    // combo box pour la section (les différentes promos dispo)
    section = new QComboBox();
    section->addItems({"Info BUT 1", "Info BUT 2", "Info BUT 3", "MMI BUT 1", "MMI BUT 2", "MMI BUT 3"});

    // tableau pour afficher les absences de l'étudiant sélectionné
    absencesTable = new QTableWidget(0, 3); // 0 ligne au début, 3 colonnes
    absencesTable->setHorizontalHeaderLabels({tr("Date"), tr("Reason"), tr("Status")});
    absencesTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    absencesTable->setSelectionBehavior(QAbstractItemView::SelectRows); // sélection par ligne entière
    absencesTable->setEditTriggers(QAbstractItemView::NoEditTriggers);  // pas d'édition directe dans le tableau

    // boutons pour gérer les absences
    btnAddAbsence    = new QPushButton(tr("Add"));
    btnEditAbsence   = new QPushButton(tr("Edit"));
    btnDeleteAbsence = new QPushButton(tr("Delete"));

    // on met les boutons d'absence dans un layout horizontal
    QHBoxLayout *absenceButtonsLayout = new QHBoxLayout();
    absenceButtonsLayout->addWidget(btnAddAbsence);
    absenceButtonsLayout->addWidget(btnEditAbsence);
    absenceButtonsLayout->addWidget(btnDeleteAbsence);

    // ajout de toutes les lignes dans le formulaire détail
    layoutDetail->addRow(tr("Last Name:"),       lastName);
    layoutDetail->addRow(tr("First Name:"),      firstName);
    layoutDetail->addRow(tr("Birth Date:"),      birthDate);
    layoutDetail->addRow(tr("Student ID:"),      studentId);
    layoutDetail->addRow(tr("INE Number:"),      ineNumber);
    layoutDetail->addRow(tr("Phone Number:"),    phoneNumber);
    layoutDetail->addRow(tr("AMU Email:"),       amuEmail);
    layoutDetail->addRow(tr("Personal Email:"),  personalEmail);
    layoutDetail->addRow(tr("Personal Address:"),personalAddress);
    layoutDetail->addRow(tr("Parent Address:"),  parentAddress);
    layoutDetail->addRow(tr("Photo Path:"),      photoPath);
    layoutDetail->addRow("",                     btnLoadPhoto);
    layoutDetail->addRow(tr("Section:"),         section);
    layoutDetail->addRow(tr("Absences:"),        absencesTable);
    layoutDetail->addRow("",                     absenceButtonsLayout);
    layoutDetail->addRow(btnSaveStudent);

    // on remplit la liste avec des données de test pour pas avoir une app vide au démarrage
    studentManager.populateWithDefaultTestData();
    populateMasterList();
    setupConnections();
    resize(900, 600); // taille par défaut de la fenêtre
}

// destructeur (Qt gère la mémoire des widgets fils automatiquement)
MainWindow::~MainWindow()
{
}

// remplit la QListWidget avec les noms des étudiants
void MainWindow::populateMasterList()
{
    liste->clear();
    const QList<Student> etudiants = studentManager.getStudents();

    for (const Student& s : etudiants)
    {
        liste->addItem(s.getDisplayName());
    }
}

// connecte tous les signaux/slots de l'interface
void MainWindow::setupConnections()
{
    // quand on clique sur un étudiant dans la liste, on affiche ses infos
    connect(liste,           &QListWidget::currentRowChanged, this, &MainWindow::onStudentSelected);
    // filtre en temps réel pendant la saisie
    connect(filtre,          &QLineEdit::textChanged,         this, &MainWindow::onFilterChanged);

    connect(btnSaveStudent,  &QPushButton::clicked, this, &MainWindow::onSaveStudent);
    connect(btnAddStudent,   &QPushButton::clicked, this, &MainWindow::onAddStudent);
    connect(btnDeleteStudent,&QPushButton::clicked, this, &MainWindow::onDeleteStudent);
    connect(btnLoadPhoto,    &QPushButton::clicked, this, &MainWindow::onLoadPhoto);

    connect(btnAddAbsence,   &QPushButton::clicked, this, &MainWindow::onAddAbsence);
    connect(btnEditAbsence,  &QPushButton::clicked, this, &MainWindow::onEditAbsence);
    connect(btnDeleteAbsence,&QPushButton::clicked, this, &MainWindow::onDeleteAbsence);
}

// remplit le formulaire de droite avec les données de l'étudiant à l'index donné
void MainWindow::populateDetailForm(int index)
{
    Student s = studentManager.getStudent(index);

    // on met à jour tous les champs avec les infos de l'étudiant
    lastName->setText(s.getLastName());
    firstName->setText(s.getFirstName());
    birthDate->setDate(s.getBirthDate());
    studentId->setText(s.getStudentId());
    ineNumber->setText(s.getIneNumber());
    phoneNumber->setText(s.getPhoneNumber());
    amuEmail->setText(s.getAmuEmail());
    personalEmail->setText(s.getPersonalEmail());
    personalAddress->setText(s.getPersonalAddress());
    parentAddress->setText(s.getParentAddress());
    section->setCurrentText(sectionToString(s.getSection()));

    // gestion de la photo
    QString path = s.getPhotoPath();
    photoPath->setProperty("imagePath", path); // on stocke le chemin dans une propriété du label

    if (path.isEmpty()) {
        photoPath->clear();
        photoPath->setText(tr("No photo"));
    } else {
        QPixmap pix(path);
        if (!pix.isNull()) {
            // on redimensionne la photo en gardant les proportions
            photoPath->setPixmap(pix.scaled(150, 150, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        } else {
            // le chemin existe mais l'image est pas lisible
            photoPath->clear();
            photoPath->setText(tr("Image error"));
        }
    }

    // on vide et reremplit le tableau des absences
    absencesTable->setRowCount(0);
    const QList<Absence> absences = s.getAbsences();
    for (int i = 0; i < absences.size(); ++i) {
        absencesTable->insertRow(i);
        absencesTable->setItem(i, 0, new QTableWidgetItem(absences[i].getDate().toString("yyyy-MM-dd")));
        absencesTable->setItem(i, 1, new QTableWidgetItem(absences[i].getReason()));
        // on affiche "Justified" ou "Unjustified" selon le bool
        QString status = absences[i].isJustified() ? tr("Justified") : tr("Unjustified");
        absencesTable->setItem(i, 2, new QTableWidgetItem(status));
    }
}

// remet tous les champs du formulaire à zéro
void MainWindow::clearDetailForm()
{
    lastName->clear();
    firstName->clear();
    birthDate->setDate(QDate::currentDate()); // date du jour par défaut
    studentId->clear();
    ineNumber->clear();
    phoneNumber->clear();
    amuEmail->clear();
    personalEmail->clear();
    personalAddress->clear();
    parentAddress->clear();
    photoPath->clear();
    photoPath->setProperty("imagePath", ""); // on remet le chemin à vide aussi
    section->setCurrentIndex(0);
    absencesTable->setRowCount(0);
}

// vérifie s'il y a des modifications non sauvegardées et demande quoi faire
// retourne true si on peut continuer, false si l'utilisateur a annulé
bool MainWindow::checkUnsavedChanges()
{
    if (!studentManager.isModified()) return true; // rien à sauvegarder, on continue

    QMessageBox::StandardButton ret = QMessageBox::warning(
        this, tr("Pending modifications"),
        tr("Unsaved changes.\nDo you want to save them before leaving?"),
        QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel
        );

    if (ret == QMessageBox::Save) {
        onSave();
        return true;
    } else if (ret == QMessageBox::Cancel) {
        // l'utilisateur veut pas quitter
        return false;
    }

    // cas Discard : on perd les modifs, c'est voulu
    return true;
}

// intercepte la fermeture de la fenêtre pour prévenir si y'a des modifs pas sauvées
void MainWindow::closeEvent(QCloseEvent *event)
{
    if (checkUnsavedChanges()) {
        event->accept(); // ok on ferme
    } else {
        event->ignore(); // on annule la fermeture
    }
}

// pour la traduction de l'interface (pas encore implémenté)
void MainWindow::retranslateUi()
{
}

// ouvre un fichier JSON via une boîte de dialogue
void MainWindow::onOpenJson()
{
    if (!checkUnsavedChanges()) return;

    QString fileName = QFileDialog::getOpenFileName(this, tr("Open JSON"), "", tr("JSON Files (*.json)"));
    if (fileName.isEmpty()) return; // l'utilisateur a annulé

    if (studentManager.loadJson(fileName)) {
        populateMasterList();
        clearDetailForm();
    } else {
        QMessageBox::warning(this, tr("Error"), tr("Cannot read this JSON file."));
    }
}

// pareil mais pour XML
void MainWindow::onOpenXml()
{
    if (!checkUnsavedChanges()) return;

    QString fileName = QFileDialog::getOpenFileName(this, tr("Open XML"), "", tr("XML Files (*.xml)"));
    if (fileName.isEmpty()) return;

    if (studentManager.loadXml(fileName)) {
        populateMasterList();
        clearDetailForm();
    } else {
        QMessageBox::warning(this, tr("Error"), tr("Cannot read this XML file."));
    }
}

// sauvegarde dans le fichier courant (si y'en a pas, appelle onSaveAs)
void MainWindow::onSave()
{
    QString currentPath = studentManager.getCurrentFilePath();

    if (currentPath.isEmpty()) {
        // pas encore de fichier associé, on redirige vers "Enregistrer sous"
        onSaveAs();
        return;
    }

    bool success = false;

    // on choisit le bon format selon l'extension du fichier
    if (currentPath.endsWith(".xml", Qt::CaseInsensitive)) {
        success = studentManager.saveXml(currentPath);
    } else {
        success = studentManager.saveJson(currentPath);
    }

    if (success) {
        QMessageBox::information(this, tr("Success"), tr("File updated."));
        studentManager.setModified(false); // plus de modifs en attente
    } else {
        QMessageBox::warning(this, tr("Error"), tr("Cannot update the file."));
    }
}

// enregistrer sous : demande un chemin à l'utilisateur
void MainWindow::onSaveAs()
{
    QString filter   = tr("JSON Files (*.json);;XML Files (*.xml)");
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save As..."), "", filter);

    if (fileName.isEmpty()) return;

    bool success = false;

    if (fileName.endsWith(".xml", Qt::CaseInsensitive)) {
        success = studentManager.saveXml(fileName);
    } else {
        // si l'utilisateur n'a pas mis d'extension, on ajoute .json
        if (!fileName.endsWith(".json", Qt::CaseInsensitive))
            fileName += ".json";
        success = studentManager.saveJson(fileName);
    }

    if (success) {
        QMessageBox::information(this, tr("Success"), tr("File saved successfully."));
        studentManager.setModified(false);
    } else {
        QMessageBox::warning(this, tr("Error"), tr("Cannot save the file."));
    }
}

// appelé quand on clique sur un étudiant dans la liste
void MainWindow::onStudentSelected(int index)
{
    if (index < 0) return; // rien de sélectionné
    populateDetailForm(index);
}

// filtre la liste selon le texte tapé dans le champ filtre
void MainWindow::onFilterChanged(QString text)
{
    QList<int> matchingIndices = studentManager.filter(text);

    // on cache les étudiants qui correspondent pas au filtre
    for (int i = 0; i < liste->count(); ++i) {
        liste->item(i)->setHidden(!matchingIndices.contains(i));
    }
}

// ajoute un nouvel étudiant avec des valeurs par défaut
void MainWindow::onAddStudent()
{
    Student s;
    s.setLastName("New");
    s.setFirstName("Student");

    studentManager.addStudent(s);
    populateMasterList();

    // on sélectionne automatiquement le nouvel étudiant (dernier de la liste)
    liste->setCurrentRow(liste->count() - 1);
    studentManager.setModified(true);
}

// supprime l'étudiant actuellement sélectionné
void MainWindow::onDeleteStudent()
{
    int index = liste->currentRow();
    if (index < 0) return; // rien de sélectionné, on fait rien

    studentManager.removeStudent(index);
    populateMasterList();
    clearDetailForm();
    studentManager.setModified(true);
}

// sauvegarde les modifications du formulaire dans le studentManager
void MainWindow::onSaveStudent()
{
    int index = liste->currentRow();
    if (index < 0) return;

    Student s = studentManager.getStudent(index);
    Student oldS = s; // on garde une copie pour comparer après

    // on récupère toutes les valeurs des champs
    s.setLastName(lastName->text());
    s.setFirstName(firstName->text());
    s.setBirthDate(birthDate->date());
    s.setStudentId(studentId->text());
    s.setIneNumber(ineNumber->text());
    s.setPhoneNumber(phoneNumber->text());
    s.setAmuEmail(amuEmail->text());
    s.setPersonalEmail(personalEmail->text());
    s.setPersonalAddress(personalAddress->text());
    s.setParentAddress(parentAddress->text());
    s.setPhotoPath(photoPath->property("imagePath").toString());
    s.setSection(sectionFromString(section->currentText()));

    // on met à jour seulement si y'a vraiment eu des changements
    if (oldS.toJson() != s.toJson()) {
        studentManager.updateStudent(index, s);
        liste->currentItem()->setText(s.getDisplayName()); // met à jour le nom dans la liste
        studentManager.setModified(true);
    }
}

// ouvre un explorateur de fichiers pour choisir une photo
void MainWindow::onLoadPhoto()
{
    int index = liste->currentRow();
    if (index < 0) return;

    QString filter   = tr("Images (*.png *.jpg *.jpeg)");
    QString fileName = QFileDialog::getOpenFileName(this, tr("Choose a profile picture"), "", filter);

    if (fileName.isEmpty()) return;

    Student s = studentManager.getStudent(index);
    s.setPhotoPath(fileName);

    studentManager.updateStudent(index, s);
    photoPath->setProperty("imagePath", fileName); // on mémorise le chemin

    // affichage de l'aperçu de la photo
    QPixmap pix(fileName);
    photoPath->setPixmap(pix.scaled(150, 150, Qt::KeepAspectRatio, Qt::SmoothTransformation));

    studentManager.setModified(true);
}

// ajoute une absence à l'étudiant sélectionné via une boîte de dialogue
void MainWindow::onAddAbsence()
{
    int index = liste->currentRow();
    if (index < 0) return;

    // on crée une petite fenêtre de dialogue pour saisir l'absence
    QDialog dialog(this);
    dialog.setWindowTitle(tr("New Absence"));
    QFormLayout form(&dialog);

    QDateEdit dateEdit(QDate::currentDate(), &dialog);
    dateEdit.setCalendarPopup(true); // calendrier graphique pour choisir la date
    QCheckBox justifiedCheck(tr("Justified"), &dialog);
    QLineEdit reasonEdit(&dialog);

    form.addRow(tr("Date:"), &dateEdit);
    form.addRow(tr("Status:"), &justifiedCheck);
    form.addRow(tr("Reason:"), &reasonEdit);

    // boutons OK / Annuler
    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dialog);
    form.addRow(&buttonBox);

    connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(&buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    // on ajoute l'absence seulement si l'utilisateur a validé
    if (dialog.exec() == QDialog::Accepted) {
        Student s = studentManager.getStudent(index);
        s.addAbsence(Absence(dateEdit.date(), justifiedCheck.isChecked(), reasonEdit.text()));
        studentManager.updateStudent(index, s);
        populateDetailForm(index); // on rafraîchit l'affichage
        studentManager.setModified(true);
    }
}

// modifie une absence existante sélectionnée dans le tableau
void MainWindow::onEditAbsence()
{
    int sIndex = liste->currentRow();       // index de l'étudiant
    int aIndex = absencesTable->currentRow(); // index de l'absence
    if (sIndex < 0 || aIndex < 0) return;

    Student s = studentManager.getStudent(sIndex);
    Absence a = s.getAbsences().at(aIndex);

    // même principe que onAddAbsence mais on pré-remplit avec les valeurs existantes
    QDialog dialog(this);
    dialog.setWindowTitle(tr("Edit Absence"));
    QFormLayout form(&dialog);

    QDateEdit dateEdit(a.getDate(), &dialog);
    dateEdit.setCalendarPopup(true);
    QCheckBox justifiedCheck(tr("Justified"), &dialog);
    justifiedCheck.setChecked(a.isJustified());
    QLineEdit reasonEdit(a.getReason(), &dialog);

    form.addRow(tr("Date:"), &dateEdit);
    form.addRow(tr("Status:"), &justifiedCheck);
    form.addRow(tr("Reason:"), &reasonEdit);

    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dialog);
    form.addRow(&buttonBox);

    connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(&buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted) {
        // on met à jour les champs de l'absence
        a.setDate(dateEdit.date());
        a.setJustified(justifiedCheck.isChecked());
        a.setReason(reasonEdit.text());

        s.editAbsence(aIndex, a);
        studentManager.updateStudent(sIndex, s);
        populateDetailForm(sIndex);
        studentManager.setModified(true);
    }
}

// supprime l'absence sélectionnée dans le tableau
void MainWindow::onDeleteAbsence()
{
    int sIndex = liste->currentRow();
    int aIndex = absencesTable->currentRow();
    if (sIndex < 0 || aIndex < 0) return; // faut qu'un étudiant ET une absence soient sélectionnés

    Student s = studentManager.getStudent(sIndex);
    s.removeAbsence(aIndex);

    studentManager.updateStudent(sIndex, s);
    populateDetailForm(sIndex); // on rafraîchit pour que la ligne disparaisse du tableau
    studentManager.setModified(true);
}