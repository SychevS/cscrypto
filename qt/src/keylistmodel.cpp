#include <keylistmodel.hpp>

#include <QString>

#include <base58.h>

namespace cscrypto {
namespace gui {

KeyListModel::KeyListModel(QObject* parent) : QAbstractListModel(parent) {}

QVariant KeyListModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid() || index.row() < 0 || size_t(index.row()) >= keys_.size()) {
        return QVariant();
    }
    if (role == Qt::DisplayRole) {
        auto pub = keys_[size_t(index.row())].first;
        QString s = QString::fromUtf8(EncodeBase58(pub.data(), pub.data() + pub.size()).c_str());
        return s;
    }
    return QVariant();
}

int KeyListModel::rowCount(const QModelIndex& /* parent */) const {
    return int(keys_.size());
}

void KeyListModel::addKeyPair(KeyPair&& keys) {
    keys_.push_back(keys);
}

} // namespace gui
} // namespace cscrypto