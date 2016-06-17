#include "jacktelop/keywindow.hpp"

int main(int argc, char **argv) {
    QApplication app(argc, argv);
    KeyInputWindow window;
    window.setWindowTitle(QString::fromUtf8("Keyboard Control"));
    window.resize(200, 200);
    window.show();
    return app.exec();
}
