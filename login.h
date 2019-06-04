#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QPushButton>
#include <QMessageBox>

namespace Ui {
class Login;
}

class Login : public QDialog
{
    Q_OBJECT

public:
    explicit Login(QWidget* parent = nullptr);
    bool logedIn() const;
    ~Login();

public slots:
    void authentication();

private:
    Ui::Login* ui;
    QLabel title_;
    QLabel username_;
    QLineEdit usernameInput_;
    QLabel password_;
    QLineEdit passwordInput_;
    QPushButton login_;
    QLabel pictureLogin_;
    QPushButton loginButton_;
    bool logedIn_;
    void design();
};

#endif // LOGIN_H
