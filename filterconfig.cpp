#include "filterconfig.h"

FilterConfig::FilterConfig() {}

FilterConfig::FilterConfig(bool is_utf8, bool is_utf16, int size_strings_classic,
                           bool case_sensitive, QString filter, bool debian_coreutils_checked,
                           bool x86_checked, bool x64_checked, bool debian_base_checked,
                           bool go_checked)
    : is_utf8(is_utf8), is_utf16(is_utf16), size_strings_classic(size_strings_classic),
      case_sensitive(case_sensitive), filter(filter),
      debian_coreutils_checked(debian_coreutils_checked), debian_base_checked(debian_base_checked),
      is_utf8_changed(false), is_utf16_changed(false), size_strings_classic_changed(false),
      case_sensitive_changed(false), filter_changed(false), debian_coreutils_checked_changed(false),
      debian_base_checked_changed(false), x86_checked(x86_checked), x86_checked_changed(false),
      x64_checked(x64_checked), x64_checked_changed(false), go_checked(go_checked),
      go_checked_changed(false), config_changed(false) {}

bool FilterConfig::getIsUtf8() const {
    return is_utf8;
}

void FilterConfig::setIsUtf8(bool is_utf8) {
    this->is_utf8   = is_utf8;
    is_utf8_changed = true;
}

bool FilterConfig::getIsUtf16() const {
    return is_utf16;
}

void FilterConfig::setIsUtf16(bool is_utf16) {
    this->is_utf16   = is_utf16;
    is_utf16_changed = true;
}

int FilterConfig::getSizeStringsClassic() const {
    return size_strings_classic;
}

void FilterConfig::setSizeStringsClassic(int size_strings_classic) {
    this->size_strings_classic   = size_strings_classic;
    size_strings_classic_changed = true;
}

bool FilterConfig::getCaseSensitive() const {
    return case_sensitive;
}

void FilterConfig::setCaseSensitive(bool case_sensitive) {
    this->case_sensitive   = case_sensitive;
    case_sensitive_changed = true;
}

QString FilterConfig::getFilter() const {
    return filter;
}

void FilterConfig::setFilter(QString filter) {
    this->filter   = filter;
    filter_changed = true;
}

bool FilterConfig::getIsUtf8Changed() const {
    return is_utf8_changed;
}

void FilterConfig::setIsUtf8Changed(bool is_utf8_changed) {
    this->is_utf8_changed = is_utf8_changed;
}

bool FilterConfig::getIsUtf16Changed() const {
    return is_utf16_changed;
}

void FilterConfig::setIsUtf16Changed(bool is_utf16_changed) {
    this->is_utf16_changed = is_utf16_changed;
}

bool FilterConfig::getSizeStringsClassicChanged() const {
    return size_strings_classic_changed;
}

void FilterConfig::setSizeStringsClassicChanged(bool size_strings_classic_changed) {
    this->size_strings_classic_changed = size_strings_classic_changed;
}

bool FilterConfig::getCaseSensitiveChanged() const {
    return case_sensitive_changed;
}

void FilterConfig::setCaseSensitiveChanged(bool case_sensitive_changed) {
    this->case_sensitive_changed = case_sensitive_changed;
}

bool FilterConfig::getFilterChanged() const {
    return filter_changed;
}

void FilterConfig::setFilterChanged(bool filter_changed) {
    this->filter_changed = filter_changed;
}
// debianbase
bool FilterConfig::getDebianBaseCheckedChanged() const {
    return debian_base_checked_changed;
}

void FilterConfig::setDebianBaseCheckedChanged(bool debian_base_checked_changed) {
    this->debian_base_checked_changed = debian_base_checked_changed;
}

bool FilterConfig::getDebianBaseChecked() const {
    return debian_base_checked;
}

void FilterConfig::setDebianBaseChecked(bool debian_base_checked) {
    this->debian_base_checked   = debian_base_checked;
    debian_base_checked_changed = true;
}
// coreutils
bool FilterConfig::getDebianCoreutilsCheckedChanged() const {
    return debian_coreutils_checked_changed;
}

void FilterConfig::setDebianCoreutilsCheckedChanged(bool debian_coreutils_checked_changed) {
    this->debian_coreutils_checked_changed = debian_coreutils_checked_changed;
}

bool FilterConfig::getDebianCoreutilsChecked() const {
    return debian_coreutils_checked;
}

void FilterConfig::setDebianCoreutilsChecked(bool debian_coreutils_checked) {
    this->debian_coreutils_checked   = debian_coreutils_checked;
    debian_coreutils_checked_changed = true;
}

// Go
bool FilterConfig::getGoCheckedChanged() const {
    return x86_checked_changed;
}

void FilterConfig::setGoCheckedChanged(bool go_checked_changed) {
    this->go_checked_changed = go_checked_changed;
}

bool FilterConfig::getGoChecked() const {
    return x86_checked;
}

void FilterConfig::setGoChecked(bool go_checked) {
    this->go_checked   = go_checked;
    go_checked_changed = true;
}

// X86
bool FilterConfig::getX86CheckedChanged() const {
    return x86_checked_changed;
}

void FilterConfig::setX86CheckedChanged(bool x86_checked_changed) {
    this->x86_checked_changed = x86_checked_changed;
}

bool FilterConfig::getX86Checked() const {
    return x86_checked;
}

void FilterConfig::setX86Checked(bool x86_checked) {
    this->x86_checked   = x86_checked;
    x86_checked_changed = true;
}
// X64
bool FilterConfig::getX64CheckedChanged() const {
    return x64_checked_changed;
}

void FilterConfig::setX64CheckedChanged(bool x64_checked_changed) {
    this->x64_checked_changed = x64_checked_changed;
}

bool FilterConfig::getX64Checked() const {
    return x64_checked;
}

void FilterConfig::setX64Checked(bool x64_checked) {
    this->x64_checked   = x64_checked;
    x64_checked_changed = true;
}
// FIN
bool FilterConfig::getConfigChanged() const {
    return config_changed;
}

void FilterConfig::setConfigChanged(bool config_changed) {
    this->config_changed = config_changed;
}

bool FilterConfig::compare(const FilterConfig& other) const {
    return (is_utf8 == other.is_utf8 && is_utf16 == other.is_utf16 &&
            size_strings_classic == other.size_strings_classic &&
            case_sensitive == other.case_sensitive && filter == other.filter &&
            debian_coreutils_checked == other.debian_coreutils_checked);
}

void FilterConfig::reset_all() {
    is_utf8_changed                  = false;
    is_utf16_changed                 = false;
    size_strings_classic_changed     = false;
    case_sensitive_changed           = false;
    filter_changed                   = false;
    debian_coreutils_checked_changed = false;
    config_changed                   = false;
}

int FilterConfig::countChangedFlags() const {
    int count = 0;
    if (is_utf8_changed) count++;
    if (is_utf16_changed) count++;
    if (size_strings_classic_changed) count++;
    if (case_sensitive_changed) count++;
    if (filter_changed) count++;
    if (debian_coreutils_checked_changed) count++;
    return count;
}
