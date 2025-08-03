#include "mainwindow.h"

#include <QButtonGroup>
#include <QFileDialog>
#include <QFont>
#include <QMessageBox>
#include <QtCore/QtCore>
#include <QtCore>
#include <iostream>

#include "./ui_mainwindow.h"
#include "analyze.h"
#include "analyzeelf.h"
#include "binarysection.h"
#include "config.h"
#include "excludelist.h"
#include "filterconfig.h"
#include "hashcalculator.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    QCoreApplication::setAttribute(Qt::AA_DontUseNativeMenuBar);
    ui->stackedWidget->setCurrentIndex(0);
    ui->all_analysis_tabs->setCurrentIndex(0);
    this->setBool_str_classic_edit(false);
    this->string_classic_extracted = false;
    this->exclude_lists            = new QVector<ExcludeList*>();
    this->old_filter               = "";
    ui->textEdit_2->insertPlainText("Open binary to start analysis");
    ui->statusbar->showMessage("Chargement ...", 5000);

    LoadExcludeLists();
    filter_config = FilterConfig(
        ui->checkbox_utf8, ui->checkbox_utf16, ui->spinbox_size_strings_classic->value(),
        ui->checkbox_case_sensitive->isChecked(), ui->txt_filter->text(),
        ui->btn_debian_coreutils->isChecked(), ui->btn_rem_x86_junk->isChecked(),
        ui->btn_rem_x64_junk->isChecked(), ui->btn_debian_base->isChecked(),
        ui->btn_rem_go_str->isChecked());
    old_filter_config = FilterConfig(
        ui->checkbox_utf8, ui->checkbox_utf16, ui->spinbox_size_strings_classic->value(),
        ui->checkbox_case_sensitive->isChecked(), ui->txt_filter->text(),
        ui->btn_debian_coreutils->isChecked(), ui->btn_rem_x86_junk->isChecked(),
        ui->btn_rem_x64_junk->isChecked(), ui->btn_debian_base->isChecked(),
        ui->btn_rem_go_str->isChecked());
    encoder_lock = false;
    ui->txt_encode_hex->blockSignals(false);

    // Thème Forêt avec des tons de vert et de marron

    themeGPTStyles.insert("MidnightBlue",
                          "background-color: #1A1B34; color: #fff; border: 1px solid #1A1B34; "
                          "QPushButton { background-color: #3C3D6E; color: #fff; } "
                          "QPushButton:pressed { background-color: #2F3057; } QLabel { font-size: "
                          "16px; }");
    themeStyles.insert("AI Noir",
                       "background-color: #0C0C0C; color: #FFFFFF; border: 1px solid #212121;");
    themeStyles.insert("QT Default",
                       ""); // Feuille de style vide pour le thème par défaut de Qt
    themeStyles.insert("Pink Lady",
                       "background-color: #F19CBB; color: #2E3440; "
                       "solid #D782A4;"); // Thème rose
    themeStyles.insert("Dark Mode",
                       "background-color: #1C1C1C; color: #D8DEE9;  "
                       "solid #2F2F2F;"); // Thème sombre
    themeStyles.insert("Black & White",
                       "background-color: #000000; color: #FFFFFF; border: 1px solid #FFFFFF;");

    auto group_string_hunting = new QButtonGroup(ui->grp_string_hunting);
    Q_UNUSED(group_string_hunting);
    this->font_size = DEFAULT_FONT_SIZE;
    /*  QString styleSheet = "QMenuBar, QMenu, QStatusBar, QToolBar, QDialog, QGroupBox, \
    QLabel, QPushButton, QToolButton, QCheckBox, QRadioButton, QComboBox, \
        QLineEdit, QTextEdit, QPlainTextEdit, QListView, QListWidget, QTreeView, QTreeWidget,
    QTableWidget, \
        QSpinBox, QDoubleSpinBox, QSlider, QScrollBar, QDial, QProgressBar, QCalendarWidget, \
        QTimeEdit, QDateTimeEdit, QTabWidget::tab, \
        QChartView, QGraphicsView, QGraphicsTextItem, QGraphicsEllipseItem, QGraphicsLineItem,
    QGraphicsPixmapItem, QGraphicsPolygonItem, QGraphicsRectItem { font-size: 23px; }";
        qApp->setStyleSheet(styleSheet);
*/
    // DoAnalyze("./ls");
}

