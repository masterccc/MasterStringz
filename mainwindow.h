#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QListWidget>
#include <QMainWindow>

#include "analyze.h"
#include "dataencoder.h"
#include "excludelist.h"
#include "filterconfig.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

  public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();
    void DoAnalyze(QString filepath);

    void PrintImports();
    bool getBool_str_classic_edit() const;
    void setBool_str_classic_edit(bool newBool_str_classic_edit);

    void FilterQlistBySize(QListWidget* list, int size);

    void FilterQlistByString(const QListWidget* list, QString filter);
    void LoadCoreutilsStrings();
    void SetBoolExcludeListByName(QString name, bool b);
    void DisplayHeaders();
    void DisplayImports();
    void DisplaySections();
    bool ExcludeListExists(QString name);
    void DispatchEncoding(enum EncodingType ft);
    void UpdateXOR();
    void UpdateMatchCount();

    void LoadExcludeLists();
  private slots:
    void on_actionClose_triggered();

    void on_lineEdit_textEdited(const QString& arg1);

    void on_txt_filter_textChanged(const QString& arg1);

    void on_list_strings_classic_doubleClicked(const QModelIndex& index);

    void on_btn_switch_string_classic_clicked();

    void on_spinbox_size_strings_classic_valueChanged(int arg1);

    void on_actionOpen_new_file_triggered();

    void on_btn_search_string_classic_clicked();

    void on_all_analysis_tabs_currentChanged(int index);

    void on_all_analysis_tabs_tabBarClicked(int index);

    void UpdateFilters();

    void on_checkbox_case_sensitive_stateChanged(int arg1);

    void on_btn_debian_coreutils_clicked(bool checked);

    void on_plainTextEdit_textChanged();

    void on_plainTextEdit_2_textChanged();

    void on_txt_encode_ascii_textChanged();

    void on_txt_encode_hex_textChanged();

    void on_txt_encode_base64_textChanged();

    void on_txt_encode_bin_textChanged();

    void on_txt_encode_xor_textChanged();

    void on_txt_spinbox_xor_key_dec_valueChanged(int arg1);

    void on_txt_encode_xorkey_hex_textChanged(const QString& arg1);

    void on_actionQt_Pink_triggered();

    void on_actionQt_Style_clear_triggered();

    void on_action_by_ChatGPT_triggered();

    void on_actionQt_Style_dark_triggered();

    void on_actionBlack_White_triggered();

    void on_font_zoom_triggered();

    void on_font_dezoom_triggered();

    void on_btn_home_file_clicked();

    void on_btn_rem_x64_junk_clicked();

    void on_btn_rem_x64_junk_clicked(bool checked);

    void on_btn_rem_x86_junk_clicked(bool checked);

    void on_chk_regex_url_stateChanged(int arg1);

    void on_chk_regex_ip_stateChanged(int arg1);

    void on_chk_regex_email_stateChanged(int arg1);

    void on_chk_regex_registry_stateChanged(int arg1);

    void on_chk_regex_unix_path_stateChanged(int arg1);

    void on_chk_regex_win_path_stateChanged(int arg1);

    void on_chk_regex_unix_path_clicked();

    void on_radioButton_clicked();

    void on_chk_regex_url_clicked();

    void on_chk_regex_ip_clicked();

    void on_chk_regex_email_clicked();

    void on_chk_regex_registry_clicked();

    void on_chk_regex_win_path_clicked();

    void on_chk_regex_base64_clicked();

    void on_pushButton_3_clicked(bool checked);

    void on_btn_debian_base_clicked(bool checked);

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_btn_save_xored_clicked();

    void on_btn_save_to_binary_clicked();
    void SaveHexData(QString hexString, unsigned char key);

    void on_btn_rem_go_str_clicked(bool checked);

  private:
    Ui::MainWindow*        ui;
    bool                   bool_str_classic_edit;
    bool                   string_classic_extracted;
    Analyze*               anal;
    QVector<QString>*      coreutils_strings;
    QVector<ExcludeList*>* exclude_lists;
    QString                old_filter;
    FilterConfig           filter_config;
    FilterConfig           old_filter_config;
    DataEncoder            encoder;
    bool                   encoder_lock;
    QMap<QString, QString> themeGPTStyles;
    QMap<QString, QString> themeStyles;
    int                    font_size;
};

#endif // MAINWINDOW_H
