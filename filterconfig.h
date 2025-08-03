#ifndef FILTERCONFIG_H
#define FILTERCONFIG_H

#include <QList>
#include <QString>

class FilterConfig {
  public:
    FilterConfig(bool is_utf8, bool is_utf16, int size_strings_classic, bool case_sensitive,
                 QString filter, bool debian_coreutils_checked, bool x86_checked, bool x64_checked,
                 bool debian_base_checked, bool go_checked);

    FilterConfig();
    bool getIsUtf8() const;
    void setIsUtf8(bool is_utf8);

    bool getIsUtf16() const;
    void setIsUtf16(bool is_utf16);

    int  getSizeStringsClassic() const;
    void setSizeStringsClassic(int size_strings_classic);

    bool getCaseSensitive() const;
    void setCaseSensitive(bool case_sensitive);

    QString getFilter() const;
    void    setFilter(QString filter);

    bool getIsUtf8Changed() const;
    void setIsUtf8Changed(bool is_utf8_changed);

    bool getIsUtf16Changed() const;
    void setIsUtf16Changed(bool is_utf16_changed);

    bool getSizeStringsClassicChanged() const;
    void setSizeStringsClassicChanged(bool changed);

    bool getCaseSensitiveChanged() const;
    void setCaseSensitiveChanged(bool changed);

    bool getFilterChanged() const;
    void setFilterChanged(bool filter_changed);

    // debian base

    bool getDebianBaseChecked() const;
    void setDebianBaseChecked(bool changed);

    bool getDebianBaseCheckedChanged() const;
    void setDebianBaseCheckedChanged(bool changed);

    // coreutils
    bool getDebianCoreutilsChecked() const;
    void setDebianCoreutilsChecked(bool changed);

    bool getDebianCoreutilsCheckedChanged() const;
    void setDebianCoreutilsCheckedChanged(bool changed);

    // Go
    bool getGoChecked() const;
    void setGoChecked(bool changed);

    bool getGoCheckedChanged() const;
    void setGoCheckedChanged(bool changed);

    // X86
    bool getX86Checked() const;
    void setX86Checked(bool changed);

    bool getX86CheckedChanged() const;
    void setX86CheckedChanged(bool changed);
    // X64
    bool getX64Checked() const;
    void setX64Checked(bool changed);

    bool getX64CheckedChanged() const;
    void setX64CheckedChanged(bool changed);

    bool getConfigChanged() const;
    void setConfigChanged(bool config_changed);

    bool compare(const FilterConfig& other) const;

    void reset_all();

    int countChangedFlags() const;

  private:
    bool    is_utf8;
    bool    is_utf16;
    int     size_strings_classic;
    bool    case_sensitive;
    QString filter;
    bool    debian_coreutils_checked;
    bool    debian_base_checked;

    bool is_utf8_changed;
    bool is_utf16_changed;
    bool size_strings_classic_changed;
    bool case_sensitive_changed;
    bool filter_changed;
    bool debian_coreutils_checked_changed;
    bool debian_base_checked_changed;
    bool x86_checked;
    bool x86_checked_changed;
    bool x64_checked;
    bool x64_checked_changed;
    bool go_checked;
    bool go_checked_changed;

    bool config_changed;
};

#endif // FILTERCONFIG_H