void MainWindow::LoadExcludeLists() {
    QVector<QString>* list_name = new QVector<QString>();

    list_name->append("x86_junks");
    list_name->append("x64_junk");
    list_name->append("go_str");
    list_name->append("debian_base");
    list_name->append("debian_coreutils");

    for (auto ln : *list_name) {
        auto nel = new ExcludeList(ln, false);
        this->exclude_lists->append(nel);
    }
}
void MainWindow::DisplayHeaders() {
    QMap<QString, QString>* header_map = anal->ParseHeader();

    ui->table_nt_header_widget->setRowCount(header_map->size());
    ui->table_nt_header_widget->setColumnCount(2);

    int row = 0;
    for (auto e : header_map->keys()) {
        ui->table_nt_header_widget->setItem(row, 0, new QTableWidgetItem(e));
        ui->table_nt_header_widget->setItem(row, 1, new QTableWidgetItem(header_map->value(e)));
        row++;
    }
    ui->table_nt_header_widget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}
void MainWindow::DisplaySections() {
    auto secs = anal->GetSections();
    if (!secs) return;
    ui->table_sections->setRowCount(secs->count());
    ui->table_sections->setColumnCount(8);
    // Nom, Offset, tailleV, tailleP, entropie, Permissiosn
    int row = 0;
    std::cout << "disp SECTIONNNN" << std::endl;
    std::cout << secs->count() << std::endl;

    for (auto e : *secs) {
        // d, s.sectionBase, s.sectionName, s.sec, s.sectionData) !=
        ui->table_sections->setItem(row, 0, new QTableWidgetItem(e.GetName()));
        ui->table_sections->setItem(row, 1, new QTableWidgetItem(QString::number(e.GetOffset())));
        ui->table_sections->setItem(row, 2,
                                    new QTableWidgetItem(QString::number(e.GetVirtualSize())));

        // RWX
        ui->table_sections->setItem(
            row, 3, new QTableWidgetItem(QString::number(e.GetPermissions().at(0))));
        ui->table_sections->setItem(
            row, 4, new QTableWidgetItem(QString::number(e.GetPermissions().at(1))));
        ui->table_sections->setItem(
            row, 5, new QTableWidgetItem(QString::number(e.GetPermissions().at(2))));

        // Entropy
        ui->table_sections->setItem(row, 6, new QTableWidgetItem(QString::number(e.GetEntropy())));
        // Comment
        ui->table_sections->setItem(row, 7, new QTableWidgetItem(QString("Suspicious")));

        row++;
    }
    ui->table_sections->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}
