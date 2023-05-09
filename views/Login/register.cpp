// Include necessary header files
#include "register.h"
#include "ui_register.h"

#include "login.h"

#include <AdminDashboard>
#include <Database>

#include <QMessageBox>

// Define positions for Manager and Admin roles
int Manager_Pos = 1, Admin_Pos = 2;

// Register constructor
Register::Register(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Register)
{
    ui->setupUi(this);

    // Hide the user role selection comboBox if no administrator exists
    if(!administratorExist())
    {
        ui->comboBox->hide();
    }

    // Hide the registration code input by default
    ui->code->hide();
}

// Register destructor
Register::~Register()
{
    delete ui;
}

// Set the leader account
void Register::setAdminAccount(Account account)
{
    leaderAccount = account;
}

// Process registration when the register button is clicked
void Register::on_RegisterButton_clicked()
{
    // Create a new account object and populate its fields
    Account account;
    account.firstName = ui->firstNameInput->text();
    account.lastName = ui->lastNameInput->text();
    account.phoneNumber = ui->phoneNumberInput->text();
    account.email = ui->emailInput->text();
    account.address = ui->addressInput->text();
    account.username = ui->usernameInput->text();
    account.password = ui->passwordInput->text();
    account.position = ui->comboBox->currentText();

    QString code = ui->code->text(), title = "Register";

    QMessageBox MsgBox;

    // Check if all required fields are filled
    if (account.username == "" || account.firstName == "" || account.lastName == "" || account.phoneNumber == "" || account.email == "" || account.address == "" || account.password == "")
    {
        MsgBox.warning(this, title, "All fields must be filled!");
        MsgBox.show();
        return;
    }

    // Verify administrator registration code
    if (account.position == "Administrator" && goToLogin)
    {
        leaderAccount = accountCodeExists(code);

        if (leaderAccount.position != "Administrator")
        {
            MsgBox.warning(this, title, "Invalid Administrator Code!");
            MsgBox.show();
            return;
        }
    }

    // Verify manager registration code
    if (account.position == "Manager" && goToLogin)
    {
        leaderAccount = accountCodeExists(code);

        if (leaderAccount.position != "Administrator" && leaderAccount.position != "Manager")
        {
                MsgBox.warning(this, title, "Invalid Manager Code!");
                MsgBox.show();
                return;
        }
    }

    // Set the account position as Administrator if it's the first run
    if (firstRun)
    {
        account.position = "Administrator";
    }

    // Check if position field is filled
    else if (account.position == "")
    {
        MsgBox.warning(this, title, "All fields must be filled!");
        MsgBox.show();
        return;
    }

    // Check if an account with the same username already exists
    Account exist = selectAccountByUsername(account.username);

    if (exist.username == "")
    {
        createAccount(account);

        // Log the account creation if a leader account exists
        if (leaderAccount.username != "")
        {
            Logs log;
            log.account = leaderAccount;
            log.time = QDateTime::currentDateTime();
            log.action = "Created Account '" + account.username + "' With Position '" + account.position + "'";
            log.type = "Account";
            createLog(log);
        }
        // Show the login screen if goToLogin is true
        if (goToLogin)
        {
            Login *l = new Login();
            l->show();
        }
        // Show the parent widget (AdminDashboard) if it exists
        else if (parent_widget != nullptr)
        {
            parent_widget->refreshTablesData();
            parent_widget->show();
        }

        // Hide the current Register window
        this->hide();
    }
    // Show a warning message if the account already exists
    else
    {
        MsgBox.warning(this, title, "Account Already Exists!");
        MsgBox.show();
        return;
    }
}

// Set the goToLogin flag to false
void Register::noReturnToLogin()
{
    goToLogin = false;
}

// Set the parent_widget as the AdminDashboard instance
void Register::setAdminDash(AdminDashboard *ad)
{
    parent_widget = ad;
}

// Show or hide the registration code input based on the selected position in the comboBox
void Register::on_comboBox_currentIndexChanged(int index)
{
    ui->comboBox->setStyleSheet("color: black;");

    if ((index == Manager_Pos || index == Admin_Pos) && goToLogin)
    {
        ui->code->show();
    }
    else
    {
        ui->code->hide();
    }
}

// Set the firstRun flag to true
void Register::setFirstRun()
{
    firstRun = true;
}

// Handle the close event for the Register window
void Register::closeEvent(QCloseEvent *event)
{
    // Show the Login window if goToLogin is true and firstRun is false
    if (goToLogin && !firstRun)
    {
        Login *l = new Login;
        l->show();
    }
    // Show the parent_widget (AdminDashboard) if it exists
    else if (parent_widget != nullptr)
    {
        parent_widget->show();
    }
    // Quit the application if no other windows should be displayed
    else
    {
        qApp->quit();
    }

    // Accept the close event
    event->accept();
}
