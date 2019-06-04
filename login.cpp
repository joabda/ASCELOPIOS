#include "login.h"
#include "ui_login.h"

Login::Login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
    logedIn_ = false;
    design();
}

void Login::design()
{
    QVBoxLayout* mainLayout = new QVBoxLayout;
    setLayout(mainLayout);
    setFixedSize(320,400);

    setWindowTitle("ASCELOPIOS LOGIN");
    QPixmap loginPic(":/Pictures/Resources/login.jpeg");
    int pictureLoginWidth = size().width();
    int pictureLoginHeight = (size().height())/4;
    pictureLogin_.setPixmap(loginPic.scaled(pictureLoginWidth,pictureLoginHeight,Qt::KeepAspectRatio));

    username_.setText("Username: ");
    password_.setText("Password: ");
    loginButton_.setText("Login");

    mainLayout->addWidget(&pictureLogin_);
    mainLayout->addWidget(&title_);
    mainLayout->addWidget(&username_);
    mainLayout->addWidget(&usernameInput_);
    mainLayout->addWidget(&password_);
    mainLayout->addWidget(&passwordInput_);
    mainLayout->addWidget(&loginButton_);
    passwordInput_.setEchoMode(QLineEdit::Password);

    // REMOVE AFTER FINISHING
    usernameInput_.setText("Admin");
    passwordInput_.setText("MyPassword");

    QObject::connect(&loginButton_, SIGNAL(clicked()), this, SLOT(authentication()));
    QObject::connect(&loginButton_, SIGNAL(pressed()), this, SLOT(authentication()));
}

void Login::authentication()
{
    if(usernameInput_.text() == "Admin" && passwordInput_.text() == "MyPassword")
    {
        close();
        logedIn_ = true;
    }
    else
    {
        QMessageBox mbox;
        mbox.setText("Invalid Username or Password\n");
        mbox.exec();
    }
}

bool Login::logedIn() const
{
    return logedIn_;
}

Login::~Login()
{
    delete ui;
}