void MainWindow::DisplayImports() {
    // Imports
    auto imps     = anal->GetImports();
    int  items_nb = imps->at(0)->count();
    ui->table_nt_imports->setRowCount(imps->size());
    ui->table_nt_imports->setColumnCount(items_nb);
    int row = 0;
    for (auto imp : *imps) {
        for (int i; i < items_nb; i++) {
            ui->table_nt_imports->setItem(row, i, new QTableWidgetItem(imp->at(0)));
        }
        row++;
    }

    ui->table_nt_imports->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void MainWindow::DoAnalyze(QString filepath) {
    //
    HashCalculator* hc = new HashCalculator(filepath);
    ui->textEdit_2->append("MD5: " + hc->getMD5());
    ui->textEdit_2->append("Sha256: " + hc->getSHA256());
    ui->textEdit_2->append("Sha512: " + hc->getSHA512());
    ui->textEdit_2->append("Ssdeep: " + hc->getSSDeep());
    switch (Analyze::GetFileType(filepath)) {
        case FILETYPE::ELF:

            this->anal = new AnalyzeElf(filepath);
            break;
        default:
            this->close();
    }

    DisplayHeaders();
    DisplaySections();

    // ui->statusbar->showMessage(anal->GetAnalyzeDuration(),10);
}

void MainWindow::PrintImports() {}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_actionClose_triggered() {
    MainWindow::close();
}

void MainWindow::on_lineEdit_textEdited(const QString& arg1) // supp
{
    bool                     case_sensitive = ui->checkbox_case_sensitive->isChecked();
    enum Qt::CaseSensitivity case_s = case_sensitive ? Qt::CaseSensitive : Qt::CaseInsensitive;
    for (int row = 0; row < ui->list_strings_classic->count(); ++row) {
        ui->list_strings_classic->item(row)->setHidden(
            !ui->list_strings_classic->item(row)->text().contains(arg1, case_s));
    }
}

/*
 *   Filters Begin
 */

void MainWindow::on_txt_filter_textChanged(const QString& arg1) {
    this->filter_config.setFilter(arg1);
    UpdateFilters();
}

void MainWindow::on_list_strings_classic_doubleClicked(const QModelIndex& index) {
    Q_UNUSED(index);
    on_btn_switch_string_classic_clicked();
    // QApplication::clipboard()->setText(ui->list_strings_classic->item(QModelIndex(index));
}

void MainWindow::on_btn_switch_string_classic_clicked() {
    // changer de stack
    if (this->getBool_str_classic_edit()) {
        ui->btn_switch_string_classic->setText("Edition");
        ui->stack_strings_classic->setCurrentIndex(0);
    } else {
        ui->btn_switch_string_classic->setText("Listing");
        ui->stack_strings_classic->setCurrentIndex(1);
        ui->text_strings_classic->clear();
        for (int row = 0; row < ui->list_strings_classic->count(); ++row) {
            if (!ui->list_strings_classic->item(row)->isHidden()) {
                ui->text_strings_classic->appendPlainText(
                    ui->list_strings_classic->item(row)->text());
            }
        }
    }
    this->setBool_str_classic_edit(!this->getBool_str_classic_edit());
}

bool MainWindow::getBool_str_classic_edit() const {
    return bool_str_classic_edit;
}

void MainWindow::setBool_str_classic_edit(bool newBool_str_classic_edit) {
    bool_str_classic_edit = newBool_str_classic_edit;
}

void MainWindow::FilterQlistBySize(QListWidget* list, int size) {
    for (int row = 0; row < list->count(); ++row) {
        QListWidgetItem* item = list->item(row);

        item->setHidden(item->isHidden() || item->text().length() < size);
    }
}
void MainWindow::FilterQlistByString(const QListWidget* list, QString filter) {
    bool                     case_sensitive = ui->checkbox_case_sensitive->isChecked();
    enum Qt::CaseSensitivity case_s = case_sensitive ? Qt::CaseSensitive : Qt::CaseInsensitive;

    const int num_threads = THREAD_NUM;              // Nombre de threads à utiliser
    const int num_items   = list->count();           // Nombre total d'éléments dans la liste
    const int chunk_size  = num_items / num_threads; // Taille de chaque fragment de la boucle

    // Tableau de booléens pour stocker l'état caché de chaque élément
    std::vector<bool> hidden_states(num_items);

    // Mutex pour verrouiller l'accès au tableau de booléens
    // std::mutex hidden_states_mutex;

    QRegExp filter_regex(ui->txt_filter->text());

    // Fonction exécutée par chaque thread
    unsigned int bcount = 0;

    auto filter_thread = [&](int start, int end) {
        for (int row = start; row < end; ++row) {
            bool    hidden = false; // hidden_states[row];
            QString text   = list->item(row)->text();
            for (auto xlist : *exclude_lists) {
                if (!hidden && xlist->getIs_activated()) {
                    for (auto xstr : *xlist->getList()) {
                        if (xstr.size() == text.size() && xstr == text) {
                            hidden = true;
                            break;
                        }
                    }
                }
            }

            if (hidden) {
                hidden_states[row] = true;
                ;
                continue;
            }

            // ajouter !hidden à chaque point :
            // Only one is executed (radio) :
            if (ui->chk_regex_email->isChecked()) {
                hidden = hidden || !text.contains(REGEX_IP);
            } else if (ui->chk_regex_ip->isChecked()) {
                hidden = hidden || !text.contains(REGEX_IP);
            } else if (ui->chk_regex_base64->isChecked()) {
                hidden = hidden || !text.contains(REGEX_BASE64);
            } else if (ui->chk_regex_url->isChecked()) {
                hidden = hidden || !text.contains(REGEX_URL);
            } else if (ui->chk_regex_registry->isChecked()) {
                hidden = hidden || !text.contains(REGEX_REGISTRY);
            } else if (ui->chk_regex_unix_path->isChecked()) {
                hidden = hidden || !text.contains(REGEX_UNIX_PATH);
            } else if (ui->chk_regex_win_path->isChecked()) {
                hidden = hidden || !text.contains(REGEX_WIN_PATH);
            }

            // Use hidden_states[row] from here, hidden is no more valid
            //  if(!ui->txt_filter->text().isEmpty()){
            if (ui->chk_regex_string_classic->isChecked()) {
                hidden = hidden || !text.contains(filter_regex);
            } else {
                hidden = hidden || !text.contains(filter, case_s);
            }

            if (hidden) { // default is false, only write if true
                hidden_states[row] = hidden;
            }
        }
    };

    // Création des threads
    std::vector<std::thread> threads;
    for (int i = 0; i < num_threads - 1; ++i) {
        int start = i * chunk_size;
        int end   = start + chunk_size;
        threads.emplace_back(filter_thread, start, end);
        std::cout << "item : " << num_items << "start/: " << start << " end: " << end << std::endl;
    }

    // Traitement dudernier fragment
    int start           = (num_threads - 1) * chunk_size;
    int last_chunk_size = num_items - (num_threads - 1) * chunk_size;
    int end             = start + last_chunk_size;
    threads.emplace_back(filter_thread, start, end);
    std::cout << "item : " << num_items << "start/: " << start << " end: " << end << std::endl;

    // Attente de la fin des threads
    for (auto& t : threads) {
        t.join();
    }

    // Mise à jour de l'état caché de chaque élément en fonction du tableau de
    // booléens
    for (int row = 0; row < num_items; ++row) {
        bool             item_hidden = hidden_states[row];
        QListWidgetItem* item        = list->item(row);
        item->setHidden(item_hidden);
    }
}

void MainWindow::on_spinbox_size_strings_classic_valueChanged(int arg1) {
    this->filter_config.setSizeStringsClassic(arg1);
    UpdateFilters();
}

void MainWindow::on_actionOpen_new_file_triggered() {
    auto fileName = QFileDialog::getOpenFileName(this, tr("Open file"), NULL, NULL);
    if (!fileName.isEmpty()) {
        ui->stackedWidget->setCurrentIndex(1);
        DoAnalyze(fileName);
    }
}

/* reset button */
void MainWindow::on_btn_search_string_classic_clicked() {
    ui->spinbox_size_strings_classic->setValue(5);
    ui->txt_filter->clear();
    ui->chk_disable_string_hunt->setChecked(true);
    auto list = ui->list_strings_classic;
    for (int row = 0; row < list->count(); ++row) {
        QListWidgetItem* item = list->item(row);
        item->setHidden(false);
    }

    ui->btn_debian_coreutils->setChecked(false);
    ui->btn_rem_x86_junk->setChecked(false);
    ui->btn_rem_x64_junk->setChecked(false);
    ui->btn_debian_base->setChecked(false);
    ui->btn_rem_go_str->setChecked(false);
}

void MainWindow::on_all_analysis_tabs_currentChanged(int index) {
    Q_UNUSED(index);
    if (ui->all_analysis_tabs->currentWidget()->objectName() == "tab_strings_classic") {
        if (!string_classic_extracted) {
            auto strs = this->anal->GetStrings(ui->checkbox_utf8->isChecked(),
                                               ui->checkbox_utf16->isChecked());
            for (auto e : *strs) {
                ui->list_strings_classic->addItem(e);
            }
            this->string_classic_extracted = true;
        }
    }
}

void MainWindow::UpdateFilters() {
    auto list = ui->list_strings_classic;
    QApplication::setOverrideCursor(Qt::WaitCursor);

    // Only size increased
    if (this->filter_config.countChangedFlags() == 1 &&
        this->filter_config.getSizeStringsClassicChanged() &&
        this->filter_config.getSizeStringsClassic() >
            this->old_filter_config.getSizeStringsClassic()) {
        std::cout << "Only size increase " << std::endl;
        this->FilterQlistBySize(list, this->filter_config.getSizeStringsClassic());
        this->filter_config.reset_all();
        this->old_filter_config = this->filter_config;
        UpdateMatchCount();
        QApplication::restoreOverrideCursor();
        return;
    }

    ui->groupBox->setTitle("Loading ... ");
    // Order de ceci important, si la taille augmente uniquement pas besoin de
    // clear le flag
    for (int row = 0; row < list->count(); ++row) {
        list->item(row)->setHidden(false);
    }

    QElapsedTimer timer;
    timer.start();
    this->FilterQlistByString(list, this->filter_config.getFilter());
    std::cout << "endof filter by string " << timer.elapsed() << " milliseconds" << std::endl;
    timer.restart();

    this->FilterQlistBySize(list, this->filter_config.getSizeStringsClassic());
    UpdateMatchCount();
    QApplication::restoreOverrideCursor();
}

void MainWindow::UpdateMatchCount() {
    auto         list          = ui->list_strings_classic;
    unsigned int count_visible = 0;
    for (int row = 0; row < list->count(); ++row) {
        !list->item(row)->isHidden() && count_visible++;
    }

    ui->groupBox->setTitle("Filters (Number of matches : " + QString::number(count_visible) + ")");
    this->filter_config.reset_all();
}

void MainWindow::on_all_analysis_tabs_tabBarClicked(int index) {
    Q_UNUSED(index);
}

void MainWindow::on_checkbox_case_sensitive_stateChanged(int arg1) {
    this->filter_config.setCaseSensitiveChanged(arg1);
    UpdateFilters();
}

void MainWindow::SetBoolExcludeListByName(QString name, bool b) {
    for (auto x : *exclude_lists) {
        ui->textEdit_2->append(x->getName());
        if (x->getName() == name) {
            ui->textEdit_2->append("cmp" + x->getName() + "=" + name);
            x->setIs_activated(b);
        }
    }
}

bool MainWindow::ExcludeListExists(QString name) {
    for (auto xl : *this->exclude_lists) {
        if (xl->getName() == name) return true;
    }
    return false;
}

void MainWindow::on_btn_debian_coreutils_clicked(bool checked) {
    QString resource_file = "debian_coreutils";

    SetBoolExcludeListByName(resource_file, checked);
    this->filter_config.setDebianCoreutilsChecked(checked);
    UpdateFilters();
}

void MainWindow::DispatchEncoding(enum EncodingType ft) {
    // Effacement des objets QPlainText avant de les remplir
    /*   ui->txt_encode_ascii->setPlainText()
  ui->txt_encode_ascii->clear();
  ui->txt_encode_base64->clear();
  ui->txt_encode_hex->clear();
  ui->txt_encode_bin->clear();
*/
    // Remplissage des objets QPlainText avec les valeurs des objets de la classe
    // DataEncoder
    switch (ft) {
        case BINARY:
            ui->txt_encode_ascii->setPlainText(encoder.str_ascii());
            ui->txt_encode_base64->setPlainText(this->encoder.str_b64());
            ui->txt_encode_hex->setPlainText(this->encoder.str_hex());
            break;
        case HEX:
            ui->txt_encode_ascii->setPlainText(encoder.str_ascii());
            ui->txt_encode_base64->setPlainText(this->encoder.str_b64());
            ui->txt_encode_bin->setPlainText(this->encoder.str_bin());
            break;
        case ASCII:
            ui->txt_encode_base64->setPlainText(this->encoder.str_b64());
            ui->txt_encode_hex->setPlainText(this->encoder.str_hex());
            ui->txt_encode_bin->setPlainText(this->encoder.str_bin());
            break;
        case BASE64:
            ui->txt_encode_ascii->setPlainText(encoder.str_ascii());
            ui->txt_encode_hex->setPlainText(this->encoder.str_hex());
            ui->txt_encode_bin->setPlainText(this->encoder.str_bin());
            break;
    }
}

void MainWindow::UpdateXOR() {
    auto hexString = ui->txt_encode_hex->toPlainText();
    int  key       = ui->txt_spinbox_xor_key_dec->value();

    QByteArray hexData =
        QByteArray::fromHex(hexString.toUtf8()); // Conversion de la chaîne hexadécimale en tableau
    // de bytes
    QByteArray xorData;             // Tableau de bytes résultant du XOR
    xorData.resize(hexData.size()); // Réserver la taille du tableau résultant

    for (int i = 0; i < hexData.size(); i++) {
        xorData[i] = hexData[i] ^ key; // XOR de chaque byte avec la clé
    }

    QString asciiString =
        QString::fromUtf8(xorData); // Conversion du tableau de bytes résultant en chaîne ASCII
    ui->txt_encode_xor->setPlainText(asciiString);
}

void MainWindow::on_txt_encode_ascii_textChanged() {
    std::cout << "asciichanged" << std::endl;

    ui->txt_encode_base64->blockSignals(true);
    ui->txt_encode_bin->blockSignals(true);
    ui->txt_encode_hex->blockSignals(true);

    this->encoder.Convert(ASCII, ui->txt_encode_ascii->toPlainText());
    DispatchEncoding(ASCII);

    ui->txt_encode_base64->blockSignals(false);
    ui->txt_encode_bin->blockSignals(false);
    ui->txt_encode_hex->blockSignals(false);
}
void MainWindow::on_txt_encode_hex_textChanged() {
    std::cout << "HEX CHANGED" << std::endl;
    QString input = ui->txt_encode_hex->toPlainText();
    QRegExp hexRegex("^[0-9A-Fa-f ]*$");

    if (!hexRegex.exactMatch(input)) {
        std::cout << "HEX INVALID" << std::endl;
        // Données invalides, ne pas convertir
        return;
    }
    std::cout << "HEX VALIDE" << std::endl;
    ui->txt_encode_base64->blockSignals(true);
    ui->txt_encode_ascii->blockSignals(true);
    ui->txt_encode_bin->blockSignals(true);

    this->encoder.Convert(HEX, input);
    DispatchEncoding(HEX);

    ui->txt_encode_base64->blockSignals(false);
    ui->txt_encode_ascii->blockSignals(false);
    ui->txt_encode_bin->blockSignals(false);
}

void MainWindow::on_txt_encode_base64_textChanged() {
    QString input = ui->txt_encode_base64->toPlainText();
    QRegExp base64Regex("^[A-Za-z0-9+/]*={0,2}$");

    if (!base64Regex.exactMatch(input)) {
        return;
    }

    ui->txt_encode_ascii->blockSignals(true);
    ui->txt_encode_bin->blockSignals(true);
    ui->txt_encode_hex->blockSignals(true);
    this->encoder.Convert(BASE64, input);
    DispatchEncoding(BASE64);
    ui->txt_encode_ascii->blockSignals(false);
    ui->txt_encode_bin->blockSignals(false);
    ui->txt_encode_hex->blockSignals(false);
}

void MainWindow::on_txt_encode_bin_textChanged() {
    QString input = ui->txt_encode_bin->toPlainText();
    QRegExp binaryRegex("^[01 ]*$");

    if (!binaryRegex.exactMatch(input)) {
        // Données invalides, ne pas convertir
        return;
    }

    ui->txt_encode_base64->blockSignals(true);
    ui->txt_encode_ascii->blockSignals(true);
    ui->txt_encode_hex->blockSignals(true);

    this->encoder.Convert(BINARY, input);
    DispatchEncoding(BINARY);

    ui->txt_encode_base64->blockSignals(false);
    ui->txt_encode_ascii->blockSignals(false);
    ui->txt_encode_hex->blockSignals(false);
}

void MainWindow::on_txt_encode_xor_textChanged() {}

void MainWindow::on_txt_spinbox_xor_key_dec_valueChanged(int arg1) {
    ui->txt_encode_xorkey_hex->blockSignals(true);
    ui->txt_encode_xorkey_hex->setText(QString::number(arg1, 16));
    UpdateXOR();
    ui->txt_encode_xorkey_hex->blockSignals(true);
}

void MainWindow::on_txt_encode_xorkey_hex_textChanged(const QString& arg1) {
    ui->txt_spinbox_xor_key_dec->blockSignals(true);
    ui->txt_spinbox_xor_key_dec->setValue(arg1.toInt(NULL, 16));
    UpdateXOR();
    ui->txt_spinbox_xor_key_dec->blockSignals(true);
}

void MainWindow::on_plainTextEdit_textChanged() {}

void MainWindow::on_plainTextEdit_2_textChanged() {}

void MainWindow::on_actionQt_Pink_triggered() {
    this->setStyleSheet("");
    this->setStyleSheet(themeStyles.find("Pink Lady").value());
}

void MainWindow::on_actionQt_Style_clear_triggered() {
    this->setStyleSheet(themeStyles.find("QT Default").value());
}

void MainWindow::on_action_by_ChatGPT_triggered() {
    QList<QString>    themeKeys   = themeGPTStyles.keys();
    QRandomGenerator* generator   = QRandomGenerator::global();
    QString           randomTheme = themeKeys.at(generator->bounded(themeKeys.size()));
    std::cout << randomTheme.toStdString() << std::endl;
    // Appliquer la feuille de style correspondante à la fenêtre principale
    this->setStyleSheet(themeGPTStyles.find(randomTheme).value());
}

void MainWindow::on_actionQt_Style_dark_triggered() {
    this->setStyleSheet(themeStyles.find("Dark Mode").value());
}

void MainWindow::on_actionBlack_White_triggered() {
    this->setStyleSheet(themeStyles.find("AI Noir").value());
}

void MainWindow::on_font_zoom_triggered() {
    this->font_size++;
    this->setFont(QFont("Arial", font_size, 3, false));
}

void MainWindow::on_font_dezoom_triggered() {
    this->font_size--;
    this->setFont(QFont("Arial", font_size, 3, false));
}

void MainWindow::on_btn_home_file_clicked() {
    auto fileName = QFileDialog::getOpenFileName(this, tr("Open file"), NULL, NULL);
    ui->stackedWidget->setCurrentIndex(1);
    DoAnalyze(fileName);
}

void MainWindow::on_btn_rem_x64_junk_clicked(bool checked) {
    QString resource_file = "x64_junk";
    SetBoolExcludeListByName(resource_file, checked);
    this->filter_config.setX64Checked(checked);
    std::cout << "taille list : " << this->exclude_lists->count()
              << this->exclude_lists->at(0)->getList()->at(0).toStdString() << std::endl;
    UpdateFilters();
}

void MainWindow::on_btn_rem_x64_junk_clicked() {
    std::cout << "passparla" << std::endl;
}

void MainWindow::on_btn_rem_x86_junk_clicked(bool checked) {
    QString resource_file = "x86_junk";
    SetBoolExcludeListByName(resource_file, checked);
    this->filter_config.setX86Checked(checked);
    UpdateFilters();
}

void MainWindow::on_chk_regex_url_stateChanged(int arg1) {
    Q_UNUSED(arg1);
    UpdateFilters();
}

void MainWindow::on_chk_regex_ip_stateChanged(int arg1) {
    Q_UNUSED(arg1);
    UpdateFilters();
}

void MainWindow::on_chk_regex_email_stateChanged(int arg1) {
    Q_UNUSED(arg1);
    UpdateFilters();
}

void MainWindow::on_chk_regex_registry_stateChanged(int arg1) {
    Q_UNUSED(arg1);
    UpdateFilters();
}

void MainWindow::on_chk_regex_unix_path_stateChanged(int arg1) {
    Q_UNUSED(arg1);
    UpdateFilters();
}

void MainWindow::on_chk_regex_win_path_stateChanged(int arg1) {
    Q_UNUSED(arg1);
    UpdateFilters();
}

void MainWindow::on_chk_regex_unix_path_clicked() {
    UpdateFilters();
}

void MainWindow::on_radioButton_clicked() {
    UpdateFilters();
}

void MainWindow::on_chk_regex_url_clicked() {
    UpdateFilters();
}

void MainWindow::on_chk_regex_ip_clicked() {
    UpdateFilters();
}

void MainWindow::on_chk_regex_email_clicked() {
    UpdateFilters();
}

void MainWindow::on_chk_regex_registry_clicked() {
    UpdateFilters();
}

void MainWindow::on_chk_regex_win_path_clicked() {
    UpdateFilters();
}

void MainWindow::on_chk_regex_base64_clicked() {
    UpdateFilters();
}
void MainWindow::on_pushButton_3_clicked(bool checked) {
    Q_UNUSED(checked);
}

void MainWindow::on_btn_debian_base_clicked(bool checked) {
    QString resource_file = "debian_base";
    SetBoolExcludeListByName(resource_file, checked);
    this->filter_config.setDebianBaseChecked(checked);
    UpdateFilters();
}

void MainWindow::SaveHexData(QString hexString, unsigned char key) {
    // Convertir la chaîne hexadécimale en un tableau de bytes
    QByteArray byteArray = QByteArray::fromHex(hexString.toUtf8());

    // Demander à l'utilisateur de choisir l'emplacement du fichier
    QString fileName = QFileDialog::getSaveFileName(nullptr, "Sauvegarder le fichier binaire", "",
                                                    "Fichiers binaires (*.bin)");

    // Si l'utilisateur a annulé la sélection de fichier, quitter la fonction
    if (fileName.isEmpty()) {
        return;
    }

    // Ouvrir le fichier pour l'écriture
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::critical(nullptr, "Erreur", "Impossible d'ouvrir le fichier pour l'écriture");
        return;
    }

    if (key > 0) {
        for (int i = 0; i < byteArray.count(); i++) {
            byteArray[i] = byteArray[i] ^ key;
        }
    }

    // Écrire les données dans le fichier
    qint64 bytesWritten = file.write(byteArray);

    // Vérifier si l'écriture a réussi
    if (bytesWritten == -1) {
        QMessageBox::critical(nullptr, "Erreur",
                              "Une erreur s'est produite lors de l'écriture dans le fichier");
    } else {
        QMessageBox::information(nullptr, "Succès",
                                 QString("Le fichier binaire a été sauvegardé avec "
                                         "succès à l'emplacement suivant : %1")
                                     .arg(fileName));
    }

    // Fermer le fichier
    file.close();
}

void MainWindow::on_pushButton_clicked() {}

void MainWindow::on_btn_save_xored_clicked() {
    // Récupérer la valeur du SpinBox en tant qu'unsigned char

    unsigned char key = ui->txt_spinbox_xor_key_dec->value() & 0xFF;
    // Appeler la fonction SaveHexData() en utilisant la clé XOR
#include <stdio.h>
    printf("key : <%u>", key);
    SaveHexData(ui->txt_encode_hex->toPlainText(), key);
}

void MainWindow::on_pushButton_2_clicked() {
    ui->txt_encode_hex->clear();
}

void MainWindow::on_btn_save_to_binary_clicked() {
    SaveHexData(ui->txt_encode_hex->toPlainText(), 0);
}

void MainWindow::on_btn_rem_go_str_clicked(bool checked) {
    QString resource_file = "go_str";
    SetBoolExcludeListByName(resource_file, checked);
    this->filter_config.setGoChecked(checked);
    UpdateFilters();
}
