#include "MainWindow.hpp"
#include "Config.h"
#include <QThread>
#include <QMovie>
#include <QInputDialog>
#include <QFileDialog>
#include <QActionGroup>

/// These file contains Member definitions of the MainWindow class
/// Related to handle settings

void MainWindow::_updateSelectedVersion()
{
    QAction *checked = _versionGroup->checkedAction();
    _settings.setValue("General/GameVersion",checked->text());

    QFont font;
    for (auto verAction : _versionGroup->actions())
        verAction->setFont(font);

    font.setBold(true);
    checked->setFont(font);
    this->_settings.sync();
}

void MainWindow::_updateSelectedLogLevel()
{
    QAction *checked = _logGroup->checkedAction();
    _settings.setValue("Debug/LogLevel", checked->text());
    GLOBAL_LOG_LEVEL = checked->text();

    QFont font;
    for (auto act : _logGroup->actions())
        act->setFont(font);

    font.setBold(true);
    checked->setFont(font);
    this->_settings.sync();
}

void MainWindow::_toggleNoBackup()
{
    _settings.setValue("General/NoBackupOk", !_settings.value("General/NoBackupOk",false).toBool());
    _settings.sync();
}

void MainWindow::_getCustomSteamPath()
{
    QString dir = QFileDialog::getExistingDirectory(this, "Select the folder", "C:\\", QFileDialog::ShowDirsOnly);
    if (dir.isEmpty())
        return;
    _settings.setValue("SteamPath/CustomPath", dir);
    _settings.setValue("SteamPath/Auto", false);
    ui->actionAutoSteam->setChecked(false);
    this->_MHManager.setSteamDirectory(dir);
    ui->actionSteam_Current->setText(dir.prepend("Current : "));
    this->_settings.sync();
}

void MainWindow::_setAutoSteam()
{
    if (_settings.value("SteamPath/Auto", true).toBool())
    {
        ui->actionAutoSteam->setChecked(true);
        return;
    }

    _settings.setValue("SteamPath/Auto", true);
    this->_settings.sync();
    this->_MHManager.unSetSteamDirectory();
    if (!this->_MHManager.processIsOpen())
    {
        auto Dia = new DialogWindow(this, "Automatic Steam Path", "Steam path has been set to automatic, but MHW seems to be closed. Open it and press Search MHW Character Data.", Status::WARNING);
        Dia->show();
        return;
    }
    this->_MHManager.findSteamPath();
}

void MainWindow::_toggleAutoUpdates()
{
    _settings.setValue("SteamPath/Auto", !_settings.value("SteamPath/Auto", true).toBool());
    this->_settings.sync();
}