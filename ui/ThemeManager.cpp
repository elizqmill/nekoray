#include <QColor>
#include <QPalette>
#include <QStyle>
#include <QApplication>
#include <QStyleFactory>

#include "ThemeManager.hpp"

ThemeManager *themeManager = new ThemeManager;

extern QString ReadFileText(const QString &path);

void ThemeManager::ApplyTheme(const QString &theme) {
    auto internal = [=] {
        if (this->system_style_name.isEmpty()) {
            this->system_style_name = qApp->style()->objectName();
        }
        if (this->current_theme == theme) {
            return;
        }

        bool ok;
        auto themeId = theme.toInt(&ok);

        if (ok && themeId == 4) {
            // Dark: Fusion style with a proper dark palette
            qApp->setStyle(QStyleFactory::create("Fusion"));

            QPalette darkPalette;
            darkPalette.setColor(QPalette::Window, QColor(0x2b, 0x2b, 0x2b));
            darkPalette.setColor(QPalette::WindowText, Qt::white);
            darkPalette.setColor(QPalette::Disabled, QPalette::WindowText, QColor(0x7f, 0x7f, 0x7f));
            darkPalette.setColor(QPalette::Base, QColor(0x24, 0x24, 0x24));
            darkPalette.setColor(QPalette::AlternateBase, QColor(0x2b, 0x2b, 0x2b));
            darkPalette.setColor(QPalette::ToolTipBase, QColor(0x24, 0x24, 0x24));
            darkPalette.setColor(QPalette::ToolTipText, Qt::white);
            darkPalette.setColor(QPalette::Text, Qt::white);
            darkPalette.setColor(QPalette::Disabled, QPalette::Text, QColor(0x7f, 0x7f, 0x7f));
            darkPalette.setColor(QPalette::Dark, QColor(0x23, 0x23, 0x23));
            darkPalette.setColor(QPalette::Shadow, QColor(0x14, 0x14, 0x14));
            darkPalette.setColor(QPalette::Button, QColor(0x36, 0x36, 0x36));
            darkPalette.setColor(QPalette::ButtonText, Qt::white);
            darkPalette.setColor(QPalette::Disabled, QPalette::ButtonText, QColor(0x7f, 0x7f, 0x7f));
            darkPalette.setColor(QPalette::BrightText, Qt::red);
            darkPalette.setColor(QPalette::Link, QColor(0x2a, 0x82, 0xda));
            darkPalette.setColor(QPalette::LinkVisited, QColor(0x5a, 0x4a, 0xda));
            darkPalette.setColor(QPalette::Highlight, QColor(0x2a, 0x82, 0xda));
            darkPalette.setColor(QPalette::Disabled, QPalette::Highlight, QColor(0x50, 0x50, 0x50));
            darkPalette.setColor(QPalette::HighlightedText, Qt::white);
            darkPalette.setColor(QPalette::Disabled, QPalette::HighlightedText, QColor(0x7f, 0x7f, 0x7f));
            darkPalette.setColor(QPalette::PlaceholderText, QColor(0x8a, 0x8a, 0x8a));

            qApp->setPalette(darkPalette);
            qApp->setStyleSheet(
                "QToolTip { color: #ffffff; background-color: #242424; border: 1px solid #4a4a4a; padding: 2px; }\n"
                "QTableView QTableCornerButton::Section { background-color: #363636; }\n"
                "QHeaderView::section { background-color: #363636; color: #ffffff; padding: 2px; border: 1px solid #2b2b2b; }\n"
                "QMenu::separator { height: 1px; background: #4a4a4a; margin-left: 6px; margin-right: 6px; }\n"
                "QLineEdit[readOnly=\"true\"], QTextEdit[readOnly=\"true\"] { color: #a0a0a0; }\n");

            current_theme = theme;
        } else if (ok) {
            // System & Built-in
            QString qss;

            if (themeId != 0) {
                QString path;
                std::map<QString, QString> replace;
                switch (themeId) {
                    case 1:
                        path = ":/themes/feiyangqingyun/qss/flatgray.css";
                        replace[":/qss/"] = ":/themes/feiyangqingyun/qss/";
                        break;
                    case 2:
                        path = ":/themes/feiyangqingyun/qss/lightblue.css";
                        replace[":/qss/"] = ":/themes/feiyangqingyun/qss/";
                        break;
                    case 3:
                        path = ":/themes/feiyangqingyun/qss/blacksoft.css";
                        replace[":/qss/"] = ":/themes/feiyangqingyun/qss/";
                        break;
                    default:
                        return;
                }
                qss = ReadFileText(path);
                for (auto const &[a, b]: replace) {
                    qss = qss.replace(a, b);
                }
            }

            auto system_style = QStyleFactory::create(this->system_style_name);

            if (themeId == 0) {
                // system theme
                qApp->setPalette(system_style->standardPalette());
                qApp->setStyle(system_style);
                qApp->setStyleSheet("");
            } else {
                if (themeId == 1 || themeId == 2 || themeId == 3) {
                    // feiyangqingyun theme
                    QString paletteColor = qss.mid(20, 7);
                    qApp->setPalette(QPalette(paletteColor));
                } else {
                    // other theme
                    qApp->setPalette(system_style->standardPalette());
                }
                qApp->setStyleSheet(qss);
            }
        } else {
            // QStyleFactory
            const auto &_style = QStyleFactory::create(theme);
            if (_style != nullptr) {
                qApp->setPalette(_style->standardPalette());
                qApp->setStyle(_style);
                qApp->setStyleSheet("");
            }
        }

        current_theme = theme;
    };
    internal();

    auto nekoray_css = ReadFileText(":/neko/neko.css");
    qApp->setStyleSheet(qApp->styleSheet().append("\n").append(nekoray_css));
}
