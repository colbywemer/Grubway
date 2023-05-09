// Include necessary header files
#include <Database>
#include <LoginAndRegister>

#include <QApplication>
#include <QCoreApplication>
#include <QDialog>
#include <QDir>
#include <QLabel>
#include <QMenuBar>
#include <QMessageBox>
#include <QPixmap>
#include <QProcess>
#include <QVBoxLayout>

// Define application constants
#define VERSION "1.0.0.0"
#define QMAKE_TARGET_DESCRIPTION "Get your Grub on™"
#define QMAKE_TARGET_DEVELOPER "The Vanguard"
#define QMAKE_TARGET_PRODUCT  "Grubway"
#define QMAKE_TARGET_COPYRIGHT  "Copyright (C) 2023 Grubway"

using namespace std;

#define ERROR -1

// Define platform-specific constants for Python and pip
#if __APPLE__
QString pip = "/Library/Frameworks/Python.framework/Versions/Current/bin/pip3",
    python = "/Library/Frameworks/Python.framework/Versions/Current/bin/python3";
#endif

// Initialize a global QProcess object
QProcess *process = new QProcess;

// Function to kill the process when the application exits
void my_exit_function() {
    process->close();
}

// Function to show the About App info
void showAboutApp() {
    QDialog aboutDialog;
    aboutDialog.setWindowTitle("About Grubway");

    QVBoxLayout layout(&aboutDialog);

// Add app icon to the About App window
#if __APPLE__
    QLabel iconLabel;
    QDir appDir(QCoreApplication::applicationDirPath());
    QString resourcesPath = appDir.absoluteFilePath("../Resources/grubwayicon.png");
    QPixmap appIconPixmap(resourcesPath);
#else
    QLabel iconLabel;
    QPixmap appIconPixmap(":/icons/grubwayicon.png");
#endif
    iconLabel.setPixmap(appIconPixmap.scaled(256, 256, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    layout.addWidget(&iconLabel, 0, Qt::AlignCenter);

    // Set about app text
    QString aboutAppText = QCoreApplication::applicationName() + " - " + QMAKE_TARGET_DESCRIPTION + "\n\nVersion " +
        VERSION + "\n\nDeveloped by " + QMAKE_TARGET_DEVELOPER + "\n\n" + QMAKE_TARGET_COPYRIGHT;

    QLabel aboutText(aboutAppText);
    layout.addWidget(&aboutText);

    // Configure about dialog layout
    aboutDialog.setLayout(&layout);
    aboutDialog.setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    aboutDialog.setMinimumSize(aboutDialog.sizeHint());
    aboutDialog.setMaximumSize(aboutDialog.sizeHint());
    aboutDialog.setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
    aboutDialog.exec();
}

int main(int argc, char *argv[])
{
    // Initialize the QApplication object
    QApplication a(argc, argv);

    // Create the database tables if they do not exist
    createTables();

// Set Windows app icon
#if !__APPLE__
    QIcon appIcon(":/icons/grubwayicon.png");
    a.setWindowIcon(appIcon);
#endif

    // Create the menu bar and the About App menu item
    QMenuBar menuBar;
    QMenu *aboutMenu = menuBar.addMenu("About");
    QAction *aboutAppAction = new QAction("About Grubway", &menuBar);

    // Connect the About App action to the showAboutApp function
    QObject::connect(aboutAppAction, &QAction::triggered, showAboutApp);
    // Add the About App action to the Help menu
    aboutMenu->addAction(aboutAppAction);

    // Variables to check if the Flask server app
    bool has_flask = false;

    QProcess *pip_process = new QProcess;
    QString std_output;

    // Launch pip to check if Flask is installed
#if WIN32
    pip_process->start("pip.exe", QStringList() << "freeze");
#elif __APPLE__
    pip_process->start(pip, QStringList() << "freeze");
#else
    pip_process->start("pip", QStringList() << "freeze");
#endif

    // Wait for the pip process to finish and read the std output
    pip_process->waitForFinished();
    std_output = pip_process->readAllStandardOutput();
    pip_process->close();

    QMessageBox MsgBox;

    // Display an error message if the pip process fails to start
    if (pip_process->error() == QProcess::ProcessError(QProcess::FailedToStart))
    {
        MsgBox.critical(nullptr, "Execution Error", "Unable to launch 'pip', please make sure the program is installed and located in your system PATH.");
        MsgBox.show();

        return ERROR;
    }

    // Check if Flask is installed by searching for it in the standard output
    if (std_output.contains("Flask"))
    {
        has_flask = true;
    }

    // Display an error message if Flask is not installed
    if (!has_flask)
    {
        MsgBox.critical(nullptr, "Webserver Error", "Package 'flask' is not installed. Please make sure you install flask. You can install flask with 'pip install flask'.");
        MsgBox.show();

        return ERROR;
    }

    // Start the Flask server
#if WIN32
    process->start("python.exe", QStringList() << QDir::currentPath() + "\\web\\script.py");
#elif __APPLE__
    process->start(python, QStringList() << QDir::currentPath() + "/web/script.py");
#else
    process->start("python", QStringList() << QDir::currentPath() + "/web/script.py");
#endif
    // Display the Register or Login window based on whether any accounts exist
    if (!administratorExist())
    {
        Register *r = new Register;
        r->setFirstRun();
#if WIN32 || __APPLE__
        r->setMenuBar(&menuBar);
#endif
        r->show();
    }else{
        Login *l = new Login;
#if WIN32 || __APPLE__
        l->setMenuBar(&menuBar);
#endif
        l->show();
    }


    // Register the exit function to kill the process when the application exits
    atexit(my_exit_function);

    // Execute the QApplication event loop
    return a.exec();
}
