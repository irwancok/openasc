#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QtGui/QDialog>

#define VER_STR "Version 0.1b"

namespace Ui {
    class AboutDialog;
}

class AboutDialog : public QDialog {
    Q_OBJECT
    Q_DISABLE_COPY(AboutDialog)
public:
    explicit AboutDialog(QWidget *parent = 0);
    virtual ~AboutDialog();

protected:
    virtual void changeEvent(QEvent *e);

private:
    Ui::AboutDialog *m_ui;
private slots:
		void pushButtonClosePressed();
};

#endif // ABOUTDIALOG_H
