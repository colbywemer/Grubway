#include "login.h"
#include "ui_login.h"

#include "register.h"

#include <AdminDashboard>
#include <CashierScreen>
#include <Database>

#include <QHBoxLayout>
#include <QMessageBox>
#include <QPixmap>

// Login Class constructor
Login::Login(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Login)
{
    // Set up the UI for the login screen
    ui->setupUi(this);
}

// Login Class destructor
Login::~Login()
{
    // Delete the UI object
    delete ui;
}

// Slot for handling LoginButton click event
void Login::on_LoginButton_clicked()
{
    // Get the user input for username and password
    QString username = ui ->usernameInput -> text(),
        password = ui ->passwordInput -> text();

    // Create a QMessageBox object for displaying warnings
    QMessageBox MsgBox;

    // Set a common title for warning messages
    QString title = "Login";

    // Check if the username field is empty
    if (username == "")
    {
        // Show a warning message and return
        MsgBox.warning(this, title, "Username field must not be empty!");
        MsgBox.show();

        return;
    }

    // Check if the password field is empty
    else if (password == "")
    {
        // Show a warning message and return
        MsgBox.warning(this, title, "Password field must not be empty!");
        MsgBox.show();

        return;
    }

    // Retrieve the account information based on the input username
    Account account = selectAccountByUsername(username);

    // Check if the input password matches the stored password
    if (password == account.password)
    {
        // If the user is a Manager or Administrator, show the AdminDashboard
        if (account.position == "Manager" || account.position == "Administrator")
        {
            AdminDashboard *ad = new AdminDashboard;
            ad->setUsrname(username);
            ad->show();

            // Hide the login screen
            this->hide();
        }
        // If the user is a Cashier, show the CashierScreen
        else if(account.position == "Cashier")
        {
            CashierScreen *cs = new CashierScreen();
            cs->setUsername(username);
            cs->show();

            // Hide the login screen
            this->hide();
        }
        // If the position is not recognized, show a warning message
        else{
            MsgBox.warning(this, title, "Username Or Password Incorrect, Unsuccessful Login");
            MsgBox.show();
        }
    }
    // If the password is incorrect, show a warning message
    else
    {
        MsgBox.warning(this, title, "Username Or Password Incorrect, Unsuccessful Login");
        MsgBox.show();
    }
}

// Slot for handling RegisterButton click event
void Login::on_RegisterButton_clicked()
{
    // Create a Register object and show the registration screen
    Register *r = new Register;
    r->show();

    // Hide the login screen
    this->hide();
}

