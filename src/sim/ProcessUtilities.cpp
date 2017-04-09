#include "ProcessUtilities.h"

#include <QDateTime>
#include <QStringList>

namespace mms {

bool ProcessUtilities::start(
    const QString& command,
    const QString& directory,
    QProcess* process
) {
    QStringList args = command.split(' ', QString::SkipEmptyParts);
    QString bin;
    if (!args.isEmpty()) {
        bin = args.at(0);
        args.removeFirst();
    }
    process->setWorkingDirectory(directory);
    process->start(bin, args);
    return process->waitForStarted();
}

void ProcessUtilities::build(
        const QString& name,
        const QStringList& names,
        const QString& buildCommand,
        const QString& dirPath,
        TextDisplayWidget* display,
        QPushButton* button,
        QObject* parent) {

	// Clear the build display
	if (display->autoClearCheckBox->isChecked()) {
		display->textEdit->clear();
	}

	// Perform some validation
    if (!names.contains(name)) {
		display->textEdit->appendPlainText("[CORRUPT ALGORITHM CONFIG]\n");
        return;
    }
	if (buildCommand.isEmpty()) {
		display->textEdit->appendPlainText("[EMPTY BUILD COMMAND]\n");
        return;
	}
	if (dirPath.isEmpty()) {
		display->textEdit->appendPlainText("[EMPTY DIRECTORY]\n");
        return;
	}

    // Instantiate a new process
    QProcess* process = new QProcess(parent);

    // Display all build output/errors
    QObject::connect(process, &QProcess::readyReadStandardOutput, parent, [=](){
        display->textEdit->appendPlainText(process->readAllStandardOutput());
    });
    QObject::connect(process, &QProcess::readyReadStandardError, parent, [=](){
        display->textEdit->appendPlainText(process->readAllStandardError());
    });

    // Re-enable build button when build finishes, clean up the process
    QObject::connect(
        process,
        static_cast<void(QProcess::*)(int, QProcess::ExitStatus)>(
            &QProcess::finished
        ),
        parent,
        [=](int exitCode, QProcess::ExitStatus exitStatus){
            button->setEnabled(true);
    		display->textEdit->appendPlainText(
				exitStatus == QProcess::NormalExit && exitCode == 0
				? "[BUILD COMPLETE]\n"
				: "[BUILD FAILED]\n"
			);
            delete process;
        }
    );

    // GUI upkeep before build starts
    button->setEnabled(false);
    display->textEdit->appendPlainText(
		QString("[BUILD INITIATED] - ") +
		QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss AP") +
		QString("\n")
	);

    // Start the build process
    bool success = start(buildCommand, dirPath, process);
    if (!success) {
        button->setEnabled(true);
    	display->textEdit->appendPlainText(
            QString("[BUILD FAILED TO START] - ") +
            process->errorString() +
            QString("\n")
        );
        delete process;
    }
}

} // namespace mms
