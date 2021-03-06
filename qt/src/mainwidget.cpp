#include "mainwidget.hpp"

#include <QSizePolicy>
#include <QStatusBar>
#include <QTabWidget>
#include <QtSql/QSqlError>
#include <QVBoxLayout>

#include "cipherwidget.hpp"
#include "hashwidget.hpp"
#include "keyexchangewidget.hpp"
#include "keygenwidget.hpp"
#include "signwidget.hpp"
#include "storagewidget.hpp"
#include "utils.hpp"

namespace {
const char* kDatabaseName = "cscrypto_db.sqlite";
} // namespace

namespace cscrypto {
namespace gui {

MainWidget::MainWidget(QWidget* parent)
        : QWidget(parent),
          tabs_(new QTabWidget(this)),
          statusBar_(new QStatusBar(this)),
          dbOpener_(kDatabaseName) {
    fillWidgets();
    fillTabs();

    QVBoxLayout* mainLayout = new QVBoxLayout;

    mainLayout->addWidget(tabs_);
    mainLayout->addWidget(statusBar_);

    setLayout(mainLayout);

    setWindowIcon(QIcon(":/resources/cs.png"));
}

void MainWidget::fillTabs() {
    tabs_->setSizePolicy(QSizePolicy(QSizePolicy::Policy::Expanding,
                                     QSizePolicy::Policy::Expanding));

    for (const auto& w : widgets_) {
        tabs_->addTab(w.second, w.first);
    }
}

void MainWidget::fillWidgets() {
    widgets_.insert(std::make_pair("keygen", new KeyGenWidget(*statusBar_, &keysModel_, tabs_)));
    widgets_.insert(std::make_pair("sign", new SignWidget(*statusBar_, &keysModel_, tabs_)));
    widgets_.insert(std::make_pair("hash", new HashWidget(*statusBar_, tabs_)));
    widgets_.insert(std::make_pair("cipher", new CipherWidget(encryptionKeysModel_,
                                                              decryptionKeysModel_,
                                                              *statusBar_, tabs_)));
    if (dbOpener_.isOpened()) {
        toStatusBar(*statusBar_, tr(kDatabaseName) + tr(" opened."));
        widgets_.insert(std::make_pair("storage", new StorageWidget(*statusBar_, importedKeysModel_, tabs_)));
        widgets_.insert(std::make_pair("key exchange", new KeyExchangeWidget(encryptionKeysModel_, decryptionKeysModel_,
                                                                             *statusBar_, &keysModel_, importedKeysModel_, tabs_)));
    }
    else {
        toStatusBar(*statusBar_, tr(kDatabaseName) + tr(" not opened. Storage widget unavailable: ") + QSqlDatabase::database().lastError().text());
    }
}

} // namespace gui
} // namespace cscrypto
